/**
  ******************************************************************************
  * 文件名程: oled.c 
  * 作    者: Jason_xy
  * 个人博客：https://jason-xy.cn
  * 版    本: V1.0
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
  ******************************************************************************
  */

#include "oled.h"
#include "oledfont.h"

//几个变量声明
uint8_t **Hzk;	//字模矩阵

//初始化命令
uint8_t CMD_Data[]={
0xAE, 0x00, 0x10, 0x40, 0xB0, 0x81, 0xFF, 0xA1, 0xA6, 0xA8, 0x3F,
					
0xC8, 0xD3, 0x00, 0xD5, 0x80, 0xD8, 0x05, 0xD9, 0xF1, 0xDA, 0x12,
					
0xD8, 0x30, 0x8D, 0x14, 0xAF};

void WriteCmd()
{
	uint8_t i = 0;
	for(i=0; i<27; i++){
		HAL_I2C_Mem_Write(&hi2c1 ,0x78,0x00,I2C_MEMADD_SIZE_8BIT,CMD_Data+i,1,0x100);
	}
}

//向设备写控制命令
void OLED_WR_CMD(uint8_t cmd)
{
	HAL_I2C_Mem_Write(&hi2c1 ,0x78,0x00,I2C_MEMADD_SIZE_8BIT,&cmd,1,0x100);
}

//向设备写数据
void OLED_WR_DATA(uint8_t data)
{
	HAL_I2C_Mem_Write(&hi2c1 ,0x78,0x40,I2C_MEMADD_SIZE_8BIT,&data,1,0x100);
}

//初始化oled屏幕
void OLED_Init(void)
{ 	
	HAL_Delay(200);
	WriteCmd();
}

//清屏size12 size16要清两行，其他函数有类似情况
void OLED_Clear()
{
	uint8_t i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_CMD(0xb0+i);
		OLED_WR_CMD (0x00); 
		OLED_WR_CMD (0x10); 
		for(n=0;n<128;n++)
			OLED_WR_DATA(0);
	} 
}

//清行
void OLED_Clearrow(uint8_t i)
{
	uint8_t n;
	OLED_WR_CMD(0xb0+i);
		OLED_WR_CMD (0x00); 
		OLED_WR_CMD (0x10); 
		for(n=0;n<128;n++)
			OLED_WR_DATA(0);
}

//开启OLED显示    
void OLED_Display_On(void)
{
	OLED_WR_CMD(0X8D);  //SET DCDC命令
	OLED_WR_CMD(0X14);  //DCDC ON
	OLED_WR_CMD(0XAF);  //DISPLAY ON
}

//关闭OLED显示     
void OLED_Display_Off(void)
{
	OLED_WR_CMD(0X8D);  //SET DCDC命令
	OLED_WR_CMD(0X10);  //DCDC OFF
	OLED_WR_CMD(0XAE);  //DISPLAY OFF
}

//确定起始坐标		 
void OLED_Set_Pos(uint8_t x, uint8_t y) 
{ 	
	OLED_WR_CMD(0xb0+y);
	OLED_WR_CMD(((x&0xf0)>>4)|0x10);
	OLED_WR_CMD(x&0x0f);
} 

//启动
void OLED_On(void)  
{  
	uint8_t i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_CMD(0xb0+i);    //设置页地址（0~7）
		OLED_WR_CMD(0x00);      //设置显示位置—列低地址
		OLED_WR_CMD(0x10);      //设置显示位置—列高地址   
		for(n=0;n<128;n++)
			OLED_WR_DATA(1); 
	} //更新显示
}

unsigned int oled_pow(uint8_t m,uint8_t n)
{
	unsigned int result=1;	 
	while(n--)result*=m;    
	return result;
}

//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~63
//mode:0,反白显示;1,正常显示				 
//size:选择字体 16/12 
void OLED_ShowChar(uint8_t x, uint8_t y, uint8_t chr, uint8_t Char_Size)
{      	
	unsigned char c=0,i=0;	
	c=chr-' ';//得到偏移后的值			
	if(x>128-1)
	{
		x=0;
		y=y+2;
	}
	if(Char_Size == 16)
	{
		OLED_Set_Pos(x,y);	
		for(i=0;i<8;i++)
		OLED_WR_DATA(F8x16[c*16+i]);
		OLED_Set_Pos(x,y+1);
		for(i=0;i<8;i++)
		OLED_WR_DATA(F8x16[c*16+i+8]);
	}
	else {	
		OLED_Set_Pos(x,y);
		for(i=0;i<6;i++)
		OLED_WR_DATA(F6x8[c][i]);
	}
}

//显示2个数字
//x,y :起点坐标	 
//len :数字的位数
//size:字体大小
//mode:模式	0,填充模式;1,叠加模式
//num:数值(0~4294967295);	 		  
void OLED_ShowNum(uint8_t x,uint8_t y,unsigned int num,uint8_t len,uint8_t size2)
{         	
	uint8_t t,temp;
	uint8_t enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/oled_pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size2/2)*t,y,' ',size2);
				continue;
			}else
				enshow=1; 
		 	 
		}
	 	OLED_ShowChar(x+(size2/2)*t,y,temp+'0',size2); 
	}
} 

//显示一个字符号串
void OLED_ShowString(uint8_t x,uint8_t y,uint8_t *chr,uint8_t Char_Size)
{
	unsigned char j=0;
	while (chr[j]!='\0')
	{	OLED_ShowChar(x,y,chr[j],Char_Size);
			x+=8;
		if(x>120){x=0;y+=2;}
			j++;
	}
}

//显示汉字
//hzk 用取模软件得出的数组
void OLED_ShowCHinese(uint8_t x,uint8_t y,uint8_t no)
{      			    
	uint8_t t,adder=0;
	OLED_Set_Pos(x,y);	
    for(t=0;t<16;t++)
	{
		OLED_WR_DATA(Hzk[2*no][t]);
		adder+=1;
     }	
		OLED_Set_Pos(x,y+1);	
    for(t=0;t<16;t++)
	{	
		OLED_WR_DATA(Hzk[2*no+1][t]);
		adder+=1;
    }					
}

//OLED数据界面绘制
void OLED_Draw_interface(void)
{
  OLED_ShowString(0,0,(uint8_t*)"Gx=",12);
  OLED_ShowString(42,0,(uint8_t*)"Gy=",12);
  OLED_ShowString(84,0,(uint8_t*)"Gz=",12);
  OLED_ShowString(0,1,(uint8_t*)"Ax=",12);
  OLED_ShowString(42,1,(uint8_t*)"Ay=",12);
  OLED_ShowString(84,1,(uint8_t*)"Az=",12);
  OLED_ShowString(0,2,(uint8_t*)"Mx=",12);
  OLED_ShowString(42,2,(uint8_t*)"My=",12);
  OLED_ShowString(84,2,(uint8_t*)"Mz=",12);
  OLED_ShowString(0,3,(uint8_t*)"C1=",12);
  OLED_ShowString(42,3,(uint8_t*)"C2=",12);
  OLED_ShowString(84,3,(uint8_t*)"C3=",12);
  OLED_ShowString(0,4,(uint8_t*)"C4=",12);
  OLED_ShowString(42,4,(uint8_t*)"C5=",12);
  OLED_ShowString(84,4,(uint8_t*)"C6=",12);
  OLED_ShowString(0,5,(uint8_t*)"M1=",12);
  OLED_ShowString(64,5,(uint8_t*)"M2=",12);
  OLED_ShowString(0,6,(uint8_t*)"M3=",12);
  OLED_ShowString(64,6,(uint8_t*)"M4=",12);
  OLED_ShowString(0,7,(uint8_t*)"T=",12);
  OLED_ShowString(64,7,(uint8_t*)"P=",12);
}

//OLED数据显示 3组
void OLED_Show_3num(uint16_t x, uint16_t y, uint16_t z, uint8_t row)
{
	x=(int)x,y=(int)y,z=(int)z,row=(int)row;

	OLED_ShowNum(22,row,x,3,12);
	OLED_ShowNum(64,row,y,3,12);
	OLED_ShowNum(106,row,z,3,12);
}

void OLED_Show_2num(uint16_t x, uint16_t y, uint8_t row)
{
	x=(int)x,y=(int)y,row=(int)row;
	
	OLED_ShowNum(24,row,x,3,12);
	OLED_ShowNum(88,row,y,3,12);
}
