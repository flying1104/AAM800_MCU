#ifndef __AUDIOPLAY_H
#define __AUDIOPLAY_H
#include "sys.h"
#include "ff.h"
#include "wavplay.h"
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
     

						  
#define I2S_TX_DMA_BUF_SIZE    8192		//����TX DMA �����С(����192Kbps@24bit��ʱ��,��Ҫ����8192��Ų��Ῠ)

//���ֲ��Ų����������
typedef enum {
	AP_OK=0X00,				//�����������
	AP_NEXT,				//������һ��
	AP_PREV,				//������һ��
	AP_ERR=0X80,			//�����д���(û����������,������ʾ����) 
}APRESULT;

/*
//���ֲ��ſ�����
typedef __packed struct
{  
	//2��I2S�����BUF
	u8 *i2sbuf1;
	u8 *i2sbuf2; 
	u8 *tbuf;				//��ʱ����,����24bit�����ʱ����Ҫ�õ�
	FIL *file;				//��Ƶ�ļ�ָ��
	
	u8 status;				//bit0:0,��ͣ����;1,��������
							//bit1:0,��������;1,�������� 
}__audiodev; 
extern __audiodev audiodev;	//���ֲ��ſ�����
*/
typedef __packed struct
{  
	//2��I2S�����BUF
	u8 *i2sbuf1;
	u8 *i2sbuf2; 
	u8 *tbuf;			//��ʱ����
	FIL *file;			//��Ƶ�ļ�ָ�� 	
	u32(*file_seek)(u32);//�ļ�������˺��� 

	vu8 status;			//bit0:0,��ͣ����;1,��������
						//bit1:0,��������;1,��������  
						//bit2~3:����
						//bit4:0,�����ֲ���;1,���ֲ����� (������)		
						//bit5:0,�޶���;1,ִ����һ���и����(������)
						//bit6:0,�޶���;1,������ֹ����(���ǲ�ɾ����Ƶ��������),������ɺ�,���������Զ������λ
 						//bit7:0,��Ƶ����������ɾ��/����ɾ��;1,��Ƶ����������������(�������ִ��)
	
	u8 mode;			//����ģʽ
						//0,ȫ��ѭ��;1,����ѭ��;2,�������;
	
	u8 *path;			//��ǰ�ļ���·��
	u8 *name;			//��ǰ���ŵ�MP3��������
	u16 namelen;		//name��ռ�ĵ���.
	u16 curnamepos;		//��ǰ��ƫ��

    u32 totsec ;		//���׸�ʱ��,��λ:��
    u32 cursec ;		//��ǰ����ʱ�� 
    u32 bitrate;	   	//������(λ��)
	u32 samplerate;		//������ 
	u16 bps;			//λ��,����16bit,24bit,32bit
	
	u16 curindex;		//��ǰ���ŵ���Ƶ�ļ�����
	u16 mfilenum;		//�����ļ���Ŀ	    
	u16 *mfindextbl;	//��Ƶ�ļ�������
	
}__audiodev; 
extern __audiodev audiodev;	//���ֲ��ſ�����

typedef __packed struct
{
	u32 index;
	u8 name[240];
	struct __audiolist *list;
}__audiolist;
extern __audiolist audiolist;

#ifndef AUDIO_MIN			
#define AUDIO_MIN(x,y)	((x)<(y)? (x):(y))
#endif

void wav_i2s_dma_callback(void);

void audio_start(void);
void audio_stop(void);
u16 audio_get_tnum(u8 *path);
void audio_index_show(u16 index,u16 total);
void audio_msg_show(u32 totsec,u32 cursec,u32 bitrate);
void audio_play(void);
u8 audio_play_song(u8* fname);

void audio_list_show(u8 *path, u16 totwavnum);

 
#endif












