/*
 ****************************************************************************
 *
 *                  UNIVERSITY OF WATERLOO SE 350 RTX LAB
 *
 *        Copyright 2020-2022 Yiqing Huang and NXP Semiconductors
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
 * @file        uart_irq.c
 * @brief       UART IRQ handler. It receives input char through RX interrupt
 *              and then writes a string containing the input char through
 *              TX interrupts.
 *
 * @version     V1.2022.02
 * @authors     Yiqing Huang and NXP Semiconductors
 * @date        2022 FEB
 *****************************************************************************/
#include <LPC17xx.h>
#include "uart_irq.h"
#include "uart_polling.h"
#include "k_inc.h"
#include "queue.h"
#include "k_msg.h"

#include "rtx.h"
#ifdef DEBUG_0
#include "printf.h"
#endif


/**************************************************************************//**
 * @brief: initialize the n_uart
 * NOTES: It only supports UART0. It can be easily extended to support UART1 IRQ.
 * The step number in the comments matches the item number in Section 14.1 on pg 298
 * of LPC17xx_UM
 *****************************************************************************/
int uart_irq_init(int n_uart) {

	LPC_UART_TypeDef *pUart;

	if ( n_uart ==0 ) {
		/*
		Steps 1 & 2: system control configuration.
		Under CMSIS, system_LPC17xx.c does these two steps

		-----------------------------------------------------
		Step 1: Power control configuration.
				See table 46 pg63 in LPC17xx_UM
		-----------------------------------------------------
		Enable UART0 power, this is the default setting
		done in system_LPC17xx.c under CMSIS.
		Enclose the code for your refrence
		//LPC_SC->PCONP |= BIT(3);

		-----------------------------------------------------
		Step2: Select the clock source.
			   Default PCLK=CCLK/4 , where CCLK = 100MHZ.
			   See tables 40 & 42 on pg56-57 in LPC17xx_UM.
		-----------------------------------------------------
		Check the PLL0 configuration to see how XTAL=12.0MHZ
		gets to CCLK=100MHZin system_LPC17xx.c file.
		PCLK = CCLK/4, default setting after reset.
		Enclose the code for your reference
		//LPC_SC->PCLKSEL0 &= ~(BIT(7)|BIT(6));

		-----------------------------------------------------
		Step 5: Pin Ctrl Block configuration for TXD and RXD
				See Table 79 on pg108 in LPC17xx_UM.
		-----------------------------------------------------
		Note this is done before Steps3-4 for coding purpose.
		*/

		/* Pin P0.2 used as TXD0 (Com0) */
		LPC_PINCON->PINSEL0 |= (1 << 4);

		/* Pin P0.3 used as RXD0 (Com0) */
		LPC_PINCON->PINSEL0 |= (1 << 6);

		pUart = (LPC_UART_TypeDef *) LPC_UART0;

	} else if ( n_uart == 1) {

		/* see Table 79 on pg108 in LPC17xx_UM */
		/* Pin P2.0 used as TXD1 (Com1) */
		LPC_PINCON->PINSEL4 |= (2 << 0);

		/* Pin P2.1 used as RXD1 (Com1) */
		LPC_PINCON->PINSEL4 |= (2 << 2);

		pUart = (LPC_UART_TypeDef *) LPC_UART1;

	} else {
		return 1; /* not supported yet */
	}

	/*
	-----------------------------------------------------
	Step 3: Transmission Configuration.
			See section 14.4.12.1 pg313-315 in LPC17xx_UM
			for baud rate calculation.
	-----------------------------------------------------
	*/

	/* Step 3a: DLAB=1, 8N1 */
	pUart->LCR = UART_8N1; /* see uart.h file */

	/* Step 3b: 115200 baud rate @ 25.0 MHZ PCLK */
	pUart->DLM = 0; /* see table 274, pg302 in LPC17xx_UM */
	pUart->DLL = 9;    /* see table 273, pg302 in LPC17xx_UM */

	/* FR = 1.507 ~ 1/2, DivAddVal = 1, MulVal = 2
	   FR = 1.507 = 25MHZ/(16*9*115200)
	   see table 285 on pg312 in LPC_17xxUM
	*/
	pUart->FDR = 0x21;



	/*
	-----------------------------------------------------
	Step 4: FIFO setup.
		   see table 278 on pg305 in LPC17xx_UM
	-----------------------------------------------------
		enable Rx and Tx FIFOs, clear Rx and Tx FIFOs
	Trigger level 0 (1 char per interrupt)
	*/

	pUart->FCR = 0x07;

	/* Step 5 was done between step 2 and step 4 a few lines above */

	/*
	-----------------------------------------------------
	Step 6 Interrupt setting and enabling
	-----------------------------------------------------
	*/
	/* Step 6a:
	   Enable interrupt bit(s) wihtin the specific peripheral register.
		   Interrupt Sources Setting: RBR, THRE or RX Line Stats
	   See Table 50 on pg73 in LPC17xx_UM for all possible UART0 interrupt sources
	   See Table 275 on pg 302 in LPC17xx_UM for IER setting
	*/
	/* disable the Divisior Latch Access Bit DLAB=0 */
	pUart->LCR &= ~(BIT(7));

	/* enable RBR and RLS interrupts */
	pUart->IER = IER_RBR | IER_RLS;

	/* Step 6b: set UART interrupt priority and enable the UART interrupt from the system level */
	if ( n_uart == 0 ) {
		/* UART0 IRQ priority setting */
		NVIC_SetPriority(UART0_IRQn, 0x08);
		NVIC_EnableIRQ(UART0_IRQn);
	} else if ( n_uart == 1 ) {
		NVIC_SetPriority(UART1_IRQn, 0x08);
		NVIC_EnableIRQ(UART1_IRQn);
	} else {
		return 1; /* not supported yet */
	}

	return 0;
}


/**
 * @brief: use CMSIS ISR for UART0 IRQ Handler
 * NOTE: This example shows how to save/restore all registers rather than just
 *       those backed up by the exception stack frame. We add extra
 *       push and pop instructions in the assembly routine.
 *       The actual uart0_iproc does the rest of irq handling
 */
__asm void UART0_IRQHandler(void)
{
	PRESERVE8
	IMPORT  uart0_iproc
	IMPORT  interrupt_handler
	CPSID   I                               // disable interrupt
	PUSH    {r4-r11, lr}                    // save all registers
IUART_SAVE
	// save the sp of the current running process into its PCB
	LDR     R1, =__cpp(&gp_current_process) // load R1 with &gp_current_process
	LDR     R2, [R1]                        // load R2 with gp_current_process value
	STR     SP, [R2, #PCB_MSP_OFFSET]       // save MSP to gp_current_process->mp_sp

	// save the interrupted process's PCB in gp_pcb_interrupted
	LDR     R3, =__cpp(&gp_pcb_interrupted) // load &gp_pcb_interrupted to R3
	STR     R2, [R3]                        // assign gp_current_process to gp_pcb_interrupted

	// update gp_current_process to point to the gp_pcb_uart_iproc
	LDR		R1, =__cpp(&gp_pcb_uart_iproc)	// load R1 with &gp_pcb_uart_iproc
	LDR		R2, [R1]						// load R2 with gp_pcb_uart_iproc value
	LDR		R3, =__cpp(&gp_current_process) // load R3 with &gp_current_process
	STR 	R2, [R3]						// assign gp_pcb_uart_iproc to gp_current_process

IUART_EXEC
	// update gp_current_process to the PCB of uart_iproc0
	LDR     R1, =__cpp(&gp_pcb_uart_iproc)	// load R1 with &gp_pcb_uart_iproc
	LDR     R2, [R1]                        // load R2 with gp_pcb_uart_iproc value
	LDR     SP, [R2, #PCB_MSP_OFFSET]       // load MSP with UART_IPROC's SP (i.e. gp_pcb_uart_iproc->mp_sp)
	BL      uart0_iproc                   	// execute the uart i-process

IUART_RESTORE
	// update the gp_current_process to gp_pcb_interrupted
	LDR		R1, =__cpp(&gp_pcb_interrupted) // load R1 with &gp_pcb_interrupted
	LDR		R2, [R1]						// load R2 with gp_pcb_interrupted value
	LDR		R3, =__cpp(&gp_current_process) // load R3 with &gp_current_process
	STR 	R2, [R3]						// assign gp_pcb_interrupted to gp_current_process

	// restore the interrupted process's PCB to gp_current_process
	LDR     R1, =__cpp(&gp_pcb_interrupted)
	LDR     R2, [R1]
	LDR     SP, [R2, #PCB_MSP_OFFSET]       // load MSP with gp_current_process->mp_sp
	BL      interrupt_handler               // run a Non-IPROC
	CPSIE   I                               // enable interrupt
	POP     {r4-r11, pc}                    // restore all registers
}


char buffer[UART_PRINT_BUFFER_SIZE] = "";
char *buffer_char = &buffer[0];

queue blocked_on_receive_queues[NUM_PRIORITIES];


int write_pcb_to_dest(char* dest, PCB* pcb_node) {
	char* result;
	int size = 0;
	switch (pcb_node->m_pid) {
		case 0:
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
		case 9:			
			sprintf(dest, "%d ", pcb_node->m_pid);
			return 2;
		case 10:
			result = "SET_PRIO ";
			size = 9;
			break;
		case 11:
			result = "CLOCK ";
			size = 6;
			break;
		case 12:
			result = "KCD ";
			size = 4;
			break;
		case 13:
			result = "CRT ";
			size = 4;
			break;
		case 14:
			result = "TIMER_I ";
			size = 8;
			break;
		case 15:
			result = "UART_I ";
			size = 6;
			break;
		default:
			debug(("if you're reading this, I'm dead.\r\n"));
			break;
	}

	sprintf(dest, result);
	return size;
}


int ready_queue_name_length = 14;
char *ready_queue_names[5] = {
	"READY_HIGH:   ",
	"READY_MEDIUM: ",
	"READY_LOW:    ",
	"READY_LOWEST: ",
	"READY_NULL:   "
};

int blocked_queue_name_length = 16;
char *blocked_queue_names[4] = {
	"BLOCKED_HIGH:   ",
	"BLOCKED_MEDIUM: ",
	"BLOCKED_LOW:    ",
	"BLOCKED_LOWEST: "
};

int blocked_receive_queue_name_length = 24;
char *blocked_receive_queue_names[4] = {
	"BLOCKED_RECEIVE_HIGH:   ",
	"BLOCKED_RECEIVE_MEDIUM: ",
	"BLOCKED_RECEIVE_LOW:    ",
	"BLOCKED_RECEIVE_LOWEST: "
};


/**
 * @brief: c UART0 IRQ Handler
 */
void uart0_iproc(void)
{
	uint8_t IIR_IntId;        /* Interrupt ID from IIR */
	LPC_UART_TypeDef *pUart = (LPC_UART_TypeDef *) LPC_UART0;

	/* Reading IIR automatically acknowledges the interrupt */
	IIR_IntId = (pUart->IIR) >> 1 ; /* skip pending bit in IIR */

	if (IIR_IntId & IIR_RDA) { /* Receive Data Available */

		/* Read UART. Reading RBR will clear the interrupt */
		int char_in = pUart->RBR;
		debug(("uart0_iproc: Reading a char = %c \r\n", char_in));

		MSG_BUF* msg;
		PCB *cur;

		switch (char_in) {
			// print ready queue
			case '!':
				for (int i = 0; i < NUM_PRIORITIES; ++i) {
					msg = k_request_memory_block_nb();

					if (msg == NULL) {
						debug(("uart0_iproc: no memory blocks available\r\n"));
						return;
					}

					msg->mtype = CRT_DISPLAY;
					
					sprintf(msg->mtext, "\n");
					sprintf(msg->mtext + 1, ready_queue_names[i]);
					serialize_queue_to_dest(
						msg->mtext + 1 + ready_queue_name_length,
						&ready_queues[i], (int (*)(char *, node *)) &write_pcb_to_dest
					);
					k_send_message(PID_CRT, msg);
				}

				break;
				
			// print blocked memory queue
			case '@':
				// null process never blocked on memory, so don't print it
				for (int i = 0; i < NUM_PRIORITIES - 1; ++i) {
					msg = k_request_memory_block_nb();

					if (msg == NULL) {
						debug(("uart0_iproc: no memory blocks available\r\n"));
						return;
					}

					msg->mtype = CRT_DISPLAY;

					sprintf(msg->mtext, "\n");
					sprintf(msg->mtext + 1, blocked_queue_names[i]);
					serialize_queue_to_dest(
						msg->mtext + 1 + blocked_queue_name_length,
						&blocked_queues[i], (int (*)(char *, node *)) &write_pcb_to_dest
					);
					k_send_message(PID_CRT, msg);
				}
				break;
				
			// print blocked receive "queue"
			case '#':

				// null process never blocked on message, so don't print it
				for (int p = 0; p < NUM_PRIORITIES - 1; ++p) {
					msg = k_request_memory_block_nb();

					if (msg == NULL) {
						debug(("uart0_iproc: no memory blocks available\r\n"));
						return;
					}

					msg->mtype = CRT_DISPLAY;

					// initialize queue
					blocked_on_receive_queues[p].head = NULL;
					blocked_on_receive_queues[p].tail = NULL;

					// add all processes of priority p that are blocked on message to the queue
					for (int i = 0; i < NUM_TOTAL_USERSPACE_PROCS + NUM_I_PROCS; ++i) {
						cur = gp_pcbs[i];
						if (gp_pcbs[i]->m_state == BLK_MSG && p == cur->m_priority) {
							enqueue(&blocked_on_receive_queues[p], (node *) cur);
						}
					}

					sprintf(msg->mtext, "\n");
					sprintf(msg->mtext + 1, blocked_receive_queue_names[p]);
					serialize_queue_to_dest(
						msg->mtext + 1 + blocked_receive_queue_name_length,
						&blocked_on_receive_queues[p], (int (*)(char *, node *)) &write_pcb_to_dest
					);
					k_send_message(PID_CRT, msg);
				}

				break;
				
			// umut magic
			case '$':
				// this code is extremely important I will hold the 3% lab grade hostage
				// with it don't remove pls thx <3 <3
				for (int r = 0; r < 25; ++r) {
					msg = k_request_memory_block_nb();

					if (msg == NULL) {
						debug(("uart0_iproc: no memory blocks available\r\n"));
						return;
					}

					msg->mtype = CRT_DISPLAY;
					for (int c = 0; c <= 65; ++c) {
						msg->mtext[c] = *((char *) (COOL_NUMBER + 65 * r + c));
					}
					msg->mtext[65] = '\0';
					k_send_message(PID_CRT, msg);
				}
				break;
				
			default:
				msg = k_request_memory_block_nb();

				if (msg == NULL) {
					debug(("uart0_iproc: no memory blocks available\r\n"));
					return;
				}

				msg->mtype = KEY_IN;
				msg->mtext[0] = char_in;
				msg->mtext[1] = '\0';
				k_send_message(PID_KCD, msg);
				break;
		}

	} else if (IIR_IntId & IIR_THRE) {
		// Buffer empty, check message queue
		if (*buffer_char == '\0') {
			MSG_BUF *msg = k_receive_message_nb(NULL);

			// Message queue empty
			if (msg == NULL) {
				pUart->IER &= ~IER_THRE; // Clear the IER_THRE bit
				buffer_char = &buffer[0];
				buffer[0] = '\0';
				return;
			}

			int buffer_i = 0;
			int msg_i = 0;
			while(1) {
				buffer[buffer_i] = msg->mtext[msg_i];
				if (msg->mtext[msg_i] == '\r') {
					buffer[buffer_i + 1] = '\n';
					++buffer_i;
				}
				else if (msg->mtext[msg_i] == '\0') {
					break;
				}
				++buffer_i;
				++msg_i;
			}
			
			buffer_char = &buffer[0];
			k_release_memory_block(msg);
		}

		debug(("uart0_iproc: Writing a char = %c \r\n", *buffer_char));

		// Print from buffer
		pUart->THR = *buffer_char;
		buffer_char++;

	} else {
		debug(("uart0_iproc: FATAL ERROR: Should not get here!\r\n"));
		return;
	}
}


/*
 *===========================================================================
 *                             END OF FILE
 *===========================================================================
 */
