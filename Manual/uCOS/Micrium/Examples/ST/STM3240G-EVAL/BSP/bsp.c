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
*                                    MICRIUM BOARD SUPPORT PACKAGE
*
*                                     ST Microelectronics STM32
*                                              on the
*
*                                         STM3240G-EVAL
*                                        Evaluation Board
*
* Filename      : bsp.c
* Version       : V1.00
* Programmer(s) : FF
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#define   BSP_MODULE
#include  <bsp.h>
#include  <bsp_os.h>

#include  <stm32f4xx_hal.h>


/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/

#define  BSP_BIT_RCC_PLLCFGR_PLLM               25u
#define  BSP_BIT_RCC_PLLCFGR_PLLN              336u
#define  BSP_BIT_RCC_PLLCFGR_PLLP                2u
#define  BSP_BIT_RCC_PLLCFGR_PLLQ                7u


#define  BSP_GPIOG_LED1                        DEF_BIT_06
#define  BSP_GPIOG_LED2                        DEF_BIT_08
#define  BSP_GPIOI_LED3                        DEF_BIT_09
#define  BSP_GPIOC_LED4                        DEF_BIT_07

/*
*********************************************************************************************************
*                                           LOCAL CONSTANTS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                          LOCAL DATA TYPES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            LOCAL TABLES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                             REGISTERS
*********************************************************************************************************
*/

#define  BSP_REG_DEM_CR                       (*(CPU_REG32 *)0xE000EDFC)
#define  BSP_REG_DWT_CR                       (*(CPU_REG32 *)0xE0001000)
#define  BSP_REG_DWT_CYCCNT                   (*(CPU_REG32 *)0xE0001004)
#define  BSP_REG_DBGMCU_CR                    (*(CPU_REG32 *)0xE0042004)

/*
*********************************************************************************************************
*                                            REGISTER BITS
*********************************************************************************************************
*/

#define  BSP_DBGMCU_CR_TRACE_IOEN_MASK                   0x10
#define  BSP_DBGMCU_CR_TRACE_MODE_ASYNC                  0x00
#define  BSP_DBGMCU_CR_TRACE_MODE_SYNC_01                0x40
#define  BSP_DBGMCU_CR_TRACE_MODE_SYNC_02                0x80
#define  BSP_DBGMCU_CR_TRACE_MODE_SYNC_04                0xC0
#define  BSP_DBGMCU_CR_TRACE_MODE_MASK                   0xC0

#define  BSP_BIT_DEM_CR_TRCENA                    DEF_BIT_24

#define  BSP_BIT_DWT_CR_CYCCNTENA                 DEF_BIT_00

/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/

static  void  BSP_LED_Init        (void);


/*
*********************************************************************************************************
*                                               BSP_Init()
*
* Description : Initialize the Board Support Package (BSP).
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : (1) This function SHOULD be called before any other BSP function is called.
*
*               (2) CPU instruction / data tracing requires the use of the following pins :
*                   (a) (1) Aysynchronous     :  PB[3]
*                       (2) Synchronous 1-bit :  PE[3:2]
*                       (3) Synchronous 2-bit :  PE[4:2]
*                       (4) Synchronous 4-bit :  PE[6:2]
*
*                   (c) The application may wish to adjust the trace bus width depending on I/O
*                       requirements.
*               (3) The voltage scaling allows optimizing the power consumption when the device is
*                   clocked below the maximum system frequency, to update the voltage scaling value
*                   regarding system frequency refer to product datasheet.
*********************************************************************************************************
*/

void  BSP_Init (void)
{
    RCC_OscInitTypeDef  RCC_OscInitStruct;
    RCC_ClkInitTypeDef  RCC_ClkInitStruct;

    BSP_IntInit();

    HAL_RCC_DeInit();

    __HAL_RCC_PWR_CLK_ENABLE();                                 /* Enable Power Control clock.                          */
                                                                /* See Note 3.                                          */
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

                                                                /* PLLCLK    = HSE * (PLLN / PLLM)      = 336MHz.       */
                                                                /* SYSCLK    = PLLCLK / PLLP            = 168MHz.       */
                                                                /* OTG_FSCLK = PLLCLK / PLLQ            =  48MHz.       */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState       = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState   = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource  = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM       = BSP_BIT_RCC_PLLCFGR_PLLM;
    RCC_OscInitStruct.PLL.PLLN       = BSP_BIT_RCC_PLLCFGR_PLLN;
    RCC_OscInitStruct.PLL.PLLP       = BSP_BIT_RCC_PLLCFGR_PLLP;
    RCC_OscInitStruct.PLL.PLLQ       = BSP_BIT_RCC_PLLCFGR_PLLQ;
    HAL_RCC_OscConfig(&RCC_OscInitStruct);


    RCC_ClkInitStruct.ClockType      = RCC_CLOCKTYPE_SYSCLK |
                                       RCC_CLOCKTYPE_HCLK   |
                                       RCC_CLOCKTYPE_PCLK1  |
                                       RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;          /* HCLK    = AHBCLK  = PLL / AHBPRES(1) = 168MHz.       */
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;           /* APB1CLK = AHBCLK  / APB1DIV(4)       = 42MHz (max).  */
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;           /* APB2CLK = AHBCLK  / APB2DIV(2)       = 84MHz.        */
    HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);

                                                                /* STM32F405x/407x/415x/417x Revision Z devices: ...... */
    if (HAL_GetREVID() == 0x1001)                               /* ....prefetch is supported                            */
    {
      __HAL_FLASH_PREFETCH_BUFFER_ENABLE();                     /* Enable the Flash prefetch                            */
    }

    BSP_LED_Init();                                             /* Init LEDs.                                           */

#ifdef TRACE_EN                                                 /* See project / compiler preprocessor options.         */
    BSP_CPU_REG_DBGMCU_CR |=  BSP_DBGMCU_CR_TRACE_IOEN_MASK;    /* Enable tracing (see Note #2).                        */
    BSP_CPU_REG_DBGMCU_CR &= ~BSP_DBGMCU_CR_TRACE_MODE_MASK;    /* Clr trace mode sel bits.                             */
    BSP_CPU_REG_DBGMCU_CR |=  BSP_DBGMCU_CR_TRACE_MODE_SYNC_04; /* Cfg trace mode to synch 4-bit.                       */
#endif
}


/*
*********************************************************************************************************
*                                            BSP_CPU_ClkFreq()
*
* Description : Read CPU registers to determine the CPU clock frequency of the chip.
*
* Argument(s) : none.
*
* Return(s)   : The CPU clock frequency, in Hz.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

CPU_INT32U  BSP_CPU_ClkFreq (void)
{
    CPU_INT32U  hclk_freq;


    hclk_freq = HAL_RCC_GetHCLKFreq();
    return (hclk_freq);
}


/*
*********************************************************************************************************
*                                            HAL_InitTick()
*
* Description : This function has been overwritten from the STM32F4xx HAL libraries because Micrium's RTOS
*               has its own Systick initialization and because it is recomended to initialize the tick after
*               multi-tasking has started.
*
* Argument(s) : TickPriority          Tick interrupt priority.
*
* Return(s)   : HAL_OK.
*
* Caller(s)   : HAL_InitTick ()) is called automatically at the beginning of the program after reset by
*               HAL_Init() or at any time when clock is configured, by HAL_RCC_ClockConfig().
*
* Note(s)     : none.
*********************************************************************************************************
*/

HAL_StatusTypeDef HAL_InitTick(uint32_t TickPriority)
{
    HAL_NVIC_SetPriorityGrouping(0);

    if (OSRunning > 0u) {                                       /*Check if multi-tasking has started.                   */
        BSP_Tick_Init();
    }

    return (HAL_OK);
}


/*
*********************************************************************************************************
*                                            BSP_Tick_Init()
*
* Description : Initialize all the peripherals that required OS Tick services (OS initialized)
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/
void BSP_Tick_Init (void)
{
    CPU_INT32U  cpu_clk_freq;
    CPU_INT32U  cnts;

    cpu_clk_freq = BSP_CPU_ClkFreq();                           /* Determine SysTick reference freq.                    */

#if (OS_VERSION >= 30000u)
    cnts  = cpu_clk_freq / (CPU_INT32U)OSCfg_TickRate_Hz;       /* Determine nbr SysTick increments.                    */
#else
    cnts  = cpu_clk_freq / (CPU_INT32U)OS_TICKS_PER_SEC;        /* Determine nbr SysTick increments.                    */
#endif

    OS_CPU_SysTickInit(cnts);                                   /* Init uC/OS periodic time src (SysTick).              */
}


/*
*********************************************************************************************************
*                                           BSP_LED_Init()
*
* Description : Initialize any or all the LEDs on the board.
*
* Argument(s) : led     The ID of the LED to control:
*
*                       0    inialize ALL  LEDs
*                       1    inialize user LED1
*                       2    inialize user LED2
*                       3    inialize user LED3
*                       4    inialize user LED4
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

static void  BSP_LED_Init()
{
    GPIO_InitTypeDef  gpio_init;


    BSP_PeriphEn(BSP_PERIPH_ID_GPIOG);                          /* Configure GPIOG for LED1 and LED2                    */

    gpio_init.Pin   = BSP_GPIOG_LED1 | BSP_GPIOG_LED2;
    gpio_init.Pin   = BSP_GPIOG_LED1 | BSP_GPIOG_LED2;
    gpio_init.Mode  = GPIO_MODE_OUTPUT_PP;
    gpio_init.Pull  = GPIO_PULLUP;
    gpio_init.Speed = GPIO_SPEED_HIGH;

    HAL_GPIO_Init(GPIOG, &gpio_init);

    BSP_PeriphEn(BSP_PERIPH_ID_GPIOI);                          /* Configure GPIOI for LED3                             */

    gpio_init.Pin = BSP_GPIOI_LED3;
    HAL_GPIO_Init(GPIOI, &gpio_init);


    BSP_PeriphEn(BSP_PERIPH_ID_GPIOC);                          /* Configure GPIOC for LED4                             */

    gpio_init.Pin = BSP_GPIOC_LED4;
    HAL_GPIO_Init(GPIOC, &gpio_init);
}


/*
*********************************************************************************************************
*                                             BSP_LED_On()
*
* Description : Turn ON any or all the LEDs on the board.
*
* Argument(s) : led     The ID of the LED to control:
*
*                       0    turns ON ALL  LEDs
*                       1    turns ON user LED1
*                       2    turns ON user LED2
*                       3    turns ON user LED3
*                       4    turns ON user LED4
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  BSP_LED_On (CPU_INT08U  led)
{
    switch (led) {
        case 0u:
             HAL_GPIO_WritePin(GPIOG, (BSP_GPIOG_LED1 | BSP_GPIOG_LED2), GPIO_PIN_SET);
             HAL_GPIO_WritePin(GPIOI, BSP_GPIOI_LED3, GPIO_PIN_SET);
             HAL_GPIO_WritePin(GPIOC, BSP_GPIOC_LED4, GPIO_PIN_SET);
             break;


        case 1u:
             HAL_GPIO_WritePin(GPIOG, BSP_GPIOG_LED1, GPIO_PIN_SET);
             break;


        case 2u:
             HAL_GPIO_WritePin(GPIOG, BSP_GPIOG_LED2, GPIO_PIN_SET);
             break;


        case 3u:
             HAL_GPIO_WritePin(GPIOI, BSP_GPIOI_LED3, GPIO_PIN_SET);
             break;


        case 4u:
             HAL_GPIO_WritePin(GPIOC, BSP_GPIOC_LED4, GPIO_PIN_SET);
             break;


        default:
             break;
    }
}


/*
*********************************************************************************************************
*                                              BSP_LED_Off()
*
* Description : Turn OFF any or all the LEDs on the board.
*
* Argument(s) : led     The ID of the LED to control:
*
*                       0    turns OFF ALL the LEDs
*                       1    turns OFF user LED1
*                       2    turns OFF user LED2
*                       3    turns OFF user LED3
*                       4    turns OFF user LED4
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  BSP_LED_Off (CPU_INT08U led)
{
    switch (led) {
        case 0u:
             HAL_GPIO_WritePin(GPIOG, (BSP_GPIOG_LED1 | BSP_GPIOG_LED2), GPIO_PIN_RESET);
             HAL_GPIO_WritePin(GPIOI, BSP_GPIOI_LED3, GPIO_PIN_RESET);
             HAL_GPIO_WritePin(GPIOC, BSP_GPIOC_LED4, GPIO_PIN_RESET);
             break;


        case 1u:
             HAL_GPIO_WritePin(GPIOG, BSP_GPIOG_LED1, GPIO_PIN_RESET);
             break;


        case 2u:
             HAL_GPIO_WritePin(GPIOG, BSP_GPIOG_LED2, GPIO_PIN_RESET);
             break;


        case 3u:
             HAL_GPIO_WritePin(GPIOI, BSP_GPIOI_LED3, GPIO_PIN_RESET);
             break;


        case 4u:
             HAL_GPIO_WritePin(GPIOC, BSP_GPIOC_LED4, GPIO_PIN_RESET);
             break;


        default:
             break;
    }
}


/*
*********************************************************************************************************
*                                            BSP_LED_Toggle()
*
* Description : TOGGLE any or all the LEDs on the board.
*
* Argument(s) : led     The ID of the LED to control:
*
*                       0    TOGGLE ALL the LEDs
*                       1    TOGGLE user LED1
*                       2    TOGGLE user LED2
*                       3    TOGGLE user LED3
*                       4    TOGGLE user LED4
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  BSP_LED_Toggle (CPU_INT08U  led)
{
    switch (led) {
        case 0u:
             HAL_GPIO_TogglePin(GPIOG,(BSP_GPIOG_LED1 | BSP_GPIOG_LED2));
             HAL_GPIO_TogglePin(GPIOI, BSP_GPIOI_LED3);
             HAL_GPIO_TogglePin(GPIOC, BSP_GPIOC_LED4);
             break;


        case 1u:
             HAL_GPIO_TogglePin(GPIOG,BSP_GPIOG_LED1);
             break;


        case 2u:
             HAL_GPIO_TogglePin(GPIOG, BSP_GPIOG_LED2);
             break;


        case 3u:
             HAL_GPIO_TogglePin(GPIOI, BSP_GPIOI_LED3);
             break;


        case 4u:
             HAL_GPIO_TogglePin(GPIOC, BSP_GPIOC_LED4);
             break;


        default:
             break;
    }
}
