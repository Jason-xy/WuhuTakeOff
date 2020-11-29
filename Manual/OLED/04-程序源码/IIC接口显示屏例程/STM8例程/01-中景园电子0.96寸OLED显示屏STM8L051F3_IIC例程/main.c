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
//              SCL  PC1
//              SDA   PC0
//              RES  PB6（如果直接选用4针iic接口的屏RES信号可以不考虑，直接忽略）
         
//              ----------------------------------------------------------------
// 修改历史   :
// 日    期   : 
// 作    者   : HuangKai
// 修改内容   : 创建文件
//版权所有，盗版必究。
//Copyright(C) 中景园电子2014/3/16
//All rights reserved
//******************************************************************************/
#include "stm8l15x.h"//STM8L051/151等系列共用库函数
#include "oled.h"
#include "bmp.h"
/* Private defines -----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/


void Delay(uint16_t nCount)
{
  /* Decrement nCount value */
  while (nCount != 0)
  {   
    nCount--;
  }
}
void main(void)
{	u8 t;
		//delay_init();	    	 //?óê±oˉêy3?ê??ˉ	  
	//	NVIC_Configuration(); 	 //éè??NVIC?D??·?×é2:2???à??ó??è??￡?2???ìó|ó??è?? 	LED_Init();			     //LED???ú3?ê??ˉ
		OLED_Init();			//3?ê??ˉOLED  
		OLED_Clear()  	; 
		
		 OLED_ShowString(30,0,"OLED TEST");
		OLED_ShowString(8,2,"ZHONGJINGYUAN");  
		OLED_ShowString(20,4,"2014/05/01");  
		OLED_ShowString(0,6,"ASCII:");  
		OLED_ShowString(63,6,"CODE:");  
		t=' '; 
		OLED_ShowCHinese(0,0,0);//?D
		OLED_ShowCHinese(18,0,1);//?°
		OLED_ShowCHinese(36,0,2);//?°
		OLED_ShowCHinese(54,0,3);//μ?
		OLED_ShowCHinese(72,0,4);//×ó
		OLED_ShowCHinese(90,0,5);//??
		OLED_ShowCHinese(108,0,6);//??
                
                delay_ms(50);
       
		OLED_Clear();
		delay_ms(50);
              
			
		t=' ';
	while(1) 
	{		LED_Clr();
          //OLED_Clr() ;
		OLED_Clear();

		OLED_ShowCHinese(0,0,0);//?D
		OLED_ShowCHinese(18,0,1);//?°
		OLED_ShowCHinese(36,0,2);//?°
		OLED_ShowCHinese(54,0,3);//μ?
		OLED_ShowCHinese(72,0,4);//×ó
		OLED_ShowCHinese(90,0,5);//??
		OLED_ShowCHinese(108,0,6);//??
                
		OLED_ShowString(0,2,"0.96' OLED TEST");
	//	OLED_ShowString(8,2,"ZHONGJINGYUAN");  
	 	OLED_ShowString(20,4,"2014/05/01");  
		OLED_ShowString(0,6,"ASCII:");  
		OLED_ShowString(63,6,"CODE:");  
		OLED_ShowChar(48,6,t);//??ê?ASCII×?·?	   
		t++;
		if(t>'~')t=' ';
		OLED_ShowNum(103,6,t,3,16);//??ê?ASCII×?·?μ????μ 	

		
		delay_ms(80);
                LED_Set();
			delay_ms(80);
		
	
           
		OLED_Clear();
		delay_ms(50);
		//OLED_DrawBMP(0,0,128,8,BMP1);  //í?????ê?(í?????ê?é÷ó?￡?éú3éμ?×?±í??′ó￡??á??ó????à????￡?FLASH????8Kò???é÷ó?)
		//delay_ms(500);
		//OLED_DrawBMP(0,0,128,8,BMP2);
		//delay_ms(500);
	}	  
	
}

#ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval : None
  */
void assert_failed(u8* file, u32 line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
