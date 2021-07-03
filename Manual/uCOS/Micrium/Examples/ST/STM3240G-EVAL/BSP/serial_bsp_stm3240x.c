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
*                                            DEVICE DRIVER
*
*                                                STM32
*                                          BSP FOR STM3240x
*
* Filename      : serial_bsp_stm3240x.c
* Version       : V2.00.01
* Programmer(s) : FGK
*                 FF
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#include  <stm32f4xx_hal.h>
#include  <serial_bsp_stm3240x.h>


/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/

                                                                /* ----------- REGISTER BASE SPECIFICATIONS ----------- */
#define  SERIAL_BSP_STM32_USART1_BASE     ((void *)0x40011000)
#define  SERIAL_BSP_STM32_USART2_BASE     ((void *)0x40004400)
#define  SERIAL_BSP_STM32_USART3_BASE     ((void *)0x40004800)
#define  SERIAL_BSP_STM32_USART4_BASE     ((void *)0x40004C00)
#define  SERIAL_BSP_STM32_USART5_BASE     ((void *)0x40005000)
#define  SERIAL_BSP_STM32_USART6_BASE     ((void *)0x40011400)


/*
*********************************************************************************************************
*                                               EXTERNS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/
                                                                /* ------------------ USART1 BSP API ------------------ */
static  void        SerialBSP_STM32_USART1_ClkEn       (SERIAL_ERR   *perr);
static  void        SerialBSP_STM32_USART1_ClkDis      (SERIAL_ERR   *perr);
static  void        SerialBSP_STM32_USART1_CfgGPIO     (CPU_BOOLEAN   flow_ctrl,
                                                        SERIAL_ERR   *perr);
static  void        SerialBSP_STM32_USART1_CfgInt      (SERIAL_DEV   *pdev,
                                                        SERIAL_ERR   *perr);
static  CPU_INT32U  SerialBSP_STM32_USART1_ClkFreqGet  (void);

                                                                /* ------------------ USART2 BSP API ------------------ */
static  void        SerialBSP_STM32_USART2_ClkEn       (SERIAL_ERR   *perr);
static  void        SerialBSP_STM32_USART2_ClkDis      (SERIAL_ERR   *perr);
static  void        SerialBSP_STM32_USART2_CfgGPIO     (CPU_BOOLEAN   flow_ctrl,
                                                        SERIAL_ERR   *perr);
static  void        SerialBSP_STM32_USART2_CfgInt      (SERIAL_DEV   *pdev,
                                                        SERIAL_ERR   *perr);
static  CPU_INT32U  SerialBSP_STM32_USART2_ClkFreqGet  (void);

                                                                /* ------------------ USART3 BSP API ------------------ */
static  void        SerialBSP_STM32_USART3_ClkEn       (SERIAL_ERR   *perr);
static  void        SerialBSP_STM32_USART3_ClkDis      (SERIAL_ERR   *perr);
static  void        SerialBSP_STM32_USART3_CfgGPIO     (CPU_BOOLEAN   flow_ctrl,
                                                        SERIAL_ERR   *perr);
static  void        SerialBSP_STM32_USART3_CfgInt      (SERIAL_DEV   *pdev,
                                                        SERIAL_ERR   *perr);
static  CPU_INT32U  SerialBSP_STM32_USART3_ClkFreqGet  (void);

                                                                /* ------------------- USART4 BSP API ----------------- */
static  void        SerialBSP_STM32_USART4_ClkEn       (SERIAL_ERR   *perr);
static  void        SerialBSP_STM32_USART4_ClkDis      (SERIAL_ERR   *perr);
static  void        SerialBSP_STM32_USART4_CfgGPIO     (CPU_BOOLEAN   flow_ctrl,
                                                        SERIAL_ERR   *perr);
static  void        SerialBSP_STM32_USART4_CfgInt      (SERIAL_DEV   *pdev,
                                                        SERIAL_ERR   *perr);
static  CPU_INT32U  SerialBSP_STM32_USART4_ClkFreqGet  (void);

                                                                /* ------------------- USART5 BSP API ----------------- */
static  void        SerialBSP_STM32_USART5_ClkEn       (SERIAL_ERR   *perr);
static  void        SerialBSP_STM32_USART5_ClkDis      (SERIAL_ERR   *perr);
static  void        SerialBSP_STM32_USART5_CfgGPIO     (CPU_BOOLEAN   flow_ctrl,
                                                        SERIAL_ERR   *perr);
static  void        SerialBSP_STM32_USART5_CfgInt      (SERIAL_DEV   *pdev,
                                                        SERIAL_ERR   *perr);
static  CPU_INT32U  SerialBSP_STM32_USART5_ClkFreqGet  (void);

                                                                /* ------------------ USART6 BSP API ------------------ */
static  void        SerialBSP_STM32_USART6_ClkEn       (SERIAL_ERR   *perr);
static  void        SerialBSP_STM32_USART6_ClkDis      (SERIAL_ERR   *perr);
static  void        SerialBSP_STM32_USART6_CfgGPIO     (CPU_BOOLEAN   flow_ctrl,
                                                        SERIAL_ERR   *perr);
static  void        SerialBSP_STM32_USART6_CfgInt      (SERIAL_DEV   *pdev,
                                                        SERIAL_ERR   *perr);
static  CPU_INT32U  SerialBSP_STM32_USART6_ClkFreqGet  (void);


static  void        SerialBSP_STM32_USART1_IntHandler    (void);
static  void        SerialBSP_STM32_USART2_IntHandler    (void);
static  void        SerialBSP_STM32_USART3_IntHandler    (void);
static  void        SerialBSP_STM32_USART4_IntHandler    (void);
static  void        SerialBSP_STM32_USART5_IntHandler    (void);
static  void        SerialBSP_STM32_USART6_IntHandler    (void);

/*
*********************************************************************************************************
*                                   SERIAL INTERFACE DEVICE BSP API
*********************************************************************************************************
*/

static  SERIAL_BSP_API  SerialBSP_STM32_USART1 = {
    SerialBSP_STM32_USART1_ClkEn,
    SerialBSP_STM32_USART1_ClkDis,
    SerialBSP_STM32_USART1_CfgGPIO,
    SerialBSP_STM32_USART1_CfgInt,
    SerialBSP_STM32_USART1_ClkFreqGet
};

static  SERIAL_BSP_API  SerialBSP_STM32_USART2 = {
    SerialBSP_STM32_USART2_ClkEn,
    SerialBSP_STM32_USART2_ClkDis,
    SerialBSP_STM32_USART2_CfgGPIO,
    SerialBSP_STM32_USART2_CfgInt,
    SerialBSP_STM32_USART2_ClkFreqGet
};

static  SERIAL_BSP_API  SerialBSP_STM32_USART3 = {
    SerialBSP_STM32_USART3_ClkEn,
    SerialBSP_STM32_USART3_ClkDis,
    SerialBSP_STM32_USART3_CfgGPIO,
    SerialBSP_STM32_USART3_CfgInt,
    SerialBSP_STM32_USART3_ClkFreqGet
};

static  SERIAL_BSP_API  SerialBSP_STM32_USART4 = {
    SerialBSP_STM32_USART4_ClkEn,
    SerialBSP_STM32_USART4_ClkDis,
    SerialBSP_STM32_USART4_CfgGPIO,
    SerialBSP_STM32_USART4_CfgInt,
    SerialBSP_STM32_USART4_ClkFreqGet
};

static  SERIAL_BSP_API  SerialBSP_STM32_USART5 = {
    SerialBSP_STM32_USART5_ClkEn,
    SerialBSP_STM32_USART5_ClkDis,
    SerialBSP_STM32_USART5_CfgGPIO,
    SerialBSP_STM32_USART5_CfgInt,
    SerialBSP_STM32_USART5_ClkFreqGet
};

static  SERIAL_BSP_API  SerialBSP_STM32_USART6 = {
    SerialBSP_STM32_USART6_ClkEn,
    SerialBSP_STM32_USART6_ClkDis,
    SerialBSP_STM32_USART6_CfgGPIO,
    SerialBSP_STM32_USART6_CfgInt,
    SerialBSP_STM32_USART6_ClkFreqGet
};


/*
*********************************************************************************************************
*********************************************************************************************************
*                                      SERIAL INTERFACE DEVICES
*********************************************************************************************************
*********************************************************************************************************
*/

SERIAL_DEV_CFG  SerialDevCfg_STM32_USART1 = {
     SERIAL_BSP_STM32_USART1_BASE,
    &SerialDrv_STM32_API,
    &SerialBSP_STM32_USART1
};

SERIAL_DEV_CFG  SerialDevCfg_STM32_USART2 = {
     SERIAL_BSP_STM32_USART2_BASE,
    &SerialDrv_STM32_API,
    &SerialBSP_STM32_USART2
};

SERIAL_DEV_CFG  SerialDevCfg_STM32_USART3 = {
     SERIAL_BSP_STM32_USART3_BASE,
    &SerialDrv_STM32_API,
    &SerialBSP_STM32_USART3
};

SERIAL_DEV_CFG  SerialDevCfg_STM32_USART4 = {
     SERIAL_BSP_STM32_USART4_BASE,
    &SerialDrv_STM32_API,
    &SerialBSP_STM32_USART4
};

SERIAL_DEV_CFG  SerialDevCfg_STM32_USART5 = {
     SERIAL_BSP_STM32_USART5_BASE,
    &SerialDrv_STM32_API,
    &SerialBSP_STM32_USART5
};

SERIAL_DEV_CFG  SerialDevCfg_STM32_USART6 = {
     SERIAL_BSP_STM32_USART6_BASE,
    &SerialDrv_STM32_API,
    &SerialBSP_STM32_USART6
};

/*
*********************************************************************************************************
*                                   SERIAL INTERFACE DEVICE POINTER
*
* Note(s) : (1) A serial interface device pointer is required to be passed to the serial interface device
*               driver api ISR_Handler().
*********************************************************************************************************
*/

static  SERIAL_DEV  *SerialDev_STM32_USART1_DevPtr;
static  SERIAL_DEV  *SerialDev_STM32_USART2_DevPtr;
static  SERIAL_DEV  *SerialDev_STM32_USART3_DevPtr;
static  SERIAL_DEV  *SerialDev_STM32_USART4_DevPtr;
static  SERIAL_DEV  *SerialDev_STM32_USART5_DevPtr;
static  SERIAL_DEV  *SerialDev_STM32_USART6_DevPtr;

/*$PAGE*/
/*
*********************************************************************************************************
*                                       SerialBSP_STM32_USARTx_ClkEn()
*
* Description : Enable device/peripheral clock.
*
* Argument(s) : perr        Pointer to variable that will receive the return error code from this function :
*
*                               SERIAL_ERR_NONE                 Device/peripheral clock enabled successfully.
*
* Return(s)   : none.
*
* Caller(s)   : SerialDrv_Open() via 'pdev->BSP_API->ClkEn()'.
*
* Note(s)     : none.
*********************************************************************************************************
*/


static  void  SerialBSP_STM32_USART1_ClkEn (SERIAL_ERR  *perr)
{
   *perr = SERIAL_ERR_NONE;
}

static  void  SerialBSP_STM32_USART2_ClkEn (SERIAL_ERR  *perr)
{
   *perr = SERIAL_ERR_NONE;
}

static  void  SerialBSP_STM32_USART3_ClkEn (SERIAL_ERR  *perr)
{
    BSP_PeriphEn(BSP_PERIPH_ID_GPIOC);                          /* Enable GPIOC clock.                                  */
    BSP_PeriphEn(BSP_PERIPH_ID_USART3);                         /* Enable UART clock.                                   */
}

static  void  SerialBSP_STM32_USART4_ClkEn (SERIAL_ERR  *perr)
{
   *perr = SERIAL_ERR_NONE;
}

static  void  SerialBSP_STM32_USART5_ClkEn (SERIAL_ERR  *perr)
{
   *perr = SERIAL_ERR_NONE;
}

static  void  SerialBSP_STM32_USART6_ClkEn (SERIAL_ERR  *perr)
{
   *perr = SERIAL_ERR_NONE;
}

/*$PAGE*/
/*
*********************************************************************************************************
*                                         SerialBSP_STM32_USARTx_ClkDis()
*
* Description : Disable device/peripheral clock.
*
* Argument(s) : perr        Pointer to variable that will receive the return error code from this function :
*
*                               SERIAL_ERR_NONE                 Device/peripheral clock disabled successfully.
*
* Return(s)   : none.
*
* Caller(s)   : SerialDrv_Close() via 'pdev->BSP_API->ClkDis()'.
*
* Note(s)     : none.
*********************************************************************************************************
*/


static  void  SerialBSP_STM32_USART1_ClkDis (SERIAL_ERR  *perr)
{
   *perr = SERIAL_ERR_NONE;
}

static  void  SerialBSP_STM32_USART2_ClkDis (SERIAL_ERR  *perr)
{
   *perr = SERIAL_ERR_NONE;
}

static  void  SerialBSP_STM32_USART3_ClkDis (SERIAL_ERR  *perr)
{
    BSP_PeriphDis(BSP_PERIPH_ID_USART3);                        /* Disable UART clock.                                   */

   *perr = SERIAL_ERR_NONE;
}

static  void  SerialBSP_STM32_USART4_ClkDis (SERIAL_ERR  *perr)
{
   *perr = SERIAL_ERR_NONE;
}

static  void  SerialBSP_STM32_USART5_ClkDis (SERIAL_ERR  *perr)
{
   *perr = SERIAL_ERR_NONE;
}

static  void  SerialBSP_STM32_USART6_ClkDis (SERIAL_ERR  *perr)
{
   *perr = SERIAL_ERR_NONE;
}

/*$PAGE*/
/*
*********************************************************************************************************
*                                       SerialBSP_STM32_USARTx_CfgGPIO()
*
* Description : Configure device/peripheral input/output pins.
*
* Argument(s) : flow_ctrl   Flow control enabled.
*
*               perr        Pointer to variable that will receive the return error code from this function :
*
*                               SERIAL_ERR_NONE                 Device/peripheral GPIO configured successfully.
*                               SERIAL_ERR_DRV_INVALID          Device configuration invalid.
*
* Return(s)   : none.
*
* Caller(s)   : SerialDrv_Open() via 'pdev->BSP_API->CfgGPIO()'.
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  void  SerialBSP_STM32_USART1_CfgGPIO (CPU_BOOLEAN   flow_ctrl,
                                              SERIAL_ERR   *perr)
{
    if (flow_ctrl == DEF_ENABLED) {
        *perr = SERIAL_ERR_DRV_INVALID;
         return;
    }

   *perr = SERIAL_ERR_NONE;
}

static  void  SerialBSP_STM32_USART2_CfgGPIO (CPU_BOOLEAN   flow_ctrl,
                                              SERIAL_ERR   *perr)
{
    if (flow_ctrl == DEF_ENABLED) {
        *perr = SERIAL_ERR_DRV_INVALID;
         return;
    }

   *perr = SERIAL_ERR_NONE;
}

static  void  SerialBSP_STM32_USART3_CfgGPIO (CPU_BOOLEAN   flow_ctrl,
                                              SERIAL_ERR   *perr)
{
    GPIO_InitTypeDef  GPIO_InitStructure;


    if (flow_ctrl == DEF_ENABLED) {
        *perr = SERIAL_ERR_DRV_INVALID;
         return;
    }

                                                                /* Configure GPIOC.10 as push-pull.                     */
    GPIO_InitStructure.Pin       = GPIO_PIN_10;
    GPIO_InitStructure.Speed     = GPIO_SPEED_FAST;
    GPIO_InitStructure.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStructure.Pull      = GPIO_PULLUP;
    GPIO_InitStructure.Alternate = GPIO_AF7_USART3;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);

                                                                /* Configure GPIOC.11 as input floating.                 */
    GPIO_InitStructure.Pin       = GPIO_PIN_11;
    GPIO_InitStructure.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStructure.Alternate = GPIO_AF7_USART3;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);

   *perr = SERIAL_ERR_NONE;
}

static  void  SerialBSP_STM32_USART4_CfgGPIO (CPU_BOOLEAN   flow_ctrl,
                                              SERIAL_ERR   *perr)
{
    if (flow_ctrl == DEF_ENABLED) {
        *perr = SERIAL_ERR_DRV_INVALID;
         return;
    }

   *perr = SERIAL_ERR_NONE;
}

static  void  SerialBSP_STM32_USART5_CfgGPIO (CPU_BOOLEAN   flow_ctrl,
                                              SERIAL_ERR   *perr)
{
    if (flow_ctrl == DEF_ENABLED) {
        *perr = SERIAL_ERR_DRV_INVALID;
         return;
    }

   *perr = SERIAL_ERR_NONE;
}

static  void  SerialBSP_STM32_USART6_CfgGPIO (CPU_BOOLEAN   flow_ctrl,
                                              SERIAL_ERR   *perr)
{
    if (flow_ctrl == DEF_ENABLED) {
        *perr = SERIAL_ERR_DRV_INVALID;
         return;
    }

   *perr = SERIAL_ERR_NONE;
}

/*$PAGE*/
/*
*********************************************************************************************************
*                                       SerialBSP_STM32_USARTx_CfgInt()
*
* Description : Configure device/peripheral interrupt.
*
* Argument(s) : pdev        Pointer to device.
*
*               perr        Pointer to variable that will receive the return error code from this function :
*
*                               SERIAL_ERR_NONE                 Device/peripheral interrupt configured successfully.
*
* Return(s)   : none.
*
* Caller(s)   : SerialDrv_Open() via 'pdev->BSP_API->CfgInt()'.
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  void  SerialBSP_STM32_USART1_CfgInt (SERIAL_DEV  *pdev,
                                             SERIAL_ERR  *perr)
{
    SerialDev_STM32_USART1_DevPtr = pdev;

    BSP_IntVectSet(BSP_INT_ID_USART1, SerialBSP_STM32_USART1_IntHandler);
    BSP_IntEn(BSP_INT_ID_USART1);

   *perr = SERIAL_ERR_NONE;
}

static  void  SerialBSP_STM32_USART2_CfgInt (SERIAL_DEV  *pdev,
                                             SERIAL_ERR  *perr)
{
    SerialDev_STM32_USART2_DevPtr = pdev;

    BSP_IntVectSet(BSP_INT_ID_USART2, SerialBSP_STM32_USART2_IntHandler);
    BSP_IntEn(BSP_INT_ID_USART2);

   *perr = SERIAL_ERR_NONE;
}

static  void  SerialBSP_STM32_USART3_CfgInt (SERIAL_DEV  *pdev,
                                             SERIAL_ERR  *perr)
{
    SerialDev_STM32_USART3_DevPtr = pdev;

    BSP_IntVectSet(BSP_INT_ID_USART3, SerialBSP_STM32_USART3_IntHandler);
    BSP_IntEn(BSP_INT_ID_USART3);

   *perr = SERIAL_ERR_NONE;
}

static  void  SerialBSP_STM32_USART4_CfgInt (SERIAL_DEV  *pdev,
                                             SERIAL_ERR  *perr)
{
    SerialDev_STM32_USART4_DevPtr = pdev;

    BSP_IntVectSet(BSP_INT_ID_USART4, SerialBSP_STM32_USART4_IntHandler);
    BSP_IntEn(BSP_INT_ID_USART4);

   *perr = SERIAL_ERR_NONE;
}

static  void  SerialBSP_STM32_USART5_CfgInt (SERIAL_DEV  *pdev,
                                             SERIAL_ERR  *perr)
{
    SerialDev_STM32_USART5_DevPtr = pdev;

    BSP_IntVectSet(BSP_INT_ID_USART5, SerialBSP_STM32_USART5_IntHandler);
    BSP_IntEn(BSP_INT_ID_USART5);

   *perr = SERIAL_ERR_NONE;
}

static  void  SerialBSP_STM32_USART6_CfgInt (SERIAL_DEV  *pdev,
                                             SERIAL_ERR  *perr)
{
    SerialDev_STM32_USART6_DevPtr = pdev;

    BSP_IntVectSet(BSP_INT_ID_USART6, SerialBSP_STM32_USART6_IntHandler);
    BSP_IntEn(BSP_INT_ID_USART6);

   *perr = SERIAL_ERR_NONE;
}

/*$PAGE*/
/*
*********************************************************************************************************
*                                     SerialBSP_STM32_USARTx_ClkFreqGet()
*
* Description : Get peripheral clock frequency.
*
* Argument(s) : none.
*
* Return(s)   : Peripheral clock frequency, in Hz.
*
* Caller(s)   : SerialDrv_Open() via 'pdev->BSP_API->ClkFreqGet()'.
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  CPU_INT32U  SerialBSP_STM32_USART1_ClkFreqGet (void)
{
    CPU_INT32U  clk_freq;


    clk_freq = BSP_PeriphClkFreqGet(BSP_PERIPH_ID_USART1);
    return (clk_freq);
}

static  CPU_INT32U  SerialBSP_STM32_USART2_ClkFreqGet (void)
{
    CPU_INT32U  clk_freq;


    clk_freq = BSP_PeriphClkFreqGet(BSP_PERIPH_ID_USART2);
    return (clk_freq);
}

static  CPU_INT32U  SerialBSP_STM32_USART3_ClkFreqGet (void)
{
    CPU_INT32U  clk_freq;


    clk_freq = BSP_PeriphClkFreqGet(BSP_PERIPH_ID_USART3);
    return (clk_freq);
}

static  CPU_INT32U  SerialBSP_STM32_USART4_ClkFreqGet (void)
{
    CPU_INT32U  clk_freq;


    clk_freq = BSP_PeriphClkFreqGet(BSP_PERIPH_ID_USART4);
    return (clk_freq);
}

static  CPU_INT32U  SerialBSP_STM32_USART5_ClkFreqGet (void)
{
    CPU_INT32U  clk_freq;


    clk_freq = BSP_PeriphClkFreqGet(BSP_PERIPH_ID_USART5);
    return (clk_freq);
}

static  CPU_INT32U  SerialBSP_STM32_USART6_ClkFreqGet (void)
{
    CPU_INT32U  clk_freq;


    clk_freq = BSP_PeriphClkFreqGet(BSP_PERIPH_ID_USART6);
    return (clk_freq);
}

/*$PAGE*/
/*
*********************************************************************************************************
*                                     SerialBSP_STM32_USARTx_IntHandler()
*
* Description : Handle serial port interrupt.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : This is an ISR.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  SerialBSP_STM32_USART1_IntHandler (void)
{
    SERIAL_DEV      *p_dev;
    SERIAL_DEV_CFG  *p_cfg;


    p_dev = (SERIAL_DEV *)SerialDev_STM32_USART1_DevPtr;
    p_cfg = p_dev->Dev_Cfg;
    p_cfg->Drv_API->ISR_Handler(p_dev, SERIAL_ISR_TYPE_UNKNOWN);
}

void  SerialBSP_STM32_USART2_IntHandler (void)
{
    SERIAL_DEV      *p_dev;
    SERIAL_DEV_CFG  *p_cfg;


    p_dev = (SERIAL_DEV *)SerialDev_STM32_USART2_DevPtr;
    p_cfg = p_dev->Dev_Cfg;
    p_cfg->Drv_API->ISR_Handler(p_dev, SERIAL_ISR_TYPE_UNKNOWN);
}

void  SerialBSP_STM32_USART3_IntHandler (void)
{
    SERIAL_DEV      *p_dev;
    SERIAL_DEV_CFG  *p_cfg;


    p_dev = (SERIAL_DEV *)SerialDev_STM32_USART3_DevPtr;
    p_cfg = p_dev->Dev_Cfg;
    p_cfg->Drv_API->ISR_Handler(p_dev, SERIAL_ISR_TYPE_UNKNOWN);
}

void  SerialBSP_STM32_USART4_IntHandler (void)
{
    SERIAL_DEV      *p_dev;
    SERIAL_DEV_CFG  *p_cfg;


    p_dev = (SERIAL_DEV *)SerialDev_STM32_USART4_DevPtr;
    p_cfg = p_dev->Dev_Cfg;
    p_cfg->Drv_API->ISR_Handler(p_dev, SERIAL_ISR_TYPE_UNKNOWN);
}

void  SerialBSP_STM32_USART5_IntHandler (void)
{
    SERIAL_DEV      *p_dev;
    SERIAL_DEV_CFG  *p_cfg;


    p_dev = (SERIAL_DEV *)SerialDev_STM32_USART5_DevPtr;
    p_cfg = p_dev->Dev_Cfg;
    p_cfg->Drv_API->ISR_Handler(p_dev, SERIAL_ISR_TYPE_UNKNOWN);
}

void  SerialBSP_STM32_USART6_IntHandler (void)
{
    SERIAL_DEV      *p_dev;
    SERIAL_DEV_CFG  *p_cfg;


    p_dev = (SERIAL_DEV *)SerialDev_STM32_USART6_DevPtr;
    p_cfg = p_dev->Dev_Cfg;
    p_cfg->Drv_API->ISR_Handler(p_dev, SERIAL_ISR_TYPE_UNKNOWN);
}
