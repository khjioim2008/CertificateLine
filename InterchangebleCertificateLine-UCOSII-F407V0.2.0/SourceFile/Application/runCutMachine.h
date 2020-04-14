#ifndef __runCutMachine_H
#define __runCutMachine_H
//extern void runApplication(void);
#include "ram.h"
extern void controlCutMachine(CutMotorns_Struct *Name);
extern void runChamferPostionSensor(void);
extern void runCutMachine(CutMotorns_Struct *Name);
extern void manualCutMem(CutMotorns_Struct *Name);
extern void continuousCutMem(CutMotorns_Struct *Name);
extern void StopCutMem(CutMotorns_Struct *Name);
#endif
