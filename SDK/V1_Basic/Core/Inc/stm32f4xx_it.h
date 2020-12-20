/**
  ******************************************************************************
  * 文件名程: stm32f4xx_it.h 
  * 作    者: Jason_xy
  * 个人博客：https://jason-xy.cn
  * 版    本: V1.0
  * 编写日期: 2020-11-8
  * 功    能: 中断服务函数
  ******************************************************************************
  * 说明：
  * 无。
  * 
	* 功能：
	* 1.提供用户程序中断入口
  * 2.提供系统中断入口
  ******************************************************************************
  */

#ifndef __STM32F4xx_IT_H
#define __STM32F4xx_IT_H

#ifdef __cplusplus
 extern "C" {
#endif


void NMI_Handler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void SVC_Handler(void);
void DebugMon_Handler(void);
void PendSV_Handler(void);
void SysTick_Handler(void);
void TIM2_IRQHandler(void);
void TIM4_IRQHandler(void);
void USART1_IRQHandler(void);


#ifdef __cplusplus
}
#endif

#endif /* __STM32F4xx_IT_H */
