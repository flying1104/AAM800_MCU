#include "types.h"
#include "stm32f4xx.h"
#include "dev_delay.h"
#include "dev_key.h"
#include "dev_pin_conf.h"



void Key_Gpio_Init(void) {	
	GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(KEY_GPIOE_CLK | KEY_GPIOG_CLK, ENABLE);//使能GPIOA,GPIOE时钟
 
  GPIO_InitStructure.GPIO_Pin = KEY_GPIOE_PIN; //KEY0 KEY1 KEY2对应引脚
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输入模式
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(KEY_GPIOE, &GPIO_InitStructure);//初始化GPIOE2,3,4
	
	 
  GPIO_InitStructure.GPIO_Pin = KEY_GPIOG_PIN;//WK_UP对应引脚PA0
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN ;//下拉
  GPIO_Init(KEY_GPIOG, &GPIO_InitStructure);//初始化GPIOA0	
	
	
	//KEY LED
	GPIO_InitStructure.GPIO_Pin = KEY_LED_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	Key_Dump_Config(LED_OFF);
	Key_Bypass_Config(LED_OFF);
}

void Key_Dump_Config(uchar config) 
{
	if(config == LED_ON)	KEY_DUMP_LED = LED_ON;
	else 									KEY_DUMP_LED = LED_OFF;
}

void Key_Bypass_Config(uchar config)
{
	if(config == LED_ON)	KEY_BYPASS_LED = LED_ON;
	else 										KEY_BYPASS_LED = LED_OFF;
}



