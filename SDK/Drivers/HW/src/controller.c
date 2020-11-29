/**
  ******************************************************************************
  * �ļ�����: controller.c 
  * ��    ��: Jason_xy
  * ���˲��ͣ�https://jason-xy.cn
  * ��    ��: V1.0
  * ��д����: 2020-11-15
  * ��    ��: ���ջ�ģ������
  ******************************************************************************
  * ˵����
  * 1.��Ҫ�����TIM�ĳ�ʼ����
  * 2.����ǰ����ʱ��Ƶ��100MHz�������Լ��İ����趨�������ҵ�Psc = 99������ͨ��ÿһ������1us��
  * 
  * ���ܣ�
  * 1.���ջ����ݲɼ���
  * 2.���ݲɼ����ݸı����ת�١�
  ******************************************************************************
  */

#include "controller.h"

//ʱ��Ƶ��100MHz��Psc = 99������ͨ��ÿһ������1us��

//�жϴ���ͨ��
volatile HAL_TIM_ActiveChannel ActiveChannel[4]={	HAL_TIM_ACTIVE_CHANNEL_1, 
                                                    HAL_TIM_ACTIVE_CHANNEL_2, 
                                                    HAL_TIM_ACTIVE_CHANNEL_3, 
                                                    HAL_TIM_ACTIVE_CHANNEL_4};

//TIMͨ��
volatile uint8_t Channel[4] = { 0x00000000U, //TIM_CHANNEL_1
                                0x00000004U, //TIM_CHANNEL_2
                                0x00000008U, //TIM_CHANNEL_3
                                0x0000000CU};//TIM_CHANNEL_4

volatile uint32_t CapVal[6] = { 0 };	//��һ���½��ؼ���ֵ
volatile uint8_t Flag[6] = { 0 };	//����״̬

const float Cycle = 20000; //PWM������ 20000us
float Duty[6] = { 0 };  //��������ռ�ձ�
int i = 0;  //��ʱ������
int cap = 0;//���ڽ��в����ͨ��ʶ��

//���벶����������
void Input_Capture_Init(void)
{
    //����ͨ�������ж�
    for(i = 0; i < 4; i++)
    {
        HAL_TIM_IC_Start_IT(&htim2,Channel[i]);
    }
    for(i = 3; i < 5; i++)
    {
        HAL_TIM_IC_Start_IT(&htim4,Channel[i]);
    }
}

//���벶���жϻص�����
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
    if(htim->Instance == TIM2)
    {
        for(i = 0; i < 4; i++)    
        {
            if(htim->Channel == ActiveChannel[i] && Duty[5] > 0.07)
            {
                cap = 1;    //��־�Ƿ������һ�β���
                switch(Flag[i])//����״̬
                {
                    case 0://��һ�β���������
                    __HAL_TIM_SET_COUNTER(htim,0);  //��������0
                    TIM_RESET_CAPTUREPOLARITY (htim, Channel[i]);   //���ô�������
                    TIM_SET_CAPTUREPOLARITY(htim, Channel[i], TIM_ICPOLARITY_FALLING);  //����Ϊ�½��ز���
                    Flag[i]++;  //�޸Ĳ���״̬
                    break;

                    case 1://��һ�β����½���
                    CapVal[i] = HAL_TIM_ReadCapturedValue(htim,Channel[i]); //��ȡ������ֵ
                    if(CapVal[i] <= 2500)   //���˳�ʱ���
                    Duty[i] = CapVal[i] / Cycle;    //����ռ�ձȣ�Cycle = 20000us
                    Motor_Set(Duty[i], Channel[i]); //���ĵ���ת��
                    //׼��������һ�β���
                    TIM_RESET_CAPTUREPOLARITY(htim, Channel[i]);    //���ô�������
                    TIM_SET_CAPTUREPOLARITY(htim, Channel[i], TIM_ICPOLARITY_RISING);   //����Ϊ�½��ز���
                    Flag[i] = 0;    //���ò���״̬
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
                switch(Flag[i])//����״̬
                {
                    case 0:
                    __HAL_TIM_SET_COUNTER(htim,0);
                    TIM_RESET_CAPTUREPOLARITY (htim, Channel[i-2]);
                    TIM_SET_CAPTUREPOLARITY(htim, Channel[i-2], TIM_ICPOLARITY_FALLING);
                    Flag[i]++;
                    break;

                    case 1:
                    CapVal[i] = HAL_TIM_ReadCapturedValue(htim,Channel[i-2]);
                    if(CapVal[i] <= 2500)   //���˳�ʱ���
                    Duty[i] = CapVal[i] / Cycle;//Cycle = 20000us
                    if(Duty[5] <= 0.055)
                    Motor_Lock();   //�������
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
