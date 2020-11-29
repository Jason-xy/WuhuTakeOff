/**
  ******************************************************************************
  * �ļ�����: stm32f4xx_it.c 
  * ��    ��: Jason_xy
  * ���˲��ͣ�https://jason-xy.cn
  * ��    ��: V1.0
  * ��д����: 2020-11-8
  * ��    ��: �жϷ�����
  ******************************************************************************
  * ˵����
  * �ޡ�
  * 
	* ���ܣ�
	* 1.�ṩ�û������ж����
  * 2.�ṩϵͳ�ж����
  ******************************************************************************
  */

#include "main.h"
#include "stm32f4xx_it.h"

extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim4;
extern UART_HandleTypeDef huart1;


//ϵͳ��શ�ʱ���ж�
void SysTick_Handler(void)
{
  HAL_IncTick();
}

//TIM2���벶���ж�
void TIM2_IRQHandler(void)
{
  HAL_TIM_IRQHandler(&htim2);
}

//TIM4���벶���ж�
void TIM4_IRQHandler(void)
{
  HAL_TIM_IRQHandler(&htim4);
}

//USART1�ж�
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
