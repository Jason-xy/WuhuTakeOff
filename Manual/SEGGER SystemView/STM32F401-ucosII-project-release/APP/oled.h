#ifndef __OLED_I2C_H
#define __OLED_I2C_H

#include "codetab.h"
#include "i2c.h"
#include "stm32f4xx.h"
#include "sysTick.h"

void OLED_Config(void);

//void I2C_WriteByte(uint8_t addr, uint8_t data);
void WriteCmd(unsigned char I2C_Command);
void WriteDat(unsigned char I2C_Data);

void OLED_Init(void);
void OLED_SetPos(unsigned char x, unsigned char y);
void OLED_Fill(unsigned char fill_Data);
void OLED_CLS(void);
void OLED_ON(void);
void OLED_OFF(void);
void OLED_ShowStr(unsigned char x, unsigned char y, unsigned char ch[],
                  unsigned char TextSize);
void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char N);
void OLED_DrawBMP(unsigned char x0, unsigned char y0, unsigned char x1,
                  unsigned char y1, unsigned char BMP[]);

unsigned int oled_pow(uint8_t m, uint8_t n);
void OLED_Set_Pos(uint8_t x, uint8_t y);
void OLED_ShowChar(uint8_t x, uint8_t y, uint8_t chr, uint8_t Char_Size);
void OLED_ShowNum(uint8_t x, uint8_t y, unsigned int num, uint8_t len,
                  uint8_t size2);
void OLED_Show_3num(uint16_t x, uint16_t y, uint16_t z, uint8_t row);
#endif
