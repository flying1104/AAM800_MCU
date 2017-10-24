#ifndef __DEV_HMILCD
#define __DEV_HMILCD



#define USART2_REC_LEN 200
#define HMID_ID_LEN  12

typedef __packed struct
{
	uchar page;
	uchar id;
	uchar active;
}__hmi_id;

extern __hmi_id hmd_id;

void Uart2_Init(uint baud);
void HMISends(char *buf1);
void HMISendb(uchar k);
uchar HMIGetId(void);


#endif

