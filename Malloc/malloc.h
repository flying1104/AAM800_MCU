#ifndef __MALLOC_H
#define __MALLOC_H
#include "types.h"
#include "stm32f4xx.h"
 
#ifndef NULL
#define NULL 0
#endif

//定义三个内存池
#define SRAMIN	 0		//内部内存池
#define SRAMEX   1		//外部内存池
#define SRAMCCM  2		//CCM内存池(此部分SRAM仅仅CPU可以访问!!!)


#define SRAMBANK 	3	//定义支持的SRAM块数.	


//mem1内存参数设定.mem1完全处于内部SRAM里面.
#define MEM1_BLOCK_SIZE			32  	  						//内存块大小为32字节
#define MEM1_MAX_SIZE			100*1024  						//最大管理内存 100K
#define MEM1_ALLOC_TABLE_SIZE	MEM1_MAX_SIZE/MEM1_BLOCK_SIZE 	//内存表大小

//mem2内存参数设定.mem2的内存池处于外部SRAM里面
#define MEM2_BLOCK_SIZE			32  	  						//内存块大小为32字节
#define MEM2_MAX_SIZE			960 *1024  						//最大管理内存960K
#define MEM2_ALLOC_TABLE_SIZE	MEM2_MAX_SIZE/MEM2_BLOCK_SIZE 	//内存表大小
		 
//mem3内存参数设定.mem3处于CCM,用于管理CCM(特别注意,这部分SRAM,仅CPU可以访问!!)
#define MEM3_BLOCK_SIZE			32  	  						//内存块大小为32字节
#define MEM3_MAX_SIZE			60 *1024  						//最大管理内存60K
#define MEM3_ALLOC_TABLE_SIZE	MEM3_MAX_SIZE/MEM3_BLOCK_SIZE 	//内存表大小
		 


//内存管理控制器
struct _m_mallco_dev
{
	void (*init)(uchar);					//初始化
	uchar (*perused)(uchar);		  	    	//内存使用率
	uchar 	*membase[SRAMBANK];				//内存池 管理SRAMBANK个区域的内存
	ushort *memmap[SRAMBANK]; 				//内存管理状态表
	uchar  memrdy[SRAMBANK]; 				//内存管理是否就绪
};
extern struct _m_mallco_dev mallco_dev;	 //在mallco.c里面定义

void mymemset(void *s,uchar c,ulong count);	//设置内存
void mymemcpy(void *des,void *src,ulong n);//复制内存     
void my_mem_init(uchar memx);				//内存管理初始化函数(外/内部调用)
ulong my_mem_malloc(uchar memx,ulong size);	//内存分配(内部调用)
uchar my_mem_free(uchar memx,ulong offset);		//内存释放(内部调用)
uchar my_mem_perused(uchar memx);				//获得内存使用率(外/内部调用) 
////////////////////////////////////////////////////////////////////////////////
//用户调用函数
void myfree(uchar memx,void *ptr);  			//内存释放(外部调用)
void *mymalloc(uchar memx,ulong size);			//内存分配(外部调用)
void *myrealloc(uchar memx,void *ptr,ulong size);//重新分配内存(外部调用)
#endif













