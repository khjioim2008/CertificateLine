#include "commandList.h"
#include "commandCore.h"

u8 commandStringList[][50] = 
{
	"text",
	"commandLeftHobMotor",
	"commandRightHobMotor",
	"commandLeftChamferMotor",
	"commandRightChamferMotor",
	"commandFlotasmCutMotor",
	"commandCutMachineSeq",
	"commandMachineStep",
	"commandStampACMotor",
	"commandStampSeq",
	"commandPagingSeq",
	"commandPagingConveyerMotor",
	"commandHobMotor",
	"commandPIDMotor",
	"commandCutStep"
};

/*
A5 DB 14 00 01 05 06 01 50 64 64 64 64 01 03 00 01 00 3E 7D A3 BF
*/

//回调函数数组定义
Command_Proc_Func Command_Proc_Func_Table[] = 
{
	text,
	commandLeftHobMotor,
	commandRightHobMotor,
	commandLeftChamferMotor,
	commandRightChamferMotor,
	commandFlotasmCutMotor,
	commandCutMachineSeq,
	commandMachineStep,
	commandStampACMotor,
	commandStampSeq,
	commandPagingSeq,
	commandPagingConveyerMotor,
	commandHobMotor,
	commandPIDMotor,
	commandCutStep
};
