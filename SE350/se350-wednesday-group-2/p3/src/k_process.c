/*
 ****************************************************************************
 *
 *                  UNIVERSITY OF WATERLOO SE 350 RTX LAB
 *
 *           Copyright 2020-2022 Yiqing Huang and Thomas Reidemeister
 *                          All rights reserved.
 *---------------------------------------------------------------------------
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *  - Redistributions of source code must retain the above copyright
 *    notice and the following disclaimer.
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
 * @file        k_process.c
 * @brief       kernel process management C file
 *
 * @version     V1.2022.01
 * @authors     Yiqing Huang, Thomas Reidemeister
 * @date        2022 JAN
 * @note        The example code shows one way of implementing context switching.
 *              The code only has minimal sanity check.
 *              There is no stack overflow check.
 *              The implementation assumes only two simple user processes and
 *              NO HARDWARE INTERRUPTS.
 *              The purpose is to show one way of doing context switch
 *              under stated assumptions.
 *              These assumptions are not true in the required RTX Project!!!
 *              If you decide to use this piece of code,
 *              you need to understand the assumptions and the limitations.
 *
 *****************************************************************************/

#include "k_process.h"
#include "k_rtx.h"

#include "sysproc_a.h"
#include "sysproc_b.h"
#include "sysproc_c.h"
#include "sysproc_clock.h"
#include "sysproc_crt.h"
#include "sysproc_kcd.h"
#include "sysproc_set_prio.h"

#include "queue.h"
#include "common.h"
/*
 *==========================================================================
 *                            GLOBAL VARIABLES
 *==========================================================================
 */

PCB **gp_pcbs;                  /* array of pcbs */
PCB *gp_current_process = NULL; /* always point to the current RUN process */
PCB *gp_pcb_timer_iproc = NULL; /* always point to the Timer I-Process PCB */
PCB *gp_pcb_uart_iproc 	= NULL; /* always point to the UART I-Process PCB */
PCB *gp_pcb_crt_iproc 	= NULL; /* always point to the CRT Process PCB */
PCB *gp_pcb_kcd_iproc 	= NULL; /* always point to the KCD Process PCB */
PCB *gp_pcb_interrupted = NULL; /* always point to the interrupted process PCB */

/* process initialization table */
PROC_INIT g_proc_table[NUM_TOTAL_USERSPACE_PROCS];

/* priority queue for ready processes */
queue *ready_queues; // array of queues initialized in k_memory

/* priority queue for memory blocked processes */
queue *blocked_queues; // array of queues initialized in k_memory

/**************************************************************************//**
 * @biref initialize all processes in the system
 * @note  We assume there are only two user processes in the system in
 *        this example starter code.
 *        The preprocessor ECE350_DEMO should be defined
 *****************************************************************************/
void process_init(PROC_INIT *test_proc_info, int num)
{
	U32 *sp;

	g_proc_table[0].m_pid        = PID_NULL;
	g_proc_table[0].m_priority   = PRI_NULL;
	g_proc_table[0].m_stack_size = USR_SZ_STACK;
	g_proc_table[0].mpf_start_pc = &null_process;

	/* fill out the initialization table */
	for (int i = 1; i < num + 1; i++ ) {
		g_proc_table[i].m_pid        = test_proc_info[i-1].m_pid;
		g_proc_table[i].m_priority   = test_proc_info[i-1].m_priority;
		g_proc_table[i].m_stack_size = test_proc_info[i-1].m_stack_size;
		g_proc_table[i].mpf_start_pc = test_proc_info[i-1].mpf_start_pc;

		// check user process priority constraints
		if (g_proc_table[i].m_priority < HIGH || g_proc_table[i].m_priority >= PRI_NULL) {
			debug(("process_init: FATAL ERROR: USER PROCESS INVALID priority\r\n"));
		}
	}

	for (int i = num+1; i < num + NUM_SYS_PROCS; i++) {
		g_proc_table[i].m_pid = i;
		// g_proc_table[i].m_priority = HIGH;
		// USE ABOVE, BELOW IS TEMPORARY UNTIL ALL PROC IMPLEMENTED
		g_proc_table[i].m_priority = LOWEST;
		g_proc_table[i].m_stack_size = USR_SZ_STACK;

		// check system process priority restrictions
		if (g_proc_table[i].m_priority < HIGH || g_proc_table[i].m_priority > PRI_NULL) {
			debug(("process_init: FATAL ERROR: invalid priority\r\n"));
		}
	}

	g_proc_table[7].mpf_start_pc = &a_proc;
	g_proc_table[8].mpf_start_pc = &b_proc;
	g_proc_table[9].mpf_start_pc = &c_proc;
	g_proc_table[10].mpf_start_pc = &set_prio_proc;
	g_proc_table[11].mpf_start_pc = &clock_proc;
	g_proc_table[12].mpf_start_pc = &kcd_proc;
	g_proc_table[13].mpf_start_pc = &crt_proc;

	// UNCOMMENT AS YOU IMPLEMENT (DO THIS IN LOOP WHEN ALL DONE)
	// System processes interfere with p1/p2 tests, set priority to null when testing
	g_proc_table[7].m_priority = LOWEST;
	g_proc_table[8].m_priority = LOWEST;
	g_proc_table[9].m_priority = LOWEST;
	g_proc_table[10].m_priority = HIGH;
	g_proc_table[11].m_priority = HIGH;
	g_proc_table[12].m_priority = HIGH;
	g_proc_table[13].m_priority = HIGH;

	/* initialize exception stack frame (i.e. initial context) for each process */
	/* add pcb addresses to ready queues */

	for (int i = 0; i < num + NUM_SYS_PROCS; i++) {
		(gp_pcbs[i])->m_pid = (g_proc_table[i]).m_pid;
		(gp_pcbs[i])->m_priority = (g_proc_table[i]).m_priority;
		(gp_pcbs[i])->m_state = NEW;

		int priority = (g_proc_table[i]).m_priority;

		int rc = enqueue(&ready_queues[priority], (node*) gp_pcbs[i]);
		if (rc != RTX_OK) {
			debug(("process_init: FATAL ERROR: enqueue_process() failed\r\n"));
		}

		sp = alloc_stack((g_proc_table[i]).m_stack_size);
		*(--sp)  = INITIAL_xPSR; // user process initial xPSR
		*(--sp)  = (U32)((g_proc_table[i]).mpf_start_pc); // PC contains the entry point of the process
		for (int j = 0; j < 6; j++ ) { // R0-R3, R12 are cleared with 0
			*(--sp) = 0x0;
		}

		(gp_pcbs[i])->mp_sp = sp;
		(gp_pcbs[i])->next_mem_blk = NULL;
		(gp_pcbs[i])->m_message_queue.head = NULL;
		(gp_pcbs[i])->m_message_queue.tail = NULL;
	}

	/* Initialize the Timer I-Process */
	gp_pcb_timer_iproc = gp_pcbs[14];
	gp_pcb_timer_iproc->m_pid = PID_TIMER_IPROC;
	gp_pcb_timer_iproc->m_priority = HIGH; // Required to make sure sending messages doesn't pre-empt inside Timer
	gp_pcb_timer_iproc->m_state = IPROC;
	gp_pcb_timer_iproc->next_mem_blk = NULL;
	gp_pcb_timer_iproc->m_message_queue.head = NULL;
	gp_pcb_timer_iproc->m_message_queue.tail = NULL;
	gp_pcb_timer_iproc->mp_sp = alloc_stack(STACK_SIZE_IPROC);
	/* NOTE we do not need to create exception stack frame for an IPROC
		since they are running in handler mode and never get into the handler
		mode from the thread mode and they never exit from the handler mode
		back to thread mode either
	*/

	/* Initialize the UART I-Process */
	gp_pcb_uart_iproc = gp_pcbs[15];
	gp_pcb_uart_iproc->m_pid = PID_UART_IPROC;
	gp_pcb_uart_iproc->m_priority = HIGH;
	gp_pcb_uart_iproc->m_state = IPROC;
	gp_pcb_uart_iproc->next_mem_blk = NULL;
	gp_pcb_uart_iproc->m_message_queue.head = NULL;
	gp_pcb_uart_iproc->m_message_queue.tail = NULL;
	gp_pcb_uart_iproc->mp_sp = alloc_stack(STACK_SIZE_IPROC);
	/* NOTE we do not need to create exception stack frame for an IPROC
		since they are running in handler mode and never get into the handler
		mode from the thread mode and they never exit from the handler mode
		back to thread mode either
	*/
}

/**************************************************************************//**
 * @brief   scheduler, pick the pid of the next to run process
 * @return  PCB pointer of the next to run process
 *          NULL if error happens
 * @post    if gp_current_process was NULL, then it gets set to pcbs[0].
 *          No other effect on other global variables.
 *****************************************************************************/

PCB *scheduler(void)
{
	node *next_process = NULL;
	for (int i = 0 ; i < 5; i++) {
		// find highest priority non-empty queue
		if (!empty(&ready_queues[i])) {
			// fetch pointer to node into next_process
			int rc = dequeue(&next_process, &ready_queues[i]);

			if (rc != RTX_OK) {
				debug(("scheduler: FATAL ERROR: dequeue process failed\r\n"));
			}

			if (((PCB*) next_process)->m_state != BLK_MEM && ((PCB*) next_process)->m_state != BLK_MSG) {
				// put the process back into the queue if it is not blocked
				rc = enqueue(&ready_queues[i], next_process);

				if (rc != RTX_OK) {
					debug(("scheduler: FATAL ERROR: enqueue process failed\r\n"));
				}
			} else {
				debug(("scheduler: FATAL ERROR: process with state BLK_MEM or BLK_MSG is in ready queue\r\n"));
			}

			break;
		}
	}

	return (PCB*) next_process;
}

PCB* try_scheduler(void)
{
	node *next_process = NULL;

	for (int i = 0; i < 5; i++) {
		if (!empty(&ready_queues[i])) {
			int rc = try_dequeue(&next_process, &ready_queues[i]);

			if (rc != RTX_OK) {
				debug(("try_scheduler: FATAL ERROR: dequeue process failed\r\n"));
			}

			if (((PCB*) next_process)->m_state == BLK_MEM || ((PCB*) next_process)->m_state == BLK_MEM) {
				debug(("try_scheduler: FATAL ERROR: process with state BLK_MEM or BLK_MSG is in ready queue\r\n"));
			}

			break;
		}
	}

	return (PCB*) next_process;
}


/**************************************************************************//**
 * @brief   switch out old pcb (p_pcb_old), run the new pcb (gp_current_process)
 * @param   p_pcb_old, the old pcb that was in RUN
 * @return  RTX_OK upon success
 *          RTX_ERR upon failure
 * @pre     p_pcb_old and gp_current_process are pointing to valid PCBs.
 * @post    if gp_current_process was NULL, then it gets set to pcbs[0].
 * 			if the previous state was RUN, the new state is RDY. Otherwise it
 * 			does not change anything.
 *          No other effect on other global variables.
 *****************************************************************************/

int process_switch(PCB *p_pcb_old)
{
	PROC_STATE_E state;

	state = gp_current_process->m_state;

	if (state == NEW) {
		if (gp_current_process != p_pcb_old && p_pcb_old->m_state != NEW) {
			if (p_pcb_old->m_state == RUN){
				p_pcb_old->m_state = RDY;
			}
			p_pcb_old->mp_sp = (U32 *) __get_MSP();
		}
		gp_current_process->m_state = RUN;

		__set_MSP((U32) gp_current_process->mp_sp);
		__rte();  // pop exception stack frame from the stack for a new process
	}

	/* The following will only execute if the if block above is FALSE */

	if (gp_current_process != p_pcb_old) {
		if (state == RDY){
			if (p_pcb_old->m_state == RUN){
				p_pcb_old->m_state = RDY;
			}
			p_pcb_old->mp_sp = (U32 *) __get_MSP(); // save the old process's sp
			gp_current_process->m_state = RUN;
			__set_MSP((U32) gp_current_process->mp_sp); //switch to the new proc's stack
		} else {
			gp_current_process = p_pcb_old; // revert back to the old proc on error

			debug(("process_switch: FATAL ERROR: Trying to switch to a process that is not in a RDY state\r\n"));

			return RTX_ERR;
		}
	}
	return RTX_OK;
}

/**************************************************************************//**
 * @brief   release_processor().
 * @return  RTX_ERR on error and zero on success
 * @post    gp_current_process gets updated to next to run process
 *****************************************************************************/

int k_release_processor(void)
{
	// Ensure that the currently running process is placed to the back of its ready queue
	// (if it has just been placed in a blocked queue do nothing)
	if (gp_current_process != NULL &&
		gp_current_process->m_pid != PID_NULL &&
		gp_current_process->m_state != BLK_MEM &&
		gp_current_process->m_state != BLK_MSG)
	{
		int cur_priority = k_get_process_priority(gp_current_process->m_pid);

		if (cur_priority == RTX_ERR) {
			debug(("k_release_processor: FATAL ERROR: could not find priority of pid in queue sanitization"));
		}

		node *moved = NULL;
		int rc = move_elt_if(&moved, &ready_queues[cur_priority], &ready_queues[cur_priority], pcb_node_is_pid, &gp_current_process->m_pid);
		if (rc != RTX_OK) {
			debug(("k_release_processor: FATAL ERROR: could not moved running process to back of own queue"));
		}
		if (moved == NULL) {
			debug(("k_release_processor didn't move process to back - it is probably in blocked queue"));
		}
	}

	PCB *p_pcb_old = NULL;

	p_pcb_old = gp_current_process;
	gp_current_process = scheduler();

	if (gp_current_process == NULL) {
		// Revert back to the old process
		gp_current_process = p_pcb_old;

		debug(("k_release_processor: FATAL ERROR: scheduler() proposed switching to a NULL process, not doing anything and returning an error.\r\n"));

		return RTX_ERR;
	}

    if ( p_pcb_old == NULL ) {
		p_pcb_old = gp_current_process;
	}

	process_switch(p_pcb_old);
	return RTX_OK;
}

// PRE: Pass non-null valid PCB node, pid is U32*
BOOL pcb_node_is_pid(node *node, void *pid) {
	if (node == NULL) {
		debug(("pcb_node_is_pid: FATAL ERROR: Passed NULL node to pcb_node_is_pid"));
	}

	U32 desired_pid = * (U32*) pid;
	U32 node_pid = ((PCB *) node)->m_pid;

	return node_pid == desired_pid;
}

PCB *k_get_process_by_pid(int pid) {
	for (int i = 0; i < NUM_TOTAL_USERSPACE_PROCS + NUM_I_PROCS; ++i) {
		if (gp_pcbs[i]->m_pid == pid) {
			return gp_pcbs[i];
		}
	}
	debug(("k_get_process_by_pid: FATAL ERROR: Process PID %d\r\n invalid", pid));
	return NULL;
}

int k_get_process_priority(int pid)
{
	PCB *process = k_get_process_by_pid(pid);
	if (!process) {
		debug(("k_get_process_priority: FATAL ERROR: could not find PCB for pid %d\r\n", pid));
		return RTX_ERR;
	}
	return process->m_priority;
}

int k_set_process_priority(int pid, int prio_new)
{
	/* Cannot change priority of null process or set a process to the same priority as the null process */
	if (pid == 0 || prio_new == PRI_NULL) {
		debug(("k_set_process_priority: ERROR: Cannot change priority of null process or set a process to the same priority as the null process\r\n"));

		return RTX_ERR;
	}

	/* Make sure the priority is valid; in [0, 4) */
	if (prio_new < HIGH || prio_new >= PRI_NULL) {
		debug(("k_set_process_priority: ERROR: Invalid priority %d. Priority must be in [0, 4)\r\n", prio_new));

		return RTX_ERR;
	}

	int curr_priority = k_get_process_priority(pid);
	if (curr_priority == RTX_ERR) {
		debug(("k_set_process_priority: ERROR: No process found with given pid %d\r\n", pid));

		return RTX_ERR;
	}

	/* If the process is already at the new priority, we don't need to do anything */
	if (curr_priority == prio_new) {
		debug(("k_set_process_priority: Process %d is already at the new priority %d\r\n", pid, prio_new));

		return RTX_OK;
	}

	BOOL target_process_blocked = 0;

	/* Try to move process to new priority in ready queues */
	PCB *proc = NULL;
	int rc = move_elt_if((node **) &proc, &ready_queues[curr_priority], &ready_queues[prio_new], &pcb_node_is_pid, &pid);
	k_get_process_by_pid(pid)->m_priority = prio_new;

	if (rc != RTX_OK) {
		debug(("k_set_process_priority: FATAL ERROR: move_elt_if failed\r\n"));

		return RTX_ERR;
	}

	/* Process not found in ready queues - must be blocked */
	if (proc == NULL) {
		target_process_blocked = 1;
		// Try to move pid process from blocked queue to new priority blocked queue
		rc = move_elt_if((node **) &proc, &blocked_queues[curr_priority], &blocked_queues[prio_new], &pcb_node_is_pid, &pid);
		if (rc != RTX_OK) {
			debug(("k_set_process_priority: FATAL ERROR: move_elt_if failed\r\n"));

			return RTX_ERR;
		}
		if (proc->m_state != RDY && proc->m_state != NEW  && proc->m_state != RUN) {
			debug(("k_set_process_priority: FATAL ERROR: No process found with given pid and priority\r\n"));

			return RTX_ERR;
		} else {
			debug(("k_set_process_priority: blocked process not found in ready queue, no issue here :) \r\n"));
		}
	}

	/* Calculate pre-emption */

	int curr_proc_priority = k_get_process_priority(gp_current_process->m_pid);
	if (curr_proc_priority == RTX_ERR) {
		debug(("k_set_process_priority: ERROR: No process found with gp_current_process->pid %d\r\n", gp_current_process->m_pid));

		return RTX_ERR;
	}

	// If process changed its own priority, run scheduler
	// Set priority of the process itself
	if (pid == gp_current_process->m_pid) {
		rc = k_release_processor();
		if (rc != RTX_OK) {
			debug(("k_set_process_priority: FATAL ERROR: k_release_processor() failed\r\n"));

			return RTX_ERR;
		}
	}

	// If the process whose priority changed is higher priority, pre-emption occurs
	else if (prio_new < curr_proc_priority && !target_process_blocked) {
		debug(("k_set_process_priority: Process %d is higher priority (%d) than current process %d (%d). Premption occurs\r\n", pid, prio_new, gp_current_process->m_pid, curr_priority));
		rc = k_release_processor();
		if (rc != RTX_OK) {
			debug(("k_set_process_priority: FATAL ERROR: k_release_processor() failed\r\n"));

			return RTX_ERR;
		}
	}

	return RTX_OK;
}

void null_process(void) {
	debug(("null_process: running\r\n"));
	while (1) {
		// no-op
	}
}

// ATTENTION: NO UNCONDITIONAL DEBUG STATEMENTS ALLOWED (error msg OK)
// For more information, see k_timer.c
int interrupt_handler(void) {
	if (gp_current_process == NULL)
	{
		// interrupt_handler: FATAL ERROR: gp_current_process is NULL, this should never happen
		return RTX_ERR;
	}

	// Get the priority of the current process.
	int curr_priority = k_get_process_priority(gp_current_process->m_pid);

	if (curr_priority == RTX_ERR) {
		// interrupt_handler: FATAL ERROR: could not find priority of pid in queue sanitization.
		return RTX_ERR;
	}

	// Get the highest priority process in the ready queue.
	PCB* highest_priority_new_ready_process = try_scheduler();

	if (highest_priority_new_ready_process == NULL) {
		// interrupt_handler: FATAL ERROR: No process found in ready queue.
		return RTX_ERR;
	}

	// Get the priority of the highest priority process in the ready queue.
	int highest_priority_new_ready_process_priority = k_get_process_priority(highest_priority_new_ready_process->m_pid);

	if (highest_priority_new_ready_process_priority == RTX_ERR) {
		// interrupt_handler: FATAL ERROR: could not find priority of pid in queue sanitization.
		return RTX_ERR;
	}

	// If the highest priority process in the ready queue is of higher priority than the current process, preemption occurs.
	if (highest_priority_new_ready_process_priority < curr_priority) {
		k_release_processor();
	}

	return RTX_OK;
}

/*
 *===========================================================================
 *                             END OF FILE
 *===========================================================================
 */
