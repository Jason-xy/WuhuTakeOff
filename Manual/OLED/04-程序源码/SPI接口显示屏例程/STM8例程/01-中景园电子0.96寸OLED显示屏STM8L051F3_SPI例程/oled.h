//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//中景园电子
//店铺地址：http://shop73023976.taobao.com/?spm=2013.1.0.0.M4PqC2
//
//  文 件 名   : main.c
//  版 本 号   : v2.0
//  作    者   : HuangKai
//  生成日期   : 2011-11-07
//  最近修改   : 
//  功能描述   : OLED 4接口演示例程(STM8L051F3)
//              说明: 
//              ----------------------------------------------------------------
//              GND    电源地
//              VCC  接3.3v电源
//              D0   PC1
//              D1   PC0
//              RES  PB6
//              DC   PB5
//              CS   PB4             
//              ----------------------------------------------------------------
// 修改历史   :
// 日    期   : 
// 作    者   : HuangKai
// 修改内容   : 创建文件
//版权所有，盗版必究。
//Copyright(C) 中景园电子2014/3/16
//All rights reserved
//******************************************************************************/

#ifndef __OLED_H
#define __OLED_H			  	 
#include "stm8l15x.h"//STM8L051/151等系列共用库函数

#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据
#define OLED_MODE 0

/****************时钟*********************/
#define OLED_SCLK_PORT  (GPIOC)
#define OLED_SCLK_PINS  (GPIO_Pin_1)

/****************数据*********************/
#define OLED_SDIN_PORT  (GPIOC)
#define OLED_SDIN_PINS  (GPIO_Pin_0)

/****************复位*********************/
#define OLED_RST_PORT  (GPIOB)
#define OLED_RST_PINS  (GPIO_Pin_7)

/****************数据/命令*********************/
#define OLED_DC_PORT  (GPIOB)
#define OLED_DC_PINS  (GPIO_Pin_6)

/****************片选*********************/
#define OLED_CS_PORT  (GPIOB)
#define OLED_CS_PINS  (GPIO_Pin_5)

#define LED_PORT  (GPIOD)
#define LED_PINS  (GPIO_Pin_0)


#define OLED_SCLK_Clr() GPIO_WriteBit(OLED_SCLK_PORT, OLED_SCLK_PINS,0)
#define OLED_SCLK_Set() GPIO_WriteBit(OLED_SCLK_PORT, OLED_SCLK_PINS,1)

#define OLED_SDIN_Clr() GPIO_WriteBit(OLED_SDIN_PORT, OLED_SDIN_PINS,0)
#define OLED_SDIN_Set() GPIO_WriteBit(OLED_SDIN_PORT, OLED_SDIN_PINS,1)

#define OLED_RST_Clr() GPIO_WriteBit(OLED_RST_PORT, OLED_RST_PINS,0)
#define OLED_RST_Set() GPIO_WriteBit(OLED_RST_PORT, OLED_RST_PINS,1)


#define OLED_DC_Clr() GPIO_WriteBit(OLED_DC_PORT, OLED_DC_PINS,0)
#define OLED_DC_Set() GPIO_WriteBit(OLED_DC_PORT, OLED_DC_PINS,1)


#define OLED_CS_Clr()  GPIO_WriteBit(OLED_CS_PORT, OLED_CS_PINS,0)
#define OLED_CS_Set()  GPIO_WriteBit(OLED_CS_PORT, OLED_CS_PINS,1)


#define LED_Clr()  GPIO_WriteBit(LED_PORT, LED_PINS,0)
#define LED_Set()  GPIO_WriteBit(LED_PORT, LED_PINS,1)



#define SIZE 16
#define XLevelL		0x02
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64	    						  
//-----------------OLED端口定义----------------  					   

void delay_ms(unsigned int ms);
		     

//OLED控制用函数
	    
void OLED_Display_On(void);
void OLED_Display_Off(void);	   							   		    
void OLED_Init(void);
void OLED_Clear(void);
void OLED_DrawPoint(u8 x,u8 y,u8 t);
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot);
void OLED_ShowChar(u8 x,u8 y,u8 chr);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size2);
void OLED_ShowString(u8 x,u8 y, u8 *p);	 
void OLED_Set_Pos(unsigned char x, unsigned char y);
void OLED_ShowCHinese(u8 x,u8 y,u8 no);
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]);
void LED_ON(void);
void LED_OFF(void);
#endif  
	 



