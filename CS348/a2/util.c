/*******************************************************************************
**
** Source File Name = util.c  1.10
**
** Licensed Materials - Property of IBM
**
** (C) COPYRIGHT International Business Machines Corp. 1995, 1999
** All Rights Reserved.
**
** US Government Users Restricted Rights - Use, duplication or
** disclosure restricted by GSA ADP Schedule Contract with IBM Corp.
**
**
**    PURPOSE : Utility procedures for API implemented programs.  Procedures
**              include the checking of error, and a procedure that retreives
**              data
**    APIs USED :
**       GET SQLSTATE MESSAGE                   sqlogstt()
**       GET ERROR MESSAGE                      sqlaintp()
**
**    STRUCTURES USED :
**       sqlda
**       sqlca
**
** For more information about these samples see the README file.
**
** For more information on programming in C, see the:
**   -  "Programming in C and C++" section of the Application Development Guide
** For more information on building C applications, see the:
**   -  "Building C Applications" section of the Application Building Guide.
**
** For more information on the SQL language see the SQL Reference.
**
*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <sql.h>
#include <sqlenv.h>
#include <sqlda.h>
#include <sqlca.h>
#include <string.h>
#include <ctype.h>

/* #include "system.h" */

#ifdef DB268K
/* Need to include ASLM for 68K applications */
#include <LibraryManager.h>
#endif

#define SQLSTATE sqlca.sqlstate

#ifndef max
#define max(A, B) ((A) > (B) ? (A) : (B))
#endif
#ifndef min
#define min(A, B) ((A) > (B) ? (B) : (A))
#endif

#define CHECKERR(CE_STR) check_error(CE_STR, &sqlca)
#define LOBLENGTH 29

#if defined(DB2OS2)
#include <conio.h>
#elif defined(DB2AIX)
#include <curses.h>
#endif

short *errdata = NULL; /* Hold error counts */

#define mem_error(xx)                                     \
    {                                                     \
        printf("SQCERROR--Out of memory when %s.\n", xx); \
        exit(-1);                                         \
    }

#define MAXERRORS 65536

struct lob_file
{
    unsigned long name_length;
    unsigned long data_length;
    unsigned long file_option;
    char name[255];
};

struct col_info
{
    char name[19]; /* name of column */
    int coltype;   /* column type */
    short length,  /* length of the column */
        colno;     /* column number */
};

struct tab_info
{
    char name[19],  /* name of table */
        creator[9], /* creator of the table */
        type;
    short colcount;               /* number of columns */
    struct col_info **alpha_list, /* alphabetical listing of columns */
        **num_list;               /* numerical listing of columns */
};

/*******************************************************************************
** Procedure : check_error
**
** Purpose :   This procedure checks the SQLCACODE flag and prints out any
**             information that is available related to the specific error.
**
*******************************************************************************/
int check_error(char eString[], struct sqlca *caPointer)
{
    char eBuffer[1024];
    char sBuffer[1024];
    char message[1024];
    char messToken[1024];
    short rc, Erc;
    int status = 0;

    if (caPointer->sqlcode != 0 && caPointer->sqlcode != 100)
    {
        strcpy(message, "");

        sprintf(messToken, "--- error report ---\n");
        strcat(message, messToken);

        sprintf(messToken, "ERROR occurred : %s.\nSQLCODE : %ld\n", eString,
                (long)caPointer->sqlcode);
        strcat(message, messToken);

        /**********************\
        * GET SQLSTATE MESSAGE *
        \**********************/
        rc = sqlogstt(sBuffer, 1024, 80, caPointer->sqlstate);

        /******************************\
        * GET ERROR MESSAGE API called *
        \******************************/
        Erc = sqlaintp(eBuffer, 1024, 80, caPointer);

        /* return code is the length of the eBuffer string */
        if (Erc > 0)
        {
            sprintf(messToken, "%s", eBuffer);
            strcat(message, messToken);
        }

        if (caPointer->sqlcode < 0)
        {
            if (rc == 0)
            {
                sprintf(messToken, "\n%s", sBuffer);
                strcat(message, messToken);
            }
            sprintf(messToken, "--- end error report ---\n");
            strcat(message, messToken);

            printf("%s", message);
            return 1;
        }
        else
        { /* errorCode is just a Warning message */
            if (rc == 0)
            {
                sprintf(messToken, "\n%s", sBuffer);
                strcat(message, messToken);
            }
            sprintf(messToken, "--- end error report ---\n");
            strcat(message, messToken);

            sprintf(messToken, "WARNING - CONTINUING PROGRAM WITH WARNINGS!\n");
            strcat(message, messToken);

            printf("%s", message);
            return 0;
        } /* endif */
    }     /* endif */

    return 0;
}
/* COMMENT OUT ON */

/* functions/procedures on setting up & outputting info from SQLDA */

/*******************************************************************************
 * PROCEDURE : init_da
 * The following procedure allocate memory space for the prepared SQL statement
 *  to reside in.  The use of an internally described function called
 *  SQLDASIZE is used to calculate the proper amount of memory is needed.
 *******************************************************************************/
void init_da(struct sqlda **DAPointer, int DAsqln)
{
    *DAPointer = (struct sqlda *)malloc(SQLDASIZE(DAsqln));
    if (*DAPointer == NULL)
    {
        fprintf(stderr, "\ninit_da : out of memory error.\n");
        exit(-1);
    }
    strncpy((*DAPointer)->sqldaid, "SQLDA   ", sizeof((*DAPointer)->sqldaid));
    (*DAPointer)->sqldabc = (long)SQLDASIZE(DAsqln);
    (*DAPointer)->sqln = DAsqln;
    (*DAPointer)->sqld = 0;
}

/*******************************************************************************
 * FUNCTION : alloc_host_vars
 * This function allocates memory to be filled with the data directed from an
 *  SQLDA pointer.
 *******************************************************************************/
void alloc_host_vars(struct sqlda *sqldaPointer)
{
    short idx;
    unsigned int memsize = 0;
    long longmemsize = 0;
    int precision = 0;

    for (idx = 0; idx < sqldaPointer->sqld; idx++)
    {
        switch (sqldaPointer->sqlvar[idx].sqltype)
        {
        case SQL_TYP_BLOB:
        case SQL_TYP_NBLOB:
        case SQL_TYP_CLOB:
        case SQL_TYP_NCLOB:
        case SQL_TYP_DBCLOB:
        case SQL_TYP_NDBCLOB:
            longmemsize = GETSQLDALONGLEN(sqldaPointer, idx);

#if defined(DB2WIN)
            memsize = (unsigned int)(min(longmemsize, 64000));
            SETSQLDALONGLEN(sqldaPointer, idx, memsize);
#else
            memsize = longmemsize;
#endif

            sqldaPointer->sqlvar[idx].sqldata = (char *SQL_POINTER)
                malloc(memsize);
            break;
        case SQL_TYP_VARCHAR:
        case SQL_TYP_NVARCHAR:
        case SQL_TYP_LONG:
        case SQL_TYP_NLONG:
        case SQL_TYP_DATE:
        case SQL_TYP_NDATE:
        case SQL_TYP_TIME:
        case SQL_TYP_NTIME:
        case SQL_TYP_STAMP:
        case SQL_TYP_NSTAMP:
            sqldaPointer->sqlvar[idx].sqltype = SQL_TYP_NCSTR;
            sqldaPointer->sqlvar[idx].sqldata = (char *SQL_POINTER)
                malloc((sqldaPointer->sqlvar[idx].sqllen));
            memsize = (sqldaPointer->sqlvar[idx].sqllen);
            break;
        case SQL_TYP_DECIMAL:
        case SQL_TYP_NDECIMAL:
            precision = ((char *)&(sqldaPointer->sqlvar[idx].sqllen))[0];
            sqldaPointer->sqlvar[idx].sqldata = (char *SQL_POINTER)
                malloc((precision + 2) / 2);
            memsize = (precision + 2) / 2;
            break;

        default:
            sqldaPointer->sqlvar[idx].sqldata = (char *SQL_POINTER)
                malloc(sqldaPointer->sqlvar[idx].sqllen);
            memsize = sqldaPointer->sqlvar[idx].sqllen;
            break;
        } /* endswitch */

        if (sqldaPointer->sqlvar[idx].sqldata == NULL)
        {
            fprintf(stderr, "\nalloc host vars: out of memory error.\n");
            exit(-1);
        }
        else
        {
            memset(sqldaPointer->sqlvar[idx].sqldata, '\0', memsize);
        } /* endif */

        /*
        ** If the SQLTYPE is odd, the host var requires a null indicator variable
        ** this is needed to allocate memory for "sqlind" which is used in the
        ** "display_da" procedure when detecting whether or not the host indicator
        ** is NULLable or not.
        */
        if (sqldaPointer->sqlvar[idx].sqltype & 1)
        {
            /* Allocate storage for short int */
            sqldaPointer->sqlvar[idx].sqlind = (short *)malloc(sizeof(short));
            /* Detect memory allocation error */
            if (sqldaPointer->sqlvar[idx].sqlind == NULL)
            {
                fprintf(stderr, "Out of dynamic memory while "
                                "allocating for your select statement\n");
                exit(-1);
            }
            else
            {
                /* initialize memory to zero */
                memset(sqldaPointer->sqlvar[idx].sqldata, '\0', sizeof(short));
            } /* endif */
        }     /* endif */
    }         /* endfor */
}

/*******************************************************************************
 * FUNCTION : free_da
 * This function frees up the memory that has been allocated for the use of
 *  an SQLDA data structure.
 *******************************************************************************/
void free_da(struct sqlda *sqldaPointer)
{
    short idx;
    for (idx = 0; idx < sqldaPointer->sqld; idx++)
    {
        free(sqldaPointer->sqlvar[idx].sqldata);
        if (sqldaPointer->sqlvar[idx].sqltype & 1)
        {
            free(sqldaPointer->sqlvar[idx].sqlind);
        }
    } /* endfor */
    free(sqldaPointer);
}

/*******************************************************************************
 * PROCEDURE : print_var
 * The following procedure prints out the SQLDA SQLVAR variables.
 * The procedure first determines which type the data is and then goes through
 *  the appropriate sub-routines that are required to print out the data.
 *
 *******************************************************************************/
void print_var(char *ptr, int type, short collen, short datalen)
{
    short idx, ind; /* Array idx variables */
    /* Variables for decoding packed decimal data */
    short bottom, point;
    unsigned short top, precision, scale;

    short *iptr;  /* Pointer for short integer data. */
    char blen;    /* Single byte length variable */
    long *lptr;   /* Pointer for long integer data */
    double *dptr; /* Pointer for float data */
    short maxCollen = max(collen, datalen);

    maxCollen = max(maxCollen, 5); /* setting a minimum column length */
    /*
    ** Determine the type of data, coerce or decode the data for output
    ** if necessary, then output the data.
    */

    switch (type)
    {
    case SQL_TYP_INTEGER:  /* long */
    case SQL_TYP_NINTEGER: /* long with null indicator */
        lptr = (long *)ptr;
        printf("%*ld", maxCollen - 1, *lptr);
        break;
    case SQL_TYP_SMALL:  /* short */
    case SQL_TYP_NSMALL: /* short with null indicator */
        iptr = (short *)ptr;
        printf("%*d", maxCollen, *iptr);
        break;
    case SQL_TYP_DECIMAL:  /* decimal */
    case SQL_TYP_NDECIMAL: /* decimal with null indicator */
                           /* Determine the scale and precision */
        precision = ((char *)&(maxCollen))[0];
        scale = ((char *)&(maxCollen))[1];

        /*****************************************************************************/
        /* Note:  Precision can only be odd because internally only odd are stored.  */
        /*        When and if it happens that an even precision can occur, the       */
        /*        precision must be incremented by 1 in order for the proper         */
        /*        calculation of "idx" and "point" to occur, and the correct value */
        /*        outputted.                                                         */
        /*****************************************************************************/
        if ((precision % 2) == 0)
            precision += 1;

        /* Calculate the total number of bytes */
        idx = (short)(precision + 2) / 2;
        point = precision - scale;

        /* Determine the sign */
        bottom = *(ptr + idx - 1) & 0x000F; /* sign */
        if ((bottom == 0x000D) || (bottom == 0x000B))
        {
            printf("-");
        }
        else
        {
            printf(" ");
        }

        /* Decode and print the decimal number */
        for (ind = 0; ind < idx; ind++)
        {
            top = *(ptr + ind) & 0x00F0;
            top = (top >> 4);
            bottom = *(ptr + ind) & 0x000F;
            if (point-- == 0)
                printf(".");
            printf("%d", top);
            /*****************************************************************************/
            /* Ignore bottom of last half byte because its the sign.                     */
            /*****************************************************************************/
            if (ind < idx - 1)
            { /* sign half byte ? */
                if (point-- == 0)
                    printf(".");
                printf("%d", bottom);
            }
        }
        if (scale == 0)
            printf(".");
        break;

    case SQL_TYP_FLOAT:  /* double */
    case SQL_TYP_NFLOAT: /* double with null indicator */
        dptr = (double *)ptr;
        printf("%*.*e", maxCollen, datalen, *dptr);
        break;
    case SQL_TYP_CHAR:  /* fixed length character string */
    case SQL_TYP_NCHAR: /* fixed length character string with null
                           indicator */
        printf("%-*.*s", maxCollen, datalen, ptr);
        break;

    case SQL_TYP_LSTR:  /* varying length character string, 1-byte length */
    case SQL_TYP_NLSTR: /* varying length character string, 1-byte length,
                           with null indicator */

        /*
        ** Initialize blen to the value the length field in the
        ** varchar data structure.
        */
        blen = *ptr;

        /* Advance the data pointer beyond the length field */
        ptr += sizeof(char);
        for (idx = 0; (idx < (short)blen) && (idx < maxCollen);
             idx++)
            printf("%c", *(ptr + idx));
        break;
    case SQL_TYP_CSTR:  /* null terminated varying length character string */
    case SQL_TYP_NCSTR: /* null terminate varying length character
                            string with null indicator */
        /*
        ** Advance the data pointer beyond the length field and print the data.
        */
        printf("%-*.*s", maxCollen, datalen, ptr);
        break;
    default:
        /* Unknown data type */
        printf("%-*.*s", maxCollen, datalen, "UNKNOWN");
        break;
    }
    printf(" ");
}

/*******************************************************************************
 * PROCEDURE : display_col_titles
 * The following procedure displays the column names, which is available from
 *  the SQLDA data structure.  This is then passed on to "print_var" to be
 *  outputted on to the screen.
 *******************************************************************************/
void display_col_titles(struct sqlda *sqldaPointer)
{
    short sqlvarIndex, numBytes;

    for (sqlvarIndex = 0; sqlvarIndex < sqldaPointer->sqld; sqlvarIndex++)
    {
        switch (sqldaPointer->sqlvar[sqlvarIndex].sqltype)
        {
        case SQL_TYP_DECIMAL:
        case SQL_TYP_NDECIMAL:
            numBytes = max(sqldaPointer->sqlvar[sqlvarIndex].sqlname.length,
                           ((char *)&(sqldaPointer->sqlvar[sqlvarIndex].sqllen))[0] + 2);
            break;
        case SQL_TYP_BLOB:
        case SQL_TYP_NBLOB:
        case SQL_TYP_DBCLOB:
        case SQL_TYP_NDBCLOB:
            numBytes = LOBLENGTH;
            break;
        case SQL_TYP_CLOB:
        case SQL_TYP_NCLOB:
            numBytes = LOBLENGTH;
            break;
        case SQL_TYP_GRAPHIC:
        case SQL_TYP_NGRAPHIC:
            break;
        default:
            numBytes = max(sqldaPointer->sqlvar[sqlvarIndex].sqlname.length,
                           sqldaPointer->sqlvar[sqlvarIndex].sqllen);
            break;
        } /* endswitch */
        print_var(sqldaPointer->sqlvar[sqlvarIndex].sqlname.data,
                  SQL_TYP_CSTR, numBytes,
                  sqldaPointer->sqlvar[sqlvarIndex].sqlname.length);
    }
    printf("\n");
}

/*******************************************************************************
 * PROCEDURE : display_da
 * The following procedure displays the output of pointer which has been
 *  passed through.  All pertinent information on the structure of the outputted
 *  data is available from this pointer, and is further examined in the
 *  procedure "print_var".
 *******************************************************************************/
void display_da(struct sqlda *sqldaPointer)
{
    short numBytes, sqlvarIndex;
    struct lob
    {
        long length;
        char *data;
    } * lobPointer;

    /* Output the contents for all host variables */
    for (sqlvarIndex = 0; sqlvarIndex < sqldaPointer->sqld; sqlvarIndex++)
    {
        numBytes = sqldaPointer->sqlvar[sqlvarIndex].sqllen;

        if (sqldaPointer->sqlvar[sqlvarIndex].sqltype & 1 &&
            *(sqldaPointer->sqlvar[sqlvarIndex].sqlind) < 0)
        {
            /* the data in the sqlda is NULL */
            switch (sqldaPointer->sqlvar[sqlvarIndex].sqltype)
            {
            case SQL_TYP_NBLOB:
            case SQL_TYP_NDBCLOB:
                printf("LOB length = NULL       | ");
                break;
            case SQL_TYP_NCLOB:
                print_var("NULL CLOB", SQL_TYP_CSTR, LOBLENGTH, LOBLENGTH);
                break;
            case SQL_TYP_NGRAPHIC:
                break;
            case SQL_TYP_NDECIMAL:
                numBytes = max(sqldaPointer->sqlvar[sqlvarIndex].sqlname.length,
                               ((char *)&(sqldaPointer->sqlvar[sqlvarIndex].sqllen))[0] + 2);

                print_var("-", SQL_TYP_CSTR, numBytes,
                          sqldaPointer->sqlvar[sqlvarIndex].sqlname.length);
                break;
            default:
                /* Output the data in the sqlda */
                print_var(sqldaPointer->sqlvar[sqlvarIndex].sqldata,
                          sqldaPointer->sqlvar[sqlvarIndex].sqltype,
                          sqldaPointer->sqlvar[sqlvarIndex].sqlname.length,
                          numBytes);
                break;
            } /* endswitch */
        }
        else
        { /* output the data in the sqlda */
            switch (sqldaPointer->sqlvar[sqlvarIndex].sqltype)
            {
            case SQL_TYP_CLOB:
            case SQL_TYP_NCLOB:
            case SQL_TYP_BLOB:
            case SQL_TYP_NBLOB:
            case SQL_TYP_DBCLOB:
            case SQL_TYP_NDBCLOB:
                lobPointer = (struct lob *)(sqldaPointer->sqlvar[sqlvarIndex].sqldata);
                if (*sqldaPointer->sqlvar[sqlvarIndex].sqlind == 0)
                {
                    printf("LOB length = %10ld       | ", lobPointer->length);
                }
                else
                {
                    /* the sqlvar LOB has been truncated */
                    printf("LOB length = %10ld trunc.| ", lobPointer->length);
                } /* endif */
                break;
            case SQL_TYP_GRAPHIC:
            case SQL_TYP_NGRAPHIC:
                printf("GRAPHIC length = %5ld | ",
                       sqldaPointer->sqlvar[sqlvarIndex].sqlname.length);
                break;
            default:
                /* Output the data in the sqlda */
                print_var(sqldaPointer->sqlvar[sqlvarIndex].sqldata,
                          sqldaPointer->sqlvar[sqlvarIndex].sqltype,
                          sqldaPointer->sqlvar[sqlvarIndex].sqlname.length,
                          numBytes);
                break;
            } /* endswitch */
        }     /* endif */
    }         /* endfor */
    printf("\n");
}
/* COMMENT OUT OFF */

/******************************************************************************
 *  init_error_log -- This routine prepares the buffer for recording error
 *                    occurrences.
 ******************************************************************************/
void init_error_log(void)
{
    long i = 0; /* Loop counter */

    if ((errdata = (short *)calloc(MAXERRORS, sizeof(short))) == NULL)
        mem_error("creating error buffer");
    for (i = 0; i < MAXERRORS; i++)
        errdata[i] = 0;
    return;
} /* end init_error_log */

/******************************************************************************
 *  sqlerror -- This routine displays the error information contained in
 *              the SQLCA and logs the changes in the error buffer (if
 *              allocated).
 ******************************************************************************/
void sqlerror(struct sqlca *sqlca)
{
    char errbuf[500]; /* Hold error message */
    long i, j;        /* Loop counters */
    int rc;           /* Return code from sqlaintp() */

    if (sqlca->sqlcode != 0 || sqlca->sqlwarn[0] != ' ')
    {
        if (sqlca->sqlcode == 0)
            printf("SQL Informational Message:\n");
        else if (sqlca->sqlcode < 0)
            printf("SQL Error:\n");
        else
            printf("SQL Warning:\n");

        printf("\n                        SQLCA Dump (SQLCODE = %d)\n", sqlca->sqlcode);
        printf(" ---------------------------------------------------------------------------\n");
        for (i = 0; i < SQLCA_SIZE; i += 20)
        {
            printf("  ");
            for (j = 0; j < 20; j++)
            {
                if (i + j < SQLCA_SIZE)
                    printf("%02X", ((unsigned char *)sqlca)[i + j]);
                else
                    printf("  ");

                if (j % 2)
                    printf(" ");
            } /* end for */
            printf("| ");
            for (j = 0; j < 20; j++)
            {
                if (i + j < SQLCA_SIZE)
                    printf("%c", isprint(((char *)sqlca)[i + j]) ? ((char *)sqlca)[i + j] : '.');
                else
                    printf(" ");
            } /* end for */
            printf("\n");
        } /* end for */
        printf(" ---------------------------------------------------------------------------\n\n");

        if (errdata != NULL && sqlca->sqlcode >= -32768 && sqlca->sqlcode <= 32767)
            errdata[MAXERRORS / 2 + sqlca->sqlcode]++;

        printf("\tSQLCODE:\t%ld\n", sqlca->sqlcode);
        printf("\tSQLERRP:\t");
        for (i = 0; i < 8; i++)
            printf("%c", sqlca->sqlerrp[i]);
        printf("\n");
        for (i = 0; i < 6; i++)
            if (sqlca->sqlerrd[i])
                printf("\tSQLERRD%d:\t%d\n", i + 1, sqlca->sqlerrd[i]);
        for (i = 0; i < 11; i++)
            if (sqlca->sqlwarn[i] != ' ')
                printf("\tSQLWARN%c:\t%c\n", '0' + i, sqlca->sqlwarn[i]);
        printf("\tSQLSTATE:\t");
        for (i = 0; i < 5; i++)
            printf("%c", sqlca->sqlstate[i]);
        printf("\n");

        if (sqlca->sqlcode != 0)
        {
            switch (rc = sqlaintp(errbuf, 500, 0, sqlca))
            {
            case -1:
                printf("SQCERROR--Insufficient memory to get error message.\n");
                printf("\tSQLERRMC:\t%s\n", errbuf);
                sprintf(errbuf, "sqlaintp=%d", rc);
                return;
                break;
            case -2:
                printf("SQCERROR--No error.\n");
                printf("\tSQLERRMC:\t%s\n", errbuf);
                sprintf(errbuf, "sqlaintp=%d", rc);
                break;
            case -3:
                printf("SQCERROR--SQLCA does not reference a valid error.\n");
                printf("\tSQLERRMC:\t%s\n", errbuf);
                sprintf(errbuf, "sqlaintp=%d", rc);
                return;
                break;
            case -4:
                printf("SQCERROR--The line width is less than 0.\n");
                printf("\tSQLERRMC:\t%s\n", errbuf);
                sprintf(errbuf, "sqlaintp=%d", rc);
                return;
                break;
            case -5:
                printf("SQCERROR--The SQLCA is invalid.\n");
                printf("\tSQLERRMC:\t%s\n", errbuf);
                sprintf(errbuf, "sqlaintp=%d", rc);
                return;
                break;
            default:
                if (rc < 0)
                {
                    printf("SQCERROR--Unknown return code from sqlaintp.  (rc = %d)\n", rc);
                    printf("\tSQLERRMC:\t%s\n", errbuf);
                    sprintf(errbuf, "sqlaintp=%d", rc);
                    return;
                } /* end if */
                break;
            } /* end switch */
            printf("\tSQLERRMC:\t%s\n", errbuf);
        }
        else
        {
            printf("\tSQLERRMC:\tn/a\n");
        } /* end if */
    }     /* end if */
    return;
} /* end sqlerror */

/******************************************************************************
 *  display_errors -- This routine displays a summary of the errors in the log.
 ******************************************************************************/
void display_errors(void)
{
    long i; /* Loop counter */

    if (errdata != NULL)
    {
        printf("\n\n===========================================\n");
        printf("               Error Summary\n");
        printf("===========================================\n\n");
        printf("              SQLCODE| Count\n");
        printf("              -------+-------\n");
        for (i = 0; i < MAXERRORS; i++)
            if (errdata[i])
                printf("              %6ld | %5d\n", i - MAXERRORS / 2, errdata[i]);
        printf("              ---------------\n");
    } /* end if */
    return;
} /* end display_errors */

/******************************************************************************
 *  free_error_log -- This routine releases the buffer for recording error
 *                    occurrences.
 ******************************************************************************/
void free_error_log(void)
{
    long i = 0; /* Loop counter */
    free(errdata);

    return;
} /* end free_error_log */
