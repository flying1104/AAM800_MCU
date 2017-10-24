#ifndef _DEV_I2C_
#define _DEV_I2C_

#include "dev_io.h"

/**********************************************
						IIC_SET
***********************************************								
1.SCL	--	PB6
2.SDA	--	PB7	
***********************************************/

//#define SDA_IN_SET  {GPIOB->CRL &= 0X0FFFFFFF; GPIOB->CRL |= (uint)8 << 28;}
//#define SDA_OUT_SET {GPIOB->CRL &= 0X0FFFFFFF; GPIOB->CRL |= (uint)3 << 28;}

#define SDA_IN_SET  {GPIOB->MODER &= ~(3 << (7 * 2)); GPIOB->MODER |= 0 << (7 * 2);}	//PB7输入模式
#define SDA_OUT_SET {GPIOB->MODER &= ~(3 << (7 * 2)); GPIOB->MODER |= 1 << (7 * 2);} //PB7输出模式

#define SCL_WR	PBout(6)	
#define SDA_WR	PBout(7)
#define SDA_RD	PBin(7)


/**********************************************
						IIC_FUN_SET
***********************************************								

***********************************************/

void IIC_Init(void);
void IIC_Module_Init(uchar *tab,uchar dev_addr);
void IIC_Write_Module(uchar reg,uchar mask,uchar data,uchar dev_addr);
void IIC_Write(uchar dev_addr, uchar data_addr, uchar data);
void IIC_Write_16B(uchar dev_addr, uchar data_addr, ushort data);
uchar IIC_Read(uchar dev_addr,uchar data_addr);
ushort IIC_Read_16B(uchar dev_addr, uchar data_addr);
void IIC_Start(void);
void IIC_Stop(void);
uchar IIC_Wait_Ack(void);
void IIC_Ack(void);
void IIC_NAck(void);
void IIC_Send_Byte(uchar data);
uchar IIC_Read_Byte(uchar ack);






#endif




















