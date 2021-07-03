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
* Filename      : serial_line_dflt.c
* Version       : V2.00.01
* Programmer(s) : FGK
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#define    SERIAL_LINE_DFLT_MODULE
#include  <serial.h>


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

SERIAL_LINE_DRV_API  SerialLine_Dflt = {
    SerialLine_Open,
    SerialLine_Close,
    SerialLine_RdOctet,
    SerialLine_WrOctet
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
* Note(s)     : 1) Interrupts are assumed to be disabled when this function is called.
*********************************************************************************************************
*/

static  void  *SerialLine_Open (SERIAL_DEV  *pdev,
                                SERIAL_ERR  *perr)
{
   (void)&pdev;

   *perr = SERIAL_ERR_NONE;

    return ((void *)0);
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
* Note(s)     : 1) Interrupts are assumed to be disabled when this function is called.
*********************************************************************************************************
*/

static  void  SerialLine_Close (SERIAL_DEV  *pdev,
                                void        *pline_data,
                                SERIAL_ERR  *perr)
{
   (void)&pdev;
   (void)&pline_data;

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
    CPU_BOOLEAN  rd;


   (void)&pdev;

    rd = SerialBuf_RdOctet(pbuf, pdatum);
    if (rd == DEF_NO) {
       *perr = SERIAL_ERR_UNDERFLOW;
        return;
    }

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
    CPU_BOOLEAN  wr;


   (void)&pdev;

    wr = SerialBuf_WrOctet(pbuf, datum);
    if (wr == DEF_NO) {
       *perr = SERIAL_ERR_OVERFLOW;
        return;
    }

   *perr = SERIAL_ERR_NONE;
}
