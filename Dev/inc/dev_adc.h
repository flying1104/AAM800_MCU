#ifndef __DEV_ADC
#define __DEV_ADC




void Adc_Init(void); 				//ADCͨ����ʼ��
ushort  Get_Adc(uchar ch); 				//���ĳ��ͨ��ֵ 
ushort Get_Adc_Average(uchar ch,uchar times);//�õ�ĳ��ͨ����������������ƽ��ֵ  


#endif



