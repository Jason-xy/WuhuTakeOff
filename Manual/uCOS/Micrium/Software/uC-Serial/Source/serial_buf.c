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
* Filename      : serial_buf.c
* Version       : V2.00.01
* Programmer(s) : FGK
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#define    SERIAL_BUF_MODULE
#include  <serial.h>


/*$PAGE*/
/*
*********************************************************************************************************
*                                          SerialBuf_Init()
*
* Description : Initialize buffer.
*
* Argument(s) : pbuf        Pointer to buffer.
*
*               pdata       Pointer to buffer data.
*
*               len         Buffer length.
*
*               full        Indicates that buffer is full upon initialization.
*
* Return(s)   : None.
*
* Caller(s)   : Application.
*
* Note(s)     : None.
*********************************************************************************************************
*/

void  SerialBuf_Init (SERIAL_BUF   *pbuf,
                      CPU_INT08U   *pdata,
                      CPU_SIZE_T    len,
                      CPU_BOOLEAN   full)
{
    pbuf->Len      = len;
    pbuf->EmptyCnt = ((full == DEF_YES) ? (0) : (len));
    pbuf->IxRd     = 0;
    pbuf->IxWr     = 0;
    pbuf->DataPtr  = pdata;
}


/*$PAGE*/
/*
*********************************************************************************************************
*                                           SerialBuf_Clr()
*
* Description : Clear buffer.
*
* Argument(s) : pbuf        Pointer to buffer.
*
* Return(s)   : None.
*
* Caller(s)   : Application.
*
* Note(s)     : (1) Write index is moved to read index to reset circular buffer.
*********************************************************************************************************
*/

void  SerialBuf_Clr  (SERIAL_BUF  *pbuf)
{
    pbuf->EmptyCnt = pbuf->Len;
    pbuf->IxWr     = pbuf->IxRd;
}


/*$PAGE*/
/*
*********************************************************************************************************
*                                           SerialBuf_Rd()
*
* Description : Read from buffer.
*
* Argument(s) : pbuf        Pointer to buffer.
*
*               pdest       Pointer to destination buffer.
*
*               len         Number of octets to read.
*
* Return(s)   : The number of octets read from buffer.
*
* Caller(s)   : Application.
*
* Note(s)     : None.
*********************************************************************************************************
*/

CPU_SIZE_T  SerialBuf_Rd (SERIAL_BUF  *pbuf,
                          CPU_INT08U  *pdest,
                          CPU_SIZE_T   len)
{
    CPU_SIZE_T   buf_avail;
    CPU_SIZE_T   buf_copy_start;
    CPU_SIZE_T   buf_copy_end;
    CPU_SIZE_T   buf_copy_tot;
    CPU_SIZE_T   ix_rd;
    CPU_SIZE_T   ix_rd_new;
    CPU_SIZE_T   buf_len;
    CPU_INT08U  *pdest_08;
    CPU_SR_ALLOC();


    CPU_CRITICAL_ENTER();
    buf_len   = pbuf->Len;
    buf_avail = buf_len - pbuf->EmptyCnt;                       /* Calc nbr data octets in buf.                         */
    pdest_08  = (CPU_INT08U *)pdest;



    if (buf_avail == 0) {                                       /* ------------------ HANDLE EMPTY BUF ---------------- */
        CPU_CRITICAL_EXIT();
        return ((CPU_SIZE_T)0);
    }



                                                                /* ------------- CALC BUF IX & LEN TO COPY ------------ */
    ix_rd          = pbuf->IxRd;
    buf_copy_tot   = DEF_MIN(buf_avail,    len);                /* Calc nbr data octets tot to copy.                    */
    buf_copy_end   = DEF_MIN(buf_copy_tot, buf_len - ix_rd);    /* Calc nbr data octets to copy from buf end.           */
    buf_copy_start = buf_copy_tot - buf_copy_end;               /* Calc nbr data octets to copy from buf start.         */
    if (buf_copy_start > 0) {                                   /* Update buf ix rd.                                    */
        pbuf->IxRd = buf_copy_start;
    } else {
        ix_rd_new = ix_rd + buf_copy_tot;
        if (ix_rd_new == buf_len) {
            pbuf->IxRd = 0;
        } else {
            pbuf->IxRd = ix_rd_new;
        }
    }
    CPU_CRITICAL_EXIT();

                                                                /* --------------- COPY DATA AT BUF END --------------- */
                                                                /* Copy data.                                           */
    Mem_Copy((void *)pdest_08, (void *)&pbuf->DataPtr[ix_rd], buf_copy_end);
                                                                /* -------------- COPY DATA AT BUF START -------------- */
    pdest_08 += buf_copy_end;                                   /* Adj buf ptr.                                         */
                                                                /* Copy data.                                           */
    Mem_Copy((void *)pdest_08, (void *)&pbuf->DataPtr[0], buf_copy_start);



    CPU_CRITICAL_ENTER();
    pbuf->EmptyCnt += buf_copy_tot;                             /* Update buf empty octets rem.                         */
    CPU_CRITICAL_EXIT();

    return (buf_copy_tot);
}


/*$PAGE*/
/*
*********************************************************************************************************
*                                         SerialBuf_RdOctet()
*
* Description : Read octet from buffer.
*
* Argument(s) : pbuf        Pointer to buffer.
*
*               pdatum      Pointer to variable that will receive octet.
*
* Return(s)   : DEF_YES, if a octet was read from the buffer.
*               DEF_NO,  otherwise.
*
* Caller(s)   : Application.
*
* Note(s)     : None.
*********************************************************************************************************
*/

CPU_BOOLEAN  SerialBuf_RdOctet (SERIAL_BUF  *pbuf,
                                CPU_INT08U  *pdatum)
{
    CPU_SIZE_T  empty_cnt;
    CPU_SIZE_T  ix_rd;
    CPU_SIZE_T  len;
    CPU_SR_ALLOC();


    CPU_CRITICAL_ENTER();
    len       = pbuf->Len;
    empty_cnt = pbuf->EmptyCnt;

    if (empty_cnt == len) {
        CPU_CRITICAL_EXIT();
        return (DEF_NO);
    }

    ix_rd          = pbuf->IxRd;
   *pdatum         = pbuf->DataPtr[ix_rd];
    pbuf->EmptyCnt = empty_cnt + 1;

    if (ix_rd + 1 == len) {
        pbuf->IxRd = 0;
    } else {
        pbuf->IxRd = ix_rd + 1;
    }
    CPU_CRITICAL_EXIT();

    return (DEF_YES);
}


/*$PAGE*/
/*
*********************************************************************************************************
*                                         SerialBuf_WrOctet()
*
* Description : Write octet to buffer.
*
* Argument(s) : pbuf        Pointer to buffer.
*
*               datum       Octet to write.
*
* Return(s)   : DEF_YES, if the octet was stored in buffer.
*               DEF_NO,  otherwise.
*
* Caller(s)   : Application.
*
* Note(s)     : None.
*********************************************************************************************************
*/

CPU_BOOLEAN  SerialBuf_WrOctet (SERIAL_BUF  *pbuf,
                                CPU_INT08U   datum)
{
    CPU_SIZE_T  empty_cnt;
    CPU_SIZE_T  ix_wr;
    CPU_SIZE_T  len;
    CPU_SR_ALLOC();


    CPU_CRITICAL_ENTER();
    empty_cnt = pbuf->EmptyCnt;

    if (empty_cnt == 0) {
        CPU_CRITICAL_EXIT();
        return (DEF_NO);
    }

    ix_wr                = pbuf->IxWr;
    pbuf->DataPtr[ix_wr] = datum;
    pbuf->EmptyCnt       = empty_cnt - 1;
    len                  = pbuf->Len;

    if (ix_wr + 1 == len) {
        pbuf->IxWr = 0;
    } else {
        pbuf->IxWr = ix_wr + 1;
    }
    CPU_CRITICAL_EXIT();

    return (DEF_YES);
}


/*$PAGE*/
/*
*********************************************************************************************************
*                                         SerialBuf_Cmp()
*
* Description : Compare incoming octets to buffer.
*
* Argument(s) : pbuf        Pointer to buffer.
*
*               datum       Octet to compare.
*
* Return(s)   : DEF_YES, if compare finished successfully.
*               DEF_NO,  otherwise.
*
* Caller(s)   : Application.
*
* Note(s)     : None.
*********************************************************************************************************
*/

CPU_BOOLEAN  SerialBuf_Cmp (SERIAL_BUF  *pbuf,
                            CPU_INT08U   datum)
{
    CPU_SIZE_T   ix_rd;
    CPU_BOOLEAN  full;
    CPU_SR_ALLOC();


    CPU_CRITICAL_ENTER();
    full = SerialBuf_IsFull(pbuf);
    if (full == DEF_NO) {
        CPU_CRITICAL_EXIT();
        return (DEF_NO);
    }

    ix_rd = pbuf->IxRd;
    if(pbuf->DataPtr[ix_rd] != datum) {
        pbuf->IxRd = 0;                                         /* Reset comparison.                                    */
        CPU_CRITICAL_EXIT();
        return (DEF_NO);
    }

    ix_rd++;

    if (ix_rd == pbuf->Len) {
        pbuf->IxRd = 0;
        CPU_CRITICAL_EXIT();
        return (DEF_YES);
    }

    pbuf->IxRd = ix_rd;
    CPU_CRITICAL_EXIT();
    return (DEF_NO);
}


/*$PAGE*/
/*
*********************************************************************************************************
*                                         SerialBuf_IsEmpty()
*
* Description : Determine whether buffer is empty.
*
* Argument(s) : pbuf        Pointer to buffer.
*
* Return(s)   : DEF_YES, if buffer is empty.
*               DEF_NO,  otherwise.
*
* Caller(s)   : Application.
*
* Note(s)     : (1) Interrupts are assumed to be disabled when this function is called.
*********************************************************************************************************
*/

CPU_BOOLEAN  SerialBuf_IsEmpty (SERIAL_BUF  *pbuf)
{
    CPU_BOOLEAN  empty;


    empty = ((pbuf->EmptyCnt == pbuf->Len) ? (DEF_YES) : (DEF_NO));
    return (empty);
}


/*$PAGE*/
/*
*********************************************************************************************************
*                                         SerialBuf_IsFull()
*
* Description : Determine whether buffer is full.
*
* Argument(s) : pbuf        Pointer to buffer.
*
* Return(s)   : DEF_YES, if buffer is full.
*               DEF_NO,  otherwise.
*
* Caller(s)   : Application.
*
* Note(s)     : (1) Interrupts are assumed to be disabled when this function is called.
*********************************************************************************************************
*/

CPU_BOOLEAN  SerialBuf_IsFull (SERIAL_BUF  *pbuf)
{
    CPU_BOOLEAN  full;


    full = ((pbuf->EmptyCnt == 0) ? (DEF_YES) : (DEF_NO));
    return (full);
}


/*$PAGE*/
/*
*********************************************************************************************************
*                                          SerialBuf_Size()
*
* Description : Retrieve buffer size.
*
* Argument(s) : pbuf        Pointer to buffer.
*
* Return(s)   : Size of buffer in octets.
*
* Caller(s)   : Application.
*
* Note(s)     : (1) Interrupts are assumed to be disabled when this function is called.
*********************************************************************************************************
*/

CPU_SIZE_T  SerialBuf_Size (SERIAL_BUF  *pbuf)
{
    CPU_SIZE_T  size;


    size = pbuf->Len;
    return (size);
}


/*$PAGE*/
/*
*********************************************************************************************************
*                                         SerialBuf_DataLen()
*
* Description : Determine buffer data area length.
*
* Argument(s) : pbuf        Pointer to buffer.
*
* Return(s)   : Number of octets in buffer data area.
*
* Caller(s)   : Application.
*
* Note(s)     : (1) Interrupts are assumed to be disabled when this function is called.
*********************************************************************************************************
*/

CPU_SIZE_T  SerialBuf_DataLen (SERIAL_BUF  *pbuf)
{
    CPU_SIZE_T  len;


    len = pbuf->Len - pbuf->EmptyCnt;
    return (len);
}
