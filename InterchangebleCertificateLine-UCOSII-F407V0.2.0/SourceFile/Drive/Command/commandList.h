#ifndef __COMMANDLIST_H_
#define __COMMANDLIST_H_
#include "commandConfig.h"

//command里面的参数和函数列表放在该文件中

typedef void (*Command_Proc_Func)(Program);

extern u8 commandStringList[][50] ;

extern Command_Proc_Func Command_Proc_Func_Table[];

extern void text(Program data);

extern void commandLeftHobMotor(Program data);

extern void commandRightHobMotor(Program data);

extern void commandLeftChamferMotor(Program data);

extern void commandRightChamferMotor(Program data);

extern void commandFlotasmCutMotor(Program data);

extern void commandCutMachineSeq(Program data);

extern void commandMachineStep(Program data);

extern void commandStampACMotor(Program data);

extern void commandStampSeq(Program data);

extern void commandPagingSeq(Program data);

extern void commandPagingConveyerMotor(Program data);

extern void commandHobMotor(Program data);

extern void commandPIDMotor(Program data);

extern void commandCutStep(Program data);
#endif


