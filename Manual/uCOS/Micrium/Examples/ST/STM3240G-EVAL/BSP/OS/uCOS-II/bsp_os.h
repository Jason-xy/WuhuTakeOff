/*
*********************************************************************************************************
*                                            EXAMPLE CODE
*
*               This file is provided as an example on how to use Micrium products.
*
*               Please feel free to use any application code labeled as 'EXAMPLE CODE' in
*               your application products.  Example code may be used as is, in whole or in
*               part, or may be used as a reference only. This file can be modified as
*               required to meet the end-product requirements.
*
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*
*               You can find our product's user manual, API reference, release notes and
*               more information at https://doc.micrium.com.
*               You can contact us at www.micrium.com.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                    MICRIUM BOARD SUPPORT PACKAGE
*                                               OS LAYER
*
* Filename      : bsp_os.h
* Version       : V1.00
* Programmer(s) : FT
*                 EHS
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                                 MODULE
*
* Note(s) : (1) This header file is protected from multiple pre-processor inclusion through use of the
*               BSP present pre-processor macro definition.
*********************************************************************************************************
*/

#ifndef  BSP_OS_PRESENT
#define  BSP_OS_PRESENT


/*
*********************************************************************************************************
*                                              INCLUDE FILES
*********************************************************************************************************
*/

#include  <ucos_ii.h>


/*
*********************************************************************************************************
*                                                 EXTERNS
*********************************************************************************************************
*/

#ifdef   BSP_OS_MODULE
#define  BSP_OS_EXT
#else
#define  BSP_OS_EXT  extern
#endif


/*
*********************************************************************************************************
*                                               DEFINES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                             DATA TYPES
*********************************************************************************************************
*/

typedef   OS_EVENT    *BSP_OS_SEM;
typedef   CPU_INT16U   BSP_OS_SEM_VAL;


/*
*********************************************************************************************************
*                                          GLOBAL VARIABLES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                               MACRO'S
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            FUNCTION PROTOTYPES
*********************************************************************************************************
*/

CPU_BOOLEAN  BSP_OS_SemCreate          (BSP_OS_SEM     *p_sem,
                                        BSP_OS_SEM_VAL  sem_val,
                                        CPU_CHAR       *p_sem_name);

CPU_BOOLEAN  BSP_OS_SemWait            (BSP_OS_SEM     *p_sem,
                                        CPU_INT32U      dly_ms);

CPU_BOOLEAN  BSP_OS_SemPost            (BSP_OS_SEM     *p_sem);

void         BSP_OS_TimeDlyMs          (CPU_INT32U      dly_ms);

void         BSP_OS_TimeDly            (CPU_INT32U      dly_tick);
/*
*********************************************************************************************************
*                                          CONFIGURATION ERRORS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                             MODULE END
*********************************************************************************************************
*/

#endif                                                          /* End of module include.                               */
