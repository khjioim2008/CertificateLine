#ifndef __COMMANDCORE_H_
#define __COMMANDCORE_H_
#include "commandConfig.h"
#include "string.h"
#include "commandUtil.h"
#include "commandList.h"
#include "usecom.h"
#include "runPagingMachine.h"

//#include "uart.h"
void CommandScan(uint8_t *j,uint32_t len);
void CommandCallBackInit(void);

#endif


