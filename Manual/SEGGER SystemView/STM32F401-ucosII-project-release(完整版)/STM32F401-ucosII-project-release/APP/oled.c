/**
  ******************************************************************************
  * @file    OLED_I2C.c
  * @brief   128*64点阵的OLED显示屏驱动文件，仅适用于SD1306驱动IIC通信方式显示屏
  ******************************************************************************
        * Function List:
        *	1. void I2C_Configuration(void)-- 配置CPU的硬件I2C
        * 2. void I2C_WriteByte(uint8_t addr,uint8_t data) --
  向寄存器地址写一个byte的数据
        * 3. void WriteCmd(unsigned char I2C_Command) -- 写命令
        * 4. void WriteDat(unsigned char I2C_Data) -- 写数据
        * 5. void OLED_Init(void) -- OLED屏初始化
        * 6. void OLED_Set Pos(unsigned char x, unsigned char y) --
  设置起始点坐标
        * 7. void OLED_Fill(unsigned char fill_Data) -- 全屏填充
        * 8. void OLED_CLS(void) -- 清屏
        * 9. void OLED_ON(void) -- 唤醒
        * 10. void OLED_OFF(void) -- 睡眠
        * 11. void OLED_ShowStr(unsigned char x, unsigned char y, unsigned char
  ch[], unsigned char TextSize) -- 显示字符串(字体大小有6*8和8*16两种)
        * 12. void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char
  N) -- 显示中文(中文需要先取模，然后放到codetab.h中)
        * 13. void OLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned char
  x1,unsigned char y1,unsigned char BMP[]) -- BMP图片
        *
  *
  ******************************************************************************
  */

#include "oled.h"
/**
 * @brief  I2C3 I/O配置
 * @param  无
 * @retval 无
 */

void OLED_Config(void) { I2C3_Configuration(); }

/**
 * @brief  I2C_WriteByte，向OLED寄存器地址写一个byte的数据
 * @param  addr：寄存器地址
 *					data：要写入的数据
 * @retval 无
 */
//void I2C_WriteByte(uint8_t addr, uint8_t data) {
//    while (I2C_GetFlagStatus(I2C3, I2C_FLAG_BUSY))
//        ;

//    I2C_GenerateSTART(I2C3, ENABLE);  //开启I2C3
//    while (!I2C_CheckEvent(I2C3, I2C_EVENT_MASTER_MODE_SELECT))
//        ; /*EV5,主模式*/

//    I2C_Send7bitAddress(I2C3, OLED_ADDRESS,
//                        I2C_Direction_Transmitter);  //器件地址 -- 默认0x78
//    while (!I2C_CheckEvent(I2C3, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
//        ;

//    I2C_SendData(I2C3, addr);  //寄存器地址
//    while (!I2C_CheckEvent(I2C3, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
//        ;

//    I2C_SendData(I2C3, data);  //发送数据
//    while (!I2C_CheckEvent(I2C3, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
//        ;

//    I2C_GenerateSTOP(I2C3, ENABLE);  //关闭I2C3总线
//}

/**
 * @brief  WriteCmd，向OLED写入命令
 * @param  I2C_Command：命令代码
 * @retval 无
 */
void WriteCmd(unsigned char I2C_Command)  //写命令
{
    I2C_WriteByte(I2C3,OLED_ADDRESS,0x00, I2C_Command);
}

/**
 * @brief  WriteDat，向OLED写入数据
 * @param  I2C_Data：数据
 * @retval 无
 */
void WriteDat(unsigned char I2C_Data)  //写数据
{
    I2C_WriteByte(I2C3,OLED_ADDRESS,0x40, I2C_Data);
}

/**
 * @brief  OLED_Init，初始化OLED
 * @param  无
 * @retval 无
 */
void OLED_Init(void) {
    Delay_ms(1000);  // 1s,这里的延时很重要,上电后延时，没有错误的冗余设计
    WriteCmd(0xAE);  // display off
    WriteCmd(0x20);  // Set Memory Addressing Mode
    WriteCmd(0x10);  // 00,Horizontal Addressing Mode;01,Vertical Addressing
                     // Mode;10,Page Addressing Mode (RESET);11,Invalid
    WriteCmd(0xb0);  // Set Page Start Address for Page Addressing Mode,0-7
    WriteCmd(0xc8);  // Set COM Output Scan Direction
    WriteCmd(0x00);  //---set low column address
    WriteCmd(0x10);  //---set high column address
    WriteCmd(0x40);  //--set start line address
    WriteCmd(0x81);  //--set contrast control register
    WriteCmd(0xff);  //亮度调节 0x00~0xff
    WriteCmd(0xa1);  //--set segment re-map 0 to 127
    WriteCmd(0xa6);  //--set normal display
    WriteCmd(0xa8);  //--set multiplex ratio(1 to 64)
    WriteCmd(0x3F);  //
    WriteCmd(0xa4);  // 0xa4,Output follows RAM content;0xa5,Output ignores RAM
                     // content
    WriteCmd(0xd3);  //-set display offset
    WriteCmd(0x00);  //-not offset
    WriteCmd(0xd5);  //--set display clock divide ratio/oscillator frequency
    WriteCmd(0xf0);  //--set divide ratio
    WriteCmd(0xd9);  //--set pre-charge period
    WriteCmd(0x22);  //
    WriteCmd(0xda);  //--set com pins hardware configuration
    WriteCmd(0x12);
    WriteCmd(0xdb);  //--set vcomh
    WriteCmd(0x20);  // 0x20,0.77xVcc
    WriteCmd(0x8d);  //--set DC-DC enable
    WriteCmd(0x14);  //
    WriteCmd(0xaf);  //--turn on oled panel
}

/**
 * @brief  OLED_SetPos，设置光标
 * @param  x,光标x位置
 *					y，光标y位置
 * @retval 无
 */
void OLED_SetPos(unsigned char x, unsigned char y)  //设置起始点坐标
{
    WriteCmd(0xb0 + y);
    WriteCmd(((x & 0xf0) >> 4) | 0x10);
    WriteCmd((x & 0x0f) | 0x01);
}

/**
 * @brief  OLED_Fill，填充整个屏幕
 * @param  fill_Data:要填充的数据
 * @retval 无
 */
void OLED_Fill(unsigned char fill_Data)  //全屏填充
{
    unsigned char m, n;
    for (m = 0; m < 8; m++) {
        WriteCmd(0xb0 + m);  // page0-page1
        WriteCmd(0x00);      // low column start address
        WriteCmd(0x10);      // high column start address
        for (n = 0; n < 128; n++) {
            WriteDat(fill_Data);
        }
    }
}

/**
 * @brief  OLED_CLS，清屏
 * @param  无
 * @retval 无
 */
void OLED_CLS(void)  //清屏
{
    OLED_Fill(0x00);
}

/**
 * @brief  OLED_ON，将OLED从休眠中唤醒
 * @param  无
 * @retval 无
 */
void OLED_ON(void) {
    WriteCmd(0X8D);  //设置电荷泵
    WriteCmd(0X14);  //开启电荷泵
    WriteCmd(0XAF);  // OLED唤醒
}

/**
 * @brief  OLED_OFF，让OLED休眠 -- 休眠模式下,OLED功耗不到10uA
 * @param  无
 * @retval 无
 */
void OLED_OFF(void) {
    WriteCmd(0X8D);  //设置电荷泵
    WriteCmd(0X10);  //关闭电荷泵
    WriteCmd(0XAE);  // OLED休眠
}

/**
 * @brief  OLED_ShowStr，显示codetab.h中的ASCII字符,有6*8和8*16可选择
 * @param  x,y : 起始点坐标(x:0~127, y:0~7);
 *					ch[] :- 要显示的字符串;
 *					TextSize : 字符大小(1:6*8 ; 2:8*16)
 * @retval 无
 */
void OLED_ShowStr(unsigned char x, unsigned char y, unsigned char ch[],
                  unsigned char TextSize) {
    unsigned char c = 0, i = 0, j = 0;
    switch (TextSize) {
        case 1: {
            while (ch[j] != '\0') {
                c = ch[j] - 32;
                if (x > 126) {
                    x = 0;
                    y++;
                }
                OLED_SetPos(x, y);
                for (i = 0; i < 6; i++) WriteDat(F6x8[c][i]);
                x += 6;
                j++;
            }
        } break;
        case 2: {
            while (ch[j] != '\0') {
                c = ch[j] - 32;
                if (x > 120) {
                    x = 0;
                    y++;
                }
                OLED_SetPos(x, y);
                for (i = 0; i < 8; i++) WriteDat(F8X16[c * 16 + i]);
                OLED_SetPos(x, y + 1);
                for (i = 0; i < 8; i++) WriteDat(F8X16[c * 16 + i + 8]);
                x += 8;
                j++;
            }
        } break;
    }
}

/**
 * @brief  OLED_ShowCN，显示codetab.h中的汉字,16*16点阵
 * @param  x,y: 起始点坐标(x:0~127, y:0~7);
 *					N:汉字在codetab.h中的索引
 * @retval 无
 */
void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char N) {
    unsigned char wm = 0;
    unsigned int adder = 32 * N;
    OLED_SetPos(x, y);
    for (wm = 0; wm < 16; wm++) {
        WriteDat(F16x16[adder]);
        adder += 1;
    }
    OLED_SetPos(x, y + 1);
    for (wm = 0; wm < 16; wm++) {
        WriteDat(F16x16[adder]);
        adder += 1;
    }
}

/**
 * @brief  OLED_DrawBMP，显示BMP位图
 * @param  x0,y0 :起始点坐标(x0:0~127, y0:0~7);
 *					x1,y1 :
 *起点对角线(结束点)的坐标(x1:1~128,y1:1~8)
 * @retval 无
 */
void OLED_DrawBMP(unsigned char x0, unsigned char y0, unsigned char x1,
                  unsigned char y1, unsigned char BMP[]) {
    unsigned int j = 0;
    unsigned char x, y;

    if (y1 % 8 == 0)
        y = y1 / 8;
    else
        y = y1 / 8 + 1;
    for (y = y0; y < y1; y++) {
        OLED_SetPos(x0, y);
        for (x = x0; x < x1; x++) {
            WriteDat(BMP[j++]);
        }
    }
}

//显示2个数字
// x,y :起点坐标
// len :数字的位数
// size:字体大小
// mode:模式	0,填充模式;1,叠加模式
// num:数值(0~4294967295);
unsigned int oled_pow(uint8_t m, uint8_t n) {
    unsigned int result = 1;
    while (n--) result *= m;
    return result;
}
void OLED_Set_Pos(uint8_t x, uint8_t y) {
    WriteCmd(0xb0 + y);
    WriteCmd(((x & 0xf0) >> 4) | 0x10);
    WriteCmd(x & 0x0f);
}
void OLED_ShowChar(uint8_t x, uint8_t y, uint8_t chr, uint8_t Char_Size) {
    unsigned char c = 0, i = 0;
    c = chr - ' ';  //得到偏移后的值
    if (x > 128 - 1) {
        x = 0;
        y = y + 2;
    }
    if (Char_Size == 16) {
        OLED_Set_Pos(x, y);
        for (i = 0; i < 8; i++) WriteDat(F8X16[c * 16 + i]);
        OLED_Set_Pos(x, y + 1);
        for (i = 0; i < 8; i++) WriteDat(F8X16[c * 16 + i + 8]);
    } else {
        OLED_Set_Pos(x, y);
        for (i = 0; i < 6; i++) WriteDat(F6x8[c][i]);
    }
}

void OLED_ShowNum(uint8_t x, uint8_t y, unsigned int num, uint8_t len,
                  uint8_t size2) {
    uint8_t t, temp;
    uint8_t enshow = 0;
    for (t = 0; t < len; t++) {
        temp = (num / oled_pow(10, len - t - 1)) % 10;
        if (enshow == 0 && t < (len - 1)) {
            if (temp == 0) {
                OLED_ShowChar(x + (size2 / 2) * t, y, ' ', size2);
                continue;
            } else
                enshow = 1;
        }
        OLED_ShowChar(x + (size2 / 2) * t, y, temp + '0', size2);
    }
}

// OLED数据显示 3组
void OLED_Show_3num(uint16_t x, uint16_t y, uint16_t z, uint8_t row) {
    x = (int)x, y = (int)y, z = (int)z, row = (int)row;

    OLED_ShowNum(22, row, x, 3, 12);
    OLED_ShowNum(64, row, y, 3, 12);
    OLED_ShowNum(106, row, z, 3, 12);
}
