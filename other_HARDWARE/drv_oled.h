#ifndef  _DRV_OLED_H_
#define  _DRV_OLED_H_

#include "drv_i2c.h"

#define OLED_ADDR	0x78

void OLED_Init(void);
void oled_WriteCmd(unsigned char oled_Command);
void oled_WriteDat(unsigned char oled_Data);
void OLED_SetPos(unsigned char x, unsigned char y);
void OLED_Fill(unsigned char fill_Data);
void OLED_CLS(void);
void OLED_ON(void);
void OLED_OFF(void);
void OLED_ShowStr(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize);
void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char N);
void OLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[]);




#endif

