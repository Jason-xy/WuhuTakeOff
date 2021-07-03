@*
@********************************************************************************************************
@                                    EXCEPTION VECTORS & STARTUP CODE
@
@ File      : startup.s
@ For       : ARMv7M Cortex-M4
@ Mode      : Thumb2
@ Toolchain : Atollic TrueSTUDIO(R)
@ Notes     : This module performs:
@                 - Set the initial SP
@                 - Set the initial PC == Reset_Handler,
@                 - Set the vector table entries with the exceptions ISR address
@                 - Branches to main in the C library (which eventually
@                   calls main()).
@              After Reset the Cortex-M4 processor is in Thread mode,
@              priority is Privileged, and the Stack is set to Main.
@********************************************************************************************************
@


@********************************************************************************************************
@                                      CODE GENERATION DIRECTIVES
@********************************************************************************************************
    .syntax unified
    .cpu cortex-m4
    .fpu softvfp
    .thumb


@********************************************************************************************************
@                                           PUBLIC FUNCTIONS
@********************************************************************************************************

    .global  g_pfnVectors


@********************************************************************************************************
@                                       LINKER SCRIPT DEFINITIONS
@********************************************************************************************************

    .word  _sidata                          @ start addr for the init values of the .data section.
    .word  _sdata                           @ start addr for the .data section.
    .word  _edata                           @ end address for the .data section.
    .word  _sbss                            @ start address for the .bss section.
    .word  _ebss                            @ end address for the .bss section.


@********************************************************************************************************
@                                               EQUATES
@********************************************************************************************************


@********************************************************************************************************
@                                      void Reset_Handler (void)
@
@ Note(s) : 1) This code gets called after a reset event.
@
@           2) Reset_Handler() MUST:
@              a) Copy .data section from ROM to RAM.
@              b) Clear .bss section (Zero init).
@              c) Call low level initialzation routine.
@              d) Run static constructors.
@              e) Enter main.
@              f) Loop forever if returning from main.
@********************************************************************************************************

    .section  .text.Reset_Handler
    .weak  Reset_Handler
    .type  Reset_Handler, %function
Reset_Handler:
    ldr sp, =_estack                        @ set stack pointer.

                                            @ Copy the data segment initializers from flash to SRAM
    movs r1, #0
    b LoopCopyDataInit

CopyDataInit:
    ldr r3, =_sidata
    ldr r3, [r3, r1]
    str r3, [r0, r1]
    adds r1, r1, #4

LoopCopyDataInit:
    ldr r0, =_sdata
    ldr r3, =_edata
    adds r2, r0, r1
    cmp r2, r3
    bcc CopyDataInit
    ldr r2, =_sbss
    b LoopFillZerobss

                                            @ Zero fill the bss segment.
FillZerobss:
    movs r3, #0
    str r3, [r2], #4

LoopFillZerobss:
    ldr r3, = _ebss
    cmp r2, r3
    bcc FillZerobss

                                            @ Call the clock system intitialization function.
    bl  SystemInit

#if (defined(__VFP_FP__) && !defined(__SOFTFP__))
                                            @ Enable Floating Point Support at reset for FPU
    LDR.W   R0, =0xE000ED88                 @ Load address of CPACR register
    LDR     R1, [R0]                        @ Read value at CPACR
    ORR     R1,  R1, #(0xF <<20)            @ Set bits 20-23 to enable CP10 and CP11 coprocessors
                                            @ Write back the modified CPACR value
    STR     R1, [R0]                        @ Wait for store to complete
    DSB

                                            @ Disable automatic FP register content
                                            @ Disable lazy context switch
    LDR.W   R0, =0xE000EF34                 @ Load address to FPCCR register
    LDR     R1, [R0]
    AND     R1,  R1, #(0x3FFFFFFF)          @ Clear the LSPEN and ASPEN bits
    STR     R1, [R0]
    ISB                                     @ Reset pipeline now the FPU is enabled
#endif
                                            @ Call static constructors
    bl __libc_init_array
                                            @ Call the application's entry point.
    bl  main
    bx  lr

.size	Reset_Handler, .-Reset_Handler


@/*
@*********************************************************************************************************
@*                                  EXCEPTION / INTERRUPT VECTOR TABLE
@*
@* Note(s) : (1) The Cortex-M4 may have up to 256 external interrupts, which are the final entries in the
@*               vector table.  The STM32F4xx has 81 external interrupt vectors.
@*
@*           (2) Interrupts vector 2-13 are implemented in this file as infinite loop for debuging
@*               purposes only. The application might implement a recover procedure if it is needed.
@*
@*           (3) OS_CPU_PendSVHandler() and OS_CPU_SysTickHandler() are implemented in the generic OS
@*               port.
@*********************************************************************************************************
@*/

    .section  .isr_vector,"a",%progbits
    .type  g_pfnVectors, %object
    .size  g_pfnVectors, .-g_pfnVectors

                                            @ Vector Table Mapped to Address 0 at Reset
    .extern  BSP_IntHandlerWWDG
    .extern  BSP_IntHandlerPVD
    .extern  BSP_IntHandlerTAMP_STAMP
    .extern  BSP_IntHandlerRTC_WKUP
    .extern  BSP_IntHandlerFLASH
    .extern  BSP_IntHandlerRCC
    .extern  BSP_IntHandlerEXTI0
    .extern  BSP_IntHandlerEXTI1
    .extern  BSP_IntHandlerEXTI2
    .extern  BSP_IntHandlerEXTI3
    .extern  BSP_IntHandlerEXTI4
    .extern  BSP_IntHandlerDMA1_CH0
    .extern  BSP_IntHandlerDMA1_CH1
    .extern  BSP_IntHandlerDMA1_CH2
    .extern  BSP_IntHandlerDMA1_CH3
    .extern  BSP_IntHandlerDMA1_CH4
    .extern  BSP_IntHandlerDMA1_CH5
    .extern  BSP_IntHandlerDMA1_CH6
    .extern  BSP_IntHandlerADC
    .extern  BSP_IntHandlerCAN1_TX
    .extern  BSP_IntHandlerCAN1_RX0
    .extern  BSP_IntHandlerCAN1_RX1
    .extern  BSP_IntHandlerCAN1_SCE
    .extern  BSP_IntHandlerEXTI9_5
    .extern  BSP_IntHandlerTIM1_BRK_TIM9
    .extern  BSP_IntHandlerTIM1_UP_TIM10
    .extern  BSP_IntHandlerTIM1_TRG_COM_TIM11
    .extern  BSP_IntHandlerTIM1_CC
    .extern  BSP_IntHandlerTIM2
    .extern  BSP_IntHandlerTIM3
    .extern  BSP_IntHandlerTIM4
    .extern  BSP_IntHandlerI2C1_EV
    .extern  BSP_IntHandlerI2C1_ER
    .extern  BSP_IntHandlerI2C2_EV
    .extern  BSP_IntHandlerI2C2_ER
    .extern  BSP_IntHandlerSPI1
    .extern  BSP_IntHandlerSPI2
    .extern  BSP_IntHandlerUSART1
    .extern  BSP_IntHandlerUSART2
    .extern  BSP_IntHandlerUSART3
    .extern  BSP_IntHandlerEXTI15_10
    .extern  BSP_IntHandlerRTCAlarm
    .extern  BSP_IntHandlerOTG_FS_WKUP
    .extern  BSP_IntHandlerTIM8_BRK_TIM12
    .extern  BSP_IntHandlerTIM8_UP_TIM13
    .extern  BSP_IntHandlerTIM8_TRG_COM_TIM14
    .extern  BSP_IntHandlerTIM8_CC
    .extern  BSP_IntHandlerDMA1_STREAM7
    .extern  BSP_IntHandlerFSMC
    .extern  BSP_IntHandlerSDIO
    .extern  BSP_IntHandlerTIM5
    .extern  BSP_IntHandlerSPI3
    .extern  BSP_IntHandlerUSART4
    .extern  BSP_IntHandlerUSART5
    .extern  BSP_IntHandlerTIM6_DAC
    .extern  BSP_IntHandlerTIM7
    .extern  BSP_IntHandlerDMA2_CH0
    .extern  BSP_IntHandlerDMA2_CH1
    .extern  BSP_IntHandlerDMA2_CH2
    .extern  BSP_IntHandlerDMA2_CH3
    .extern  BSP_IntHandlerDMA2_CH4
    .extern  BSP_IntHandlerETH
    .extern  BSP_IntHandlerETHWakeup
    .extern  BSP_IntHandlerCAN2_TX
    .extern  BSP_IntHandlerCAN2_RX0
    .extern  BSP_IntHandlerCAN2_RX1
    .extern  BSP_IntHandlerCAN2_SCE
    .extern  BSP_IntHandlerOTG_FS
    .extern  BSP_IntHandlerDMA2_CH5
    .extern  BSP_IntHandlerDMA2_CH6
    .extern  BSP_IntHandlerDMA2_CH7
    .extern  BSP_IntHandlerUSART6
    .extern  BSP_IntHandlerI2C3_EV
    .extern  BSP_IntHandlerI2C3_ER
    .extern  BSP_IntHandlerOTG_HS_EP1_OUT
    .extern  BSP_IntHandlerOTG_HS_EP1_IN
    .extern  BSP_IntHandlerOTG_HS_WKUP
    .extern  BSP_IntHandlerOTG_HS
    .extern  BSP_IntHandlerDCMI
    .extern  BSP_IntHandlerCRYP
    .extern  BSP_IntHandlerHASH_RNG
    .extern  BSP_IntHandlerFPU

    .extern  OS_CPU_PendSVHandler
    .extern  OS_CPU_SysTickHandler

g_pfnVectors:
                                                    @ Processor exception vectors
    .word     _estack                               @ Top of Stack
    .word     Reset_Handler                         @ Reset Handler
    .word     App_NMI_ISR                           @ NMI Handler
    .word     App_Fault_ISR                         @ Hard Fault Handler
    .word     App_MemFault_ISR                      @ MPU Fault Handler
    .word     App_BusFault_ISR                      @ Bus Fault Handler
    .word     App_UsageFault_ISR                    @ Usage Fault Handler
    .word     App_Spurious_ISR                      @ Reserved
    .word     App_Spurious_ISR                      @ Reserved
    .word     App_Spurious_ISR                      @ Reserved
    .word     App_Spurious_ISR                      @ Reserved
    .word     App_Spurious_ISR                      @ SVCall Handler
    .word     App_Spurious_ISR                      @ Debug Monitor Handler
    .word     App_Spurious_ISR                      @ Reserved
    .word     OS_CPU_PendSVHandler                  @ PendSV Handler
    .word     OS_CPU_SysTickHandler                 @ SysTick Handler

                                                    @  External Interrupts
    .word     BSP_IntHandlerWWDG                    @/* 16, INTISR[  0]  Window Watchdog                       */
    .word     BSP_IntHandlerPVD                     @/* 17, INTISR[  1]  PVD through EXTI Line Detection.      */
    .word     BSP_IntHandlerTAMP_STAMP              @/* 18, INTISR[  2]  Tamper and TimeStamp Interrupt.       */
    .word     BSP_IntHandlerRTC_WKUP                @/* 19, INTISR[  3]  RTC Wakeup Global Interrupt.          */
    .word     BSP_IntHandlerFLASH                   @/* 20, INTISR[  4]  FLASH Global Interrupt.               */
    .word     BSP_IntHandlerRCC                     @/* 21, INTISR[  5]  RCC Global Interrupt.                 */
    .word     BSP_IntHandlerEXTI0                   @/* 22, INTISR[  6]  EXTI Line0 Interrupt.                 */
    .word     BSP_IntHandlerEXTI1                   @/* 23, INTISR[  7]  EXTI Line1 Interrupt.                 */
    .word     BSP_IntHandlerEXTI2                   @/* 24, INTISR[  8]  EXTI Line2 Interrupt.                 */
    .word     BSP_IntHandlerEXTI3                   @/* 25, INTISR[  9]  EXTI Line3 Interrupt.                 */
    .word     BSP_IntHandlerEXTI4                   @/* 26, INTISR[ 10]  EXTI Line4 Interrupt.                 */
    .word     BSP_IntHandlerDMA1_CH0                @/* 27, INTISR[ 11]  DMA Channel0 Global Interrupt.        */
    .word     BSP_IntHandlerDMA1_CH1                @/* 28, INTISR[ 12]  DMA Channel1 Global Interrupt.        */
    .word     BSP_IntHandlerDMA1_CH2                @/* 29, INTISR[ 13]  DMA Channel2 Global Interrupt.        */
    .word     BSP_IntHandlerDMA1_CH3                @/* 30, INTISR[ 14]  DMA Channel3 Global Interrupt.        */
    .word     BSP_IntHandlerDMA1_CH4                @/* 31, INTISR[ 15]  DMA Channel4 Global Interrupt.        */
    .word     BSP_IntHandlerDMA1_CH5                @/* 32, INTISR[ 16]  DMA Channel5 Global Interrupt.        */
    .word     BSP_IntHandlerDMA1_CH6                @/* 33, INTISR[ 17]  DMA Channel6 Global Interrupt.        */
    .word     BSP_IntHandlerADC                     @/* 34, INTISR[ 18]  ADC1, ADC2 & ADC3 Global Interrupt.   */
    .word     BSP_IntHandlerCAN1_TX                 @/* 35, INTISR[ 19]  CAN1 TX  Interrupts.                  */
    .word     BSP_IntHandlerCAN1_RX0                @/* 36, INTISR[ 20]  CAN1 RX0 Interrupts.                  */
    .word     BSP_IntHandlerCAN1_RX1                @/* 37, INTISR[ 21]  CAN1 RX1 Interrupt.                   */
    .word     BSP_IntHandlerCAN1_SCE                @/* 38, INTISR[ 22]  CAN1 SCE Interrupt.                   */
    .word     BSP_IntHandlerEXTI9_5                 @/* 39, INTISR[ 23]  EXTI Line[9:5] Interrupt.             */
    .word     BSP_IntHandlerTIM1_BRK_TIM9           @/* 40, INTISR[ 24]  TIM1 Break  Interrupt & TIM9 Glb int. */
    .word     BSP_IntHandlerTIM1_UP_TIM10           @/* 41, INTISR[ 25]  TIM1 Update Interrupt & TIM10 Glb int.*/
    .word     BSP_IntHandlerTIM1_TRG_COM_TIM11      @/* 42, INTISR[ 26]  TIM1 Trig & Commutation Interrupts... */
                                                    @/* .................and TIM11 global interrupt.           */
    .word     BSP_IntHandlerTIM1_CC                 @/* 43, INTISR[ 27]  TIM1 Capture Compare Interrupt.       */
    .word     BSP_IntHandlerTIM2                    @/* 44, INTISR[ 28]  TIM2 Global Interrupt.                */
    .word     BSP_IntHandlerTIM3                    @/* 45, INTISR[ 29]  TIM3 Global Interrupt.                */
    .word     BSP_IntHandlerTIM4                    @/* 46, INTISR[ 30]  TIM4 Global Interrupt.                */
    .word     BSP_IntHandlerI2C1_EV                 @/* 47, INTISR[ 31]  I2C1 Event  Interrupt.                */
    .word     BSP_IntHandlerI2C1_ER                 @/* 48, INTISR[ 32]  I2C1 Error  Interrupt.                */
    .word     BSP_IntHandlerI2C2_EV                 @/* 49, INTISR[ 33]  I2C2 Event  Interrupt.                */
    .word     BSP_IntHandlerI2C2_ER                 @/* 50, INTISR[ 34]  I2C2 Error  Interrupt.                */
    .word     BSP_IntHandlerSPI1                    @/* 51, INTISR[ 35]  SPI1 Global Interrupt.                */
    .word     BSP_IntHandlerSPI2                    @/* 52, INTISR[ 36]  SPI2 Global Interrupt.                */
    .word     BSP_IntHandlerUSART1                  @/* 53, INTISR[ 37]  USART1 Global Interrupt.              */
    .word     BSP_IntHandlerUSART2                  @/* 54, INTISR[ 38]  USART2 Global Interrupt.              */
    .word     BSP_IntHandlerUSART3                  @/* 55, INTISR[ 39]  USART3 Global Interrupt.              */
    .word     BSP_IntHandlerEXTI15_10               @/* 56, INTISR[ 40]  EXTI Line [15:10] Interrupts.         */
    .word     BSP_IntHandlerRTCAlarm                @/* 57, INTISR[ 41]  RTC Alarm EXT Line Interrupt.         */
    .word     BSP_IntHandlerOTG_FS_WKUP             @/* 58, INTISR[ 42]  USB OTG Wakeup through EXTI line int. */
    .word     BSP_IntHandlerTIM8_BRK_TIM12          @/* 59, INTISR[ 43]  TIM8 Break and TIM12 global Int.      */
    .word     BSP_IntHandlerTIM8_UP_TIM13           @/* 60, INTISR[ 44]  TIM8 Update and TIM13 global Int.     */
    .word     BSP_IntHandlerTIM8_TRG_COM_TIM14      @/* 61, INTISR[ 45]  TIM8 Trigger/Commutation and TIM14.   */
    .word     BSP_IntHandlerTIM8_CC                 @/* 62, INTISR[ 46]  TIM8 Capture Compare Int.             */
    .word     BSP_IntHandlerDMA1_STREAM7            @/* 63, INTISR[ 47]  DMA1 Stream7 Int.                     */
    .word     BSP_IntHandlerFSMC                    @/* 64, INTISR[ 48]  FSMC global Int.                      */
    .word     BSP_IntHandlerSDIO                    @/* 65, INTISR[ 49]  SDIO global Int.                      */
    .word     BSP_IntHandlerTIM5                    @/* 66, INTISR[ 50]  TIM5 global Interrupt.                */
    .word     BSP_IntHandlerSPI3                    @/* 67, INTISR[ 51]  SPI3 global Interrupt.                */
    .word     BSP_IntHandlerUSART4                  @/* 68, INTISR[ 52]  UART4 global Interrupt.               */
    .word     BSP_IntHandlerUSART5                  @/* 69, INTISR[ 53]  UART5 global Interrupt.               */
    .word     BSP_IntHandlerTIM6_DAC                @/* 70, INTISR[ 54]  TIM6 global Interrupt, DAC1 & DAC2 .. */
                                                    @/* ................ underrun error interrupts.            */

    .word     BSP_IntHandlerTIM7                    @/* 71, INTISR[ 55]  TIM7 global Interrupt.                */
    .word     BSP_IntHandlerDMA2_CH0                @/* 72, INTISR[ 56]  DMA2 Channel 0 global Interrupt.      */
    .word     BSP_IntHandlerDMA2_CH1                @/* 73, INTISR[ 57]  DMA2 Channel 1 global Interrupt.      */
    .word     BSP_IntHandlerDMA2_CH2                @/* 74, INTISR[ 58]  DMA2 Channel 2 global Interrupt.      */
    .word     BSP_IntHandlerDMA2_CH3                @/* 75, INTISR[ 59]  DMA2 Channel 3 global Interrupt.      */
    .word     BSP_IntHandlerDMA2_CH4                @/* 76, INTISR[ 60]  DMA2 Channel 4 global Interrupt.      */
    .word     BSP_IntHandlerETH                     @/* 77, INTISR[ 61]  ETH global Interrupt.                 */
    .word     BSP_IntHandlerETHWakeup               @/* 78, INTISR[ 62]  ETH WakeUp from EXTI line Interrupt.  */
    .word     BSP_IntHandlerCAN2_TX                 @/* 79, INTISR[ 63]  CAN2 TX Interrupts.                   */
    .word     BSP_IntHandlerCAN2_RX0                @/* 80, INTISR[ 64]  CAN2 RX0 Interrupts.                  */
    .word     BSP_IntHandlerCAN2_RX1                @/* 81, INTISR[ 65]  CAN2 RX1 Interrupt.                   */
    .word     BSP_IntHandlerCAN2_SCE                @/* 82, INTISR[ 66]  CAN2 SCE Interrupt.                   */
    .word     BSP_IntHandlerOTG_FS                  @/* 83, INTISR[ 67]  OTG global Interrupt.                 */
    .word     BSP_IntHandlerDMA2_CH5                @/* 84, INTISR[ 68]  DMA2 Channel 5 global Interrupt.      */
    .word     BSP_IntHandlerDMA2_CH6                @/* 85, INTISR[ 69]  DMA2 Channel 6 global Interrupt.      */
    .word     BSP_IntHandlerDMA2_CH7                @/* 86, INTISR[ 70]  DMA2 Channel 7 global Interrupt.      */
    .word     BSP_IntHandlerUSART6                  @/* 87, INTISR[ 71]  UART6 global Interrupt.               */
    .word     BSP_IntHandlerI2C3_EV                 @/* 88, INTISR[ 72]  I2C3 Event  Interrupt.                */
    .word     BSP_IntHandlerI2C3_ER                 @/* 89, INTISR[ 73]  I2C3 Error  Interrupt.                */
    .word     BSP_IntHandlerOTG_HS_EP1_OUT          @/* 90, INTISR[ 74]  OTG HS EP1 OUT Interrupt.             */
    .word     BSP_IntHandlerOTG_HS_EP1_IN           @/* 91, INTISR[ 75]  OTG HS EP1 IN Interrupt.              */
    .word     BSP_IntHandlerOTG_HS_WKUP             @/* 92, INTISR[ 76]  OTG HS Wakeup Interrupt.              */
    .word     BSP_IntHandlerOTG_HS                  @/* 93, INTISR[ 77]  OTG HS global Interrupt.              */
    .word     BSP_IntHandlerDCMI                    @/* 94, INTISR[ 78]  DCMI global Interrupt.                */
    .word     BSP_IntHandlerCRYP                    @/* 95, INTISR[ 79]  CRYPT crypto global Interrupt.        */
    .word     BSP_IntHandlerHASH_RNG                @/* 96, INTISR[ 80]  HASH and RNG global Interrupt.        */
    .word     BSP_IntHandlerFPU                     @/* 97, INTISR[ 81]  FPU global Interrupt.                 */


@ Dummy Exception Handlers (infinite loops which can be modified)

    .section	.text.App_NMI_ISR
App_NMI_ISR:
	b	App_NMI_ISR
	.size	App_NMI_ISR, .-App_NMI_ISR


    .section	.text.App_Fault_ISR
App_Fault_ISR:
	b	App_Fault_ISR
	.size	App_Fault_ISR, .-App_Fault_ISR


    .section	.text.App_MemFault_ISR
App_MemFault_ISR:
	b	App_MemFault_ISR
	.size	App_MemFault_ISR, .-App_MemFault_ISR


    .section	.text.App_BusFault_ISR
App_BusFault_ISR:
	b	App_BusFault_ISR
	.size	App_BusFault_ISR, .-App_BusFault_ISR


    .section	.text.App_UsageFault_ISR
App_UsageFault_ISR:
	b	App_UsageFault_ISR
	.size	App_UsageFault_ISR, .-App_UsageFault_ISR

    .section	.text.App_Spurious_ISR
App_Spurious_ISR:
	b	App_Spurious_ISR
	.size	App_Spurious_ISR, .-App_Spurious_ISR


    .section	.text.App_Reserved_ISR
App_Reserved_ISR:
	b	App_Reserved_ISR
	.size	App_Reserved_ISR, .-App_Reserved_ISR

