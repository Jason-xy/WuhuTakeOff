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
*                                          Micrium uC/OS-II
*
* Filename      : serial_os.c
* Version       : V2.00.01
* Programmer(s) : FGK
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#include  <serial.h>
#include  <serial_os.h>
#include  <ucos_ii.h>

/*
*********************************************************************************************************
*                                              DEFINES
*********************************************************************************************************
*/

#define  SERIAL_OS_MAX_NBR_SEM                  ((4u + SERIAL_CFG_TX_DESC_NBR) * SERIAL_CFG_MAX_NBR_IF)


/*
*********************************************************************************************************
*                                            LOCAL TABLES
*********************************************************************************************************
*/

static  OS_EVENT  *OSEventObj[SERIAL_OS_MAX_NBR_SEM];

/*
*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/

static  CPU_SIZE_T  OSEventObjIx;


/*$PAGE*/
/*
*********************************************************************************************************
*                                           SerialOS_Init()
*
* Description : Initialize OS layer.
*
* Argument(s) : perr        Pointer to variable that will receive the return error code from this function :
*
*                               SERIAL_OS_ERR_NONE          Initialization of OS layer successful.
*                               SERIAL_OS_ERR_INIT          Initialization of OS layer failed.
*
* Return(s)   : None.
*
* Caller(s)   : Serial_Init().
*
* Note(s)     : None.
*********************************************************************************************************
*/

void  SerialOS_Init (SERIAL_ERR  *perr)
{
    CPU_SIZE_T   ix;
    OS_EVENT    *psem;


    for (ix = 0; ix < SERIAL_OS_MAX_NBR_SEM; ix++) {
        psem = OSSemCreate(0);
        if (psem != (OS_EVENT *)0) {
            OSEventObj[ix] = psem;
        } else {
           *perr = SERIAL_OS_ERR_INIT;
            return;
        }
    }

    OSEventObjIx = SERIAL_OS_MAX_NBR_SEM;
   *perr         = SERIAL_OS_ERR_NONE;
}


/*$PAGE*/
/*
*********************************************************************************************************
*                                        SerialOS_SemCreate()
*
* Description : Create a semaphore.
*
* Argument(s) : cnt         Initial value for the semaphore.
*
* Return(s)   : Pointer to semaphore, if NO errors.
*               Pointer to NULL,      otherwise.
*
* Caller(s)   : Serial_Wr().
*
* Note(s)     : None.
*********************************************************************************************************
*/

void  *SerialOS_SemCreate (CPU_INT16U  cnt)
{
    OS_EVENT  *psem;
    INT8U      os_err;
    CPU_SR_ALLOC();


    CPU_CRITICAL_ENTER();
    if (OSEventObjIx < 1u) {
        CPU_CRITICAL_EXIT();
        return ((void *)0);
    }

    if (OSEventObjIx > SERIAL_OS_MAX_NBR_SEM) {
        CPU_CRITICAL_EXIT();
        return ((void *)0);
    }

                                                                /* -------------- GET OS EVENT FROM POOL -------------- */
    OSEventObjIx--;
    psem = OSEventObj[OSEventObjIx];
    CPU_CRITICAL_EXIT();

    OSSemSet(psem, cnt, &os_err);

    return ((void *)psem);
}


/*$PAGE*/
/*
*********************************************************************************************************
*                                          SerialOS_SemDel()
*
* Description : Delete a semaphore.
*
* Argument(s) : psem        Pointer to semaphore to delete.
*
* Return(s)   : None.
*
* Caller(s)   : Serial_WrCallback().
*
* Note(s)     : (1) Always delete the semaphore even if tasks are pending on it.
*
*               (2) MUST NOT be called within a critical section.
*********************************************************************************************************
*/

void  SerialOS_SemDel (void  *psem)
{
    INT8U       os_err;
#if (SERIAL_CFG_ARG_CHK_EXT_EN == DEF_ENABLED)
    CPU_SIZE_T  i;
#endif
    CPU_SR_ALLOC();


    OSSemPendAbort((OS_EVENT *) psem,
                   (INT8U     ) OS_PEND_OPT_BROADCAST,
                   (INT8U    *)&os_err);

    CPU_CRITICAL_ENTER();
    if (OSEventObjIx >= SERIAL_OS_MAX_NBR_SEM) {
        CPU_CRITICAL_EXIT();
        return;
    }

#if (SERIAL_CFG_ARG_CHK_EXT_EN == DEF_ENABLED)                  /* ----------------- VALIDATE OS EVENT ---------------- */
    for (i = 0u; i < OSEventObjIx; i++) {
        if (OSEventObj[i] == (OS_EVENT *)psem) {
            CPU_CRITICAL_EXIT();
            return;
        }
    }
#endif
                                                                /* --------------- FREE OS EVENT TO POOL -------------- */
    OSEventObj[OSEventObjIx] = (OS_EVENT *)psem;
    OSEventObjIx++;

    CPU_CRITICAL_EXIT();
}


/*$PAGE*/
/*
*********************************************************************************************************
*                                         SerialOS_SemWait()
*
* Description : Wait for a semaphore to become available.
*
* Argument(s) : psem        Pointer to semaphore to wait.
*
*               timeout_ms  Optional timeout period in milliseconds. If specified 0, wait indefinitely.
*
*               perr        Pointer to variable that will receive the return error code from this function :
*
*                               SERIAL_OS_ERR_NONE          The wait on semaphore was successful.
*                               SERIAL_OS_ERR_TIMEOUT       Semaphore not recv'd within specified 'timeout'.
*                               SERIAL_OS_ERR_SIGNAL        Waiting on semaphore could not be performed.
*
* Return(s)   : None.
*
* Caller(s)   : Serial_Wr().
*
* Note(s)     : None.
*********************************************************************************************************
*/

void  SerialOS_SemWait (void        *psem,
                        CPU_INT32U   timeout_ms,
                        SERIAL_ERR  *perr)
{
    INT32U  ticks;
    INT8U   os_err;


    ticks = (timeout_ms * OS_TICKS_PER_SEC + (DEF_TIME_NBR_mS_PER_SEC - 1u)) / DEF_TIME_NBR_mS_PER_SEC;

    OSSemPend((OS_EVENT *) psem,
              (INT32U    ) ticks,
              (INT8U    *)&os_err);

    switch (os_err) {
        case OS_ERR_NONE:
            *perr = SERIAL_OS_ERR_NONE;
             break;


        case OS_ERR_TIMEOUT:
            *perr = SERIAL_OS_ERR_TIMEOUT;
             break;


        default:
            *perr = SERIAL_OS_ERR_SIGNAL;
             break;
    }
}


/*$PAGE*/
/*
*********************************************************************************************************
*                                        SerialOS_SemTryLock()
*
* Description : Try to acquire semaphore if available.
*
* Argument(s) : psem        Pointer to semaphore to acquire.
*
* Return(s)   : DEF_OK,   if semaphore acquired.
*               DEF_FAIL, otherwise.
*
* Caller(s)   : Serial_WrAsync().
*
* Note(s)     : None.
*********************************************************************************************************
*/

CPU_BOOLEAN  SerialOS_SemTryLock (void  *psem)
{
    INT16U  cnt;


    cnt = OSSemAccept((OS_EVENT *)psem);
    if (cnt == 0) {
        return (DEF_FAIL);
    } else {
        return (DEF_OK);
    }
}


/*$PAGE*/
/*
*********************************************************************************************************
*                                        SerialOS_SemSignal()
*
* Description : Signal a semaphore.
*
* Argument(s) : psem        Pointer to semaphore to signal.
*
* Return(s)   : DEF_OK,   if semaphore signaled.
*               DEF_FAIL, otherwise.
*
* Caller(s)   : Serial_WrCallback().
*
* Note(s)     : None.
*********************************************************************************************************
*/

CPU_BOOLEAN  SerialOS_SemSignal (void  *psem)
{
    INT8U  os_err;


    os_err = OSSemPost((OS_EVENT *)psem);
    if (os_err == OS_ERR_NONE) {
        return (DEF_OK);
    } else {
        return (DEF_FAIL);
    }
}
