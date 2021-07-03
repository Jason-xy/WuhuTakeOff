#include "motor.h"

void Motor_Config(void) { TIM2_PWMOUTPUT_Config(); }

void Motor_Unlock(void) {
    TIM_SetCompare1(TIM2, 1000);
    TIM_SetCompare2(TIM2, 1000);
    TIM_SetCompare3(TIM2, 1000);
    TIM_SetCompare4(TIM2, 1000);
    Delay_ms(2000);
    TIM_SetCompare1(TIM2, 500);
    TIM_SetCompare2(TIM2, 500);
    TIM_SetCompare3(TIM2, 500);
    TIM_SetCompare4(TIM2, 500);
    Delay_ms(2000);
}

void Motor_Set(double percentage, uint8_t channel) {
    int tmp = (int)percentage;
    switch (channel) {
        case 1:
//			printf("%d\n",tmp);
            TIM_SetCompare1(TIM2, tmp);
            break;
        case 2:
            TIM_SetCompare2(TIM2, tmp);
            break;
        case 3:
            TIM_SetCompare3(TIM2, tmp);
            break;
        case 4:
            TIM_SetCompare4(TIM2, tmp);
            break;
    }
}

void Motor_Test(void) {
    int a = 500;
    while (1) {
        TIM_SetCompare1(TIM2, a);
        TIM_SetCompare2(TIM2, a);
        TIM_SetCompare3(TIM2, a);
        TIM_SetCompare4(TIM2, a);
        printf("a=%d\n", a);
        Delay_ms(200);
        a = a + 5;
        if (a >= 1300) a = 500;
    }
}
