/*=================================================================*\
Filename				:	main.c
Author				:	Wisely
Description			:	
Revision	History	:	2017-08-16
							V1.0
Company				:
Email					:
Copyright(c) ,DreamFly Technology Inc,All right reserved
\*=================================================================*/
#include "types.h"
#include "dev_cs8416.h"
#include "aam800_mcu_eval_init.h"
#include "main.h"
#include <stdio.h>

int main(void)
{
	ushort tmp;
	
	Eval_Base_Init();
	
	Eval_Dev_Init();
	
	Board_Version();
	
	tmp = Cs8416_Rd(0x05);
	
	printf("cs8416_05 = %x\n", tmp);
	
	while(1)
	{
		
	}
}




