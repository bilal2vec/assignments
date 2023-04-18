/*
 ****************************************************************************
 *
 *                  UNIVERSITY OF WATERLOO SE 350 RTX LAB
 *
 *                     Copyright 2020-2022 Yiqing Huang
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
 * @file        k_process.h
 * @brief       process management hearder file
 *
 * @version     V1.2021.01
 * @authors     Yiqing Huang and Thomas Reidemeister
 * @date        2021 JAN
 * @note        Assuming there are only two user processes in the system
 *****************************************************************************/

#ifndef K_PROCESS_H_
#define K_PROCESS_H_

#include "k_inc.h"
#include "queue.h"

/* priority queue for ready processes */
extern queue *ready_queues; // array of queues initialized in k_memory

/* priority queue for blocked processes */
extern queue *blocked_queues; // array of queues initialized in k_memory


/*
 *===========================================================================
 *                             MACROS
 *===========================================================================
 */

#define INITIAL_xPSR 0x01000000        /* user process initial xPSR value */

/*
 *===========================================================================
 *                            FUNCTION PROTOTYPES
 *===========================================================================
 */

extern void process_init(PROC_INIT *proc_info, int num);	/* initialize all procs in the system */
extern PCB	*scheduler(void);          						/* pick the pcb of the next to run process */
extern int	process_switch(PCB *p_pcb_old);
extern int	k_release_processor(void);						/* kernel release_process function */
extern int 	k_get_process_priority(int);					/* get process priority */
extern int 	k_set_process_priority(int, int);				/* set process priority */

extern BOOL pcb_node_is_pid(node *node, void *pid);

extern PCB * k_get_process_by_pid(int pid);					/* get pcb by pid */

extern void null_process(void);								/* null process */

extern int interrupt_handler(void);                        /* interrupt handler */

#endif /* ! K_PROCESS_H_ */
/*
 *===========================================================================
 *                             END OF FILE
 *===========================================================================
 */
