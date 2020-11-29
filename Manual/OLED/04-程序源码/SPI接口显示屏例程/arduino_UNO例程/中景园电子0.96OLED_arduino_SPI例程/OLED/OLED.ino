//////////////////////////////////////////////////////////////////////////////////   
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//中景园电子
//店铺地址：https://oled-zjy.taobao.com/
//
//  文 件 名   : main.c
//  版 本 号   : v2.0
//  作    者   : HuangKai
//  生成日期   : 2019-3-19
//  最近修改   : 
//  功能描述   : arduino UNO OLED显示屏例程
//              说明: 
//              ----------------------------------------------------------------
//              GND  电源地
//              VCC  3.3v电源
//              D0   8（SCL）
//              D1   9（SDA）
//              RES  10
//              DC   11
//              CS   12              
//              ----------------------------------------------------------------
// 修改历史   :
// 日    期   : 
// 作    者   : HuangKai
// 修改内容   : 创建文件
//版权所有，盗版必究。
//Copyright(C) 中景园电子2019-3-19
//All rights reserved
//******************************************************************************/
#include "font.h"

int scl=8;//定义数字接口8
int sda=9;//定义数字接口9
int res=10;//定义数字接口10
int dc=11;//定义数字接口11
int cs=12;//定义数字接口12

#define OLED_SCLK_Clr() digitalWrite(scl,LOW)//SCL
#define OLED_SCLK_Set() digitalWrite(scl,HIGH)

#define OLED_SDIN_Clr() digitalWrite(sda,LOW)//SDA
#define OLED_SDIN_Set() digitalWrite(sda,HIGH)

#define OLED_RST_Clr() digitalWrite(res,LOW)//RES
#define OLED_RST_Set() digitalWrite(res,HIGH)

#define OLED_DC_Clr()  digitalWrite(dc,LOW)//DC
#define OLED_DC_Set()  digitalWrite(dc,HIGH)
          
#define OLED_CS_Clr()  digitalWrite(cs,LOW)//CS
#define OLED_CS_Set()  digitalWrite(cs,HIGH)


#define OLED_CMD  0  //写命令
#define OLED_DATA 1 //写数据

uint8_t OLED_GRAM[128][8];//将要显示的缓存内容


void setup()
{
  uint8_t t=' ';
  OLED_Init();
  OLED_ColorTurn(0);//0正常显示 1反色显示
  OLED_DisplayTurn(0);//0正常显示 1翻转180度显示
  while(1)
  {
    OLED_ShowPicture(0,0,128,8,BMP1);
    delay(500);
    OLED_Clear();
    OLED_ShowChinese(0,0,0,16);//中
    OLED_ShowChinese(18,0,1,16);//景
    OLED_ShowChinese(36,0,2,16);//园
    OLED_ShowChinese(54,0,3,16);//电
    OLED_ShowChinese(72,0,4,16);//子
    OLED_ShowChinese(90,0,5,16);//科
    OLED_ShowChinese(108,0,6,16);//技
    OLED_ShowString(8,16,"ZHONGJINGYUAN",16);
    OLED_ShowString(20,32,"2014/05/01",16);
    OLED_ShowString(0,48,"ASCII:",16);  
    OLED_ShowString(63,48,"CODE:",16);
    OLED_ShowChar(48,48,t,16);//显示ASCII字符    
    t++;
    if(t>'~')t=' ';
    OLED_ShowNum(103,48,t,3,16);
    OLED_Refresh();
    delay(500);
    OLED_Clear();
    OLED_ShowChinese(0,0,0,16);  //16*16 中
    OLED_ShowChinese(16,0,0,24); //24*24 中
    OLED_ShowChinese(24,20,0,32);//32*32 中
    OLED_ShowChinese(64,0,0,64); //64*64 中
    OLED_Refresh();
    delay(500);
    OLED_Clear();
    OLED_ShowString(0,0,"ABC",12);//6*12 “ABC”
    OLED_ShowString(0,12,"ABC",16);//8*16 “ABC”
    OLED_ShowString(0,28,"ABC",24);//12*24 “ABC”
    OLED_Refresh();
    delay(500);
  }
}

void loop()
{
}
//反显函数
void OLED_ColorTurn(u8 i)
{
  if(!i) OLED_WR_Byte(0xA6,OLED_CMD);//正常显示
  else  OLED_WR_Byte(0xA7,OLED_CMD);//反色显示
}

//屏幕旋转180度
void OLED_DisplayTurn(u8 i)
{
  if(i==0)
    {
      OLED_WR_Byte(0xC8,OLED_CMD);//正常显示
      OLED_WR_Byte(0xA1,OLED_CMD);
    }
else
    {
      OLED_WR_Byte(0xC0,OLED_CMD);//反转显示
      OLED_WR_Byte(0xA0,OLED_CMD);
    }
}
void OLED_WR_Byte(uint8_t dat,uint8_t cmd)
{  
  uint8_t i;
  if(cmd)
    OLED_DC_Set();
  else
    OLED_DC_Clr();
  OLED_CS_Clr();
  for(i=0;i<8;i++)
  {
    OLED_SCLK_Clr();
    if(dat&0x80)
       OLED_SDIN_Set();
    else 
       OLED_SDIN_Clr();
    OLED_SCLK_Set();
    dat<<=1;   
  }                
}

//更新显存到OLED  
void OLED_Refresh(void)
{
  uint8_t i,n;
  for(i=0;i<8;i++)
  {
     OLED_WR_Byte(0xb0+i,OLED_CMD); //设置行起始地址
     OLED_WR_Byte(0x00,OLED_CMD);   //设置低列起始地址
     OLED_WR_Byte(0x10,OLED_CMD);   //设置高列起始地址
     for(n=0;n<128;n++)
     OLED_WR_Byte(OLED_GRAM[n][i],OLED_DATA);
  }
}
//清屏函数
void OLED_Clear(void)
{
  uint8_t i,n;
  for(i=0;i<8;i++)
  {
     for(n=0;n<128;n++)
      {
       OLED_GRAM[n][i]=0;//清除所有数据
      }
  }
  OLED_Refresh();//更新显示
}

//画点 
//x:0~127
//y:0~63
void OLED_DrawPoint(uint8_t x,uint8_t y)
{
  uint8_t i,m,n;
  i=y/8;
  m=y%8;
  n=1<<m;
  OLED_GRAM[x][i]|=n;
}

//清除一个点
//x:0~127
//y:0~63
void OLED_ClearPoint(uint8_t x,uint8_t y)
{
  uint8_t i,m,n;
  i=y/8;
  m=y%8;
  n=1<<m;
  OLED_GRAM[x][i]=~OLED_GRAM[x][i];
  OLED_GRAM[x][i]|=n;
  OLED_GRAM[x][i]=~OLED_GRAM[x][i];
}

//画线
//x:0~128
//y:0~64
void OLED_DrawLine(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2)
{
  uint8_t i,k,k1,k2,y0;
  if(x1==x2)    //画竖线
  {
      for(i=0;i<(y2-y1);i++)
      {
        OLED_DrawPoint(x1,y1+i);
      }
  }
  else if(y1==y2)   //画横线
  {
      for(i=0;i<(x2-x1);i++)
      {
        OLED_DrawPoint(x1+i,y1);
      }
  }
  else      //画斜线
  {
    k1=y2-y1;
    k2=x2-x1;
    k=k1*10/k2;
    for(i=0;i<(x2-x1);i++)
      {
        OLED_DrawPoint(x1+i,y1+i*k/10);
      }
  }
}
//x,y:圆心坐标
//r:圆的半径
void OLED_DrawCircle(uint8_t x,uint8_t y,uint8_t r)
{
  int a, b,num;
    a = 0;
    b = r;
    while(2 * b * b >= r * r)      
    {
        OLED_DrawPoint(x + a, y - b);
        OLED_DrawPoint(x - a, y - b);
        OLED_DrawPoint(x - a, y + b);
        OLED_DrawPoint(x + a, y + b);
 
        OLED_DrawPoint(x + b, y + a);
        OLED_DrawPoint(x + b, y - a);
        OLED_DrawPoint(x - b, y - a);
        OLED_DrawPoint(x - b, y + a);
        
        a++;
        num = (a * a + b * b) - r*r;//计算画的点离圆心的距离
        if(num > 0)
        {
            b--;
            a--;
        }
    }
}

//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~63
//size:选择字体 12/16/24
//取模方式 逐列式
void OLED_ShowChar(uint8_t x,uint8_t y,const char chr,uint8_t size1)
{
  uint8_t i,m,temp,size2,chr1;
  uint8_t y0=y;
  size2=(size1/8+((size1%8)?1:0))*(size1/2);  //得到字体一个字符对应点阵集所占的字节数
  chr1=chr-' ';  //计算偏移后的值
  for(i=0;i<size2;i++)
  {
    if(size1==12)
        { 
          temp=pgm_read_byte(&asc2_1206[chr1][i]);
        } //调用1206字体
    else if(size1==16)
        {
          temp=pgm_read_byte(&asc2_1608[chr1][i]);
        } //调用1608字体
    else if(size1==24)
        {
          temp=pgm_read_byte(&asc2_2412[chr1][i]);
        } //调用2412字体
    else return;
        for(m=0;m<8;m++)           //写入数据
        {
          if(temp&0x80)OLED_DrawPoint(x,y);
          else OLED_ClearPoint(x,y);
          temp<<=1;
          y++;
          if((y-y0)==size1)
          {
            y=y0;
            x++;
            break;
          }
        }
  }
}


//显示字符串
//x,y:起点坐标  
//size1:字体大小 
//*chr:字符串起始地址 
void OLED_ShowString(uint8_t x,uint8_t y,const char *chr,uint8_t size1)
{
  while((*chr>=' ')&&(*chr<='~'))//判断是不是非法字符!
  {
    OLED_ShowChar(x,y,*chr,size1);
    x+=size1/2;
    if(x>128-size1/2)  //换行
    {
      x=0;
      y+=size1;
    }
    chr++;
  }
}

//m^n
u32 OLED_Pow(uint8_t m,uint8_t n)
{
  u32 result=1;
  while(n--)
  {
    result*=m;
  }
  return result;
}

////显示2个数字
////x,y :起点坐标  
////len :数字的位数
////size:字体大小
void OLED_ShowNum(uint8_t x,uint8_t y,int num,uint8_t len,uint8_t size1)
{
  uint8_t t,temp;
  for(t=0;t<len;t++)
  {
    temp=(num/OLED_Pow(10,len-t-1))%10;
      if(temp==0)
      {
        OLED_ShowChar(x+(size1/2)*t,y,'0',size1);
      }
      else 
      {
        OLED_ShowChar(x+(size1/2)*t,y,temp+'0',size1);
      }
  }
}

//显示汉字
//x,y:起点坐标
//num:汉字对应的序号
//取模方式 列行式
void OLED_ShowChinese(uint8_t x,uint8_t y,const uint8_t num,uint8_t size1)
{
  uint8_t i,m,n=0,temp,chr1;
  uint8_t x0=x,y0=y;
  uint8_t size3=size1/8;
  while(size3--)
  {
    chr1=num*size1/8+n;
    n++;
      for(i=0;i<size1;i++)
      {
        if(size1==16)
            {temp=pgm_read_byte(&Hzk1[chr1][i]);}//调用16*16字体
        else if(size1==24)
            {temp=pgm_read_byte(&Hzk2[chr1][i]);}//调用24*24字体
        else if(size1==32)       
            {temp=pgm_read_byte(&Hzk3[chr1][i]);}//调用32*32字体
        else if(size1==64)
            {temp=pgm_read_byte(&Hzk4[chr1][i]);}//调用64*64字体
        else return;
              
            for(m=0;m<8;m++)
              {
                if(temp&0x01)OLED_DrawPoint(x,y);
                else OLED_ClearPoint(x,y);
                temp>>=1;
                y++;
              }
              x++;
              if((x-x0)==size1)
              {x=x0;y0=y0+8;}
              y=y0;
       }
  }
}

//配置写入数据的起始位置
void OLED_WR_BP(uint8_t x,uint8_t y)
{
  OLED_WR_Byte(0xb0+y,OLED_CMD);//设置行起始地址
  OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);
  OLED_WR_Byte((x&0x0f),OLED_CMD);
}

//x0,y0：起点坐标
//x1,y1：终点坐标
//BMP[]：要写入的图片数组
void OLED_ShowPicture(uint8_t x0,uint8_t y0,uint8_t x1,uint8_t y1,const uint8_t BMP[])
{
  int j=0;
  uint8_t t;
  uint8_t x,y;
  for(y=y0;y<y1;y++)
   {
     OLED_WR_BP(x0,y);
     for(x=x0;x<x1;x++)
     {
       t=pgm_read_byte(&BMP[j++]);
       OLED_WR_Byte(t,OLED_DATA);
     }
   }
}

//OLED的初始化
void OLED_Init(void)
{
  pinMode(scl,OUTPUT);//设置数字8
  pinMode(sda,OUTPUT);//设置数字9
  pinMode(res,OUTPUT);//设置数字10
  pinMode(dc,OUTPUT);//设置数字11
  pinMode(cs,OUTPUT);//设置数字12
  
  OLED_RST_Set();
  delay(100);
  OLED_RST_Clr();//复位
  delay(200);
  OLED_RST_Set();
  
  OLED_WR_Byte(0xAE,OLED_CMD);//--turn off oled panel
  OLED_WR_Byte(0x00,OLED_CMD);//---set low column address
  OLED_WR_Byte(0x10,OLED_CMD);//---set high column address
  OLED_WR_Byte(0x40,OLED_CMD);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
  OLED_WR_Byte(0x81,OLED_CMD);//--set contrast control register
  OLED_WR_Byte(0xCF,OLED_CMD);// Set SEG Output Current Brightness
  OLED_WR_Byte(0xA1,OLED_CMD);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
  OLED_WR_Byte(0xC8,OLED_CMD);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
  OLED_WR_Byte(0xA6,OLED_CMD);//--set normal display
  OLED_WR_Byte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
  OLED_WR_Byte(0x3f,OLED_CMD);//--1/64 duty
  OLED_WR_Byte(0xD3,OLED_CMD);//-set display offset Shift Mapping RAM Counter (0x00~0x3F)
  OLED_WR_Byte(0x00,OLED_CMD);//-not offset
  OLED_WR_Byte(0xd5,OLED_CMD);//--set display clock divide ratio/oscillator frequency
  OLED_WR_Byte(0x80,OLED_CMD);//--set divide ratio, Set Clock as 100 Frames/Sec
  OLED_WR_Byte(0xD9,OLED_CMD);//--set pre-charge period
  OLED_WR_Byte(0xF1,OLED_CMD);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
  OLED_WR_Byte(0xDA,OLED_CMD);//--set com pins hardware configuration
  OLED_WR_Byte(0x12,OLED_CMD);
  OLED_WR_Byte(0xDB,OLED_CMD);//--set vcomh
  OLED_WR_Byte(0x40,OLED_CMD);//Set VCOM Deselect Level
  OLED_WR_Byte(0x20,OLED_CMD);//-Set Page Addressing Mode (0x00/0x01/0x02)
  OLED_WR_Byte(0x02,OLED_CMD);//
  OLED_WR_Byte(0x8D,OLED_CMD);//--set Charge Pump enable/disable
  OLED_WR_Byte(0x14,OLED_CMD);//--set(0x10) disable
  OLED_WR_Byte(0xA4,OLED_CMD);// Disable Entire Display On (0xa4/0xa5)
  OLED_WR_Byte(0xA6,OLED_CMD);// Disable Inverse Display On (0xa6/a7) 
  OLED_WR_Byte(0xAF,OLED_CMD);
  OLED_Clear();
}


