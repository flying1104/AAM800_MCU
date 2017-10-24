#ifndef __DEV_LCD1602
#define __DEV_LCD1602

#include "dev_io.h"

/**********************************************************
										LCD1602 PIN SET
***********************************************************						
1.LCD_PWR			--	PE4
2.LCD_BL_PWR	--	PF8	
3.LCD_RS			--	PE5
4.LCD_RW			--	PE6
5.LCD_EN			--	PC13
6.LCD_D0			--	PF0
7.LCD_D1			--	PF1
8.LCD_D2			--	PF2
9.LCD_D3			--	PF3
10.LCD_D4			--	PF4
11.LCD_D5			--	PF5
12.LCD_D6			--	PF6
13.LCD_D7			--	PF7
***********************************************************/

#define LCD_BUSY_IN_SET  {GPIOF->MODER &= ~(3 << (7 * 2)); GPIOF->MODER |= 0 << 7*2;}	//PF7输入模式
#define LCD_BUSY_OUT_SET {GPIOF->MODER &= ~(3 << (7 * 2)); GPIOF->MODER |= 1 << 7*2;} //PF7输出模式

#define LCD_PWR_ON			1
#define LCD_PWR_OFF			0

#define LCD_BL_PWR_ON		1		
#define LCD_BL_PWR_OFF	0

#define LCD_PWR_POUT		PEout(4)
#define LCD_BL_PWR_POUT	PFout(8)
#define LCD_RS_POUT			PEout(5)
#define LCD_RW_POUT			PEout(6)
#define LCD_EN_POUT			PCout(13)
#define LCD_BUSY_PIN		PFin(7)

void Lcd1602_Gpio_Init(void);
void Lcd1602_Init(void) ;
void Lcd1602_Xy(uchar x, uchar y);
void Lcd1602_Putc(uchar data, uchar x, uchar y);
void Lcd1602_Puts(uchar *data, uchar x, uchar y);
void Lcd1602_List_Show(uchar *data);
void Lcd1602_Char_Show(uchar *data, uchar x, uchar y, uchar len);
void Lcd1602_Cursor(uchar mode, uchar x, uchar y);
void Lcd1602_Cursor_Show(uchar x,uchar y);
void Lcd1602_Cursor_Close(void);
void Lcd1602_Wcmd(uchar data);
void Lcd1602_Wdata(uchar data);
void Lcd1602_Write_Data(uchar data);
void Lcd1602_ChackBusy(void);
void Lcd1602_Pwr(uchar config);
void Lcd1602_BL_Pwr(uchar config) ;




#endif
