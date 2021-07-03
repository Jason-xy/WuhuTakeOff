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
*                                         EXCEPTION VECTORS
*
*                                     ST Microelectronics STM32
*                                              on the
*
*                                           STM3240G-EVAL
*                                         Evaluation Board
*
* Filename      : cstartup.c
* Version       : V1.00
* Programmer(s) : FT
*                 FF
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#include  <cpu.h>
#include  <os.h>

#include  <bsp.h>


/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                          LOCAL DATA TYPES
*********************************************************************************************************
*/

typedef  union {
    CPU_FNCT_VOID   Fnct;
    void           *Ptr;
} APP_INTVECT_ELEM;


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

#pragma language=extended
#pragma segment="CSTACK"

static  void  App_Reset_ISR       (void);

static  void  App_NMI_ISR         (void);

static  void  App_Fault_ISR       (void);

static  void  App_BusFault_ISR    (void);

static  void  App_UsageFault_ISR  (void);

static  void  App_MemFault_ISR    (void);

static  void  App_Spurious_ISR    (void);

extern  void  __iar_program_start (void);

extern  void  SystemInit          (void);


/*
*********************************************************************************************************
*                                     LOCAL CONFIGURATION ERRORS
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                  EXCEPTION / INTERRUPT VECTOR TABLE
*
* Note(s) : (1) The Cortex-M3 may have up to 256 external interrupts, which are the final entries in the
*               vector table.  The STM32F4xxx has 81 external interrupt vectors.
*
*           (2) Interrupts vector 2-13 are implemented in this file as infinite loop for debuging
*               purposes only. The application might implement a recover procedure if it is needed.
*
*           (3) OS_CPU_PendSVHandler() and OS_CPU_SysTickHandler() are implemented in the generic OS
*               port.
*********************************************************************************************************
*/

__root  const  APP_INTVECT_ELEM  __vector_table[] @ ".intvec" = {
    { .Ptr = (void *)__sfe( "CSTACK" )},                        /*  0, SP start value.                                    */
    App_Reset_ISR,                                              /*  1, PC start value.                                    */
    App_NMI_ISR,                                                /*  2, NMI.                                               */
    App_Fault_ISR,                                              /*  3, Hard Fault.                                        */
    App_MemFault_ISR,                                           /*  4, Memory Management.                                 */
    App_BusFault_ISR,                                           /*  5, Bus Fault.                                         */
    App_UsageFault_ISR,                                         /*  6, Usage Fault.                                       */
    App_Spurious_ISR,                                           /*  7, Reserved.                                          */
    App_Spurious_ISR,                                           /*  8, Reserved.                                          */
    App_Spurious_ISR,                                           /*  9, Reserved.                                          */
    App_Spurious_ISR,                                           /* 10, Reserved.                                          */
    App_Spurious_ISR,                                           /* 11, SVCall.                                            */
    App_Spurious_ISR,                                           /* 12, Debug Monitor.                                     */
    App_Spurious_ISR,                                           /* 13, Reserved.                                          */
    OS_CPU_PendSVHandler,                                       /* 14, PendSV Handler.                                    */
    OS_CPU_SysTickHandler,                                      /* 15, uC/OS-III Tick ISR Handler.                        */

    BSP_IntHandlerWWDG,                                         /* 16, INTISR[  0]  Window Watchdog.                      */
    BSP_IntHandlerPVD,                                          /* 17, INTISR[  1]  PVD through EXTI Line Detection.      */
    BSP_IntHandlerTAMP_STAMP,                                   /* 18, INTISR[  2]  Tamper and TimeStamp Interrupt.       */
    BSP_IntHandlerRTC_WKUP,                                     /* 19, INTISR[  3]  RTC Wakeup Global Interrupt.          */
    BSP_IntHandlerFLASH,                                        /* 20, INTISR[  4]  FLASH Global Interrupt.               */
    BSP_IntHandlerRCC,                                          /* 21, INTISR[  5]  RCC Global Interrupt.                 */
    BSP_IntHandlerEXTI0,                                        /* 22, INTISR[  6]  EXTI Line0 Interrupt.                 */
    BSP_IntHandlerEXTI1,                                        /* 23, INTISR[  7]  EXTI Line1 Interrupt.                 */
    BSP_IntHandlerEXTI2,                                        /* 24, INTISR[  8]  EXTI Line2 Interrupt.                 */
    BSP_IntHandlerEXTI3,                                        /* 25, INTISR[  9]  EXTI Line3 Interrupt.                 */
    BSP_IntHandlerEXTI4,                                        /* 26, INTISR[ 10]  EXTI Line4 Interrupt.                 */
    BSP_IntHandlerDMA1_CH0,                                     /* 27, INTISR[ 11]  DMA Channel0 Global Interrupt.        */
    BSP_IntHandlerDMA1_CH1,                                     /* 28, INTISR[ 12]  DMA Channel1 Global Interrupt.        */
    BSP_IntHandlerDMA1_CH2,                                     /* 29, INTISR[ 13]  DMA Channel2 Global Interrupt.        */
    BSP_IntHandlerDMA1_CH3,                                     /* 30, INTISR[ 14]  DMA Channel3 Global Interrupt.        */
    BSP_IntHandlerDMA1_CH4,                                     /* 31, INTISR[ 15]  DMA Channel4 Global Interrupt.        */
    BSP_IntHandlerDMA1_CH5,                                     /* 32, INTISR[ 16]  DMA Channel5 Global Interrupt.        */
    BSP_IntHandlerDMA1_CH6,                                     /* 33, INTISR[ 17]  DMA Channel6 Global Interrupt.        */
    BSP_IntHandlerADC,                                          /* 34, INTISR[ 18]  ADC1, ADC2 & ADC3 Global Interrupt.   */
    BSP_IntHandlerCAN1_TX,                                      /* 35, INTISR[ 19]  CAN1 TX  Interrupts.                  */
    BSP_IntHandlerCAN1_RX0,                                     /* 36, INTISR[ 20]  CAN1 RX0 Interrupts.                  */
    BSP_IntHandlerCAN1_RX1,                                     /* 37, INTISR[ 21]  CAN1 RX1 Interrupt.                   */
    BSP_IntHandlerCAN1_SCE,                                     /* 38, INTISR[ 22]  CAN1 SCE Interrupt.                   */
    BSP_IntHandlerEXTI9_5,                                      /* 39, INTISR[ 23]  EXTI Line[9:5] Interrupt.             */
    BSP_IntHandlerTIM1_BRK_TIM9,                                /* 40, INTISR[ 24]  TIM1 Break  Interrupt & TIM9 Glb int. */
    BSP_IntHandlerTIM1_UP_TIM10,                                /* 41, INTISR[ 25]  TIM1 Update Interrupt & TIM10 Glb int.*/
    BSP_IntHandlerTIM1_TRG_COM_TIM11,                           /* 42, INTISR[ 26]  TIM1 Trig & Commutation Interrupts... */
                                                                /* .................and TIM11 global interrupt.           */

    BSP_IntHandlerTIM1_CC,                                      /* 43, INTISR[ 27]  TIM1 Capture Compare Interrupt.       */
    BSP_IntHandlerTIM2,                                         /* 44, INTISR[ 28]  TIM2 Global Interrupt.                */
    BSP_IntHandlerTIM3,                                         /* 45, INTISR[ 29]  TIM3 Global Interrupt.                */
    BSP_IntHandlerTIM4,                                         /* 46, INTISR[ 30]  TIM4 Global Interrupt.                */
    BSP_IntHandlerI2C1_EV,                                      /* 47, INTISR[ 31]  I2C1 Event  Interrupt.                */
    BSP_IntHandlerI2C1_ER,                                      /* 48, INTISR[ 32]  I2C1 Error  Interrupt.                */
    BSP_IntHandlerI2C2_EV,                                      /* 49, INTISR[ 33]  I2C2 Event  Interrupt.                */
    BSP_IntHandlerI2C2_ER,                                      /* 50, INTISR[ 34]  I2C2 Error  Interrupt.                */
    BSP_IntHandlerSPI1,                                         /* 51, INTISR[ 35]  SPI1 Global Interrupt.                */
    BSP_IntHandlerSPI2,                                         /* 52, INTISR[ 36]  SPI2 Global Interrupt.                */
    BSP_IntHandlerUSART1,                                       /* 53, INTISR[ 37]  USART1 Global Interrupt.              */
    BSP_IntHandlerUSART2,                                       /* 54, INTISR[ 38]  USART2 Global Interrupt.              */
    BSP_IntHandlerUSART3,                                       /* 55, INTISR[ 39]  USART3 Global Interrupt.              */
    BSP_IntHandlerEXTI15_10,                                    /* 56, INTISR[ 40]  EXTI Line [15:10] Interrupts.         */
    BSP_IntHandlerRTCAlarm,                                     /* 57, INTISR[ 41]  RTC Alarm EXT Line Interrupt.         */
    BSP_IntHandlerOTG_FS_WKUP,                                  /* 58, INTISR[ 42]  USB OTG Wakeup through EXTI line int. */
    BSP_IntHandlerTIM8_BRK_TIM12,                               /* 59, INTISR[ 43]  TIM8 Break and TIM12 global Int.      */
    BSP_IntHandlerTIM8_UP_TIM13,                                /* 60, INTISR[ 44]  TIM8 Update and TIM13 global Int.     */
    BSP_IntHandlerTIM8_TRG_COM_TIM14,                           /* 61, INTISR[ 45]  TIM8 Trigger/Commutation and TIM14.   */
    BSP_IntHandlerTIM8_CC,                                      /* 62, INTISR[ 46]  TIM8 Capture Compare Int.             */
    BSP_IntHandlerDMA1_STREAM7,                                 /* 63, INTISR[ 47]  DMA1 Stream7 Int.                     */
    BSP_IntHandlerFSMC,                                         /* 64, INTISR[ 48]  FSMC global Int.                      */
    BSP_IntHandlerSDIO,                                         /* 65, INTISR[ 49]  SDIO global Int.                      */
    BSP_IntHandlerTIM5,                                         /* 66, INTISR[ 50]  TIM5 global Interrupt.                */
    BSP_IntHandlerSPI3,                                         /* 67, INTISR[ 51]  SPI3 global Interrupt.                */
    BSP_IntHandlerUSART4,                                       /* 68, INTISR[ 52]  UART4 global Interrupt.               */
    BSP_IntHandlerUSART5,                                       /* 69, INTISR[ 53]  UART5 global Interrupt.               */
    BSP_IntHandlerTIM6_DAC,                                     /* 70, INTISR[ 54]  TIM6 global Interrupt, DAC1 & DAC2 .. */
                                                                /* ................ underrun error interrupts.            */

    BSP_IntHandlerTIM7,                                         /* 71, INTISR[ 55]  TIM7 global Interrupt.                */
    BSP_IntHandlerDMA2_CH0,                                     /* 72, INTISR[ 56]  DMA2 Channel 0 global Interrupt.      */
    BSP_IntHandlerDMA2_CH1,                                     /* 73, INTISR[ 57]  DMA2 Channel 1 global Interrupt.      */
    BSP_IntHandlerDMA2_CH2,                                     /* 74, INTISR[ 58]  DMA2 Channel 2 global Interrupt.      */
    BSP_IntHandlerDMA2_CH3,                                     /* 75, INTISR[ 59]  DMA2 Channel 3 global Interrupt.      */
    BSP_IntHandlerDMA2_CH4,                                     /* 76, INTISR[ 60]  DMA2 Channel 4 global Interrupt.      */
    BSP_IntHandlerETH,                                          /* 77, INTISR[ 61]  ETH global Interrupt.                 */
    BSP_IntHandlerETHWakeup,                                    /* 78, INTISR[ 62]  ETH WakeUp from EXTI line Interrupt.  */
    BSP_IntHandlerCAN2_TX,                                      /* 79, INTISR[ 63]  CAN2 TX Interrupts.                   */
    BSP_IntHandlerCAN2_RX0,                                     /* 80, INTISR[ 64]  CAN2 RX0 Interrupts.                  */
    BSP_IntHandlerCAN2_RX1,                                     /* 81, INTISR[ 65]  CAN2 RX1 Interrupt.                   */
    BSP_IntHandlerCAN2_SCE,                                     /* 82, INTISR[ 66]  CAN2 SCE Interrupt.                   */
    BSP_IntHandlerOTG_FS,                                       /* 83, INTISR[ 67]  OTG global Interrupt.                 */
    BSP_IntHandlerDMA2_CH5,                                     /* 84, INTISR[ 68]  DMA2 Channel 5 global Interrupt.      */
    BSP_IntHandlerDMA2_CH6,                                     /* 85, INTISR[ 69]  DMA2 Channel 6 global Interrupt.      */
    BSP_IntHandlerDMA2_CH7,                                     /* 86, INTISR[ 70]  DMA2 Channel 7 global Interrupt.      */
    BSP_IntHandlerUSART6,                                       /* 87, INTISR[ 71]  UART6 global Interrupt.               */
    BSP_IntHandlerI2C3_EV,                                      /* 88, INTISR[ 72]  I2C3 Event  Interrupt.                */
    BSP_IntHandlerI2C3_ER,                                      /* 89, INTISR[ 73]  I2C3 Error  Interrupt.                */
    BSP_IntHandlerOTG_HS_EP1_OUT,                               /* 90, INTISR[ 74]  OTG HS EP1 OUT Interrupt.             */
    BSP_IntHandlerOTG_HS_EP1_IN,                                /* 91, INTISR[ 75]  OTG HS EP1 IN Interrupt.              */
    BSP_IntHandlerOTG_HS_WKUP,                                  /* 92, INTISR[ 76]  OTG HS Wakeup Interrupt.              */
    BSP_IntHandlerOTG_HS,                                       /* 93, INTISR[ 77]  OTG HS global Interrupt.              */
    BSP_IntHandlerDCMI,                                         /* 94, INTISR[ 78]  DCMI global Interrupt.                */
    BSP_IntHandlerCRYP,                                         /* 95, INTISR[ 79]  CRYPT crypto global Interrupt.        */
    BSP_IntHandlerHASH_RNG,                                     /* 96, INTISR[ 80]  HASH and RNG global Interrupt.        */
    BSP_IntHandlerFPU                                           /* 97, INTISR[ 81]  FPU global Interrupt.                 */
};


/*
*********************************************************************************************************
*                                            App_Reset_ISR()
*
* Description : Handle Reset.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : This is an ISR.
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  void  App_Reset_ISR (void)
{
    SystemInit();

#if __ARMVFP__                                                  /* Enable access to Floating-point coprocessor.         */
    CPU_REG_NVIC_CPACR = CPU_REG_NVIC_CPACR_CP10_FULL_ACCESS | CPU_REG_NVIC_CPACR_CP11_FULL_ACCESS;

    DEF_BIT_CLR(CPU_REG_SCB_FPCCR, DEF_BIT_31);                 /* Disable automatic FP register content                */
    DEF_BIT_CLR(CPU_REG_SCB_FPCCR, DEF_BIT_30);                 /* Disable Lazy context switch                          */
#endif

    __iar_program_start();
}


/*
*********************************************************************************************************
*                                            App_NMI_ISR()
*
* Description : Handle Non-Maskable Interrupt (NMI).
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : This is an ISR.
*
* Note(s)     : (1) Since the NMI is not being used, this serves merely as a catch for a spurious
*                   exception.
*********************************************************************************************************
*/

static  void  App_NMI_ISR (void)
{
    while (DEF_TRUE) {
        ;
    }
}


/*
*********************************************************************************************************
*                                             App_Fault_ISR()
*
* Description : Handle hard fault.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : This is an ISR.
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  void  App_Fault_ISR (void)
{
    while (DEF_TRUE) {
        ;
    }
}


/*
*********************************************************************************************************
*                                           App_BusFault_ISR()
*
* Description : Handle bus fault.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : This is an ISR.
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  void  App_BusFault_ISR (void)
{
    while (DEF_TRUE) {
        ;
    }
}


/*
*********************************************************************************************************
*                                          App_UsageFault_ISR()
*
* Description : Handle usage fault.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : This is an ISR.
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  void  App_UsageFault_ISR (void)
{
    while (DEF_TRUE) {
        ;
    }
}


/*
*********************************************************************************************************
*                                           App_MemFault_ISR()
*
* Description : Handle memory fault.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : This is an ISR.
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  void  App_MemFault_ISR (void)
{
    while (DEF_TRUE) {
        ;
    }
}


/*
*********************************************************************************************************
*                                           App_Spurious_ISR()
*
* Description : Handle spurious interrupt.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : This is an ISR.
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  void  App_Spurious_ISR (void)
{
    while (DEF_TRUE) {
        ;
    }
}
