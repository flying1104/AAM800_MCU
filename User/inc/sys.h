#ifndef __SYS_H
#define __SYS_H	 
#include "stm32f4xx.h" 


//����Ϊ��ຯ��
void WFI_SET(void);		//ִ��WFIָ��
void INTX_DISABLE(void);//�ر������ж�
void INTX_ENABLE(void);	//���������ж�
void MSR_MSP(u32 addr);	//���ö�ջ��ַ 


void Stm32_Clock_Init(u32 plln,u32 pllm,u32 pllp,u32 pllq);
void MY_NVIC_SetVectorTable(u32 NVIC_VectTab,u32 Offset)	;
u8 Sys_Clock_Set(u32 plln,u32 pllm,u32 pllp,u32 pllq);

#endif











