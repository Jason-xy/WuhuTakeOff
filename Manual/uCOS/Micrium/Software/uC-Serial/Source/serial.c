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
* Filename      : serial.c
* Version       : V2.00.01
* Programmer(s) : FGK
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#define    SERIAL_MODULE
#include  <serial.h>
#include  <serial_os.h>


/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/

#define  SERIAL_RD_STATE_CLOSED                            0u
#define  SERIAL_RD_STATE_OPENED                            1u
#define  SERIAL_RD_STATE_RD                                2u
#define  SERIAL_RD_STATE_CHK                               3u

#define  SERIAL_WR_STATE_CLOSED                            0u
#define  SERIAL_WR_STATE_OPENED                            1u
#define  SERIAL_WR_STATE_WR                                2u
#define  SERIAL_WR_STATE_WR_BUF                            3u


/*
*********************************************************************************************************
*                                                FLAGS
*********************************************************************************************************
*/

#define  SERIAL_STATE_NONE                                 0u
#define  SERIAL_STATE_CLOSED                               1u
#define  SERIAL_STATE_CLOSING                              2u
#define  SERIAL_STATE_OPENED                               3u


/*
*********************************************************************************************************
*                                        FORWARD DECLARATIONS
*********************************************************************************************************
*/

typedef  struct  serial_buf_desc  SERIAL_BUF_DESC;


/*
*********************************************************************************************************
*                                      SERIAL CALLBACK INFO TYPE
*********************************************************************************************************
*/

typedef  struct  serial_callback_info {                         /* ---------------- RD/WR CALLBACK INFO --------------- */
    void        *SignalPtr;                                     /* Ptr to signal to wait.                               */
    CPU_INT32U   Timeout;                                       /* Timeout in milliseconds. Wait indefinitely, if 0.    */
    CPU_SIZE_T   Len;                                           /* Nbr of octets successfully read/written.             */
    SERIAL_ERR   Err;                                           /* Rtn err code.                                        */
} SERIAL_CALLBACK_INFO;


/*
*********************************************************************************************************
*                                    SERIAL BUFFER DESCRIPTOR TYPE
*********************************************************************************************************
*/

struct  serial_buf_desc {
    CPU_SIZE_T             Len;                                 /* Buf len.                                             */
    CPU_INT08U            *DataPtr;                             /* Ptr to buf data.                                     */

    SERIAL_CALLBACK_FNCT  *Callback;                            /* Rd/Wr complete callback.                             */
    void                  *CallbackArg;                         /* Rd/Wr complete context.                              */
    void                  *CallbackBuf;                         /* Rd/Wr complete buffer pointer.                       */

    SERIAL_BUF_DESC       *NextPtr;
    SERIAL_BUF_DESC       *PrevPtr;
};


/*
*********************************************************************************************************
*                                        SERIAL INTERFACE TYPE
*
* Note(s) : (1) Serial interface initialization flag set when an interface has been successfully added
*               & initialized to the interface table.  Once set, this flag is never cleared since the
*               removal of interfaces is currently not allowed.
*********************************************************************************************************
*/

typedef  struct  serial_if {
    CPU_BOOLEAN            Init;                                /* IF init status (see Note #1).                        */

    SERIAL_IF_NBR          Nbr;                                 /* IF nbr.                                              */
    CPU_INT08U             State;                               /* IF state.                                            */

    CPU_CHAR              *NamePtr;                             /* Ptr to dev name (cfg'd @ dev add).                   */

    SERIAL_DEV             Dev;                                 /* Dev             (cfg'd @ dev add).                   */

#if (SERIAL_CFG_TX_DESC_NBR > 0)
    MEM_POOL               TxBufPool;
    SERIAL_BUF_DESC       *TxBufListStart;
    SERIAL_BUF_DESC       *TxBufListEnd;
#endif

                                                                /* --------------- RD COMMUNICATION DATA -------------- */
    CPU_INT08U             RdState;                             /* Rd state.                                            */
    void                  *RdSem;                               /* Rd semaphore.                                        */
    SERIAL_CALLBACK_FNCT  *RdCallback;                          /* Rd complete callback.                                */
    void                  *RdCallbackArg;                       /* Rd complete context.                                 */
    void                  *RdCallbackBuf;                       /* Rd complete buffer pointer.                          */
#if (SERIAL_CFG_RD_BUF_EN == DEF_ENABLED)
    CPU_SIZE_T             RdCallbackLen;                       /* Rd complete buffer len.                              */
#endif
    SERIAL_BUF             RdUserBuf;                           /* Rd user buf.                                         */



                                                                /* --------------- WR COMMUNICATION DATA -------------- */
    CPU_INT08U             WrState;                             /* Wr state.                                            */
    void                  *WrSem;                               /* Wr semaphore.                                        */
    SERIAL_CALLBACK_FNCT  *WrCallback;                          /* Wr complete callback.                                */
    void                  *WrCallbackArg;                       /* Wr complete context.                                 */
    void                  *WrCallbackBuf;                       /* Wr complete buffer pointer.                          */
    SERIAL_BUF             WrUserBuf;                           /* Wr user buf.                                         */



#if (SERIAL_CFG_RD_BUF_EN == DEF_ENABLED)                       /* -------------------- RD BUFFER --------------------- */
    CPU_BOOLEAN            RdBufEn;                             /* Indicates buf presence.                              */
    SERIAL_ERR             RdBufErr;                            /* Rd err (while rd'ing into rd buf).                   */
    SERIAL_BUF             RdBuf;                               /* Buf.                                                 */
#endif

#if (SERIAL_CFG_WR_BUF_EN == DEF_ENABLED)                       /* -------------------- WR BUFFER --------------------- */
    CPU_BOOLEAN            WrBufEn;                             /* Indicates buf presence.                              */
    SERIAL_BUF             WrBuf;                               /* Buf.                                                 */
#endif
} SERIAL_IF;


/*
*********************************************************************************************************
*                                            LOCAL TABLES
*********************************************************************************************************
*/

static  SERIAL_IF  Serial_IF_Tbl[SERIAL_IF_NBR_TOT];

/*
*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/

static  SERIAL_IF_NBR  SerialIF_NbrNext;


/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/
                                                                /* Clr if.                                              */
static  void              Serial_IF_Clr      (SERIAL_IF             *pif);
                                                                /* Clr rd callback arg and user buf.                    */
static  void              Serial_IF_RdClr    (SERIAL_IF             *pif);
                                                                /* Clr wr callback arg and user buf.                    */
static  void              Serial_IF_WrClr    (SERIAL_IF             *pif);
                                                                /* Reset rx'er.                                         */
static  void              Serial_IF_RxReset  (SERIAL_IF             *pif);
                                                                /* Start rx'er.                                         */
static  void              Serial_IF_RxStart  (SERIAL_IF             *pif);
                                                                /* Stop  rx'er.                                         */
static  void              Serial_IF_RxStop   (SERIAL_IF             *pif);

                                                                /* Validate serial interface number.                    */
static  CPU_BOOLEAN       Serial_IF_IsValid  (SERIAL_IF_NBR          if_nbr,
                                              SERIAL_ERR            *perr);


                                                                /* Internal rd callback.                                */
static  void              Serial_RdCallback  (SERIAL_IF_NBR          if_nbr,
                                              void                  *parg,
                                              void                  *pbuf,
                                              CPU_SIZE_T             len,
                                              SERIAL_ERR             err);

                                                                /* Rd async & sync handler.                             */
static  void              Serial_RdHandler   (SERIAL_IF             *pif,
                                              void                  *pdest,
                                              CPU_SIZE_T             len,
                                              SERIAL_CALLBACK_FNCT  *phandler,
                                              void                  *parg,
                                              CPU_BOOLEAN            async,
                                              CPU_INT32U             timeout_ms,
                                              SERIAL_ERR            *perr);

                                                                /* Start rd.                                            */
static  void              Serial_RdStart     (SERIAL_IF             *pif,
                                              void                  *pdest,
                                              CPU_SIZE_T             len);

                                                                /* Wait for rd end.                                     */
static  CPU_SIZE_T        Serial_RdEnd       (SERIAL_IF             *pif,
                                              SERIAL_CALLBACK_INFO  *pinfo,
                                              SERIAL_ERR            *perr);


                                                                /* Internal wr callback.                                */
static  void              Serial_WrCallback  (SERIAL_IF_NBR          if_nbr,
                                              void                  *parg,
                                              void                  *pbuf,
                                              CPU_SIZE_T             len,
                                              SERIAL_ERR             err);

                                                                /* Insert wr buf into tx list.                          */
#if (SERIAL_CFG_TX_DESC_NBR > 0)
static  void              Serial_WrBufInsert (SERIAL_IF             *pif,
                                              SERIAL_BUF_DESC       *pbuf,
                                              SERIAL_ERR            *perr);
#endif

                                                                /* Remove wr buf from tx list.                          */
static  CPU_BOOLEAN       Serial_WrBufRemove (SERIAL_IF             *pif,
                                              SERIAL_BUF_DESC       *pbuf);

                                                                /* Init wr buf and start tx'er.                         */
static  void              Serial_WrBufTxStart(SERIAL_IF             *pif,
                                              SERIAL_BUF_DESC       *pbuf,
                                              SERIAL_ERR            *perr);

                                                                /* Start wr.                                            */
static  void              Serial_WrStart     (SERIAL_IF             *pif,
                                              SERIAL_BUF_DESC       *pbuf,
                                              SERIAL_ERR            *perr);

                                                                /* Tx next wr buf from tx list.                         */
static  void              Serial_WrNext      (SERIAL_IF             *pif);

                                                                /* Wait for wr end.                                     */
static  CPU_SIZE_T        Serial_WrEnd       (SERIAL_IF             *pif,
                                              SERIAL_CALLBACK_INFO  *pinfo,
                                              SERIAL_ERR            *perr);


/*$PAGE*/
/*
*********************************************************************************************************
*********************************************************************************************************
*                                     SERIAL INTERFACE FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                            Serial_Init()
*
* Description : Initialize the serial communication module:
*
* Argument(s) : None.
*
* Return(s)   : SERIAL_ERR_NONE,                    if NO errors.
*
*               Specific initialization error code, otherwise.
*
* Caller(s)   : Application.
*
* Note(s)     : (1) SerialInit() MUST be called ... :
*
*                   (a) ONLY ONCE from a product's application; ...
*                   (b) (1) AFTER  product's OS has been initialized
*                       (2) BEFORE product's application calls any network protocol suite function(s)
*********************************************************************************************************
*/

SERIAL_ERR  Serial_Init (void)
{
    SERIAL_IF_NBR   ix;
    SERIAL_IF      *pif;
    SERIAL_ERR      ser_err;
#if (SERIAL_CFG_TX_DESC_NBR > 0)
    LIB_ERR         lib_err;
#endif


                                                                /* -------------- PERFORM SERIAL/OS INIT -------------- */
    SerialOS_Init(&ser_err);
    if (ser_err != SERIAL_OS_ERR_NONE) {
        return (ser_err);
    }

                                                                /* ----------------- INIT IF DATA TBL ----------------- */
                                                                /* Clr all IF data.                                     */
    for (ix = SERIAL_IF_NBR_MIN; ix < SERIAL_IF_NBR_TOT; ix++) {
        pif = &Serial_IF_Tbl[ix];

        Serial_IF_Clr(pif);

        pif->Nbr     = (SERIAL_IF_NBR  )ix;
        pif->Init    = (CPU_BOOLEAN    )DEF_NO;
        pif->State   = (CPU_INT08U     )SERIAL_STATE_NONE;
        pif->NamePtr = (CPU_CHAR      *)0;

        pif->RdSem   = (void          *)0;
        pif->WrSem   = (void          *)0;


#if (SERIAL_CFG_TX_DESC_NBR > 0)
        Mem_PoolClr(&pif->TxBufPool, &lib_err);

        pif->TxBufListStart = (SERIAL_BUF_DESC *)0;
        pif->TxBufListEnd   = (SERIAL_BUF_DESC *)0;
#endif
    }

    SerialIF_NbrNext = SERIAL_IF_NBR_MIN;

    return (SERIAL_ERR_NONE);
}


/*$PAGE*/
/*
*********************************************************************************************************
*                                         Serial_DevDrvAdd()
*
* Description : Add a serial device driver.
*
* Argument(s) : pname       Pointer to name that will be used to open the device.
*
*               pdev_cfg    Pointer to device configuration structure.
*
*               rd_buf_len  Length of read buffer.
*
*               wr_buf_len  Length of write buffer.
*
*               perr        Pointer to variable that will receive the return error code from this function :
*
*                               SERIAL_ERR_NONE                 Serial device driver successfully added.
*                               SERIAL_ERR_NULL_PTR             NULL pointer passed for argument 'pname',
*                                                                   'pdev_cfg', 'pdev_cfg->Drv_API' or
*                                                                   'pdev_cfg->Dev_Cfg'.
*                               SERIAL_ERR_DRV_INVALID          NULL/invalid members in 'pdev_cfg->Drv_API'.
*                               SERIAL_ERR_DEV_DUP_NAME         'pname' duplicates name of existing device.
*                               SERIAL_ERR_IF_NOT_AVAIL         Resource for new interface NOT available.
*                               SERIAL_ERR_MEM_ALLOC            Memory could NOT be allocated for new interface
*                                                                   entry or read/write buffers.
*                               SERIAL_ERR_BUF_POOL_INIT        Buffer pool could NOT be initialized.
*
* Return(s)   : None.
*
* Caller(s)   : Application.
*
* Note(s)     : None.
*********************************************************************************************************
*/

void  Serial_DevDrvAdd (CPU_CHAR        *pname,
                        SERIAL_DEV_CFG  *pdev_cfg,
                        CPU_SIZE_T       rd_buf_len,
                        CPU_SIZE_T       wr_buf_len,
                        SERIAL_ERR      *perr)
{
    SERIAL_IF      *pif;
    SERIAL_DEV     *pdev;
    SERIAL_IF_NBR   ix;
    CPU_INT16S      cmp;
    CPU_BOOLEAN     init;
#if (SERIAL_CFG_RD_BUF_EN == DEF_ENABLED) || \
    (SERIAL_CFG_WR_BUF_EN == DEF_ENABLED)
    CPU_INT08U     *pbuf_data;
#endif
#if (SERIAL_CFG_RD_BUF_EN       == DEF_ENABLED) || \
    (SERIAL_CFG_WR_BUF_EN       == DEF_ENABLED) || \
    (SERIAL_CFG_TX_DESC_NBR >  0)
    CPU_SIZE_T      octets_reqd;
    LIB_ERR         lib_err;
#endif
    CPU_SR_ALLOC();


#if (SERIAL_CFG_ARG_CHK_EXT_EN == DEF_ENABLED)                  /* ------------------- VALIDATE ARGS ------------------ */
    if (pname == (CPU_CHAR *)0) {
       *perr = SERIAL_ERR_NULL_PTR;
        return;
    }

    if (pdev_cfg == (SERIAL_DEV_CFG *)0) {
       *perr = SERIAL_ERR_NULL_PTR;
        return;
    }

    if (pdev_cfg->Drv_API == (SERIAL_DRV_API *)0) {
       *perr = SERIAL_ERR_NULL_PTR;
        return;
    }

    if ((pdev_cfg->Drv_API->Init        == (void *)0) ||        /* Validate drv API.                                    */
        (pdev_cfg->Drv_API->Open        == (void *)0) ||
        (pdev_cfg->Drv_API->Close       == (void *)0) ||
        (pdev_cfg->Drv_API->RxStart     == (void *)0) ||
        (pdev_cfg->Drv_API->RxStop      == (void *)0) ||
        (pdev_cfg->Drv_API->RxOctet     == (void *)0) ||
        (pdev_cfg->Drv_API->TxStart     == (void *)0) ||
        (pdev_cfg->Drv_API->TxStop      == (void *)0) ||
        (pdev_cfg->Drv_API->TxOctet     == (void *)0) ||
        (pdev_cfg->Drv_API->ISR_Handler == (void *)0)) {
        *perr = SERIAL_ERR_DRV_INVALID;
         return;
    }
#endif


    CPU_CRITICAL_ENTER();
                                                                /* ---------------------- INIT DEV -------------------- */
    if (SerialIF_NbrNext >= SERIAL_IF_NBR_TOT) {                /* Chk next IF nbr.                                     */
        CPU_CRITICAL_EXIT();
       *perr = SERIAL_ERR_IF_NOT_AVAIL;
        return;
    }

    for (ix = SERIAL_IF_NBR_MIN; ix < SerialIF_NbrNext; ix++) { /* Chk if dev name dup'd.                               */
        pif = &Serial_IF_Tbl[ix];
        cmp =  Str_Cmp(pif->NamePtr, pname);
        if (cmp == 0) {
            CPU_CRITICAL_EXIT();
           *perr = SERIAL_ERR_DEV_DUP_NAME;
            return;
        }
    }

                                                                /* --------------------- SEARCH DRV ------------------- */
    init = DEF_TRUE;
    for (ix = SERIAL_IF_NBR_MIN; ix < SerialIF_NbrNext; ix++) {
        pif = &Serial_IF_Tbl[ix];

        if ((pif->Init                 == DEF_YES) &&
            (pif->Dev.Dev_Cfg->Drv_API == pdev_cfg->Drv_API)) {
            init = DEF_FALSE;
            break;
        }
    }

                                                                /* ---------------------- INIT DRV -------------------- */
    if (init == DEF_TRUE) {
        pdev_cfg->Drv_API->Init(perr);
        if (*perr != SERIAL_ERR_NONE) {
             CPU_CRITICAL_EXIT();
             return;
        }
    }

    pif = &Serial_IF_Tbl[SerialIF_NbrNext];


#if (SERIAL_CFG_TX_DESC_NBR > 0)
    Mem_PoolCreate((MEM_POOL   *)&pif->TxBufPool,
                   (void       *) 0,
                   (CPU_SIZE_T  ) 0,
                   (CPU_SIZE_T  ) SERIAL_CFG_TX_DESC_NBR,
                   (CPU_SIZE_T  ) sizeof(SERIAL_BUF_DESC),
                   (CPU_SIZE_T  ) sizeof(CPU_DATA),
                   (CPU_SIZE_T *)&octets_reqd,
                   (LIB_ERR    *)&lib_err);

    if (lib_err != LIB_MEM_ERR_NONE) {
        CPU_CRITICAL_EXIT();
       *perr = SERIAL_ERR_BUF_POOL_INIT;
        return;
    }
#endif

    pif->WrSem = SerialOS_SemCreate(1 + SERIAL_CFG_TX_DESC_NBR);
    if (pif->WrSem == (void *)0) {
        CPU_CRITICAL_EXIT();
       *perr = SERIAL_ERR_SIGNAL;
        return;
    }

    pif->RdSem = SerialOS_SemCreate(1);
    if (pif->RdSem == (void *)0) {
        CPU_CRITICAL_EXIT();
       *perr = SERIAL_ERR_SIGNAL;
        return;
    }

    pif->NamePtr = pname;
    SerialIF_NbrNext++;                                         /* Update IF cnt.                                       */

    CPU_CRITICAL_EXIT();

                                                                /* ------------------- LINK DEV DESC ------------------ */
    pdev               = &pif->Dev;
    pdev->IF_Nbr       =  pif->Nbr;
    pdev->Dev_Cfg      =  pdev_cfg;
    pdev->Drv_Data     = (void                *)0;
    pdev->LineDrv_API  = (SERIAL_LINE_DRV_API *)0;
    pdev->LineDrv_Data = (void                *)0;

   *perr = SERIAL_ERR_NONE;

   (void)&rd_buf_len;
#if (SERIAL_CFG_RD_BUF_EN == DEF_ENABLED)                       /* -------------------- INIT RD BUF ------------------- */
    pif->RdBufEn = DEF_NO;

    if (rd_buf_len > 0) {                                       /* Alloc rd buf.                                        */
        pbuf_data = (CPU_INT08U *)Mem_HeapAlloc((CPU_SIZE_T  ) rd_buf_len,
                                                (CPU_SIZE_T  ) sizeof(CPU_ALIGN),
                                                (CPU_SIZE_T *)&octets_reqd,
                                                (LIB_ERR    *)&lib_err);
        if (pbuf_data == (CPU_INT08U *)0) {
           *perr = SERIAL_ERR_MEM_ALLOC;                        /* Indicate rd buf alloc failure, but continue add IF.  */
        } else {
            pif->RdBufEn  = DEF_YES;
            SerialBuf_Init((SERIAL_BUF  *)&(pif->RdBuf),
                           (CPU_INT08U  *)  pbuf_data,
                           (CPU_SIZE_T   )  rd_buf_len,
                           (CPU_BOOLEAN  )  DEF_NO);
        }
    }
#endif


   (void)&wr_buf_len;
#if (SERIAL_CFG_WR_BUF_EN == DEF_ENABLED)                       /* -------------------- INIT WR BUF ------------------- */
    pif->WrBufEn = DEF_NO;

    if (wr_buf_len > 0) {                                       /* Alloc wr buf.                                        */
        pbuf_data = (CPU_INT08U *)Mem_HeapAlloc((CPU_SIZE_T  ) wr_buf_len,
                                                (CPU_SIZE_T  ) sizeof(CPU_ALIGN),
                                                (CPU_SIZE_T *)&octets_reqd,
                                                (LIB_ERR    *)&lib_err);
        if (pbuf_data == (CPU_INT08U *)0) {
           *perr = SERIAL_ERR_MEM_ALLOC;                        /* Indicate wr buf alloc failure, but continue add IF.  */
        } else {
            pif->WrBufEn  = DEF_YES;
            SerialBuf_Init((SERIAL_BUF  *)&(pif->WrBuf),
                           (CPU_INT08U  *)  pbuf_data,
                           (CPU_SIZE_T   )  wr_buf_len,
                           (CPU_BOOLEAN  )  DEF_NO);
        }
    }
#endif


    CPU_CRITICAL_ENTER();
    pif->Init  = DEF_YES;
    pif->State = SERIAL_STATE_CLOSED;
    CPU_CRITICAL_EXIT();
}


/*$PAGE*/
/*
*********************************************************************************************************
*                                         Serial_IF_IsValid()
*
* Description : Validate serial interface number.
*
* Argument(s) : if_nbr      Serial interface number to validate.
*
*               perr        Pointer to variable that will receive the return error code from this function :
*
*                               SERIAL_ERR_NONE                 Serial interface successfully validated.
*                               SERIAL_ERR_IF_INVALID_NBR       Invalid serial interface number.
*                               SERIAL_ERR_IF_NOT_OPEN          Serial interface NOT open.
*
* Return(s)   : DEF_YES, serial interface number is valid.
*
*               DEF_NO,  otherwise.
*
* Caller(s)   : various.
*
* Note(s)     : None.
*********************************************************************************************************
*/

CPU_BOOLEAN  Serial_IF_IsValid (SERIAL_IF_NBR   if_nbr,
                                SERIAL_ERR     *perr)
{
    SERIAL_IF  *pif;


#if (SERIAL_CFG_ARG_CHK_EXT_EN == DEF_ENABLED)                  /* -------------- VALIDATE SERIAL IF NBR -------------- */
    if (if_nbr >= SERIAL_IF_NBR_TOT) {
       *perr = SERIAL_ERR_IF_INVALID_NBR;
        return (DEF_NO);
    }
#endif

                                                                /* ---------------- VALIDATE SERIAL IF ---------------- */
    if (if_nbr >= SerialIF_NbrNext) {
       *perr = SERIAL_ERR_IF_INVALID_NBR;
        return (DEF_NO);
    }

    pif = &Serial_IF_Tbl[if_nbr];
    if (pif->Init != DEF_YES) {
       *perr = SERIAL_ERR_IF_INVALID_NBR;
        return (DEF_NO);
    }

    if (pif->State != SERIAL_STATE_OPENED) {
       *perr = SERIAL_ERR_IF_NOT_OPEN;
        return (DEF_NO);
    }


   *perr = SERIAL_ERR_NONE;
    return (DEF_YES);
}


/*$PAGE*/
/*
*********************************************************************************************************
*                                         Serial_SetLineDrv()
*
* Description : Set serial interface line driver.
*
* Argument(s) : if_nbr      Serial interface number to validate.
*
*               pline_api   Pointer to line driver API,
*                               OR
*                           Pointer to NULL, if the default line driver will be used.
*
*               perr        Pointer to variable that will receive the return error code from this function :
*
*                               SERIAL_ERR_NONE                 Serial interface successfully validated.
*
*                                                               --- RETURNED BY Serial_IF_IsValid() : ---
*                               SERIAL_ERR_IF_INVALID_NBR       Invalid serial interface number.
*                               SERIAL_ERR_IF_NOT_OPEN          Serial interface NOT open.
*
* Return(s)   : None.
*
* Caller(s)   : Application.
*
* Note(s)     : None.
*********************************************************************************************************
*/

void  Serial_SetLineDrv (SERIAL_IF_NBR         if_nbr,
                         SERIAL_LINE_DRV_API  *pline_api,
                         SERIAL_ERR           *perr)
{
    SERIAL_IF    *pif;
    SERIAL_DEV   *pdev;
    CPU_BOOLEAN   valid;
    void         *pline_data;
    SERIAL_ERR    ser_err;
    CPU_SR_ALLOC();


    if (pline_api == (SERIAL_LINE_DRV_API *)0) {                /* Dflt line drv.                                       */
        pline_api  = (SERIAL_LINE_DRV_API *)&SerialLine_Dflt;
    }

                                                                /* ------------------ VALIDATE IF NBR ----------------- */
    CPU_CRITICAL_ENTER();
    valid = Serial_IF_IsValid(if_nbr, perr);
    if (valid != DEF_YES) {                                     /* Rtn err if IF not valid.                             */
        CPU_CRITICAL_EXIT();
        return;
    }

                                                                /* ----------------- GET LINE DRV INFO ---------------- */
    pif  = &Serial_IF_Tbl[if_nbr];
    pdev = &pif->Dev;

    pline_data = pline_api->Open(pdev, perr);                   /* 'Open' new line drv.                                 */
    if (*perr != SERIAL_ERR_NONE) {
        CPU_CRITICAL_EXIT();
        return;
    }

    pdev->LineDrv_API->Close(pdev, pdev->LineDrv_Data, perr);   /* 'Close' cur line drv.                                */
    if (*perr != SERIAL_ERR_NONE) {
        pline_api->Close(pdev, pline_data, &ser_err);           /* 'Close' new line drv.                                */
        CPU_CRITICAL_EXIT();
        return;
    }

#if (SERIAL_CFG_RD_BUF_EN == DEF_ENABLED)                       /* Flush rd buf.                                        */
    if (pif->RdBufEn == DEF_YES) {
        SerialBuf_Clr(&(pif->RdBuf));
    }
#endif

    pdev->LineDrv_API  = pline_api;
    pdev->LineDrv_Data = pline_data;
    CPU_CRITICAL_EXIT();

   *perr = SERIAL_ERR_NONE;
}


/*$PAGE*/
/*
*********************************************************************************************************
*                                            Serial_Open()
*
* Description : Open a serial interface for communication.
*
* Argument(s) : pname       Pointer to device name.
*
*               pcfg        Pointer to serial port setup data.
*
*               perr        Pointer to variable that will receive the return error code from this function :
*
*                               SERIAL_ERR_NONE                 Device successfully opened.
*                               SERIAL_ERR_NULL_PTR             NULL pointer passed for 'pname' or 'pcfg'.
*                               SERIAL_ERR_DEV_NOT_FOUND        Device not found.
*                               SERIAL_ERR_DEV_ALREADY_OPEN     Device is already open.
*                               SERIAL_ERR_DRV_OPEN             Driver could NOT be open or device
*                                                                   initialization NOT successful.
*
* Return(s)   : Interface number.
*
* Caller(s)   : Application.
*
* Note(s)     : (1) The serial interface is opened with the default line driver.
*********************************************************************************************************
*/

SERIAL_IF_NBR  Serial_Open (CPU_CHAR       *pname,
                            SERIAL_IF_CFG  *pcfg,
                            SERIAL_ERR     *perr)
{
    CPU_INT16S            cmp;
    SERIAL_IF_NBR         ix;
    CPU_BOOLEAN           found;
    SERIAL_IF            *pif;
    SERIAL_DEV           *pdev;
    SERIAL_LINE_DRV_API  *pline_api;
    void                 *pline_data;
    CPU_SR_ALLOC();


#if (SERIAL_CFG_ARG_CHK_EXT_EN == DEF_ENABLED)                  /* ------------------- VALIDATE ARGS ------------------ */
    if (pname == (CPU_CHAR *)0) {
       *perr = SERIAL_ERR_NULL_PTR;
        return (SERIAL_IF_NBR_NONE);
    }

    if (pcfg == (SERIAL_IF_CFG *)0) {
       *perr = SERIAL_ERR_NULL_PTR;
        return (SERIAL_IF_NBR_NONE);
    }
#endif

                                                                /* --------------------- FIND DEV --------------------- */
    pif   = (SERIAL_IF *)0;
    found = DEF_NO;
    for (ix = SERIAL_IF_NBR_MIN; ix < SerialIF_NbrNext; ix++) {
        pif = &Serial_IF_Tbl[ix];
        cmp =  Str_Cmp(pif->NamePtr, pname);
        if (cmp == 0) {
            found = DEF_YES;
            break;
        }
    }

    if (found == DEF_NO) {                                      /* IF not found.                                        */
       *perr = SERIAL_ERR_DEV_NOT_FOUND;
        return (SERIAL_IF_NBR_NONE);
    }


    pdev = &pif->Dev;

                                                                /* --------------------- OPEN DEV --------------------- */
    CPU_CRITICAL_ENTER();
    if (pif->State != SERIAL_STATE_CLOSED) {                    /* Chk if port already open.                            */
        CPU_CRITICAL_EXIT();
       *perr = SERIAL_ERR_DEV_ALREADY_OPEN;
        return (SERIAL_IF_NBR_NONE);
    }

    pline_api  = &SerialLine_Dflt;                              /* Dflt   line drv.                                     */
    pline_data =  pline_api->Open(pdev, perr);                  /* 'Open' line drv.                                     */
    if (*perr != SERIAL_ERR_NONE) {
        CPU_CRITICAL_EXIT();
        return (SERIAL_IF_NBR_NONE);
    }

    pdev->Dev_Cfg->Drv_API->Open(pdev, pcfg, perr);             /* Open dev with drv.                                   */
    if (*perr != SERIAL_ERR_NONE) {
        pline_api->Close(pdev, pline_data, perr);               /* 'Close' line drv.                                    */
        CPU_CRITICAL_EXIT();
       *perr = SERIAL_ERR_DRV_OPEN;
        return (SERIAL_IF_NBR_NONE);
    }

    pdev->LineDrv_API  = pline_api;
    pdev->LineDrv_Data = pline_data;

#if (SERIAL_CFG_RD_BUF_EN == DEF_ENABLED)
    if (pif->RdBufEn == DEF_YES) {                              /* Start rx'er (if rd buf en'd).                        */
        pif->RdBufErr = SERIAL_ERR_NONE;
        pdev->Dev_Cfg->Drv_API->RxStart(pdev, perr);

        if (*perr != SERIAL_ERR_NONE) {
            CPU_CRITICAL_EXIT();
            return (SERIAL_IF_NBR_NONE);
        }
    }
#endif

    pif->RdState = SERIAL_RD_STATE_OPENED;
    pif->WrState = SERIAL_WR_STATE_OPENED;
    pif->State   = SERIAL_STATE_OPENED;
    CPU_CRITICAL_EXIT();

    return (pif->Nbr);
}


/*$PAGE*/
/*
*********************************************************************************************************
*                                           Serial_Close()
*
* Description : Close a serial interface.
*
* Argument(s) : if_nbr      Interface number.
*
*               perr        Pointer to variable that will receive the return error code from this function :
*
*                               SERIAL_ERR_NONE                 Device successfully closed.
*
*                                                               - RETURNED BY Serial_IF_IsValid() : -
*                               SERIAL_ERR_IF_INVALID_NBR       Invalid serial interface number.
*                               SERIAL_ERR_IF_NOT_OPEN          Serial interface NOT open.
*
* Return(s)   : None.
*
* Caller(s)   : Application.
*
* Note(s)     : None.
*********************************************************************************************************
*/

void  Serial_Close (SERIAL_IF_NBR   if_nbr,
                    SERIAL_ERR     *perr)
{
    SERIAL_IF             *pif;
    SERIAL_DEV            *pdev;
    CPU_BOOLEAN            valid;
    SERIAL_CALLBACK_FNCT  *rd_callback;
    void                  *rd_callback_arg;
    void                  *rd_callback_buf;
    SERIAL_CALLBACK_FNCT  *wr_callback;
    void                  *wr_callback_arg;
    void                  *wr_callback_buf;
#if (SERIAL_CFG_TX_DESC_NBR > 0)
    SERIAL_BUF_DESC       *pbuf;
    SERIAL_BUF_DESC       *pbuf_next;
    LIB_ERR                lib_err;
#endif
    CPU_SR_ALLOC();

                                                                /* ------------------ VALIDATE IF NBR ----------------- */
    CPU_CRITICAL_ENTER();
    valid = Serial_IF_IsValid(if_nbr, perr);
    if (valid != DEF_YES) {                                     /* Rtn err if IF not valid.                             */
        CPU_CRITICAL_EXIT();
        return;
    }

    pif  = &Serial_IF_Tbl[if_nbr];
    pdev = &pif->Dev;
                                                                /* ------------------- ABORT DEV I/O ------------------ */
    pdev->Dev_Cfg->Drv_API->RxStop(pdev, perr);                 /* Stop dev rx.                                         */
    pdev->Dev_Cfg->Drv_API->TxStop(pdev, perr);                 /* Stop dev tx.                                         */

                                                                /* --------------------- CLOSE DEV -------------------- */
    pif->State = SERIAL_STATE_CLOSING;
    pdev->Dev_Cfg->Drv_API->Close(pdev, perr);                  /* Close dev  drv.                                      */
    pdev->LineDrv_API->Close(pdev, pdev->LineDrv_Data, perr);   /* Close line drv.                                      */

    rd_callback_arg = pif->RdCallbackArg;
    rd_callback_buf = pif->RdCallbackBuf;
    rd_callback     = pif->RdCallback;

    wr_callback_arg = pif->WrCallbackArg;
    wr_callback_buf = pif->WrCallbackBuf;
    wr_callback     = pif->WrCallback;

    Serial_IF_Clr(pif);                                         /* Clr IF.                                              */

#if (SERIAL_CFG_RD_BUF_EN == DEF_ENABLED)                       /* Clr rd buf.                                          */
    if (pif->RdBufEn == DEF_YES) {
        SerialBuf_Clr(&(pif->RdBuf));
    }
#endif
#if (SERIAL_CFG_WR_BUF_EN == DEF_ENABLED)                       /* Clr wr buf.                                          */
    if (pif->WrBufEn == DEF_YES) {
        SerialBuf_Clr(&(pif->WrBuf));
    }
#endif
    CPU_CRITICAL_EXIT();

    SerialOS_SemDel(pif->WrSem);
    pif->WrSem = (void *)0;

    SerialOS_SemDel(pif->RdSem);
    pif->RdSem = (void *)0;


    if (rd_callback != (SERIAL_CALLBACK_FNCT *)0) {
      (*rd_callback)((SERIAL_IF_NBR)pif->Nbr,                   /* Call rd callback fnct.                               */
                     (void        *)rd_callback_arg,
                     (void        *)rd_callback_buf,
                     (CPU_SIZE_T   )0,
                     (SERIAL_ERR   )SERIAL_ERR_IF_CLOSING);
    }


    if (wr_callback != (SERIAL_CALLBACK_FNCT *)0) {
      (*wr_callback)((SERIAL_IF_NBR)pif->Nbr,                   /* Call wr callback fnct.                               */
                     (void        *)wr_callback_arg,
                     (void        *)wr_callback_buf,
                     (CPU_SIZE_T   )0,
                     (SERIAL_ERR   )SERIAL_ERR_IF_CLOSING);
    }


#if (SERIAL_CFG_TX_DESC_NBR > 0)
    pif->TxBufListEnd = (SERIAL_BUF_DESC *)0;
    pbuf              =  pif->TxBufListStart;                   /* Inform any pending callback.                         */
    while (pbuf != (SERIAL_BUF_DESC *)0) {
        pbuf->Callback((SERIAL_IF_NBR)pif->Nbr,
                       (void        *)pbuf->CallbackArg,
                       (void        *)pbuf->CallbackBuf,
                       (CPU_SIZE_T   )0,
                       (SERIAL_ERR   )SERIAL_ERR_IF_CLOSING);

        pbuf_next          =  pbuf->NextPtr;
        pbuf_next->PrevPtr = (SERIAL_BUF_DESC *)0;

        Mem_PoolBlkFree(&pif->TxBufPool, pbuf, &lib_err);

        pbuf                = pbuf_next;
        pif->TxBufListStart = pbuf;
    }
#endif


    CPU_CRITICAL_ENTER();
    pif->State = SERIAL_STATE_CLOSED;
    CPU_CRITICAL_EXIT();

   *perr = SERIAL_ERR_NONE;
}


/*$PAGE*/
/*
*********************************************************************************************************
*                                             Serial_Rd()
*
* Description : Read synchronously from a serial interface with an optional time-out interval.
*
* Argument(s) : if_nbr      Interface number.
*
*               pdest       Pointer to destination buffer.
*
*               len         Number of octets to read.
*
*               timeout_ms  Optional timeout period in milliseconds. If specified 0, wait indefinitely.
*
*               perr        Pointer to variable that will receive the return error code from this function :
*
*                               SERIAL_ERR_NONE                 Device successfully read.
*                               SERIAL_ERR_SIGNAL               OS semaphore could NOT be created.
*                               SERIAL_ERR_IF_INVALID_NBR       Invalid serial interface number.
*                               SERIAL_ERR_IF_NOT_OPEN          Serial interface NOT open.
*
*                                                               --- RETURNED BY Serial_RdHandler() : ---
*                               SERIAL_ERR_NULL_PTR             NULL pointer passed for argument 'pdest'.
*
*                                                               ----- RETURNED BY Serial_RdEnd() : -----
*                               SERIAL_ERR_TIMEOUT              Operation timed-out.
*                               SERIAL_ERR_FAIL                 Device read failed.
*
* Return(s)   : Number of octets read.
*
* Caller(s)   : Application.
*
* Note(s)     : (1) Only ONE task/context can read from the same serial interface at a time.  If the receiver
*                   is busy when this function is called, an error will be returned.
*********************************************************************************************************
*/

CPU_SIZE_T  Serial_Rd (SERIAL_IF_NBR   if_nbr,
                       void           *pdest,
                       CPU_SIZE_T      len,
                       CPU_INT32U      timeout_ms,
                       SERIAL_ERR     *perr)
{
    SERIAL_IF             *pif;
    SERIAL_CALLBACK_INFO   rd_info;
    CPU_SIZE_T             len_rd;
    void                  *psignal;
#if (SERIAL_CFG_ARG_CHK_EXT_EN == DEF_ENABLED)
    CPU_BOOLEAN            valid;
#endif


#if (SERIAL_CFG_ARG_CHK_EXT_EN == DEF_ENABLED)                  /* ------------------- VALIDATE ARGS ------------------ */
    valid = Serial_IF_IsValid(if_nbr, perr);
    if (valid != DEF_YES) {                                     /* Rtn err if IF not valid.                             */
        return ((CPU_SIZE_T)0);
    }
#endif

    if (len == (CPU_SIZE_T)0) {                                 /* Chk for zero len.                                    */
       *perr = SERIAL_ERR_NONE;
        return ((CPU_SIZE_T)0);
    }

    psignal = SerialOS_SemCreate((CPU_INT16U)0);
    if (psignal == (void *)0) {
        *perr  = SERIAL_ERR_SIGNAL;
         return ((CPU_SIZE_T)0);
    }
    rd_info.SignalPtr = (void     *)psignal;
    rd_info.Timeout   = (CPU_INT32U)timeout_ms;
    rd_info.Len       = (CPU_SIZE_T)0;
    rd_info.Err       = (SERIAL_ERR)SERIAL_ERR_NONE;

    pif = &Serial_IF_Tbl[if_nbr];

    Serial_RdHandler((SERIAL_IF            *) pif,
                     (void                 *) pdest,
                     (CPU_SIZE_T            ) len,
                     (SERIAL_CALLBACK_FNCT *)&Serial_RdCallback,
                     (void                 *)&rd_info,
                     (CPU_BOOLEAN           ) DEF_NO,
                     (CPU_INT32U            ) timeout_ms,
                     (SERIAL_ERR           *) perr);

    if (*perr == SERIAL_ERR_NONE) {
        len_rd = Serial_RdEnd(pif, &rd_info, perr);             /* Wait for rd end.                                     */
    } else {
        len_rd = 0;
    }

    SerialOS_SemDel(psignal);
    return (len_rd);
}


/*$PAGE*/
/*
*********************************************************************************************************
*                                          Serial_RdAsync()
*
* Description : Read asynchronously from a serial interface.
*
* Argument(s) : if_nbr      Interface number.
*
*               pdest       Pointer to destination buffer.
*
*               len         Number of octets to read.
*
*               phandler    Address of read complete callback function.
*
*               parg        Pointer to read complete callback context.
*
*               perr        Pointer to variable that will receive the return error code from this function :
*
*                               SERIAL_ERR_NONE                 Asynchronous operation successful.
*                               SERIAL_ERR_NULL_PTR             NULL pointer passed for argument 'pdest'.
*                               SERIAL_ERR_NULL_FNCT            NULL pointer passed for argument 'phandler'.
*                               SERIAL_ERR_IF_IN_USE            Serial interface is in use.
*                               SERIAL_ERR_IF_INVALID_NBR       Invalid serial interface number.
*                               SERIAL_ERR_IF_NOT_OPEN          Serial interface NOT open.
*
* Return(s)   : None.
*
* Caller(s)   : Application.
*
* Note(s)     : (1) Only ONE task/context can read from the same serial interface at a time.  If the receiver
*                   is busy when this function is called, an error will be returned.
*********************************************************************************************************
*/

void  Serial_RdAsync (SERIAL_IF_NBR          if_nbr,
                      void                  *pdest,
                      CPU_SIZE_T             len,
                      SERIAL_CALLBACK_FNCT  *phandler,
                      void                  *parg,
                      SERIAL_ERR            *perr)
{
    SERIAL_IF    *pif;
#if (SERIAL_CFG_ARG_CHK_EXT_EN == DEF_ENABLED)
    CPU_BOOLEAN   valid;
#endif


#if (SERIAL_CFG_ARG_CHK_EXT_EN == DEF_ENABLED)                  /* ------------------- VALIDATE ARGS ------------------ */
    valid = Serial_IF_IsValid(if_nbr, perr);
    if (valid != DEF_YES) {                                     /* Rtn err if IF not valid.                             */
        return;
    }
#endif

    if (len == (CPU_SIZE_T)0) {                                 /* Chk for zero len.                                    */
       *perr = SERIAL_ERR_NONE;
        return;
    }

    pif = &Serial_IF_Tbl[if_nbr];

    Serial_RdHandler(pif,
                     pdest,
                     len,
                     phandler,
                     parg,
                     DEF_YES,
                     0,
                     perr);
}


/*$PAGE*/
/*
*********************************************************************************************************
*                                          Serial_WaitFor()
*
* Description : Wait for octet array to arrive on serial interface.
*
* Argument(s) : if_nbr      Interface number.
*
*               psrc        Pointer to buffer to wait for.
*
*               len         Number of octets in the buffer.
*
*               timeout_ms  Optional timeout period in milliseconds. If specified 0, wait indefinitely.
*
*               perr        Pointer to variable that will receive the return error code from this function :
*
*                               SERIAL_ERR_NONE                 Octet array arrived on serial interface.
*                               SERIAL_ERR_NULL_PTR             NULL pointer passed for argument 'psrc'.
*                               SERIAL_ERR_SIGNAL               OS semaphore could NOT be created.
*
*                                                               -- RETURNED BY Serial_IF_IsValid() : --
*                               SERIAL_ERR_IF_INVALID_NBR       Invalid serial interface number.
*                               SERIAL_ERR_IF_NOT_OPEN          Serial interface NOT open.
*
*                                                               ----- RETURNED BY Serial_RdEnd() : ----
*                               SERIAL_ERR_TIMEOUT              Operation timed-out.
*                               SERIAL_ERR_FAIL                 Operation failed.
*
* Return(s)   : None.
*
* Caller(s)   : Application.
*
* Note(s)     : (1) Only ONE task/context can read from the same serial interface at a time.  If the receiver
*                   is busy when this function is called, an error will be returned.
*
*               (2) This function bypasses the line driver for new incoming octets, and may use rd
*                   buf for initial comparison.
*********************************************************************************************************
*/

void  Serial_WaitFor (SERIAL_IF_NBR   if_nbr,
                      void           *psrc,
                      CPU_SIZE_T      len,
                      CPU_INT32U      timeout_ms,
                      SERIAL_ERR     *perr)
{
    SERIAL_CALLBACK_INFO   rd_info;
    SERIAL_IF             *pif;
    CPU_BOOLEAN            valid;
    void                  *psignal;
#if (SERIAL_CFG_RD_BUF_EN == DEF_ENABLED)
    CPU_BOOLEAN            cmp;
    CPU_BOOLEAN            empty;
    CPU_BOOLEAN            rd;
    CPU_INT08U             datum;
#endif
    CPU_SR_ALLOC();


#if (SERIAL_CFG_ARG_CHK_EXT_EN == DEF_ENABLED)                  /* ------------------- VALIDATE ARGS ------------------ */
    if (psrc == (void *)0) {                                    /* Validate src buf ptr.                                */
       *perr = SERIAL_ERR_NULL_PTR;
        return;
    }
#endif

    if (len == (CPU_SIZE_T)0) {                                 /* Chk for zero len.                                    */
       *perr = SERIAL_ERR_NONE;
        return;
    }

                                                                /* ------------------ VALIDATE IF NBR ----------------- */
    valid = Serial_IF_IsValid(if_nbr, perr);
    if (valid != DEF_YES) {                                     /* Rtn err if IF not valid.                             */
        return;
    }

    pif = &Serial_IF_Tbl[if_nbr];
    SerialOS_SemWait(pif->RdSem, 0, perr);
    if (*perr != SERIAL_OS_ERR_NONE) {
         return;
    }

    CPU_CRITICAL_ENTER();

    SerialBuf_Init((SERIAL_BUF  *)&(pif->RdUserBuf),            /* Init cmp buf.                                        */
                   (CPU_INT08U  *)  psrc,
                   (CPU_SIZE_T   )  len,
                   (CPU_BOOLEAN  )  DEF_YES);

#if (SERIAL_CFG_RD_BUF_EN == DEF_ENABLED)                       /* -------------------- CHK RD BUF -------------------- */
    if (pif->RdBufEn == DEF_YES) {
        empty = SerialBuf_IsEmpty(&(pif->RdBuf));
        if (empty == DEF_NO) {
            CPU_CRITICAL_EXIT();
            cmp = DEF_NO;
            do {
                CPU_CRITICAL_ENTER();
                rd = SerialBuf_RdOctet(&(pif->RdBuf), &datum);
                if (rd == DEF_YES) {
                    cmp = SerialBuf_Cmp(&(pif->RdUserBuf), datum);
                    if (cmp != DEF_YES) {
                        CPU_CRITICAL_EXIT();
                    }
                }
            } while ((rd  == DEF_YES) &&
                     (cmp != DEF_YES));

            empty = SerialBuf_IsEmpty(&(pif->RdBuf));
            if ((cmp   == DEF_YES) &&
               ((empty == DEF_NO)  ||
                (pif->RdBufErr == SERIAL_ERR_NONE))) {
                    CPU_CRITICAL_EXIT();
                    SerialOS_SemSignal(pif->RdSem);
                   *perr  = SERIAL_ERR_NONE;
                    return;
            }

            if (pif->RdBufErr != SERIAL_ERR_NONE) {             /* Restart cmp if rd buf err.                           */
                SerialBuf_Init((SERIAL_BUF  *)&(pif->RdUserBuf),
                               (CPU_INT08U  *)  psrc,
                               (CPU_SIZE_T   )  len,
                               (CPU_BOOLEAN  )  DEF_YES);

                pif->RdBufErr  = SERIAL_ERR_NONE;               /* Flush rd buf.                                        */
                SerialBuf_Clr(&(pif->RdBuf));
            }

        } else if (pif->RdBufErr != SERIAL_ERR_NONE) {
            pif->RdBufErr = SERIAL_ERR_NONE;                    /* Flush rd buf.                                        */
            SerialBuf_Clr(&(pif->RdBuf));
        }
    }
#endif


    psignal = SerialOS_SemCreate((CPU_INT16U)0);
    if (psignal == (void *)0) {
        CPU_CRITICAL_EXIT();
        SerialOS_SemSignal(pif->RdSem);
       *perr = SERIAL_ERR_SIGNAL;
        return;
    }
    rd_info.SignalPtr = (void     *)psignal;
    rd_info.Timeout   = (CPU_INT32U)timeout_ms;
    rd_info.Len       = (CPU_SIZE_T)0;
    rd_info.Err       = (SERIAL_ERR)SERIAL_ERR_NONE;


                                                                /* --------------------- START RD --------------------- */
    pif->RdCallback    = &Serial_RdCallback;                    /* Callback called upon completion.                     */
    pif->RdCallbackArg = (void *)&rd_info;                      /* Arg     passed to callback.                          */
    pif->RdCallbackBuf =  psrc;                                 /* Buf     passed to callback.                          */
#if (SERIAL_CFG_RD_BUF_EN == DEF_ENABLED)
    pif->RdCallbackLen =  len;                                  /* Buf len passed to callback.                          */
#endif
    pif->RdState       =  SERIAL_RD_STATE_CHK;

    Serial_IF_RxStart(pif);                                     /* Start rx'er.                                         */
    CPU_CRITICAL_EXIT();


   (void)Serial_RdEnd(pif, &rd_info, perr);                     /* Wait for rd end.                                     */
    SerialOS_SemDel(psignal);
}


/*$PAGE*/
/*
*********************************************************************************************************
*                                             Serial_Wr()
*
* Description : Write synchronously to a serial interface with an optional time-out interval.
*
* Argument(s) : if_nbr      Interface number.
*
*               psrc        Pointer to source buffer.
*
*               len         Number of octets to write.
*
*               timeout_ms  Optional timeout period in milliseconds. If specified 0, wait indefinitely.
*
*               perr        Pointer to variable that will receive the return error code from this function :
*
*                               SERIAL_ERR_NONE                 Device successfully written.
*                               SERIAL_ERR_SIGNAL               OS semaphore could NOT be created.
*                               SERIAL_ERR_NULL_PTR             NULL pointer passed for argument 'psrc'.
*                               SERIAL_ERR_IF_INVALID_NBR       Invalid serial interface number.
*                               SERIAL_ERR_IF_NOT_OPEN          Serial interface NOT open.
*
*                                                               ---- RETURNED BY Serial_WrStart() : ---
*                               SERIAL_ERR_BUF_ALLOC            Buffer structure could NOT be allocated.
*
*                                                               ---- RETURNED BY Serial_WrEnd() : -----
*                               SERIAL_ERR_TIMEOUT              Operation timed-out.
*                               SERIAL_ERR_FAIL                 Device write failed.
*
* Return(s)   : Number of octets written.
*
* Caller(s)   : Application.
*
* Note(s)     : (1) Multiple tasks/contexts may write to the same serial interface at any time.
*
*               (2) A synchronous write CANNOT be requested in interrupt context.
*********************************************************************************************************
*/

CPU_SIZE_T  Serial_Wr (SERIAL_IF_NBR   if_nbr,
                       void           *psrc,
                       CPU_SIZE_T      len,
                       CPU_INT32U      timeout_ms,
                       SERIAL_ERR     *perr)
{
    SERIAL_IF             *pif;
    CPU_BOOLEAN            valid;
    SERIAL_CALLBACK_INFO   wr_info;
    CPU_SIZE_T             len_wr;
    void                  *psignal;
    SERIAL_BUF_DESC        buf;
    CPU_SR_ALLOC();


#if (SERIAL_CFG_ARG_CHK_EXT_EN == DEF_ENABLED)                  /* ------------------- VALIDATE ARGS ------------------ */
    if (psrc == (void *)0) {                                    /* Validate src buf ptr.                                */
       *perr = SERIAL_ERR_NULL_PTR;
        return ((CPU_SIZE_T)0);
    }
#endif


    if (len == (CPU_SIZE_T)0) {                                 /* Chk for zero len.                                    */
       *perr = SERIAL_ERR_NONE;
        return ((CPU_SIZE_T)0);
    }

    wr_info.Timeout = (CPU_INT32U)timeout_ms;
    wr_info.Len     = (CPU_SIZE_T)0;
    wr_info.Err     = (SERIAL_ERR)SERIAL_ERR_NONE;


    buf.Len         =  len;
    buf.DataPtr     =  psrc;
    buf.Callback    = &Serial_WrCallback;                       /* Callback called upon completion.                     */
    buf.CallbackArg = &wr_info;                                 /* Argument passed to callback.                         */
    buf.CallbackBuf =  psrc;                                    /* Buf      passed to callback.                         */


                                                                /* ------------------ VALIDATE IF NBR ----------------- */
    valid = Serial_IF_IsValid(if_nbr, perr);
    if (valid != DEF_YES) {                                     /* Rtn err if IF not valid.                             */
        return ((CPU_SIZE_T)0);
    }


    pif = &Serial_IF_Tbl[if_nbr];

    SerialOS_SemWait(pif->WrSem, 0, perr);
    if (*perr != SERIAL_OS_ERR_NONE) {
         return ((CPU_SIZE_T)0);
    }

    psignal = SerialOS_SemCreate((CPU_INT16U)0);
    if (psignal == (void *)0) {
        SerialOS_SemSignal(pif->WrSem);
       *perr = SERIAL_ERR_SIGNAL;
        return ((CPU_SIZE_T)0);
    }

    wr_info.SignalPtr = (void *)psignal;

    CPU_CRITICAL_ENTER();
    Serial_WrStart(pif, &buf, perr);                            /* Start wr.                                            */
    CPU_CRITICAL_EXIT();

    if (*perr == SERIAL_ERR_NONE) {
        len_wr = Serial_WrEnd(pif, &wr_info, perr);             /* Wait for wr end.                                     */
    } else{
        len_wr = 0;
    }

    SerialOS_SemDel(psignal);
    return (len_wr);
}


/*$PAGE*/
/*
*********************************************************************************************************
*                                          Serial_WrAsync()
*
* Description : Write asynchronously to a serial interface.
*
* Argument(s) : if_nbr      Interface number.
*
*               psrc        Pointer to source buffer.
*
*               len         Number of octets to write.
*
*               phandler    Address of write complete callback function.
*
*               parg        Pointer to write complete callback context.
*
*               perr        Pointer to variable that will receive the return error code from this function :
*
*                               SERIAL_ERR_NONE             Device successfully written.
*                               SERIAL_ERR_NULL_PTR         NULL pointer passed for argument 'psrc'.
*                               SERIAL_ERR_NULL_FNCT        NULL pointer passed for argument 'phandler'.
*                               SERIAL_ERR_IF_IN_USE        Serial interface is in use.
*
*                                                           ---- RETURNED BY Serial_IF_IsValid() : ----
*                               SERIAL_ERR_IF_INVALID_NBR   Invalid serial interface number.
*                               SERIAL_ERR_IF_NOT_OPEN      Serial interface NOT open.
*
* Return(s)   : None.
*
* Caller(s)   : Application.
*
* Note(s)     : (1) Multiple tasks/contexts may write to the same serial interface at any time.
*********************************************************************************************************
*/

void  Serial_WrAsync (SERIAL_IF_NBR          if_nbr,
                      void                  *psrc,
                      CPU_SIZE_T             len,
                      SERIAL_CALLBACK_FNCT  *phandler,
                      void                  *parg,
                      SERIAL_ERR            *perr)
{
    SERIAL_IF        *pif;
    CPU_BOOLEAN       valid;
    SERIAL_BUF_DESC   buf;
    CPU_BOOLEAN       lock;
    CPU_SR_ALLOC();


#if (SERIAL_CFG_ARG_CHK_EXT_EN == DEF_ENABLED)                  /* ------------------- VALIDATE ARGS ------------------ */
    if (psrc == (void *)0) {                                    /* Validate src buf ptr.                                */
       *perr = SERIAL_ERR_NULL_PTR;
        return;
    }
    if (phandler == (void *)0) {                                /* Validate callback fnct ptr.                          */
       *perr = SERIAL_ERR_NULL_FNCT;
        return;
    }
#endif

    if (len == (CPU_SIZE_T)0) {                                 /* Chk for zero len.                                    */
       *perr = SERIAL_ERR_NONE;
        return;
    }


    buf.Len         = len;
    buf.DataPtr     = psrc;
    buf.Callback    = phandler;                                 /* Callback called upon completion.                     */
    buf.CallbackArg = parg;                                     /* Argument passed to callback.                         */
    buf.CallbackBuf = psrc;                                     /* Buf      passed to callback.                         */


    pif = &Serial_IF_Tbl[if_nbr];

                                                                /* ------------------ VALIDATE IF NBR ----------------- */
    CPU_CRITICAL_ENTER();
    valid = Serial_IF_IsValid(if_nbr, perr);
    if (valid != DEF_YES) {                                     /* Rtn err if IF not valid.                             */
        CPU_CRITICAL_EXIT();
        return;
    }

    lock = SerialOS_SemTryLock(pif->WrSem);
    if (lock != DEF_OK) {
        CPU_CRITICAL_EXIT();
       *perr = SERIAL_ERR_IF_IN_USE;
        return;
    }

    Serial_WrStart(pif, &buf, perr);                            /* Start wr.                                            */
    CPU_CRITICAL_EXIT();
}


/*$PAGE*/
/*
*********************************************************************************************************
*********************************************************************************************************
*                                     FUNCTIONS CALLED BY DRIVER
*********************************************************************************************************
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                        SerialIF_GetDevPtr()
*
* Description : Get serial interface's device pointer.
*
* Argument(s) : if_nbr      Serial interface number to validate.
*
*               perr        Pointer to variable that will receive the return error code from this function :
*
*                               SERIAL_ERR_NONE                 Serial interface device structure successfully
*                                                                   returned.
*
*                                                               ------ RETURNED BY Serial_IF_IsValid() : -----
*                               SERIAL_ERR_IF_INVALID_NBR       Invalid serial interface number.
*                               SERIAL_ERR_IF_NOT_OPEN          Serial interface NOT open.
*
* Return(s)   : Pointer to serial interface device structure, if NO errors.
*               Pointer to NULL,                              otherwise.
*
* Caller(s)   : Line driver functions.
*
*               This function is an INTERNAL serial interface suite function & MUST NOT be called by
*               application function(s).
*
* Note(s)     : None.
*********************************************************************************************************
*/

SERIAL_DEV  *SerialIF_GetDevPtr (SERIAL_IF_NBR   if_nbr,
                                 SERIAL_ERR     *perr)
{
    SERIAL_IF    *pif;
    SERIAL_DEV   *pdev;
    CPU_BOOLEAN   valid;

                                                                /* ------------------ VALIDATE IF NBR ----------------- */
    valid = Serial_IF_IsValid(if_nbr, perr);
    if (valid != DEF_YES) {                                     /* Rtn err if IF not valid.                             */
        return ((SERIAL_DEV *)0);
    }

                                                                /* ----------------- GET LINE DRV INFO ---------------- */
    pif  = &Serial_IF_Tbl[if_nbr];
    pdev = &pif->Dev;
   *perr =  SERIAL_ERR_NONE;

    return (pdev);
}


/*$PAGE*/
/*
*********************************************************************************************************
*                                            SerialIF_Rx()
*
* Description : Handle a completed read.
*
* Argument(s) : pdev        Pointer to device.
*
*               datum       Read octet.
*
* Return(s)   : None.
*
* Caller(s)   : Driver ISR Handler.
*
*               This function is an INTERNAL serial interface suite function & MUST NOT be called by
*               application function(s).
*
* Note(s)     : None.
*********************************************************************************************************
*/

void  SerialIF_Rx (SERIAL_DEV  *pdev,
                   CPU_INT08U   datum)
{
    SERIAL_IF             *pif;
    SERIAL_IF_NBR          if_nbr;
    SERIAL_ERR             err;
    SERIAL_BUF            *pbuf;
    SERIAL_CALLBACK_FNCT  *callback;
    void                  *callback_arg;
    void                  *callback_buf;
    CPU_BOOLEAN            done;
    CPU_BOOLEAN            cmp;
    CPU_BOOLEAN            full;
    CPU_SIZE_T             rd_len;
#if (SERIAL_CFG_RD_BUF_EN == DEF_ENABLED)
    CPU_BOOLEAN            rd;
#endif
    CPU_SR_ALLOC();


#if (SERIAL_CFG_ARG_CHK_EXT_EN == DEF_ENABLED)                  /* ------------------- VALIDATE ARGS ------------------ */
    if (pdev == (void *)0) {                                    /* Validate dev ptr.                                    */
        return;
    }

    Serial_IF_IsValid(pdev->IF_Nbr, &err);
    if (err != SERIAL_ERR_NONE) {
        return;
    }
#endif


    if_nbr =  pdev->IF_Nbr;
    pif    = &Serial_IF_Tbl[if_nbr];
    done   =  DEF_NO;

    CPU_CRITICAL_ENTER();
    switch (pif->RdState) {
        case SERIAL_RD_STATE_OPENED:                            /* -------------------- RD INTO BUF ------------------- */
#if (SERIAL_CFG_RD_BUF_EN == DEF_ENABLED)
             if (pif->RdBufEn == DEF_YES) {
                 if (pif->RdBufErr == SERIAL_ERR_NONE) {        /* Wr octet.                                            */
                     pdev->LineDrv_API->WrOctet(pdev,
                                              &(pif->RdBuf),
                                                datum,
                                              &(pif->RdBufErr));
                 }
             }
#endif
             CPU_CRITICAL_EXIT();
             return;



        case SERIAL_RD_STATE_CLOSED:
        default:
             pdev->Dev_Cfg->Drv_API->RxStop(pdev, &err);        /* Stop rx'er.                                          */
             CPU_CRITICAL_EXIT();
             return;



        case SERIAL_RD_STATE_RD:                                /* --------------- RD DATA INTO USER BUF -------------- */
             pbuf = &(pif->RdUserBuf);
             pdev->LineDrv_API->WrOctet(pdev,                   /* Wr octet.                                            */
                                        pbuf,
                                        datum,
                                       &err);

             full = SerialBuf_IsFull(pbuf);

             if ((err  != SERIAL_ERR_NONE) ||
                 (full == DEF_YES)) {
                 done = DEF_YES;
             }
             break;



        case SERIAL_RD_STATE_CHK:                               /* ------------- CHK DATA AGAINST USER BUF ------------ */
#if (SERIAL_CFG_RD_BUF_EN == DEF_ENABLED)
             pbuf = &(pif->RdUserBuf);

             if (pif->RdBufEn == DEF_YES) {
                 if (pif->RdBufErr != SERIAL_ERR_NONE) {
                     pif->RdBufErr  = SERIAL_ERR_NONE;          /* Flush rd buf.                                        */
                     SerialBuf_Clr(&(pif->RdBuf));
                                                                /* Restart cmp if rd buf err.                           */
                     SerialBuf_Init((SERIAL_BUF  *)pbuf,
                                    (CPU_INT08U  *)pif->RdCallbackBuf,
                                    (CPU_SIZE_T   )pif->RdCallbackLen,
                                    (CPU_BOOLEAN  )DEF_YES);
                 }

                 pdev->LineDrv_API->WrOctet(pdev,
                                          &(pif->RdBuf),
                                            datum,
                                          &(pif->RdBufErr));

                 do {
                     rd = SerialBuf_RdOctet(&(pif->RdBuf), &datum);
                     if (rd == DEF_YES) {
                         cmp = SerialBuf_Cmp(pbuf, datum);
                         if (cmp == DEF_YES) {
                             done = DEF_YES;
                             rd   = DEF_NO;
                         }
                     }
                 } while (rd == DEF_YES);

             } else {

                 cmp = SerialBuf_Cmp(pbuf, datum);
                 if (cmp == DEF_YES) {
                     done = DEF_YES;
                     err  = SERIAL_ERR_NONE;
                 }
             }
#else
             pbuf = &(pif->RdUserBuf);
             cmp  = SerialBuf_Cmp(pbuf, datum);
             if (cmp == DEF_YES) {
                 done = DEF_YES;
                 err  = SERIAL_ERR_NONE;
             }
#endif
             break;
    }

    if (done == DEF_YES) {
        rd_len        = SerialBuf_DataLen(pbuf);
#if (SERIAL_CFG_RD_BUF_EN == DEF_ENABLED)
        rd_len       += pif->RdCallbackLen;
#endif
        callback_arg  = pif->RdCallbackArg;
        callback_buf  = pif->RdCallbackBuf;
        callback      = pif->RdCallback;

        Serial_IF_RxReset(pif);
        SerialOS_SemSignal(pif->RdSem);

        if (callback != (SERIAL_CALLBACK_FNCT *)0) {
            CPU_CRITICAL_EXIT();

            (*callback)(pif->Nbr,                               /* Call callback fnct.                                  */
                        callback_arg,
                        callback_buf,
                        rd_len,
                        err);

            return;
        }
    }

    CPU_CRITICAL_EXIT();
}


/*$PAGE*/
/*
*********************************************************************************************************
*                                            SerialIF_Tx()
*
* Description : Handle a completed transmission.
*
* Argument(s) : pdev        Pointer to device.
*
* Return(s)   : None.
*
* Caller(s)   : Driver ISR Handler.
*
*               This function is an INTERNAL serial interface suite function & MUST NOT be called by
*               application function(s).
*
* Note(s)     : None.
*********************************************************************************************************
*/

void  SerialIF_Tx (SERIAL_DEV  *pdev)
{
    SERIAL_IF             *pif;
    SERIAL_IF_NBR          if_nbr;
    SERIAL_ERR             err;
    SERIAL_BUF            *pbuf;
    SERIAL_CALLBACK_FNCT  *callback;
    void                  *callback_arg;
    void                  *callback_buf;
    CPU_INT08U             datum;
    CPU_SIZE_T             wr_len;
    CPU_SIZE_T             data_len;
    CPU_SIZE_T             buf_size;
    CPU_SR_ALLOC();


#if (SERIAL_CFG_ARG_CHK_EXT_EN == DEF_ENABLED)                  /* ------------------- VALIDATE ARGS ------------------ */
    if (pdev == (void *)0) {                                    /* Validate dev ptr.                                    */
        return;
    }

    Serial_IF_IsValid(pdev->IF_Nbr, &err);
    if (err != SERIAL_ERR_NONE) {
        return;
    }
#endif


    if_nbr   =  pdev->IF_Nbr;
    pif      = &Serial_IF_Tbl[if_nbr];

    CPU_CRITICAL_ENTER();
    switch (pif->WrState) {
        case SERIAL_WR_STATE_WR:                                /* ------------------- TX NEXT OCTET ------------------ */
             pbuf = &(pif->WrUserBuf);
             pdev->LineDrv_API->RdOctet(pdev,                   /* Rd next octet.                                       */
                                        pbuf,
                                       &datum,
                                       &err);

             if (err == SERIAL_ERR_NONE) {                      /* Tx next octet.                                       */
                 pdev->Dev_Cfg->Drv_API->TxOctet(pdev, datum, &err);
                 CPU_CRITICAL_EXIT();
                 return;
             }


                                                                /* ------------- FINAL OCTET ALREADY WR'N ------------- */
             buf_size     = SerialBuf_Size(pbuf);
             data_len     = SerialBuf_DataLen(pbuf);
             wr_len       = buf_size - data_len;
             callback     = pif->WrCallback;
             callback_arg = pif->WrCallbackArg;
             callback_buf = pif->WrCallbackBuf;

             Serial_IF_WrClr(pif);                              /* Clr wr info.                                         */

             Serial_WrNext(pif);
             CPU_CRITICAL_EXIT();

             SerialOS_SemSignal(pif->WrSem);

             if (callback != (SERIAL_CALLBACK_FNCT *)0) {
               (*callback)(pif->Nbr,                            /* Call callback fnct.                                  */
                           callback_arg,
                           callback_buf,
                           wr_len,
                           err == SERIAL_ERR_UNDERFLOW ? SERIAL_ERR_NONE : err);
             }
             break;


#if (SERIAL_CFG_WR_BUF_EN == DEF_ENABLED)
        case SERIAL_WR_STATE_WR_BUF:                            /* ------------------- TX NEXT OCTET ------------------ */
             Serial_WrNext(pif);
             CPU_CRITICAL_EXIT();
             break;
#endif


        case SERIAL_WR_STATE_CLOSED:                            /* ----------------- NO WR IN PROGRESS ---------------- */
        case SERIAL_WR_STATE_OPENED:
        default:
             pdev->Dev_Cfg->Drv_API->TxStop(pdev, &err);        /* Stop tx'er.                                          */
             CPU_CRITICAL_EXIT();
             break;
    }
}


/*$PAGE*/
/*
*********************************************************************************************************
*                                         SerialIF_TxWrBuf()
*
* Description : Handle a transmit echo.
*
* Argument(s) : pdev        Pointer to device.
*
*               datum       Octet to transmit.
*
* Return(s)   : None.
*
* Caller(s)   : SerialLine_WrOctet().
*
*               This function is an INTERNAL serial interface suite function & MUST NOT be called by
*               application function(s).
*
* Note(s)     : (1) Interrupts are assumed to be disabled when this function is called.
*********************************************************************************************************
*/
#if (SERIAL_CFG_WR_BUF_EN == DEF_ENABLED)
void  SerialIF_TxWrBuf (SERIAL_DEV  *pdev,
                        CPU_INT08U   datum)
{
    SERIAL_IF      *pif;
    SERIAL_IF_NBR   if_nbr;
    SERIAL_ERR      err;


#if (SERIAL_CFG_ARG_CHK_EXT_EN == DEF_ENABLED)                  /* ------------------- VALIDATE ARGS ------------------ */
    if (pdev == (void *)0) {                                    /* Validate dev ptr.                                    */
        return;
    }

    Serial_IF_IsValid(pdev->IF_Nbr, &err);
    if (err != SERIAL_ERR_NONE) {
        return;
    }
#endif


    if_nbr   =  pdev->IF_Nbr;
    pif      = &Serial_IF_Tbl[if_nbr];

    if (pif->WrBufEn == DEF_YES) {
        switch (pif->WrState) {
            case SERIAL_WR_STATE_OPENED:
                                                                /* Tx first octet.                                      */
                 pdev->Dev_Cfg->Drv_API->TxOctet(pdev, datum, &err);
                 break;


            case SERIAL_WR_STATE_WR:
            case SERIAL_WR_STATE_WR_BUF:
                (void)SerialBuf_WrOctet(&(pif->WrBuf), datum);
                 break;


            default:
            case SERIAL_WR_STATE_CLOSED:
                 break;
        }
    }
}
#endif


/*$PAGE*/
/*
*********************************************************************************************************
*********************************************************************************************************
*                                           LOCAL FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                           Serial_IF_Clr()
*
* Description : Clear interface.
*
* Argument(s) : pif         Pointer to interface.
*
* Return(s)   : None.
*
* Caller(s)   : Serial_Init(),
*               Serial_Close().
*
* Note(s)     : None.
*********************************************************************************************************
*/

static  void  Serial_IF_Clr (SERIAL_IF  *pif)
{
    pif->RdState = SERIAL_RD_STATE_CLOSED;
    Serial_IF_RdClr(pif);

    pif->WrState = SERIAL_WR_STATE_CLOSED;
    Serial_IF_WrClr(pif);

    pif->Dev.Dev_Cfg      = (SERIAL_DEV_CFG      *)0;
    pif->Dev.Drv_Data     = (void                *)0;
    pif->Dev.LineDrv_API  = (SERIAL_LINE_DRV_API *)0;
    pif->Dev.LineDrv_Data = (void                *)0;
}


/*$PAGE*/
/*
*********************************************************************************************************
*                                           Serial_IF_RdClr()
*
* Description : Clear read callback arguments and user buffer.
*
* Argument(s) : pif         Pointer to interface.
*
* Return(s)   : None.
*
* Caller(s)   : Serial_IF_Clr(),
*               Serial_IF_RxReset().
*
* Note(s)     : None.
*********************************************************************************************************
*/

static  void  Serial_IF_RdClr (SERIAL_IF  *pif)
{
    pif->RdCallback    = (SERIAL_CALLBACK_FNCT *)0;
    pif->RdCallbackArg = (void                 *)0;
    pif->RdCallbackBuf = (void                 *)0;
#if (SERIAL_CFG_RD_BUF_EN == DEF_ENABLED)
    pif->RdCallbackLen = (CPU_SIZE_T            )0;
#endif
    SerialBuf_Clr(&(pif->RdUserBuf));
}


/*$PAGE*/
/*
*********************************************************************************************************
*                                           Serial_IF_WrClr()
*
* Description : Clear write callback arguments and user buffer.
*
* Argument(s) : pif         Pointer to interface.
*
* Return(s)   : None.
*
* Caller(s)   : Serial_IF_Clr(),
*               Serial_IF_Tx().
*
* Note(s)     : None.
*********************************************************************************************************
*/

static  void  Serial_IF_WrClr (SERIAL_IF  *pif)
{
    pif->WrCallback    = (SERIAL_CALLBACK_FNCT *)0;
    pif->WrCallbackArg = (void                 *)0;
    pif->WrCallbackBuf = (void                 *)0;
    SerialBuf_Clr(&(pif->WrUserBuf));
}


/*$PAGE*/
/*
*********************************************************************************************************
*                                         Serial_IF_RxReset()
*
* Description : Reset receiver.
*
* Argument(s) : pif         Pointer to interface.
*
* Return(s)   : None.
*
* Caller(s)   : Serial_WaitFor(),
*               Serial_IF_Rx().
*
* Note(s)     : (1) Interrupts are assumed to be disabled when this function is called.
*********************************************************************************************************
*/

static  void  Serial_IF_RxReset (SERIAL_IF  *pif)
{
    Serial_IF_RxStop(pif);
    pif->RdState = SERIAL_RD_STATE_OPENED;
    Serial_IF_RdClr(pif);
}


/*$PAGE*/
/*
*********************************************************************************************************
*                                         Serial_IF_RxStart()
*
* Description : Start receiver.
*
* Argument(s) : pif         Pointer to interface.
*
* Return(s)   : None.
*
* Caller(s)   : Serial_RdStart(),
*               Serial_WaitFor().
*
* Note(s)     : (1) Interrupts are assumed to be disabled when this function is called.
*********************************************************************************************************
*/

static  void  Serial_IF_RxStart (SERIAL_IF  *pif)
{
    SERIAL_DEV  *pdev;
    SERIAL_ERR   err;


    pdev = &pif->Dev;
                                                                /* Start rx'er, if necessary.                           */
#if (SERIAL_CFG_RD_BUF_EN == DEF_ENABLED)
    if (pif->RdBufEn == DEF_NO) {
        pdev->Dev_Cfg->Drv_API->RxStart(pdev, &err);
    }
#else
    pdev->Dev_Cfg->Drv_API->RxStart(pdev, &err);
#endif
}


/*$PAGE*/
/*
*********************************************************************************************************
*                                         Serial_IF_RxStop()
*
* Description : Stop receiver.
*
* Argument(s) : pif         Pointer to interface.
*
* Return(s)   : None.
*
* Caller(s)   : Serial_IF_RxReset().
*
* Note(s)     : (1) Interrupts are assumed to be disabled when this function is called.
*********************************************************************************************************
*/

static  void  Serial_IF_RxStop (SERIAL_IF  *pif)
{
    SERIAL_DEV  *pdev;
    SERIAL_ERR   err;


    pdev = &pif->Dev;
                                                                /* Stop rx'er, if necessary.                            */
#if (SERIAL_CFG_RD_BUF_EN == DEF_ENABLED)
    if (pif->RdBufEn == DEF_NO) {
        pdev->Dev_Cfg->Drv_API->RxStop(pdev, &err);
    }
#else
    pdev->Dev_Cfg->Drv_API->RxStop(pdev, &err);
#endif
}


/*$PAGE*/
/*
*********************************************************************************************************
*                                          Serial_RdHandler()
*
* Description : Read asynchronously or synchronously from a serial interface.
*
* Argument(s) : pif         Pointer to interface.
*
*               pdest       Pointer to destination buffer.
*
*               len         Number of octets to read.
*
*               phandler    Address of read complete callback function.
*
*               parg        Pointer to read complete callback context.
*
*               async       Select asynchronously or synchronously method.
*
*               timeout_ms  Optional timeout period in milliseconds for synchronous operation.
*                           If specified 0, wait indefinitely.
*
*               perr        Pointer to variable that will receive the return error code from this function :
*
*                               SERIAL_ERR_NONE                 Read operation completed successfully.
*                               SERIAL_ERR_NULL_PTR             NULL pointer passed for argument 'pdest'.
*                               SERIAL_ERR_NULL_FNCT            NULL pointer passed for argument 'phandler'.
*                               SERIAL_ERR_IF_IN_USE            Serial interface is in use.
*
*                                                               ---- RETURNED BY Serial_IF_IsValid() : ----
*                               SERIAL_ERR_IF_INVALID_NBR       Invalid serial interface number.
*                               SERIAL_ERR_IF_NOT_OPEN          Serial interface NOT open.
*
* Return(s)   : None.
*
* Caller(s)   : Application.
*
* Note(s)     : (1) Only ONE task/context can read from the same serial interface at a time.  If the receiver
*                   is busy when this function is called, an error will be returned.
*********************************************************************************************************
*/

static  void  Serial_RdHandler (SERIAL_IF             *pif,
                                void                  *pdest,
                                CPU_SIZE_T             len,
                                SERIAL_CALLBACK_FNCT  *phandler,
                                void                  *parg,
                                CPU_BOOLEAN            async,
                                CPU_INT32U             timeout_ms,
                                SERIAL_ERR            *perr)
{
#if (SERIAL_CFG_RD_BUF_EN == DEF_ENABLED)
    CPU_SIZE_T    buf_len;
    CPU_SIZE_T    buf_rem;
    CPU_SIZE_T    buf_copy;
    SERIAL_ERR    buf_err;
    CPU_BOOLEAN   use_rd_buf;
#endif
    CPU_BOOLEAN   lock;
    CPU_SIZE_T    rd_rem;
    void         *pbuf;
    CPU_SR_ALLOC();


#if (SERIAL_CFG_ARG_CHK_EXT_EN == DEF_ENABLED)                  /* ------------------- VALIDATE ARGS ------------------ */
    if (pdest == (void *)0) {                                   /* Validate dest buf ptr.                               */
       *perr = SERIAL_ERR_NULL_PTR;
        return;
    }
    if (phandler == (void *)0) {                                /* Validate callback fnct ptr.                          */
       *perr = SERIAL_ERR_NULL_FNCT;
        return;
    }
#endif


#if 0                                                           /* Chk'd at caller.                                     */
    if (len == (CPU_SIZE_T)0) {                                 /* Chk for zero len.                                    */
       *perr = SERIAL_ERR_NONE;
        return;
    }
#endif


                                                                /* ------------------ ACQUIRE RD LOCK ----------------- */
    if (async == DEF_YES) {
        CPU_CRITICAL_ENTER();
        lock = SerialOS_SemTryLock(pif->RdSem);
        if (lock != DEF_OK) {
            CPU_CRITICAL_EXIT();
           *perr = SERIAL_ERR_IF_IN_USE;
            return;
        }

    } else {

        SerialOS_SemWait(pif->RdSem, timeout_ms, perr);
        if (*perr != SERIAL_OS_ERR_NONE) {
             return;
        }

        CPU_CRITICAL_ENTER();
    }


    rd_rem = len;                                               /* Save len.                                            */
    pbuf   = pdest;                                             /* Save dest buf ptr.                                   */

#if (SERIAL_CFG_RD_BUF_EN == DEF_ENABLED)                       /* -------------------- CHK RD BUF -------------------- */
    use_rd_buf = DEF_NO;
    buf_copy   = 0;
    buf_err    = SERIAL_ERR_NONE;

    if (pif->RdBufEn == DEF_YES) {
        buf_len  = SerialBuf_DataLen(&(pif->RdBuf));
        buf_copy = DEF_MIN(buf_len, len);
        rd_rem   = len - buf_copy;
        pdest    = (CPU_INT08U *)pdest + buf_copy;

        if (buf_copy > 0) {
            use_rd_buf = DEF_YES;
        }

        if (rd_rem > 0) {
            if (pif->RdBufErr == SERIAL_ERR_OVERFLOW) {         /* Flush rd buf if overflow.                            */
                use_rd_buf    = DEF_NO;
                pif->RdBufErr = SERIAL_ERR_NONE;
                SerialBuf_Clr(&(pif->RdBuf));

                rd_rem = len;
                pdest  = pbuf;


            } else if (pif->RdBufErr != SERIAL_ERR_NONE) {      /* Inform callback on any other err if rd buf is empty. */

                buf_rem = buf_len - buf_copy;
                if (buf_rem == 0) {
                    buf_err = pif->RdBufErr;
                    pif->RdBufErr = SERIAL_ERR_NONE;
                    rd_rem = 0;
                }
            }
        }
    }
#endif


    if (rd_rem > 0) {
                                                                /* --------------------- START RD --------------------- */
        pif->RdCallback    = phandler;                          /* Callback called upon completion.                     */
        pif->RdCallbackArg = parg;                              /* Arg     passed to callback.                          */
        pif->RdCallbackBuf = pbuf;                              /* Buf     passed to callback.                          */
#if (SERIAL_CFG_RD_BUF_EN == DEF_ENABLED)
        pif->RdCallbackLen = len - rd_rem;                      /* Buf len passed to callback.                          */
#endif
        pif->RdState       = SERIAL_RD_STATE_RD;

        Serial_RdStart(pif, pdest, rd_rem);                     /* Start rd.                                            */
    }

    CPU_CRITICAL_EXIT();

   *perr = SERIAL_ERR_NONE;

#if (SERIAL_CFG_RD_BUF_EN == DEF_ENABLED)                       /* -------------------- RD FROM BUF ------------------- */
    if (use_rd_buf == DEF_YES) {
        SerialBuf_Rd(&(pif->RdBuf), pbuf, buf_copy);

        if (rd_rem == 0) {
            SerialOS_SemSignal(pif->RdSem);

          (*phandler)(pif->Nbr,                                 /* Call callback fnct.                                  */
                      parg,
                      pbuf,
                      buf_copy,
                      buf_err);
        }
    }
#endif
}


/*$PAGE*/
/*
*********************************************************************************************************
*                                          Serial_RdStart()
*
* Description : Start read.
*
* Argument(s) : pif         Pointer to interface.
*
*               pdest       Pointer to destination buffer.
*
*               len         Number of octets to read.
*
* Return(s)   : None.
*
* Caller(s)   : Serial_Rd().
*
* Note(s)     : (1) Interrupts are assumed to be disabled when this function is called.
*********************************************************************************************************
*/

static  void  Serial_RdStart (SERIAL_IF   *pif,
                              void        *pdest,
                              CPU_SIZE_T   len)
{
    SerialBuf_Init((SERIAL_BUF  *)&(pif->RdUserBuf),            /* Init buf.                                            */
                   (CPU_INT08U  *)  pdest,
                   (CPU_SIZE_T   )  len,
                   (CPU_BOOLEAN  )  DEF_NO);

    Serial_IF_RxStart(pif);
}


/*$PAGE*/
/*
*********************************************************************************************************
*                                           Serial_RdEnd()
*
* Description : Wait for read to complete.
*
* Argument(s) : pif         Pointer to interface.
*
*               pinfo       Pointer to callback information structure.
*
*               perr        Pointer to variable that will receive the return error code from this function :
*
*                               SERIAL_ERR_NONE                 Device successfully read.
*                               SERIAL_ERR_FAIL                 Device read failed.
*                               SERIAL_ERR_TIMEOUT              Device read timed out.
*
* Return(s)   : Number of octets read.
*
* Caller(s)   : Serial_Rd().
*
* Note(s)     : None.
*********************************************************************************************************
*/

static  CPU_SIZE_T  Serial_RdEnd (SERIAL_IF             *pif,
                                  SERIAL_CALLBACK_INFO  *pinfo,
                                  SERIAL_ERR            *perr)
{
    CPU_SIZE_T  len;
    CPU_SR_ALLOC();


    SerialOS_SemWait(pinfo->SignalPtr, pinfo->Timeout, perr);
    switch (*perr) {
        case SERIAL_OS_ERR_NONE:
#if 0
             SerialOS_SemSignal(pif->RdSem);                    /* Signal'd before callback.                            */
#endif
            *perr = pinfo->Err;
             len  = pinfo->Len;
             break;


        case SERIAL_OS_ERR_TIMEOUT:
             CPU_CRITICAL_ENTER();
             len  = SerialBuf_DataLen(&(pif->RdUserBuf));
#if (SERIAL_CFG_RD_BUF_EN == DEF_ENABLED)
             len += pif->RdCallbackLen;
#endif
             Serial_IF_RxReset(pif);
             CPU_CRITICAL_EXIT();

             SerialOS_SemSignal(pif->RdSem);
            *perr = SERIAL_ERR_TIMEOUT;
             break;


        case SERIAL_OS_ERR_SIGNAL:
        default:
             CPU_CRITICAL_ENTER();
             len  = SerialBuf_DataLen(&(pif->RdUserBuf));
#if (SERIAL_CFG_RD_BUF_EN == DEF_ENABLED)
             len += pif->RdCallbackLen;
#endif
             Serial_IF_RxReset(pif);
             CPU_CRITICAL_EXIT();

             SerialOS_SemSignal(pif->RdSem);
            *perr = SERIAL_ERR_FAIL;
             break;
    }

    return (len);
}


/*$PAGE*/
/*
*********************************************************************************************************
*                                         Serial_RdCallback()
*
* Description : Internal read complete callback.
*
* Argument(s) : if_nbr      Interface number.
*               ------      Argument validated in Serial_RdAsync().
*
*               parg        Pointer to argument.
*
*               pbuf        Pointer to buffer passed to read function.
*
*               len         Number of octets read.
*
*               err         Read error.
*
* Return(s)   : None.
*
* Caller(s)   : Serial_Rd().
*
* Note(s)     : None.
*********************************************************************************************************
*/

static  void  Serial_RdCallback (SERIAL_IF_NBR   if_nbr,
                                 void           *parg,
                                 void           *pbuf,
                                 CPU_SIZE_T      len,
                                 SERIAL_ERR      err)
{
    SERIAL_CALLBACK_INFO  *rd_info;


   (void)&if_nbr;
   (void)&pbuf;

    rd_info      = (SERIAL_CALLBACK_INFO *)parg;
    rd_info->Len =  len;
    rd_info->Err =  err;

   (void)SerialOS_SemSignal(rd_info->SignalPtr);
}


/*$PAGE*/
/*
*********************************************************************************************************
*                                        Serial_WrBufInsert()
*
* Description : Insert write buffer at the end of the transmit list.
*
* Argument(s) : pif         Pointer to interface.
*
*               pbuf        Pointer to buffer descriptor.
*
*               perr        Pointer to variable that will receive the return error code from this function :
*
*                               SERIAL_ERR_NONE             Write buffer successfully queued.
*                               SERIAL_ERR_BUF_ALLOC        Buffer structure could NOT be allocated.
*
* Return(s)   : None.
*
* Caller(s)   : Serial_WrStart().
*
* Note(s)     : (1) Interrupts are assumed to be disabled when this function is called.
*********************************************************************************************************
*/
static  void  Serial_WrBufInsert (SERIAL_IF        *pif,
                                  SERIAL_BUF_DESC  *pbuf,
                                  SERIAL_ERR       *perr)
{
#if (SERIAL_CFG_TX_DESC_NBR > 0)
    SERIAL_BUF_DESC  *pbuf_new;
    SERIAL_BUF_DESC  *plist_end;
    CPU_BOOLEAN       list_empty;
    LIB_ERR           lib_err;
#endif


   (void)&pif;
   (void)&pbuf;

#if (SERIAL_CFG_TX_DESC_NBR > 0)
    pbuf_new = (SERIAL_BUF_DESC *)Mem_PoolBlkGet((MEM_POOL *)&pif->TxBufPool,
                                                 (CPU_SIZE_T) sizeof(SERIAL_BUF_DESC),
                                                 (LIB_ERR  *)&lib_err);
    if (lib_err != LIB_MEM_ERR_NONE) {
       *perr = SERIAL_ERR_BUF_ALLOC;
        return;
    }


    pbuf_new->Len         = pbuf->Len;
    pbuf_new->DataPtr     = pbuf->DataPtr;
    pbuf_new->Callback    = pbuf->Callback;                     /* Callback called upon completion.                     */
    pbuf_new->CallbackArg = pbuf->CallbackArg;                  /* Argument passed to callback.                         */
    pbuf_new->CallbackBuf = pbuf->CallbackBuf;                  /* Buf      passed to callback.                         */


    list_empty = DEF_FALSE;
    if ((pif->TxBufListStart == (SERIAL_BUF_DESC *)0) &&
        (pif->TxBufListEnd   == (SERIAL_BUF_DESC *)0)) {
        list_empty = DEF_TRUE;
    }

    plist_end     =  pif->TxBufListEnd;
    pbuf->PrevPtr =  plist_end;
    pbuf->NextPtr = (SERIAL_BUF_DESC *)0;

    if (list_empty == DEF_TRUE) {
        pif->TxBufListStart = pbuf_new;
    } else {
        plist_end->NextPtr  = pbuf_new;
    }
    pif->TxBufListEnd = pbuf_new;


   *perr = SERIAL_ERR_NONE;
#else
   *perr = SERIAL_ERR_BUF_ALLOC;
#endif
}


/*$PAGE*/
/*
*********************************************************************************************************
*                                        Serial_WrBufRemove()
*
* Description : Remove write buffer from start of the transmit list.
*
* Argument(s) : pif         Pointer to interface.
*
*               pbuf        Pointer to buffer descriptor.
*
* Return(s)   : Pointer to write buffer removed, if successful.
*
*               Pointer to NULL,                 otherwise.
*
* Caller(s)   : Serial_WrStartNext().
*
* Note(s)     : (1) Interrupts are assumed to be disabled when this function is called.
*********************************************************************************************************
*/

static  CPU_BOOLEAN  Serial_WrBufRemove (SERIAL_IF        *pif,
                                         SERIAL_BUF_DESC  *pbuf)
{
    CPU_BOOLEAN       valid;
#if (SERIAL_CFG_TX_DESC_NBR > 0)
    SERIAL_BUF_DESC  *pbuf_next;
    LIB_ERR           lib_err;
#endif


#if (SERIAL_CFG_ARG_CHK_EXT_EN == DEF_ENABLED)                  /* ------------------- VALIDATE ARGS ------------------ */
    if (pbuf == (SERIAL_BUF_DESC *)0) {
        return (DEF_NO);
    }
#endif

   (void)&pif;
   (void)&pbuf;
    valid = DEF_NO;

#if (SERIAL_CFG_TX_DESC_NBR > 0)
    pbuf_next = pif->TxBufListStart;

    if (pbuf_next == (SERIAL_BUF_DESC *)0) {
        return (valid);
    }

    if (pif->TxBufListStart == pif->TxBufListEnd) {
        pif->TxBufListStart = (SERIAL_BUF_DESC *)0;
        pif->TxBufListEnd   = (SERIAL_BUF_DESC *)0;
    } else {
        pif->TxBufListStart = pbuf_next->NextPtr;
    }

    pbuf->Len         = pbuf_next->Len;
    pbuf->DataPtr     = pbuf_next->DataPtr;
    pbuf->Callback    = pbuf_next->Callback;
    pbuf->CallbackArg = pbuf_next->CallbackArg;
    pbuf->CallbackBuf = pbuf_next->CallbackBuf;

    Mem_PoolBlkFree(&pif->TxBufPool, pbuf_next, &lib_err);
    if (lib_err != LIB_MEM_ERR_NONE)  {
        return (DEF_NO);
    }

    valid = DEF_YES;
#endif

    return (valid);
}


/*$PAGE*/
/*
*********************************************************************************************************
*                                        Serial_WrBufTxStart()
*
* Description : Initialize write buffer and transmit first octet.
*
* Argument(s) : pif         Pointer to interface.
*
*               pbuf        Pointer to buffer descriptor.
*
*               perr        Pointer to variable that will receive the return error code from this function :
*
*                               SERIAL_ERR_NONE             Write buffer successfully initialized.
*
* Return(s)   : None.
*
* Caller(s)   : Serial_WrStart(),
*               Serial_WrNext().
*
* Note(s)     : (1) Interrupts are assumed to be disabled when this function is called.
*********************************************************************************************************
*/

static  void  Serial_WrBufTxStart (SERIAL_IF        *pif,
                                   SERIAL_BUF_DESC  *pbuf,
                                   SERIAL_ERR       *perr)
{
    SERIAL_DEV  *pdev;
    CPU_INT08U   datum;


    SerialBuf_Init((SERIAL_BUF  *)&(pif->WrUserBuf),            /* Init buf.                                            */
                   (CPU_INT08U  *)  pbuf->DataPtr,
                   (CPU_SIZE_T   )  pbuf->Len,
                   (CPU_BOOLEAN  )  DEF_YES);

    pif->WrCallback    = pbuf->Callback;
    pif->WrCallbackArg = pbuf->CallbackArg;
    pif->WrCallbackBuf = pbuf->CallbackBuf;

    pdev = &pif->Dev;
    pdev->LineDrv_API->RdOctet(pdev,                            /* Get octet.                                           */
                             &(pif->WrUserBuf),
                              &datum,
                               perr);
    if (*perr != SERIAL_ERR_NONE) {
         return;
    }

    pdev->Dev_Cfg->Drv_API->TxOctet(pdev, datum, perr);         /* Tx first octet.                                      */
}


/*$PAGE*/
/*
*********************************************************************************************************
*                                          Serial_WrStart()
*
* Description : Start write operation.
*
* Argument(s) : pif         Pointer to interface.
*
*               pbuf        Pointer to buffer descriptor.
*
*               perr        Pointer to variable that will receive the return error code from this function :
*
*                               SERIAL_ERR_NONE             Device successfully written.
*                               SERIAL_ERR_IF_NOT_OPEN      Serial interface NOT open.
*
* Return(s)   : None.
*
* Caller(s)   : Serial_WrAsync().
*
* Note(s)     : (1) Interrupts are assumed to be disabled when this function is called.
*********************************************************************************************************
*/

static  void  Serial_WrStart (SERIAL_IF        *pif,
                              SERIAL_BUF_DESC  *pbuf,
                              SERIAL_ERR       *perr)
{
    SERIAL_DEV  *pdev;
    SERIAL_ERR   err;


    switch (pif->WrState) {
        case SERIAL_WR_STATE_OPENED:
             pif->WrState = SERIAL_WR_STATE_WR;
             Serial_WrBufTxStart(pif, pbuf, perr);              /* Init buf to start tx'er.                             */
             if (*perr != SERIAL_ERR_NONE) {
                 return;
             }

             pdev = &pif->Dev;
             pdev->Dev_Cfg->Drv_API->TxStart(pdev, &err);       /* Start tx'er.                                         */
             break;



        case SERIAL_WR_STATE_WR:
        case SERIAL_WR_STATE_WR_BUF:
             Serial_WrBufInsert(pif, pbuf, perr);               /* Insert buf at the end of tx list.                    */
             break;


        case SERIAL_WR_STATE_CLOSED:
            *perr = SERIAL_ERR_IF_NOT_OPEN;
             break;
    }
}


/*$PAGE*/
/*
*********************************************************************************************************
*                                           Serial_WrNext()
*
* Description : Retrieve write buffer from transmit list and start write.
*
* Argument(s) : pif         Pointer to interface.
*
* Return(s)   : None.
*
* Caller(s)   : Serial_IF_Tx().
*
* Note(s)     : (1) Interrupts are assumed to be disabled when this function is called.
*
*               (2) If write buffer is enabled, first check if there is data waiting to be transfer from
*                   the write buffer.
*********************************************************************************************************
*/

static  void  Serial_WrNext (SERIAL_IF  *pif)
{
    SERIAL_BUF_DESC   buf;
    SERIAL_DEV       *pdev;
    SERIAL_ERR        err;
    CPU_BOOLEAN       valid;
#if (SERIAL_CFG_WR_BUF_EN == DEF_ENABLED)
    SERIAL_BUF       *pbuf;
    CPU_INT08U        datum;
    CPU_BOOLEAN       read;
#endif


#if (SERIAL_CFG_WR_BUF_EN == DEF_ENABLED)
    if (pif->WrBufEn == DEF_YES) {
        pbuf = &(pif->WrBuf);
        read = SerialBuf_RdOctet(pbuf, &datum);
        if (read == DEF_YES) {
            pif->WrState = SERIAL_WR_STATE_WR_BUF;
            pdev         = &pif->Dev;
            pdev->Dev_Cfg->Drv_API->TxOctet(pdev, datum, &err); /* Tx first octet.                                      */
            return;
        }
    }
#endif

    valid = Serial_WrBufRemove(pif, &buf);
    if (valid != DEF_YES) {                                     /* If no wr buf avail, stop tx'er.                      */
        pif->WrState = SERIAL_WR_STATE_OPENED;
        pdev         = &pif->Dev;
        pdev->Dev_Cfg->Drv_API->TxStop(pdev, &err);
    } else {                                                    /* Start next wr.                                       */
        pif->WrState = SERIAL_WR_STATE_WR;
        Serial_WrBufTxStart(pif, &buf, &err);
    }
}


/*$PAGE*/
/*
*********************************************************************************************************
*                                           Serial_WrEnd()
*
* Description : Wait for write to complete.
*
* Argument(s) : pif         Pointer to interface.
*
*               pinfo       Pointer to callback information structure.
*
*               perr        Pointer to variable that will receive the return error code from this function :
*
*                               SERIAL_ERR_NONE                 Device successfully written.
*                               SERIAL_ERR_FAIL                 Device write failed.
*                               SERIAL_ERR_TIMEOUT              Device write timed out.
*
* Return(s)   : Number of octets written.
*
* Caller(s)   : Serial_Wr().
*
* Note(s)     : None.
*********************************************************************************************************
*/

static  CPU_SIZE_T  Serial_WrEnd (SERIAL_IF             *pif,
                                  SERIAL_CALLBACK_INFO  *pinfo,
                                  SERIAL_ERR            *perr)
{
    CPU_SIZE_T  len;
    CPU_SIZE_T  buf_size;
    CPU_SIZE_T  data_len;
    CPU_SR_ALLOC();


    SerialOS_SemWait(pinfo->SignalPtr, pinfo->Timeout, perr);
    switch (*perr) {
        case SERIAL_OS_ERR_NONE:
#if 0
             SerialOS_SemSignal(pif->WrSem);                    /* Signal'd before callback.                            */
#endif
            *perr = pinfo->Err;
             len  = pinfo->Len;
             break;


        case SERIAL_OS_ERR_TIMEOUT:
             CPU_CRITICAL_ENTER();
             buf_size = SerialBuf_Size(&(pif->WrUserBuf));
             data_len = SerialBuf_DataLen(&(pif->WrUserBuf));
             len      = buf_size - data_len;

             Serial_IF_WrClr(pif);                              /* Clr wr info.                                         */
             Serial_WrNext(pif);
             CPU_CRITICAL_EXIT();

             SerialOS_SemSignal(pif->WrSem);
            *perr = SERIAL_ERR_TIMEOUT;
             break;


        case SERIAL_OS_ERR_SIGNAL:
        default:
             CPU_CRITICAL_ENTER();
             buf_size = SerialBuf_Size(&(pif->WrUserBuf));
             data_len = SerialBuf_DataLen(&(pif->WrUserBuf));
             len      = buf_size - data_len;

             Serial_IF_WrClr(pif);                              /* Clr wr info.                                         */
             Serial_WrNext(pif);
             CPU_CRITICAL_EXIT();

             SerialOS_SemSignal(pif->WrSem);
            *perr = SERIAL_ERR_FAIL;
             break;
    }

    return (len);
}


/*$PAGE*/
/*
*********************************************************************************************************
*                                         Serial_WrCallback()
*
* Description : Internal write complete callback.
*
* Argument(s) : if_nbr      Interface number.
*               ------      Argument validated in Serial_WrAsync().
*
*               parg        Pointer to argument.
*
*               pbuf        Pointer to buffer passed to write function.
*
*               len         Number of octets written.
*
*               err         Write error.
*
* Return(s)   : None.
*
* Caller(s)   : Serial_Wr().
*
* Note(s)     : None.
*********************************************************************************************************
*/

static  void  Serial_WrCallback (SERIAL_IF_NBR   if_nbr,
                                 void           *parg,
                                 void           *pbuf,
                                 CPU_SIZE_T      len,
                                 SERIAL_ERR      err)
{
    SERIAL_CALLBACK_INFO  *wr_info;


   (void)&if_nbr;
   (void)&pbuf;

    wr_info      = (SERIAL_CALLBACK_INFO *)parg;
    wr_info->Len =  len;
    wr_info->Err =  err;

   (void)SerialOS_SemSignal(wr_info->SignalPtr);
}

