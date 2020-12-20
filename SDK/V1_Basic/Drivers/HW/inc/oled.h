/**
  ******************************************************************************
  * 文件名程: oled.h
  * 作    者: Jason_xy
  * 个人博客：https://jason-xy.cn
  * 版    本: V1.0.1
  * 编写日期: 2020-9-18
  * 功    能: OLED模块驱动
  ******************************************************************************
  * 说明：
  * 1.需要先完成I2C的初始化。
  * 
  * 功能：
  * 1.OLED初始化。
  * 2.显存写操作。
  * 3.数字定点显示。
  * 4.字符定点显示。
  * 5.字符串定点显示。
  * 6.汉字显示。
  * 7.bmp位图显示。
  * 8.飞控界面绘制。
  * 9.自定义数据格式显示。
  * 
  * 更新：
  * 2020-12-16
  * 1.修改ShowNum支持int类型带符号数字输出。
  ******************************************************************************
  */

#ifndef __OLED_H_
#define __OLED_H_	

#include "stdint.h"
#include "i2c.h"



	
void WriteCmd(void);
//向设备写控制命令
void OLED_WR_CMD(uint8_t cmd);
//向设备写数据
void OLED_WR_DATA(uint8_t data);
//初始化oled屏幕
void OLED_Init(void);
//清屏
void OLED_Clear(void);
//清行
void OLED_Clearrow(uint8_t i);
//开启OLED显示    
void OLED_Display_On(void);
//关闭OLED显示     
void OLED_Display_Off(void);
//设置光标
void OLED_Set_Pos(uint8_t x, uint8_t y);

void OLED_On(void);
	
//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~63
//mode:0,反白显示;1,正常显示				 
//size:选择字体 16/12 
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t Char_Size);

 //显示2个数字
//x,y :起点坐标	 
//len :数字的位数
//size:字体大小
//mode:模式	0,填充模式;1,叠加模式
//num:数值(0~4294967295);	 		  
void OLED_ShowNum(uint8_t x,uint8_t y,int num,uint8_t len,uint8_t size2);

//显示一个字符号串
void OLED_ShowString(uint8_t x,uint8_t y,uint8_t *chr,uint8_t Char_Size);

//显示汉字
//hzk 用取模软件得出的数组
void OLED_ShowCHinese(uint8_t x,uint8_t y,uint8_t no);

//OLED数据界面绘制
void OLED_Draw_interface(void);

//OLED数据显示 3组
void OLED_Show_3num(int x, int y, int z, unsigned int row);

//OLED数据显示 2组
void OLED_Show_2num(int x, int y, unsigned int row);


#endif
