/*******************************************************************************
**
** Source File Name = util.h  1.7
**
** Licensed Materials - Property of IBM
**
** (C) COPYRIGHT International Business Machines Corp. 1995, 1997 
** All Rights Reserved.
**
** US Government Users Restricted Rights - Use, duplication or
** disclosure restricted by GSA ADP Schedule Contract with IBM Corp.
**
**
** PURPOSE : Header file for util.c
**
*******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/* Defines for BINDING */
#define DEFAULT_ISOLVL     0
#define REPEATABLE_READ    1
#define READ_STABILITY     2
#define CURSOR_STABILITY   3
#define UNCOMMITTED_READ   4

#define MEMERROR	32000


int check_error (char eString[], struct sqlca *caPointer);

void init_da (struct sqlda **DAPointer, int DAsqln);

void alloc_host_vars (struct sqlda *sqldaPointer);

void free_da (struct sqlda *sqldaPointer);

void print_var (char *ptr, int type, short collen, short datalen);

void display_col_titles (struct sqlda *sqldaPointer);

void display_da (struct sqlda *sqldaPointer);

/* Routines involving Error Logging */
extern void init_error_log(void);
extern void sqlerror(struct sqlca *sqlca);
extern void display_errors(void);
extern void free_error_log(void);

#ifdef __cplusplus
}
#endif