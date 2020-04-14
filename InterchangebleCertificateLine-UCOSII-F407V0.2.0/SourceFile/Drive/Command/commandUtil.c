#include "commandUtil.h"
#include "string.h"
#include "stdlib.h"
//#include "uart.h"
char buffer[500];

/*复制命令到buffer*/

void Command_Copy(u8 *data,int len)
{
	int i;
	memset(buffer, 0, len);
	for(i=0;i<len;i++)
	{
		buffer[i]=data[i];	  
	}
		buffer[i]='\0';
}

/*初始化*/

Job* create_job(char *cmd)
{
	Job *job = (Job*)malloc(sizeof(Job));
	job->cmd = (char*)malloc(sizeof(char) * strlen(cmd));
	strcpy(job->cmd, cmd); 
	job->progs_num = 0;
	job->progs = NULL;
	return job;
}

/*通过二次指针查询参数数量*/

int arg_num(char **arg)
{
	int len = 0;
	char *start = arg[0];
	while(start != NULL){
		len++;
		start = arg[len];
	}
	return len;
}

/*将字符串数组组成Program结构体变量并*/

Program* create_program(char **arg)
{
	Program *prog = (Program*)malloc(sizeof(Program));
	int counter,i ;
	counter = arg_num(arg);
	prog->args = (char**)calloc(counter, sizeof(char*));
	for(i = 0; i < counter; i++)
	{
		int len = strlen(arg[i]);
		prog->args[i] = (char*)malloc(len+1);
		strcpy(prog->args[i], arg[i]);	
	}
	prog->args[i] = NULL;
	return prog;
}

/*添加到job变量中*/

void add_program(Job *job, Program *prog)
{
	Program *ps =(Program*)malloc(sizeof(Program) * (job->progs_num+1));
	memcpy(ps, job->progs,job->progs_num * sizeof(Program));
	ps[job->progs_num++] = *prog;
	free(prog);
	free(job->progs);
	job->progs = ps;
}

/*按空格分割指令，保存参数*/

void parse_cmd(Job *job, char *line)
{
	char **args =(char**)malloc(30 * sizeof(char*));//分配空间
	int i=0,j=0;
	char *arg;
	Program *prog;
	arg= strtok(line, " ");
	args[0] = (char*)malloc(strlen(arg) * sizeof(char)+1);
	strcpy(args[0], arg);
	i=1;
	while((arg = strtok(NULL, " ")) != NULL)
	{
		args[i] = (char*)malloc(strlen(arg) * sizeof(char)+1);		
		strcpy(args[i],arg);
		i++;
	}
   args[i]=NULL;
	prog = create_program(args);
	add_program(job, prog);
	for(j = 0; j < i; j++)
	{
		free(args[j]);
	}
	free(args);
}

/*释放空间*/

void destroy_job(Job *job)
{
   int i,j;
	for(i=0;i<job->progs_num;i++){
	   j=0;
		while(job->progs[i].args[j]!=NULL){
		  free(job->progs[i].args[j]);
			j++;
		}
	  free(job->progs[i].args[j]);
	  free(job->progs[i].args);
	}
	
	free(job->progs);
	free(job->cmd);
	free(job);
}

/*按,分割指令，分别保存*/

void arg_cmd(Job *job, char *line)
{
  char **args =(char**)malloc(30* sizeof(char*));
	int i=0,j=0,k=0;
	char *arg;
	Program *prog;
	arg= strtok(line, ",");
	args[0] = (char*)malloc(strlen(arg) * sizeof(char)+1);		
	strcpy(args[0], arg);
	i=1;
	while((arg = strtok(NULL, ",")) != NULL)
	{
		args[i] = (char*)malloc(strlen(arg) * sizeof(char)+1);
		strcpy(args[i],arg);
		i++;
	}
	for(k=0;k<i;k++)
	{
	  parse_cmd(job,args[k]);
	
	}
	for(j = 0; j < i; j++)
	{
		free(args[j]);
	}
	free(args);
}
	
