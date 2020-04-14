#include "Configure.h"
#include "ram.h"
#include "systemValues.h"
#include "stepLinkedList.h"
#include "key/bsp_key.h"
//#include "includes.h"

systicStruct tick;
//°´¼ü
struct Button CutBtn;
struct Button PlasticBtn;
struct Button WorkFinishBtn;
struct Button PaperExitBtn;
/******·ÖÒ³******/
PagingMotorns_Struct PagingMachine;

Sign SignPagingExistPaper;
Sign SignPagingFrictionOrigin;
Sign SignPagingFrictionOut;
Sign SignPagingFrictionOutOne;
Sign SignPagingFrictionOutTwo;
Sign SignPagingFrictionOutThree;
Sign SignPagingFrictionOutFour;
Sign SignPagingRollerOut;

/******¸ÖÓ¡******/
StampMotorns_Struct StampMachine;
List StampStepList;

Sign SignStampPostion;
Sign SignStampACMotor;
Sign SignStampWaitUp;
Sign SignStampWaitDown;

/******²ÃÇÐ******/
CutMotorns_Struct CutMachine;
List CutStepList;

Sign SignCutFlotasmPostion;
Sign SignCutLeftFlotasmCut;
Sign SignCutRightFlotasmCut;
Sign SignCutChamferMotor;
Sign SignCutChamferPostion;
Sign SignCutMachineFront;
Sign SignCutMachineBehind;

