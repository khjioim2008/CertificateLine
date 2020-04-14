#ifndef __runPagingMachine_H
#define __runPagingMachine_H
#include "ram.h"
void runPagingMachine(PagingMotorns_Struct * Name);
void controlPagingMachine(PagingMotorns_Struct * Name);
extern void manualPlasticMem(PagingMotorns_Struct *Name);
extern void continuousPlasticMem(PagingMotorns_Struct *Name);
extern void stopPlasticMem(PagingMotorns_Struct *Name);

#endif
