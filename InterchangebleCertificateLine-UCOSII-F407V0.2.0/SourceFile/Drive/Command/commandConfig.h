#ifndef __COMMANDCONFIG_H_
#define __COMMANDCONFIG_H_
#define COMMAND_NUM					15	//��ǰshell��������������.��Ҫ�������������
typedef struct 
{
	char   **args;//����
}Program;
typedef struct
{
	char     *cmd;
	int      progs_num;//������
	Program  *progs;//����
}Job;//ָ��

typedef unsigned char u8;
typedef unsigned int u32;
#endif
