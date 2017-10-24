#include "types.h"
#include "adm200_eval_init.h"
#include "lcd_menu.h"
#include "front_panel.h"
#include "dev_time.h"
//#include "adm200_conf.h"
#include "audioplay.h"
#include "sys_property.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



extern TypeSysProStr	*pSysProHead;
extern TypeSysProStr	*pSysProCur;
extern TypeSysProStr  *pSysProNew;
extern TypeBaseRegStr		baseReg;
//extern TypeBaseRegStr		curBaseReg;
extern TypeBaseStatusStr baseStatus;

/*
uchar menuLevel1Info[] = {"In=XLR  Time=180Freq=44.1k"};
uchar menuLevel1Logo[] = {"DreamFly........ADM200_INIT....."};
uchar menuLevel1Init[] = {"Initializating..Please wait....."};
*/

uchar menuString0[] = {"      TIME:           180       "};
uchar menuString1[] = {"In=XLR  Time=180Freq=44.1k      "};
uchar menuString2[] = {"Set Delay Time:    TIME = 180   "};
uchar menuString3[] = {"Set Audio Input:   Input = XLR  "};
uchar menuString4[] = {"Set Audio Freq:    Freq = 44.1k "};
uchar menuString5[] = {"Load Default ?     Mode =  NO   "};
uchar menuString6[] = {" Are you Sure?  OK=Yes  Other=No"};

uchar name0[] = {"delayInit"};	
uchar name1[] = {"instruction"};
uchar name2[] = {"delayTime"};
uchar name3[] = {"audioInput"};
uchar name4[] = {"audioFreq"};
uchar name5[] = {"loadDefault"};	
uchar name6[] = {"Confirm"};





void Sys_Property_Set(void)
{
	uchar keyPass = KEY_NULL;
	uchar dataTmp = 0;
	TypeSysProStr *tmpSysPro;
	
	keyPass = Key_Scan();
	if(pSysProCur->index >= SYS_INSTRUCTION && pSysProCur->index <= SYS_LOADDEFAULT)
	{		
		if(keyPass == RIGHTKEY)
		{
			keyPass = KEY_NULL;
			switch(pSysProCur->index)
			{
				case SYS_LOADDEFAULT:
//					pSysProCur = pSysProCur;
					break;
				
				default:
//					printf("keyPass = %x\n", keyPass);
					pSysProCur = pSysProCur->prev;
//					printf("name = %s\n",pSysProCur->name);
					break;
			}
		}else if(keyPass == LEFTKEY)
		{
			keyPass = KEY_NULL;
			switch(pSysProCur->index)
			{
				case SYS_INSTRUCTION:
					break;
				
				default:
					pSysProCur = pSysProCur->next;
					break;
			}
		}
		
		if(pSysProCur->index >= SYS_DELAYTIME && pSysProCur->index <= SYS_LOADDEFAULT)
		{
			dataTmp = pSysProCur->property;

			if(keyPass == UPKEY)
			{
				keyPass = KEY_NULL;
				switch(pSysProCur->index)
				{
					case SYS_DELAYTIME:
						if(pSysProCur->property >= DELAYTIMEMAX) baseReg.delayTimeNum = pSysProCur->property = DELAYTIMEMAX;
						else {pSysProCur->property = ++dataTmp; 	baseReg.delayTimeNum = dataTmp;}
//						baseStatus.propertyFlag = SYS_DELAYTIME;
						break;
					case SYS_AUDIOIN:
						if(pSysProCur->property >= AUDIOINMAX) baseReg.audioInMode = pSysProCur->property = AUDIOINMIN;
						else {pSysProCur->property = ++dataTmp; 	baseReg.audioInMode = dataTmp;}
						baseStatus.propertyFlag = SYS_AUDIOIN;
						break;
					case SYS_AUDIOFRE:
						if(pSysProCur->property >= AUDIOFREQMAX) baseReg.audioFreqMode = pSysProCur->property = ADUIOFREQMIN;
						else {pSysProCur->property = ++dataTmp;		baseReg.audioFreqMode = dataTmp;}
						baseStatus.propertyFlag = SYS_AUDIOFRE;
						break;
					case SYS_LOADDEFAULT:
						if(pSysProCur->property >= LOADDEFAULTMAX) pSysProCur->property = LOADDEFAULTMIN;
						else pSysProCur->property++;
						break;
					default:
						break;
				}
			}
			else if(keyPass == DOWNKEY)
			{
				keyPass = KEY_NULL;
				switch(pSysProCur->index)
				{
					case SYS_DELAYTIME:
						if(pSysProCur->property == DELAYTIMEMIN) baseReg.delayTimeNum = pSysProCur->property = DELAYTIMEMIN;
						else {pSysProCur->property = --dataTmp; 	baseReg.delayTimeNum = dataTmp;}
						baseStatus.propertyFlag = SYS_DELAYTIME;
						break;
					case SYS_AUDIOIN:
						if(pSysProCur->property <= AUDIOINMIN) baseReg.audioInMode = pSysProCur->property = AUDIOINMAX;
						else {pSysProCur->property = --dataTmp; 	baseReg.audioInMode = dataTmp;}
						baseStatus.propertyFlag = SYS_AUDIOIN;
						break;
					case SYS_AUDIOFRE:
						if(pSysProCur->property <= ADUIOFREQMIN) baseReg.audioFreqMode = pSysProCur->property = AUDIOFREQMAX;
						else {pSysProCur->property = --dataTmp;		baseReg.audioFreqMode = dataTmp;}
						baseStatus.propertyFlag = SYS_AUDIOFRE;
						break;
					case SYS_LOADDEFAULT:
						if(pSysProCur->property <= LOADDEFAULTMIN) pSysProCur->property = LOADDEFAULTMAX;
						else pSysProCur->property--;
						break;
					default:
						break;
				}
			}
		}	
	}
	
	if(pSysProCur->index == SYS_LOADDEFAULT)
	{
		if(keyPass == OKKEY && pSysProCur->property == LOADDEFAULTYES)
		{
			baseStatus.loadDefaultFlag = 1;
			pSysProNew = pSysProCur;
			pSysProCur = Sys_Property_Get(SYS_CONFIRM);
			keyPass = KEY_NULL;
		}
	}
	else if(pSysProCur->index != SYS_LOADDEFAULT && baseStatus.loadDefaultFlag == 0)
	{
		tmpSysPro = Sys_Property_Get(SYS_LOADDEFAULT);
		tmpSysPro->property = LOADDEFAULTNO;
	}

	if(pSysProCur->index == SYS_INSTRUCTION)
	{
		if(keyPass == OKKEY)
		{
			baseStatus.delayFlag = 1;
			pSysProNew = pSysProCur;
			pSysProCur = Sys_Property_Get(SYS_CONFIRM);
			keyPass = KEY_NULL;
		}
	}

	if(pSysProCur->index == SYS_CONFIRM)
	{
//		printf("baseStatus.loadDefaultFlag = %x\n",baseStatus.loadDefaultFlag);
//		printf("keyPass = %x\n", keyPass);
		if(baseStatus.loadDefaultFlag == 1 && keyPass == OKKEY)
		{
			baseStatus.loadDefaultFlag = 0;
			Sys_BaseStatus_Loaddefault();
			pSysProCur = Sys_Property_Get(SYS_INSTRUCTION);				
		}
		else if(baseStatus.delayFlag == 1 && keyPass == OKKEY)
		{
			baseStatus.delayFlag = 0;
			pSysProCur = Sys_Property_Get(SYS_DELAYINIT);	
			Fpga_Cmd_Set(AUDIOCMDSTARTREG, 0);
//			Fpga_Cmd_Set(AUDIOCMDBYPASSREG, 1);
			Fpga_Cmd_Set(AUDIOCMDMUTEREG, 0);
			Delay_Ms(20);
//			Fpga_Cmd_Set(AUDIOCMDBYPASSREG, 1);
			Fpga_Cmd_Set(AUDIOOUTREG, AUDIOOUTWAV);
			Fpga_Cmd_Set(AUDIOCMDMUTEREG, 0);
			Fpga_Cmd_Set(AUDIOCMDSTARTREG, 1);
			baseReg.curDelayTimeNum = 0;
			Delay_Time_Enable();
			audio_play();
		}
		else if(keyPass != KEY_NULL)
		{
			baseStatus.loadDefaultFlag = 0;
			pSysProCur = Sys_Property_Get(SYS_INSTRUCTION);
		}
	}	
	
	if(pSysProCur->index == SYS_INSTRUCTION)
	{
		if(keyPass == BYPASSKEY)
		{
			baseStatus.propertyFlag = SYS_BYPASS;
		}
	}
	
	if(pSysProCur->index == SYS_INSTRUCTION || pSysProCur->index == SYS_DELAYINIT)
	{
		if(keyPass == DUMPKEY)
		{
			baseStatus.propertyFlag = SYS_DUMP;
			
			Delay_Ms(10);
			Key_Led_Set(SYS_DUMP, LEDOFF);
			
			pSysProCur = Sys_Property_Get(SYS_DELAYINIT);	
			Fpga_Cmd_Set(AUDIOCMDSTARTREG, 0);
//			Fpga_Cmd_Set(AUDIOCMDBYPASSREG, 1);
			Fpga_Cmd_Set(AUDIOCMDMUTEREG, 0);
			Delay_Ms(20);
			Fpga_Cmd_Set(AUDIOOUTREG, AUDIOOUTWAV);
			Fpga_Cmd_Set(AUDIOCMDMUTEREG, 0);
			Fpga_Cmd_Set(AUDIOCMDSTARTREG, 1);
			baseReg.curDelayTimeNum = 0;
			Delay_Time_Enable();
			audio_play();
		}
	}
}

void Sys_Property_Tx(void)
{
	TypeSysProStr *tmpSys;
	uchar property;
	
	if(baseStatus.propertyFlag != 0)
	{
		printf("propertyFlag = %d\n", 	baseStatus.propertyFlag);		
		switch(baseStatus.propertyFlag)
		{
			case SYS_DELAYTIME:
				tmpSys = Sys_Property_Get(SYS_DELAYTIME);
				property = tmpSys->property;
				printf("property = %d\n", property);
				Fpga_Cmd_Set(AUDIODELAYTIMEREG, property);
				break;
			case SYS_AUDIOIN:
				tmpSys = Sys_Property_Get(SYS_AUDIOIN);
				property = tmpSys->property;
				printf("property = %d\n", property);
				Fpga_Cmd_Set(AUDIOINREG, property);
				break;
			case SYS_AUDIOFRE:
				tmpSys = Sys_Property_Get(SYS_AUDIOFRE);
				property = tmpSys->property;
				printf("property = %d\n", property);
				Fpga_Cmd_Set(AUDIOFREQREG, property);
				break;
			case SYS_LOADDEFAULT:
				tmpSys = Sys_Property_Get(SYS_DELAYTIME);
				property = tmpSys->property;
				printf("property = %d\n", property);
				Fpga_Cmd_Set(AUDIODELAYTIMEREG, property);
				tmpSys = Sys_Property_Get(SYS_AUDIOIN);
				property = tmpSys->property;
				printf("property = %d\n", property);
				Fpga_Cmd_Set(AUDIOINREG, property);
				tmpSys = Sys_Property_Get(SYS_AUDIOFRE);
				property = tmpSys->property;
				printf("property = %d\n", property);
				Fpga_Cmd_Set(AUDIOFREQREG, property);
				break;
			case SYS_BYPASS:
				baseStatus.bypassFlag = (baseStatus.bypassFlag & 0xFE) | (~baseStatus.bypassFlag & 0x01);
				printf("bypassFlag = %d\n",baseStatus.bypassFlag);
				switch(!(baseStatus.bypassFlag & 0x01))
				{
					case LEDON:
							Key_Led_Set(SYS_BYPASS, LEDON);
							Fpga_Cmd_Set(AUDIOOUTREG, AUDIOOUTBYPASS);
						break;
					case LEDOFF:
							Key_Led_Set(SYS_BYPASS, LEDOFF);
							Fpga_Cmd_Set(AUDIOOUTREG, AUDIOOUTDELAY);
						break;				
				}
				break;
			case SYS_DUMP:
				
				break;
			
			default:
				
				break;
		}
		Fpga_Cmd_Set(AUDIOSETCOMPLETEREG, AUDIOSETENABLE);
		baseStatus.propertyFlag = 0;
	}
}

void Sys_BaseStatus_Init(void)
{	
	uchar i;
	uchar tmp;
	
	baseReg.delayTimeNum				= 5;
	baseReg.curDelayTimeNum			= 0;
	baseReg.audioInMode					= AUDIOINXLR;
	baseReg.audioFreqMode 			= AUDIOFREQ48K;
	baseReg.loadDefaultMode			= LOADDEFAULTNO;
	
	baseStatus.bypassFlag				= 0;
	baseStatus.dumpFlag					= 0;
	baseStatus.bypassLedFlag		= LEDOFF;
	baseStatus.dumpLedFlag			= LEDOFF;
}

void Sys_BaseStatus_Loaddefault(void)
{
	TypeSysProStr *tmpSysPro;
	
	AT24CXX_WriteOneByte(SYS_DELAYTIME_REG, SYS_DELAYTIME_DEFAULT_VAL);
	AT24CXX_WriteOneByte(SYS_AUDIOIN_REG, SYS_AUDIOIN_DEFAULT_VAL);
	AT24CXX_WriteOneByte(SYS_AUDIOFRE_REG, SYS_AUDIOFRE_DEFAULT_VAL);
	
	baseReg.delayTimeNum			= SYS_DELAYTIME_DEFAULT_VAL;
	baseReg.curDelayTimeNum		= 0;
	baseReg.audioInMode				= SYS_AUDIOIN_DEFAULT_VAL;
	baseReg.audioFreqMode 		= SYS_AUDIOFRE_DEFAULT_VAL;
	baseReg.loadDefaultMode		= LOADDEFAULTNO;
	
	tmpSysPro = Sys_Property_Get(SYS_DELAYTIME);
	tmpSysPro->property = SYS_DELAYTIME_DEFAULT_VAL;
	tmpSysPro = Sys_Property_Get(SYS_AUDIOIN);
	tmpSysPro->property = SYS_AUDIOIN_DEFAULT_VAL;
	tmpSysPro = Sys_Property_Get(SYS_AUDIOFRE);
	tmpSysPro->property = SYS_AUDIOFRE_DEFAULT_VAL;
	tmpSysPro = Sys_Property_Get(SYS_LOADDEFAULT);
	tmpSysPro->property = LOADDEFAULTNO;
}


void Sys_Property_Init(void) 
{
	uchar property1 = baseReg.delayTimeNum;
	uchar property2 = baseReg.audioInMode;
	uchar property3 = baseReg.audioFreqMode;
	
//	TypeSysProStr *pCur = pSysProHead;
//	TypeSysProStr *pPrev = NULL;	
	
	pSysProCur 	= Sys_Property_Add_To_List(pSysProHead,SYSINDEX0, name0, SYSINDEX0 , menuString0);
	pSysProCur 	= Sys_Property_Add_To_List(pSysProCur, SYSINDEX1, name1, SYSINDEX1 , menuString1);	
	pSysProCur 	= Sys_Property_Add_To_List(pSysProCur, SYSINDEX2, name2, property1 , menuString2);	
	pSysProCur 	= Sys_Property_Add_To_List(pSysProCur, SYSINDEX3, name3, property2 , menuString3);
	pSysProCur 	= Sys_Property_Add_To_List(pSysProCur, SYSINDEX4, name4, property3 , menuString4);
	pSysProCur 	= Sys_Property_Add_To_List(pSysProCur, SYSINDEX5, name5, SYSINDEX5 , menuString5);
	pSysProCur 	= Sys_Property_Add_To_List(pSysProCur, SYSINDEX6, name6, SYSINDEX6 , menuString6);
	pSysProHead 	= pSysProCur;
}

TypeSysProStr *Sys_Property_Get(uchar index)
{
	TypeSysProStr *getPro = pSysProHead;
		
	while(getPro->next != NULL)
	{
		if(getPro->index == index) break;
		else getPro = getPro->next;
	}
	
	return getPro;
}


TypeSysProStr *Sys_Property_Add_To_List(TypeSysProStr *pCur, \
																			uchar index, \
																			uchar *name, \
																			uchar property, \
																			uchar *lcdMenu 
																			)
{
	TypeSysProStr *newPro = NULL;	
	
	newPro = (TypeSysProStr*)malloc(sizeof(TypeSysProStr));
	
	newPro->index = index;
	newPro->name = name;
	newPro->property = property;
	newPro->pMenuString = lcdMenu;
	newPro->next = pCur;
	
	if(pCur == NULL) pCur->prev = NULL;
	else pCur->prev = newPro;
	
	return newPro;
}

void Fpga_Cmd_Set(ushort addr, ushort data) 
{
	Spi1_SW_Wr(addr, data);
	Delay_Us(10);
}




