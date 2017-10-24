#include "types.h"
#include "pca_conf.h"
#include "pca_82d_eval.h"
#include "pca_base.h"
#include "dev_led.h"
#include "dev_key.h"
#include "dev_pwr.h"
#include "dev_lcd.h"
#include "dev_sc.h"
#include "sys_delay.h"
#include "dev_exti.h"
#include "pca_base.h"
#include <stdio.h>

extern __IO uint Card1Inserted;
extern __IO uint Card2Inserted;
extern __IO uint Card3Inserted;
extern __IO uint Card4Inserted;

extern __IO uchar vell_mode;
extern __IO uint vell_pre;

extern USART_TypeDef * SC_USART[COMn];

uchar pwr_key_pass;

void SC1_IRQHandler(void)
{
  if(USART_GetITStatus(SC_USART[SAM1], USART_IT_FE) != RESET)
  {
    USART_ReceiveData(SC_USART[SAM1]);

    SC_ParityErrorHandler(SAM1);
  }
  
  if(USART_GetITStatus(SC_USART[SAM1], USART_IT_PE) != RESET)
  {
    USART_ITConfig(SC_USART[SAM1], USART_IT_RXNE, ENABLE);
		
    USART_ReceiveData(SC_USART[SAM1]);
  }
  
  if(USART_GetITStatus(SC_USART[SAM1], USART_IT_RXNE) != RESET)
  {
    USART_ITConfig(SC_USART[SAM1], USART_IT_RXNE, DISABLE);
    USART_ReceiveData(SC_USART[SAM1]);
  }
  
  if(USART_GetITStatus(SC_USART[SAM1], USART_IT_ORE) != RESET)
  {
    USART_ReceiveData(SC_USART[SAM1]);
  }

  if(USART_GetITStatus(SC_USART[SAM1], USART_IT_NE) != RESET)
  {
    USART_ReceiveData(SC_USART[SAM1]);
  }
}

void SC2_IRQHandler(void)
{
  if(USART_GetITStatus(SC_USART[SAM2], USART_IT_FE) != RESET)
  {
    USART_ReceiveData(SC_USART[SAM2]);

    SC_ParityErrorHandler(SAM2);
  }
  
  if(USART_GetITStatus(SC_USART[SAM2], USART_IT_PE) != RESET)
  {
    USART_ITConfig(SC_USART[SAM2], USART_IT_RXNE, ENABLE);
		
    USART_ReceiveData(SC_USART[SAM2]);
  }
  
  if(USART_GetITStatus(SC_USART[SAM2], USART_IT_RXNE) != RESET)
  {
    USART_ITConfig(SC_USART[SAM2], USART_IT_RXNE, DISABLE);
    USART_ReceiveData(SC_USART[SAM2]);
  }
  
  if(USART_GetITStatus(SC_USART[SAM2], USART_IT_ORE) != RESET)
  {
    USART_ReceiveData(SC_USART[SAM2]);
  }

  if(USART_GetITStatus(SC_USART[SAM2], USART_IT_NE) != RESET)
  {
    USART_ReceiveData(SC_USART[SAM2]);
  }
}
void SC3_IRQHandler(void)
{
  if(USART_GetITStatus(SC_USART[SAM3], USART_IT_FE) != RESET)
  {
    USART_ReceiveData(SC_USART[SAM3]);

    SC_ParityErrorHandler(SAM3);
  }
  
  if(USART_GetITStatus(SC_USART[SAM3], USART_IT_PE) != RESET)
  {
    USART_ITConfig(SC_USART[SAM3], USART_IT_RXNE, ENABLE);
		
    USART_ReceiveData(SC_USART[SAM3]);
  }
  
  if(USART_GetITStatus(SC_USART[SAM3], USART_IT_RXNE) != RESET)
  {
    USART_ITConfig(SC_USART[SAM3], USART_IT_RXNE, DISABLE);
    USART_ReceiveData(SC_USART[SAM3]);
  }
  
  if(USART_GetITStatus(SC_USART[SAM3], USART_IT_ORE) != RESET)
  {
    USART_ReceiveData(SC_USART[SAM3]);
  }

  if(USART_GetITStatus(SC_USART[SAM3], USART_IT_NE) != RESET)
  {
    USART_ReceiveData(SC_USART[SAM3]);
  }
}
void SC4_IRQHandler(void)
{
	if(USART_GetITStatus(SC_USART[SAM4], USART_IT_FE) != RESET)
  {
    USART_ReceiveData(SC_USART[SAM4]);

    SC_ParityErrorHandler(SAM4);
  }
  
  if(USART_GetITStatus(SC_USART[SAM4], USART_IT_PE) != RESET)
  {
    USART_ITConfig(SC_USART[SAM4], USART_IT_RXNE, ENABLE);
		
    USART_ReceiveData(SC_USART[SAM4]);
  }
  
  if(USART_GetITStatus(SC_USART[SAM4], USART_IT_RXNE) != RESET)
  {
    USART_ITConfig(SC_USART[SAM4], USART_IT_RXNE, DISABLE);
    USART_ReceiveData(SC_USART[SAM4]);
  }
  
  if(USART_GetITStatus(SC_USART[SAM4], USART_IT_ORE) != RESET)
  {
		USART_ReceiveData(SC_USART[SAM4]);
  }

  if(USART_GetITStatus(SC_USART[SAM4], USART_IT_NE) != RESET)
  {
    USART_ReceiveData(SC_USART[SAM4]);
  }
}

void EXTI0_IRQHandler(void){

	if(EXTI_GetITStatus(PWR_KEY_DETECT_EXTI) != RESET)
  {
    EXTI_ClearITPendingBit(PWR_KEY_DETECT_EXTI);
		if(Pwr_Key_Check()){
			Sys_Enter_StandBy();
		}
  }	
}


void EXTI1_IRQHandler(void){
  if(EXTI_GetITStatus(SC2_DETECT_EXTI) != RESET)
  {
    EXTI_ClearITPendingBit(SC2_DETECT_EXTI);
		
    Card2Inserted = 1;

    SC_PowerCmd(SAM2, ENABLE);

    SC_Reset(SAM2, Bit_RESET);
  }
}

void EXTI3_IRQHandler(void){
  if(EXTI_GetITStatus(PWR_CHG_DETECT_EXTI) != RESET)
  {
    EXTI_ClearITPendingBit(PWR_CHG_DETECT_EXTI);
		
		if(PWR_CHG_VL_KEY == 1){
			LED1_SET;
			LED2_CLR;
			vell_mode = VELLCHG_MODE;			
			LcdPrintVell(vell_pre,VELLCHG_MODE);
		}else{
			LED1_CLR;
			LED2_SET;
			vell_mode = VELLNOR_MODE;
			LcdPrintVell(vell_pre,VELLNOR_MODE);
		}
  }
}


void EXTI15_10_IRQHandler(void){
	if(EXTI_GetITStatus(SC1_DETECT_EXTI) != RESET)
  {
    EXTI_ClearITPendingBit(SC1_DETECT_EXTI);

    Card1Inserted = 1;

    SC_PowerCmd(SAM1, ENABLE);

    SC_Reset(SAM1, Bit_RESET);
  }
	
	if(EXTI_GetITStatus(SC3_DETECT_EXTI) != RESET)
  {
    EXTI_ClearITPendingBit(SC3_DETECT_EXTI);
	
    Card3Inserted = 1;

    SC_PowerCmd(SAM3, ENABLE);

    SC_Reset(SAM3, Bit_RESET);
  }	
	
	if(EXTI_GetITStatus(SC4_DETECT_EXTI) != RESET)
  {
    EXTI_ClearITPendingBit(SC4_DETECT_EXTI);

    Card4Inserted = 1;

    SC_PowerCmd(SAM4, ENABLE);

    SC_Reset(SAM4, Bit_RESET);
  }
	
	if(EXTI_GetITStatus(PWR_MON_DETECT_EXTI) != RESET)
  {
    EXTI_ClearITPendingBit(PWR_MON_DETECT_EXTI);
  }
}








