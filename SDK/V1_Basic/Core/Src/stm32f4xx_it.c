/**
  ******************************************************************************
  * 文件名程: stm32f4xx_it.c 
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

#include "main.h"
#include "stm32f4xx_it.h"

extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim4;
extern UART_HandleTypeDef huart1;


//系统嘀嗒定时器中断
void SysTick_Handler(void)
{
  HAL_IncTick();
}

//TIM2输入捕获中断
void TIM2_IRQHandler(void)
{
  HAL_TIM_IRQHandler(&htim2);
}

//TIM4输入捕获中断
void TIM4_IRQHandler(void)
{
  HAL_TIM_IRQHandler(&htim4);
}

//USART1中断
void USART1_IRQHandler(void)
{
  HAL_UART_IRQHandler(&huart1);
}

/******************************************************************************/
/*           Cortex-M4 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{

}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{

  while (1)
  {

  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  while (1)
  {

  }
}

/**
  * @brief This function handles Pre-fetch fault, memory access fault.
  */
void BusFault_Handler(void)
{

  while (1)
  {

  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  while (1)
  {

  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{

}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{

}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{

}
