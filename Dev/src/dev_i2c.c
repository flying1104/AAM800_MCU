#include "types.h"
#include "stm32f4xx.h"
#include "dev_pin_conf.h"
#include "sys_delay.h"
#include "dev_i2c.h"


void IIC_Init(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(I2C_GPIO_CLK,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = SCL_PIN | SDA_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(I2C_GPIO, &GPIO_InitStructure);
	
	SCL_WR = 1;
	SDA_WR = 1;
}

void IIC_Module_Init(uchar *tab,uchar dev_addr) {
   uchar r,m,v;
   while(1){
		r = *tab++;
	  m = *tab++;
	  v = *tab++;
	  if(r == 0xff){
	     if(m == 0xff)
		    break;
	     else
		    Delay_Ms(m);
	  }
	  else{
	     IIC_Write_Module(r,m,v,dev_addr);
	  }
   }   
}

void IIC_Write_Module(uchar reg,uchar mask,uchar data,uchar dev_addr) {
	 uchar val;
   if(mask == 0xff)
      IIC_Write(dev_addr,reg,data);
   else{
			val = IIC_Read(dev_addr,reg);
			val &= ~mask;
			val |= (data&mask);
			IIC_Write(dev_addr,reg,val);
   }     
}

void IIC_Write(uchar dev_addr, uchar data_addr, uchar data){
	IIC_Start();
	IIC_Send_Byte(dev_addr);
	IIC_Wait_Ack();
	IIC_Send_Byte(data_addr);
	IIC_Wait_Ack();
	IIC_Send_Byte(data);
	IIC_Wait_Ack();
	IIC_Stop();
	Delay_Ms(10);
}

void IIC_Write_16B(uchar dev_addr, uchar data_addr, ushort data){
	IIC_Start();
	IIC_Send_Byte(dev_addr);
	IIC_Wait_Ack();
	IIC_Send_Byte(data_addr);
	IIC_Wait_Ack();
	IIC_Send_Byte((uchar)data >> 8);
	IIC_Wait_Ack();
	IIC_Send_Byte((uchar)data);
	IIC_Wait_Ack();
	IIC_Stop();
	Delay_Ms(10);
}


uchar IIC_Read(uchar dev_addr, uchar data_addr){
	uchar tmp = 0;
	
	IIC_Start();
	IIC_Send_Byte(dev_addr);
	IIC_Wait_Ack();
	IIC_Send_Byte(data_addr);
	IIC_Wait_Ack();
	IIC_Start();
	IIC_Send_Byte(dev_addr + 1);
	IIC_Wait_Ack();
	tmp = IIC_Read_Byte(0);
	IIC_Stop();
  Delay_Ms(10);	
	
	return tmp;
}

ushort IIC_Read_16B(uchar dev_addr, uchar data_addr){
	ushort tmp = 0;
	
	IIC_Start();
	IIC_Send_Byte(dev_addr);
	IIC_Wait_Ack();
	IIC_Send_Byte(data_addr);
	IIC_Wait_Ack();
	IIC_Start();
	IIC_Send_Byte(dev_addr + 1);
	IIC_Wait_Ack();
	tmp = IIC_Read_Byte(1);
	tmp <<= 8;
	tmp |= IIC_Read_Byte(0);
	IIC_Stop();
	
	return tmp;
}

void IIC_Start(void){
	SDA_OUT_SET;
	SDA_WR = 1;
	SCL_WR = 1;
	Delay_Us(4);
	SDA_WR = 0;
	Delay_Us(4);
	SCL_WR = 0;
}

void IIC_Stop(void){
	SDA_OUT_SET;
	SCL_WR = 0;
	SDA_WR = 0;
	Delay_Us(4);
	SCL_WR = 1;
	SDA_WR = 1;
	Delay_Us(4);
}

uchar IIC_Wait_Ack(void){
	uchar err = 0;
	SDA_IN_SET;
	SDA_WR = 1;
	Delay_Us(1);
	SCL_WR = 1;
	Delay_Us(1);
	while(SDA_RD){
		err++;
		if(err > 250){
			IIC_Stop();
			return 1;
		}
	}
	SCL_WR = 0;
	return 0;
}

void IIC_Ack(void){
	SCL_WR = 0;
	SDA_OUT_SET;
	SDA_WR = 0;
	Delay_Us(2);
	SCL_WR = 1;
	Delay_Us(2);
	SCL_WR = 0;
}

void IIC_NAck(void){
	SCL_WR = 0;
	SDA_OUT_SET;
	SDA_WR = 1;
	Delay_Us(2);
	SCL_WR = 1;
	Delay_Us(2);
	SCL_WR = 0;
}

void IIC_Send_Byte(uchar data){
	uchar i;
	SDA_OUT_SET;
	SCL_WR = 0;
	for(i = 0; i < 8; i++){
		SDA_WR = (data & 0x80) >> 7;
		data <<= 1;
		Delay_Us(2);
		SCL_WR = 1;
		Delay_Us(2);
		SCL_WR = 0;
		Delay_Us(2);
	}
}

uchar IIC_Read_Byte(uchar ack){
	uchar i,data = 0;
	SDA_IN_SET;
	for(i = 0; i < 8; i++){
		SCL_WR = 0;
		Delay_Us(2);
		SCL_WR = 1;
		data <<= 1;
		if(SDA_RD)data++;
		Delay_Us(1);
	}
	if(!ack)
		IIC_NAck();
	else
		IIC_Ack();
	
	return data;
}


















