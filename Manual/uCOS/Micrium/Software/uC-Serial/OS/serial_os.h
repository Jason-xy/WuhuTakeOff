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
*                                       OPERATING SYSTEM LAYER
*
* Filename      : serial_os.h
* Version       : V2.00.01
* Programmer(s) : FGK
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/

void          SerialOS_Init      (SERIAL_ERR  *perr);           /* Initialize OS layer.                     */

void         *SerialOS_SemCreate (CPU_INT16U   cnt);            /* Create semaphore.                        */

void          SerialOS_SemDel    (void        *psem);           /* Delete semaphore.                        */

CPU_BOOLEAN   SerialOS_SemTryLock(void        *psem);           /* Try to acquire semaphore if available.   */

void          SerialOS_SemWait   (void        *psem,            /* Wait for semaphore to become available.  */
                                  CPU_INT32U   timeout_ms,
                                  SERIAL_ERR  *perr);

CPU_BOOLEAN   SerialOS_SemSignal (void        *psem);           /* Signal semaphore.                        */
