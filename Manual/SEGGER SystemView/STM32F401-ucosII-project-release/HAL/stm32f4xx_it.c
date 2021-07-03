#include "receiver.h"
#include "stm32f4xx.h"
#include "sysTick.h"
#include "ucos_ii.h"
#include "hm10.h"


void USART6_IRQHandler(void) {
#if OS_CRITICAL_METHOD == 3u                               /* Allocate storage for CPU status register */
    OS_CPU_SR  cpu_sr = 0u;
#endif
	OS_ENTER_CRITICAL();
	OSIntEnter();
	OS_EXIT_CRITICAL();
	
	HM10_IRQ_IRQHandler();
	
	OSIntExit(); 
	
}

void TIM3_IRQHandler(void) {
#if OS_CRITICAL_METHOD == 3u                               /* Allocate storage for CPU status register */
    OS_CPU_SR  cpu_sr = 0u;
#endif
	OS_ENTER_CRITICAL();
	OSIntEnter();
	OS_EXIT_CRITICAL();
	
	Receiver_IRQ_Handler();
	
	OSIntExit();
}

void HardFault_Handler(void) {
    while (1)
        ;
}

void OS_CPU_SysTickHandler(void)
{	
	OS_CPU_SR  cpu_sr;
	
	TimingDelay_Decrement();	

	OS_ENTER_CRITICAL();                         /* Tell uC/OS-II that we are starting an ISR          */
	OSIntNesting++;
	OS_EXIT_CRITICAL();

	OSTimeTick();                                /* Call uC/OS-II's OSTimeTick()                       */
	
	OSIntExit();                                 /* Tell uC/OS-II that we are leaving the ISR          */
}
