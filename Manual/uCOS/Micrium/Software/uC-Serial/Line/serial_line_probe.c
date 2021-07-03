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
* Filename      : serial_line_probe.c
* Version       : V2.00.01
* Programmer(s) : FGK
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#define    SERIAL_LINE_PROBE_MODULE
#include  <serial.h>
#include  <serial_line_probe.h>


/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                        SERIAL PACKET FORMAT
*
* Note(s):  (1) All packets include the following parts:
*
*                   (A)  4 1-byte start delimiters, forming the ASCII representation of "uCPr".  These
*                        are the constants PROBE_SERIAL_PROTOCOL_?X_SD0-PROBE_SERIAL_PROTOCOL_?X_SD4;
*                   (B)  1 2-byte length, the length of the data segment;
*                   (C)  1 2-byte padding, unused;
*                   (D)  n   bytes of data; and
*                   (E)  1 1-byte checksum; and
*                   (F)  1 1-byte end delimiter, the character '/', which is the constant PROBE_SERIAL_PROTOCOL_?X_ED.
*
*                                       +-------------------+-------------------+
*                                       |   'u'   |   'C'   |   'P'   |   'r'   |
*                                       +-------------------+-------------------+
*                                       |       Length      | PktCtr  | Padding |
*                                       +-------------------+-------------------+
*                                       |                  Data                 |   The data segment does not need to end on
*                                       |                   .                   |   a four-byte boundary, as might be inferred
*                                       |                   .                   |   from this diagram.
*                                       |                   .                   |
*                                       +-------------------+-------------------+
*                                       | Checksum|   '/'   |
*                                       +-------------------+
*********************************************************************************************************
*/

                                                                /* ------------- INBOUND PACKET DELIMITERS ------------ */
#define  SERIAL_LINE_PROBE_PROTOCOL_RX_SD0              0x75    /* Start delimiters.                                    */
#define  SERIAL_LINE_PROBE_PROTOCOL_RX_SD1              0x43
#define  SERIAL_LINE_PROBE_PROTOCOL_RX_SD2              0x50
#define  SERIAL_LINE_PROBE_PROTOCOL_RX_SD3              0x72
#define  SERIAL_LINE_PROBE_PROTOCOL_RX_ED               0x2F    /* End   delimiter.                                     */

                                                                /* ------------ OUTBOUND PACKET DELIMITERS ------------ */
#define  SERIAL_LINE_PROBE_PROTOCOL_TX_SD0              0x75    /* Start delimiters.                                    */
#define  SERIAL_LINE_PROBE_PROTOCOL_TX_SD1              0x43
#define  SERIAL_LINE_PROBE_PROTOCOL_TX_SD2              0x50
#define  SERIAL_LINE_PROBE_PROTOCOL_TX_SD3              0x72
#define  SERIAL_LINE_PROBE_PROTOCOL_TX_ED               0x2F    /* End   delimiter.                                     */

                                                                /* ----------- RECEIVE STATE MACHINE STATES ----------- */
#define  SERIAL_LINE_PROBE_RX_STATE_SD0                    0    /* Waiting for start first  start delimiter (SD0).      */
#define  SERIAL_LINE_PROBE_RX_STATE_SD1                    1    /* Waiting for start second start delimiter (SD1).      */
#define  SERIAL_LINE_PROBE_RX_STATE_SD2                    2    /* Waiting for start third  start delimiter (SD2).      */
#define  SERIAL_LINE_PROBE_RX_STATE_SD3                    3    /* Waiting for start fourth start delimiter (SD3).      */
#define  SERIAL_LINE_PROBE_RX_STATE_LEN1                   4    /* Waiting for length,  first  byte.                    */
#define  SERIAL_LINE_PROBE_RX_STATE_LEN2                   5    /* Waiting for length,  second byte.                    */
#define  SERIAL_LINE_PROBE_RX_STATE_CTR                    6    /* Waiting for packet counter.                          */
#define  SERIAL_LINE_PROBE_RX_STATE_PAD                    7    /* Waiting for padding byte.                            */
#define  SERIAL_LINE_PROBE_RX_STATE_DATA                   8    /* Waiting for data.                                    */
#define  SERIAL_LINE_PROBE_RX_STATE_CHKSUM                 9    /* Waiting for checksum.                                */
#define  SERIAL_LINE_PROBE_RX_STATE_ED                    10    /* Waiting for end delimiter.                           */

                                                                /* ---------- TRANSMIT STATE MACHINE STATES ----------- */
#define  SERIAL_LINE_PROBE_TX_STATE_SD0                    0    /* Waiting to send start first  start delim. (SD0).     */
#define  SERIAL_LINE_PROBE_TX_STATE_SD1                    1    /* Waiting to send start second start delim. (SD1).     */
#define  SERIAL_LINE_PROBE_TX_STATE_SD2                    2    /* Waiting to send start third  start delim. (SD2).     */
#define  SERIAL_LINE_PROBE_TX_STATE_SD3                    3    /* Waiting to send start fourth start delim. (SD3).     */
#define  SERIAL_LINE_PROBE_TX_STATE_LEN1                   4    /* Waiting to send length,  first  byte.                */
#define  SERIAL_LINE_PROBE_TX_STATE_LEN2                   5    /* Waiting to send length,  second byte.                */
#define  SERIAL_LINE_PROBE_TX_STATE_CTR                    6    /* Waiting to send packet counter.                      */
#define  SERIAL_LINE_PROBE_TX_STATE_PAD                    7    /* Waiting to send padding byte.                        */
#define  SERIAL_LINE_PROBE_TX_STATE_DATA                   8    /* Waiting to send data.                                */
#define  SERIAL_LINE_PROBE_TX_STATE_CHKSUM                 9    /* Waiting to send checksum.                            */
#define  SERIAL_LINE_PROBE_TX_STATE_ED                    10    /* Waiting to send end delimiter.                       */
#define  SERIAL_LINE_PROBE_TX_STATE_LAST                  11    /* Final byte sent.                                     */

#define  SERIAL_LINE_PROBE_USE_CHECKSUM            DEF_FALSE    /* DO NOT CHANGE.                                       */


/*
*********************************************************************************************************
*                                        FORWARD DECLARATIONS
*********************************************************************************************************
*/

typedef  struct  serial_line_probe_info  SERIAL_LINE_INFO;


/*
*********************************************************************************************************
*                                          LOCAL DATA TYPES
*********************************************************************************************************
*/

struct  serial_line_probe_info {
    SERIAL_LINE_INFO  *NextPtr;                                 /* Ptr to next serial line info struct.                 */

    CPU_INT08U         RxState;                                 /* Rx state machine state.                              */
    CPU_SIZE_T         RxLenRem;
    CPU_INT08U         RxPktCtr;
#if (SERIAL_LINE_PROBE_USE_CHECKSUM == DEF_TRUE)
    CPU_INT08U         RxChkSum;
#endif
    CPU_INT08U         TxState;                                 /* Tx state machine state.                              */
#if (SERIAL_LINE_PROBE_USE_CHECKSUM == DEF_TRUE)
    CPU_INT08U         TxChkSum;
#endif
};


/*
*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/

static  SERIAL_LINE_INFO  *SerialLine_Probe_Info = (SERIAL_LINE_INFO *)0;


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

SERIAL_LINE_DRV_API  SerialLine_Probe = {
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
* Return(s)   : Pointer to line driver data.
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

    pinfo = SerialLine_Probe_Info;
    if (pinfo == (SERIAL_LINE_INFO *)0) {                       /* Find avail Probe info struct.                        */
        pinfo  = (SERIAL_LINE_INFO *)Mem_HeapAlloc((CPU_SIZE_T  ) sizeof(SERIAL_LINE_INFO),
                                                   (CPU_SIZE_T  ) sizeof(CPU_ALIGN),
                                                   (CPU_SIZE_T *)&octets_reqd,
                                                   (LIB_ERR    *)&lib_err);
        if (pinfo == (SERIAL_LINE_INFO *)0) {
           *perr = SERIAL_ERR_LINE_DRV_OPEN;
            return ((void *)0);
        }
    } else {
        SerialLine_Probe_Info = pinfo->NextPtr;
    }

    pinfo->NextPtr  = (SERIAL_LINE_INFO *)0;

    pinfo->RxState  = SERIAL_LINE_PROBE_RX_STATE_SD0;
    pinfo->RxLenRem = 0;
    pinfo->RxPktCtr = 0;
#if (SERIAL_LINE_PROBE_USE_CHECKSUM == DEF_TRUE)
    pinfo->RxChkSum = 0;
#endif

    pinfo->TxState  = SERIAL_LINE_PROBE_TX_STATE_SD0;
#if (SERIAL_LINE_PROBE_USE_CHECKSUM == DEF_TRUE)
    pinfo->TxChkSum = 0;
#endif

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

    pinfo->RxState  = SERIAL_LINE_PROBE_RX_STATE_SD0;
    pinfo->RxLenRem = 0;
#if (SERIAL_LINE_PROBE_USE_CHECKSUM == DEF_TRUE)
    pinfo->RxChkSum = 0;
#endif

    pinfo->TxState  = SERIAL_LINE_PROBE_TX_STATE_SD0;
#if (SERIAL_LINE_PROBE_USE_CHECKSUM == DEF_TRUE)
    pinfo->TxChkSum = 0;
#endif


    pinfo->NextPtr        = SerialLine_Probe_Info;
    SerialLine_Probe_Info = pinfo;


   *perr = SERIAL_ERR_NONE;
}


/*$PAGE*/
/*
*********************************************************************************************************
*                                        SerialLine_RdOctet()
*
* Description : Get next octet to transmit.
*
* Argument(s) : pdev        Pointer to device .
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
    CPU_BOOLEAN        empty;
    CPU_INT08U         datum;
    CPU_BOOLEAN        rd;
    CPU_INT08U         state;
    CPU_INT08U         state_next;
    SERIAL_LINE_INFO  *pinfo;


    pinfo = (SERIAL_LINE_INFO *)pdev->LineDrv_Data;

    state      = pinfo->TxState;
    state_next = state;

    switch (state) {
        case SERIAL_LINE_PROBE_TX_STATE_SD0:                    /* Tx start 1st start delimiter (SD0).                  */
             empty = SerialBuf_IsEmpty(pbuf);
             if (empty == DEF_YES) {
                *perr = SERIAL_ERR_UNDERFLOW;
                 return;
             }

             datum      = SERIAL_LINE_PROBE_PROTOCOL_TX_SD0;
             state_next = SERIAL_LINE_PROBE_TX_STATE_SD1;
             break;


        case SERIAL_LINE_PROBE_TX_STATE_SD1:                    /* Tx start 2nd start delimiter (SD1).                  */
             datum      = SERIAL_LINE_PROBE_PROTOCOL_TX_SD1;
             state_next = SERIAL_LINE_PROBE_TX_STATE_SD2;
             break;


        case SERIAL_LINE_PROBE_TX_STATE_SD2:                    /* Tx start 3rd start delimiter (SD2).                  */
             datum      = SERIAL_LINE_PROBE_PROTOCOL_TX_SD2;
             state_next = SERIAL_LINE_PROBE_TX_STATE_SD3;
             break;


        case SERIAL_LINE_PROBE_TX_STATE_SD3:                    /* Tx start 4th start delimiter (SD3).                  */
             datum      = SERIAL_LINE_PROBE_PROTOCOL_TX_SD3;
             state_next = SERIAL_LINE_PROBE_TX_STATE_LEN1;
             break;


        case SERIAL_LINE_PROBE_TX_STATE_LEN1:                   /* Tx 1st len byte.                                     */
             datum      = SerialBuf_Size(pbuf) & 0xFF;
             state_next = SERIAL_LINE_PROBE_TX_STATE_LEN2;
#if (SERIAL_LINE_PROBE_USE_CHECKSUM == DEF_TRUE)
             pinfo->TxChkSum = datum;
#endif
             break;


        case SERIAL_LINE_PROBE_TX_STATE_LEN2:                   /* Tx 2nd len byte.                                     */
             datum      = (CPU_INT08U)(SerialBuf_Size(pbuf) >> 8);
             state_next =  SERIAL_LINE_PROBE_TX_STATE_CTR;
#if (SERIAL_LINE_PROBE_USE_CHECKSUM == DEF_TRUE)
             pinfo->TxChkSum += datum;
#endif
             break;


        case SERIAL_LINE_PROBE_TX_STATE_CTR:                   /* Tx packet counter.                                    */
             datum      = pinfo->RxPktCtr;
             state_next = SERIAL_LINE_PROBE_TX_STATE_PAD;
#if (SERIAL_LINE_PROBE_USE_CHECKSUM == DEF_TRUE)
             pinfo->TxChkSum += datum;
#endif
             break;


        case SERIAL_LINE_PROBE_TX_STATE_PAD:                    /* Tx padding byte.                                     */
             datum      = 0;
             state_next = SERIAL_LINE_PROBE_TX_STATE_DATA;
#if (SERIAL_LINE_PROBE_USE_CHECKSUM == DEF_TRUE)
             pinfo->TxChkSum += datum;
#endif
             break;


        case SERIAL_LINE_PROBE_TX_STATE_DATA:                   /* Tx data.                                             */
             rd = SerialBuf_RdOctet(pbuf, &datum);
             if (rd == DEF_NO) {
                 datum      = 0;
                 state_next = SERIAL_LINE_PROBE_TX_STATE_ED;
             } else {
#if (SERIAL_LINE_PROBE_USE_CHECKSUM == DEF_TRUE)
                 pinfo->TxChkSum += datum;
#endif
                 state_next = SERIAL_LINE_PROBE_TX_STATE_DATA;
             }
             break;


        case SERIAL_LINE_PROBE_TX_STATE_CHKSUM:                 /* Tx checksum.                                         */
#if (SERIAL_LINE_PROBE_USE_CHECKSUM == DEF_TRUE)
             datum      = pinfo->TxChkSum;
#else
             datum      = 0;
#endif
             state_next = SERIAL_LINE_PROBE_TX_STATE_ED;
             break;


        case SERIAL_LINE_PROBE_TX_STATE_ED:                     /* Tx end delimiter.                                    */
             datum      = SERIAL_LINE_PROBE_PROTOCOL_TX_ED;
             state_next = SERIAL_LINE_PROBE_TX_STATE_LAST;
             break;


        default:
        case SERIAL_LINE_PROBE_TX_STATE_LAST:
             pinfo->TxState = SERIAL_LINE_PROBE_TX_STATE_SD0;
            *pdatum         = 0;
            *perr = SERIAL_ERR_UNDERFLOW;
             return;
    }

    pinfo->TxState = state_next;
   *pdatum         = datum;
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
    CPU_SIZE_T         len_rem;
    CPU_INT08U         state_next;
    CPU_BOOLEAN        wr;
    CPU_SIZE_T         len;
    SERIAL_LINE_INFO  *pinfo;


    pinfo = (SERIAL_LINE_INFO *)pdev->LineDrv_Data;

    switch (pinfo->RxState) {
        case SERIAL_LINE_PROBE_RX_STATE_SD0:                    /* Receive the start first  start delimiter (SD0).      */
             if (datum == SERIAL_LINE_PROBE_PROTOCOL_RX_SD0) {
                 state_next = SERIAL_LINE_PROBE_RX_STATE_SD1;
             } else {
                 state_next = SERIAL_LINE_PROBE_RX_STATE_SD0;
             }
             break;


        case SERIAL_LINE_PROBE_RX_STATE_SD1:                    /* Receive the start second start delimiter (SD1).      */
             if (datum == SERIAL_LINE_PROBE_PROTOCOL_RX_SD1) {
                 state_next = SERIAL_LINE_PROBE_RX_STATE_SD2;
             } else {
                 state_next = SERIAL_LINE_PROBE_RX_STATE_SD0;
             }
             break;


        case SERIAL_LINE_PROBE_RX_STATE_SD2:                    /* Receive the start third  start delimiter (SD2).      */
             if (datum == SERIAL_LINE_PROBE_PROTOCOL_RX_SD2) {
                 state_next = SERIAL_LINE_PROBE_RX_STATE_SD3;
             } else {
                 state_next = SERIAL_LINE_PROBE_RX_STATE_SD0;
             }
             break;


        case SERIAL_LINE_PROBE_RX_STATE_SD3:                    /* Receive the start fourth start delimiter (SD3).      */
             if (datum == SERIAL_LINE_PROBE_PROTOCOL_RX_SD3) {
                 state_next = SERIAL_LINE_PROBE_RX_STATE_LEN1;
             } else {
                 state_next = SERIAL_LINE_PROBE_RX_STATE_SD0;
             }
             break;


        case SERIAL_LINE_PROBE_RX_STATE_LEN1:                   /* Receive the first  length byte.                      */
#if (SERIAL_LINE_PROBE_USE_CHECKSUM == DEF_TRUE)
             pinfo->RxChkSum = datum;
#endif
             pinfo->RxLenRem = datum;
             state_next      = SERIAL_LINE_PROBE_RX_STATE_LEN2;
             break;


        case SERIAL_LINE_PROBE_RX_STATE_LEN2:                   /* Receive the second length byte.                      */
#if (SERIAL_LINE_PROBE_USE_CHECKSUM == DEF_TRUE)
             pinfo->RxChkSum += datum;
#endif
             pinfo->RxLenRem |= datum << 8;

             len = SerialBuf_Size(pbuf);

             if ((pinfo->RxLenRem == 0) ||
                 (pinfo->RxLenRem > len)) {
                                                                /* ... Cannot handle a packet of this size.             */
                 state_next = SERIAL_LINE_PROBE_RX_STATE_SD0;
             } else {
                 state_next = SERIAL_LINE_PROBE_RX_STATE_CTR;
             }
             break;


        case SERIAL_LINE_PROBE_RX_STATE_CTR:                    /* Receive packet counter.                              */
             pinfo->RxPktCtr = datum;
             state_next      = SERIAL_LINE_PROBE_RX_STATE_PAD;
             break;


        case SERIAL_LINE_PROBE_RX_STATE_PAD:                    /* Receive the padding byte.                            */
             state_next = SERIAL_LINE_PROBE_RX_STATE_DATA;
             break;


        case SERIAL_LINE_PROBE_RX_STATE_DATA:                   /* Receive the data.                                    */
             wr = SerialBuf_WrOctet(pbuf, datum);
             if (wr == DEF_NO) {
                 pinfo->RxState = SERIAL_LINE_PROBE_RX_STATE_SD0;
                *perr = SERIAL_ERR_OVERFLOW;
                 return;
             }

#if (SERIAL_LINE_PROBE_USE_CHECKSUM == DEF_TRUE)
             pinfo->RxChkSum += datum;
#endif
             len_rem = pinfo->RxLenRem - 1;
             if (len_rem == 0) {
                 state_next = SERIAL_LINE_PROBE_RX_STATE_CHKSUM;
             } else {
                 state_next = SERIAL_LINE_PROBE_RX_STATE_DATA;
             }
             pinfo->RxLenRem = len_rem;
             break;


        case SERIAL_LINE_PROBE_RX_STATE_CHKSUM:             /* Receive the checksum.                                */
#if (SERIAL_LINE_PROBE_USE_CHECKSUM == DEF_TRUE)
             pinfo->RxChkSum += datum;
             if ((pinfo->RxChkSum & 0xFF) == 0x00) {
                 state_next = SERIAL_LINE_PROBE_RX_STATE_ED;
             } else {
                 state_next = SERIAL_LINE_PROBE_RX_STATE_SD0;
                *perr = SERIAL_ERR_LINE_INVALID;
                 return;
             }
#else
             state_next = SERIAL_LINE_PROBE_RX_STATE_ED;
#endif
             break;


        case SERIAL_LINE_PROBE_RX_STATE_ED:                     /* Receive the end delimiter.                           */
             if (datum == SERIAL_LINE_PROBE_PROTOCOL_RX_ED) {
                *perr = SERIAL_ERR_LINE_END;
             } else {
                *perr = SERIAL_ERR_LINE_INVALID;
             }
             pinfo->RxState = SERIAL_LINE_PROBE_RX_STATE_SD0;
             return;


        default:
             state_next = SERIAL_LINE_PROBE_RX_STATE_SD0;
             break;
    }

    pinfo->RxState = state_next;
   *perr = SERIAL_ERR_NONE;
}
