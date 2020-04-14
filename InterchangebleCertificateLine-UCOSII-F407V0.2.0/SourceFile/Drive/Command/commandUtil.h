#ifndef __COMMANDUTIL_H_
#define __COMMANDUTIL_H_
#include "commandConfig.h"
extern char buffer[500];
void Command_Copy(u8 *USART_RX_BUF,int len);
Job* create_job(char *cmd);
void parse_cmd(Job *job, char *line);
void destroy_job(Job *job);
void arg_cmd(Job *job, char *line);
int arg_num(char **arg);
#endif
