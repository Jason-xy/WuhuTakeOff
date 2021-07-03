#include "receiver.h"

volatile uint8_t captureFlag[6] = {0};  //捕获状态
volatile uint32_t CapVal[6] = {0};      //第一次下降沿计数值
extern double Duty[6] ;
const float Cycle = 20000;

void Receiver_Config(void) { TIM3_Cap_Init(0xFFFF, 84 - 1); }

void Receiver_IRQ_Handler(void){
    int receiverNum;
    if (TIM_GetITStatus(TIM3, TIM_IT_CC1) != RESET) {
        receiverNum = 0;

        switch (captureFlag[receiverNum]) {
            case 0:
                TIM_Cmd(TIM3, DISABLE);
                TIM_SetCounter(TIM3, 0);
                TIM_OC1PolarityConfig(TIM3, TIM_ICPolarity_Falling);
                TIM_Cmd(TIM3, ENABLE);
                captureFlag[receiverNum]=1;
                break;
            case 1:
                CapVal[receiverNum] = TIM_GetCapture1(TIM3);
                Duty[receiverNum] = (float)((int)CapVal[receiverNum] % (int)Cycle);

                //Motor_Set(Duty[receiverNum], receiverNum + 1);
                TIM_OC1PolarityConfig(TIM3, TIM_ICPolarity_Rising);
                captureFlag[receiverNum] = 0;

                break;
        }
        TIM_ClearITPendingBit(TIM3, TIM_IT_CC1 | TIM_IT_Update);
    }

    else if (TIM_GetITStatus(TIM3, TIM_IT_CC2) != RESET) {
        receiverNum = 1;

        switch (captureFlag[receiverNum]) {
            case 0:
                TIM_Cmd(TIM3, DISABLE);
                TIM_SetCounter(TIM3, 0);
                TIM_OC2PolarityConfig(TIM3, TIM_ICPolarity_Falling);
                TIM_Cmd(TIM3, ENABLE);
                captureFlag[receiverNum]=1;
                break;
            case 1:
                CapVal[receiverNum] = TIM_GetCapture2(TIM3);
                Duty[receiverNum] = (float)((int)CapVal[receiverNum] % (int)Cycle);
                //Motor_Set(Duty[receiverNum], receiverNum + 1);
                TIM_OC2PolarityConfig(TIM3, TIM_ICPolarity_Rising);
                captureFlag[receiverNum] = 0;

                break;
        }
        TIM_ClearITPendingBit(TIM3, TIM_IT_CC2 | TIM_IT_Update);
    }

    else if (TIM_GetITStatus(TIM3, TIM_IT_CC3) != RESET) {
        receiverNum = 2;

        switch (captureFlag[receiverNum]) {
            case 0:
                TIM_Cmd(TIM3, DISABLE);
                TIM_SetCounter(TIM3, 0);
                TIM_OC3PolarityConfig(TIM3, TIM_ICPolarity_Falling);
                TIM_Cmd(TIM3, ENABLE);
                captureFlag[receiverNum]=1;
                break;
            case 1:
                CapVal[receiverNum] = TIM_GetCapture3(TIM3);
                Duty[receiverNum] = (float)((int)CapVal[receiverNum] % (int)Cycle);
                //Motor_Set(Duty[receiverNum], receiverNum + 1);
                TIM_OC3PolarityConfig(TIM3, TIM_ICPolarity_Rising);
                captureFlag[receiverNum] = 0;

                break;
        }
        TIM_ClearITPendingBit(TIM3, TIM_IT_CC3 | TIM_IT_Update);
    }

    else if (TIM_GetITStatus(TIM3, TIM_IT_CC4) != RESET) {
        receiverNum = 3;

        switch (captureFlag[receiverNum]) {
            case 0:
                TIM_Cmd(TIM3, DISABLE);
                TIM_SetCounter(TIM3, 0);
                TIM_OC4PolarityConfig(TIM3, TIM_ICPolarity_Falling);
                TIM_Cmd(TIM3, ENABLE);
                captureFlag[receiverNum]=1;
                break;
            case 1:
                CapVal[receiverNum] = TIM_GetCapture4(TIM3);
                Duty[receiverNum] = (float)((int)CapVal[receiverNum] % (int)Cycle);
                //Motor_Set(Duty[receiverNum], receiverNum + 1);
                TIM_OC4PolarityConfig(TIM3, TIM_ICPolarity_Rising);
                captureFlag[receiverNum] = 0;
        }
        TIM_ClearITPendingBit(TIM3, TIM_IT_CC4 | TIM_IT_Update);
    }
}
