#include "types.h"
#include "stm32f4xx.h"
#include "dev_pin_conf.h"
#include "dev_delay.h"
#include "dev_lcd1602.h"


void Lcd1602_Gpio_Init(void) {
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(LCD_PWR_GPIO_CLK,ENABLE);
	RCC_AHB1PeriphClockCmd(LCD_BL_PWR_GPIO_CLK,ENABLE);
	RCC_AHB1PeriphClockCmd(LCD_EN_GPIO_CLK,ENABLE);
	RCC_AHB1PeriphClockCmd(LCD_DA_GPIO_CLK,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = LCD_PWR_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(LCD_PWR_GPIO, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = LCD_BL_PWR_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(LCD_BL_PWR_GPIO, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = LCD_RS_PIN | LCD_RW_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(LCD_RS_GPIO, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = LCD_EN_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(LCD_EN_GPIO, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = LCD_DATA_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(LCD_DA_GPIO, &GPIO_InitStructure);
}

void Lcd1602_Init(void) {	
	Lcd1602_Pwr(LCD_PWR_ON);
	Lcd1602_BL_Pwr(LCD_BL_PWR_ON);
	Delay_Ms(5);
	
	LCD_EN_POUT = 0;
	
	Lcd1602_Wcmd(0x38);
	Delay_Ms(5);
	
	Lcd1602_Wcmd(0x01);
	Delay_Ms(5);
	
	Lcd1602_Wcmd(0x0c);
	Delay_Ms(5);
	
	Lcd1602_Wcmd(0x06);
	Delay_Ms(5);
}

void Lcd1602_Xy(uchar x, uchar y) {
	uchar addr;
	if(y == 0)	addr = 0x80 + x;
	else 				addr = 0xc0 + x;
	Lcd1602_Wcmd(addr);
}

void Lcd1602_Putc(uchar data, uchar x, uchar y) {
	Lcd1602_Xy(x,y);
	Lcd1602_Wdata(data);
}

void Lcd1602_Puts(uchar *data, uchar x, uchar y) {
	uchar wide;
	
	Lcd1602_Xy(x,y);
	wide = x;
	while(*data) {
		Lcd1602_Wdata(*data++);
		Lcd1602_Wcmd(0x0f);
		wide++;
		if(wide == 16) Lcd1602_Xy(0, 1);
	}	
}

void Lcd1602_List_Show(uchar *data) {
	Lcd1602_Puts(data, 0, 0);
}

void Lcd1602_Char_Show(uchar *data, uchar x, uchar y, uchar len) {
	uchar wide;
	
	Lcd1602_Xy(x,y);
	wide = x;
	while(len--) {
		Lcd1602_Wdata(*data++);
		Lcd1602_Wcmd(0x0f);
		wide++;
		if(wide == 16)	Lcd1602_Xy(0, 1);
	}
}

void Lcd1602_Cursor(uchar mode, uchar x, uchar y) {
	if(mode == 1) {
		Lcd1602_Xy(x,y);
		Delay_Ms(5);
		Lcd1602_Wcmd(0x0d);
		Delay_Ms(5);
	}else{
		Lcd1602_Wcmd(0x0c);
		Delay_Ms(5);
	}
}

void Lcd1602_Cursor_Show(uchar x,uchar y) {
	Lcd1602_Cursor(1,x,y);
}

void Lcd1602_Cursor_Close(void) {
	Lcd1602_Cursor(0,0,0);
}

void Lcd1602_Wcmd(uchar data) {	
	Lcd1602_ChackBusy();
	
	LCD_RS_POUT = 0;
	LCD_RW_POUT = 0;
	
	LCD_EN_POUT = 0;
	Lcd1602_Write_Data(data);
	LCD_EN_POUT = 1;
	Delay_Us(10);
	LCD_EN_POUT = 0;
}

void Lcd1602_Wdata(uchar data) {
	Lcd1602_ChackBusy();
	
	LCD_RS_POUT = 1;
	LCD_RW_POUT = 0;
	
	LCD_EN_POUT = 0;
	Lcd1602_Write_Data(data);
	LCD_EN_POUT = 1;
	Delay_Us(10);
	LCD_EN_POUT = 0;
}

void Lcd1602_Write_Data(uchar data) {
	ushort tmpData;
	tmpData = GPIOF->ODR & 0xff00;
	GPIOF->ODR = tmpData | (data & 0x00ff);
}

void Lcd1602_ChackBusy(void) {
	uchar data;
	
	LCD_BUSY_IN_SET;
	
	do{
		LCD_RS_POUT = 0;
		LCD_RW_POUT = 1;
		LCD_EN_POUT = 1;
		Delay_Us(10);
		data = LCD_BUSY_PIN;
		LCD_EN_POUT = 0;
		Delay_Us(10);
	}while(data);
	
	LCD_BUSY_OUT_SET;
}

void Lcd1602_BL_Pwr(uchar config) {
	if(config == LCD_BL_PWR_ON) LCD_BL_PWR_POUT = LCD_BL_PWR_ON;
	else 												LCD_BL_PWR_POUT = LCD_BL_PWR_OFF;
}

void Lcd1602_Pwr(uchar config) {
	if(config == LCD_PWR_ON) 	LCD_PWR_POUT = LCD_PWR_ON;
	else 											LCD_PWR_POUT = LCD_PWR_OFF;
}
