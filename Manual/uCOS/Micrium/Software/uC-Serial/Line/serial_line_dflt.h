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
*                                               DEFAULT
*
* Filename      : serial_line_dflt.h
* Version       : V2.00.01
* Programmer(s) : FGK
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                               MODULE
*
* Note(s) : (1) This header file is protected from multiple pre-processor inclusion through use of the
*               SERIAL_LINE_DFLT present pre-processor macro definition.
*********************************************************************************************************
*/

#ifndef   SERIAL_LINE_DFLT_PRESENT
#define   SERIAL_LINE_DFLT_PRESENT


/*
*********************************************************************************************************
*                                               EXTERNS
*********************************************************************************************************
*/

#ifdef    SERIAL_LINE_DFLT_MODULE
#define   SERIAL_LINE_DFLT_EXT
#else
#define   SERIAL_LINE_DFLT_EXT  extern
#endif


/*
*********************************************************************************************************
*                                          GLOBAL VARIABLES
*********************************************************************************************************
*/

extern  SERIAL_LINE_DRV_API  SerialLine_Dflt;                   /* Default serial line driver.                          */


/*
*********************************************************************************************************
*                                             MODULE END
*********************************************************************************************************
*/

#endif                                                          /* End of SERIAL_LINE_DFLT module include.              */
