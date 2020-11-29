//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//中景园电子
//店铺地址：http://shop73023976.taobao.com/?spm=2013.1.0.0.M4PqC2
//
//  文 件 名   : main.c
//  版 本 号   : v2.0
//  作    者   : HuangKai
//  生成日期   : 2014-0101
//  最近修改   : 
//  功能描述   : OLED 4接口演示例程(51系列)
//            说明: 显示屏插在插座上的时候，一定要注意GND,VCC对应好；不要插错了；否则容易损坏
//               程序运行以后开发板上的D6 LED灯闪烁，此时显示屏会有画面不停的刷新
//              ----------------------------------------------------------------
//              GND    电源地
//              VCC  接5V或3.3v电源
//              SCL=   P1^4（SCL）
//              SDA   P1^3（SDA）

// 修改历史   :
// 日    期   : 
// 作    者   : HuangKai
// 修改内容   : 创建文件
//版权所有，盗版必究。
//Copyright(C) 中景园电子2014/3/16
//All rights reserved
//******************************************************************************/
#include "REG51.h"
#include "oled.h"
#include "bmp.h"

 int main(void)
 {	u8 t;
			OLED_Init();			//初始化OLED  
		OLED_Clear()  	; 
	
		t=' ';
		OLED_ShowCHinese(0,0,0);//中
				OLED_ShowCHinese(18,0,1);//景
		OLED_ShowCHinese(36,0,2);//园
		OLED_ShowCHinese(54,0,3);//电
		OLED_ShowCHinese(72,0,4);//子
		OLED_ShowCHinese(90,0,5);//科
		OLED_ShowCHinese(108,0,6);//技
	 		OLED_Clear();
		OLED_ShowCHinese(0,0,0);//中
		OLED_ShowCHinese(18,0,1);//景
		OLED_ShowCHinese(36,0,2);//园
		OLED_ShowCHinese(54,0,3);//电
		OLED_ShowCHinese(72,0,4);//子
		OLED_ShowCHinese(90,0,5);//科
		OLED_ShowCHinese(108,0,6);//技
		OLED_ShowString(6,3,"0.96' OLED TEST",16);
		//OLED_ShowString(8,2,"ZHONGJINGYUAN");  
	 //	OLED_ShowString(20,4,"2014/05/01");  
		OLED_ShowString(0,6,"ASCII:",16);  
		OLED_ShowString(63,6,"CODE:",16); 
OLED_ShowChar(48,6,t,16);//显示ASCII字符			
	while(1) 
	{		

		   
		t++;
		if(t>'~')t=' ';
	LED6=0; //发光二极管
		OLED_ShowNum(103,6,t,3,16);//显示ASCII字符的码值 	
			delay_ms(20);
			LED6=1;
			delay_ms(20);

	}	  
	
}
	