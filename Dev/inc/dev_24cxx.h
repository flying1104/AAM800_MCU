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

uchar AT24CXX_ReadOneByte(ushort ReadAddr);							//ָ����ַ��ȡһ���ֽ�
void AT24CXX_WriteOneByte(ushort WriteAddr,uchar DataToWrite);		//ָ����ַд��һ���ֽ�
void AT24CXX_WriteLenByte(ushort WriteAddr,uint DataToWrite,uchar Len);//ָ����ַ��ʼд��ָ�����ȵ�����
uint AT24CXX_ReadLenByte(ushort ReadAddr,uchar Len);					//ָ����ַ��ʼ��ȡָ����������
void AT24CXX_Write(ushort WriteAddr,uchar *pBuffer,ushort NumToWrite);	//��ָ����ַ��ʼд��ָ�����ȵ�����
void AT24CXX_Read(ushort ReadAddr,uchar *pBuffer,ushort NumToRead);   	//��ָ����ַ��ʼ����ָ�����ȵ�����

uchar AT24CXX_Check(void);  //�������








#endif

















