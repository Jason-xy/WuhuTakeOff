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
* Filename      : serial_buf.h
* Version       : V2.00.01
* Programmer(s) : FGK
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                               MODULE
*
* Note(s) : (1) This header file is protected from multiple pre-processor inclusion through use of the
*               SERIAL_BUF present pre-processor macro definition.
*********************************************************************************************************
*/

#ifndef   SERIAL_BUF_PRESENT
#define   SERIAL_BUF_PRESENT


/*
*********************************************************************************************************
*                                               EXTERNS
*********************************************************************************************************
*/

#ifdef    SERIAL_BUF_MODULE
#define   SERIAL_BUF_EXT
#else
#define   SERIAL_BUF_EXT  extern
#endif


/*
*********************************************************************************************************
*                                       SERIAL BUFFER DATA TYPE
*********************************************************************************************************
*/

typedef  struct  serial_buf {
    CPU_SIZE_T   Len;                                           /* Buf len.                                             */
    CPU_SIZE_T   EmptyCnt;                                      /* Empty entries cnt.                                   */
    CPU_SIZE_T   IxRd;                                          /* Next rd ix.                                          */
    CPU_SIZE_T   IxWr;                                          /* Next wr ix.                                          */
    CPU_INT08U  *DataPtr;                                       /* Ptr to buf data area.                                */
} SERIAL_BUF;


/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/

void         SerialBuf_Init   (SERIAL_BUF   *pbuf,              /* Init buf.                                            */
                               CPU_INT08U   *pdata,
                               CPU_SIZE_T    len,
                               CPU_BOOLEAN   full);

void         SerialBuf_Clr    (SERIAL_BUF   *pbuf);             /* Clr buf.                                             */

CPU_SIZE_T   SerialBuf_Rd     (SERIAL_BUF   *pbuf,              /* Rd from buf.                                         */
                               CPU_INT08U   *psrc,
                               CPU_SIZE_T    len);

CPU_BOOLEAN  SerialBuf_RdOctet(SERIAL_BUF   *pbuf,              /* Rd octet from buf.                                   */
                               CPU_INT08U   *pdatum);

CPU_BOOLEAN  SerialBuf_WrOctet(SERIAL_BUF   *pbuf,              /* Wr octet to buf.                                     */
                               CPU_INT08U    datum);

CPU_BOOLEAN  SerialBuf_Cmp    (SERIAL_BUF   *pbuf,              /* Cmp incoming octets to buf.                          */
                               CPU_INT08U    datum);

CPU_BOOLEAN  SerialBuf_IsEmpty(SERIAL_BUF   *pbuf);             /* Determine whether buf is empty.                      */

CPU_BOOLEAN  SerialBuf_IsFull (SERIAL_BUF   *pbuf);             /* Determine whether buf is full.                       */

CPU_SIZE_T   SerialBuf_Size   (SERIAL_BUF   *pbuf);             /* Retrieve buf size.                                   */

CPU_SIZE_T   SerialBuf_DataLen(SERIAL_BUF   *pbuf);             /* Determine buf data area len.                         */


/*
*********************************************************************************************************
*                                             MODULE END
*********************************************************************************************************
*/

#endif                                                          /* End of SERIAL_BUF module include.                    */
