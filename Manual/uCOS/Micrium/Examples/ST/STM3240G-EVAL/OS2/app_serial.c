/*
*********************************************************************************************************
*                                            EXAMPLE CODE
*
*               This file is provided as an example on how to use Micrium products.
*
*               Please feel free to use any application code labeled as 'EXAMPLE CODE' in
*               your application products.  Example code may be used as is, in whole or in
*               part, or may be used as a reference only. This file can be modified as
*               required to meet the end-product requirements.
*
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*
*               You can find our product's user manual, API reference, release notes and
*               more information at https://doc.micrium.com.
*               You can contact us at www.micrium.com.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                           uC/Serial
*                                        Application Code
*
* Filename      : app_serial.c
* Version       : V1.00
* Programmer(s) : FT
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#define  APP_SERIAL_MODULE
#include <app_serial.h>


/*
*********************************************************************************************************
*                                                 ENABLE
*********************************************************************************************************
*/

#if (APP_CFG_SERIAL_EN == DEF_ENABLED)


/*
*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/

#if (APP_SERIAL_CFG_TRACE_EN == DEF_ENABLED)
static  SERIAL_IF_NBR  App_SerTraceIF_Nbr;
#endif

/*
*********************************************************************************************************
*                                          FUNCTION PROTOTYPES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                    LOCAL CONFIGURATION ERRORS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                         App_SerialInit()
*
* Description : Initialize Serial module
*
* Argument(s) : none.
*
* Caller(s)   : Application
*
* Return(s)   : none.
*
* Note(s)     : none.
*********************************************************************************************************
*/

CPU_BOOLEAN  App_SerialInit (void)
{
    SERIAL_ERR     err;
#if (APP_SERIAL_CFG_TRACE_EN == DEF_ENABLED)
    SERIAL_IF_CFG  uart_cfg;
#endif

    Serial_Init();                                             /* Initial Serial module                                */

    Serial_DevDrvAdd((CPU_CHAR       *)"USART3",               /* Add serial interface/device.                         */
                     (SERIAL_DEV_CFG *)&SerialDevCfg_STM32_USART3,
                     (CPU_SIZE_T      ) 52u,
                     (CPU_SIZE_T      ) 52u,
                     (SERIAL_ERR     *)&err);

    if (err != SERIAL_ERR_NONE) {
        return (DEF_FAIL);
    }

#if (APP_SERIAL_CFG_TRACE_EN == DEF_ENABLED)
    uart_cfg.Baudrate = SERIAL_BAUDRATE_115200;
    uart_cfg.DataBits = SERIAL_DATABITS_8;
    uart_cfg.StopBits = SERIAL_STOPBITS_1;
    uart_cfg.Parity   = SERIAL_PARITY_NONE;
    uart_cfg.FlowCtrl = SERIAL_FLOW_CTRL_NONE;


    App_SerTraceIF_Nbr = Serial_Open((CPU_CHAR      *)APP_SERIAL_CFG_TRACE_PORT_NAME,
                                     (SERIAL_IF_CFG *)&uart_cfg,
                                     (SERIAL_ERR    *)&err);
    if (err != SERIAL_ERR_NONE) {
        return (DEF_FAIL);
    }

    Serial_SetLineDrv((SERIAL_IF_NBR        ) App_SerTraceIF_Nbr,
                      (SERIAL_LINE_DRV_API *)&SerialLine_TTY ,
                      (SERIAL_ERR          *)&err);

    Serial_Wr((SERIAL_IF_NBR   )App_SerTraceIF_Nbr,
              (void           *)"\n\n",
              (CPU_SIZE_T      )2u,
              (CPU_INT32U      )0u,
              (SERIAL_ERR     *)&err);

    if (err != SERIAL_ERR_NONE) {
        return (DEF_FAIL);
    }
#endif
    return (DEF_OK);
}


/*
*********************************************************************************************************
*                                           App_SerPrintf()
*
* Description : Output  'printf()' statements
*
* Argument(s) : p_fmt       Pointer to format string.
*
* Return(s)   : The number of octets written.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

#if (APP_SERIAL_CFG_TRACE_EN == DEF_ENABLED)
void  App_SerPrintf (CPU_CHAR  *p_fmt,
                     ...)
{
    CPU_CHAR    str[80u + 1u];
    CPU_SIZE_T  len;
    SERIAL_ERR  err;
    va_list     vArgs;


    va_start(vArgs, p_fmt);

    vsprintf((char       *)str,
             (char const *)p_fmt,
                           vArgs);

    va_end(vArgs);

    len = Str_Len(str);

    Serial_Wr((SERIAL_IF_NBR   )App_SerTraceIF_Nbr,
              (void           *)&str[0u],
              (CPU_SIZE_T      )len,
              (CPU_INT32U      )0u,
              (SERIAL_ERR     *)&err);

}
#endif


/*
*********************************************************************************************************
*                                           App_SerPrintf()
*
* Description : Read a string from the serial terminal.
*
* Argument(s) : p_str       Pointer to string.
*
*               len         String length.
*
* Return(s)   : The number of octets written.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

#if (APP_SERIAL_CFG_TRACE_EN == DEF_ENABLED)
void  App_SerStrRd  (CPU_CHAR   *p_str,
                     CPU_SIZE_T  len)
{
    SERIAL_ERR  err;

    Serial_Rd((SERIAL_IF_NBR   )App_SerTraceIF_Nbr,
              (void           *)p_str,
              (CPU_SIZE_T      )len,
              (CPU_INT32U      )0,
              (SERIAL_ERR     *)&err);
}
#endif

/*
*********************************************************************************************************
*                                           App_SerStrWr()
*
* Description : Write an string in serial interface.
*
* Argument(s) : p_fmt       Pointer to format string.
*
* Return(s)   : The number of octets written.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

#if (APP_SERIAL_CFG_TRACE_EN == DEF_ENABLED)
void  App_SerStrWr (CPU_CHAR  *p_str)
{
    CPU_SIZE_T  len;
    SERIAL_ERR  err;


    if (p_str == (CPU_CHAR *)0u) {
        return;
    }

    len = Str_Len(p_str);

    Serial_Wr((SERIAL_IF_NBR   )App_SerTraceIF_Nbr,
              (void           *)p_str,
              (CPU_SIZE_T      )len,
              (CPU_INT32U      )0u,
              (SERIAL_ERR     *)&err);
}

#endif

#endif
