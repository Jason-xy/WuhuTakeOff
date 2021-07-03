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
*                                            EXAMPLE CODE
*
*                                     ST Microelectronics STM32
*                                              on the
*
*                                           STM3240G-EVAL
*                                         Evaluation Board
*
* Filename      : app.c
* Version       : V1.00
* Programmer(s) : DC
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#include  <stdarg.h>
#include  <stdio.h>
#include  <math.h>
#include  <stm32f4xx_hal.h>

#include  <cpu.h>
#include  <lib_math.h>
#include  <lib_mem.h>
#include  <os.h>

#include  <app_cfg.h>
#include  <bsp.h>

#if (APP_CFG_SERIAL_EN == DEF_ENABLED)
#include  <app_serial.h>
#endif


/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/

#define  APP_TASK_EQ_0_ITERATION_NBR              16u
#define  APP_TASK_EQ_1_ITERATION_NBR              18u


/*
*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/

                                                                /* --------------- APPLICATION GLOBALS ---------------- */
static  OS_STK       AppTaskStartStk[APP_CFG_TASK_START_STK_SIZE];

                                                                /* ------------- PROBE EXAMPLE VARIABLES -------------- */
        CPU_INT32U   AppProbe_CtrVal;
        CPU_INT32U   AppProbe_Slider;

                                                                /* --------------- SEMAPHORE TASK TEST ---------------- */
static  OS_STK       AppTaskEvent0Stk[APP_CFG_TASK_OBJ_STK_SIZE];

static  OS_STK       AppTaskEvent1Stk[APP_CFG_TASK_OBJ_STK_SIZE];
                                                                /* ------------ FLOATING POINT TEST TASK -------------- */
static  OS_STK       AppTaskEq0FpStk[APP_CFG_TASK_EQ_STK_SIZE];
static  OS_STK       AppTaskEq1FpStk[APP_CFG_TASK_EQ_STK_SIZE];

#if (OS_SEM_EN > 0u)
static  OS_EVENT    *AppTraceSem;
#endif

#if (OS_SEM_EN > 0u)
static  OS_EVENT    *AppTaskObjSem;
#endif

#if (OS_MUTEX_EN > 0u)
static  OS_EVENT    *AppTaskObjMutex;
#endif

#if (OS_Q_EN > 0u)
static  OS_EVENT    *AppTaskObjQ;
#endif

#if (OS_FLAG_EN > 0u)
static  OS_FLAG_GRP *AppTaskObjFlag;
#endif


/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/

static  void  AppTaskCreate  (void);
static  void  AppEventCreate (void);

static  void  AppTaskStart   (void  *p_arg);

static  void  AppTaskEq0Fp   (void  *p_arg);                   /* Floating Point Equation 0 task.                      */
static  void  AppTaskEq1Fp   (void  *p_arg);                   /* Floating Point Equation 1 task.                      */
static  void  AppTaskEvent0  (void  *p_arg);
static  void  AppTaskEvent1  (void  *p_arg);


/*
*********************************************************************************************************
*                                                main()
*
* Description : This is the standard entry point for C code.  It is assumed that your code will call
*               main() once you have performed all necessary initialization.
*
* Arguments   : none
*
* Returns     : none
*
* Notes       : 1) STM32F4xx HAL library initialization:
*                      a) Configures the Flash prefetch, intruction and data caches.
*                      b) Configures the Systick to generate an interrupt. However, the function ,
*                         HAL_InitTick(), that initializes the Systick has been overwritten since Micrium's
*                         RTOS has its own Systick initialization and it is recommended to initialize the
*                         Systick after multitasking has started.
*
*********************************************************************************************************
*/

int main(void)
{
#if (OS_TASK_NAME_EN > 0)
    CPU_INT08U  err;
#endif

    HAL_Init();                                                 /* See Note 1.                                          */

    Mem_Init();                                                 /* Initialize Memory Managment Module                   */
    Math_Init();                                                /* Initialize Mathematical Module                       */

    BSP_IntDisAll();                                            /* Disable all Interrupts.                              */

    OSInit();                                                   /* Init uC/OS-II.                                       */

    OSTaskCreateExt( AppTaskStart,                              /* Create the start task                                */
                     0,
                    &AppTaskStartStk[APP_CFG_TASK_START_STK_SIZE - 1],
                     APP_CFG_TASK_START_PRIO,
                     APP_CFG_TASK_START_PRIO,
                    &AppTaskStartStk[0],
                     APP_CFG_TASK_START_STK_SIZE,
                     0,
                    (OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));

#if (OS_TASK_NAME_EN > 0)
    OSTaskNameSet(         APP_CFG_TASK_START_PRIO,
                  (INT8U *)"Start Task",
                           &err);
#endif

    OSStart();                                                  /* Start multitasking (i.e. give control to uC/OS-II).  */

    while (DEF_ON) {                                            /* Should Never Get Here.                               */
        ;
    }
}


/*
*********************************************************************************************************
*                                          STARTUP TASK
*
* Description : This is an example of a startup task.  As mentioned in the book's text, you MUST
*               initialize the ticker only once multitasking has started.
*
* Arguments   : p_arg   is the argument passed to 'AppTaskStart()' by 'OSTaskCreate()'.
*
* Returns     : none
*
* Notes       : 1) The first line of code is used to prevent a compiler warning because 'p_arg' is not
*                  used.  The compiler should not generate any code for this statement.
*********************************************************************************************************
*/

static  void  AppTaskStart (void *p_arg)
{
    CPU_INT32U  r0;
    CPU_INT32U  r1;
    CPU_INT32U  r2;
    CPU_INT32U  r3;
    CPU_INT32U  r4;
    CPU_INT32U  r5;
    CPU_INT32U  r6;
    CPU_INT32U  r7;
    CPU_INT32U  r8;
    CPU_INT32U  r9;
    CPU_INT32U  r10;
    CPU_INT32U  r11;
    CPU_INT32U  r12;


   (void)p_arg;

    r0  =  0u;                                                  /* Initialize local variables.                          */
    r1  =  1u;
    r2  =  2u;
    r3  =  3u;
    r4  =  4u;
    r5  =  5u;
    r6  =  6u;
    r7  =  7u;
    r8  =  8u;
    r9  =  9u;
    r10 = 10u;
    r11 = 11u;
    r12 = 12u;

    BSP_Init();                                                 /* Initialize BSP functions                             */
    CPU_Init();                                                 /* Initialize the uC/CPU services                       */

#if (OS_TASK_STAT_EN > 0)
    OSStatInit();                                               /* Determine CPU capacity                               */
#endif

#ifdef CPU_CFG_INT_DIS_MEAS_EN
    CPU_IntDisMeasMaxCurReset();
#endif

#if (APP_CFG_SERIAL_EN == DEF_ENABLED)
    App_SerialInit();                                           /* Initialize Serial Communication for Application ...  */
#endif

    APP_TRACE_DBG(("Creating Application Events\n\r"));
    AppEventCreate();                                           /* Create Applicaiton kernel objects                    */

    APP_TRACE_DBG(("Creating Application Tasks\n\r"));
    AppTaskCreate();                                            /* Create Application tasks                             */

    BSP_LED_Off(0u);
    AppProbe_CtrVal = 0u;
    AppProbe_Slider = 200u;

    while (DEF_TRUE) {                                          /* Task body, always written as an infinite loop.       */
        BSP_LED_Toggle(0u);
        OSTimeDlyHMSM(0u, 0u, 0u, (AppProbe_Slider));

        if ((r0  !=  0u) ||                                     /* Check task context.                                  */
            (r1  !=  1u) ||
            (r2  !=  2u) ||
            (r3  !=  3u) ||
            (r4  !=  4u) ||
            (r5  !=  5u) ||
            (r6  !=  6u) ||
            (r7  !=  7u) ||
            (r8  !=  8u) ||
            (r9  !=  9u) ||
            (r10 != 10u) ||
            (r11 != 11u) ||
            (r12 != 12u)) {
           APP_TRACE_INFO(("Context Error\n"));
        }

        AppProbe_CtrVal++;

        if (AppProbe_CtrVal >= DEF_INT_32_MASK) {
            AppProbe_CtrVal = 0u;
        }
    }
}

/*
*********************************************************************************************************
*                                          AppTaskCreate()
*
* Description : Create Application Tasks.
*
* Argument(s) : none
*
* Return(s)   : none
*
* Caller(s)   : AppTaskStart()
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  void  AppTaskCreate (void)
{
#if (OS_TASK_NAME_EN > 0)
    CPU_INT08U  err;
#endif

                                                                /* ----------- CREATE KERNEL EVENT TEST TASK ---------- */
    OSTaskCreateExt( AppTaskEvent0,
                     0,
                    &AppTaskEvent0Stk[APP_CFG_TASK_OBJ_STK_SIZE - 1],
                     APP_CFG_TASK_EVENT0_PRIO,
                     APP_CFG_TASK_EVENT0_PRIO,
                    &AppTaskEvent0Stk[0],
                     APP_CFG_TASK_OBJ_STK_SIZE,
                     0,
                    (OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));

#if (OS_TASK_NAME_EN > 0)
    OSTaskNameSet(         APP_CFG_TASK_EVENT0_PRIO,
                  (INT8U *)"Kernel Events Task 0",
                           &err);
#endif

    OSTaskCreateExt( AppTaskEvent1,
                     0,
                    &AppTaskEvent1Stk[APP_CFG_TASK_OBJ_STK_SIZE - 1],
                     APP_CFG_TASK_EVENT1_PRIO,
                     APP_CFG_TASK_EVENT1_PRIO,
                    &AppTaskEvent1Stk[0],
                     APP_CFG_TASK_OBJ_STK_SIZE,
                     0,
                    (OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));

#if (OS_TASK_NAME_EN > 0)
    OSTaskNameSet(         APP_CFG_TASK_EVENT1_PRIO,
                  (INT8U *)"Kernel Events Task 1",
                           &err);
#endif

                                                                /* ------------- CREATE FLOATING POINT TASK ----------- */
    OSTaskCreateExt( AppTaskEq0Fp,
                     0,
                    &AppTaskEq0FpStk[APP_CFG_TASK_EQ_STK_SIZE - 1],
                     APP_CFG_TASK_EQ0_PRIO,
                     APP_CFG_TASK_EQ0_PRIO,
                    &AppTaskEq0FpStk[0],
                     APP_CFG_TASK_EQ_STK_SIZE,
                     0,
                    (OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR | OS_TASK_OPT_SAVE_FP));

#if (OS_TASK_NAME_EN > 0)
    OSTaskNameSet(         APP_CFG_TASK_EQ0_PRIO,
                  (INT8U *)"FP  Equation 0",
                           &err);
#endif

    OSTaskCreateExt( AppTaskEq1Fp,
                     0,
                    &AppTaskEq1FpStk[APP_CFG_TASK_EQ_STK_SIZE - 1],
                     APP_CFG_TASK_EQ1_PRIO,
                     APP_CFG_TASK_EQ1_PRIO,
                    &AppTaskEq1FpStk[0],
                     APP_CFG_TASK_EQ_STK_SIZE,
                     0,
                    (OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR | OS_TASK_OPT_SAVE_FP));

#if (OS_TASK_NAME_EN > 0)
    OSTaskNameSet(         APP_CFG_TASK_EQ1_PRIO,
                  (INT8U *)"FP  Equation 1",
                           &err);
#endif
}


/*
*********************************************************************************************************
*                                           AppEventCreate()
*
* Description : Create application kernel event tasks.
*
* Argument(s) : none
*
* Return(s)   : none
*
* Caller(s)   : AppTaskStart()
*
* Note(s)     : none.
*********************************************************************************************************
*/
static  void  AppEventCreate (void)
{
    CPU_INT08U  os_err;

#if (OS_SEM_EN > 0u)
    AppTaskObjSem = OSSemCreate(0);
    AppTraceSem   = OSSemCreate(1);

#if (OS_EVENT_NAME_EN > 0)
    OSEventNameSet(         AppTaskObjSem,
                   (INT8U *)"Trace Lock",
                            &os_err);

    OSEventNameSet(         AppTraceSem,
                   (INT8U *)"Sem Test",
                            &os_err);
#endif
#endif

#if (OS_MUTEX_EN > 0u)
    AppTaskObjMutex = OSMutexCreate( APP_CFG_MUTEX_PRIO,
                                     &os_err);

#if (OS_EVENT_NAME_EN > 0)
    OSEventNameSet(         AppTaskObjMutex,
                   (INT8U *)"Mutex Test",
                            &os_err);
#endif
#endif

#if (OS_Q_EN > 0u)
    AppTaskObjQ = OSQCreate((void *)&AppTaskObjQ,
                                      1);

#if (OS_EVENT_NAME_EN > 0)
    OSEventNameSet(          AppTaskObjQ,
                   (INT8U *)"Queue Test",
                            &os_err);
#endif
#endif

#if (OS_FLAG_EN > 0u)
    AppTaskObjFlag = OSFlagCreate( DEF_BIT_NONE,
                                   &os_err);

#if (OS_EVENT_NAME_EN > 0)
    OSFlagNameSet(         AppTaskObjFlag,
                  (INT8U *)"Flag Test",
                           &os_err);
#endif
#endif
}


/*
*********************************************************************************************************
*                                          AppTaskEvent0()
*
* Description : Test uC/OS-II Events.
*
* Argument(s) : p_arg is the argument passed to 'AppTaskEvent0' by 'OSTaskCreate()'.
*
* Return(s)   : none
*
* Caller(s)   : This is a task
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  void  AppTaskEvent0 (void  *p_arg)
{
    CPU_INT08U  os_err;

    (void)p_arg;


    while (DEF_TRUE) {
#if (OS_SEM_EN > 0u)
        os_err = OSSemPost(AppTaskObjSem);
#endif

#if (OS_MUTEX_EN > 0u)
        OSMutexPend( AppTaskObjMutex,
                     0,
                    &os_err);

        OSTimeDlyHMSM(0u, 0u, 0u, 100u);

        os_err = OSMutexPost(AppTaskObjMutex);
#endif

#if (OS_Q_EN > 0u)
        os_err = OSQPost(        AppTaskObjQ,
                         (void *)1u);
#endif

#if (OS_FLAG_EN > 0u)
        OSFlagPost( AppTaskObjFlag,
                    DEF_BIT_00,
                    OS_FLAG_SET,
                   &os_err);
#endif
        OSTimeDlyHMSM(0u, 0u, 0u, 10u);
        APP_TRACE_INFO(("Event test task 0 running ....\n"));
    }
}


/*
*********************************************************************************************************
*                                          AppTaskEvent1()
*
* Description : Test uC/OS-II events.
*
* Argument(s) : p_arg is the argument passed to 'AppTaskEvent1' by 'OSTaskCreate()'.
*
* Return(s)   : none
*
* Caller(s)   : This is a task
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  void  AppTaskEvent1 (void  *p_arg)
{
    CPU_INT08U  os_err;


    (void)p_arg;

    while (DEF_TRUE) {

#if (OS_SEM_EN > 0u)
        OSSemPend( AppTaskObjSem,
                   0,
                  &os_err);
#endif

#if (OS_MUTEX_EN > 0u)
        OSMutexPend( AppTaskObjMutex,
                     0,
                    &os_err);

        os_err = OSMutexPost(AppTaskObjMutex);
#endif

#if (OS_Q_EN > 0u)
        OSQPend( AppTaskObjQ,
                 0,
                &os_err);
#endif

#if (OS_FLAG_EN > 0u)
        OSFlagPend( AppTaskObjFlag,
                    DEF_BIT_00,
                    OS_FLAG_WAIT_SET_ALL + OS_FLAG_CONSUME,
                    0,
                   &os_err);
#endif
        OSTimeDlyHMSM(0u, 0u, 0u, 10u);
        APP_TRACE_INFO(("Event test task 1 running ....\n"));
    }
}


/*
*********************************************************************************************************
*                                             AppTaskEq0Fp()
*
* Description : This task finds the root of the following equation.
*               f(x) =  e^-x(3.2 sin(x) - 0.5 cos(x)) using the bisection mehtod
*
* Argument(s) : p_arg   is the argument passed to 'AppTaskEq0Fp' by 'OSTaskCreate()'.
*
* Return(s)   : none.
*
* Note(s)     : 1) Atollic TrueSTUDIO needs special settings to output floating point format; however,
*                  IAR and KeilMDK should work properly.
*********************************************************************************************************
*/

void  AppTaskEq0Fp (void  *p_arg)
{
    CPU_FP32    a;
    CPU_FP32    b;
    CPU_FP32    c;
    CPU_FP32    eps;
    CPU_FP32    f_a;
    CPU_FP32    f_c;
    CPU_FP32    delta;
    CPU_INT08U  iteration;
    RAND_NBR    wait_cycles;


    while (DEF_TRUE) {
        eps       = 0.00001;
        a         = 3.0;
        b         = 4.0;
        delta     = a - b;
        iteration = 0u;
        if (delta < 0) {
            delta = delta * -1.0;
        }

        while (((2.00 * eps) < delta) ||
               (iteration    > 20u  )) {
            c   = (a + b) / 2.00;
            f_a = (exp((-1.0) * a) * (3.2 * sin(a) - 0.5 * cos(a)));
            f_c = (exp((-1.0) * c) * (3.2 * sin(c) - 0.5 * cos(c)));

            if (((f_a > 0.0) && (f_c < 0.0)) ||
                ((f_a < 0.0) && (f_c > 0.0))) {
                b = c;
            } else if (((f_a > 0.0) && (f_c > 0.0)) ||
                       ((f_a < 0.0) && (f_c < 0.0))) {
                a = c;
            } else {
                break;
            }

            delta = a - b;
            if (delta < 0) {
               delta = delta * -1.0;
            }
            iteration++;

            wait_cycles = Math_Rand();
            wait_cycles = wait_cycles % 1000;

            while (wait_cycles > 0u) {
                wait_cycles--;
            }

            if (iteration > APP_TASK_EQ_0_ITERATION_NBR) {
                APP_TRACE_INFO(("AppTaskEq0Fp() max # iteration reached\n"));
                break;
            }
        }

        APP_TRACE_INFO(("Eq0 Task Running ....\n"));

        if (iteration == APP_TASK_EQ_0_ITERATION_NBR) {
#if 0                                                           /* See Note 1.                                         */
            APP_TRACE_INFO(("Root = %f; f(c) = %f; #iterations : %d\n", c, f_c, iteration));
#else
            APP_TRACE_INFO(("Eq0 task; #iterations : %d\n", iteration));
#endif
        }
    }
}



/*
*********************************************************************************************************
*                                             AppTaskEq1Fp()
*
* Description : This task finds the root of the following equation.
*               f(x) = x^2 -3 using the bisection mehtod
*
* Argument(s) : p_arg   is the argument passed to 'AppTaskEq()' by 'OSTaskCreate()'.
*
* Return(s)   : none.
*
* Note(s)     : 1) Atollic TrueSTUDIO needs special settings to properly output floating point format;
*                  however, IAR and KeilMDK should work properly.
*********************************************************************************************************
*/

void  AppTaskEq1Fp (void  *p_arg)
{
    CPU_FP32    a;
    CPU_FP32    b;
    CPU_FP32    c;
    CPU_FP32    eps;
    CPU_FP32    f_a;
    CPU_FP32    f_c;
    CPU_FP32    delta;
    CPU_INT08U  iteration;
    RAND_NBR    wait_cycles;


    while (DEF_TRUE) {
        eps       = 0.00001;
        a         = 1.0;
        b         = 4.0;
        delta     = a - b;
        iteration = 0u;

        if (delta < 0) {
            delta = delta * -1.0;
        }

        while ((2.00 * eps) < delta) {
            c   = (a + b) / 2.0;
            f_a = a * a - 3.0;
            f_c = c * c - 3.0;

            if (((f_a > 0.0) && (f_c < 0.0)) ||
                ((f_a < 0.0) && (f_c > 0.0))) {
                b = c;
            } else if (((f_a > 0.0) && (f_c > 0.0)) ||
                       ((f_a < 0.0) && (f_c < 0.0))) {
                a = c;
            } else {
                break;
            }

            delta = a - b;
            if (delta < 0) {
               delta = delta * -1.0;
            }
            iteration++;

            wait_cycles = Math_Rand();
            wait_cycles = wait_cycles % 1000;

            while (wait_cycles > 0u) {
                wait_cycles--;
            }

            if (iteration > APP_TASK_EQ_1_ITERATION_NBR) {
                APP_TRACE_INFO(("AppTaskEq1Fp() max # iteration reached\n"));
                break;
            }
        }

        APP_TRACE_INFO(("Eq1 Task Running ....\n"));

        if (iteration == APP_TASK_EQ_1_ITERATION_NBR) {
#if 0                                                           /* See Note 1.                                         */
            APP_TRACE_INFO(("Root = %f; f(c) = %f; #iterations : %d\n", c, f_c, iteration));
#else
            APP_TRACE_INFO(("Eq1 task; #iterations : %d\n", iteration));
#endif
        }
    }
}


/*
*********************************************************************************************************
*                                             App_Trace()
*
* Description : Thread-safe version of printf
*
* Argument(s) : Format string follwing the C format convention..
*
* Return(s)   : none.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  App_Trace (CPU_CHAR *format, ...)
{
    CPU_CHAR  buf_str[80 + 1];
    va_list   v_args;
    CPU_INT08U  os_err;


    va_start(v_args, format);
   (void)vsnprintf((char       *)&buf_str[0],
                   (size_t      ) sizeof(buf_str),
                   (char const *) format,
                                  v_args);
    va_end(v_args);

    OSSemPend((OS_EVENT  *)AppTraceSem,
              (INT32U     )0,
              (INT8U     *)&os_err);

    printf("%s", buf_str);

    os_err = OSSemPost((OS_EVENT *)AppTraceSem);
}
