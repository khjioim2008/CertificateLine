#ifndef __DataDeal_H
#define __DataDeal_H
//#include "Type.h"

extern u16 getMoreTimesNum(u16*dataBuf,u16 len,u16 num);
extern u16 analogAdapt(void);
extern void writeAnalogDataInflash(u16 AnalogBufper);
extern void writeAnalogDataClear(void);
extern u16 analogAdaptAverage(u16 AnalogBuf[AnalogAverageLen]);
extern u8 writeAnalogAverageBufInflash(u16 AnalogBuf[AnalogAverageLen]);
extern void clearAnalogAverageBufInflash(void);
extern void flashDefaultSet(Flash_Save_Struct * flashDefault);

#endif
