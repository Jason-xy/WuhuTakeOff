//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//�о�԰����
//���̵�ַ��http://shop73023976.taobao.com/?spm=2013.1.0.0.M4PqC2
//
//  �� �� ��   : main.c
//  �� �� ��   : v2.0
//  ��    ��   : HuangKai
//  ��������   : 2011-11-07
//  ����޸�   : 
//  ��������   : OLED 4�ӿ���ʾ����(STM8L051F3)
//              ˵��: 
//              ----------------------------------------------------------------
//              GND    ��Դ��
//              VCC  ��3.3v��Դ
//              SCL  PC1
//              SDA   PC0
//              RES  PB6�����ֱ��ѡ��4��iic�ӿڵ���RES�źſ��Բ����ǣ�ֱ�Ӻ��ԣ�
         
//              ----------------------------------------------------------------
// �޸���ʷ   :
// ��    ��   : 
// ��    ��   : HuangKai
// �޸�����   : �����ļ�
//��Ȩ���У�����ؾ���
//Copyright(C) �о�԰����2014/3/16
//All rights reserved
//******************************************************************************/
#include "stm8l15x.h"//STM8L051/151��ϵ�й��ÿ⺯��
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
		//delay_init();	    	 //?������o����y3?��??��	  
	//	NVIC_Configuration(); 	 //����??NVIC?D??��?����2:2???��??��??��??��?2???����|��??��?? 	LED_Init();			     //LED???��3?��??��
		OLED_Init();			//3?��??��OLED  
		OLED_Clear()  	; 
		
		 OLED_ShowString(30,0,"OLED TEST");
		OLED_ShowString(8,2,"ZHONGJINGYUAN");  
		OLED_ShowString(20,4,"2014/05/01");  
		OLED_ShowString(0,6,"ASCII:");  
		OLED_ShowString(63,6,"CODE:");  
		t=' '; 
		OLED_ShowCHinese(0,0,0);//?D
		OLED_ShowCHinese(18,0,1);//?��
		OLED_ShowCHinese(36,0,2);//?��
		OLED_ShowCHinese(54,0,3);//��?
		OLED_ShowCHinese(72,0,4);//����
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
		OLED_ShowCHinese(18,0,1);//?��
		OLED_ShowCHinese(36,0,2);//?��
		OLED_ShowCHinese(54,0,3);//��?
		OLED_ShowCHinese(72,0,4);//����
		OLED_ShowCHinese(90,0,5);//??
		OLED_ShowCHinese(108,0,6);//??
                
		OLED_ShowString(0,2,"0.96' OLED TEST");
	//	OLED_ShowString(8,2,"ZHONGJINGYUAN");  
	 	OLED_ShowString(20,4,"2014/05/01");  
		OLED_ShowString(0,6,"ASCII:");  
		OLED_ShowString(63,6,"CODE:");  
		OLED_ShowChar(48,6,t);//??��?ASCII��?��?	   
		t++;
		if(t>'~')t=' ';
		OLED_ShowNum(103,6,t,3,16);//??��?ASCII��?��?��????�� 	

		
		delay_ms(80);
                LED_Set();
			delay_ms(80);
		
	
           
		OLED_Clear();
		delay_ms(50);
		//OLED_DrawBMP(0,0,128,8,BMP1);  //��?????��?(��?????��?���¨�?��?����3����?��?����??�䨮��??��??��????��????��?FLASH????8K��???���¨�?)
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
