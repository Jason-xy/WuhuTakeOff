/*
*********************************************************************************************************
*                                     SERIAL (BYTE) COMMUNICATION
*
*                         (c) Copyright 2007-2009; Micrium, Inc.; Weston, FL
*
*               All rights reserved.  Protected by international copyright laws.
*               Knowledge of the source code may NOT be used to develop a similar product.
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                     SERIAL (BYTE) COMMUNICATION
*
*                                         SERIAL LINE DRIVER
*                                       TELETYPE TERMINAL (TTY)
*
* Filename      : serial_line_tty.h
* Version       : V2.00.01
* Programmer(s) : FGK
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                               MODULE
*
* Note(s) : (1) This header file is protected from multiple pre-processor inclusion through use of the
*               SERIAL_LINE_TTY present pre-processor macro definition.
*********************************************************************************************************
*/

#ifndef   SERIAL_LINE_TTY_PRESENT
#define   SERIAL_LINE_TTY_PRESENT


/*
*********************************************************************************************************
*                                               EXTERNS
*********************************************************************************************************
*/

#ifdef    SERIAL_LINE_TTY_MODULE
#define   SERIAL_LINE_TTY_EXT
#else
#define   SERIAL_LINE_TTY_EXT  extern
#endif


/*
*********************************************************************************************************
*                                          GLOBAL VARIABLES
*********************************************************************************************************
*/

extern  SERIAL_LINE_DRV_API  SerialLine_TTY;                    /* TTY serial line driver.                              */


/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/

                                                                /* ------------------- ECHO CONTROL ------------------- */
void  SerialLine_TTY_EchoRx(SERIAL_IF_NBR  if_nbr,              /* Set rx echo.                                         */
                            CPU_BOOLEAN    echo);

void  SerialLine_TTY_EchoTx(SERIAL_IF_NBR  if_nbr,              /* Set tx echo.                                         */
                            CPU_BOOLEAN    echo);


/*
*********************************************************************************************************
*                                             MODULE END
*********************************************************************************************************
*/

#endif                                                          /* End of SERIAL_LINE_TTY module include.               */
