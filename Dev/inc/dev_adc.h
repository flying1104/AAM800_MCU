#ifndef __DEV_ADC
#define __DEV_ADC




void Adc_Init(void); 				//ADC通道初始化
ushort  Get_Adc(uchar ch); 				//获得某个通道值 
ushort Get_Adc_Average(uchar ch,uchar times);//得到某个通道给定次数采样的平均值  


#endif



