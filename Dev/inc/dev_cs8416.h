#ifndef __DEV_CS8416
#define __DEV_CS8416


#define CS8416_ADDR	0x20

void Cs8416_Init(void);
uchar Cs8416_Rd(uchar addr);
void Cs8416_Wr(uchar addr, uchar data);


#endif


