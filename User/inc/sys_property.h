#ifndef __SYS_PROPERTY
#define __SYS_PROPERTY

//sys index
#define SYS_DELAYINIT		1
#define SYS_CONFIRM			2

#define SYS_INSTRUCTION	10
#define SYS_DELAYTIME		11
#define SYS_AUDIOIN			12
#define SYS_AUDIOFRE		13
#define SYS_LOADDEFAULT	14

#define SYS_BYPASS			31
#define SYS_DUMP				32

#define SYSINDEX0		SYS_DELAYINIT
#define SYSINDEX6		SYS_CONFIRM

#define SYSINDEX1		SYS_INSTRUCTION 
#define SYSINDEX2		SYS_DELAYTIME
#define SYSINDEX3		SYS_AUDIOIN
#define SYSINDEX4		SYS_AUDIOFRE
#define SYSINDEX5		SYS_LOADDEFAULT

//at24cxx reg
#define SYS_DELAYTIME_REG 0
#define SYS_AUDIOIN_REG   1
#define SYS_AUDIOFRE_REG	2

//default val
#define SYS_DELAYTIME_DEFAULT_VAL 0
#define SYS_AUDIOIN_DEFAULT_VAL		AUDIOINXLR
#define SYS_AUDIOFRE_DEFAULT_VAL	AUDIOFREQ48K

///////////////////////////////////////////////////////
// 功能按键
///////////////////////////////////////////////////////
#define LEDON				0
#define LEDOFF			1

///////////////////////////////////////////////////////
// 参数设置
///////////////////////////////////////////////////////

#define DELAYTIMEMIN		0
#define DELAYTIMEMAX		180

#define AUDIOINXLR			1
#define AUDIOINAES			2
#define AUDIOINMIN			AUDIOINXLR
#define AUDIOINMAX			AUDIOINAES

#define AUDIOFREQ32K		1
#define AUDIOFREQ44_1K	2
#define AUDIOFREQ48K		3
#define AUDIOFREQ96K		4
#define ADUIOFREQMIN		AUDIOFREQ32K
#define AUDIOFREQMAX		AUDIOFREQ96K

#define AUDIOOUTDELAY		1
#define AUDIOOUTBYPASS	2
#define AUDIOOUTMUTE		3
#define AUDIOOUTWAV			4

#define LOADDEFAULTYES	1
#define LOADDEFAULTNO		2
#define LOADDEFAULTMIN		LOADDEFAULTYES
#define LOADDEFAULTMAX		LOADDEFAULTNO

///////////////////////////////////////////////////////
// FPGA寄存器
///////////////////////////////////////////////////////

#define AUDIOINREG					0
#define AUDIOFREQREG				1
#define AUDIODELAYTIMEREG		2
#define AUDIODELAYMODEREG		3
#define AUDIOSYNCCLKINREG		4
#define AUDIODUMPSCALEREG		5
#define AUDIOSECONDSCALEREG	6
#define AUDIOINITMODEREG		7
#define AUDIOOUTREG					8
#define AUDIOSETCOMPLETEREG	9

#define AUDIOCMDSTARTREG		10
#define AUDIOCMDEXITREG			11
#define AUDIOCMDDUMPREG			12
#define AUDIOCMDSECONDREG		13
#define AUDIOCMDCOUGHREG		14
#define AUDIOCMDMUTEREG			15
#define AUDIOCMDBYPASSREG		16

#define AUDIOCMDREGMIN			AUDIOCMDSTARTREG
#define AUDIOCMDREGMAX			AUDIOCMDBYPASSREG

#define AUDIOSETENABLE			1
#define AUDIOSETDISABLE			0

void Sys_Property_Set(void);
void Sys_BaseStatus_Init(void);
void Sys_BaseStatus_Loaddefault(void); 
void Sys_Property_Init(void);
void Sys_Property_Tx(void);
TypeSysProStr *Sys_Property_Get(uchar index);
TypeSysProStr *Sys_Property_Add_To_List(TypeSysProStr *pCur, \
																			uchar index, \
																			uchar *name, \
																			uchar property, \
																			uchar *lcdMenu  
																			);
void Fpga_Cmd_Set(ushort addr, ushort data);



#endif


