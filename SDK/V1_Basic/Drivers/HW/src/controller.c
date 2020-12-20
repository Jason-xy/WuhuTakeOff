/**
  ******************************************************************************
  * 文件名程: controller.c 
  * 作    者: Jason_xy
  * 个人博客：https://jason-xy.cn
  * 版    本: V1.0
  * 编写日期: 2020-11-15
  * 功    能: 接收机模块驱动
  ******************************************************************************
  * 说明：
  * 1.需要先完成TIM的初始化。
  * 2.需提前设置时钟频率100MHz（根据自己的板子设定，比我我的Psc = 99）捕获通道每一个节拍1us。
  * 
  * 功能：
  * 1.接收机数据采集。
  * 2.根据采集数据改变马达转速。
  ******************************************************************************
  */

#include "controller.h"

//时钟频率100MHz，Psc = 99，捕获通道每一个节拍1us。

//中断触发通道
volatile HAL_TIM_ActiveChannel ActiveChannel[4]={	HAL_TIM_ACTIVE_CHANNEL_1, 
                                                    HAL_TIM_ACTIVE_CHANNEL_2, 
                                                    HAL_TIM_ACTIVE_CHANNEL_3, 
                                                    HAL_TIM_ACTIVE_CHANNEL_4};

//TIM通道
volatile uint8_t Channel[4] = { 0x00000000U, //TIM_CHANNEL_1
                                0x00000004U, //TIM_CHANNEL_2
                                0x00000008U, //TIM_CHANNEL_3
                                0x0000000CU};//TIM_CHANNEL_4

volatile uint32_t CapVal[6] = { 0 };	//第一次下降沿计数值
volatile uint8_t Flag[6] = { 0 };	//捕获状态

const float Cycle = 20000; //PWM波周期 20000us
float Duty[6] = { 0 };  //捕获所得占空比
int i = 0;  //临时计数器
int cap = 0;//正在进行捕获的通道识别

//输入捕获启动函数
void Input_Capture_Init(void)
{
    //开启通道捕获中断
    for(i = 0; i < 4; i++)
    {
        HAL_TIM_IC_Start_IT(&htim2,Channel[i]);
    }
    for(i = 3; i < 5; i++)
    {
        HAL_TIM_IC_Start_IT(&htim4,Channel[i]);
    }
}

//输入捕获中断回调函数
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
    if(htim->Instance == TIM2)
    {
        for(i = 0; i < 4; i++)    
        {
            if(htim->Channel == ActiveChannel[i] && Duty[5] > 0.07)
            {
                cap = 1;    //标志是否进行了一次捕获
                switch(Flag[i])//捕获状态
                {
                    case 0://第一次捕获到上升沿
                    __HAL_TIM_SET_COUNTER(htim,0);  //计数器置0
                    TIM_RESET_CAPTUREPOLARITY (htim, Channel[i]);   //重置触发条件
                    TIM_SET_CAPTUREPOLARITY(htim, Channel[i], TIM_ICPOLARITY_FALLING);  //设置为下降沿捕获
                    Flag[i]++;  //修改捕获状态
                    break;

                    case 1://第一次捕获到下降沿
                    CapVal[i] = HAL_TIM_ReadCapturedValue(htim,Channel[i]); //读取计数器值
                    if(CapVal[i] <= 2500)   //过滤超时情况
                    Duty[i] = CapVal[i] / Cycle;    //计算占空比，Cycle = 20000us
                    Motor_Set(Duty[i], Channel[i]); //更改单机转速
                    //准备进行下一次捕获
                    TIM_RESET_CAPTUREPOLARITY(htim, Channel[i]);    //重置触发条件
                    TIM_SET_CAPTUREPOLARITY(htim, Channel[i], TIM_ICPOLARITY_RISING);   //设置为下降沿捕获
                    Flag[i] = 0;    //重置捕获状态
                    break;
                }
            }
            if(cap)
            {
                cap = 0;
                break;
            }
        }
    }else if(htim->Instance == TIM4)
    {
        for(i = 4; i < 6; i++)
        {
            if(htim->Channel == ActiveChannel[i-2])
            {
                cap = 1;
                switch(Flag[i])//捕获状态
                {
                    case 0:
                    __HAL_TIM_SET_COUNTER(htim,0);
                    TIM_RESET_CAPTUREPOLARITY (htim, Channel[i-2]);
                    TIM_SET_CAPTUREPOLARITY(htim, Channel[i-2], TIM_ICPOLARITY_FALLING);
                    Flag[i]++;
                    break;

                    case 1:
                    CapVal[i] = HAL_TIM_ReadCapturedValue(htim,Channel[i-2]);
                    if(CapVal[i] <= 2500)   //过滤超时情况
                    Duty[i] = CapVal[i] / Cycle;//Cycle = 20000us
                    if(Duty[5] <= 0.055)
                    Motor_Lock();   //电机锁定
                    TIM_RESET_CAPTUREPOLARITY(htim, Channel[i-2]);
                    TIM_SET_CAPTUREPOLARITY(htim, Channel[i-2], TIM_ICPOLARITY_RISING);
                    Flag[i] = 0;
                    break;
                }
            }
            if(cap)
            {
                cap = 0;
                break;
            }
        }
    }
}
