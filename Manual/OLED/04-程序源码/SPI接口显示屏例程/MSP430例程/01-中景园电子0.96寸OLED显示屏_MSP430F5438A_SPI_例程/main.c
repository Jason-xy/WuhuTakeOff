//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//�о�԰����
//���̵�ַ��http://shop73023976.taobao.com/?spm=2013.1.0.0.M4PqC2
//
//  �� �� ��   : oled.c
//  �� �� ��   : v2.0
//  ��    ��   : HuangKai
//  ��������   : 2014-0101
//  ����޸�   : 
//  ��������   : 0.96��OLED �ӿ���ʾ����(MSP430F5438Aϵ��)
//  ����IC     :SSD1306
//              ˵��: 
//              ----------------------------------------------------------------
//              GND    ��Դ��
//              VCC  ��3.3v��Դ
//              D0(SCL)   P86��ʱ�ӣ�
//              D1(SDA)  P84�����ݣ�
//              RES  ��P83
//              DC   ��P91
//              CS   ��P93              
//              ----------------------------------------------------------------
// �޸���ʷ   :
// ��    ��   : 
// ��    ��   : HuangKai
// �޸�����   : �����ļ�
//��Ȩ���У�����ؾ���
//Copyright(C) �о�԰����2018/3/16
//All rights reserved
//******************************************************************************/
#include	<io430.h>

#include "oled.h"
#include "bmp.h"
int main( void )
 {	WDTCTL = WDTPW + WDTHOLD;
         u8 t;
		OLED_Init();		//��ʼ��OLED  
		OLED_Clear()  	; 
		// OLED_ShowString(30,0,"OLED TEST");
		OLED_ShowString(16,2,"MSP430F5438A");  
		OLED_ShowString(8,4,"0.96'OLED TEST");  
		OLED_ShowString(0,6,"0123456789ABCDEF");  

		t=' '; 
		OLED_ShowCHinese(0,0,0);//��
		OLED_ShowCHinese(18,0,1);//��
		OLED_ShowCHinese(36,0,2);//԰
		OLED_ShowCHinese(54,0,3);//��
		OLED_ShowCHinese(72,0,4);//��
		OLED_ShowCHinese(90,0,5);//��
		OLED_ShowCHinese(108,0,6);//��
		/**/	
		t=' ';
	while(0) 
	{		
		OLED_Clear();
		OLED_ShowCHinese(0,0,0);//��
		OLED_ShowCHinese(18,0,1);//��
		OLED_ShowCHinese(36,0,2);//԰
		OLED_ShowCHinese(54,0,3);//��
		OLED_ShowCHinese(72,0,4);//��
		OLED_ShowCHinese(90,0,5);//��
		OLED_ShowCHinese(108,0,6);//��
		OLED_ShowString(0,2,"0.96' OLED TEST");
	//	OLED_ShowString(8,2,"ZHONGJINGYUAN");  
	 	OLED_ShowString(20,4,"2014/05/01");  
		OLED_ShowString(0,6,"ASCII:");  
		OLED_ShowString(63,6,"CODE:");  
		OLED_ShowChar(48,6,t);//��ʾASCII�ַ�	   
		t++;
		if(t>'~')t=' ';
		OLED_ShowNum(103,6,t,3,16);//��ʾASCII�ַ�����ֵ 	
			
		
		delay_ms(50);
		OLED_Clear();
		delay_ms(50);
		OLED_DrawBMP(0,0,128,8,BMP1);  //ͼƬ��ʾ(ͼƬ��ʾ���ã����ɵ��ֱ�ϴ󣬻�ռ�ý϶�ռ䣬FLASH�ռ�8K��������)
		delay_ms(50);
		//OLED_DrawBMP(0,0,128,8,BMP2);
		delay_ms(50);
	}	  
	
}

