#include "types.h"
#include "sys_delay.h"
#include "dev_i2c.h"
#include "dev_cs8406.h"

static uchar cs8406_inittab[]={
// reg 	mask 	val
  0x01,	0xff,	0x01,
	0x02,	0xff,	0x00,
	0x03,	0xff,	0x00,	
	0x05,	0xff,	0x05,
	0x04,	0xff,	0x40,
	0xff,	0xff,	0xff
};

void Cs8406_Init(void)
{
//	IIC_Write(CS8406_ADDR, 0x07, 0x03);
//	Delay_Ms(10);
		IIC_Module_Init(cs8406_inittab,CS8406_ADDR);
}

uchar Cs8406_Rd(uchar addr)
{
	uchar data;
	data = IIC_Read(CS8406_ADDR, addr);  
	return (data);
}

void Cs8406_Wr(uchar addr, uchar data)
{
   IIC_Write(CS8406_ADDR, addr, data);
}





















