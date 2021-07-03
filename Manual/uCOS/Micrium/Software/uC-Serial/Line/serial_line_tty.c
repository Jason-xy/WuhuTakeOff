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
* Filename      : serial_line_tty.c
* Version       : V2.00.01
* Programmer(s) : FGK
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#define   SERIAL_LINE_TTY_MODULE
#include  <serial.h>
#include  <serial_line_tty.h>


/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/

#define  SERIAL_LINE_TTY_TX_STATE_NORMAL                   0u
#define  SERIAL_LINE_TTY_TX_STATE_CR                       1u


/*
*********************************************************************************************************
*                                        FORWARD DECLARATIONS
*********************************************************************************************************
*/

typedef  struct  serial_line_tty_info  SERIAL_LINE_INFO;


/*
*********************************************************************************************************
*                                          LOCAL DATA TYPES
*********************************************************************************************************
*/

struct  serial_line_tty_info {
    SERIAL_LINE_INFO  *NextPtr;                                 /* Ptr to next serial line info struct.                 */

    CPU_INT08U         TxState;                                 /* Tx state machine state.                              */

    CPU_BOOLEAN        EchoRx;                                  /* Echo rx'd octet.                                     */
    CPU_BOOLEAN        EchoTx;                                  /* Echo tx'd octet.                                     */
};


/*
*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/

static  SERIAL_LINE_INFO  *SerialLine_TTY_Info = (SERIAL_LINE_INFO *)0;


/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/
                                                                /* Open  line driver.                                   */
static  void  *SerialLine_Open   (SERIAL_DEV  *pdev,
                                  SERIAL_ERR  *perr);
                                                                /* Close line driver.                                   */
static  void   SerialLine_Close  (SERIAL_DEV  *pdev,
                                  void        *pline_data,
                                  SERIAL_ERR  *perr);
                                                                /* Rd octet from buf via protocol.                      */
static  void   SerialLine_RdOctet(SERIAL_DEV  *pdev,
                                  SERIAL_BUF  *pbuf,
                                  CPU_INT08U  *pdatum,
                                  SERIAL_ERR  *perr);
                                                                /* Wr octet into buf via protocol.                      */
static  void   SerialLine_WrOctet(SERIAL_DEV  *pdev,
                                  SERIAL_BUF  *pbuf,
                                  CPU_INT08U   datum,
                                  SERIAL_ERR  *perr);


/*
*********************************************************************************************************
*********************************************************************************************************
*                                  SERIAL INTERFACE LINE DRIVER API
*********************************************************************************************************
*********************************************************************************************************
*/

SERIAL_LINE_DRV_API  SerialLine_TTY = {
    SerialLine_Open,
    SerialLine_Close,
    SerialLine_RdOctet,
    SerialLine_WrOctet
};


/*$PAGE*/
/*
*********************************************************************************************************
*********************************************************************************************************
*                                     DRIVER AUXILIARY FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                       SerialLine_TTY_EchoRx()
*
* Description : Set receive echo control.
*
* Argument(s) : if_nbr      Interface number.
*
*               echo        Echo control enable or disable: DEF_ENABLED, DEF_DISABLED.
*
* Return(s)   : None.
*
* Caller(s)   : Application.
*
* Note(s)     : None.
*********************************************************************************************************
*/

void  SerialLine_TTY_EchoRx (SERIAL_IF_NBR  if_nbr,
                             CPU_BOOLEAN    echo)
{
    SERIAL_DEV        *pdev;
    SERIAL_LINE_INFO  *pinfo;
    SERIAL_ERR         err;


    pdev = SerialIF_GetDevPtr(if_nbr, &err);
    if (err != SERIAL_ERR_NONE) {
        return;
    }

                                                                /* -------------- VALIDATE LINE DRV TYPE -------------- */
    if (pdev->LineDrv_API != &SerialLine_TTY) {
        return;
    }

    pinfo = (SERIAL_LINE_INFO *)pdev->LineDrv_Data;
    pinfo->EchoRx = echo;
}


/*
*********************************************************************************************************
*                                       SerialLine_TTY_EchoTx()
*
* Description : Set transmit echo control.
*
* Argument(s) : if_nbr      Interface number.
*
*               echo        Echo control enable or disable: DEF_ENABLED, DEF_DISABLED.
*
* Return(s)   : None.
*
* Caller(s)   : Application.
*
* Note(s)     : None.
*********************************************************************************************************
*/

void  SerialLine_TTY_EchoTx (SERIAL_IF_NBR  if_nbr,
                             CPU_BOOLEAN    echo)
{
    SERIAL_DEV        *pdev;
    SERIAL_LINE_INFO  *pinfo;
    SERIAL_ERR         err;


    pdev = SerialIF_GetDevPtr(if_nbr, &err);
    if (err != SERIAL_ERR_NONE) {
        return;
    }

                                                                /* -------------- VALIDATE LINE DRV TYPE -------------- */
    if (pdev->LineDrv_API != &SerialLine_TTY) {
        return;
    }

    pinfo = (SERIAL_LINE_INFO *)pdev->LineDrv_Data;
    pinfo->EchoTx = echo;
}


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
*                                          SerialLine_Open()
*
* Description : Open serial line driver.
*
* Argument(s) : pdev        Pointer to device.
*
*               perr        Pointer to variable that will receive the return error code from this function :
*
*                               SERIAL_ERR_NONE                 Serial line driver              opened.
*                               SERIAL_ERR_LINE_OPEN            Serial line driver could NOT be opened.
*
* Return(s)   : Pointer to line driver data structure.
*
* Caller(s)   : Serial_Open().
*
* Note(s)     : (1) Interrupts are assumed to be disabled when this function is called.
*********************************************************************************************************
*/

static  void  *SerialLine_Open (SERIAL_DEV  *pdev,
                                SERIAL_ERR  *perr)
{
    SERIAL_LINE_INFO  *pinfo;
    CPU_SIZE_T         octets_reqd;
    LIB_ERR            lib_err;


   (void)&pdev;

    pinfo = SerialLine_TTY_Info;
    if (pinfo == (SERIAL_LINE_INFO *)0) {                       /* Find avail TTY info struct.                          */
        pinfo  = (SERIAL_LINE_INFO *)Mem_HeapAlloc((CPU_SIZE_T  ) sizeof(SERIAL_LINE_INFO),
                                                   (CPU_SIZE_T  ) sizeof(CPU_ALIGN),
                                                   (CPU_SIZE_T *)&octets_reqd,
                                                   (LIB_ERR    *)&lib_err);
        if (pinfo == (SERIAL_LINE_INFO *)0) {
           *perr = SERIAL_ERR_LINE_DRV_OPEN;
            return ((void *)0);
        }
    } else {
        SerialLine_TTY_Info = pinfo->NextPtr;
    }

    pinfo->NextPtr = (SERIAL_LINE_INFO *)0;
    pinfo->TxState =  SERIAL_LINE_TTY_TX_STATE_NORMAL;
    pinfo->EchoRx  =  DEF_ENABLED;
    pinfo->EchoTx  =  DEF_DISABLED;

   *perr = SERIAL_ERR_NONE;

    return ((void *)pinfo);
}


/*$PAGE*/
/*
*********************************************************************************************************
*                                         SerialLine_Close()
*
* Description : Close serial line driver.
*
* Argument(s) : pdev        Pointer to device.
*
*               pline_data  Pointer to line driver data structure.
*
*               perr        Pointer to variable that will receive the return error code from this function :
*
*                               SERIAL_ERR_NONE                 Serial line driver closed.
*
* Return(s)   : None.
*
* Caller(s)   : Serial_Close().
*
* Note(s)     : (1) Interrupts are assumed to be disabled when this function is called.
*********************************************************************************************************
*/

static  void  SerialLine_Close (SERIAL_DEV  *pdev,
                                void        *pline_data,
                                SERIAL_ERR  *perr)
{
    SERIAL_LINE_INFO  *pinfo;


   (void)&pdev;

    pinfo = (SERIAL_LINE_INFO *)pline_data;

    pinfo->TxState = SERIAL_LINE_TTY_TX_STATE_NORMAL;
    pinfo->EchoRx  = DEF_DISABLED;
    pinfo->EchoTx  = DEF_DISABLED;

    pinfo->NextPtr      = SerialLine_TTY_Info;
    SerialLine_TTY_Info = pinfo;

   *perr = SERIAL_ERR_NONE;
}


/*$PAGE*/
/*
*********************************************************************************************************
*                                        SerialLine_RdOctet()
*
* Description : Get next octet to transmit.
*
* Argument(s) : pdev        Pointer to device.
*
*               pbuf        Pointer to buffer.
*
*               pdatum      Pointer to variable that will receive data octet.
*
*               perr        Pointer to variable that will receive the return error code from this function :
*
*                               SERIAL_ERR_NONE                 Octet supplied.
*                               SERIAL_ERR_UNDERFLOW            Buffer underflow.
*
* Return(s)   : None.
*
* Caller(s)   : Serial_IF_Tx().
*
* Note(s)     : (1) Interrupts are assumed to be disabled when this function is called.
*********************************************************************************************************
*/

static  void  SerialLine_RdOctet (SERIAL_DEV  *pdev,
                                  SERIAL_BUF  *pbuf,
                                  CPU_INT08U  *pdatum,
                                  SERIAL_ERR  *perr)
{
    SERIAL_LINE_INFO  *pinfo;
    CPU_BOOLEAN        rd;


    pinfo = (SERIAL_LINE_INFO *)pdev->LineDrv_Data;
    switch (pinfo->TxState) {
        case SERIAL_LINE_TTY_TX_STATE_CR:                       /* ---------------- TX CARRIAGE RETURN ---------------- */
             pinfo->TxState = SERIAL_LINE_TTY_TX_STATE_NORMAL;
            *pdatum         = ASCII_CHAR_CARRIAGE_RETURN;
            *perr           = SERIAL_ERR_NONE;
             return;


        case SERIAL_LINE_TTY_TX_STATE_NORMAL:                   /* --------------------- TX OCTETS -------------------- */
             rd = SerialBuf_RdOctet(pbuf, pdatum);
             if (rd == DEF_NO) {
                *perr = SERIAL_ERR_UNDERFLOW;
                 return;
             }
             if (*pdatum == ASCII_CHAR_LINE_FEED) {
                 pinfo->TxState = SERIAL_LINE_TTY_TX_STATE_CR;
             }
             break;
    }

#if (SERIAL_CFG_RD_BUF_EN == DEF_ENABLED)                       /* Echo data.                                           */
    pinfo = (SERIAL_LINE_INFO *)pdev->LineDrv_Data;
    if (pinfo->EchoTx == DEF_ENABLED) {
        SerialIF_Rx(pdev, *pdatum);
    }
#endif

   *perr = SERIAL_ERR_NONE;
}


/*$PAGE*/
/*
*********************************************************************************************************
*                                        SerialLine_WrOctet()
*
* Description : Handle octet received from serial port.
*
* Argument(s) : pdev        Pointer to device.
*
*               pbuf        Pointer to buffer.
*
*               datum       Data octet to write.
*
*               perr        Pointer to variable that will receive the return error code from this function :
*
*                               SERIAL_ERR_NONE                 Octet handled.
*                               SERIAL_ERR_OVERFLOW             Buffer overflow.
*                               SERIAL_ERR_LINE_END             End of line detected.
*
* Return(s)   : None.
*
* Caller(s)   : Serial_IF_Rx().
*
* Note(s)     : (1) Interrupts are assumed to be disabled when this function is called.
*********************************************************************************************************
*/

static  void  SerialLine_WrOctet (SERIAL_DEV  *pdev,
                                  SERIAL_BUF  *pbuf,
                                  CPU_INT08U   datum,
                                  SERIAL_ERR  *perr)
{
#if (SERIAL_CFG_WR_BUF_EN == DEF_ENABLED)
    SERIAL_LINE_INFO  *pinfo;
#endif
    CPU_BOOLEAN        wr;


   (void)&pdev;

    if ((datum == ASCII_CHAR_LINE_FEED) ||
        (datum == ASCII_CHAR_CARRIAGE_RETURN)) {
        wr = SerialBuf_WrOctet(pbuf, ASCII_CHAR_NULL);
       *perr = SERIAL_ERR_LINE_END;
        return;
    }

    wr = SerialBuf_WrOctet(pbuf, datum);
    if (wr == DEF_NO) {
       *perr = SERIAL_ERR_OVERFLOW;
        return;
    }

#if (SERIAL_CFG_WR_BUF_EN == DEF_ENABLED)                       /* Echo data.                                           */
    pinfo = (SERIAL_LINE_INFO *)pdev->LineDrv_Data;
    if (pinfo->EchoRx == DEF_ENABLED) {
        SerialIF_TxWrBuf(pdev, datum);
    }
#endif

   *perr = SERIAL_ERR_NONE;
}
