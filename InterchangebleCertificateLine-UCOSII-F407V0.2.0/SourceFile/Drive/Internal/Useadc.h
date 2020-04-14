#ifndef __Useadc_H
#define __Useadc_H
#include "stm32f10x.h"

u16 Get_Adc(u8 ch);   
u16 Get_Adc_Average(u8 ch,u8 times);

u16 Get_Adc2(u8 ch);   
u16 Get_Adc_Average2(u8 ch,u8 times);
u16 Get_Adc3(void);
#endif
