#include "audioplay.h"
#include "ff.h"
#include "malloc.h"
//#include "usart.h"
//#include "wm8978.h"
#include "dev_i2s.h"
//#include "led.h"
//#include "lcd.h"
#include "sys_delay.h"
//#include "key.h"
#include "exfuns.h"  
//#include "text.h"
#include "string.h"  
#include "dev_hmilcd.h"
#include "battery_check.h"
#include "mp3play.h"
#include <stdio.h>
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F407开发板
//音乐播放器 应用代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2014/5/24
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	
 
//#define MUSIC_DIR "0:/MUSIC"
#define MUSIC_DIR "0:/MP3"

//音乐播放控制器
__audiodev audiodev;	  
__audiolist audiolist; 

//开始音频播放
void audio_start(void)
{
	audiodev.status=3<<0;//开始播放+非暂停
	I2S_Play_Start();
} 
//关闭音频播放
void audio_stop(void)
{
	audiodev.status=0;
	I2S_Play_Stop();
}  
//得到path路径下,目标文件的总个数
//path:路径		    
//返回值:总有效文件数
u16 audio_get_tnum(u8 *path)
{	  
	u8 res;
	u16 rval=0;
 	DIR tdir;	 		//临时目录
	FILINFO tfileinfo;	//临时文件信息		
	u8 *fn; 			 			   			     
    res=f_opendir(&tdir,(const TCHAR*)path); //打开目录
  	tfileinfo.lfsize=_MAX_LFN*2+1;						//长文件名最大长度
	tfileinfo.lfname=mymalloc(SRAMIN,tfileinfo.lfsize);	//为长文件缓存区分配内存
	if(res==FR_OK&&tfileinfo.lfname!=NULL)
	{
		while(1)//查询总的有效文件数
		{
	    res=f_readdir(&tdir,&tfileinfo);       		//读取目录下的一个文件
	    if(res!=FR_OK||tfileinfo.fname[0]==0)break;	//错误了/到末尾了,退出		  
			fn=(u8*)(*tfileinfo.lfname?tfileinfo.lfname:tfileinfo.fname);			 
			res=f_typetell(fn);	
			if((res&0XF0)==0X40)//取高四位,看看是不是音乐文件	
			{
				rval++;//有效文件数增加1
			}	    
		}  
	} 
	myfree(SRAMIN,tfileinfo.lfname);
	return rval;
}
//显示曲目索引
//index:当前索引
//total:总文件数
void audio_index_show(u16 index,u16 total)
{
	//显示当前曲目的索引,及总曲目数
	printf("index = %d   ",index);		//索引
	printf("total = %d\r\n",total); 	//总曲目				  	  
}
 
//显示播放时间,比特率 信息  
//totsec;音频文件总时间长度
//cursec:当前播放时间
//bitrate:比特率(位速)
void audio_msg_show(u32 totsec,u32 cursec,u32 bitrate)
{	
	static u16 playtime=0XFFFF;//播放时间标记	      
	if(playtime!=cursec)					//需要更新显示时间
	{
		playtime=cursec;
		//显示播放时间			 
		playtime=cursec;
		//显示播放时间	
		printf("PlayTime = %d:%d\r\n",playtime/60,playtime%60);    	 
		//显示总时间    	   
		printf("TotalTime = %d:%d\r\n",totsec/60, totsec%60); 		    
		//显示位率	
    printf("BitRate = %d Kbps\r\n", bitrate);	
	} 		 
}

//显示播放列表
void audio_list_show(u8 *path, u16 totwavnum)
{
	u8 res;
 	DIR tdir;	 		//临时目录
	FILINFO tfileinfo;	//临时文件信息		
	u8 *fn; 			 
	u16 tmp;
	u16 curindex;
	u16 *wavindextbl;	//音乐索引表
	u8 *pname;			//带路径的文件名
	char lcdindex = 0;
	char *plcdindex;
	
  tfileinfo.lfsize=_MAX_LFN*2+1;						//长文件名最大长度
	tfileinfo.lfname=mymalloc(SRAMIN,tfileinfo.lfsize);	//为长文件缓存区分配内存
	pname=mymalloc(SRAMIN,tfileinfo.lfsize);				//为带路径的文件名分配内存
	wavindextbl=mymalloc(SRAMIN,2*totwavnum);				//申请2*totwavnum个字节的内存,用于存放音乐文件索引
	
	res=f_opendir(&tdir,(const TCHAR*)path); //打开目录
	
	if(res==FR_OK)
	{
		curindex = 0;
		while(1)//查询总的有效文件数
		{
			tmp = tdir.index;
	    res=f_readdir(&tdir,&tfileinfo);       		//读取目录下的一个文件
	    if(res!=FR_OK||tfileinfo.fname[0]==0)break;	//错误了/到末尾了,退出		  
			fn=(u8*)(*tfileinfo.lfname?tfileinfo.lfname:tfileinfo.fname);			 
			res=f_typetell(fn);	
			if((res&0XF0)==0X40)//取高四位,看看是不是音乐文件	
			{
				wavindextbl[curindex] = tmp;
				curindex++;
			}
		}
	}

	curindex = 0;			
	res = f_opendir(&tdir,(const TCHAR*)MUSIC_DIR);
	while(res==FR_OK)
	{
		printf("curindex = %d\r\n",curindex);
		dir_sdi(&tdir,wavindextbl[curindex]);			//改变当前目录索引	
		res=f_readdir(&tdir,&tfileinfo);       		//读取目录下的一个文件
		if(res!=FR_OK||tfileinfo.fname[0]==0)break;	//错误了/到末尾了,退出
		fn=(u8*)(*tfileinfo.lfname?tfileinfo.lfname:tfileinfo.fname);
		strcpy((char*)pname,MUSIC_DIR"/");				//复制路径(目录)
		strcat((char*)pname,(const char*)fn);  			//将文件名接在后面
		printf("%s\r\n",fn);				//显示歌曲名字 

		lcdindex = curindex + 0x30;
		plcdindex = &lcdindex;
		printf("lcdindex = %d\r\n",lcdindex);
		HMISends("t");
		HMISends(plcdindex);
		HMISends(".txt=\"");
		HMISends((char*)fn);
		HMISends("\"");
		HMISendb(0xff);
		curindex++;
		if(curindex >= 11) break;
	//					if(curindex >= totwavnum) break;
	}
	myfree(SRAMIN,tfileinfo.lfname);	//释放内存			    
	myfree(SRAMIN,pname);				//释放内存			    
	myfree(SRAMIN,wavindextbl);			//释放内存	 
}

//播放音乐
void audio_play(void)
{
	u8 res;
 	DIR wavdir;	 		//目录
	FILINFO wavfileinfo;//文件信息
	u8 *fn;   			//长文件名
	u8 *pname;			//带路径的文件名
	u16 totwavnum; 		//音乐文件总数
	u16 curindex;		//图片当前索引
	u8 key;				//键值		  
 	u16 temp;
	u16 *wavindextbl;	//音乐索引表
	
//	WM8978_ADDA_Cfg(1,0);	//开启DAC
//	WM8978_Input_Cfg(0,0,0);//关闭输入通道
//	WM8978_Output_Cfg(1,0);	//开启DAC输出   
	
 	while(f_opendir(&wavdir,MUSIC_DIR))//打开音乐文件夹
 	{	    
		printf("music dir open error\n");			  
	} 	
//	printf("MUSIC_DIR OPEN\n");
	totwavnum=audio_get_tnum(MUSIC_DIR); //得到总有效文件数
	while(totwavnum==NULL)//音乐文件总数为0		
 	{	    
		printf("no file in %s\n",MUSIC_DIR);				  
	}	
	
  wavfileinfo.lfsize=_MAX_LFN*2+1;						//长文件名最大长度
	wavfileinfo.lfname=mymalloc(SRAMIN,wavfileinfo.lfsize);	//为长文件缓存区分配内存
 	pname=mymalloc(SRAMIN,wavfileinfo.lfsize);				//为带路径的文件名分配内存
 	wavindextbl=mymalloc(SRAMIN,2*totwavnum);				//申请2*totwavnum个字节的内存,用于存放音乐文件索引
 	while(wavfileinfo.lfname==NULL||pname==NULL||wavindextbl==NULL)//内存分配出错
 	{	    
		printf("memory malloc error\n");
	}  	 
 	//记录索引
    res=f_opendir(&wavdir,MUSIC_DIR); //打开目录
	if(res==FR_OK)
	{
		curindex=0;//当前索引为0
		while(1)//全部查询一遍
		{
			temp=wavdir.index;								//记录当前index
	        res=f_readdir(&wavdir,&wavfileinfo);       		//读取目录下的一个文件
	        if(res!=FR_OK||wavfileinfo.fname[0]==0)break;	//错误了/到末尾了,退出		  
     		fn=(u8*)(*wavfileinfo.lfname?wavfileinfo.lfname:wavfileinfo.fname);			 
			res=f_typetell(fn);	
			if((res&0XF0)==0X40)//取高四位,看看是不是音乐文件	
			{
				wavindextbl[curindex]=temp;//记录索引
				curindex++;
			}	    
		} 
	}

//		audio_list_show(MUSIC_DIR, totwavnum);
	
   	curindex=0;											//从0开始显示
   	res=f_opendir(&wavdir,(const TCHAR*)MUSIC_DIR); 	//打开目录
	while(res==FR_OK)//打开成功
	{	
		dir_sdi(&wavdir,wavindextbl[curindex]);			//改变当前目录索引	   
        res=f_readdir(&wavdir,&wavfileinfo);       		//读取目录下的一个文件
        if(res!=FR_OK||wavfileinfo.fname[0]==0)break;	//错误了/到末尾了,退出
     	fn=(u8*)(*wavfileinfo.lfname?wavfileinfo.lfname:wavfileinfo.fname);	
		strcpy((char*)pname,MUSIC_DIR"/");				//复制路径(目录)
		strcat((char*)pname,(const char*)fn);  			//将文件名接在后面
		printf("%s\n",fn);				//显示歌曲名字 
		HMISends("g0.txt=\"");
		HMISends((char*)fn);
		HMISends("\"");
		HMISendb(0xff);
		
		audio_index_show(curindex+1,totwavnum);
		key = audio_play_song(pname); 			 		//播放这个音频文件
		printf("key = %x\r\n",key);
		printf("curindex = %x\r\n",curindex);
		Battery_Vol();
		if(key == AP_PREV)
		{
			if(curindex) curindex--;
			else curindex = totwavnum - 1;
		}
		else if(key == AP_NEXT || key == AP_OK || key == AP_ERR)
		{
			curindex++;
			if(curindex >= totwavnum) curindex = 0;
		}else break;
	} 											  
	myfree(SRAMIN,wavfileinfo.lfname);	//释放内存			    
	myfree(SRAMIN,pname);				//释放内存			    
	myfree(SRAMIN,wavindextbl);			//释放内存	 
} 
//播放某个音频文件
u8 audio_play_song(u8* fname)
{
	u8 res;  
	res=f_typetell(fname); 
	switch(res)
	{
		case T_WAV:
			res=wav_play_song(fname);
			break;
		case T_MP3:
			res=mp3_play_song(fname);
			break;
		default://其他文件,自动跳转到下一曲
			printf("can't play:%s\r\n",fname);
			res = AP_NEXT;
			break;
	}
	return res;
}



























