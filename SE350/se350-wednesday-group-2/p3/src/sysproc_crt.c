#include <LPC17xx.h>
#include "crt.h"
#include "rtx.h"
#include "common.h"
#include "uart_def.h"

#ifdef DEBUG_0
#include "printf.h"
#endif

void crt_proc(void) {
	LPC_UART_TypeDef *pUart= (LPC_UART_TypeDef *) LPC_UART0;  
	
	while(1) {
		MSG_BUF *msg = receive_message(NULL);
		
		if (msg->mtype != CRT_DISPLAY) {
			int rc = release_memory_block(msg);
			msg = NULL;
			if (rc != RTX_OK) {
				debug(("crt: FATAL ERROR: memory release failed\r\n"));
			}
		}
		
		if (msg != NULL) {
			send_message(PID_UART_IPROC, msg);
			pUart->IER |= IER_THRE;
			pUart->THR = '\0';
		}		
	}
}
