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
 * @file		k_msg.c
 * @brief	   kernel message passing source code template
 *			  
 * @version	 V1.2021.01
 * @authors	 Yiqing Huang
 * @date		2021 JAN
 *****8************************************************************************/
#include "k_msg.h"
#include "k_rtx.h"
#include "queue.h"
/*
 *==========================================================================
 *                            GLOBAL VARIABLES
 *==========================================================================
 */
 
queue *delay_queue = NULL;


int k_send_message(int pid, void *p_msg)
{
	// Interrupts off at this point
	// See SVC_Handler in HAL.c
//	debug(("k_send_message: Sending block @ 0x%x\r\n", p_msg)); // debug message OFF due to it causing us to fail timer tests
	// to get the address of the block set a breakpoint to the start of the function

	// Set send and recv
	((MSG_BUF*)p_msg)->m_send_pid = gp_current_process->m_pid;
	((MSG_BUF*)p_msg)->m_recv_pid = pid;
	PCB *target = k_get_process_by_pid(pid);
	int rc = reassign_memory_ownership(p_msg, gp_current_process, target);

	if (rc != RTX_OK) {
		debug(("k_send_message: FATAL ERROR: failed to reassign memory ownership\r\n"));
	}

	rc = enqueue(&(target->m_message_queue), p_msg);
	if (rc != RTX_OK) {
		debug(("k_send_message: FATAL ERROR: failed to enqueue memory block onto destination heap\r\n"));
		set_unprivileged();
		return RTX_ERR;
	}
	if (target->m_state == BLK_MSG) {
		target->m_state = RDY;
		enqueue(&ready_queues[target->m_priority], (node*)target);
		if (target->m_priority < gp_current_process->m_priority) {
			// Preemption
			debug(("k_send_message: Caused preemption (dest process is higher priority than src)\r\n"));
			rc = k_release_processor();
			if (rc != RTX_OK) {
				debug(("k_send_message: FATAL ERROR: Failed to release processor when doing preemption.\r\n")); 
			}
		}
	}
	
	set_unprivileged();
	return RTX_OK;
}

int k_delayed_send(int pid, void *p_msg, int delay)
{
	// Interrupts off at this point
	// See SVC_Handler in HAL.c
	// debug(("k_delayed_send_message: Sending block @ 0x%x\r\n", p_msg)); // like above turned off in order to pass tests

	// Set send and recv
	((MSG_BUF*)p_msg)->m_send_pid = gp_current_process->m_pid;
	((MSG_BUF*)p_msg)->m_recv_pid = pid;
	((MSG_BUF*)p_msg)->m_kdata[0] = delay;

	// Assign ownership of the message block to the Timer I-Process for now.	
	int rc = reassign_memory_ownership(p_msg, gp_current_process, gp_pcb_timer_iproc);

	if (rc != RTX_OK) {
		debug(("k_send_message: FATAL ERROR: failed to reassign memory ownership\r\n"));
	}

	rc = enqueue(&(gp_pcb_timer_iproc->m_message_queue), p_msg);
	if (rc != RTX_OK) {
		debug(("k_send_message: FATAL ERROR: failed to enqueue memory block onto destination heap\r\n"));
		set_unprivileged();
		return RTX_ERR;
	}
	
	set_unprivileged();
	return RTX_OK;
}

void *k_receive_message(int *p_pid)
{
	while (empty(&gp_current_process->m_message_queue)) {
		gp_current_process->m_state = BLK_MSG;
		// Remove current process so it can't be scheduled anymore.
		PCB* tmp_cur_process = NULL;
		int rc = delete_elt_if((node**)&tmp_cur_process,
					  &ready_queues[gp_current_process->m_priority],
					  pcb_node_is_pid,
					  &gp_current_process->m_pid);
		if (rc != RTX_OK || !tmp_cur_process) {
			debug(("k_receive_message: FATAL ERROR: could not delete current process from ready queue. Process will continue running after k_release_processor()\r\n"));
		}
		
		rc = k_release_processor();
		if (rc != RTX_OK) {
			debug(("k_receive_message: FATAL ERROR: Failed to release processor.\r\n"));
		}
		// After this, the process must have one message in its queue since it's unblocked now
	}
	if (p_pid) {
		*p_pid = ((MSG_BUF*)gp_current_process->m_message_queue.head)->m_send_pid;
	}
	node* message_returned = NULL;
	int rc = dequeue(&message_returned, &gp_current_process->m_message_queue);
	if (rc != RTX_OK) {
		debug(("k_receive_message: FATAL ERROR: Could not deque message from message queue\r\n"));
	} else if (!message_returned) {
		debug(("k_receive_message: FATAL ERROR: Message queue is empty (this should never happen! process should be blocked)\r\n"));
	}
	return (void *) message_returned;
}

void *k_receive_message_nb(int *p_pid) {
	if (!empty(&gp_current_process->m_message_queue)) {
		if (p_pid) {
			*p_pid = ((MSG_BUF*)gp_current_process->m_message_queue.head)->m_send_pid;
		}
		node* message_returned = NULL;
		int rc = dequeue(&message_returned, &gp_current_process->m_message_queue);
		if (rc != RTX_OK) {
			debug(("k_receive_msg_nb: FATAL ERROR: Could not deque message from message queue\r\n"));
		} else if (!message_returned) {
			debug(("k_receive_msg_nb: FATAL ERROR: Message queue is empty (this should never happen! process should be blocked)\r\n"));
		}
		return (void *) message_returned;		
	} else {
		return NULL;
	}	
}
 
/*
 *===========================================================================
 *							 END OF FILE
 *===========================================================================
 */
