//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//�о�԰����
//���̵�ַ��http://shop73023976.taobao.com
//
//  �� �� ��   : main.c
//  �� �� ��   : v2.0
//  ��    ��   : HuangKai
//  ��������   : 2018-10-31
//  ��������   : 
//              ˵��: 
//              ----------------------------------------------------------------
//              GND  ��Դ��
//              VCC  3.3v��Դ
//              SLC  PA5
//              SDA  PA7
//              RES  PC4  //SPIģ���IICģ����Ҫʹ�ô����ţ�IICģ���û������
//              ----------------------------------------------------------------
// ��    ��   : HuangKai
// �޸�����   : �����ļ�
//��Ȩ���У�����ؾ���
//Copyright(C) �о�԰����2018-10-31
//All rights reserved
//******************************************************************************/
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "oled.h"
#include "bmp.h"
int main(void)
{
	u8 t;
	Cache_Enable();                 //��L1-Cache
	HAL_Init();				       //��ʼ��HAL��
	Stm32_Clock_Init(160,5,2,4);    //����ʱ��,400Mhz
	delay_init(400);				//��ʱ��ʼ��
	uart_init(115200);		  //���ڳ�ʼ��
	LED_Init();						  //��ʼ��LED
	OLED_Init();
	while(1)
	{
		OLED_ShowPicture(0,0,128,8,BMP1);
		delay_ms(500);
		OLED_Clear();
		OLED_ShowChinese(0,0,0,16);//��
		OLED_ShowChinese(18,0,1,16);//��
		OLED_ShowChinese(36,0,2,16);//԰
		OLED_ShowChinese(54,0,3,16);//��
		OLED_ShowChinese(72,0,4,16);//��
		OLED_ShowChinese(90,0,5,16);//��
		OLED_ShowChinese(108,0,6,16);//��
		OLED_ShowString(8,16,"ZHONGJINGYUAN",16);
		OLED_ShowString(20,32,"2014/05/01",16);
		OLED_ShowString(0,48,"ASCII:",16);  
		OLED_ShowString(63,48,"CODE:",16);
		OLED_ShowChar(48,48,t,16);//��ʾASCII�ַ�	   
		t++;
		if(t>'~')t=' ';
		OLED_ShowNum(103,48,t,3,16);
		OLED_Refresh();
		delay_ms(500);
		OLED_Clear();
		OLED_ShowChinese(0,0,0,16);  //16*16 ��
	  OLED_ShowChinese(16,0,0,24); //24*24 ��
		OLED_ShowChinese(24,20,0,32);//32*32 ��
	  OLED_ShowChinese(64,0,0,64); //64*64 ��
		OLED_Refresh();
	  delay_ms(500);
  	OLED_Clear();
		OLED_ShowString(0,0,"ABC",12);//6*12 ��ABC��
	  OLED_ShowString(0,12,"ABC",16);//8*16 ��ABC��
		OLED_ShowString(0,28,"ABC",24);//12*24 ��ABC��
	  OLED_Refresh();
		delay_ms(500);
		OLED_ScrollDisplay(11,4);
	}
}

