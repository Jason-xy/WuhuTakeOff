#include "LED.h"

#include "stm32f4xx.h"
#include "sysTick.h"
void LED_Config(void) {
    /*定义一个GPIO_InitTypeDef类型的结构体*/
    GPIO_InitTypeDef GPIO_InitStructure;

    /*开启LED相关的GPIO外设时钟*/
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

    /*选择要控制的GPIO引脚*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;

    /*设置引脚模式为输出模式*/
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;

    /*设置引脚的输出类型为推挽输出*/
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;

    /*设置引脚为上拉模式*/
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

    /*设置引脚速率为2MHz */
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;

    /*调用库函数，使用上面配置的GPIO_InitStructure初始化GPIO*/
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    while (1) {
        GPIO_SetBits(GPIOA, GPIO_Pin_5);
        Delay_ms(32);
        GPIO_ResetBits(GPIOA, GPIO_Pin_5);
        Delay_ms(32);
    }
}
