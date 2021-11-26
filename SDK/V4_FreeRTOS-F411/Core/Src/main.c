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
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "semphr.h"
//#include "timers.h"
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
extern Angle_t angle;
extern osMutexId_t GY86MutexHandle;
extern float Duty[6]; //接收机各通道PWM占空比
extern int isLock;
extern float motor1, motor2, motor3, motor4; //四个电机速度
extern uint8_t *UART1_temp;
extern uint32_t pidT; //采样周期
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void MX_FREERTOS_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void system_init(void);

#define Task_Angel_PRIO 6
#define Task_Angel_SIZE 512
TaskHandle_t Task_AngelHandler;
void Task_AngelFunction(void *argument)
{
  while (1)
  {
    GY86_RawDataUpdate();
    xSemaphoreTake(GY86MutexHandle, portMAX_DELAY);
    Attitude_Update(GY86->Gyro->data->Gyro_ds.x * ANGLE_TO_RAD, GY86->Gyro->data->Gyro_ds.y * ANGLE_TO_RAD, GY86->Gyro->data->Gyro_ds.z * ANGLE_TO_RAD, GY86->Accel->data->Accel_ms2.x, GY86->Accel->data->Accel_ms2.y, GY86->Accel->data->Accel_ms2.z,
                    GY86->Mag->data->Mag_raw.x, GY86->Mag->data->Mag_raw.y, GY86->Mag->data->Mag_raw.z);
    xSemaphoreGive(GY86MutexHandle);
    vTaskDelay(10);
  }
}

#define Task_Hight_PRIO 7
#define Task_Hight_SIZE 256
TaskHandle_t Task_HightHandler;
void Task_HightFunction(void *argument)
{
  ;
}

#define Task_PID_PRIO 8
#define Task_PID_SIZE 256
TaskHandle_t Task_PIDHandler;
void Task_PIDFunction(void *argument)
{
  static uint32_t startT, endT;
  while (1)
  {
    Motor_Exp_Calc(); //计算遥控器期望值
    endT = xTaskGetTickCount();
    xSemaphoreTake(GY86MutexHandle, portMAX_DELAY);
    pidT = endT - startT;
    Motor_Calc(); // 计算PID以及要输出的电机速度
    xSemaphoreGive(GY86MutexHandle);
    startT = xTaskGetTickCount();
    // 输出电机速度
    if (!isLock)
    {
      Motor_Set(motor1, TIM_CHANNEL_1);
      Motor_Set(motor2, TIM_CHANNEL_2);
      Motor_Set(motor3, TIM_CHANNEL_3);
      Motor_Set(motor4, TIM_CHANNEL_4);
    }
    else
    {
      Motor_Set(0, TIM_CHANNEL_1);
      Motor_Set(0, TIM_CHANNEL_2);
      Motor_Set(0, TIM_CHANNEL_3);
      Motor_Set(0, TIM_CHANNEL_4);
    }
    vTaskDelay(10);
  }
}

#define Task_ANO_PRIO 10
#define Task_ANO_SIZE 256
TaskHandle_t Task_ANOHandler;
void Task_ANOFunction(void *argument)
{
  int roll, pitch, yaw;
  while (1)
  {
    xSemaphoreTake(GY86MutexHandle, portMAX_DELAY);
    roll = angle.roll * 100;
    pitch = angle.pitch * 100;
    yaw = angle.yaw * 100;
    xSemaphoreGive(GY86MutexHandle);
    //datafusion
    ANO_Angle_Transform(roll, pitch, yaw);
    //RC
    ANO_RC_Transform((short)(Duty[0]*100.0f), (short)(Duty[1]*100.0f), (short)(Duty[2]*100.0f), (short)(Duty[3]*100.0f), (short)(Duty[4]*100.0f), (short)(Duty[5]*100.0f));
		//Motor
		ANO_MotorOut(motor1, motor2, motor3, motor4);
    vTaskDelay(10);
  }
}

#define Task_OLED_PRIO 11
#define Task_OLED_SIZE 256
TaskHandle_t Task_OLEDHandler;
void Task_OLEDFunction(void *argument)
{
  static short Gx, Gy, Gz, Ax, Ay, Az, yaw, pitch, roll, temp;
  while (1)
  {
    xSemaphoreTake(GY86MutexHandle, portMAX_DELAY);
    Gx = GY86->Gyro->data->Gyro_ds.x;
    Gy = GY86->Gyro->data->Gyro_ds.y;
    Gz = GY86->Gyro->data->Gyro_ds.z;

    Ax = GY86->Accel->data->Accel_ms2.x;
    Ay = GY86->Accel->data->Accel_ms2.y;
    Az = GY86->Accel->data->Accel_ms2.z;

    temp = GY86->MS->Temperature;

    roll = angle.roll;
    pitch = angle.pitch;
    yaw = angle.yaw;
    xSemaphoreGive(GY86MutexHandle);
    OLED_Show_3num(Gx, Gy, Gz, 0);
    OLED_Show_3num(Ax, Ay, Az, 1);
    OLED_Show_3num(pitch, roll, yaw, 2);
    OLED_Show_3num(Duty[0] * 100, Duty[1] * 100, Duty[2] * 100, 3);
    OLED_Show_3num(Duty[3] * 100, Duty[4] * 100, Duty[5] * 100, 4);
    OLED_Show_2num((short)motor1, (short)motor2, 5);
    OLED_Show_2num((short)motor3, (short)motor4, 6);
    OLED_ShowNum(24, 7, temp, 2, 12);
    vTaskDelay(200);
  }
}
void Task_ExtSensorFunction(void *argument)
{
  ;
}
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
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_USART6_UART_Init();
  /* USER CODE BEGIN 2 */
  GY86 = (GY_86 *)malloc(sizeof(GY_86));
  system_init();
  Quat_Init();
  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize(); /* Call init function for freertos objects (in freertos.c) */
  MX_FREERTOS_Init();
  /* Start scheduler */
  //osKernelStart();

  /* We should never get here as control is now taken by the scheduler */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  xTaskCreate((TaskFunction_t)Task_AngelFunction,
              (char *)"Angel_Task",
              (uint16_t)Task_Angel_SIZE,
              (void *)NULL,
              (UBaseType_t)Task_Angel_PRIO,
              (TaskHandle_t *)&Task_AngelHandler);

  xTaskCreate((TaskFunction_t)Task_ANOFunction,
              (char *)"ANO_Task",
              (uint16_t)Task_ANO_SIZE,
              (void *)NULL,
              (UBaseType_t)Task_ANO_PRIO,
              (TaskHandle_t *)&Task_ANOHandler);

  xTaskCreate((TaskFunction_t)Task_OLEDFunction,
              (char *)"OLED_Task",
              (uint16_t)Task_OLED_SIZE,
              (void *)NULL,
              (UBaseType_t)Task_OLED_PRIO,
              (TaskHandle_t *)&Task_OLEDHandler);

  xTaskCreate((TaskFunction_t)Task_PIDFunction,
              (char *)"PID_Task",
              (uint16_t)Task_PID_SIZE,
              (void *)NULL,
              (UBaseType_t)Task_PID_PRIO,
              (TaskHandle_t *)&Task_PIDHandler);

  vTaskStartScheduler();

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
  RCC_OscInitStruct.PLL.PLLM = 6;
  RCC_OscInitStruct.PLL.PLLN = 100;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
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
void system_init(void)
{
  //硬件模块
  //OLED
  OLED_Init();
  OLED_Clear();

  //ESP8266
  HAL_UART_Receive_IT(&huart1, (uint8_t *)UART1_temp, 1);
  esp8266_init();
  //输出调试信息
  OLED_Clear();
  OLED_ShowString(8, 3, (uint8_t *)"ESP8266_Init OK", 16);
  esp8266_cipsend("ESP8266_Init OK\r\n");
  HAL_Delay(1000);

  //GY-86初始
  GY86_Init();
  //输出调试信息
  OLED_Clear();
  OLED_ShowString(12, 3, (uint8_t *)"GY86_Init OK", 16);
  esp8266_cipsend("GY86_Init OK\r\n");
  HAL_Delay(1000);

  //接收机初始化
  Input_Capture_Init();
  //输出调试信息
  OLED_Clear();
  OLED_ShowString(0, 3, (uint8_t *)"Input_Capture_Init", 16);
  esp8266_cipsend("Input_Capture_Init OK\r\n");
  HAL_Delay(1000);

  //电机
  Motor_Init();
  //输出调试信息
  OLED_Clear();
  OLED_ShowString(12, 3, (uint8_t *)"Motor_Init OK", 16);
  esp8266_cipsend("Motor_Init OK\r\n");
  HAL_Delay(1000);

  //电机自动解锁
  Motor_Init();
  Motor_Unlock();
  //输出调试信息
  OLED_Clear();
  OLED_ShowString(6, 3, (uint8_t *)"Motor_Unlock OK", 16);
  esp8266_cipsend("Motor_Unlock OK\r\n");
  HAL_Delay(1000);

  //PID初始化
  PID_Init();
  //输出调试信息
  OLED_Clear();
  OLED_ShowString(6, 3, (uint8_t *)"PID_Init OK", 16);
  esp8266_cipsend("PID_Init OK\r\n");
  HAL_Delay(1000);

  //OLED图形界面绘制
  OLED_Clear();
  OLED_Draw_interface();
}
/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM10 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM10)
  {
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

#ifdef USE_FULL_ASSERT
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
