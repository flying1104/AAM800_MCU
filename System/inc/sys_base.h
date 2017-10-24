#ifndef __DEV_BASE
#define __DEV_BASE

#include "dev_io.h"


										
/***********************************************************

1.LED	BASE		--	PB12
2.SYSTEM RST 	-- 	PA6

***********************************************************/

#define LED_BASE_POUT			PBout(12)

#define SYSTEM_RST_POUT			PAout(6)
#define SYSTEM_RST_ON		0		//low rst					
#define SYSTEM_RST_OFF	1

void Led_Base_Init(void);
void Led_Base_Config(uchar status);

void System_Rst_Init(void);
void System_Rst_Config(uchar config);



#endif

