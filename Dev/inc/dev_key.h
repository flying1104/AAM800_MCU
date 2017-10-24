#ifndef __DEV_KEY
#define __DEV_KEY


#include "dev_io.h"

/**********************************************************
										KEY PIN SET
***********************************************************						
1.KEY0	--	PE15			 	BYPASS		
2.KEY1	--	PE14			 	MUTE
3.KEY2	-- 	PE13			 	COUGH
4.KEY3	--	PE12				2~8S
5.KEY4	--	PE11				DUMP
6.KEY5	--	PE10				EXIT
7.KEY6	--	PE9					START
8.KEY7	--	PE8					DOWN
9.KEY8	--	PE7					ENTER	
10.KEY9	--	PG1					UP
***********************************************************/

#define KEY_UP			KEY5_RD
#define KEY_DOWN		KEY7_RD
#define KEY_LEFT		KEY4_RD
#define KEY_RIGHT		KEY6_RD
#define KEY_OK			KEY8_RD

#define KEY_DUMP				KEY0_RD
#define KEY_BYPASS			KEY1_RD
#define KEY_DUMP_LED		KEY2_LED
#define KEY_BYPASS_LED	KEY3_LED


#define KEY0_RD	PEin(15)
#define KEY1_RD	PEin(14)
//#define KEY2_RD	PEin(13)
//#define KEY3_RD	PEin(12)
#define KEY4_RD	PEin(11)
#define KEY5_RD	PEin(10)
#define KEY6_RD	PEin(9)
#define KEY7_RD	PEin(8)
#define KEY8_RD	PEin(7)
#define KEY9_RD	PGin(1)

#define KEY2_LED	PEout(13)
#define KEY3_LED	PEout(12)

#define KEY_PASS 		0
#define KEY_UNASS		1

#define KEY_NULL		0
#define KEY0_PASS		1
#define KEY1_PASS		2
#define KEY4_PASS		3
#define KEY5_PASS		4
#define KEY6_PASS		5
#define KEY7_PASS		6
#define KEY8_PASS		7

#define UPKEY				KEY5_PASS
#define DOWNKEY			KEY7_PASS
#define OKKEY				KEY8_PASS
#define LEFTKEY			KEY4_PASS
#define RIGHTKEY		KEY6_PASS
#define DUMPKEY			KEY0_PASS
#define BYPASSKEY		KEY1_PASS

void Key_Gpio_Init(void);
void Key_Dump_Config(uchar config) ;
void Key_Bypass_Config(uchar config);




#endif


