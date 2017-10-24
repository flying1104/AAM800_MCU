#include "types.h"
#include "stm32f4xx.h"
#include "dev_pin_conf.h"
#include "dev_delay.h"
#include "dev_spi.h"


void Spi_SW_Init(void){
	Spi1_SW_Init();
}
/*
void Spi_HW_Init(void) {
	GPIO_InitTypeDef  GPIO_InitStructure;
	SPI_InitTypeDef  SPI_InitStructure;
 
 	RCC_AHB1PeriphClockCmd(SDCARD_GPIO_CLK, ENABLE);
	RCC_AHB1PeriphClockCmd(SDCARD_SPI_CLK,  ENABLE );

	GPIO_InitStructure.GPIO_Pin = SDCARD_CS_PIN;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(SDCARD_GPIO, &GPIO_InitStructure);
	
	SDCARD_CS = 1;
 
	GPIO_InitStructure.GPIO_Pin = SDCARD_SCK_PIN | SDCARD_MISO_PIN | SDCARD_MOSI_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SDCARD_GPIO, &GPIO_InitStructure);
	
	GPIO_SetBits(SDCARD_GPIO, SDCARD_SCK_PIN | SDCARD_MISO_PIN | SDCARD_MOSI_PIN);

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//����SPI����ģʽ:����Ϊ��SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		//����ͬ��ʱ�ӵĿ���״̬Ϊ�ߵ�ƽ
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//����ͬ��ʱ�ӵĵڶ��������أ��������½������ݱ�����
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;		//���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRCֵ����Ķ���ʽ
	SPI_Init(SPI1, &SPI_InitStructure);  //����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���
 
	SPI_Cmd(SPI1, ENABLE); //ʹ��SPI����
	
	SPI1_ReadWriteByte(0xff);//��������		
		
	SDCARD_CS = 1;

}

//SPI �ٶ����ú���
//SpeedSet:
//SPI_BaudRatePrescaler_2   2��Ƶ   
//SPI_BaudRatePrescaler_8   8��Ƶ   
//SPI_BaudRatePrescaler_16  16��Ƶ  
//SPI_BaudRatePrescaler_256 256��Ƶ 
  
void SPI1_SetSpeed(uchar SPI_BaudRatePrescaler) {
  assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));
	SPI1->CR1 &= 0XFFC7;
	SPI1->CR1 |= SPI_BaudRatePrescaler;	//����SPI1�ٶ� 
	SPI_Cmd(SPI1, ENABLE); 

} 

//SPIx ��дһ���ֽ�
//TxData:Ҫд����ֽ�
//����ֵ:��ȡ�����ֽ�
uchar SPI1_ReadWriteByte(uchar TxData) {		
	uchar retry=0;				 	
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET) //���ָ����SPI��־λ�������:���ͻ���ձ�־λ
		{
		retry++;
		if(retry>200)return 0;
		}			  
	SPI_I2S_SendData(SPI1, TxData); //ͨ������SPIx����һ������
	retry=0;

	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET)//���ָ����SPI��־λ�������:���ܻ���ǿձ�־λ
		{
		retry++;
		if(retry>200)return 0;
		}	  						    
	return SPI_I2S_ReceiveData(SPI1); //����ͨ��SPIx������յ�����					    
}
*/
void Spi1_SW_Init(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(SPI1_SW_GPIO_CLK_D, ENABLE);
	RCC_AHB1PeriphClockCmd(SPI1_SW_GPIO_CLK_G, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = SPI1_SW_MOSI_PIN | SPI1_SW_SCK_PIN | SPI1_SW_NSS_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SPI1_SW_GPIO_G, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = SPI1_SW_MISO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;	
	GPIO_Init(SPI1_SW_GPIO_D, &GPIO_InitStructure);	
	
	SPI1_SW_SCK = 0;
	SPI1_SW_NSS = 1;
}

void Spi1_SW_Wr(ushort addr, ushort data) {
	uchar i;
	
	SPI1_SW_NSS = 0;
	for(i = 0; i < 16; i++) {
		if(addr & 0x8000)	SPI1_SW_MOSI = 1;
		else 							SPI1_SW_MOSI = 0;
		
		SPI1_SW_SCK = 1;
		addr <<= 1;
		Delay_Us(1);
		SPI1_SW_SCK = 0;
		Delay_Us(1);
	}
	
	SPI1_SW_MOSI = 0;
	Delay_Us(10);
	
	for(i = 0; i < 16; i++) {
		if(data & 0x8000) SPI1_SW_MOSI = 1;
		else 							SPI1_SW_MOSI = 0;

		SPI1_SW_SCK = 1;
		data <<= 1;
		Delay_Us(1);
		SPI1_SW_SCK = 0;
		Delay_Us(1);
	}
	
	SPI1_SW_NSS = 1;
}

ushort Spi1_SW_Rd(ushort addr) {
	uchar i;
	ushort addrTmp, data;

	SPI1_SW_NSS = 0;
	
	addrTmp = addr | 0x8000;
	
	for(i = 0; i < 16; i++) {
		if(addrTmp & 0x8000)	SPI1_SW_MOSI = 1;
		else									SPI1_SW_MOSI = 0;

		SPI1_SW_SCK = 1;
		addrTmp <<= 1;
		Delay_Us(1);
		SPI1_SW_SCK = 0;
		Delay_Us(1);
	}
	
	SPI1_SW_MOSI = 0;
	Delay_Us(10);
	
	for(i = 0; i < 16; i++) {
		data <<= 1;
		SPI1_SW_SCK = 0;
		
		if(SPI1_SW_MISO)	data = data | 0x1;
		Delay_Us(1);
		SPI1_SW_SCK = 1;
		Delay_Us(1);
	}
	
	SPI1_SW_SCK = 0;	
	Delay_Us(1);
	SPI1_SW_NSS = 1;
	
	return data;
}






