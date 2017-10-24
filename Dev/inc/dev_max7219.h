#ifndef __DEV_MAX7219
#define __DEV_MAX7219

#include "dev_io.h"

/**********************************************************
										MAX7219 PIN SET
***********************************************************						
1.MAX7219_MOSI	--	PC10
2.MAX7219_SCK		-- 	PA11
3.MAX7219_CS		--	PA12
***********************************************************/



#define MAX7219_CS		PAout(12)
#define MAX7219_SCK		PAout(11)
#define MAX7219_MOSI	PCout(10)


#define MAX_DIG0      0x01
#define MAX_DIG1      0x02
#define MAX_DIG2      0x03
#define MAX_DIG3      0x04
#define MAX_DIG4      0x05

#define DECODE_MODE   0x09 
#define INTENSITY     0x0A 
#define SCAN_LIMIT    0x0B 
#define SHUT_DOWN     0x0C 
#define DISPLAY_TEST  0x0F 


void Max7219_Init(void);
void Max7219_Gpio_Init(void);
void Max7219_Wdata(uchar addr, uchar data);
void Max7219_Wcmd(uchar addr, uchar data);





#endif

