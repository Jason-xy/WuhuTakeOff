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
*                                          STM3240G-EVAL
*                                        Evaluation Board
*
* Filename      : bsp_periph.c
* Version       : V1.00
* Programmer(s) : EHS
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#define  BSP_PERIPH_MODULE
#include <bsp.h>


/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/

#define  BSP_PERIPH_REG_RCC_BASE                  0x40023800u
#define  BSP_PERIPH_REG_RCC_CFGR                  (*(CPU_REG32 *)(BSP_PERIPH_REG_RCC_BASE + 0x008u))
#define  BSP_PERIPH_REG_RCC_AHB1ENR               (*(CPU_REG32 *)(BSP_PERIPH_REG_RCC_BASE + 0x030u))
#define  BSP_PERIPH_REG_RCC_AHB2ENR               (*(CPU_REG32 *)(BSP_PERIPH_REG_RCC_BASE + 0x034u))
#define  BSP_PERIPH_REG_RCC_AHB3ENR               (*(CPU_REG32 *)(BSP_PERIPH_REG_RCC_BASE + 0x038u))
#define  BSP_PERIPH_REG_RCC_APB1ENR               (*(CPU_REG32 *)(BSP_PERIPH_REG_RCC_BASE + 0x040u))
#define  BSP_PERIPH_REG_RCC_APB2ENR               (*(CPU_REG32 *)(BSP_PERIPH_REG_RCC_BASE + 0x044u))


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
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                     LOCAL CONFIGURATION ERRORS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                         BSP_PeriphClkFreqGet()
*
* Description : Get clock frequency of a peripheral.
*
* Argument(s) : pwr_clk_id      Power/clock ID.
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

CPU_INT32U  BSP_PeriphClkFreqGet (CPU_DATA  pwr_clk_id)
{
    CPU_INT32U  clk_freq;
    CPU_INT32U  clk_div;


    clk_freq = BSP_CPU_ClkFreq();

    switch (pwr_clk_id) {
        case BSP_PERIPH_ID_GPIOA:
        case BSP_PERIPH_ID_GPIOB:
        case BSP_PERIPH_ID_GPIOC:
        case BSP_PERIPH_ID_GPIOD:
        case BSP_PERIPH_ID_GPIOE:
        case BSP_PERIPH_ID_GPIOF:
        case BSP_PERIPH_ID_GPIOG:
        case BSP_PERIPH_ID_GPIOH:
        case BSP_PERIPH_ID_GPIOI:
        case BSP_PERIPH_ID_CRC:
        case BSP_PERIPH_ID_BKPSRAM:
        case BSP_PERIPH_ID_CCMDATARAM:
        case BSP_PERIPH_ID_DMA1:
        case BSP_PERIPH_ID_DMA2:
        case BSP_PERIPH_ID_ETHMAC:
        case BSP_PERIPH_ID_ETHMACPTP:
        case BSP_PERIPH_ID_DCMI:
        case BSP_PERIPH_ID_CRYP:
        case BSP_PERIPH_ID_HASH:
        case BSP_PERIPH_ID_RNG:
        case BSP_PERIPH_ID_FSMC:
             return (clk_freq);

        case BSP_PERIPH_ID_TIM2:
        case BSP_PERIPH_ID_TIM3:
        case BSP_PERIPH_ID_TIM4:
        case BSP_PERIPH_ID_TIM5:
        case BSP_PERIPH_ID_TIM6:
        case BSP_PERIPH_ID_TIM7:
        case BSP_PERIPH_ID_TIM12:
        case BSP_PERIPH_ID_TIM13:
        case BSP_PERIPH_ID_TIM14:
        case BSP_PERIPH_ID_WWDG:
        case BSP_PERIPH_ID_SPI2:
        case BSP_PERIPH_ID_SPI3:
        case BSP_PERIPH_ID_USART2:
        case BSP_PERIPH_ID_USART3:
        case BSP_PERIPH_ID_USART4:
        case BSP_PERIPH_ID_USART5:
        case BSP_PERIPH_ID_I2C1:
        case BSP_PERIPH_ID_I2C2:
        case BSP_PERIPH_ID_I2C3:
        case BSP_PERIPH_ID_CAN1:
        case BSP_PERIPH_ID_CAN2:
        case BSP_PERIPH_ID_PWR:
        case BSP_PERIPH_ID_DAC:
             clk_div = ((BSP_PERIPH_REG_RCC_CFGR & 0x00001C00u) >> 10u);
             if (clk_div < 4u) {
                 return (clk_freq);
             }
             clk_div   = ((clk_div - 3u) << 1u);
             clk_freq /= clk_div;
             return (clk_freq);

        case BSP_PERIPH_ID_TIM1:
        case BSP_PERIPH_ID_TIM8:
        case BSP_PERIPH_ID_USART1:
        case BSP_PERIPH_ID_USART6:
        case BSP_PERIPH_ID_ADC1:
        case BSP_PERIPH_ID_ADC2:
        case BSP_PERIPH_ID_ADC3:
        case BSP_PERIPH_ID_SDIO:
        case BSP_PERIPH_ID_SPI1:
        case BSP_PERIPH_ID_SYSCFG:
        case BSP_PERIPH_ID_TIM9:
        case BSP_PERIPH_ID_TIM10:
        case BSP_PERIPH_ID_TIM11:
             clk_div = ((BSP_PERIPH_REG_RCC_CFGR & 0x000E000u) >> 13u);
             if (clk_div < 4u) {
                 return (clk_freq);
             }
             clk_div   = ((clk_div - 3u) << 1u);
             clk_freq /= clk_div;
             return (clk_freq);
    }

    return ((CPU_INT32U)0u);
}


/*
*********************************************************************************************************
*                                             BSP_PeriphEn()
*
* Description : Enable clock for peripheral.
*
* Argument(s) : pwr_clk_id      Power/clock ID.
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  BSP_PeriphEn (CPU_DATA  pwr_clk_id)
{
    switch (pwr_clk_id) {
        case BSP_PERIPH_ID_GPIOA:
        case BSP_PERIPH_ID_GPIOB:
        case BSP_PERIPH_ID_GPIOC:
        case BSP_PERIPH_ID_GPIOD:
        case BSP_PERIPH_ID_GPIOE:
        case BSP_PERIPH_ID_GPIOF:
        case BSP_PERIPH_ID_GPIOG:
        case BSP_PERIPH_ID_GPIOH:
        case BSP_PERIPH_ID_GPIOI:
        case BSP_PERIPH_ID_CRC:
        case BSP_PERIPH_ID_BKPSRAM:
        case BSP_PERIPH_ID_CCMDATARAM:
        case BSP_PERIPH_ID_DMA1:
        case BSP_PERIPH_ID_DMA2:
        case BSP_PERIPH_ID_ETHMAC:
        case BSP_PERIPH_ID_ETHMACPTP:
        case BSP_PERIPH_ID_ETHMACTX:
        case BSP_PERIPH_ID_ETHMACRX:
        case BSP_PERIPH_ID_OTGHS:
        case BSP_PERIPH_ID_OTGHSULPI:
             BSP_PERIPH_REG_RCC_AHB1ENR |= DEF_BIT(pwr_clk_id);
             break;

        case BSP_PERIPH_ID_DCMI:
        case BSP_PERIPH_ID_CRYP:
        case BSP_PERIPH_ID_HASH:
        case BSP_PERIPH_ID_RNG:
        case BSP_PERIPH_ID_OTGFS:
             BSP_PERIPH_REG_RCC_AHB2ENR |= DEF_BIT(pwr_clk_id - 32u);
             break;

        case BSP_PERIPH_ID_FSMC:
             BSP_PERIPH_REG_RCC_AHB3ENR |= DEF_BIT(pwr_clk_id - 64u);
             break;

        case BSP_PERIPH_ID_TIM2:
        case BSP_PERIPH_ID_TIM3:
        case BSP_PERIPH_ID_TIM4:
        case BSP_PERIPH_ID_TIM5:
        case BSP_PERIPH_ID_TIM6:
        case BSP_PERIPH_ID_TIM7:
        case BSP_PERIPH_ID_TIM12:
        case BSP_PERIPH_ID_TIM13:
        case BSP_PERIPH_ID_TIM14:
        case BSP_PERIPH_ID_WWDG:
        case BSP_PERIPH_ID_SPI2:
        case BSP_PERIPH_ID_SPI3:
        case BSP_PERIPH_ID_USART2:
        case BSP_PERIPH_ID_USART3:
        case BSP_PERIPH_ID_USART4:
        case BSP_PERIPH_ID_USART5:
        case BSP_PERIPH_ID_I2C1:
        case BSP_PERIPH_ID_I2C2:
        case BSP_PERIPH_ID_I2C3:
        case BSP_PERIPH_ID_CAN1:
        case BSP_PERIPH_ID_CAN2:
        case BSP_PERIPH_ID_PWR:
        case BSP_PERIPH_ID_DAC:
             BSP_PERIPH_REG_RCC_APB1ENR |= DEF_BIT(pwr_clk_id - 96u);
             break;

        case BSP_PERIPH_ID_TIM1:
        case BSP_PERIPH_ID_TIM8:
        case BSP_PERIPH_ID_USART1:
        case BSP_PERIPH_ID_USART6:
        case BSP_PERIPH_ID_ADC1:
        case BSP_PERIPH_ID_ADC2:
        case BSP_PERIPH_ID_ADC3:
        case BSP_PERIPH_ID_SDIO:
        case BSP_PERIPH_ID_SPI1:
        case BSP_PERIPH_ID_SYSCFG:
        case BSP_PERIPH_ID_TIM9:
        case BSP_PERIPH_ID_TIM10:
        case BSP_PERIPH_ID_TIM11:
             BSP_PERIPH_REG_RCC_APB2ENR |= DEF_BIT(pwr_clk_id - 128u);
             break;
    }
}


/*
*********************************************************************************************************
*                                             BSP_PeriphDis()
*
* Description : Disable clock for peripheral.
*
* Argument(s) : pwr_clk_id      Power/clock ID.
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  BSP_PeriphDis (CPU_DATA  pwr_clk_id)
{
    switch (pwr_clk_id) {
        case BSP_PERIPH_ID_GPIOA:
        case BSP_PERIPH_ID_GPIOB:
        case BSP_PERIPH_ID_GPIOC:
        case BSP_PERIPH_ID_GPIOD:
        case BSP_PERIPH_ID_GPIOE:
        case BSP_PERIPH_ID_GPIOF:
        case BSP_PERIPH_ID_GPIOG:
        case BSP_PERIPH_ID_GPIOH:
        case BSP_PERIPH_ID_GPIOI:
        case BSP_PERIPH_ID_CRC:
        case BSP_PERIPH_ID_BKPSRAM:
        case BSP_PERIPH_ID_CCMDATARAM:
        case BSP_PERIPH_ID_DMA1:
        case BSP_PERIPH_ID_DMA2:
        case BSP_PERIPH_ID_ETHMAC:
        case BSP_PERIPH_ID_ETHMACPTP:
        case BSP_PERIPH_ID_ETHMACTX:
        case BSP_PERIPH_ID_ETHMACRX:
        case BSP_PERIPH_ID_OTGHS:
        case BSP_PERIPH_ID_OTGHSULPI:
             BSP_PERIPH_REG_RCC_AHB1ENR &= ~DEF_BIT(pwr_clk_id);
             break;

        case BSP_PERIPH_ID_DCMI:
        case BSP_PERIPH_ID_CRYP:
        case BSP_PERIPH_ID_HASH:
        case BSP_PERIPH_ID_RNG:
        case BSP_PERIPH_ID_OTGFS:
             BSP_PERIPH_REG_RCC_AHB2ENR &= ~DEF_BIT(pwr_clk_id - 32u);
             break;

        case BSP_PERIPH_ID_FSMC:
             BSP_PERIPH_REG_RCC_AHB3ENR &= ~DEF_BIT(pwr_clk_id - 64u);
             break;

        case BSP_PERIPH_ID_TIM2:
        case BSP_PERIPH_ID_TIM3:
        case BSP_PERIPH_ID_TIM4:
        case BSP_PERIPH_ID_TIM5:
        case BSP_PERIPH_ID_TIM6:
        case BSP_PERIPH_ID_TIM7:
        case BSP_PERIPH_ID_TIM12:
        case BSP_PERIPH_ID_TIM13:
        case BSP_PERIPH_ID_TIM14:
        case BSP_PERIPH_ID_WWDG:
        case BSP_PERIPH_ID_SPI2:
        case BSP_PERIPH_ID_SPI3:
        case BSP_PERIPH_ID_USART2:
        case BSP_PERIPH_ID_USART3:
        case BSP_PERIPH_ID_USART4:
        case BSP_PERIPH_ID_USART5:
        case BSP_PERIPH_ID_I2C1:
        case BSP_PERIPH_ID_I2C2:
        case BSP_PERIPH_ID_I2C3:
        case BSP_PERIPH_ID_CAN1:
        case BSP_PERIPH_ID_CAN2:
        case BSP_PERIPH_ID_PWR:
        case BSP_PERIPH_ID_DAC:
             BSP_PERIPH_REG_RCC_APB1ENR &= ~DEF_BIT(pwr_clk_id - 96u);
             break;

        case BSP_PERIPH_ID_TIM1:
        case BSP_PERIPH_ID_TIM8:
        case BSP_PERIPH_ID_USART1:
        case BSP_PERIPH_ID_USART6:
        case BSP_PERIPH_ID_ADC1:
        case BSP_PERIPH_ID_ADC2:
        case BSP_PERIPH_ID_ADC3:
        case BSP_PERIPH_ID_SDIO:
        case BSP_PERIPH_ID_SPI1:
        case BSP_PERIPH_ID_SYSCFG:
        case BSP_PERIPH_ID_TIM9:
        case BSP_PERIPH_ID_TIM10:
        case BSP_PERIPH_ID_TIM11:
             BSP_PERIPH_REG_RCC_APB2ENR &= ~DEF_BIT(pwr_clk_id - 128u);
             break;
    }
}
