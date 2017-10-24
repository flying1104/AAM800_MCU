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

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//设置SPI工作模式:设置为主SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//设置SPI的数据大小:SPI发送接收8位帧结构
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		//串行同步时钟的空闲状态为高电平
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//串行同步时钟的第二个跳变沿（上升或下降）数据被采样
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;		//定义波特率预分频的值:波特率预分频值为256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRC值计算的多项式
	SPI_Init(SPI1, &SPI_InitStructure);  //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器
 
	SPI_Cmd(SPI1, ENABLE); //使能SPI外设
	
	SPI1_ReadWriteByte(0xff);//启动传输		
		
	SDCARD_CS = 1;

}

//SPI 速度设置函数
//SpeedSet:
//SPI_BaudRatePrescaler_2   2分频   
//SPI_BaudRatePrescaler_8   8分频   
//SPI_BaudRatePrescaler_16  16分频  
//SPI_BaudRatePrescaler_256 256分频 
  
void SPI1_SetSpeed(uchar SPI_BaudRatePrescaler) {
  assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));
	SPI1->CR1 &= 0XFFC7;
	SPI1->CR1 |= SPI_BaudRatePrescaler;	//设置SPI1速度 
	SPI_Cmd(SPI1, ENABLE); 

} 

//SPIx 读写一个字节
//TxData:要写入的字节
//返回值:读取到的字节
uchar SPI1_ReadWriteByte(uchar TxData) {		
	uchar retry=0;				 	
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET) //检查指定的SPI标志位设置与否:发送缓存空标志位
		{
		retry++;
		if(retry>200)return 0;
		}			  
	SPI_I2S_SendData(SPI1, TxData); //通过外设SPIx发送一个数据
	retry=0;

	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET)//检查指定的SPI标志位设置与否:接受缓存非空标志位
		{
		retry++;
		if(retry>200)return 0;
		}	  						    
	return SPI_I2S_ReceiveData(SPI1); //返回通过SPIx最近接收的数据					    
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






