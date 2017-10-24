#ifndef __DEV_UART
#define __DEV_UART



#define USART_REC_LEN  			200  	//定义最大接收字节数 200



/**********************************************
						USART_FUN_SET
***********************************************								

***********************************************/

void Usart_Init(uint baud);

uchar Uart1_Receive_Info(void);







#endif

