//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//中景园电子
//店铺地址：http://shop73023976.taobao.com/?spm=2013.1.0.0.M4PqC2
//
//  文 件 名   : oled.c
//  版 本 号   : v2.0
//  作    者   : HuangKai
//  生成日期   : 2014-0101
//  最近修改   : 
//  功能描述   : 0.96寸OLED 接口演示例程(MSP430F5438A系列)
//  驱动IC     :SSD1306
//              说明: 
//              ----------------------------------------------------------------
//              GND    电源地
//              VCC  接3.3v电源
//              D0(SCL)   P86（时钟）
//              D1(SDA)  P84（数据）
//              RES  接P83
//              DC   接P91
//              CS   接P93              
//              ----------------------------------------------------------------
// 修改历史   :
// 日    期   : 
// 作    者   : HuangKai
// 修改内容   : 创建文件
//版权所有，盗版必究。
//Copyright(C) 中景园电子2018/3/16
//All rights reserved
//******************************************************************************/
#include	<io430.h>

#include "oled.h"
#include "bmp.h"
int main( void )
 {	WDTCTL = WDTPW + WDTHOLD;
         u8 t;
		OLED_Init();		//初始化OLED  
		OLED_Clear()  	; 
		// OLED_ShowString(30,0,"OLED TEST");
		OLED_ShowString(16,2,"MSP430F5438A");  
		OLED_ShowString(8,4,"0.96'OLED TEST");  
		OLED_ShowString(0,6,"0123456789ABCDEF");  

		t=' '; 
		OLED_ShowCHinese(0,0,0);//中
		OLED_ShowCHinese(18,0,1);//景
		OLED_ShowCHinese(36,0,2);//园
		OLED_ShowCHinese(54,0,3);//电
		OLED_ShowCHinese(72,0,4);//子
		OLED_ShowCHinese(90,0,5);//科
		OLED_ShowCHinese(108,0,6);//技
		/**/	
		t=' ';
	while(0) 
	{		
		OLED_Clear();
		OLED_ShowCHinese(0,0,0);//中
		OLED_ShowCHinese(18,0,1);//景
		OLED_ShowCHinese(36,0,2);//园
		OLED_ShowCHinese(54,0,3);//电
		OLED_ShowCHinese(72,0,4);//子
		OLED_ShowCHinese(90,0,5);//科
		OLED_ShowCHinese(108,0,6);//技
		OLED_ShowString(0,2,"0.96' OLED TEST");
	//	OLED_ShowString(8,2,"ZHONGJINGYUAN");  
	 	OLED_ShowString(20,4,"2014/05/01");  
		OLED_ShowString(0,6,"ASCII:");  
		OLED_ShowString(63,6,"CODE:");  
		OLED_ShowChar(48,6,t);//显示ASCII字符	   
		t++;
		if(t>'~')t=' ';
		OLED_ShowNum(103,6,t,3,16);//显示ASCII字符的码值 	
			
		
		delay_ms(50);
		OLED_Clear();
		delay_ms(50);
		OLED_DrawBMP(0,0,128,8,BMP1);  //图片显示(图片显示慎用，生成的字表较大，会占用较多空间，FLASH空间8K以下慎用)
		delay_ms(50);
		//OLED_DrawBMP(0,0,128,8,BMP2);
		delay_ms(50);
	}	  
	
}

