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
* Filename      : serial.h
* Version       : V2.00.01
* Programmer(s) : FGK
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                               MODULE
*
* Note(s) : (1) This header file is protected from multiple pre-processor inclusion through use of the
*               SERIAL present pre-processor macro definition.
*********************************************************************************************************
*/

#ifndef   SERIAL_PRESENT
#define   SERIAL_PRESENT


/*
*********************************************************************************************************
*                                        SERIAL VERSION NUMBER
*
* Note(s) : (1) (a) The Serial software version is denoted as follows :
*
*                       Vx.yy
*
*                           where
*                                  V     denotes 'Version' label
*                                  x     denotes major software version revision number
*                                  yy    denotes minor software version revision number
*
*               (b) The Serial software version label #define is formatted as follows :
*
*                       ver = x.yy * 100
*
*                           where
*                                  ver   denotes software version number scaled as
*                                        an integer value
*                                  x.yy  denotes software version number
*********************************************************************************************************
*/

#define  SERIAL_VERSION            20001u                         /* See Note #1.                                         */


/*
*********************************************************************************************************
*                                               EXTERNS
*********************************************************************************************************
*/

#ifdef    SERIAL_MODULE
#define   SERIAL_EXT
#else
#define   SERIAL_EXT  extern
#endif


/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#include  <cpu.h>

#include  <lib_def.h>
#include  <lib_ascii.h>
#include  <lib_str.h>
#include  <lib_mem.h>

#include  <app_cfg.h>

#include  <serial_buf.h>
#include  <serial_type.h>
#include  <serial_line_dflt.h>


/*
*********************************************************************************************************
*                                           DEFAULT DEFINES
*********************************************************************************************************
*/

#ifndef  SERIAL_CFG_TX_DESC_NBR                                 /* Number of descriptors in transmit queue.             */
#define  SERIAL_CFG_TX_DESC_NBR                            0
#endif

#ifndef  SERIAL_CFG_ARG_CHK_EXT_EN
#define  SERIAL_CFG_ARG_CHK_EXT_EN                         DEF_DISABLED
#endif

#ifndef  SERIAL_CFG_RD_BUF_EN
#define  SERIAL_CFG_RD_BUF_EN                              DEF_DISABLED
#endif

#ifndef  SERIAL_CFG_WR_BUF_EN
#define  SERIAL_CFG_WR_BUF_EN                              DEF_DISABLED
#endif

#if (LIB_VERSION > 13800u)                                      /* Check used for LIB version 1.38 or greater.          */
#if (LIB_MEM_CFG_HEAP_SIZE > 0u)
#define  LIB_MEM_CFG_ALLOC_EN                      DEF_ENABLED

#else
#define  LIB_MEM_CFG_ALLOC_EN                      DEF_DISABLED
#endif
#endif

/*
*********************************************************************************************************
*                                      SERIAL ERROR CODE DEFINES
*********************************************************************************************************
*/

#define  SERIAL_ERR_NONE                                   0
#define  SERIAL_ERR_NULL_PTR                               1
#define  SERIAL_ERR_NULL_FNCT                              2
#define  SERIAL_ERR_SIGNAL                                 3
#define  SERIAL_ERR_UNDERFLOW                              4
#define  SERIAL_ERR_OVERFLOW                               5
#define  SERIAL_ERR_IF_INVALID_NBR                         6
#define  SERIAL_ERR_BUF_POOL_INIT                          7
#define  SERIAL_ERR_BUF_ALLOC                              8
#define  SERIAL_ERR_MEM_ALLOC                              9
#define  SERIAL_ERR_FAIL                                  10
#define  SERIAL_ERR_TIMEOUT                               11

#define  SERIAL_ERR_DRV_INVALID                           20
#define  SERIAL_ERR_DRV_OPEN                              21
#define  SERIAL_ERR_DRV_IO_ERR                            22

#define  SERIAL_ERR_DEV_DUP_NAME                          30
#define  SERIAL_ERR_DEV_NOT_FOUND                         31
#define  SERIAL_ERR_DEV_ALREADY_OPEN                      32

#define  SERIAL_ERR_IF_NOT_AVAIL                          40
#define  SERIAL_ERR_IF_NOT_OPEN                           41
#define  SERIAL_ERR_IF_IN_USE                             42
#define  SERIAL_ERR_IF_CLOSING                            43

#define  SERIAL_ERR_LINE_END                              50
#define  SERIAL_ERR_LINE_INVALID                          51
#define  SERIAL_ERR_LINE_DRV_OPEN                         52

#define  SERIAL_OS_ERR_NONE                               60
#define  SERIAL_OS_ERR_SIGNAL                             61
#define  SERIAL_OS_ERR_TIMEOUT                            62
#define  SERIAL_OS_ERR_INIT                               63


/*
*********************************************************************************************************
*                                 SERIAL DEVICE CONFIGURATION DEFINES
*********************************************************************************************************
*/

#define  SERIAL_BAUDRATE_110                             110
#define  SERIAL_BAUDRATE_300                             300
#define  SERIAL_BAUDRATE_1200                           1200
#define  SERIAL_BAUDRATE_2400                           2400
#define  SERIAL_BAUDRATE_4800                           4800
#define  SERIAL_BAUDRATE_9600                           9600
#define  SERIAL_BAUDRATE_19200                         19200
#define  SERIAL_BAUDRATE_38400                         38400
#define  SERIAL_BAUDRATE_57600                         57600
#define  SERIAL_BAUDRATE_115200                       115200
#define  SERIAL_BAUDRATE_230400                       230400
#define  SERIAL_BAUDRATE_460800                       460800
#define  SERIAL_BAUDRATE_921600                       921600

#define  SERIAL_DATABITS_5                                 5
#define  SERIAL_DATABITS_6                                 6
#define  SERIAL_DATABITS_7                                 7
#define  SERIAL_DATABITS_8                                 8

#define  SERIAL_STOPBITS_1                                 0
#define  SERIAL_STOPBITS_1_5                               1
#define  SERIAL_STOPBITS_2                                 2

#define  SERIAL_PARITY_NONE                                0
#define  SERIAL_PARITY_ODD                                 1
#define  SERIAL_PARITY_EVEN                                2
#define  SERIAL_PARITY_MARK                                3
#define  SERIAL_PARITY_SPACE                               4

#define  SERIAL_FLOW_CTRL_NONE                             0
#define  SERIAL_FLOW_CTRL_HARDWARE                         1


/*
*********************************************************************************************************
*                                          ISR TYPE DEFINES
*********************************************************************************************************
*/

#define  SERIAL_ISR_TYPE_UNKNOWN                           0u
#define  SERIAL_ISR_TYPE_RX                                1u
#define  SERIAL_ISR_TYPE_RX_OVERRUN                        2u
#define  SERIAL_ISR_TYPE_TX_RDY                            3u
#define  SERIAL_ISR_TYPE_TX_COMPLETE                       4u
#define  SERIAL_ISR_TYPE_ERR                               5u


/*
*********************************************************************************************************
*                                        SERIAL IF NBR DEFINES
*********************************************************************************************************
*/

#define  SERIAL_IF_NBR_MAX_VAL                             DEF_INT_08U_MAX_VAL
#define  SERIAL_IF_NBR_MIN_VAL                             DEF_INT_08U_MIN_VAL

#define  SERIAL_IF_NBR_NONE                                SERIAL_IF_NBR_MAX_VAL
#define  SERIAL_IF_NBR_MIN                                 SERIAL_IF_NBR_MIN_VAL
#define  SERIAL_IF_NBR_MAX                                (SERIAL_IF_NBR_MAX_VAL - 1)
#define  SERIAL_IF_NBR_TOT                                (SERIAL_IF_NBR_MIN_VAL + SERIAL_CFG_MAX_NBR_IF)


/*
*********************************************************************************************************
*                                             DATA TYPES
*********************************************************************************************************
*/

typedef  CPU_INT16U  SERIAL_ERR;


/*
*********************************************************************************************************
*                            SERIAL INTERFACE DEVICE DRIVER API DATA TYPE
*********************************************************************************************************
*/

typedef  const  struct  serial_drv_api {
    void        (*Init)         (SERIAL_ERR     *perr);         /* Driver initialization.                               */

    void        (*Open)         (SERIAL_DEV     *pdev,          /* Open  device driver.                                 */
                                 SERIAL_IF_CFG  *pcfg,
                                 SERIAL_ERR     *perr);

    void        (*Close)        (SERIAL_DEV     *pdev,          /* Close device driver.                                 */
                                 SERIAL_ERR     *perr);

    void        (*RxStart)      (SERIAL_DEV     *pdev,          /* Start receiver.                                      */
                                 SERIAL_ERR     *perr);

    void        (*RxStop)       (SERIAL_DEV     *pdev,          /* Stop  receiver.                                      */
                                 SERIAL_ERR     *perr);

    void        (*RxOctet)      (SERIAL_DEV     *pdev,          /* Receive octet from device.                           */
                                 CPU_INT08U     *pdatum,
                                 SERIAL_ERR     *perr);

    void        (*TxStart)      (SERIAL_DEV     *pdev,          /* Start transmitter.                                   */
                                 SERIAL_ERR     *perr);

    void        (*TxStop)       (SERIAL_DEV     *pdev,          /* Stop  transmitter.                                   */
                                 SERIAL_ERR     *perr);

    void        (*TxOctet)      (SERIAL_DEV     *pdev,          /* Transmit octet with device.                          */
                                 CPU_INT08U      datum,
                                 SERIAL_ERR     *perr);

    void        (*ISR_Handler)  (SERIAL_DEV     *pdev,          /* ISR handler.                                         */
                                 CPU_INT08U      type);
} SERIAL_DRV_API;


/*
*********************************************************************************************************
*                              SERIAL INTERFACE DEVICE BSP API DATA TYPE
*********************************************************************************************************
*/

typedef  const  struct  serial_bsp_api {
    void        (*ClkEn)        (SERIAL_ERR   *perr);           /* En  device/peripheral clk.                           */

    void        (*ClkDis)       (SERIAL_ERR   *perr);           /* Dis device/peripheral clk.                           */

    void        (*CfgGPIO)      (CPU_BOOLEAN   flow_ctrl,       /* Cfg device/peripheral GPIO.                          */
                                 SERIAL_ERR   *perr);

    void        (*CfgInt)       (SERIAL_DEV   *pdev,            /* Cfg device/peripheral int ctrl.                      */
                                 SERIAL_ERR   *perr);

    CPU_INT32U  (*ClkFreqGet)   (void);                         /* Get device/peripheral clk freq.                      */
} SERIAL_BSP_API;


/*
*********************************************************************************************************
*                           SERIAL INTERFACE DEVICE CONFIGURATION DATA TYPE
*********************************************************************************************************
*/

typedef  struct  serial_dev_cfg {
    void                 *BaseAddr;                             /* Device registers' base address.                      */
    SERIAL_DRV_API       *Drv_API;                              /* Device driver API.                                   */

    SERIAL_BSP_API       *BSP_API;                              /* Optional device BSP API. Dependent on device driver. */
} SERIAL_DEV_CFG;


/*
*********************************************************************************************************
*                             SERIAL INTERFACE LINE DRIVER API DATA TYPE
*********************************************************************************************************
*/

struct  serial_line_drv_api {
    void       *(*Open)         (SERIAL_DEV  *pdev,             /* Open  line driver.                                   */
                                 SERIAL_ERR  *perr);

    void        (*Close)        (SERIAL_DEV  *pdev,             /* Close line driver.                                   */
                                 void        *pline_data,
                                 SERIAL_ERR  *perr);

    void        (*RdOctet)      (SERIAL_DEV  *pdev,             /* Rd octet from buf via protocol.                      */
                                 SERIAL_BUF  *pbuf,
                                 CPU_INT08U  *pdatum,
                                 SERIAL_ERR  *perr);

    void        (*WrOctet)      (SERIAL_DEV  *pdev,             /* Wr octet into buf via protocol.                      */
                                 SERIAL_BUF  *pbuf,
                                 CPU_INT08U   datum,
                                 SERIAL_ERR  *perr);
};


/*
*********************************************************************************************************
*                                  SERIAL INTERFACE DEVICE DATA TYPE
*********************************************************************************************************
*/

struct  serial_dev {
    SERIAL_IF_NBR         IF_Nbr;                               /* IF nbr.                                              */

    SERIAL_DEV_CFG       *Dev_Cfg;                              /* Device configuration.                                */

    void                 *Drv_Data;                             /* Device driver internal data.                         */

    SERIAL_LINE_DRV_API  *LineDrv_API;                          /* Line Driver API.                                     */
    void                 *LineDrv_Data;                         /* Line Driver internal data.                           */
};


/*
*********************************************************************************************************
*                              SERIAL INTERFACE CONFIGURATION DATA TYPE
*********************************************************************************************************
*/

struct  serial_if_cfg {
    CPU_INT32U  Baudrate;
    CPU_INT08U  DataBits;
    CPU_INT08U  StopBits;
    CPU_INT08U  Parity;
    CPU_INT08U  FlowCtrl;
};


/*
*********************************************************************************************************
*                                     CALLBACK FUNCTION DATA TYPE
*********************************************************************************************************
*/

typedef  void  (SERIAL_CALLBACK_FNCT)     (SERIAL_IF_NBR   if_nbr,
                                           void           *parg,
                                           void           *pbuf,
                                           CPU_SIZE_T      len,
                                           SERIAL_ERR      err);


/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/

                                                                /* ---------------- INIT/CFG FUNCTIONS ---------------- */
                                                                /* Init module.                                         */
SERIAL_ERR     Serial_Init       (void);
                                                                /* Add device driver.                                   */
void           Serial_DevDrvAdd  (CPU_CHAR              *pname,
                                  SERIAL_DEV_CFG        *pdev_cfg,
                                  CPU_SIZE_T             rd_buf_len,
                                  CPU_SIZE_T             wr_buf_len,
                                  SERIAL_ERR            *perr);

                                                                /* -------------- COMMUNICATION FUNCTIONS ------------- */
                                                                /* Open interface.                                      */
SERIAL_IF_NBR  Serial_Open       (CPU_CHAR              *pname,
                                  SERIAL_IF_CFG         *pcfg,
                                  SERIAL_ERR            *perr);
                                                                /* Close interface.                                     */
void           Serial_Close      (SERIAL_IF_NBR          if_nbr,
                                  SERIAL_ERR            *perr);
                                                                /* Set interface line drv.                              */
void           Serial_SetLineDrv (SERIAL_IF_NBR          if_nbr,
                                  SERIAL_LINE_DRV_API   *pline_api,
                                  SERIAL_ERR            *perr);
                                                                /* Read from interface.                                 */
CPU_SIZE_T     Serial_Rd         (SERIAL_IF_NBR          if_nbr,
                                  void                  *pdest,
                                  CPU_SIZE_T             len,
                                  CPU_INT32U             timeout_ms,
                                  SERIAL_ERR            *perr);
                                                                /* Read from interface asynchronously.                  */
void           Serial_RdAsync    (SERIAL_IF_NBR          if_nbr,
                                  void                  *pdest,
                                  CPU_SIZE_T             len,
                                  SERIAL_CALLBACK_FNCT  *phandler,
                                  void                  *parg,
                                  SERIAL_ERR            *perr);
                                                                /* Wait for octet array to arrive on interface.         */
void           Serial_WaitFor    (SERIAL_IF_NBR          if_nbr,
                                  void                  *psrc,
                                  CPU_SIZE_T             len,
                                  CPU_INT32U             timeout_ms,
                                  SERIAL_ERR            *perr);
                                                                /* Write to interface.                                  */
CPU_SIZE_T     Serial_Wr         (SERIAL_IF_NBR          if_nbr,
                                  void                  *psrc,
                                  CPU_SIZE_T             len,
                                  CPU_INT32U             timeout_ms,
                                  SERIAL_ERR            *perr);
                                                                /* Write to interface asynchronously.                   */
void           Serial_WrAsync    (SERIAL_IF_NBR          if_nbr,
                                  void                  *psrc,
                                  CPU_SIZE_T             len,
                                  SERIAL_CALLBACK_FNCT  *phandler,
                                  void                  *parg,
                                  SERIAL_ERR            *perr);



                                                                /* --------- HANDLERS CALLED BY DEV DRV LAYER --------- */
                                                                /* Get device ptr.                                      */
SERIAL_DEV    *SerialIF_GetDevPtr(SERIAL_IF_NBR          if_nbr,
                                  SERIAL_ERR            *perr);
                                                                /* Handle rx complete.                                  */
void           SerialIF_Rx       (SERIAL_DEV            *pdev,
                                  CPU_INT08U             datum);
                                                                /* Handle tx complete.                                  */
void           SerialIF_Tx       (SERIAL_DEV            *pdev);

#if (SERIAL_CFG_WR_BUF_EN == DEF_ENABLED)                       /* Handle tx echo.                                      */
void           SerialIF_TxWrBuf  (SERIAL_DEV            *pdev,
                                  CPU_INT08U             datum);
#endif


/*
*********************************************************************************************************
*                                        CONFIGURATION ERRORS
*********************************************************************************************************
*/

#ifdef   SERIAL_CFG_ARG_CHK_EXT_EN
#if    ((SERIAL_CFG_ARG_CHK_EXT_EN != DEF_ENABLED ) && \
        (SERIAL_CFG_ARG_CHK_EXT_EN != DEF_DISABLED))
#error  "SERIAL_CFG_ARG_CHK_EXT_EN        illegally #define'd in 'app_cfg.h'"
#error  "                                [MUST be DEF_ENABLED ]             "
#error  "                                [     || DEF_DISABLED]             "
#endif
#endif


#ifndef  SERIAL_CFG_MAX_NBR_IF
#error  "SERIAL_CFG_MAX_NBR_IF                  not #define'd in 'app_cfg.h'"
#error  "                                [MUST be >=   1]                   "
#error  "                                [     && <  255]                   "

#elif  ((SERIAL_CFG_MAX_NBR_IF < 1) || \
        (SERIAL_CFG_MAX_NBR_IF > SERIAL_IF_NBR_MAX))
#error  "SERIAL_CFG_MAX_NBR_IF            illegally #define'd in 'app_cfg.h'"
#error  "                                [MUST be >=   1]                   "
#error  "                                [     && <  255]                   "
#endif



#ifdef   SERIAL_CFG_RD_BUF_EN
#if    ((SERIAL_CFG_RD_BUF_EN != DEF_ENABLED ) && \
        (SERIAL_CFG_RD_BUF_EN != DEF_DISABLED))
#error  "SERIAL_CFG_RD_BUF_EN             illegally #define'd in 'app_cfg.h'"
#error  "                                [MUST be DEF_ENABLED ]             "
#error  "                                [     || DEF_DISABLED]             "
#endif
#endif


#ifdef   SERIAL_CFG_WR_BUF_EN
#if    ((SERIAL_CFG_WR_BUF_EN != DEF_ENABLED ) && \
        (SERIAL_CFG_WR_BUF_EN != DEF_DISABLED))
#error  "SERIAL_CFG_WR_BUF_EN             illegally #define'd in 'app_cfg.h'"
#error  "                                [MUST be DEF_ENABLED ]             "
#error  "                                [     || DEF_DISABLED]             "
#endif
#endif


#ifndef  LIB_MEM_CFG_ALLOC_EN
#error  "LIB_MEM_CFG_ALLOC_EN                   not #define'd in 'lib_cfg.h'"
#error  "                                [MUST be DEF_ENABLED ]             "
#error  "                                [     || DEF_DISABLED]             "

#elif   (LIB_MEM_CFG_ALLOC_EN != DEF_ENABLED)
#error  "LIB_MEM_CFG_ALLOC_EN             illegally #define'd in 'lib_cfg.h'"
#error  "                                [MUST be DEF_ENABLED]              "
#endif


/*
*********************************************************************************************************
*                                             MODULE END
*********************************************************************************************************
*/

#endif                                                          /* End of SERIAL module include.                        */
