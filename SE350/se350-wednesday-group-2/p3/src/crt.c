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
 * @file        k_crt.c
 * @brief       UART IRQ handler. It receives input char through RX interrupt
 *              and then writes a string containing the input char through
 *              TX interrupts.
 *
 * @version     V1.2022.02
 * @authors     Yiqing Huang and NXP Semiconductors
 * @date        2022 FEB
 *****************************************************************************/

#include <LPC17xx.h>
#include "crt.h"
#include "rtx.h"
#include "common.h"
#include "uart_def.h"

#ifdef DEBUG_0
#include "printf.h"
#endif

void crt(void) {
	LPC_UART_TypeDef *pUart= (LPC_UART_TypeDef *) LPC_UART0;  
	
	while(1) {
		MSG_BUF *msg = receive_message(NULL);
		
		if (msg->mtype != CRT_DISPLAY) {
			int rc = release_memory_block(msg);
			
			if (rc != RTX_OK) {
				debug(("crt: FATAL ERROR: memory release failed\r\n"));
			}
		}
		
		send_message(PID_UART_IPROC, msg);
		pUart->IER |= IER_THRE;
	}
}


/*
 *===========================================================================
 *                             END OF FILE
 *===========================================================================
 */
