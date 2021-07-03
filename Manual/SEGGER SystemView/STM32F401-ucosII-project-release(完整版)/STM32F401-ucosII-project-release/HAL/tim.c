#include "tim.h"


void TIM2_PWMOUTPUT_Config(void) {
    /*定义一个GPIO_InitTypeDef类型的结构体*/
    GPIO_InitTypeDef GPIO_InitStructure;

    /*开启相关的GPIO外设时钟*/
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    /* 定时器通道引脚复用 */
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource0,
                     GPIO_AF_TIM2);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource1,
                     GPIO_AF_TIM2);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource2,
                     GPIO_AF_TIM2);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource3,
                     GPIO_AF_TIM2);

    /* 定时器通道引脚配置 */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;

    // 开启TIMx_CLK,x[2,3,4,5,12,13,14]
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    /* 累计 TIM_Period个后产生一个更新或者中断*/
    //当定时器从0计数到8399，即为8400次，为一个定时周期
    // TIM_TimeBaseStructure.TIM_Period = 8400-1;
    TIM_TimeBaseStructure.TIM_Period = 10000 - 1;

    // 通用控制定时器时钟源TIMxCLK = HCLK/2=84MHz
    // 设定定时器频率为=TIMxCLK/(TIM_Prescaler+1)=100KHz
    // TIM_TimeBaseStructure.TIM_Prescaler = 840-1;
    // TIM_TimeBaseStructure.TIM_Prescaler = 199;
    TIM_TimeBaseStructure.TIM_Prescaler = 168 - 1;
    // 采样时钟分频
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    // 计数方式
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

    // 初始化定时器TIMx, x[2,3,4,5,12,13,14]
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    /*PWM模式配置*/
    /* PWM1 Mode configuration: Channel1 */
	//配置为PWM模式1
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;  
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 0;
	//当定时器计数值小于CCR1_Val时为高电平
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  
	//使能通道1
    TIM_OC1Init(TIM2, &TIM_OCInitStructure);  
    TIM_OC2Init(TIM2, &TIM_OCInitStructure);  //使能通道2
    TIM_OC3Init(TIM2, &TIM_OCInitStructure);  //使能通道3
    TIM_OC4Init(TIM2, &TIM_OCInitStructure);  //使能通道4

    /*使能通道1234重载*/
    TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);
    TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);
    TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);
    TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);

    // 使能定时器
    TIM_Cmd(TIM2, ENABLE);
}

void TIM3_Cap_Init(u16 arr, u16 psc) {
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    TIM_ICInitTypeDef TIM3_ICInitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);   //时钟TIM3
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);  //使能GPIOA
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);  //使能GPIOA

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;  //速度100MHz
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //复用推免输出
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;      //下拉

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;  // PA6
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_TIM3);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_TIM3);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource0, GPIO_AF_TIM3);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource1, GPIO_AF_TIM3);

    //初始化定时器TIM3

    TIM_TimeBaseStructure.TIM_Period =
        arr;  //设置计数器自动重装值 	通常为：0xFFFF
    TIM_TimeBaseStructure.TIM_Prescaler = psc;  //预分频器，psc = 1M = 1us
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    //	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    ////重复计数设置

    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);  //参数初始化

    TIM_ClearITPendingBit(TIM3, TIM_FLAG_Update);  //清除TIM3更新中断标志

    //初始化TIM3的输入捕获参数
    TIM3_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;  //上升沿捕获
    TIM3_ICInitStructure.TIM_ICSelection =
        TIM_ICSelection_DirectTI;                           //映射到TI1上
    TIM3_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;  //不分频，直接捕获
    TIM3_ICInitStructure.TIM_ICFilter =
        0x00;  // IC1F = 000 配置输入滤波器 不滤波

    TIM3_ICInitStructure.TIM_Channel = TIM_Channel_1;  // cc1s = 01  通道选择
    TIM_ICInit(TIM3, &TIM3_ICInitStructure);
    TIM3_ICInitStructure.TIM_Channel = TIM_Channel_2;  // cc1s = 02  通道选择
    TIM_ICInit(TIM3, &TIM3_ICInitStructure);
    TIM3_ICInitStructure.TIM_Channel = TIM_Channel_3;  // cc1s = 03  通道选择
    TIM_ICInit(TIM3, &TIM3_ICInitStructure);
    TIM3_ICInitStructure.TIM_Channel = TIM_Channel_4;  // cc1s = 04  通道选择
    TIM_ICInit(TIM3, &TIM3_ICInitStructure);

    TIM_ITConfig(TIM3, TIM_IT_CC1 | TIM_IT_Update,
                 ENABLE);  //允许更新中断，允许CC1IE捕获中断
    TIM_ITConfig(TIM3, TIM_IT_CC2 | TIM_IT_Update,
                 ENABLE);  //允许更新中断，允许CC1IE捕获中断
    TIM_ITConfig(TIM3, TIM_IT_CC3 | TIM_IT_Update,
                 ENABLE);  //允许更新中断，允许CC1IE捕获中断
    TIM_ITConfig(TIM3, TIM_IT_CC4 | TIM_IT_Update,
                 ENABLE);  //允许更新中断，允许CC1IE捕获中断
    //中断分组初始化
    TIM_Cmd(TIM3, ENABLE);
	//捕获中断：TIM3_CC_IRQn;
	//TIM3的触发中断：TIM3_TRG_COM_TIM31_IRQn
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  
    //优先级配置                
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
