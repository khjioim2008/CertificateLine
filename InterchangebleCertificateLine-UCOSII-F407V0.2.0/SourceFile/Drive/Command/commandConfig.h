#ifndef __COMMANDCONFIG_H_
#define __COMMANDCONFIG_H_
#define COMMAND_NUM					15	//当前shell包含的命令总数.需要根据命令的数量
typedef struct 
{
	char   **args;//参数
}Program;
typedef struct
{
	char     *cmd;
	int      progs_num;//命令数
	Program  *progs;//命令
}Job;//指令

typedef unsigned char u8;
typedef unsigned int u32;
#endif
