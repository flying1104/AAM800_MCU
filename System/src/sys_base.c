#include "types.h"
#include "stm32f4xx.h" 
#include "dev_pin_conf.h"
#include "sys_base.h"


void Led_Base_Init(void) {
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(LED_BASE_GPIO_CLK, ENABLE);

	GPIO_InitStructure.GPIO_Pin = LED_BASE_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(LED_BASE_GPIO, &GPIO_InitStructure);

	Led_Base_Config(LED_ON);
}

void Led_Base_Config(uchar config) {
	if(config == LED_ON)	LED_BASE_POUT = LED_ON;
	else 									LED_BASE_POUT = LED_OFF;
}


void System_Rst_Init(void) {
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(SYSTEM_RST_GPIO_CLK, ENABLE);

	GPIO_InitStructure.GPIO_Pin = SYSTEM_RST_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(SYSTEM_RST_GPIO, &GPIO_InitStructure);

	System_Rst_Config(SYSTEM_RST_ON);
}

void System_Rst_Config(uchar config) {
	if(config == SYSTEM_RST_ON) 	SYSTEM_RST_POUT = SYSTEM_RST_ON;
	else 												  SYSTEM_RST_POUT = SYSTEM_RST_OFF;
}













