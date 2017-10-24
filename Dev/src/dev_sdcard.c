#include "types.h"
#include "stm32f10x.h"
#include "dev_spi.h"
#include "dev_sdcard.h"
#include <stdio.h>



uchar SDCard_Type = 0;//SD�������� 


//��ֲʱ��Ľӿ�
//data:Ҫд�������
//����ֵ:����������
uchar SDCard_SPI_ReadWriteByte(uchar data) {
	return SPI1_ReadWriteByte(data);
}	  

//SD����ʼ����ʱ��,��Ҫ����
void SDCard_SPI_SpeedLow(void) {
 	SPI1_SetSpeed(SPI_BaudRatePrescaler_256);//���õ�����ģʽ	
}

//SD������������ʱ��,���Ը�����
void SDCard_SPI_SpeedHigh(void) {
 	SPI1_SetSpeed(SPI_BaudRatePrescaler_2);//���õ�����ģʽ	
}

///////////////////////////////////////////////////////////////////////////////////
//ȡ��ѡ��,�ͷ�SPI����
void SDCard_DisSelect(void) {
	SDCARD_CS = 1;
 	SDCard_SPI_ReadWriteByte(0xff);//�ṩ�����8��ʱ��
}

//ѡ��sd��,���ҵȴ���׼��OK
//����ֵ:0,�ɹ�;1,ʧ��;
uchar SDCard_Select(void) {
	SDCARD_CS = 0;
	if(SDCard_WaitReady() == 0)return 0;//�ȴ��ɹ�
	SDCard_DisSelect();
	return 1;//�ȴ�ʧ��
}

//�ȴ���׼����
//����ֵ:0,׼������;����,�������
uchar SDCard_WaitReady(void) {
	ulong t = 0;
	do
	{
		if(SDCard_SPI_ReadWriteByte(0XFF) == 0XFF) return 0;//OK
		t++;		  	
	}while(t<0XFFFFFF);//�ȴ� 
	
	return 1;
}

//�ȴ�SD����Ӧ
//Response:Ҫ�õ��Ļ�Ӧֵ
//����ֵ:0,�ɹ��õ��˸û�Ӧֵ
//    ����,�õ���Ӧֵʧ��
uchar SDCard_GetResponse(uchar Response) {
	ushort Count = 0xFFFF;//�ȴ�����	   
	
	while ((SDCard_SPI_ReadWriteByte(0XFF) != Response) && Count) Count--;//�ȴ��õ�׼ȷ�Ļ�Ӧ  	
  
	if (Count == 0) return MSD_RESPONSE_FAILURE;//�õ���Ӧʧ��   
	else return MSD_RESPONSE_NO_ERROR;//��ȷ��Ӧ
}

//��sd����ȡһ�����ݰ�������
//buf:���ݻ�����
//len:Ҫ��ȡ�����ݳ���.
//����ֵ:0,�ɹ�;����,ʧ��;	
uchar SDCard_RecvData(uchar *buf, ushort len) {			  	  
	if(SDCard_GetResponse(0xFE)) return 1;//�ȴ�SD������������ʼ����0xFE
    while(len--)//��ʼ��������
    {
        *buf=SPI1_ReadWriteByte(0xFF);
        buf++;
    }
    //������2��αCRC��dummy CRC��
    SDCard_SPI_ReadWriteByte(0xFF);
    SDCard_SPI_ReadWriteByte(0xFF);	
		
    return 0;//��ȡ�ɹ�
}

//��sd��д��һ�����ݰ������� 512�ֽ�
//buf:���ݻ�����
//cmd:ָ��
//����ֵ:0,�ɹ�;����,ʧ��;	
uchar SDCard_SendBlock(uchar *buf, uchar cmd) {	
	ushort t;		  	  
	if(SDCard_WaitReady()) return 1;//�ȴ�׼��ʧЧ
	
	SDCard_SPI_ReadWriteByte(cmd);
	
	if(cmd!=0XFD)//���ǽ���ָ��
	{
		for(t=0;t<512;t++)SPI1_ReadWriteByte(buf[t]);//����ٶ�,���ٺ�������ʱ��
	    SDCard_SPI_ReadWriteByte(0xFF);//����crc
	    SDCard_SPI_ReadWriteByte(0xFF);
			t = SDCard_SPI_ReadWriteByte(0xFF);//������Ӧ
		
		if((t&0x1F)!=0x05)return 2;//��Ӧ����									  					    
	}						 									  					    
    return 0;//д��ɹ�
}

//��SD������һ������
//����: u8 cmd   ���� 
//      u32 arg  �������
//      u8 crc   crcУ��ֵ	   
//����ֵ:SD�����ص���Ӧ															  
uchar SDCard_SendCmd(uchar cmd, ulong arg, uchar crc) {
  uchar r1;	
	uchar Retry=0; 
	
	SDCard_DisSelect();//ȡ���ϴ�Ƭѡ
	if(SDCard_Select()) return 0XFF;//ƬѡʧЧ 
	//����
    SDCard_SPI_ReadWriteByte(cmd | 0x40);//�ֱ�д������
    SDCard_SPI_ReadWriteByte(arg >> 24);
    SDCard_SPI_ReadWriteByte(arg >> 16);
    SDCard_SPI_ReadWriteByte(arg >> 8);
    SDCard_SPI_ReadWriteByte(arg);	  
    SDCard_SPI_ReadWriteByte(crc); 
	
	if(cmd == CMD12) SDCard_SPI_ReadWriteByte(0xff);//Skip a stuff byte when stop reading
    //�ȴ���Ӧ����ʱ�˳�
	Retry=0X1F;
	do
	{
		r1 = SDCard_SPI_ReadWriteByte(0xFF);
	}while((r1&0X80) && Retry--);	 
	//����״ֵ̬
    return r1;
}		    																			  
//��ȡSD����CID��Ϣ��������������Ϣ
//����: u8 *cid_data(���CID���ڴ棬����16Byte��	  
//����ֵ:0��NO_ERR
//		 1������														   
uchar SDCard_GetCID(uchar *cid_data) {
	uchar r1;	   
	//��CMD10�����CID
	r1 = SDCard_SendCmd(CMD10,0,0x01);

	if(r1==0x00) {
		r1=SDCard_RecvData(cid_data,16);//����16���ֽڵ�����	 
	}
	
	SDCard_DisSelect();//ȡ��Ƭѡ
	if(r1) return 1;
	else return 0;
}								

//��ȡSD����CSD��Ϣ�������������ٶ���Ϣ
//����:u8 *cid_data(���CID���ڴ棬����16Byte��	    
//����ֵ:0��NO_ERR
//		 1������														   
uchar SDCard_GetCSD(uchar *csd_data) {
	uchar r1;	 
	
	r1 = SDCard_SendCmd(CMD9,0,0x01);//��CMD9�����CSD
	
	if(r1==0) {
		r1 = SDCard_RecvData(csd_data, 16);//����16���ֽڵ����� 
	}
	
	SDCard_DisSelect();//ȡ��Ƭѡ
	if(r1)return 1;
	else return 0;
}  
//��ȡSD����������������������   
//����ֵ:0�� ȡ�������� 
//       ����:SD��������(������/512�ֽ�)
//ÿ�������ֽ�����Ϊ512����Ϊ�������512�����ʼ������ͨ��.														  
ulong SDCard_GetSectorCount(void) {
	uchar csd[16];
	ulong Capacity;  
	uchar n;
	ushort csize;  

//ȡCSD��Ϣ������ڼ��������0
	if(SDCard_GetCSD(csd) != 0) return 0;	    
	//���ΪSDHC�����������淽ʽ����
	if((csd[0] & 0xC0) == 0x40)	 //V2.00�Ŀ�
	{	
	csize = csd[9] + ((ushort)csd[8] << 8) + 1;
	Capacity = (ulong)csize << 10;//�õ�������	 		   
	}else//V1.XX�Ŀ�
	{	
	n = (csd[5] & 15) + ((csd[10] & 128) >> 7) + ((csd[9] & 3) << 1) + 2;
	csize = (csd[8] >> 6) + ((u16)csd[7] << 2) + ((u16)(csd[6] & 3) << 10) + 1;
	Capacity= (ulong)csize << (n - 9);//�õ�������   
	}
	return Capacity;
}
//��ʼ��SD��
int MMC_disk_initialize(void) {
	uchar r1;      // ���SD���ķ���ֵ
	ulong retry;  // �������г�ʱ����
	uchar buf[4];  
	ushort i;

	Spi_HW_Init();		//��ʼ��IO
	
 	SDCard_SPI_SpeedLow();	//���õ�����ģʽ 
 	for(i=0;i<10;i++) SDCard_SPI_ReadWriteByte(0XFF);//��������74������
	
	retry=20;
	do
	{
		r1 = SDCard_SendCmd(CMD0,0,0x95);//����IDLE״̬
	}while((r1!=0X01) && retry--);

 	SDCard_Type=0;//Ĭ���޿�
	
	if(r1==0X01)
	{
		if(SDCard_SendCmd(CMD8,0x1AA,0x87) == 1)//SD V2.0
		{
			for(i=0;i<4;i++) buf[i] = SDCard_SPI_ReadWriteByte(0XFF);	//Get trailing return value of R7 resp
			if(buf[2]==0X01&&buf[3]==0XAA)//���Ƿ�֧��2.7~3.6V
			{
				retry=0XFFFE;
				do
				{
					SDCard_SendCmd(CMD55,0,0X01);	//����CMD55
					r1 = SDCard_SendCmd(CMD41,0x40000000,0X01);//����CMD41
				}while(r1&&retry--);
				if(retry&&SDCard_SendCmd(CMD58,0,0X01)==0)//����SD2.0���汾��ʼ
				{
					for(i=0;i<4;i++) buf[i] = SDCard_SPI_ReadWriteByte(0XFF);//�õ�OCRֵ
					if(buf[0]&0x40) SDCard_Type = SD_TYPE_V2HC;    //���CCS
					else SDCard_Type=SD_TYPE_V2;   
				}
			}
		}else//SD V1.x/ MMC	V3
		{
			SDCard_SendCmd(CMD55,0,0X01);		//����CMD55
			r1 = SDCard_SendCmd(CMD41,0,0X01);	//����CMD41
			if(r1<=1)
			{		
				SDCard_Type = SD_TYPE_V1;
				retry=0XFFFE;
				do //�ȴ��˳�IDLEģʽ
				{
					SDCard_SendCmd(CMD55,0,0X01);	//����CMD55
					r1 = SDCard_SendCmd(CMD41,0,0X01);//����CMD41
				}while(r1&&retry--);
			}else
			{
				SDCard_Type = SD_TYPE_MMC;//MMC V3
				retry=0XFFFE;
				do //�ȴ��˳�IDLEģʽ
				{											    
					r1 = SDCard_SendCmd(CMD1,0,0X01);//����CMD1
				}while(r1&&retry--);  
			}
			if(retry==0||SDCard_SendCmd(CMD16,512,0X01)!=0) SDCard_Type = SD_TYPE_ERR;//����Ŀ�
		}
	}
	SDCard_DisSelect();//ȡ��Ƭѡ
	SDCard_SPI_SpeedHigh();//����
	if(SDCard_Type)return 0;
	else if(r1)return r1; 	   
	return 0xaa;//��������
}
//��SD��
//buf:���ݻ�����
//sector:����
//cnt:������
//����ֵ:0,ok;����,ʧ��.
int MMC_disk_read(uchar *buf, ulong sector, uint cnt) {
	uchar r1;
	if(SDCard_Type != SD_TYPE_V2HC) sector <<= 9;//ת��Ϊ�ֽڵ�ַ
	if(cnt==1)
	{
		r1 = SDCard_SendCmd(CMD17,sector,0X01);//������
		if(r1 == 0)//ָ��ͳɹ�
		{
			r1 = SDCard_RecvData(buf,512);//����512���ֽ�	   
		}
	}else
	{
		r1 = SDCard_SendCmd(CMD18,sector,0X01);//����������
		do
		{
			r1 = SDCard_RecvData(buf,512);//����512���ֽ�	 
			buf+=512;  
		}while(--cnt && r1==0); 	
		SDCard_SendCmd(CMD12,0,0X01);	//����ֹͣ����
	}   
	SDCard_DisSelect();//ȡ��Ƭѡ
	return r1;//
}
//дSD��
//buf:���ݻ�����
//sector:��ʼ����
//cnt:������
//����ֵ:0,ok;����,ʧ��.
int MMC_disk_write(uchar *buf, ulong sector, uint cnt) {
	uchar r1;
	
	if(SDCard_Type != SD_TYPE_V2HC) sector *= 512;//ת��Ϊ�ֽڵ�ַ
	if(cnt==1)
	{
		r1 = SDCard_SendCmd(CMD24,sector,0X01);//������
		if(r1==0)//ָ��ͳɹ�
		{
			r1 = SDCard_SendBlock(buf,0xFE);//д512���ֽ�	   
		}
	}else
	{
		if(SDCard_Type != SD_TYPE_MMC)
		{
			SDCard_SendCmd(CMD55,0,0X01);	
			SDCard_SendCmd(CMD23,cnt,0X01);//����ָ��	
		}
 		r1 = SDCard_SendCmd(CMD25,sector,0X01);//����������
		if(r1==0)
		{
			do
			{
				r1 = SDCard_SendBlock(buf,0xFC);//����512���ֽ�	 
				buf += 512;  
			}while(--cnt && r1==0);
			r1 = SDCard_SendBlock(0,0xFD);//����512���ֽ� 
		}
	}   
	SDCard_DisSelect();//ȡ��Ƭѡ
	return r1;//
}	   

int MMC_disk_status(void) {
	return 0;
}
