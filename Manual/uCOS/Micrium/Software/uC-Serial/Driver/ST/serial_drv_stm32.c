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
*
* Filename      : serial_drv_stm32.c
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
#include  <serial_drv_stm32.h>


/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/

#define  SR_TXE                 DEF_BIT_07
#define  SR_TC                  DEF_BIT_06
#define  SR_RXNE                DEF_BIT_05
#define  SR_ORE                 DEF_BIT_03


#define  CR1_UE                 DEF_BIT_13
#define  CR1_M                  DEF_BIT_12
#define  CR1_PCE                DEF_BIT_10
#define  CR1_PS                 DEF_BIT_09
#define  CR1_TXEIE              DEF_BIT_07
#define  CR1_TCIE               DEF_BIT_06
#define  CR1_RXNEIE             DEF_BIT_05
#define  CR1_TE                 DEF_BIT_03
#define  CR1_RE                 DEF_BIT_02

#define  CR1_PARITY_NONE        DEF_BIT_NONE
#define  CR1_PARITY_EVEN        DEF_BIT_10
#define  CR1_PARITY_ODD         DEF_BIT_FIELD(2,  9)


#define  CR2_STOP_BITS_1        DEF_BIT_MASK (0, 12)
#define  CR2_STOP_BITS_2        DEF_BIT_MASK (2, 12)
#define  CR2_STOP_BITS_1_5      DEF_BIT_MASK (3, 12)

#define  CR2_STOP_MASK          DEF_BIT_FIELD(2, 12)
#define  CR2_CLKEN              DEF_BIT_11
#define  CR2_CPOL               DEF_BIT_10
#define  CR2_CPHA               DEF_BIT_09
#define  CR2_LBCL               DEF_BIT_08

#define  CR3_CTSE               DEF_BIT_09
#define  CR3_RTSE               DEF_BIT_08


/*
*********************************************************************************************************
*                                          LOCAL DATA TYPES
*********************************************************************************************************
*/

typedef  struct  serial_reg {                                   /* Serial dev reg struct.                               */
    CPU_REG16  SR;
    CPU_REG16  RESERVED0;
    CPU_REG16  DR;
    CPU_REG16  RESERVED1;
    CPU_REG16  BRR;
    CPU_REG16  RESERVED2;
    CPU_REG16  CR1;
    CPU_REG16  RESERVED3;
    CPU_REG16  CR2;
    CPU_REG16  RESERVED4;
    CPU_REG16  CR3;
    CPU_REG16  RESERVED5;
    CPU_REG16  GTPR;
    CPU_REG16  RESERVED6;
} SERIAL_REG;


/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/
                                                                /* Driver initialization.                               */
static  void        SerialDrv_Init       (SERIAL_ERR     *perr);

                                                                /* Driver open.                                         */
static  void        SerialDrv_Open       (SERIAL_DEV     *pdev,
                                          SERIAL_IF_CFG  *pcfg,
                                          SERIAL_ERR     *perr);

                                                                /* Driver close.                                        */
static  void        SerialDrv_Close      (SERIAL_DEV     *pdev,
                                          SERIAL_ERR     *perr);

                                                                /* Driver receiver start.                               */
static  void        SerialDrv_RxStart    (SERIAL_DEV     *pdev,
                                          SERIAL_ERR     *perr);

                                                                /* Driver receiver stop.                                */
static  void        SerialDrv_RxStop     (SERIAL_DEV     *pdev,
                                          SERIAL_ERR     *perr);

                                                                /* Driver octet receive.                                */
static  void        SerialDrv_RxOctet    (SERIAL_DEV     *pdev,
                                          CPU_INT08U     *pdatum,
                                          SERIAL_ERR     *perr);

                                                                /* Driver transmitter start.                            */
static  void        SerialDrv_TxStart    (SERIAL_DEV     *pdev,
                                          SERIAL_ERR     *perr);

                                                                /* Driver transmitter stop.                             */
static  void        SerialDrv_TxStop     (SERIAL_DEV     *pdev,
                                          SERIAL_ERR     *perr);

                                                                /* Driver octet transmit.                               */
static  void        SerialDrv_TxOctet    (SERIAL_DEV     *pdev,
                                          CPU_INT08U      datum,
                                          SERIAL_ERR     *perr);

                                                                /* Driver ISR handler.                                  */
static  void        SerialDrv_ISR_Handler(SERIAL_DEV     *pdev,
                                          CPU_INT08U      type);


/*
*********************************************************************************************************
*********************************************************************************************************
*                                 SERIAL INTERFACE DEVICE DRIVER API
*********************************************************************************************************
*********************************************************************************************************
*/

SERIAL_DRV_API  SerialDrv_STM32_API = {
    SerialDrv_Init,
    SerialDrv_Open,
    SerialDrv_Close,
    SerialDrv_RxStart,
    SerialDrv_RxStop,
    SerialDrv_RxOctet,
    SerialDrv_TxStart,
    SerialDrv_TxStop,
    SerialDrv_TxOctet,
    SerialDrv_ISR_Handler
};


/*$PAGE*/
/*
*********************************************************************************************************
*********************************************************************************************************
*                                     DRIVER INTERFACE FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                          SerialDrv_Init()
*
* Description : Initialize serial device driver.
*
* Argument(s) : perr        Pointer to variable that will receive the return error code from this function :
*
*                               SERIAL_ERR_NONE                 Driver initialized.
*
* Return(s)   : None.
*
* Caller(s)   : Serial_DrvAdd() via 'pdrv_api->Init()'.
*
* Note(s)     : None.
*********************************************************************************************************
*/

static  void  SerialDrv_Init (SERIAL_ERR  *perr)
{
   *perr = SERIAL_ERR_NONE;
}


/*$PAGE*/
/*
*********************************************************************************************************
*                                          SerialDrv_Open()
*
* Description : Open a serial device for communication.
*
* Argument(s) : pdev        Pointer to device.
*
*               pcfg        Pointer to interface configuration.
*
*               perr        Pointer to variable that will receive the return error code from this function :
*
*                               SERIAL_ERR_NONE                 Device              opened.
*                               SERIAL_ERR_DRV_OPEN             Device could NOT be opened.
*                               SERIAL_ERR_DRV_INVALID          Device configuration invalid.
*                               SERIAL_ERR_MEM_ALLOC            Memory could NOT be allocated for device
*                                                                   internal data.
*
* Return(s)   : None.
*
* Caller(s)   : Serial_Open() via 'pdev->Drv_API->Open()'.
*
* Note(s)     : (1) Interrupts are assumed to be disabled when this function is called.
*********************************************************************************************************
*/

static  void  SerialDrv_Open (SERIAL_DEV     *pdev,
                              SERIAL_IF_CFG  *pcfg,
                              SERIAL_ERR     *perr)
{
    SERIAL_DEV_CFG  *p_cfg;
    SERIAL_REG      *p_reg;
    CPU_BOOLEAN      flow_ctrl;
    CPU_INT16U       cr1;
    CPU_INT16U       cr2;
    CPU_INT16U       stop;
    CPU_INT16U       parity;
    CPU_INT32U       clk_freq;
    CPU_INT32U       baudrate;
    CPU_INT32U       div_int;
    CPU_INT32U       div_frac;


    p_cfg = pdev->Dev_Cfg;
    p_cfg->BSP_API->ClkEn(perr);                                /* En dev-specific HW clk.                              */
    if (*perr != SERIAL_ERR_NONE) {
         return;
    }

    p_reg = (SERIAL_REG *)p_cfg->BaseAddr;

    switch (pcfg->StopBits) {
        case SERIAL_STOPBITS_1:
             stop = CR2_STOP_BITS_1;
             break;


        case SERIAL_STOPBITS_1_5:
             stop = CR2_STOP_BITS_1_5;
             break;


        case SERIAL_STOPBITS_2:
        default:
             stop = CR2_STOP_BITS_2;
             break;
    }

    cr2         =  p_reg->CR2;
    cr2        &= ~CR2_STOP_MASK;
    cr2        |=  stop;
    p_reg->CR2  =  cr2;


    switch (pcfg->DataBits) {
        case SERIAL_DATABITS_8:
             break;


        case SERIAL_DATABITS_5:
        case SERIAL_DATABITS_6:
        case SERIAL_DATABITS_7:
        default:
            *perr = SERIAL_ERR_DRV_INVALID;
             return;
    }

    switch (pcfg->Parity) {
        case SERIAL_PARITY_ODD:
             parity = CR1_PARITY_ODD;
             break;


        case SERIAL_PARITY_EVEN:
             parity = CR1_PARITY_EVEN;
             break;


        case SERIAL_PARITY_NONE:
             parity = CR1_PARITY_NONE;
             break;


        case SERIAL_PARITY_MARK:
        case SERIAL_PARITY_SPACE:
        default:
            *perr = SERIAL_ERR_DRV_INVALID;
             return;
    }

    cr1         =   p_reg->CR1;
    cr1        &= ~(CR1_M   |
                    CR1_PCE |
                    CR1_PS);
    cr1        |=   parity |
                    CR1_TE |
                    CR1_RE;
    p_reg->CR1  =   cr1;

    switch (pcfg->FlowCtrl) {
        case SERIAL_FLOW_CTRL_HARDWARE:
             flow_ctrl = DEF_ENABLED;

             p_reg->CR3 |= CR3_CTSE |
                           CR3_RTSE;
             break;


        case SERIAL_FLOW_CTRL_NONE:
        default:
             flow_ctrl = DEF_DISABLED;

             p_reg->CR3 &= ~(CR3_CTSE |
                             CR3_RTSE);
             break;
    }

    p_cfg->BSP_API->CfgGPIO(flow_ctrl, perr);                   /* Cfg dev-specific GPIO.                               */
    if (*perr != SERIAL_ERR_NONE) {
         return;
    }
    p_cfg->BSP_API->CfgInt(pdev, perr);                         /* Cfg dev-specific int.                                */
    if (*perr != SERIAL_ERR_NONE) {
         return;
    }

                                                                /* ------------------- CFG BAUD RATE ------------------ */
    baudrate = pcfg->Baudrate;
    clk_freq = p_cfg->BSP_API->ClkFreqGet();                    /* Get periph clk freq.                                 */
    div_int  = (25 * clk_freq + 2 * baudrate) / (4 * baudrate);
    div_frac = ((div_int % 100) * 16 + 50) / 100;
    div_int /= 100;
    if (div_int > 4095) {
       *perr = SERIAL_ERR_DRV_INVALID;
        return;
    }

    p_reg->BRR = (div_int << 4) |                                 /* Set baud rate.                                       */
                 (div_frac & 0xF);


    cr2         =   p_reg->CR2;
    cr2        &= ~(CR2_CLKEN |
                  CR2_CPOL  |
                  CR2_LBCL);
    cr2        |=   CR2_CPHA;
    p_reg->CR2  =   cr2;

    p_reg->CR1 |= CR1_UE;                                         /* En rx'er & tx'er.                                    */


   *perr = SERIAL_ERR_NONE;
}


/*$PAGE*/
/*
*********************************************************************************************************
*                                          SerialDrv_Close()
*
* Description : Close a serial device for communication.
*
* Argument(s) : pdev        Pointer to device.
*
*               perr        Pointer to variable that will receive the return error code from this function :
*
*                               SERIAL_ERR_NONE                 Device closed.
*
* Return(s)   : None.
*
* Caller(s)   : Serial_Close() via 'pdev->Drv_API->Close()'.
*
* Note(s)     : (1) Interrupts are assumed to be disabled when this function is called.
*********************************************************************************************************
*/

static  void  SerialDrv_Close (SERIAL_DEV  *pdev,
                               SERIAL_ERR  *perr)
{
    SERIAL_REG      *p_reg;
    SERIAL_DEV_CFG  *p_cfg;

                                                                /* -------------------- RESET UART -------------------- */
    p_cfg = pdev->Dev_Cfg;
    p_reg = (SERIAL_REG *)p_cfg->BaseAddr;
    p_reg->CR1 &= ~CR1_UE;                                      /* Dis rx'er & tx'er.                                   */
    p_reg->CR1 &= ~(CR1_RXNEIE |                                /* Dis rx int.                                          */
                    CR1_TXEIE);                                 /* Dis tx int.                                          */

    p_cfg->BSP_API->ClkDis(perr);                               /* Dis dev-specific HW clk.                             */
    if (*perr != SERIAL_ERR_NONE) {
         return;
    }

   *perr = SERIAL_ERR_NONE;
}


/*$PAGE*/
/*
*********************************************************************************************************
*                                         SerialDrv_RxStart()
*
* Description : Start receiving data & getting data received interrupts.
*
* Argument(s) : pdev        Pointer to device.
*
*               perr        Pointer to variable that will receive the return error code from this function :
*
*                               SERIAL_ERR_NONE                 Device reception started.
*
* Return(s)   : None.
*
* Caller(s)   : Serial_Open(),
*               Serial_IF_RxStart() via 'pdev->Drv_API->RxStart()'.
*
* Note(s)     : (1) Interrupts are assumed to be disabled when this function is called.
*********************************************************************************************************
*/

static  void  SerialDrv_RxStart (SERIAL_DEV  *pdev,
                                 SERIAL_ERR  *perr)
{
    SERIAL_REG      *p_reg;
    SERIAL_DEV_CFG  *p_cfg;


    p_cfg = pdev->Dev_Cfg;
    p_reg = (SERIAL_REG *)p_cfg->BaseAddr;
    p_reg->CR1 |= CR1_RXNEIE;                                     /* En rx int.                                           */

   *perr = SERIAL_ERR_NONE;
}


/*$PAGE*/
/*
*********************************************************************************************************
*                                         SerialDrv_RxStop()
*
* Description : Stop receiving data and data received interrupts.
*
* Argument(s) : pdev        Pointer to device.
*
*               perr        Pointer to variable that will receive the return error code from this function :
*
*                               SERIAL_ERR_NONE                 Device reception stopped.
*
* Return(s)   : None.
*
* Caller(s)   : Serial_Close(),
*               Serial_IF_Rx(),
*               Serial_IF_RxStop() via 'pdev->Drv_API->RxStop()'.
*
* Note(s)     : (1) Interrupts are assumed to be disabled when this function is called.
*********************************************************************************************************
*/

static  void  SerialDrv_RxStop (SERIAL_DEV  *pdev,
                                SERIAL_ERR  *perr)
{
    SERIAL_REG      *p_reg;
    SERIAL_DEV_CFG  *p_cfg;


    p_cfg = pdev->Dev_Cfg;
    p_reg = (SERIAL_REG *)p_cfg->BaseAddr;
    p_reg->CR1 &= ~CR1_RXNEIE;                                    /* Dis rx int.                                          */

   *perr = SERIAL_ERR_NONE;
}


/*$PAGE*/
/*
*********************************************************************************************************
*                                         SerialDrv_RxOctet()
*
* Description : Receive data octet.
*
* Argument(s) : pdev        Pointer to device.
*
*               pdatum      Pointer to variable that will receive the received data octet.
*
*               perr        Pointer to variable that will receive the return error code from this function :
*
*                               SERIAL_ERR_NONE                 Device octet     transmitted,
*                               SERIAL_ERR_DRV_IO_ERR           Device octet NOT transmitted.
*
* Return(s)   : None.
*
* Caller(s)   : Serial_IF_Rx() via 'pdrv_api->RxOctet()'.
*
* Note(s)     : (1) Interrupts are assumed to be disabled when this function is called.
*********************************************************************************************************
*/

static  void  SerialDrv_RxOctet (SERIAL_DEV  *pdev,
                                 CPU_INT08U  *pdatum,
                                 SERIAL_ERR  *perr)
{
    SERIAL_REG      *p_reg;
    SERIAL_DEV_CFG  *p_cfg;


    p_cfg = pdev->Dev_Cfg;
    p_reg = (SERIAL_REG *)p_cfg->BaseAddr;
    if ((p_reg->SR & SR_RXNE) == 0) {                             /* If rx not rdy, return err.                           */
       *perr = SERIAL_ERR_DRV_IO_ERR;
        return;
    }

   *pdatum = p_reg->DR & 0xFF;
   *perr   = SERIAL_ERR_NONE;
}


/*$PAGE*/
/*
*********************************************************************************************************
*                                         SerialDrv_TxStart()
*
* Description : Start transmitting data & getting data transmitted interrupts.
*
* Argument(s) : pdev        Pointer to device.
*
*               perr        Pointer to variable that will receive the return error code from this function :
*
*                               SERIAL_ERR_NONE                 Device transmission started.
*
* Return(s)   : None.
*
* Caller(s)   : Serial_IF_Tx(),
*               Serial_WrStart() via 'pdev->Drv_API->TxStart()'.
*
* Note(s)     : (1) Interrupts are assumed to be disabled when this function is called.
*********************************************************************************************************
*/

static  void  SerialDrv_TxStart (SERIAL_DEV  *pdev,
                                 SERIAL_ERR  *perr)
{
    SERIAL_REG      *p_reg;
    SERIAL_DEV_CFG  *p_cfg;


    p_cfg = pdev->Dev_Cfg;
    p_reg = (SERIAL_REG *)p_cfg->BaseAddr;
    p_reg->CR1 |= CR1_TCIE;                                       /* En tx int.                                           */

   *perr = SERIAL_ERR_NONE;
}


/*$PAGE*/
/*
*********************************************************************************************************
*                                         SerialDrv_TxStop()
*
* Description : Stop transmitting data & getting data transmitted interrupts.
*
* Argument(s) : pdev        Pointer to device.
*
*               perr        Pointer to variable that will receive the return error code from this function :
*
*                               SERIAL_ERR_NONE                 Device transmission stopped.
*
* Return(s)   : None.
*
* Caller(s)   : Serial_Close(),
*               Serial_IF_Tx(),
*               Serial_WrStartNext() via 'pdev->Drv_API->TxStop()'.
*
* Note(s)     : (1) Interrupts are assumed to be disabled when this function is called.
*********************************************************************************************************
*/

static  void  SerialDrv_TxStop (SERIAL_DEV  *pdev,
                                SERIAL_ERR  *perr)
{
    SERIAL_REG      *p_reg;
    SERIAL_DEV_CFG  *p_cfg;


    p_cfg   = pdev->Dev_Cfg;
    p_reg = (SERIAL_REG *)p_cfg->BaseAddr;
    p_reg->CR1 &= ~CR1_TCIE;                                      /* Dis tx int.                                          */

   *perr = SERIAL_ERR_NONE;
}


/*$PAGE*/
/*
*********************************************************************************************************
*                                         SerialDrv_TxOctet()
*
* Description : Transmit data octet.
*
* Argument(s) : pdev        Pointer to device.
*
*               datum       Data octet to transmit.
*
*               perr        Pointer to variable that will receive the return error code from this function :
*
*                               SERIAL_ERR_NONE                 Device octet     transmitted,
*                               SERIAL_ERR_DRV_IO_ERR           Device octet NOT transmitted.
*
* Return(s)   : None.
*
* Caller(s)   : Serial_IF_Tx()   via      'pdrv_api->TxOctet()',
*               Serial_WrStart() via 'pdev->Drv_API->TxOctet()'.
*
* Note(s)     : (1) Interrupts are assumed to be disabled when this function is called.
*********************************************************************************************************
*/

static  void  SerialDrv_TxOctet (SERIAL_DEV  *pdev,
                                 CPU_INT08U   datum,
                                 SERIAL_ERR  *perr)
{
    SERIAL_REG      *p_reg;
    SERIAL_DEV_CFG  *p_cfg;


    p_cfg   = pdev->Dev_Cfg;
    p_reg = (SERIAL_REG *)p_cfg->BaseAddr;
    if ((p_reg->SR & SR_TXE) == 0) {                              /* If tx not rdy, return err.                           */
       *perr = SERIAL_ERR_DRV_IO_ERR;
        return;
    }

    p_reg->DR = datum;                                            /* Tx octet.                                            */

   *perr = SERIAL_ERR_NONE;
}


/*$PAGE*/
/*
*********************************************************************************************************
*                                       SerialDrv_ISR_Handler()
*
* Description : Handle serial interface's device interrupt service routine (ISR) function(s).
*
* Argument(s) : pdev        Pointer to device.
*
*               type        Device  interrupt type(s) to handle :
*
*                               SERIAL_ISR_TYPE_UNKNOWN         Handle unknown device           ISR(s).
*                               SERIAL_ISR_TYPE_RX              Handle device receive           ISR(s).
*                               SERIAL_ISR_TYPE_RX_OVERRUN      Handle device receive  overrun  ISR(s).
*                               SERIAL_ISR_TYPE_TX_RDY          Handle device transmit ready    ISR(s).
*                               SERIAL_ISR_TYPE_TX_COMPLETE     Handle device transmit complete ISR(s).
*                               SERIAL_ISR_TYPE_ERR             Handle device error             ISR(s).
*
* Return(s)   : None.
*
* Caller(s)   : Device driver(s)' Board Support Package (BSP) Interrupt Service Routine (ISR) handler(s).
*
* Note(s)     : None.
*********************************************************************************************************
*/

static  void  SerialDrv_ISR_Handler (SERIAL_DEV  *pdev,
                                     CPU_INT08U   type)
{
    SERIAL_REG      *p_reg;
    SERIAL_DEV_CFG  *p_cfg;
    CPU_INT08U       datum;
    CPU_INT16U       status;
    CPU_INT16U       mask;


   (void)&type;

    p_cfg  = pdev->Dev_Cfg;
    p_reg  = (SERIAL_REG *)p_cfg->BaseAddr;
    status =  p_reg->SR;
    mask   =  p_reg->CR1;
                                                                /* --------------- HANDLE WR COMPLETION --------------- */
    if (DEF_BIT_IS_SET(status, SR_TC) &&
        DEF_BIT_IS_SET(mask, CR1_TCIE)) {
        SerialIF_Tx(pdev);                                      /* Inform serial core of wr completion.                 */
    }


                                                                /* --------------- HANDLE RD COMPLETION --------------- */
    if (DEF_BIT_IS_SET(status, SR_RXNE) &&
        DEF_BIT_IS_SET(mask, CR1_RXNEIE)) {
        datum = p_reg->DR & 0xFF;
        SerialIF_Rx(pdev, datum);                               /* Inform serial core of rx completion.                 */
    } else if (DEF_BIT_IS_SET(status, SR_ORE)) {                /* Clr overrun condition.                               */
        datum = p_reg->DR & 0xFF;
    }
}
