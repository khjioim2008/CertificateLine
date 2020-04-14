#include "commandUtil.h"
#include "string.h"
#include "stdlib.h"
//#include "uart.h"
char buffer[500];

/*�������buffer*/

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

/*��ʼ��*/

Job* create_job(char *cmd)
{
	Job *job = (Job*)malloc(sizeof(Job));
	job->cmd = (char*)malloc(sizeof(char) * strlen(cmd));
	strcpy(job->cmd, cmd); 
	job->progs_num = 0;
	job->progs = NULL;
	return job;
}

/*ͨ������ָ���ѯ��������*/

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

/*���ַ����������Program�ṹ�������*/

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

/*��ӵ�job������*/

void add_program(Job *job, Program *prog)
{
	Program *ps =(Program*)malloc(sizeof(Program) * (job->progs_num+1));
	memcpy(ps, job->progs,job->progs_num * sizeof(Program));
	ps[job->progs_num++] = *prog;
	free(prog);
	free(job->progs);
	job->progs = ps;
}

/*���ո�ָ�ָ��������*/

void parse_cmd(Job *job, char *line)
{
	char **args =(char**)malloc(30 * sizeof(char*));//����ռ�
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

/*�ͷſռ�*/

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

/*��,�ָ�ָ��ֱ𱣴�*/

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
	
