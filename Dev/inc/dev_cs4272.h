#ifndef _DEV_CS4272_
#define _DEV_CS4272_

#define CS4272_ADDR 0x22

void Cs4272_Init(void);
uchar Cs4272_Rd(uchar addr);
void Cs4272_Wr(uchar addr,uchar data);


#endif
