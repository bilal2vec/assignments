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
 * @file        main_svc.c
 * @brief       main routine to start up the RTX and processes 
 *              
 * @version     V1.2021.01
 * @authors     Yiqing Huang
 * @date        2021 JAN
 * @note        Standard C library is not allowed in the final kernel code.
 *              A tiny printf function for embedded application development
 *              taken from http://www.sparetimelabs.com/tinyprintf/tinyprintf.php
 *              is configured to use UAR1 to output.
 *              Note that init_printf(NULL, putc) must be called to initialize 
 *              the printf function.
 *****************************************************************************/

#include <LPC17xx.h>
#include "rtx.h"
#include "uart_polling.h"
#include "printf.h"
#include "ae.h"

/**************************************************************************//**
 * @brief   	system set up before calling rtx_init()          
 * @return      0 on success and non-zero on failure
 * @note        leave empty if there is nothing to be done 
 *****************************************************************************/
int pre_rtx_init(void *arg) 
{
    // Anything you want to do before calling rtx_init() 
    return NULL;
}

/**************************************************************************//**
 * @brief   	main routine
 *          
 * @return      0 on success and non-zero on failure
 *****************************************************************************/
int main() 
{   
    /* user test process initialization table */
    PROC_INIT test_procs[NUM_TEST_PROCS];
    
    /* CMSIS system initialization */
    SystemInit();
    
    __disable_irq();
    
    /* uart1 by polling */  
    uart1_init();                        
    
    /* initialize printf to use uart1 by polling */
    init_printf(NULL, putc);
    
    __enable_irq();
        
    /* initialize the third-party testing framework */
    ae_init(test_procs, NUM_TEST_PROCS, &pre_rtx_init, NULL);
    
    /* start the RTX and built-in processes */
    rtx_init(test_procs, NUM_TEST_PROCS);  
  
    /* We should never reach here!!! */
    return RTX_ERR;  
}
/*
 *===========================================================================
 *                             END OF FILE
 *===========================================================================
 */
