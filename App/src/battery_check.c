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
#include "dev_adc.h"
#include "stm32f4xx.h"
#include "battery_check.h"
#include <stdio.h>

#define BATTERY_CHECK_COUNT 20

ushort Battery_Check_Read(void)
{	
	ushort Battery_Check_Value = 0;

	Battery_Check_Value = Get_Adc_Average(ADC_Channel_9, BATTERY_CHECK_COUNT);
		
	return Battery_Check_Value;	
}

uchar Battery_Vol(void)
{
	uchar per;
	ushort adcVol;
	short fPer;
	float batterVol;
	
	adcVol = Battery_Check_Read();
	
	batterVol = ((adcVol * 3.3) /4096) * (51 + 75) / 75;
	fPer = (short)((batterVol * 100) / 4.2);
	printf("battery = %f\n", batterVol);	
	printf("fper = %d\n", fPer);
	
	return per;
}








