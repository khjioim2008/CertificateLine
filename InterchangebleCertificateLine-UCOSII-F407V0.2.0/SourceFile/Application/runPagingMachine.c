/*-------------------------------<< >>-----------------------------------*/
/*
 * - :CRJ--
 * - :20170000.000.00.1  
 * - 
 * - IC 
 * - Level:0 ,1
 * - :STM32F407-
 **/
/*___________________________________END______________________________________*/

#include "Drives.h"
#include "Resources.h"
#include "Configure.h"

#include "stm32f4xx_hal.h"
#include "key/bsp_key.h"
#include "usartx/bsp_usartx.h"
#include "EncoderTIM/bsp_EncoderTIM.h"
#include "DCMotor/bsp_BDCMotor.h"
#include "Sensor/bsp_Sensor.h"
#include "StepMotor/bsp_STEPMOTOR.h" 
#include "stepLinkedList.h"
#include  <stdlib.h>
#include  <string.h>
#include <stdio.h>

#include "includes.h"
//#define SAMPLING                  0x01    // �������
//__IO uint16_t time_count = 0;              // ʱ�������ÿ1ms����һ(��δ�ʱ��Ƶ���й�)
//__IO uint8_t Time_Flag   = 0;              // ����ʱ����

#define PLASTIC_STEP_SPEED     20 //���ܹ����ٶ� ��λ��mm/s
#define ROLLER_STEP_SPEED     100 //׷����ֵ��ٶ� ��λ��mm/s
#define ROLLER_AND_PLASTIC_DIS    26300 //��������֮��ľ���  ��λ0.001mm 26.3mm

extern __IO uint16_t Machine_Status;
extern OS_EVENT *CutFullSem; // ��ӡ�������ź���
extern int32_t PagingCaptureNumber;     // ���벶����
extern int16_t OverflowCountTwo;        //������������� ������
extern int32_t CaptureNumber;
extern int16_t OverflowCount;

float S_plastic;
float S_target = 0; //��λ0.001mm 
uint16_t S_waitTime;  //�ȴ�ʱ��
uint16_t S_chaseTime; //׷��ʱ��
uint8_t firstStatus;

__IO uint16_t machinePaperCount = 0; 

/*
  * ��������: ��ҳһ��ֽ
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
static void runPagingPaper(PagingMotorns_Struct * Name)
{
		switch(Name->PagingSeq)
		{
			case 0:
				break;
			case 1: //6400*2 
				break;
			case 2:
					STEPMOTOR_CerAxisMoveRel(FRICTION_STEP,12800,5000,5000,FRICTION_STEP_SPEED);
				//	cerStepSrd[FRICTION_STEP].decel_start = UINT32_MAX;	//�޸Ĳ���ֵ,ʹ�õ�����˶������в������,���ָ�������STEPMOTOR_AxisMoveRel����֮��ִָ��
					Name->PagingSeq = 3;

				break;
			case 3:
				if(SignPagingFrictionOrigin.SIO == 0){
					Name->PagingSeq = 4;
				}
				break;
			case 4:
				if(SignPagingFrictionOutOne.SIO == 1){
				  cerStepSrd[FRICTION_STEP].run_state = STOP;
					//listStopCmd[FRICTION_STEP]  = True;
						Name->PagingSeq = 5;
				}
				else if(cerMotionStatus[FRICTION_STEP] == 0){ //ת��һת��ûֽ
					Name->PagingSeq = 8;
				}
				break;
				case 5:
			  break;
				case 6:
				STEPMOTOR_CerAxisMoveRel(FRICTION_STEP,12800,5000,5000,FRICTION_STEP_SPEED);
				cerStepSrd[FRICTION_STEP].decel_start = UINT32_MAX;	//�޸Ĳ���ֵ,ʹ�õ�����˶������в������,���ָ�������STEPMOTOR_AxisMoveRel����֮��ִָ��				break;
			  Name->PagingSeq = 7;
				break;
				case 7:
				if(SignPagingFrictionOrigin.SIO == 1){
					cerStepSrd[FRICTION_STEP].run_state = STOP;
				}
				break;
				case 8:
					break;
		}
}

static void controlPagingPaper(PagingMotorns_Struct * Name)
{
	Name->PagingSeq = 2;
}

static void controlPagingPaperStepTwo(PagingMotorns_Struct * Name)
{
	Name->PagingSeq = 6;
}

static uint8_t readPagingPaper(PagingMotorns_Struct * Name)
{
	return Name->PagingSeq;
}

/*
  * ��������: ���ֽ�� 
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
static void runRollerPaper(PagingMotorns_Struct * Name)
{
	Name->DisTimeCount ++;
	if(Name->PagingHeatingCount > 0){
	  Name->PagingHeatingCount --;
		Machine_Status |= PLASTIC_STEP_MOVE;
	}else{
		if((cerMotionStatus[PLASTIC_STEP] == 1) && (Name->RollerSeq!=0)){
			
			listStopCmd[PLASTIC_STEP] = True;
			Machine_Status &= ~PLASTIC_STEP_MOVE;
		}
	}
		switch(Name->RollerSeq)
		{
			case 0:
				
				break;
			case 1: //6400*2 
				break;
			case 2:
				if(Name->FirstPaperStatus == 0){//36 Լ���� 45mm/s
					STEPMOTOR_CerAxisMoveRel(ROLLER_STEP,INT32_MAX,500,3000,ROLLER_SYNC_SPEED);
					cerStepSrd[ROLLER_STEP].decel_start = UINT32_MAX;
					Name->RollerSeq = 3;
				}else{
					STEPMOTOR_CerAxisMoveRel(ROLLER_STEP,INT32_MAX,500,3000,ROLLER_INITIAL_SPEED);
					cerStepSrd[ROLLER_STEP].decel_start = UINT32_MAX;
					Name->RollerSeq = 3;
				}
				break;
			case 3:
				if(SignPagingRollerOut.SIO == 1){ 
					if(cerMotionStatus[PLASTIC_STEP] == 0){
					STEPMOTOR_CerAxisMoveRel(PLASTIC_STEP,INT32_MAX,100, 100,35); //������20mm/s
					listStopCmd[PLASTIC_STEP] = False;
					
					}
						//ֽ�� SignPagingRollerOut ������Ƭ���ֵľ�����86.3MM
					//(125mm + 86.3mm )/20mm = 10.565s
					Name->PagingHeatingCount = 10565;
					if(Name->FirstPaperStatus == 0){
						Name->FirstPaperStatus = 1;
						//cerStepSrd[ROLLER_STEP].medle_delay = (int32_t)(A_T_x10/ROLLER_SYNC_SPEED);//�ع��ٶȵĶ�ʱ��ֵ
						//cerStepSrd[ROLLER_STEP].decel_start = 0;	//���ÿ�ʼ���ٵ�λ��
						//cerStepSrd[ROLLER_STEP].run_state = RUN;	//���ָ�������STEPMOTOR_AxisMoveRel����֮��ִָ��					
						
						machinePaperCount ++;  //ֽ�ż�һ 2020.4.11
						Name->RollerSeq = 6; //��һ��
					}else{
					//	printf("����ֵ:%d\r\n",Name->DisTimeCount);
						/*
						�ڶ����ڵ�һ��β����ʧ��ʱ�򣬵��ﴫ������ʱ��t1
						�����ֵ��ٶ�Ϊ�㶨Vs = 20mm/s
						�ڶ������һ�ŵļ��S= Vs * t1 = 20 * 0.4 = 8 mm;
						ʵ������10mm����
						*/
						S_plastic = PLASTIC_STEP_SPEED * Name->DisTimeCount;  //20*0.4 = 8  S_plastic �ĵ�λ��0.001 mm
						if(S_plastic < S_target){
						 cerStepSrd[ROLLER_STEP].run_state = STOP;	
						 S_waitTime = (S_target - S_plastic) / PLASTIC_STEP_SPEED; 
						 machinePaperCount ++;  //ֽ�ż�һ 2020.4.11
						 Name->RollerSeq = 4;
						}
						else{//else if(S_plastic >= S_target){ //100mm/s �ٶ�׷��							
						if ((S_plastic - S_target) >= ROLLER_AND_PLASTIC_DIS){//׷���� 26300�ǻ�е�ṹ����
						//׷��ʱ��Tmax = 26.3mm / 100mm/s = 0.263s = 263ms	
						//ROLLER_AND_PLASTIC_DIS / ROLLER_STEP_SPEED = 263ms
								//S_chaseTime = 263;
								 	S_chaseTime = 263; //2020.4.3
							 }else{//׷����
									S_chaseTime = (S_plastic - S_target) / ROLLER_STEP_SPEED; //100mm/s
							 }
							 machinePaperCount ++;  //ֽ�ż�һ 2020.4.11
							 Name->RollerSeq = 5;
						}
//						printf("S_plastic:%.2f\r\n",S_plastic);
//						printf("S_waitTime:%d\r\n",S_waitTime);
//					  printf("S_chaseTime:%d\r\n",S_chaseTime);
					}
				}
				break;
			case 4:
					if(S_waitTime > 0){
						S_waitTime --;
					}else{
					  STEPMOTOR_CerAxisMoveRel(ROLLER_STEP,INT32_MAX,500,3000,ROLLER_SYNC_SPEED);
						S_waitTime = 0;
						Name->RollerSeq = 6;
					}
				break;
			case 5:
					if(S_chaseTime > 0){
						S_chaseTime --;
					}else{
						cerStepSrd[ROLLER_STEP].medle_delay = (int32_t)(A_T_x10/ROLLER_SYNC_SPEED);//�ع��ٶȵĶ�ʱ��ֵ
						cerStepSrd[ROLLER_STEP].decel_start = 0;	//���ÿ�ʼ���ٵ�λ��
						cerStepSrd[ROLLER_STEP].run_state = RUN;	//���ָ�������STEPMOTOR_AxisMoveRel����֮��ִָ��								S_waitTime = 0;
						S_chaseTime = 0;
						Name->RollerSeq = 6;
						}
				break;
			case 6:
					if(SignPagingRollerOut.SIO == 0){
						Name->RollerSeq = 2;
						Name->DisTimeCount = 0;
					}
					break;
				
		}
}

static void controlRollerPaper(PagingMotorns_Struct * Name)
{
	Name->RollerSeq = 2;
	Name->DisTimeCount = 0;
	Name->FirstPaperStatus = 0;
}

static uint8_t readRollerPaper(PagingMotorns_Struct * Name)
{
	return Name->RollerSeq;
}

static void closeRollerPaper(PagingMotorns_Struct * Name)
{
	listStopCmd[ROLLER_STEP] = True;
}


static void closePagingMachine(PagingMotorns_Struct * Name)
{
	controlPagingConveyerMotor(Name,0,0);
	controlHobMotor(&PagingMachine,0,0);
	closeRollerPaper(Name);
}

void controlPagingMachine(PagingMotorns_Struct * Name)
{
	Name->PagingTryCount = 0; //��������
	
	Machine_Status &= ~NO_PAPER_TRUE; //��ֽ
	
	PagingMachine.Sequence   = 1;	
	controlHobMotor(&PagingMachine,0,3);
	
 controlRollerPaper(&PagingMachine);	
	
}

/*
  * ��������: ��ҳӦ�ú���
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
void runPagingMachine(PagingMotorns_Struct * Name)
{

	switch(Name->Sequence)
	{
		case 0:
			break;
		case 1:		
		if(SignPagingRollerOut.SIO == 0){//��Ҫֽ
			Name->Sequence = 2;
		}
		break;
		case 2:
		if((SignPagingFrictionOutTwo.SIO == 0) && (SignPagingFrictionOutThree.SIO == 0)
		&& (SignPagingFrictionOutFour.SIO == 0)){
			Name->Sequence = 3;
		}else{
			controlPagingConveyerMotor(Name,0,3);
		}
			break;
		case 3:
		if(SignPagingFrictionOutOne.SIO == 1){
			controlPagingPaperStepTwo(Name);
			controlPagingConveyerMotor(Name,0,3);
		}else{
			
		}
		Name->Sequence = 5;
			break;
		case 4:
			break;
		
		
		
		
		case 5:
			if(Name->PagingTryCount < 3){
			if(SignPagingFrictionOutOne.SIO == 0){	
				controlPagingPaper(Name);
				controlPagingConveyerMotor(Name,0,3);
				Name->PagingTryCount ++;
				Name->Sequence = 6;
			}else{//��ʼ��ʱ����ֽ,��ǰ����
				controlPagingConveyerMotor(Name,0,3);
				//controlRollerPaper(Name);
			}
		}else{//����5���ж�Ϊ��ֽ
			Name->Sequence = 8;
		}
			break;
		case 6:
			if(readPagingPaper(Name) == 5){ //�ڶ���
					if(!(Machine_Status & BEHIND_TRUE)){
						Name->Sequence = 7;
						Name->PagingTryCount = 0;  //��ֽ��������
					}
			}else if(readPagingPaper(Name) == 8){ //������
				Name->Sequence = 5;
			}
			break;
		case 7:
		if((SignPagingFrictionOutTwo.SIO == 0) && (SignPagingFrictionOutThree.SIO == 0)
		&& (SignPagingFrictionOutFour.SIO == 0)){
			controlPagingPaperStepTwo(Name);
			Name->Sequence = 5;
		}
			break;
		case 8:
		if((SignPagingFrictionOutOne.SIO == 0)&&(SignPagingFrictionOutTwo.SIO == 0)
			&&(SignPagingFrictionOutThree.SIO == 0)&&(SignPagingFrictionOutFour.SIO == 0)
			&&(SignPagingRollerOut.SIO == 0)){
					if(Machine_Status & WORK_FINISH){
						Name->Sequence = 9;
					}else{
						closePagingMachine(Name);
						Name->Sequence = 9;
					}
		}
			break;
		
		case 9:
			if(Machine_Status & WORK_FINISH){
				if(machinePaperCount > 0){
					Name->PagingHeatingCount = 2000;
					if(cerMotionStatus[PLASTIC_STEP] == STOP){
						listStopCmd[PLASTIC_STEP] = False;
						STEPMOTOR_CerAxisMoveRel(PLASTIC_STEP,INT32_MAX,100, 100,35); //������20mm/s
					}
				}else{
					Machine_Status &= ~WORK_FINISH;
					Name->Sequence = 0;
				}
			}
			break;
		
	}

		runPagingPaper(Name);
		runRollerPaper(Name);
		runHobMotor(Name);
		runPagingConveyerMotor(Name);
	
	//signalStabilityT(&SignPagingExistPaper,sensorPagingExistPaper,2000);
		signalStability(&SignPagingFrictionOrigin,sensorPagingFrictionOrigin);
		
		signalStability(&SignPagingFrictionOutOne,sensorPagingFrictionOutOne);
		signalStability(&SignPagingFrictionOutTwo,sensorPagingFrictionOutTwo);
		signalStability(&SignPagingFrictionOutThree,sensorPagingFrictionOutThree);
		signalStability(&SignPagingFrictionOutFour,sensorPagingFrictionOutFour);
		signalStability(&SignPagingRollerOut,sensorPagingRollerOut);

}

/*
  * ��������: �ֶ���Ĥ���ƺ���
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
void manualPlasticMem(PagingMotorns_Struct *Name)
{
		if(Name->Sequence != 0)return;
		while(cerMotionStatus[PLASTIC_STEP]);
		STEPMOTOR_CerAxisMoveRel(PLASTIC_STEP,215,10,10,50);//215 -- 2mm
}

/*
  * ��������: ������Ĥ���ƺ���
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
void continuousPlasticMem(PagingMotorns_Struct *Name)
{
		if(Name->Sequence != 0)return;
		while(cerMotionStatus[PLASTIC_STEP]);
		STEPMOTOR_CerAxisMoveRel(PLASTIC_STEP,INT32_MAX,10,10,50); //215 -- 2mm
		Name->ManualStatus = 1;
}

/*
  * ��������:
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
void stopPlasticMem(PagingMotorns_Struct *Name)
{
			if(Name->Sequence != 0)return;
			if(Name->ManualStatus == 1){
				Name->ManualStatus = 0;
			  listStopCmd[PLASTIC_STEP] = True;
			}
}
