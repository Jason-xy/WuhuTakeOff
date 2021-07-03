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
*                                                PROBE
*
* Filename      : serial_line_probe.h
* Version       : V2.00.01
* Programmer(s) : FGK
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                               MODULE
*
* Note(s) : (1) This header file is protected from multiple pre-processor inclusion through use of the
*               SERIAL_LINE_PROBE present pre-processor macro definition.
*********************************************************************************************************
*/

#ifndef   SERIAL_LINE_PROBE_PRESENT
#define   SERIAL_LINE_PROBE_PRESENT


/*
*********************************************************************************************************
*                                               EXTERNS
*********************************************************************************************************
*/

#ifdef    SERIAL_LINE_PROBE_MODULE
#define   SERIAL_LINE_PROBE_EXT
#else
#define   SERIAL_LINE_PROBE_EXT  extern
#endif


/*
*********************************************************************************************************
*                                          GLOBAL VARIABLES
*********************************************************************************************************
*/

extern  SERIAL_LINE_DRV_API  SerialLine_Probe;                  /* Probe serial line driver.                            */


/*
*********************************************************************************************************
*                                             MODULE END
*********************************************************************************************************
*/

#endif                                                          /* End of SERIAL_LINE_PROBE module include.             */
