#ifndef __CERTIFICATECONTROL_H
#define __CERTIFICATECONTROL_H


#include "stm32f10x.h"
void certificateLineControl_Init(void);
void certificateLine_ChangeScan(void);
void manualControl_Send(u8* manualData);
void heartBeatSend(u8 *data);
void warningSend(u8 number,u8 status);
void faultResetSend(u8 number);
extern void needPaperSend(void);
void analogSend(u16 ADCValue,u8 percent);

#endif
