/**
  ******************************************************************************
  * 文件名程: usart.c 
  * 作    者: Jason_xy
  * 个人博客：https://jason-xy.cn
  * 版    本: V1.0
  * 编写日期: 2020-9-18
  * 功    能: USART模块驱动
  ******************************************************************************
  * 说明：
  * 无。
  * 
	* 功能：
	* 1.USART1异步传输初始化。
  ******************************************************************************
  */

#include "usart.h"


//重定向c库函数printf到DEBUG_USARTx
int fputc(int ch, FILE *f)
{
  HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xFFFF);
  return ch;
}

//USART处理结构体定义
UART_HandleTypeDef huart1;

/* USART1 初始化函数 */
void MX_USART1_UART_Init(void)
{

  huart1.Instance = USART1; //USART1寄存器基地址
  huart1.Init.BaudRate = 115200;  //波特率
  huart1.Init.WordLength = UART_WORDLENGTH_8B;  //字长
  huart1.Init.StopBits = UART_STOPBITS_1; //停止位
  huart1.Init.Parity = UART_PARITY_NONE;  //校验位
  huart1.Init.Mode = UART_MODE_TX_RX; //输入输出模式
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }

}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(uartHandle->Instance==USART1)
  {
    /* USART1 时钟使能 */
    __HAL_RCC_USART1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART1 GPIO配置
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USART1 中断配置 */
    HAL_NVIC_SetPriority(USART1_IRQn, 2, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
  }
}

// void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
// {

//   if(uartHandle->Instance==USART1)
//   {
//   /* USER CODE BEGIN USART1_MspDeInit 0 */

//   /* USER CODE END USART1_MspDeInit 0 */
//     /* Peripheral clock disable */
//     __HAL_RCC_USART1_CLK_DISABLE();

//     /**USART1 GPIO Configuration
//     PA9     ------> USART1_TX
//     PA10     ------> USART1_RX
//     */
//     HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9|GPIO_PIN_10);

//     /* USART1 interrupt Deinit */
//     HAL_NVIC_DisableIRQ(USART1_IRQn);
//   /* USER CODE BEGIN USART1_MspDeInit 1 */

//   /* USER CODE END USART1_MspDeInit 1 */
//   }
// }

// /* USER CODE BEGIN 1 */

// /* USER CODE END 1 */

// /************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
