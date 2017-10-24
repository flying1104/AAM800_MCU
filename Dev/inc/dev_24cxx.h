#ifndef _DEV_24CXX_
#define _DEV_24CXX_


/**********************************************
						AT24CXX_SET
***********************************************								

***********************************************/

#define AT24C04_ADDR	0xA0


/**********************************************
						AT24CXX_FUN_SET
***********************************************								

***********************************************/

uchar AT24CXX_ReadOneByte(ushort ReadAddr);							//指定地址读取一个字节
void AT24CXX_WriteOneByte(ushort WriteAddr,uchar DataToWrite);		//指定地址写入一个字节
void AT24CXX_WriteLenByte(ushort WriteAddr,uint DataToWrite,uchar Len);//指定地址开始写入指定长度的数据
uint AT24CXX_ReadLenByte(ushort ReadAddr,uchar Len);					//指定地址开始读取指定长度数据
void AT24CXX_Write(ushort WriteAddr,uchar *pBuffer,ushort NumToWrite);	//从指定地址开始写入指定长度的数据
void AT24CXX_Read(ushort ReadAddr,uchar *pBuffer,ushort NumToRead);   	//从指定地址开始读出指定长度的数据

uchar AT24CXX_Check(void);  //检查器件








#endif

















