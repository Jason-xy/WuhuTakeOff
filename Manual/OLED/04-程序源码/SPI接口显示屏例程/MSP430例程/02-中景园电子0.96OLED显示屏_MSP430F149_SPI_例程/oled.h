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
//  ��������   : 0.96��OLED �ӿ���ʾ����(MSP430F149ϵ��)
//  ����IC     :SSD1309
//              ˵��: 
//              ----------------------------------------------------------------
//              GND    ��Դ��
//              VCC  ��3.3v��Դ
//              D0   P54��ʱ�ӣ�
//              D1   P53�����ݣ�
//              RES  ��P52
//              DC   ��P51
//              CS   ��P50               
//              ----------------------------------------------------------------
// �޸���ʷ   :
// ��    ��   : 
// ��    ��   : HuangKai
// �޸�����   : �����ļ�
//��Ȩ���У�����ؾ���
//Copyright(C) �о�԰����2014/3/16
//All rights reserved
//******************************************************************************/
#ifndef __OLED_H
#define __OLED_H			  	 
#include	<io430.h>
#include	<in430.h>
#include	<intrinsics.h> 
#define  u8 unsigned char 
#define  u32 unsigned int 
#define OLED_CMD  0	//д����
#define OLED_DATA 1	//д����



#define		Set_Bit(val, bitn)		(val |= (/*1 <<*/(bitn)))		//�m�Y�쬰 1
#define		Clr_Bit(val, bitn)		(val &= ~(/*1<<*/(bitn)))		//�m�Y�쬰 0
#define		Get_Bit(val, bitn)		(val & (1<<(bitn)) )		//���o�Y�쪺��
//----------------------------------------------------------------------------------
//OLED SSD1306 SPI  ʱ��D0
//#define		OLED_SSD1306_SCLK_DIR			(P1DIR)
//#define		OLED_SSD1306_SCLK_OUT			(P1OUT)
#define		OLED_SSD1306_SCLK_PIN_NUM		(BIT4)

#define		OLED_SSD1306_SCLK_IO_INIT		(Set_Bit(P5DIR,OLED_SSD1306_SCLK_PIN_NUM))
#define		OLED_SCLK_Set()			        (Set_Bit(P5OUT,OLED_SSD1306_SCLK_PIN_NUM))
#define		OLED_SCLK_Clr()				(Clr_Bit(P5OUT,OLED_SSD1306_SCLK_PIN_NUM))

//----------------------------------------------------------------------------------
//OLED SSD1306 SPI ����D1
//#define		OLED_SSD1306_SDIN_DIR			(P1DIR)
//#define		OLED_SSD1306_SDIN_OUT			(P1OUT)
#define		OLED_SSD1306_SDIN_PIN_NUM		(BIT3)

#define		OLED_SSD1306_SDIN_IO_INIT		(Set_Bit(P5DIR,OLED_SSD1306_SDIN_PIN_NUM))
#define		OLED_SDIN_Set()				(Set_Bit(P5OUT,OLED_SSD1306_SDIN_PIN_NUM))
#define		OLED_SDIN_Clr()			        (Clr_Bit(P5OUT,OLED_SSD1306_SDIN_PIN_NUM))

//----------------------------------------------------------------------------------
//OLED SSD1306 ��λ/RES
//#define		OLED_SSD1306_RST_DIR			(P1DIR)
//#define		OLED_SSD1306_RST_OUT			(P1OUT)
#define		OLED_SSD1306_RST_PIN_NUM		(BIT2)

#define		OLED_SSD1306_RST_IO_INIT		(Set_Bit(P5DIR,OLED_SSD1306_RST_PIN_NUM))
#define		OLED_RST_Set()				(Set_Bit(P5OUT,OLED_SSD1306_RST_PIN_NUM))
#define		OLED_RST_Clr()				(Clr_Bit(P5OUT,OLED_SSD1306_RST_PIN_NUM))

//----------------------------------------------------------------------------------
//OLED SSD1306 ����/����DC
//#define		OLED_SSD1306_DC_DIR				(P1DIR)
//#define		OLED_SSD1306_DC_OUT				(P1OUT)
#define		OLED_SSD1306_DC_PIN_NUM			(BIT1)

#define		OLED_SSD1306_DC_IO_INIT			(Set_Bit(P5DIR,OLED_SSD1306_DC_PIN_NUM))
#define		OLED_DC_Set()				(Set_Bit(P5OUT,OLED_SSD1306_DC_PIN_NUM))
#define		OLED_DC_Clr() 				(Clr_Bit(P5OUT,OLED_SSD1306_DC_PIN_NUM))

//----------------------------------------------------------------------------------
//OLED SSD1306 ��ƬѡCS
//#define		OLED_SSD1306_CE_DIR				(P1DIR)
//#define		OLED_SSD1306_CE_OUT				(P1OUT)
#define		OLED_SSD1306_CE_PIN_NUM			(BIT0)

#define		OLED_SSD1306_CE_IO_INIT			(Set_Bit(P5DIR,OLED_SSD1306_CE_PIN_NUM))
#define		OLED_CS_Set()				(Set_Bit(P5OUT,OLED_SSD1306_CE_PIN_NUM))
#define		OLED_CS_Clr()				(Clr_Bit(P5OUT,OLED_SSD1306_CE_PIN_NUM))
//----------------------------------------------------------------------------------
//������LED��

#define		LED_PIN_NUM			(BIT0)

#define		LED_IO_INIT		        	(Set_Bit(P2DIR,LED_PIN_NUM))
#define		LED_Set()				(Set_Bit(P2OUT,LED_PIN_NUM))
#define		LED_Clr()				(Clr_Bit(P2OUT,LED_PIN_NUM))
//----------------------------------------------------------------------------------

#define SIZE 16
#define XLevelL		0x02
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64	    						  
//-----------------OLED�˿ڶ���----------------  					   

void delay_ms(unsigned int ms);


 		     

//OLED�����ú���
void OLED_WR_Byte(u8 dat,u8 cmd);	    
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
#endif  
	 



