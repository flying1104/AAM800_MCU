#include "types.h"
#include "stm32f10x.h"
#include "dev_pin_conf.h"
#include "dev_max7219.h"
#include "dev_delay.h"



void Max7219_Init(void) {
	uchar i;
	
	Max7219_Gpio_Init();
	Delay_Ms(10);
	
 	Max7219_Wcmd(DISPLAY_TEST, 0x00);		//Normal Operation XXXXXXX0 Display Test Mode XXXXXXXX1
	Max7219_Wcmd(DECODE_MODE, 0x0f);		//Decode Mode Select D7~D0 1 B decode 0 No decode 
	Max7219_Wcmd(SCAN_LIMIT, 0x04);			//SCAN LIMIT 0~7 0xX0~0xX7
	Max7219_Wcmd(INTENSITY, 0x04);			//Set Intensity   0xX0~0xXf
	
	for(i = 1; i < 5; i++) {
		Max7219_Wdata(i, 0x0f);
	}
	
	Max7219_Wdata(5, 0x00);
	Max7219_Wcmd(SHUT_DOWN, 0x01);	 //Normal Operation XXXXXXX1 Shutdown Mode   XXXXXXXX0
}


 //  key_led_dis(KEY_LED_ALL_OFF);

void Max7219_Gpio_Init(void) {
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(MAX7219_SPI_GPIOA_CLK, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = MAX7219_SCK_PIN | MAX7219_CS_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(MAX7219_SPI_GPIOA, &GPIO_InitStructure);
	
	RCC_APB2PeriphClockCmd(MAX7219_SPI_GPIOC_CLK, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = MAX7219_MOSI_PIN;
	GPIO_Init(MAX7219_SPI_GPIOC, &GPIO_InitStructure);	
	
	MAX7219_SCK = 1;
	MAX7219_CS = 1;
	MAX7219_MOSI = 1;
	Delay_Ms(10);
}

void Max7219_Wdata(uchar addr, uchar data) {
	uchar i;
	
	MAX7219_CS = 0;
	Delay_Us(5);
	MAX7219_SCK = 0;	
	
	for(i = 0; i < 8; i++) {
		if(addr & 0x80) MAX7219_MOSI = 1;
		else 						MAX7219_MOSI = 0;
		Delay_Us(20);
		MAX7219_SCK = 1;
		Delay_Us(40);
		MAX7219_SCK = 0;
		Delay_Us(20);
		addr = addr << 1;
	}
	
	for(i = 0; i < 8; i++) {
		if(data & 0x80) MAX7219_MOSI = 1;
		else 						MAX7219_MOSI = 0;
		Delay_Us(20);
		MAX7219_SCK = 1;
		Delay_Us(40);
		MAX7219_SCK = 0;
		Delay_Us(20);
		data = data << 1;
	}
	
	MAX7219_CS = 1;
	Delay_Ms(5);
}

void Max7219_Wcmd(uchar addr, uchar data) {
	uchar i;
	
	MAX7219_CS = 0;
	Delay_Us(5);
	MAX7219_SCK = 0;
	
	for(i = 0; i < 8; i++) {
	
		if(addr & 0x80) MAX7219_MOSI = 1;
		else 						MAX7219_MOSI = 0;
		Delay_Us(20);
		MAX7219_SCK = 1;
		Delay_Us(40);
		MAX7219_SCK = 0;
		Delay_Us(20);
		addr = addr << 1;
	}
	
	for(i = 0; i < 8; i++) {
		if(data & 0x80) MAX7219_MOSI = 1;
		else 						MAX7219_MOSI = 0;
		Delay_Us(20);
		MAX7219_SCK = 1;
		Delay_Us(40);
		MAX7219_SCK = 0;
		Delay_Us(20);
		data = data << 1;
	}
	
	MAX7219_CS = 1;
	Delay_Ms(5);
}


