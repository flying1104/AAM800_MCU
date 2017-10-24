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
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//���ֲ����� Ӧ�ô���	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/5/24
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	
 
//#define MUSIC_DIR "0:/MUSIC"
#define MUSIC_DIR "0:/MP3"

//���ֲ��ſ�����
__audiodev audiodev;	  
__audiolist audiolist; 

//��ʼ��Ƶ����
void audio_start(void)
{
	audiodev.status=3<<0;//��ʼ����+����ͣ
	I2S_Play_Start();
} 
//�ر���Ƶ����
void audio_stop(void)
{
	audiodev.status=0;
	I2S_Play_Stop();
}  
//�õ�path·����,Ŀ���ļ����ܸ���
//path:·��		    
//����ֵ:����Ч�ļ���
u16 audio_get_tnum(u8 *path)
{	  
	u8 res;
	u16 rval=0;
 	DIR tdir;	 		//��ʱĿ¼
	FILINFO tfileinfo;	//��ʱ�ļ���Ϣ		
	u8 *fn; 			 			   			     
    res=f_opendir(&tdir,(const TCHAR*)path); //��Ŀ¼
  	tfileinfo.lfsize=_MAX_LFN*2+1;						//���ļ�����󳤶�
	tfileinfo.lfname=mymalloc(SRAMIN,tfileinfo.lfsize);	//Ϊ���ļ������������ڴ�
	if(res==FR_OK&&tfileinfo.lfname!=NULL)
	{
		while(1)//��ѯ�ܵ���Ч�ļ���
		{
	    res=f_readdir(&tdir,&tfileinfo);       		//��ȡĿ¼�µ�һ���ļ�
	    if(res!=FR_OK||tfileinfo.fname[0]==0)break;	//������/��ĩβ��,�˳�		  
			fn=(u8*)(*tfileinfo.lfname?tfileinfo.lfname:tfileinfo.fname);			 
			res=f_typetell(fn);	
			if((res&0XF0)==0X40)//ȡ����λ,�����ǲ��������ļ�	
			{
				rval++;//��Ч�ļ�������1
			}	    
		}  
	} 
	myfree(SRAMIN,tfileinfo.lfname);
	return rval;
}
//��ʾ��Ŀ����
//index:��ǰ����
//total:���ļ���
void audio_index_show(u16 index,u16 total)
{
	//��ʾ��ǰ��Ŀ������,������Ŀ��
	printf("index = %d   ",index);		//����
	printf("total = %d\r\n",total); 	//����Ŀ				  	  
}
 
//��ʾ����ʱ��,������ ��Ϣ  
//totsec;��Ƶ�ļ���ʱ�䳤��
//cursec:��ǰ����ʱ��
//bitrate:������(λ��)
void audio_msg_show(u32 totsec,u32 cursec,u32 bitrate)
{	
	static u16 playtime=0XFFFF;//����ʱ����	      
	if(playtime!=cursec)					//��Ҫ������ʾʱ��
	{
		playtime=cursec;
		//��ʾ����ʱ��			 
		playtime=cursec;
		//��ʾ����ʱ��	
		printf("PlayTime = %d:%d\r\n",playtime/60,playtime%60);    	 
		//��ʾ��ʱ��    	   
		printf("TotalTime = %d:%d\r\n",totsec/60, totsec%60); 		    
		//��ʾλ��	
    printf("BitRate = %d Kbps\r\n", bitrate);	
	} 		 
}

//��ʾ�����б�
void audio_list_show(u8 *path, u16 totwavnum)
{
	u8 res;
 	DIR tdir;	 		//��ʱĿ¼
	FILINFO tfileinfo;	//��ʱ�ļ���Ϣ		
	u8 *fn; 			 
	u16 tmp;
	u16 curindex;
	u16 *wavindextbl;	//����������
	u8 *pname;			//��·�����ļ���
	char lcdindex = 0;
	char *plcdindex;
	
  tfileinfo.lfsize=_MAX_LFN*2+1;						//���ļ�����󳤶�
	tfileinfo.lfname=mymalloc(SRAMIN,tfileinfo.lfsize);	//Ϊ���ļ������������ڴ�
	pname=mymalloc(SRAMIN,tfileinfo.lfsize);				//Ϊ��·�����ļ��������ڴ�
	wavindextbl=mymalloc(SRAMIN,2*totwavnum);				//����2*totwavnum���ֽڵ��ڴ�,���ڴ�������ļ�����
	
	res=f_opendir(&tdir,(const TCHAR*)path); //��Ŀ¼
	
	if(res==FR_OK)
	{
		curindex = 0;
		while(1)//��ѯ�ܵ���Ч�ļ���
		{
			tmp = tdir.index;
	    res=f_readdir(&tdir,&tfileinfo);       		//��ȡĿ¼�µ�һ���ļ�
	    if(res!=FR_OK||tfileinfo.fname[0]==0)break;	//������/��ĩβ��,�˳�		  
			fn=(u8*)(*tfileinfo.lfname?tfileinfo.lfname:tfileinfo.fname);			 
			res=f_typetell(fn);	
			if((res&0XF0)==0X40)//ȡ����λ,�����ǲ��������ļ�	
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
		dir_sdi(&tdir,wavindextbl[curindex]);			//�ı䵱ǰĿ¼����	
		res=f_readdir(&tdir,&tfileinfo);       		//��ȡĿ¼�µ�һ���ļ�
		if(res!=FR_OK||tfileinfo.fname[0]==0)break;	//������/��ĩβ��,�˳�
		fn=(u8*)(*tfileinfo.lfname?tfileinfo.lfname:tfileinfo.fname);
		strcpy((char*)pname,MUSIC_DIR"/");				//����·��(Ŀ¼)
		strcat((char*)pname,(const char*)fn);  			//���ļ������ں���
		printf("%s\r\n",fn);				//��ʾ�������� 

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
	myfree(SRAMIN,tfileinfo.lfname);	//�ͷ��ڴ�			    
	myfree(SRAMIN,pname);				//�ͷ��ڴ�			    
	myfree(SRAMIN,wavindextbl);			//�ͷ��ڴ�	 
}

//��������
void audio_play(void)
{
	u8 res;
 	DIR wavdir;	 		//Ŀ¼
	FILINFO wavfileinfo;//�ļ���Ϣ
	u8 *fn;   			//���ļ���
	u8 *pname;			//��·�����ļ���
	u16 totwavnum; 		//�����ļ�����
	u16 curindex;		//ͼƬ��ǰ����
	u8 key;				//��ֵ		  
 	u16 temp;
	u16 *wavindextbl;	//����������
	
//	WM8978_ADDA_Cfg(1,0);	//����DAC
//	WM8978_Input_Cfg(0,0,0);//�ر�����ͨ��
//	WM8978_Output_Cfg(1,0);	//����DAC���   
	
 	while(f_opendir(&wavdir,MUSIC_DIR))//�������ļ���
 	{	    
		printf("music dir open error\n");			  
	} 	
//	printf("MUSIC_DIR OPEN\n");
	totwavnum=audio_get_tnum(MUSIC_DIR); //�õ�����Ч�ļ���
	while(totwavnum==NULL)//�����ļ�����Ϊ0		
 	{	    
		printf("no file in %s\n",MUSIC_DIR);				  
	}	
	
  wavfileinfo.lfsize=_MAX_LFN*2+1;						//���ļ�����󳤶�
	wavfileinfo.lfname=mymalloc(SRAMIN,wavfileinfo.lfsize);	//Ϊ���ļ������������ڴ�
 	pname=mymalloc(SRAMIN,wavfileinfo.lfsize);				//Ϊ��·�����ļ��������ڴ�
 	wavindextbl=mymalloc(SRAMIN,2*totwavnum);				//����2*totwavnum���ֽڵ��ڴ�,���ڴ�������ļ�����
 	while(wavfileinfo.lfname==NULL||pname==NULL||wavindextbl==NULL)//�ڴ�������
 	{	    
		printf("memory malloc error\n");
	}  	 
 	//��¼����
    res=f_opendir(&wavdir,MUSIC_DIR); //��Ŀ¼
	if(res==FR_OK)
	{
		curindex=0;//��ǰ����Ϊ0
		while(1)//ȫ����ѯһ��
		{
			temp=wavdir.index;								//��¼��ǰindex
	        res=f_readdir(&wavdir,&wavfileinfo);       		//��ȡĿ¼�µ�һ���ļ�
	        if(res!=FR_OK||wavfileinfo.fname[0]==0)break;	//������/��ĩβ��,�˳�		  
     		fn=(u8*)(*wavfileinfo.lfname?wavfileinfo.lfname:wavfileinfo.fname);			 
			res=f_typetell(fn);	
			if((res&0XF0)==0X40)//ȡ����λ,�����ǲ��������ļ�	
			{
				wavindextbl[curindex]=temp;//��¼����
				curindex++;
			}	    
		} 
	}

//		audio_list_show(MUSIC_DIR, totwavnum);
	
   	curindex=0;											//��0��ʼ��ʾ
   	res=f_opendir(&wavdir,(const TCHAR*)MUSIC_DIR); 	//��Ŀ¼
	while(res==FR_OK)//�򿪳ɹ�
	{	
		dir_sdi(&wavdir,wavindextbl[curindex]);			//�ı䵱ǰĿ¼����	   
        res=f_readdir(&wavdir,&wavfileinfo);       		//��ȡĿ¼�µ�һ���ļ�
        if(res!=FR_OK||wavfileinfo.fname[0]==0)break;	//������/��ĩβ��,�˳�
     	fn=(u8*)(*wavfileinfo.lfname?wavfileinfo.lfname:wavfileinfo.fname);	
		strcpy((char*)pname,MUSIC_DIR"/");				//����·��(Ŀ¼)
		strcat((char*)pname,(const char*)fn);  			//���ļ������ں���
		printf("%s\n",fn);				//��ʾ�������� 
		HMISends("g0.txt=\"");
		HMISends((char*)fn);
		HMISends("\"");
		HMISendb(0xff);
		
		audio_index_show(curindex+1,totwavnum);
		key = audio_play_song(pname); 			 		//���������Ƶ�ļ�
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
	myfree(SRAMIN,wavfileinfo.lfname);	//�ͷ��ڴ�			    
	myfree(SRAMIN,pname);				//�ͷ��ڴ�			    
	myfree(SRAMIN,wavindextbl);			//�ͷ��ڴ�	 
} 
//����ĳ����Ƶ�ļ�
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
		default://�����ļ�,�Զ���ת����һ��
			printf("can't play:%s\r\n",fname);
			res = AP_NEXT;
			break;
	}
	return res;
}



























