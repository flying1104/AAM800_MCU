#ifndef __DEV_CS8406
#define __DEV_CS8406

#define CS8406_ADDR 0x26

void Cs8406_Init(void);
uchar Cs8406_Rd(uchar addr);
void Cs8406_Wr(uchar addr,uchar data);


#endif




