/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
//标准库
#include "stdio.h"
//硬件模块驱动层
#include "oled.h"
#include "gy-86.h"
#include "motor.h"
#include "controller.h"
#include "esp8266.h"
//system view
#include "SEGGER_SYSVIEW.h"
#include "os_trace_events.h"
//DMP姿态解算
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h" 
//APP
#include "data_transfer.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
float pitch,roll,yaw;	//姿态解算结果
unsigned int Cap[6];  //接收机各通道实时行程 0~100%
extern float Duty[6]; //接收机各通道PWM占空比	<controller.c>
char data[100]; //调试数据缓存

const char *data_transfer_name = "Data Transfer";
const char *data_fusion_name = "Data Fusion";
const char *on_board_debug_name = "On Board Debug";
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void system_init(void);
void time_init(uint8_t SYSCLK);
void NameInit(const char* pname);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
//DATA_TRANSFER 任务
//设置任务优先级
#define DATA_TRANSFER_TASK_PRIO      			7 
//设置任务堆栈大小
#define DATA_TRANSFER_STK_SIZE  				128
//任务堆栈	
OS_STK DATA_TRANSFER_TASK_STK[DATA_TRANSFER_STK_SIZE];
//任务函数
void data_transfer_task(void *pdata);	
				 
//ON BOARE DEBUG任务
//设置任务优先级
#define ON_BORAD_DEBUG_TASK_PRIO       			10
//设置任务堆栈大小
#define ON_BORAD_DEBUG_STK_SIZE  		    		128
//任务堆栈	
OS_STK ON_BORAD_DEBUG_TASK_STK[ON_BORAD_DEBUG_STK_SIZE];
//任务函数
void on_board_debug_task(void *pdata);

//DATA FUSION任务
//设置任务优先级
#define DATA_FUSION_TASK_PRIO       			6 
//设置任务堆栈大小
#define DATA_FUSION_STK_SIZE  					128
//任务堆栈
OS_STK DATA_FUSION_TASK_STK[DATA_FUSION_STK_SIZE];
//任务函数
void data_fusion_task(void *pdata);

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_I2C2_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_USART1_UART_Init();
  //MX_TIM1_Init();
  /* USER CODE BEGIN 2 */
	time_init(100);
	system_init();
	OSInit();
	OS_TRACE_INIT();
	OS_TRACE_START();
	OSTaskCreate(data_transfer_task,(void *)0,(OS_STK *)&DATA_TRANSFER_TASK_STK[DATA_TRANSFER_STK_SIZE-1],DATA_TRANSFER_TASK_PRIO );
	NameInit(data_transfer_name);
	OSTaskCreate(on_board_debug_task,(void *)0,(OS_STK *)&ON_BORAD_DEBUG_TASK_STK[ON_BORAD_DEBUG_STK_SIZE-1],ON_BORAD_DEBUG_TASK_PRIO );
	NameInit(on_board_debug_name);
	OSTaskCreate(data_fusion_task,(void *)0,(OS_STK *)&DATA_FUSION_TASK_STK[DATA_FUSION_STK_SIZE-1],DATA_FUSION_TASK_PRIO );
	NameInit(data_fusion_name);
	OSStart();	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 12;
  RCC_OscInitStruct.PLL.PLLN = 96;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
//TIME INIT
void time_init(uint8_t SYSCLK)
{
	uint32_t reload = 0;
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);//SysTick频率为HCLK
	reload=SYSCLK;					    //每秒钟的计数次数 单位为K	   
	reload*=1000000/OS_TICKS_PER_SEC;	//根据OS_TICKS_PER_SEC设定溢出时间
											//reload为24位寄存器,最大值:16777216,在72M下,约合0.233s左右	   
	SysTick->CTRL|=SysTick_CTRL_TICKINT_Msk;//开启SYSTICK中断
	SysTick->LOAD=reload; 					//每1/OS_TICKS_PER_SEC秒中断一次	
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk; //开启SYSTICK
}	

//data_transfer
void data_transfer_task(void *pdata)
{
	while(1)
	{
		for(int i = 0; i < 6; i++)
		{
			Cap[i] =  (Duty[i] * 100 - 5) / 0.05;
		}
		ANO_DT_Send_Status(roll,pitch,yaw,2333,122,(Cap[5]>50));
		ANO_DT_Send_Senser(Ax,Ay,Az,Gx,Gy,Gz,2333,2333,2333);
		ANO_DT_Send_RCData(0,0,0,0,Cap[0],Cap[1],Cap[2],Cap[3],Cap[4],Cap[5]);
		OSTimeDly(30);
	}
}

//on_board_debug
void on_board_debug_task(void *pdata)
{	 	
	while(1)
	{
		OLED_Show_3num(Gx, Gy, Gz, 0);
		OLED_Show_3num(Ax, Ay, Az, 1);
		OLED_Show_3num(Mag_x, Mag_y, Mag_z, 2);
		OLED_Show_3num(Cap[0], Cap[1], Cap[2], 3);
		OLED_Show_3num(Cap[3], Cap[4], Cap[5], 4);
		OLED_ShowNum(24, 7, MPU_Get_Temperature(),2,12);
		OSTimeDly(1000);
	}
}

//data_fusion
void data_fusion_task(void *pdata)
{	 	
	while(1)
	{
		read_Gyroscope_DPS();
		read_Accelerometer_MPS();
		mpu_dmp_get_data(&pitch,&roll,&yaw);
		OSTimeDly(10);
	}
}

void NameInit(const char* pname){
	SEGGER_SYSVIEW_TASKINFO Info;
	Info.TaskID = (unsigned long)OSTCBList;
	Info.sName = pname;
	SEGGER_SYSVIEW_SendTaskInfo(&Info);
}

void system_init(void)
{
	//硬件模块初始化
  //OLED初始化
  OLED_Init();  
	OLED_Clear();
	
  //ESP8266初始化
	esp8266_init(); 
  //输出调试信息
  OLED_Clear();
	OLED_ShowString(8,3,(uint8_t*)"ESP8266_Init OK",16);
	esp8266_cipsend("ESP8266_Init OK\r\n");
	HAL_Delay(1000);
	
  //电机初始化
	Motor_Init(); 
  //输出调试信息
	OLED_Clear();
	OLED_ShowString(12,3,(uint8_t*)"Motor_Init OK",16);
	esp8266_cipsend("Motor_Init OK\r\n");
	HAL_Delay(1000);
	
  //输出调试信息
	OLED_Clear();
	OLED_ShowString(12,3,(uint8_t*)"Motor_Unlock",16);
	esp8266_cipsend("Motor_Unlock\r\n");
	HAL_Delay(1000);
	//电机自动解锁
	Motor_Init();
	Motor_Unlock();
	//输出调试信息
	OLED_Clear();
	OLED_ShowString(6,3,(uint8_t*)"Motor_Unlock OK",16);
	esp8266_cipsend("Motor_Unlock OK\r\n");
	HAL_Delay(1000);

  //接收机初始化
	Input_Capture_Init();
	//输出调试信息
	OLED_Clear();
	OLED_ShowString(0,3,(uint8_t*)"Input_Capture_Init",16);
	esp8266_cipsend("Input_Capture_Init OK\r\n");
	HAL_Delay(1000);
	
	//GY-86初始化
	//	GY86_Init();
	mpu_dmp_init();
	//HAL_TIM_Base_Start_IT(&htim1);
  //输出调试信息
	OLED_Clear();
	OLED_ShowString(12,3,(uint8_t*)"GY86_Init OK",16);
	esp8266_cipsend("GY86_Init OK\r\n");
	HAL_Delay(1000);

  //OLED图形界面绘制
	OLED_Clear();
	OLED_Draw_interface();
}
/* USER CODE END 4 */

 /**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM11 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM11) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
