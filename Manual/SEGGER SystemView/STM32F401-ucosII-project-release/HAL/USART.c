#include "USART.h"

static void NVIC_Configuration(void) {
    NVIC_InitTypeDef NVIC_InitStructure;

    // USART6 NVIC(中断控制器) 配置
    NVIC_InitStructure.NVIC_IRQChannel = USART6_IRQn;  //串口1中断通道
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //抢占优先级1
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;         //子优先级1
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  // IRQ通道使能
    NVIC_Init(&NVIC_InitStructure);  //根据指定的参数初始化NVIC寄存器、
}

void USART6_Config(void) {
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;

    /* 使能 USART GPIO 时钟 */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
    /* 使能 USART 时钟 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE);

    /* 连接 PXx 到 USARTx_Tx*/
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_USART6);
    /* 连接 PXx 到 USARTx__Rx*/
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_USART6);

    /* GPIO 初始化 */
    /* 配置推挽模式*/
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    /* 配置上拉*/
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    /* 配置复用模式x*/
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    /* 配置输出速率50MHz*/
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    /* TX 初始化 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    /* RX 初始化 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    /* 配置USART */
    /* 波特率设置 */
    USART_InitStructure.USART_BaudRate = 9600;
    /* 字长(数据位+校验位)：8 */
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    /* 停止位：1 个停止位 */
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    /* 校验位选择：不使用校验 */
    USART_InitStructure.USART_Parity = USART_Parity_No;
    /* 硬件流控制：不使用硬件流 */
    USART_InitStructure.USART_HardwareFlowControl =
        USART_HardwareFlowControl_None;
    /* USART 模式控制：同时使能接收和发送 */
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    /* 完成 USART 初始化配置 */
    USART_Init(USART6, &USART_InitStructure);

    /* 嵌套向量中断控制器 NVIC 配置 */
    NVIC_Configuration();

    /* 使能串口接收中断 */
    USART_ITConfig(USART6, USART_IT_RXNE, ENABLE);

    /* 使能串口 */
    USART_Cmd(USART6, ENABLE);
}

/*****************  发送一个字符 **********************/
static void Usart_SendByte(USART_TypeDef* pUSARTx, uint8_t ch) {
    /* 发送一个字节数据到USARTx */
    USART_SendData(pUSARTx, ch);

    /* 等待发送完毕 */
    while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET)
        ;
}

/*****************  接受一个字符 **********************/
unsigned char USARTGetByte(USART_TypeDef* pUSARTx, unsigned char* GetData) {
    if (USART_GetFlagStatus(pUSARTx, USART_FLAG_RXNE) == RESET) {
        return 0;  //没有收到数据
    }
    *GetData = USART_ReceiveData(pUSARTx);
    return 1;  //收到数据
}

/*****************  发送字符串 **********************/
void Usart_SendString(USART_TypeDef* pUSARTx, char* str) {
    unsigned int k = 0;
    do {
        Usart_SendByte(pUSARTx, *(str + k));
        k++;
    } while (*(str + k) != '\0');

    /* 等待发送完成 */
    while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TC) == RESET) {
    }
}

/*****************  进行收发测试 **********************/
void USARTTest(USART_TypeDef* pUSARTx) {
    unsigned char i = 0;

    while (1) {
        while (USARTGetByte(pUSARTx, &i)) {
            Usart_SendByte(pUSARTx, i);
        }
    }
}
