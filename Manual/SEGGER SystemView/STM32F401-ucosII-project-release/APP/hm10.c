#include "hm10.h"

u8 hm_flag = '0';

void HM10_Config(void) { USART6_Config(); }

void HM10_Test(void) { USARTTest(USART6); }

///重定向c库函数printf到串口，重定向后可使用printf函数
int fputc(int ch, FILE *f) {
    /* 发送一个字节数据到串口 */
    USART_SendData(USART6, (uint8_t)ch);

    /* 等待发送完毕 */
    while (USART_GetFlagStatus(USART6, USART_FLAG_TXE) == RESET)
        ;

    return (ch);
}

void HM10_IRQ_IRQHandler(void){
	if (USART_GetITStatus(USART6, USART_IT_RXNE) != RESET) {
		hm_flag = USART_ReceiveData(USART6);
	if (hm_flag == '0') {
		printf("Ble is printing the data of mpu6050\n");
	} else if (hm_flag == '1') {
		printf("Ble is printing the data of reciver\n");
	} else if (hm_flag == '2') {
		
	} else {
		printf("Wrong input, please enter 0 or 1, without CR or LF\n");
	}
}
}

