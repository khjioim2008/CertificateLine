#include "Configure.h"
#include "ram.h"
#include "systemValues.h"
#include "stepLinkedList.h"
#include "key/bsp_key.h"
//#include "includes.h"

systicStruct tick;
//����
struct Button CutBtn;
struct Button PlasticBtn;
struct Button WorkFinishBtn;
struct Button PaperExitBtn;
/******��ҳ******/
PagingMotorns_Struct PagingMachine;

Sign SignPagingExistPaper;
Sign SignPagingFrictionOrigin;
Sign SignPagingFrictionOut;
Sign SignPagingFrictionOutOne;
Sign SignPagingFrictionOutTwo;
Sign SignPagingFrictionOutThree;
Sign SignPagingFrictionOutFour;
Sign SignPagingRollerOut;

/******��ӡ******/
StampMotorns_Struct StampMachine;
List StampStepList;

Sign SignStampPostion;
Sign SignStampACMotor;
Sign SignStampWaitUp;
Sign SignStampWaitDown;

/******����******/
CutMotorns_Struct CutMachine;
List CutStepList;

Sign SignCutFlotasmPostion;
Sign SignCutLeftFlotasmCut;
Sign SignCutRightFlotasmCut;
Sign SignCutChamferMotor;
Sign SignCutChamferPostion;
Sign SignCutMachineFront;
Sign SignCutMachineBehind;

