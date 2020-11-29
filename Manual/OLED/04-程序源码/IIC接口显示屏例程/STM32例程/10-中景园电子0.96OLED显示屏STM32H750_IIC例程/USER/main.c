//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//中景园电子
//店铺地址：http://shop73023976.taobao.com
//
//  文 件 名   : main.c
//  版 本 号   : v2.0
//  作    者   : HuangKai
//  生成日期   : 2018-10-31
//  功能描述   : 
//              说明: 
//              ----------------------------------------------------------------
//              GND  电源地
//              VCC  3.3v电源
//              SLC  PA5
//              SDA  PA7
//              RES  PC4  //SPI模块改IIC模块需要使用此引脚，IIC模块用户请忽略
//              ----------------------------------------------------------------
// 作    者   : HuangKai
// 修改内容   : 创建文件
//版权所有，盗版必究。
//Copyright(C) 中景园电子2018-10-31
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
	Cache_Enable();                 //打开L1-Cache
	HAL_Init();				       //初始化HAL库
	Stm32_Clock_Init(160,5,2,4);    //设置时钟,400Mhz
	delay_init(400);				//延时初始化
	uart_init(115200);		  //串口初始化
	LED_Init();						  //初始化LED
	OLED_Init();
	while(1)
	{
		OLED_ShowPicture(0,0,128,8,BMP1);
		delay_ms(500);
		OLED_Clear();
		OLED_ShowChinese(0,0,0,16);//中
		OLED_ShowChinese(18,0,1,16);//景
		OLED_ShowChinese(36,0,2,16);//园
		OLED_ShowChinese(54,0,3,16);//电
		OLED_ShowChinese(72,0,4,16);//子
		OLED_ShowChinese(90,0,5,16);//科
		OLED_ShowChinese(108,0,6,16);//技
		OLED_ShowString(8,16,"ZHONGJINGYUAN",16);
		OLED_ShowString(20,32,"2014/05/01",16);
		OLED_ShowString(0,48,"ASCII:",16);  
		OLED_ShowString(63,48,"CODE:",16);
		OLED_ShowChar(48,48,t,16);//显示ASCII字符	   
		t++;
		if(t>'~')t=' ';
		OLED_ShowNum(103,48,t,3,16);
		OLED_Refresh();
		delay_ms(500);
		OLED_Clear();
		OLED_ShowChinese(0,0,0,16);  //16*16 中
	  OLED_ShowChinese(16,0,0,24); //24*24 中
		OLED_ShowChinese(24,20,0,32);//32*32 中
	  OLED_ShowChinese(64,0,0,64); //64*64 中
		OLED_Refresh();
	  delay_ms(500);
  	OLED_Clear();
		OLED_ShowString(0,0,"ABC",12);//6*12 “ABC”
	  OLED_ShowString(0,12,"ABC",16);//8*16 “ABC”
		OLED_ShowString(0,28,"ABC",24);//12*24 “ABC”
	  OLED_Refresh();
		delay_ms(500);
		OLED_ScrollDisplay(11,4);
	}
}

