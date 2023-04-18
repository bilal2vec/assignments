/*
 ****************************************************************************
 *
 *				  UNIVERSITY OF WATERLOO SE 350 RTOS LAB
 *
 *					 Copyright 2020-2022 Yiqing Huang
 *
 *		  This software is subject to an open source license and
 *		  may be freely redistributed under the terms of MIT License.
 ****************************************************************************
 */

/**************************************************************************/ /**
* @file		ae_proc*.c
* @brief	   test processes template file
*
* @version	 V1.2022.01
* @authors	 Yiqing Huang
* @date		2022 JAN
* @note		Each process is in an infinite loop. Processes never terminate.
*			  This file needs to be completed by students.
*
*****************************************************************************/

#include "ae_proc.h"
#include "ae_util.h"
#include "printf.h"
#include "rtx.h"
#include "uart_polling.h"

/*
 *===========================================================================
 *							 MACROS
 *===========================================================================
 */

#ifdef AE_ENABLE

#define NUM_TESTS 2 // number of tests

#ifdef AE_ECE350
#define NUM_INIT_TASKS 2 // number of tasks during initialization
#endif				   // AE_ECE350

#endif // AE_ENABLE
/*
 *===========================================================================
 *							 GLOBAL VARIABLES
 *===========================================================================
 */

#ifdef AE_ENABLE

#ifdef AE_ECE350
TASK_INIT g_init_tasks[NUM_INIT_TASKS];
#endif

const char PREFIX[] = "G2-TS2";
const char PREFIX_LOG[] = "G2-TS2-LOG ";
const char PREFIX_LOG2[] = "G2-TS2-LOG2";

AE_XTEST g_ae_xtest; // test data, re-use for each test
AE_CASE g_ae_cases[NUM_TESTS];
AE_CASE_TSK g_tsk_cases[NUM_TESTS];

#endif // AE_ENABLE

/* Tests for changing process priority */
/* Initially all tests are at HIGH priority */
/* Proc 1 and Proc 2 changes themselves to LOWEST priority immediately,
 * preempting themselves and making sure they don't print the message about
 * having set themselves to LOWEST */
/* Proc 3 changes itself to MEDIUM priority immediately, preempting itself and
 * making sure it doesn't print the message about having set itself to MEDIUM */
/* Proc 4, 5, and 6 set themeselves to HIGH priority, and since they are already
 * at HIGH priority nothing happens and they each print a message saying they
 * are already at HIGH priority in ascending order of process PIDs */
/* Procs 4, 5, and 6 then keep looping; printing their pid and then releasing
 * the processor */
/* After 10 iterations of this, Proc 6 will set the priority of Proc 1 to HIGH,
 * *not* preempting itself, and then set its own priority to LOWEST, preempting
 * itself and making sure it doesn't print the message about having set itself
 * to LOWEST */
/* After this, process 4, 5, and 1 will print their pid and then release the
 * processor in a loop forever */

/* initialization table item */
void set_test_procs(PROC_INIT* procs, int num)
{
	for (int i = 0; i < num; i++) {
		procs[i].m_pid = (U32)(i + 1);
		procs[i].m_stack_size = USR_SZ_STACK;
	}

	procs[0].mpf_start_pc = &proc1;
	procs[0].m_priority = HIGH;

	procs[1].mpf_start_pc = &proc2;
	procs[1].m_priority = MEDIUM;

	procs[2].mpf_start_pc = &proc3;
	procs[2].m_priority = LOW;

	procs[3].mpf_start_pc = &proc4;
	procs[3].m_priority = LOWEST;

	procs[4].mpf_start_pc = &proc5;
	procs[4].m_priority = LOWEST;

	procs[5].mpf_start_pc = &proc6;
	procs[5].m_priority = LOWEST;
}

void proc1(void)
{
	uart1_put_string("proc1: \r\n");
	release_processor();

	for (int i = 0; i < 10; ++i) {
		int pid;
		MSG_BUF* msg = receive_message(&pid);
		printf("Proc 1 got %d-th message from PID %d: %s\r\n", i, pid, msg->mtext);
		release_memory_block(msg);
		release_processor();
	}
	set_process_priority(1, LOWEST);
	while (1) {
		uart1_put_string("proc1: \r\n");
		release_processor();
	}
}

void proc2(void)
{
	uart1_put_string("proc2: \r\n");

	release_processor();

	for (int i = 0; i < 10; ++i) {
		MSG_BUF* msg = request_memory_block();
		msg->mtype = DEFAULT;
		strcpy(msg->mtext, "THEY'RE IN MY WALLS");
		send_message(1, msg);
		// release_processor();
	}
	// send to process of lower priority
	for (int i = 0; i < 10; ++i) {
		MSG_BUF* msg = request_memory_block();
		msg->mtype = DEFAULT;
		strcpy(msg->mtext, "THEY'RE IN MY WALLS");
		send_message(3, msg);
	}
	// set own priority to lowest
	set_process_priority(2, LOWEST);

	while (1) {
		uart1_put_string("proc2: \r\n");
		release_processor();
	}
}

void proc3(void)
{
	uart1_put_string("proc3: \r\n");

	release_processor();
	for (int i = 0; i < 10; ++i) {
		int pid;
		MSG_BUF* msg = receive_message(&pid);
		printf("Proc 3 got %d-th message from PID %d: %s\r\n", i, pid, msg->mtext);
		release_memory_block(msg);
	}
	while (1) {
		// uart1_put_string("proc3: \r\n");
		release_processor();
	}
}

void proc4(void)
{
	uart1_put_string("proc4: \r\n");

	release_processor();

	while (1) {
		uart1_put_string("proc4: \r\n");
		release_processor();
	}
}

void proc5(void)
{

	uart1_put_string("proc5: \r\n");

	release_processor();

	while (1) {
		uart1_put_string("proc5: \r\n");
		release_processor();
	}
}

void proc6(void)
{

	uart1_put_string("proc6: \r\n");

	release_processor();

	while (1) {
		uart1_put_string("proc6: \r\n");
		release_processor();
	}
}
/*
 *===========================================================================
 *							 END OF FILE
 *===========================================================================
 */
