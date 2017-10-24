#include "types.h"
#include "stm32f4xx.h"
#include "dev_delay.h"
#include "dev_key.h"
#include "dev_pin_conf.h"



void Key_Gpio_Init(void) {	
	GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(KEY_GPIOE_CLK | KEY_GPIOG_CLK, ENABLE);//ʹ��GPIOA,GPIOEʱ��
 
  GPIO_InitStructure.GPIO_Pin = KEY_GPIOE_PIN; //KEY0 KEY1 KEY2��Ӧ����
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//��ͨ����ģʽ
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(KEY_GPIOE, &GPIO_InitStructure);//��ʼ��GPIOE2,3,4
	
	 
  GPIO_InitStructure.GPIO_Pin = KEY_GPIOG_PIN;//WK_UP��Ӧ����PA0
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN ;//����
  GPIO_Init(KEY_GPIOG, &GPIO_InitStructure);//��ʼ��GPIOA0	
	
	
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



