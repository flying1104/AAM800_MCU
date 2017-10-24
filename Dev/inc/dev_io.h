#ifndef __DEV_IO
#define __DEV_IO

//#include "dev_conf.h"

#define BITBAND(addr, bitnum) ((addr & 0xF0000000) + 0x2000000 + ((addr & 0xFFFFF) << 5) + (bitnum << 2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 

/**********************************************
						GPIO_SET
***********************************************								

***********************************************/

#define GPIO_ODR_OFFSET	0x14
#define GPIO_IDR_OFFSET	0x10

#define GPIOA_ODR_Addr    (GPIOA_BASE + GPIO_ODR_OFFSET) 
#define GPIOB_ODR_Addr    (GPIOB_BASE + GPIO_ODR_OFFSET) 
#define GPIOC_ODR_Addr    (GPIOC_BASE + GPIO_ODR_OFFSET) 
#define GPIOD_ODR_Addr    (GPIOD_BASE + GPIO_ODR_OFFSET) 
#define GPIOE_ODR_Addr    (GPIOE_BASE + GPIO_ODR_OFFSET) 
#define GPIOF_ODR_Addr    (GPIOF_BASE + GPIO_ODR_OFFSET)     
#define GPIOG_ODR_Addr    (GPIOG_BASE + GPIO_ODR_OFFSET)  

#define GPIOA_IDR_Addr    (GPIOA_BASE + GPIO_IDR_OFFSET) 
#define GPIOB_IDR_Addr    (GPIOB_BASE + GPIO_IDR_OFFSET) 
#define GPIOC_IDR_Addr    (GPIOC_BASE + GPIO_IDR_OFFSET) 
#define GPIOD_IDR_Addr    (GPIOD_BASE + GPIO_IDR_OFFSET) 
#define GPIOE_IDR_Addr    (GPIOE_BASE + GPIO_IDR_OFFSET) 
#define GPIOF_IDR_Addr    (GPIOF_BASE + GPIO_IDR_OFFSET) 
#define GPIOG_IDR_Addr    (GPIOG_BASE + GPIO_IDR_OFFSET) 

#define GPIO_BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 
 
#define PAout(n)   GPIO_BIT_ADDR(GPIOA_ODR_Addr,n)  
#define PAin(n)    GPIO_BIT_ADDR(GPIOA_IDR_Addr,n)  

#define PBout(n)   GPIO_BIT_ADDR(GPIOB_ODR_Addr,n)
#define PBin(n)    GPIO_BIT_ADDR(GPIOB_IDR_Addr,n)  

#define PCout(n)   GPIO_BIT_ADDR(GPIOC_ODR_Addr,n) 
#define PCin(n)    GPIO_BIT_ADDR(GPIOC_IDR_Addr,n)

#define PDout(n)   GPIO_BIT_ADDR(GPIOD_ODR_Addr,n) 
#define PDin(n)    GPIO_BIT_ADDR(GPIOD_IDR_Addr,n)  

#define PEout(n)   GPIO_BIT_ADDR(GPIOE_ODR_Addr,n)
#define PEin(n)    GPIO_BIT_ADDR(GPIOE_IDR_Addr,n) 

#define PFout(n)   GPIO_BIT_ADDR(GPIOF_ODR_Addr,n)
#define PFin(n)    GPIO_BIT_ADDR(GPIOF_IDR_Addr,n) 

#define PGout(n)   GPIO_BIT_ADDR(GPIOG_ODR_Addr,n)
#define PGin(n)    GPIO_BIT_ADDR(GPIOG_IDR_Addr,n) 

#define PHout(n)   GPIO_BIT_ADDR(GPIOH_ODR_Addr,n)  
#define PHin(n)    GPIO_BIT_ADDR(GPIOH_IDR_Addr,n)  

#define PIout(n)   GPIO_BIT_ADDR(GPIOI_ODR_Addr,n) 
#define PIin(n)    GPIO_BIT_ADDR(GPIOI_IDR_Addr,n)  


#endif




