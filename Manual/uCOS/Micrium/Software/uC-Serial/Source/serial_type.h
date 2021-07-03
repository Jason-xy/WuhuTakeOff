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
* Filename      : serial_type.h
* Version       : V2.00.01
* Programmer(s) : FGK
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                               MODULE
*
* Note(s) : (1) This header file is protected from multiple pre-processor inclusion through use of the
*               SERIAL_TYPE present pre-processor macro definition.
*********************************************************************************************************
*/

#ifndef   SERIAL_TYPE_PRESENT
#define   SERIAL_TYPE_PRESENT


/*
*********************************************************************************************************
*                                        FORWARD DECLARATIONS
*********************************************************************************************************
*/

typedef         struct  serial_dev           SERIAL_DEV;
typedef         struct  serial_if_cfg        SERIAL_IF_CFG;
typedef  const  struct  serial_line_drv_api  SERIAL_LINE_DRV_API;


/*
*********************************************************************************************************
*                                             DATA TYPES
*********************************************************************************************************
*/

typedef  CPU_INT08U  SERIAL_IF_NBR;


/*
*********************************************************************************************************
*                                             MODULE END
*********************************************************************************************************
*/

#endif                                                          /* End of SERIAL_TYPE module include.                   */
