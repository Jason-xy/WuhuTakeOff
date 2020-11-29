//////////////////////////////////////////////////////////////////////////////////	 
//±¾³ÌĞòÖ»¹©Ñ§Ï°Ê¹ÓÃ£¬Î´¾­×÷ÕßĞí¿É£¬²»µÃÓÃÓÚÆäËüÈÎºÎÓÃÍ¾
//ÖĞ¾°Ô°µç×Ó
//µêÆÌµØÖ·£ºhttp://shop73023976.taobao.com/?spm=2013.1.0.0.M4PqC2
//
//  ÎÄ ¼ş Ãû   : main.c
//  °æ ±¾ ºÅ   : v2.0
//  ×÷    Õß   : HuangKai
//  Éú³ÉÈÕÆÚ   : 2014-0101
//  ×î½üĞŞ¸Ä   : 
//  ¹¦ÄÜÃèÊö   : OLED 4½Ó¿ÚÑİÊ¾Àı³Ì(51ÏµÁĞ)
//            ËµÃ÷: ÏÔÊ¾ÆÁ²åÔÚ²å×ùÉÏµÄÊ±ºò£¬Ò»¶¨Òª×¢ÒâGND,VCC¶ÔÓ¦ºÃ£»²»Òª²å´íÁË£»·ñÔòÈİÒ×Ëğ»µ
//               ³ÌĞòÔËĞĞÒÔºó¿ª·¢°åÉÏµÄD6 LEDµÆÉÁË¸£¬´ËÊ±ÏÔÊ¾ÆÁ»áÓĞ»­Ãæ²»Í£µÄË¢ĞÂ
//              ----------------------------------------------------------------
//              GND    µçÔ´µØ
//              VCC  ½Ó5V»ò3.3vµçÔ´
//              SCL=   P1^4£¨SCL£©
//              SDA   P1^3£¨SDA£©

// ĞŞ¸ÄÀúÊ·   :
// ÈÕ    ÆÚ   : 
// ×÷    Õß   : HuangKai
// ĞŞ¸ÄÄÚÈİ   : ´´½¨ÎÄ¼ş
//°æÈ¨ËùÓĞ£¬µÁ°æ±Ø¾¿¡£
//Copyright(C) ÖĞ¾°Ô°µç×Ó2014/3/16
//All rights reserved
//******************************************************************************/
#include "REG51.h"
#ifndef __OLED_H
#define __OLED_H			  	 
//#include "sys.h"
//#include "stdlib.h"	  
#define  u8 unsigned char 
#define  u32 unsigned int 
#define OLED_CMD  0	//Ğ´ÃüÁî
#define OLED_DATA 1	//Ğ´Êı¾İ
#define OLED_MODE 0


sbit OLED_SCL=P1^4;//Ê±ÖÓ D0£¨SCLK£
sbit OLED_SDIN=P1^3;//D1£¨MOSI£© Êı¾İ
sbit LED6=P3^3; //·¢¹â¶ş¼«¹Ü



#define OLED_SCLK_Clr() OLED_SCL=0
#define OLED_SCLK_Set() OLED_SCL=1

#define OLED_SDIN_Clr() OLED_SDIN=0
#define OLED_SDIN_Set() OLED_SDIN=1

#define SIZE 16
#define XLevelL		0x02
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64	    						  
//-----------------OLED¶Ë¿Ú¶¨Òå----------------  					   

void delay_ms(unsigned int ms);


//OLED¿ØÖÆÓÃº¯Êı
void OLED_WR_Byte(unsigned dat,unsigned cmd);  
void OLED_Display_On(void);
void OLED_Display_Off(void);	   							   		    
void OLED_Init(void);
void OLED_Clear(void);
void OLED_DrawPoint(u8 x,u8 y,u8 t);
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot);
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 Char_Size);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size);
void OLED_ShowString(u8 x,u8 y, u8 *p,u8 Char_Size);	 
void OLED_Set_Pos(unsigned char x, unsigned char y);
void OLED_ShowCHinese(u8 x,u8 y,u8 no);
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]);
void Delay_50ms(unsigned int Del_50ms);
void Delay_1ms(unsigned int Del_1ms);
void fill_picture(unsigned char fill_Data);
void Picture();
void IIC_Start();
void IIC_Stop();
void Write_IIC_Command(unsigned char IIC_Command);
void Write_IIC_Data(unsigned char IIC_Data);
void Write_IIC_Byte(unsigned char IIC_Byte);
void IIC_Wait_Ack();
#endif  
	 



