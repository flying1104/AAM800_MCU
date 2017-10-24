#include "types.h"
#include "stm32f10x.h"
#include "dev_spi.h"
#include "dev_sdcard.h"
#include <stdio.h>



uchar SDCard_Type = 0;//SD卡的类型 


//移植时候的接口
//data:要写入的数据
//返回值:读到的数据
uchar SDCard_SPI_ReadWriteByte(uchar data) {
	return SPI1_ReadWriteByte(data);
}	  

//SD卡初始化的时候,需要低速
void SDCard_SPI_SpeedLow(void) {
 	SPI1_SetSpeed(SPI_BaudRatePrescaler_256);//设置到低速模式	
}

//SD卡正常工作的时候,可以高速了
void SDCard_SPI_SpeedHigh(void) {
 	SPI1_SetSpeed(SPI_BaudRatePrescaler_2);//设置到高速模式	
}

///////////////////////////////////////////////////////////////////////////////////
//取消选择,释放SPI总线
void SDCard_DisSelect(void) {
	SDCARD_CS = 1;
 	SDCard_SPI_ReadWriteByte(0xff);//提供额外的8个时钟
}

//选择sd卡,并且等待卡准备OK
//返回值:0,成功;1,失败;
uchar SDCard_Select(void) {
	SDCARD_CS = 0;
	if(SDCard_WaitReady() == 0)return 0;//等待成功
	SDCard_DisSelect();
	return 1;//等待失败
}

//等待卡准备好
//返回值:0,准备好了;其他,错误代码
uchar SDCard_WaitReady(void) {
	ulong t = 0;
	do
	{
		if(SDCard_SPI_ReadWriteByte(0XFF) == 0XFF) return 0;//OK
		t++;		  	
	}while(t<0XFFFFFF);//等待 
	
	return 1;
}

//等待SD卡回应
//Response:要得到的回应值
//返回值:0,成功得到了该回应值
//    其他,得到回应值失败
uchar SDCard_GetResponse(uchar Response) {
	ushort Count = 0xFFFF;//等待次数	   
	
	while ((SDCard_SPI_ReadWriteByte(0XFF) != Response) && Count) Count--;//等待得到准确的回应  	
  
	if (Count == 0) return MSD_RESPONSE_FAILURE;//得到回应失败   
	else return MSD_RESPONSE_NO_ERROR;//正确回应
}

//从sd卡读取一个数据包的内容
//buf:数据缓存区
//len:要读取的数据长度.
//返回值:0,成功;其他,失败;	
uchar SDCard_RecvData(uchar *buf, ushort len) {			  	  
	if(SDCard_GetResponse(0xFE)) return 1;//等待SD卡发回数据起始令牌0xFE
    while(len--)//开始接收数据
    {
        *buf=SPI1_ReadWriteByte(0xFF);
        buf++;
    }
    //下面是2个伪CRC（dummy CRC）
    SDCard_SPI_ReadWriteByte(0xFF);
    SDCard_SPI_ReadWriteByte(0xFF);	
		
    return 0;//读取成功
}

//向sd卡写入一个数据包的内容 512字节
//buf:数据缓存区
//cmd:指令
//返回值:0,成功;其他,失败;	
uchar SDCard_SendBlock(uchar *buf, uchar cmd) {	
	ushort t;		  	  
	if(SDCard_WaitReady()) return 1;//等待准备失效
	
	SDCard_SPI_ReadWriteByte(cmd);
	
	if(cmd!=0XFD)//不是结束指令
	{
		for(t=0;t<512;t++)SPI1_ReadWriteByte(buf[t]);//提高速度,减少函数传参时间
	    SDCard_SPI_ReadWriteByte(0xFF);//忽略crc
	    SDCard_SPI_ReadWriteByte(0xFF);
			t = SDCard_SPI_ReadWriteByte(0xFF);//接收响应
		
		if((t&0x1F)!=0x05)return 2;//响应错误									  					    
	}						 									  					    
    return 0;//写入成功
}

//向SD卡发送一个命令
//输入: u8 cmd   命令 
//      u32 arg  命令参数
//      u8 crc   crc校验值	   
//返回值:SD卡返回的响应															  
uchar SDCard_SendCmd(uchar cmd, ulong arg, uchar crc) {
  uchar r1;	
	uchar Retry=0; 
	
	SDCard_DisSelect();//取消上次片选
	if(SDCard_Select()) return 0XFF;//片选失效 
	//发送
    SDCard_SPI_ReadWriteByte(cmd | 0x40);//分别写入命令
    SDCard_SPI_ReadWriteByte(arg >> 24);
    SDCard_SPI_ReadWriteByte(arg >> 16);
    SDCard_SPI_ReadWriteByte(arg >> 8);
    SDCard_SPI_ReadWriteByte(arg);	  
    SDCard_SPI_ReadWriteByte(crc); 
	
	if(cmd == CMD12) SDCard_SPI_ReadWriteByte(0xff);//Skip a stuff byte when stop reading
    //等待响应，或超时退出
	Retry=0X1F;
	do
	{
		r1 = SDCard_SPI_ReadWriteByte(0xFF);
	}while((r1&0X80) && Retry--);	 
	//返回状态值
    return r1;
}		    																			  
//获取SD卡的CID信息，包括制造商信息
//输入: u8 *cid_data(存放CID的内存，至少16Byte）	  
//返回值:0：NO_ERR
//		 1：错误														   
uchar SDCard_GetCID(uchar *cid_data) {
	uchar r1;	   
	//发CMD10命令，读CID
	r1 = SDCard_SendCmd(CMD10,0,0x01);

	if(r1==0x00) {
		r1=SDCard_RecvData(cid_data,16);//接收16个字节的数据	 
	}
	
	SDCard_DisSelect();//取消片选
	if(r1) return 1;
	else return 0;
}								

//获取SD卡的CSD信息，包括容量和速度信息
//输入:u8 *cid_data(存放CID的内存，至少16Byte）	    
//返回值:0：NO_ERR
//		 1：错误														   
uchar SDCard_GetCSD(uchar *csd_data) {
	uchar r1;	 
	
	r1 = SDCard_SendCmd(CMD9,0,0x01);//发CMD9命令，读CSD
	
	if(r1==0) {
		r1 = SDCard_RecvData(csd_data, 16);//接收16个字节的数据 
	}
	
	SDCard_DisSelect();//取消片选
	if(r1)return 1;
	else return 0;
}  
//获取SD卡的总扇区数（扇区数）   
//返回值:0： 取容量出错 
//       其他:SD卡的容量(扇区数/512字节)
//每扇区的字节数必为512，因为如果不是512，则初始化不能通过.														  
ulong SDCard_GetSectorCount(void) {
	uchar csd[16];
	ulong Capacity;  
	uchar n;
	ushort csize;  

//取CSD信息，如果期间出错，返回0
	if(SDCard_GetCSD(csd) != 0) return 0;	    
	//如果为SDHC卡，按照下面方式计算
	if((csd[0] & 0xC0) == 0x40)	 //V2.00的卡
	{	
	csize = csd[9] + ((ushort)csd[8] << 8) + 1;
	Capacity = (ulong)csize << 10;//得到扇区数	 		   
	}else//V1.XX的卡
	{	
	n = (csd[5] & 15) + ((csd[10] & 128) >> 7) + ((csd[9] & 3) << 1) + 2;
	csize = (csd[8] >> 6) + ((u16)csd[7] << 2) + ((u16)(csd[6] & 3) << 10) + 1;
	Capacity= (ulong)csize << (n - 9);//得到扇区数   
	}
	return Capacity;
}
//初始化SD卡
int MMC_disk_initialize(void) {
	uchar r1;      // 存放SD卡的返回值
	ulong retry;  // 用来进行超时计数
	uchar buf[4];  
	ushort i;

	Spi_HW_Init();		//初始化IO
	
 	SDCard_SPI_SpeedLow();	//设置到低速模式 
 	for(i=0;i<10;i++) SDCard_SPI_ReadWriteByte(0XFF);//发送最少74个脉冲
	
	retry=20;
	do
	{
		r1 = SDCard_SendCmd(CMD0,0,0x95);//进入IDLE状态
	}while((r1!=0X01) && retry--);

 	SDCard_Type=0;//默认无卡
	
	if(r1==0X01)
	{
		if(SDCard_SendCmd(CMD8,0x1AA,0x87) == 1)//SD V2.0
		{
			for(i=0;i<4;i++) buf[i] = SDCard_SPI_ReadWriteByte(0XFF);	//Get trailing return value of R7 resp
			if(buf[2]==0X01&&buf[3]==0XAA)//卡是否支持2.7~3.6V
			{
				retry=0XFFFE;
				do
				{
					SDCard_SendCmd(CMD55,0,0X01);	//发送CMD55
					r1 = SDCard_SendCmd(CMD41,0x40000000,0X01);//发送CMD41
				}while(r1&&retry--);
				if(retry&&SDCard_SendCmd(CMD58,0,0X01)==0)//鉴别SD2.0卡版本开始
				{
					for(i=0;i<4;i++) buf[i] = SDCard_SPI_ReadWriteByte(0XFF);//得到OCR值
					if(buf[0]&0x40) SDCard_Type = SD_TYPE_V2HC;    //检查CCS
					else SDCard_Type=SD_TYPE_V2;   
				}
			}
		}else//SD V1.x/ MMC	V3
		{
			SDCard_SendCmd(CMD55,0,0X01);		//发送CMD55
			r1 = SDCard_SendCmd(CMD41,0,0X01);	//发送CMD41
			if(r1<=1)
			{		
				SDCard_Type = SD_TYPE_V1;
				retry=0XFFFE;
				do //等待退出IDLE模式
				{
					SDCard_SendCmd(CMD55,0,0X01);	//发送CMD55
					r1 = SDCard_SendCmd(CMD41,0,0X01);//发送CMD41
				}while(r1&&retry--);
			}else
			{
				SDCard_Type = SD_TYPE_MMC;//MMC V3
				retry=0XFFFE;
				do //等待退出IDLE模式
				{											    
					r1 = SDCard_SendCmd(CMD1,0,0X01);//发送CMD1
				}while(r1&&retry--);  
			}
			if(retry==0||SDCard_SendCmd(CMD16,512,0X01)!=0) SDCard_Type = SD_TYPE_ERR;//错误的卡
		}
	}
	SDCard_DisSelect();//取消片选
	SDCard_SPI_SpeedHigh();//高速
	if(SDCard_Type)return 0;
	else if(r1)return r1; 	   
	return 0xaa;//其他错误
}
//读SD卡
//buf:数据缓存区
//sector:扇区
//cnt:扇区数
//返回值:0,ok;其他,失败.
int MMC_disk_read(uchar *buf, ulong sector, uint cnt) {
	uchar r1;
	if(SDCard_Type != SD_TYPE_V2HC) sector <<= 9;//转换为字节地址
	if(cnt==1)
	{
		r1 = SDCard_SendCmd(CMD17,sector,0X01);//读命令
		if(r1 == 0)//指令发送成功
		{
			r1 = SDCard_RecvData(buf,512);//接收512个字节	   
		}
	}else
	{
		r1 = SDCard_SendCmd(CMD18,sector,0X01);//连续读命令
		do
		{
			r1 = SDCard_RecvData(buf,512);//接收512个字节	 
			buf+=512;  
		}while(--cnt && r1==0); 	
		SDCard_SendCmd(CMD12,0,0X01);	//发送停止命令
	}   
	SDCard_DisSelect();//取消片选
	return r1;//
}
//写SD卡
//buf:数据缓存区
//sector:起始扇区
//cnt:扇区数
//返回值:0,ok;其他,失败.
int MMC_disk_write(uchar *buf, ulong sector, uint cnt) {
	uchar r1;
	
	if(SDCard_Type != SD_TYPE_V2HC) sector *= 512;//转换为字节地址
	if(cnt==1)
	{
		r1 = SDCard_SendCmd(CMD24,sector,0X01);//读命令
		if(r1==0)//指令发送成功
		{
			r1 = SDCard_SendBlock(buf,0xFE);//写512个字节	   
		}
	}else
	{
		if(SDCard_Type != SD_TYPE_MMC)
		{
			SDCard_SendCmd(CMD55,0,0X01);	
			SDCard_SendCmd(CMD23,cnt,0X01);//发送指令	
		}
 		r1 = SDCard_SendCmd(CMD25,sector,0X01);//连续读命令
		if(r1==0)
		{
			do
			{
				r1 = SDCard_SendBlock(buf,0xFC);//接收512个字节	 
				buf += 512;  
			}while(--cnt && r1==0);
			r1 = SDCard_SendBlock(0,0xFD);//接收512个字节 
		}
	}   
	SDCard_DisSelect();//取消片选
	return r1;//
}	   

int MMC_disk_status(void) {
	return 0;
}
