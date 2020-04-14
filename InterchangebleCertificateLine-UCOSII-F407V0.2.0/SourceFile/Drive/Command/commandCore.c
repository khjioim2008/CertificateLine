#include "commandCore.h"
#include "motor.h"
#include "pid.h"
#include "StepMotor/bsp_STEPMOTOR.h" 
#include "DCMotor/bsp_BDCMotor.h"

void CommandScan(uint8_t *data,uint32_t len)
{
		int i=0,k=0;
	  Job* job;
	/*复制字符串*/
	
	Command_Copy(data,len);
	
	/*初始化job*/
	
	job=create_job(buffer);
	
	/*处理命令*/
	
	arg_cmd(job,buffer);

  /*执行命令*/	
	for(i=0;i<job->progs_num;i++)
	{
		//printf("%s",job->progs[i].args[0]);
		for(k=0;k<COMMAND_NUM;k++)
		{
			if(strcmp(job->progs[i].args[0],commandStringList[k])==0)
			{
				
				Command_Proc_Func_Table[k](job->progs[i]);					
				break;
			}
		 }		
	}
	
	/*释放指针*/
	
	destroy_job(job);
} 
void CommandCallBackInit(void)
{
	CertificateCom_RecMsgCallback(CommandScan);
}

int stringint(char* a){
  int num=0,i=0; 
	while(a[i]!='\0')
	{
	   num*=10;
		 num+=a[i++]-'0';
	}
	return num;
}

void text(Program data)
{
	printf("test was successful\r\n");
}

void commandLeftHobMotor(Program data)
{
	uint8_t  function = 0;
	uint16_t time = 0;
	if(data.args[1] != NULL){
		time = stringint(data.args[1]);
		if(data.args[2] != NULL){
			function = stringint(data.args[2]);
			//controlLeftHobMotor(&CutMachine,time,function);
			printf("controlLeftHobMotor succeed!!! Time:%d,Function:%d\r\n",time,function);
		}else{
		printf("Error!!! Expected an expression_2\r\n");
		}
	}else{
		printf("Error!!! Expected an expression_1\r\n");
	}
}

void commandRightHobMotor(Program data)
{
	uint8_t  function = 0;
	uint16_t time = 0;
	if(data.args[1] != NULL){
		time = stringint(data.args[1]);
		if(data.args[2] != NULL){
			function = stringint(data.args[2]);
			//controlRightHobMotor(&CutMachine,time,function);
			printf("controlRightHobMotor succeed!!! Time:%d,Function:%d\r\n",time,function);
		}else{
		printf("Error!!! Expected an expression_2\r\n");
		}
	}else{
		printf("Error!!! Expected an expression_1\r\n");
	}
}

void commandLeftChamferMotor(Program data)
{
	uint8_t  function = 0;
	uint16_t time = 0;
	if(data.args[1] != NULL){
		time = stringint(data.args[1]);
		if(data.args[2] != NULL){
			function = stringint(data.args[2]);
			//controlLeftChamferMotor(&CutMachine,time,function);
			controlChamferMotor(&CutMachine,time,function);
			printf("controlLeftChamferMotor succeed!!! Time:%d,Function:%d\r\n",time,function);
		}else{
		printf("Error!!! Expected an expression_2\r\n");
		}
	}else{
		printf("Error!!! Expected an expression_1\r\n");
	}
}

void commandRightChamferMotor(Program data)
{
	uint8_t  function = 0;
	uint16_t time = 0;
	if(data.args[1] != NULL){
		time = stringint(data.args[1]);
		if(data.args[2] != NULL){
			function = stringint(data.args[2]);
			printf("controlRightChamferMotor succeed!!! Time:%d,Function:%d\r\n",time,function);
		}else{
		printf("Error!!! Expected an expression_2\r\n");
		}
	}else{
		printf("Error!!! Expected an expression_1\r\n");
	}
}

void commandFlotasmCutMotor(Program data)
{
	uint8_t  function = 0;
	uint16_t time = 0;
	if(data.args[1] != NULL){
		time = stringint(data.args[1]);
		if(data.args[2] != NULL){
			function = stringint(data.args[2]);
			controlFlotasmCutMotor(&CutMachine,time,function);
			printf("controlFlotasmCutMotor succeed!!! Time:%d,Function:%d\r\n",time,function);
		}else{
		printf("Error!!! Expected an expression_2\r\n");
		}
	}else{
		printf("Error!!! Expected an expression_1\r\n");
	}
}


void commandCutMachineSeq(Program data)
{
	uint8_t seq=0;
		if(data.args[1] != NULL){
		seq = stringint(data.args[1]);
		CutMachine.Sequence = seq;
		printf("controlCutMachine.Sequence Succeed!!! seq:%d\r\n",seq);
		}
    else{
		printf("Error!!! Expected an expression_2\r\n");
		}
}

void commandMachineStep(Program data)
{
	uint32_t StepNum,step,accel,decel,speed;
		if(data.args[1] != NULL){
		StepNum = stringint(data.args[1]);
			if(data.args[2] != NULL){
		   step = stringint(data.args[2]);
				if(data.args[3] != NULL){
				accel = stringint(data.args[3]);
					if(data.args[4] != NULL){
						decel = stringint(data.args[4]);
						if(data.args[5] != NULL){
							speed = stringint(data.args[5]);
							STEPMOTOR_CerAxisMoveRel(StepNum,step,accel, decel, speed);
							printf("gMachineStep Succeed!!! \r\n");
						}
					}
				}
		}
		}
    else{
		printf("Error!!! Expected an expression_1\r\n");
		}
}

void commandStampACMotor(Program data)
{
		uint16_t time;
		uint8_t status;
			if(data.args[1] != NULL){
				time = stringint(data.args[1]);
				if(data.args[2] != NULL){
					status = stringint(data.args[2]);
				controlStampACMotor(&StampMachine,time,status);
			printf("controlPIDStepMotorAdd succeed!!! time:%d,status:%d\r\n",time,status);
				}else{
				printf("Error!!! Expected an expression_2\r\n");
				}
			}else{
				printf("Error!!! Expected an expression_1\r\n");
			}

}

void commandStampStepAdd(Program data)
{
	uint32_t step,accel,decel,speed;
		if(data.args[1] != NULL){
			if(data.args[2] != NULL){
		   step = stringint(data.args[2]);
				if(data.args[3] != NULL){
				accel = stringint(data.args[3]);
					if(data.args[4] != NULL){
						decel = stringint(data.args[4]);
						if(data.args[5] != NULL){
							speed = stringint(data.args[5]);
					  	STEPMOTOR_CerAxisMoveRel(1,step,accel, decel, speed);
							printf("controlPagingMachineStep Succeed!!! \r\n");
						}
					}
				}
		}
		}
    else{
		printf("Error!!! Expected an expression_1\r\n");
		}
}

void commandStampSeq(Program data)
{
	if(data.args[1] != NULL){
	StampMachine.Sequence = 1;
	printf("controlStampSeq Succeed!!! \r\n");	
	}else{
	printf("Error!!! Expected an expression_1\r\n");
	}
}

void commandPagingSeq(Program data)
{
		uint16_t S_Tar = 0;
		if(data.args[1] != NULL){

			//cerStepSrd[PLASTIC_STEP].decel_start = UINT32_MAX;
			if(data.args[2] != NULL){
				S_Tar =  stringint(data.args[2]);
				S_target = S_Tar;
				
			
			//controlPIDPagingMachineStep();
			
			//controlHobMotor(&PagingMachine,0,3);
		//	PagingMachine.Sequence   = 1;
			//STEPMOTOR_CerAxisMoveRel(PLASTIC_STEP,INT32_MAX,100, 100,35); //20mm/s
		 controlPagingMachine(&PagingMachine);

				//	cerStepSrd[FRICTION_STEP].decel_start = UINT32_MAX;
	
			printf("S_Target:%.2f\r\n",S_target);
			}else{
				printf("Error!!! Expected an expression_2\r\n");

			}
			
			printf("controlPagingSeq Succeed!!! \r\n");	
	}else{
	printf("Error!!! Expected an expression_1\r\n");
	}
}

void commandPagingConveyerMotor(Program data)
{
	uint8_t  function = 0;
	uint16_t time = 0;
	if(data.args[1] != NULL){
		time = stringint(data.args[1]);
		if(data.args[2] != NULL){
			function = stringint(data.args[2]);
		//	controlRightChamferMotor(&CutMachine,time,function);
			controlPagingConveyerMotor(&PagingMachine,time,function);
			printf("controlRightChamferMotor succeed!!! Time:%d,Function:%d\r\n",time,function);
		
		
		
		}else{
		printf("Error!!! Expected an expression_2\r\n");
		}
	}else{
		printf("Error!!! Expected an expression_1\r\n");
	}
}
void commandHobMotor(Program data)
{
		uint8_t  function = 0;
	uint16_t time = 0;
	if(data.args[1] != NULL){
		time = stringint(data.args[1]);
		if(data.args[2] != NULL){
			function = stringint(data.args[2]);
		//	controlRightChamferMotor(&CutMachine,time,function);
			controlHobMotor(&PagingMachine,time,function);
			printf("controlRightChamferMotor succeed!!! Time:%d,Function:%d\r\n",time,function);
		}else{
		printf("Error!!! Expected an expression_2\r\n");
		}
	}else{
		printf("Error!!! Expected an expression_1\r\n");
	}
}

void commandPIDMotor(Program data)
{
	controlPIDPagingMachineStep();
	printf("commandPIDMotor succeed!!!");
	
}

void commandCutStep(Program data)
{
	uint32_t StepNum,step,accel,decel,speed;
		if(data.args[1] != NULL){
		StepNum = stringint(data.args[1]);
			if(data.args[2] != NULL){
		   step = stringint(data.args[2]);
				if(data.args[3] != NULL){
				accel = stringint(data.args[3]);
					if(data.args[4] != NULL){
						decel = stringint(data.args[4]);
						if(data.args[5] != NULL){
							speed = stringint(data.args[5]);
								printf("gMachineStep Succeed!!! \r\n");
							STEPMOTOR_CerAxisMoveRel_CutTim9(StepNum,step,accel, decel, speed);
							printf("CutStep Succeed!!! \r\n");
						}
					}
				}
		}
		}
    else{
		printf("Error!!! Expected an expression_1\r\n");
		}
}

