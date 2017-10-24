#include "types.h"
#include "stm32f4xx.h"
#include "dev_pin_conf.h"
#include "sys_uart.h"
#include <stdio.h>

uchar USART_RX_BUF[USART_REC_LEN];
uchar USART_RX_STA = 0; 

void Usart_Init(uint baud){
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;	
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //ʹ��GPIOAʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//ʹ��USART1ʱ��
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); //GPIOA9����ΪUSART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); //GPIOA10����ΪUSART1
	
		//USART1�˿�����
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; //GPIOA9��GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOA,&GPIO_InitStructure); //��ʼ��PA9��PA10

   //USART1 ��ʼ������
	USART_InitStructure.USART_BaudRate = baud;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
  USART_Init(USART1, &USART_InitStructure); //��ʼ������1
	
  USART_Cmd(USART1, ENABLE);  //ʹ�ܴ���1 
	

	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//��������ж�

	//Usart1 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//����1�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���

}

int fputc(int ch, FILE *f)
{      
  USART_SendData(USART1,(uchar)ch); 
  while(!(USART1->SR & USART_FLAG_TXE));
	return ch;
}

uchar Uart1_Receive_Info(void) {
	uchar receiveData = 0;
	uchar size;
	uchar i;
	
	if(USART_RX_STA & 0x80){
		size = USART_RX_STA & 0x3f;
		for(i = 0; i < size; i++){
			receiveData = USART_RX_BUF[i];
			receiveData -= 0x30;
		}
		USART_RX_STA = 0;
	}
	
	return receiveData;
}

void USART1_IRQHandler(void) {
	uchar rxData;
	if(USART_GetITStatus(USART1, USART_IT_RXNE)==SET)
	{
		rxData = USART_ReceiveData(USART1);
		if((USART_RX_STA & 0x8000) == 0){
			if(USART_RX_STA & 0x4000){
				if(rxData != 0x0a){
					USART_RX_STA = 0x0000;
				}else{ 
					USART_RX_STA |= 0x8000;
				}
			}else{
				if(rxData == 0x0d){
					USART_RX_STA|=0x4000;
				}else{
					USART_RX_BUF[USART_RX_STA & 0X3FFF] = rxData ;
					USART_RX_STA++;
					if(USART_RX_STA > (USART_REC_LEN-1)){
						USART_RX_STA = 0;
					}
				}		 
			}
		}
	}
}








