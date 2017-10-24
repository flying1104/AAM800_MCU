#ifndef __SYS_H
#define __SYS_H	 
#include "stm32f4xx.h" 


//以下为汇编函数
void WFI_SET(void);		//执行WFI指令
void INTX_DISABLE(void);//关闭所有中断
void INTX_ENABLE(void);	//开启所有中断
void MSR_MSP(u32 addr);	//设置堆栈地址 


void Stm32_Clock_Init(u32 plln,u32 pllm,u32 pllp,u32 pllq);
void MY_NVIC_SetVectorTable(u32 NVIC_VectTab,u32 Offset)	;
u8 Sys_Clock_Set(u32 plln,u32 pllm,u32 pllp,u32 pllq);

#endif











