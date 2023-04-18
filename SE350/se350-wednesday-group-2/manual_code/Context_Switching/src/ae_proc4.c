
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

/**************************************************************************//**
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

#include "rtx.h"
#include "uart_polling.h"
#include "printf.h"
#include "ae_proc.h"
#include "ae_util.h"
/*
 *===========================================================================
 *							 MACROS
 *===========================================================================
 */

#ifdef AE_ENABLE
	
#define NUM_TESTS	   2	   // number of tests

#ifdef AE_ECE350
#define NUM_INIT_TASKS  2	   // number of tasks during initialization
#endif // AE_ECE350

#endif // AE_ENABLE
/*
 *===========================================================================
 *							 GLOBAL VARIABLES 
 *===========================================================================
 */
 
#ifdef AE_ENABLE

#ifdef AE_ECE350
TASK_INIT	g_init_tasks[NUM_INIT_TASKS];
#endif


const char   PREFIX[]	  = "G2-TS4";
const char   PREFIX_LOG[]  = "G2-TS4-LOG ";
const char   PREFIX_LOG2[] = "G2-TS4-LOG2";

AE_XTEST	 g_ae_xtest;				// test data, re-use for each test
AE_CASE	  g_ae_cases[NUM_TESTS];
AE_CASE_TSK  g_tsk_cases[NUM_TESTS];

#endif // AE_ENABLE


/* initialization table item */
void set_test_procs(PROC_INIT *procs, int num)
{
	int i;
	for( i = 0; i < num; i++ ) {
		procs[i].m_pid		= (U32)(i+1);
		procs[i].m_priority   = LOWEST;
		procs[i].m_stack_size = USR_SZ_STACK;
	}
  
	procs[0].mpf_start_pc = &proc1;
	procs[1].mpf_start_pc = &proc2;
	procs[2].mpf_start_pc = &proc3;
	procs[3].mpf_start_pc = &proc4;
	procs[4].mpf_start_pc = &proc5;
	procs[5].mpf_start_pc = &proc6;
}

void proc1(void)
{

	debug(("proc1: \r\n"));

	void *ptr1 = request_memory_block();
	debug(("proc1: allocated ptr = %x\r\n", ptr1));

	void *ptr2 = request_memory_block();
	debug(("proc1: allocated ptr = %x\r\n", ptr2));

	void *ptr3 = request_memory_block();
	debug(("proc1: allocated ptr = %x\r\n", ptr3));

	release_memory_block(ptr1);
	debug(("proc1: released ptr = %x\r\n", ptr1));

	release_memory_block(ptr2);
	debug(("proc1: released ptr = %x\r\n", ptr2));

	ptr1 = request_memory_block();
	debug(("proc1: allocated ptr = %x\r\n", ptr1));

	while(1) {
		uart1_put_string("proc1: \r\n");

		release_processor();
	}
}

void proc2(void)
{

	void *ptrs[10];
	int i = 0;

	while(i < 10) {
		uart1_put_string("proc2: \r\n");

		ptrs[i] = request_memory_block();
		debug(("proc2: allocated ptrs[%d] = %x\r\n", i, ptrs[i]));

		i++;

		release_processor();
	}

	i = 0;
	while (i < 10) {
		uart1_put_string("proc2: \r\n");

		release_memory_block(ptrs[i]);
		debug(("proc2: released ptrs[%d] = %x\r\n", i, ptrs[i]));

		i++;

		release_processor();
	}

	uart1_put_string("proc2: set priority of proc2 to HIGH\r\n");

	set_process_priority(2, HIGH);

	uart1_put_string("proc2: finished setting priority of proc2 to HIGH\r\n");

	for (int i = 0; i < 30; i++) {
		uart1_put_string("proc2: allocating memory \r\n");

		void *ptr = request_memory_block();

		uart1_put_string("proc2: allocated memory \r\n");

		release_processor();
	}

	while(1) {
		uart1_put_string("proc2: \r\n");
		release_processor();
	}
}




void proc3(void)
{
	
	void *ptrs[10];
	int i = 0;

	while(i < 10) {
		uart1_put_string("proc3: \r\n");

		ptrs[i] = request_memory_block();
		debug(("proc3: allocated ptrs[%d] = %x\r\n", i, ptrs[i]));

		i++;

		release_processor();
	}

	for (int i = 0; i < 10; ++i) {
		uart1_put_string("proc3: not releasing memory\r\n");

		release_processor();
	}

	i = 0;
	while (i < 10) {
		uart1_put_string("proc3: before releasing memory \r\n");

		release_memory_block(ptrs[i]);

		uart1_put_string("proc3: released memory\r\n");

		i++;

		release_processor();
	}

	while(1) {
		uart1_put_string("proc3: \r\n");
		release_processor();
	}

}

void proc4(void)
{
	void *ptrs[10];
	int i = 0;

	while(i < 10) {
		uart1_put_string("proc4: \r\n");

		ptrs[i] = request_memory_block();
		debug(("proc4: allocated ptrs[%d] = %x\r\n", i, ptrs[i]));

		i++;

		release_processor();
	}

	i = 0;
	while (i < 10) {
		uart1_put_string("proc4: \r\n");

		release_memory_block(ptrs[i]);
		debug(("proc4: released ptrs[%d] = %x\r\n", i, ptrs[i]));

		i++;

		release_processor();
	}

	while(1) {
		uart1_put_string("proc4: \r\n");
		release_processor();
	}
}

void proc5(void)
{
	while(1) {
		uart1_put_string("proc5: \r\n");
		release_processor();
	}
}

void proc6(void)
{
	while(1) {
		uart1_put_string("proc6: \r\n");
		release_processor();
	}
}
/*
 *===========================================================================
 *							 END OF FILE
 *===========================================================================
 */
