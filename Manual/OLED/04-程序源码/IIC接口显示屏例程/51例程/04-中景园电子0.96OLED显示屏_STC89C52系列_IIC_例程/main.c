//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//�о�԰����
//���̵�ַ��http://shop73023976.taobao.com/?spm=2013.1.0.0.M4PqC2
//
//  �� �� ��   : main.c
//  �� �� ��   : v2.0
//  ��    ��   : HuangKai
//  ��������   : 2014-0101
//  ����޸�   : 
//  ��������   : OLED 4�ӿ���ʾ����(51ϵ��)
//            ˵��: ��ʾ�����ڲ����ϵ�ʱ��һ��Ҫע��GND,VCC��Ӧ�ã���Ҫ����ˣ�����������
//               ���������Ժ󿪷����ϵ�D6 LED����˸����ʱ��ʾ�����л��治ͣ��ˢ��
//              ----------------------------------------------------------------
//              GND    ��Դ��
//              VCC  ��5V��3.3v��Դ
//              SCL=   P1^4��SCL��
//              SDA   P1^3��SDA��

// �޸���ʷ   :
// ��    ��   : 
// ��    ��   : HuangKai
// �޸�����   : �����ļ�
//��Ȩ���У�����ؾ���
//Copyright(C) �о�԰����2014/3/16
//All rights reserved
//******************************************************************************/
#include "REG51.h"
#include "oled.h"
#include "bmp.h"

 int main(void)
 {	u8 t;
			OLED_Init();			//��ʼ��OLED  
		OLED_Clear()  	; 
	
		t=' ';
		OLED_ShowCHinese(0,0,0);//��
				OLED_ShowCHinese(18,0,1);//��
		OLED_ShowCHinese(36,0,2);//԰
		OLED_ShowCHinese(54,0,3);//��
		OLED_ShowCHinese(72,0,4);//��
		OLED_ShowCHinese(90,0,5);//��
		OLED_ShowCHinese(108,0,6);//��
	 		OLED_Clear();
		OLED_ShowCHinese(0,0,0);//��
		OLED_ShowCHinese(18,0,1);//��
		OLED_ShowCHinese(36,0,2);//԰
		OLED_ShowCHinese(54,0,3);//��
		OLED_ShowCHinese(72,0,4);//��
		OLED_ShowCHinese(90,0,5);//��
		OLED_ShowCHinese(108,0,6);//��
		OLED_ShowString(6,3,"0.96' OLED TEST",16);
		//OLED_ShowString(8,2,"ZHONGJINGYUAN");  
	 //	OLED_ShowString(20,4,"2014/05/01");  
		OLED_ShowString(0,6,"ASCII:",16);  
		OLED_ShowString(63,6,"CODE:",16); 
OLED_ShowChar(48,6,t,16);//��ʾASCII�ַ�			
	while(1) 
	{		

		   
		t++;
		if(t>'~')t=' ';
	LED6=0; //���������
		OLED_ShowNum(103,6,t,3,16);//��ʾASCII�ַ�����ֵ 	
			delay_ms(20);
			LED6=1;
			delay_ms(20);

	}	  
	
}
	