#ifndef __Resources_H
#define __Resources_H
#include "ram.h"
#include "systemValues.h"
#include "stepLinkedList.h"

extern systicStruct tick;
extern struct Button CutBtn;
extern struct Button PlasticBtn;
extern struct Button WorkFinishBtn;
extern struct Button PaperExitBtn;

/******∑÷“≥******/
extern PagingMotorns_Struct PagingMachine;
extern Sign SignPagingExistPaper;
extern Sign SignPagingFrictionOrigin;
extern Sign SignPagingFrictionOut;
extern Sign SignPagingFrictionOutOne;
extern Sign SignPagingFrictionOutTwo;
extern Sign SignPagingFrictionOutThree;
extern Sign SignPagingFrictionOutFour;
extern Sign SignPagingRollerOut;

/******∏÷”°******/
extern StampMotorns_Struct StampMachine;
extern List StampStepList;
extern Sign SignStampPostion;
extern Sign SignStampACMotor;
extern Sign SignStampWaitUp;
extern Sign SignStampWaitDown;


/******≤√«–******/
extern CutMotorns_Struct CutMachine;
extern List CutStepList;

extern Sign SignCutFlotasmPostion;
extern Sign SignCutLeftFlotasmCut;
extern Sign SignCutRightFlotasmCut;
extern Sign SignCutChamferMotor;
extern Sign SignCutChamferPostion;
extern Sign SignCutMachineFront;
extern Sign SignCutMachineBehind;

#endif
