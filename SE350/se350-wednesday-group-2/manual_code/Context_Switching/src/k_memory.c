/*
 ****************************************************************************
 *
 *				  UNIVERSITY OF WATERLOO SE 350 RTX LAB
 *
 *					 Copyright 2020-2022 Yiqing Huang
 *						  All rights reserved.
 *---------------------------------------------------------------------------
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *  - Redistributions of source code must retain the above copyright
 *	notice and the following disclaimer.
 *
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *  ARE DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDERS AND CONTRIBUTORS BE
 *  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 *---------------------------------------------------------------------------*/


/**************************************************************************//**
 * @file		k_memory.c
 * @brief	   kernel memory managment routines starter source code
 *
 * @version	 V1.2021.01
 * @authors	 Yiqing Huang
 * @date		2022 JAN
 * @attention   only two processes are initialized, not all six user test procs
 *****************************************************************************/

#include "k_rtx.h"
#include "k_process.h"

#include "queue.h"

// mem_blk can be cast to a node
// C will always allocate memory in order of declaration
typedef struct mem_blk {
	node base;
	// 128 bytes
	// a mem_blk is created every 128 bytes
	// so sizeof does not work on mem_blk, use MEM_BLK_SIZE
} mem_blk;

/*
 *==========================================================================
 *							GLOBAL VARIABLES
 *==========================================================================
 */

U32 *gp_stack; /* The last allocated stack low address. 8 bytes aligned */
			   /* The first stack starts at the RAM high address		*/
			   /* stack grows down. Fully decremental stack			 */

queue *mem_block_queue;
mem_blk *heap_start;


/* A MEM_NUM_BLKS-length word array, each element is a pointer to the PCB that owns the block at the same index in the heap. */
PCB **heap_processes;

/**************************************************************************//**
 * @brief: Initialize RAM of the SIM Target as follows:

0x10008000  +---------------------------+ High Address <--- gp_stack/RAM_END_ADDR
			|	Proc 1 STACK			|
			|---------------------------|
			|	Proc 2 STACK			|
			|---------------------------|
			|		   ...				|
			|---------------------------|
			|   Blocked Queue (Pri=4)   |
			|---------------------------|
			|   Blocked Queue (Pri=3)   |
			|---------------------------|
			|   Blocked Queue (Pri=2)   |
			|---------------------------|
			|   Blocked Queue (Pri=1)   |
			|---------------------------|
			|   Blocked Queue (Pri=0)   |<--- blocked_queues
			|---------------------------|
			|	Ready Queue (Pri=4)		|
			|---------------------------|
			|	Ready Queue (Pri=3)		|
			|---------------------------|
			|	Ready Queue (Pri=2)		|
			|---------------------------|
			|	Ready Queue (Pri=1)		|
			|---------------------------|
			|	Ready Queue (Pri=0)		|<--- ready_queues
			|---------------------------|
			| PCB ptr BLK: MEM_NUM_BLKS |
			|---------------------------|
			|			...				|
			|---------------------------|
			|		PCB ptr BLK: 1		|
			|---------------------------|
			|		PCB ptr BLK: 0	   	|
			|---------------------------|<--- heap_processes
			|		PCB (Null Proc)		|
			|---------------------------|<--- gp_pcbs[N] (Null Process)
			|		PCB N				|
			|---------------------------|<--- gp_pcbs[N-1]
			|		...					|
			|---------------------------|
			|		PCB 2				|
			|---------------------------|<--- gp_pcbs[1]
			|		PCB 1				|
			|---------------------------|<--- gp_pcbs[0]
			| PCB pointer for Null Proc |
			|---------------------------|
			|		PCB pointers		|
			|---------------------------|<--- gp_pcbs
			|		Padding				|
			|---------------------------|
			|Image$$RW_IRAM1$$ZI$$Limit |<--- kernel stack
			|...........................|
			|	   RTX  Image			|
			|							|
0x10000000	+---------------------------+ Low Address


0x2007FFFF  +---------------------------+ High Address
			|			HEAP			|
			|							|
0x2007C000  +---------------------------+ Low Address <--- AHB_SRAM_ADDR

*/
/**************************************************************************//**
 * @brief   	initialize the memory
 * @details	 	allocating memory for PCBs
 *			  	initial stack base pointer for processes
 *****************************************************************************/

// Get the index of the mem block
// 0 is the lowest address block, MEM_NUM_BLOCKS - 1 is highest address block
int get_block_index(U32 *result, void *blk_addr) {
	// Gheck if block to find index of is outside the range of the heap
	// (we are assuming the heap is a contiguous chunk of memory, if the heap is spread over
	// more than one contiguous sequence of memory we will have to check multiple intervals)
	if ((U32) blk_addr < (U32) heap_start || (U32) blk_addr >= (U32) heap_start + MEM_NUM_BLKS * MEM_BLK_SIZE) {
		debug(("get_block_index: ERROR: blk_addr is outside the range of the heap\r\n"));

		return RTX_ERR;
	}

	// Address relative to start of heap
	U32 blk_addr_in_heap = ((U32) blk_addr - (U32) heap_start);

	// Check if mem_blk is actually pointing to the start of a block
	if (blk_addr_in_heap % MEM_BLK_SIZE != 0) {
		debug(("get_block_index: ERROR: blk_addr is not the start of a block\r\n"));

		return RTX_ERR;
	}

	*result = blk_addr_in_heap / MEM_BLK_SIZE;
	return RTX_OK;
}

int reassign_memory_ownership(void *blk_addr, PCB* old_pcb, PCB* new_pcb) {
	// Get the index of the block in the heap
	U32 block_index;
	int rc = get_block_index(&block_index, blk_addr);

	// If the block is not in the heap, return an error
	if (rc != RTX_OK) {
		debug(("k_reassign_memory_ownership: ERROR: get_block_index() failed\r\n", block_index));
		return RTX_ERR;
	}

	// If the block is not owned by the old, return an error
	if (heap_processes[block_index] != old_pcb) {
		debug(("k_reassign_memory_ownership: ERROR: Attempted to free %x, but process %d does not own this block\r\n", block_index, gp_current_process->m_pid));
		return RTX_ERR;
	}

	// Reassign
	heap_processes[block_index] = new_pcb;
	return RTX_OK;
}

void memory_init(void) 
{
	U8 *p_end = (U8 *)&Image$$RW_IRAM1$$ZI$$Limit;

	debug(("memory_init: kernel space end: %x\r\n", p_end));

	/* 4 bytes padding */
	p_end += 4;

	/* allocate memory for pcb pointers  */
	/* one for each test process and one at the end for the NULL process */
	gp_pcbs = (PCB **)p_end;
	p_end += (NUM_TEST_PROCS + 1) * sizeof(PCB *);

	/* allocate memory for PCBs */
	/* one for each test process and one at the end for the NULL process */
	for (int i = 0; i < (NUM_TEST_PROCS + 1); i++) {
		gp_pcbs[i] = (PCB *)p_end;
		p_end += sizeof(PCB);
	}

	debug(("memory_init: gp_pcbs[0] = 0x%x\r\n", gp_pcbs[0]));
	debug(("memory_init: gp_pcbs[1] = 0x%x\r\n", gp_pcbs[1]));
	debug(("memory_init: gp_pcbs[2] = 0x%x\r\n", gp_pcbs[2]));
	debug(("memory_init: gp_pcbs[3] = 0x%x\r\n", gp_pcbs[3]));
	debug(("memory_init: gp_pcbs[4] = 0x%x\r\n", gp_pcbs[4]));
	debug(("memory_init: gp_pcbs[5] = 0x%x\r\n", gp_pcbs[5]));
	debug(("memory_init: gp_pcbs[6] (NULL Process) = 0x%x\r\n", gp_pcbs[6]));

	/* Allocate memory for heap memblk to PCB map */
	heap_processes = (PCB **) p_end;
	for (int i = 0; i < MEM_NUM_BLKS; ++i) {
		heap_processes[i] = NULL;
		p_end += sizeof(PCB *);
	}

	/* allocate memory for ready process queue */
	ready_queues = (queue*) p_end;

	debug(("memory_init: ready_queues = 0x%x\r\n", ready_queues));

	for (int i = 0; i < NUM_PRIORITIES; i++) {
		ready_queues[i].head = NULL;
		ready_queues[i].tail = NULL;
		p_end += sizeof(queue);
	}

	/* allocate memory for blocked process queue */
	blocked_queues = (queue*) p_end;

	debug(("memory_init: blocked_queues = 0x%x\r\n", blocked_queues));

	/* null priority never blocked */
	for (int i = 0; i < NUM_PRIORITIES - 1; i++) {
		blocked_queues[i].head = NULL;
		blocked_queues[i].tail = NULL;
		p_end += sizeof(queue);
	}

	/* prepare for alloc_stack() to allocate memory for stacks */
	gp_stack = (U32 *)RAM_END_ADDR;
	if ((U32)gp_stack & 0x04) { /* 8 bytes alignment */
		--gp_stack;
	}

	/* allocate memory for heap*/
	p_end = (U8 *) AHB_SRAM_ADDR;

	mem_block_queue = (queue *) p_end;
	mem_block_queue->head = NULL;
	mem_block_queue->tail = NULL;

	p_end += sizeof(queue);
	heap_start = (mem_blk *) p_end;

	debug(("memory_init: heap_start = 0x%x\r\n", heap_start));

	for (int i = 0; i < MEM_NUM_BLKS; ++i) {
		// initialize the block
		mem_blk* block = (mem_blk *) p_end;
		int rc = enqueue(mem_block_queue, (node* ) block);
		if (rc != RTX_OK) {
			debug(("memory_init: FATAL ERROR: Enqueueing memory blocks onto queue after initialization failed\r\n"));
		}
		U32 block_index;
		get_block_index(&block_index, (void *) block);

		p_end += MEM_BLK_SIZE;
	}
}

/**************************************************************************//**
 * @brief   	allocate stack for a process, align to 8 bytes boundary
 * @param	   	size stack size in bytes
 * @return	  	the base of the stack (i.e. high address)
 * @post		gp_stack is updated.
 *****************************************************************************/

U32 *alloc_stack(U32 size_b)
{
	U32 *sp;
	sp = gp_stack; /* gp_stack is always 8 bytes aligned */

	/* update gp_stack */
	gp_stack = (U32 *)((U8 *)sp - size_b);

	/* 8 bytes alignement adjustment to exception stack frame */
	if ((U32)gp_stack & 0x04) {
		--gp_stack;
	}
	return sp;
}

void *k_request_memory_block_nb(void) {
	node *result;

	if (dequeue(&result, mem_block_queue) != RTX_OK) {
		debug(("k_request_memory_block_nb: FATAL ERROR: dequeue failed\r\n"));
	}

	if (result == NULL) {
		debug(("k_request_memory_block_nb: No memory blocks available\r\n"));
		
		set_unprivileged();
		return result;
	}

	U32 block_index;
	int rc = get_block_index(&block_index, result);

	if (rc != RTX_OK) {
		debug(("k_request_memory_block_nb: FATAL ERROR: get_block_index failed\r\n"));
	}

	// Set the process that owns this block
	heap_processes[block_index] = gp_current_process;
	
	set_unprivileged();
	return result;
}

void *k_request_memory_block(void) {

	void *ptr = k_request_memory_block_nb();

	// Memory available
	if (ptr != NULL) {
		set_unprivileged();
		return ptr;
	}

	// Blocked on memory
	PCB* curr = NULL;
	gp_current_process->m_state = BLK_MEM;

	int curr_priority = k_get_process_priority(gp_current_process->m_pid);

	if (curr_priority == RTX_ERR) {
		debug(("k_request_memory_block: FATAL ERROR: k_get_process_priority() failed\r\n"));
	}

	int rc = move_elt_if((node **) &curr, &ready_queues[curr_priority], &blocked_queues[curr_priority], pcb_node_is_pid, &gp_current_process->m_pid);

	if (rc != RTX_OK || curr == NULL) {
		debug(("k_request_memory_block: FATAL ERROR: k_request_memory_block: Could not move node from ready to blocked\r\n"));
	}

	if (curr == NULL) {
		debug(("k_request_memory_block: FATAL ERROR: move_elt_if returning NULL\r\n"));
	}

	rc = k_release_processor();

	if (rc != RTX_OK) {
		debug(("k_request_memory_block: FATAL ERROR: k_release_processor() failed\r\n"));
	}

	// Once a process calls release_memory_block(),
	// release_memory_block will set the next_mem_blk field of this process
	// to the ptr to the memory block it freed
	if (gp_current_process->next_mem_blk != NULL) {
		ptr = gp_current_process->next_mem_blk;
		gp_current_process->next_mem_blk = NULL;
	}

	if (ptr == NULL) {
		debug(("k_request_memory_block: FATAL ERROR: k_release_processor did not pass memory to blocked process\r\n"));
	}
	
	set_unprivileged();
	return ptr;
}

int k_release_memory_block(void *p_mem_blk) {

	debug(("k_release_memory_block: Releasing block @ 0x%x\r\n", p_mem_blk));

	// Get the index of the block in the heap
	U32 block_index;
	int rc = get_block_index(&block_index, p_mem_blk);

	// If the block is not in the heap, return an error
	if (rc != RTX_OK) {
		debug(("k_release_memory_block: ERROR: get_block_index() failed\r\n", block_index));
		
		set_unprivileged();
		return RTX_ERR;
	}

	// If the block is not owned by the current process, return an error
	if (heap_processes[block_index] != gp_current_process) {
		debug(("k_release_memory_block: ERROR: Attempted to free %x, but process %d does not own this block\r\n", block_index, gp_current_process->m_pid));
		
		set_unprivileged();
		return RTX_ERR;
	}

	// Try to find a blocked process to give the memory block to
	PCB *unblocked_process = NULL;
	int unblocked_priority;
	for (int i = HIGH; i < PRI_NULL; i++) {
		if (!empty(&blocked_queues[i])) {

			rc = dequeue((node**) &unblocked_process, &blocked_queues[i]);
			unblocked_priority = i;

			if (rc != RTX_OK) {
				debug(("k_release_memory_block: FATAL ERROR: dequeue process failed\r\n"));
				
				set_unprivileged();
				return RTX_ERR;
			}

			break;
		}
	}

	// No blocked process to give memory to
	if (unblocked_process == NULL) {
		rc = enqueue(mem_block_queue, (node *) p_mem_blk);

		// If returning the memory block the heap failed, return an error
		if (rc != RTX_OK) {
			debug(("k_release_memory_block: FATAL ERROR: enqueue() failed\r\n"));
			
			set_unprivileged();
			return RTX_ERR;
		}

		// Set the process that owns this block to NULL
		heap_processes[block_index] = NULL;
	}
	// Give memory to blocked process
	else {
		// Reassign block to new process
		heap_processes[block_index] = unblocked_process;

		// Unblock the process
		unblocked_process->m_state = RDY;

		// Set the process's memory block to the one we just freed
		unblocked_process->next_mem_blk = p_mem_blk;

		// Add the process to the appropriate priority level of the ready queue
		enqueue(&ready_queues[unblocked_priority], (void*) unblocked_process);

		int curr_priority = k_get_process_priority(gp_current_process->m_pid);

		if (curr_priority == RTX_ERR) {
			debug(("k_release_memory_block: FATAL ERROR: k_get_process_priority() failed\r\n"));
			
			set_unprivileged();
			return RTX_ERR;
		}

		if (unblocked_priority < curr_priority) {
			// unblocked process should be at top of ready queue
			k_release_processor();
		}

	}
	
	set_unprivileged();
	return RTX_OK;
}
/*
 *===========================================================================
 *							 END OF FILE
 *===========================================================================
 */
