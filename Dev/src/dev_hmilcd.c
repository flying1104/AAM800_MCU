#include "types.h"
#include "stm32f4xx.h"
#include "dev_pin_conf.h"
#include "dev_hmilcd.h"
#include <stdio.h>

uchar USART2_RX_BUF[USART2_REC_LEN];
uchar USART2_RX_STA = 0;

uchar HMI_RX_BUF[HMID_ID_LEN];
uchar HMI_RX_STA = 0;

uchar HMIGetId(void)
{
	uchar page = 0;
	uchar id = 0;
	uchar active = 0;
	uchar flag = 0;
	
	printf("HMIGetId\n");
	if(HMI_RX_BUF[0] == 0x6)
	{
		if(HMI_RX_BUF[1] == 0x5)
		{
			page = HMI_RX_BUF[2];
			id = HMI_RX_BUF[3];
			active = HMI_RX_BUF[4];
			printf("page = %x\n",page);
			printf("id = %x\n",id);
			printf("active = %x\n",active);
		}
		else flag = 0x01;
	}	
	else flag = 0x01;
	
	if(flag == 0x01)
	{
		page = 0xff;
		id = 0xff;
		active = 0xff;
	}
	
	return id;	
}

void HMISends(char *buf1)		  //字符串发送函数
{
	u8 i=0;
	while(1)
	{
	 if(buf1[i]!=0)
	 	{
			USART_SendData(USART2,buf1[i]);  //发送一个字节
			while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET){};//等待发送结束
		 	i++;
		}
	 else 
	 return ;

		}
	}
void HMISendb(uchar k)		         //字节发送函数
{		 
	u8 i;
	 for(i=0;i<3;i++)
	 {
	 if(k!=0)
	 	{
			USART_SendData(USART2,k);  //发送一个字节
			while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET){};//等待发送结束
		}
	 else 
	 return ;

	 } 
} 


void Uart2_Init(uint baud)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;	
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //使能GPIOA时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//使能USART1时钟
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2); //GPIOA2复用为USART2
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2); //GPIOA3复用为USART2
	
		//USART2端口配置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3; //GPIOA9与GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure); //初始化PA2，PA3

   //USART2 初始化设置
	USART_InitStructure.USART_BaudRate = baud;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
  USART_Init(USART2, &USART_InitStructure); //初始化串口2
	
  USART_Cmd(USART2, ENABLE);  //使能串口2
	

	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启相关中断

	//Usart2 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;//串口2中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =4;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
}

/*
void USART2_IRQHandler(void)                	//串口2中断服务程序
{
	uchar rxData;
	uchar i;
	
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{
		rxData = USART_ReceiveData(USART2);
		
		
		if((USART2_RX_STA & 0x8000) == 0) //接收未完成
		{
			if(USART2_RX_STA & 0x4000) //接收到了0xff
			{
				if(rxData != 0xff)//0x0a = 换行符
				{
					USART2_RX_STA = 0x0000;
				}
				else
				{ 
					USART2_RX_STA |= 0x8000;
				}
			}
			else
			{
				if(rxData == 0xff)//0x0d = 回车键
				{
					USART2_RX_STA |= 0x4000;
				}
				else
				{
					USART2_RX_BUF[USART2_RX_STA & 0X3FFF] = rxData;
					HMI_RX_BUF[HMI_RX_STA & 0x3fff] = rxData - 0x30;					
					USART2_RX_STA++;
					HMI_RX_STA++;
					if(USART2_RX_STA > (USART2_REC_LEN - 1))
					{
						USART2_RX_STA = 0;
					}
				}		 
			}
		}
	}
}
*/

void USART2_IRQHandler(void)                	//串口2中断服务程序
{
	uchar rxData;
	uchar i;
	
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{
		rxData = USART_ReceiveData(USART2);
		
		if((USART2_RX_STA & 0x8000) == 0) //接收未完成
		{
			if(USART2_RX_STA & 0x4000) //接收到了0x0d
			{
				if(rxData != 0x0a)//0x0a = 换行符
				{
					USART2_RX_STA = 0x0000;
				}
				else
				{ 
					USART2_RX_STA |= 0x8000;
				}
			}
			else
			{
				if(rxData == 0x0d)//0x0d = 回车键
				{
					USART2_RX_STA |= 0x4000;
				}
				else
				{
					USART2_RX_BUF[USART2_RX_STA & 0X3FFF] = rxData;
					HMI_RX_BUF[HMI_RX_STA & 0x3fff] = rxData;					
					USART2_RX_STA++;
					HMI_RX_STA++;
					if(USART2_RX_STA > (USART2_REC_LEN - 1))
					{
						USART2_RX_STA = 0;
					}
				}		 
			}
		}
	}
}



