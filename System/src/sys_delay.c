#include "types.h"
#include "stm32f4xx.h"
#include "sys_delay.h"

static uchar fac_us = 0;
static ushort fac_ms = 0;

void Delay_Init(uchar sysclk){
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);	
	fac_us = sysclk / 8;
	fac_ms = fac_us * 1000;
}

//nms <= 0xffffff * 8 * 1000 / sysclk
//120MHZ nms <= 1118ms = 1.118s
void Delay_Ms(ushort nms){
	uint tmp;
	SysTick->LOAD = (uint)nms * fac_ms;
	SysTick->VAL	= 0x00;
	SysTick->CTRL	= 0x01;
	do{
		tmp = SysTick->CTRL;
	}while((tmp & 0x01) && !(tmp & (1 << 16)));
	SysTick->CTRL = 0x00;
	SysTick->VAL	= 0x00;
}


void Delay_Us(uint nus){
	uint tmp;
	SysTick->LOAD = nus * fac_us;
	SysTick->VAL	= 0x00;
	SysTick->CTRL = 0x01;
	do{
			tmp = SysTick->CTRL;
	}while((tmp & 0x01) && !(tmp & (1 << 16)));
	SysTick->CTRL = 0x00;
	SysTick->VAL 	= 0x00;
}

