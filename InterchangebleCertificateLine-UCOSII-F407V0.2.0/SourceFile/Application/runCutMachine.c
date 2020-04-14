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
#include "stm32f4xx_hal.h"
#include "Configure.h"
#include "DCMotor/bsp_BDCMotor.h"
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

/* ˽�б��� ------------------------------------------------------------------*/
__IO uint8_t PostionSensor_Status   = 0;    
__IO uint8_t CutFlasmPostionSensor_Status   = 0;
__IO uint8_t needCutting_Status   = 0;
__IO uint8_t Wait_Status   = 0;
__IO uint8_t Syncs_Status   = 0;
__IO uint8_t Accel_Status   = 0;
int32_t CaptureNumberPre;
int32_t HeadStep_countPre;
int32_t TailStep_countPre;

extern __IO uint16_t Machine_Status;
extern __IO uint16_t machinePaperCount;
/* ��չ���� ------------------------------------------------------------------*/
extern int32_t CaptureNumber;     // ���벶����
extern int16_t OverflowCount;              //������������� ������
extern int32_t SUM_Pulse;


/*--------------------------����״̬---------------------------------*/
void runChamferPostionSensor (void)
{
	if((SignCutChamferPostion.SIO == 1) && (PostionSensor_Status == 0)) PostionSensor_Status = 1;	//ֽ����
	else if((SignCutChamferPostion.SIO == 1) && (PostionSensor_Status == 1)) PostionSensor_Status = 2;	//��ֽ
	else if((SignCutChamferPostion.SIO == 0) && (PostionSensor_Status == 2)) PostionSensor_Status = 3;	//����ֽ����
	else if((SignCutChamferPostion.SIO == 0) && (PostionSensor_Status == 3)) PostionSensor_Status = 0;	//��ֽ
}

/*--------------------------����״̬---------------------------------*/
void runCutFlasmPostionSensor (void)
{
	if((SignCutFlotasmPostion.SIO == 1) && (CutFlasmPostionSensor_Status == 0)) CutFlasmPostionSensor_Status = 1;	//ֽ����
	else if((SignCutFlotasmPostion.SIO == 1) && (CutFlasmPostionSensor_Status == 1)) CutFlasmPostionSensor_Status = 2;	//��ֽ
	else if((SignCutFlotasmPostion.SIO == 0) && (CutFlasmPostionSensor_Status == 2)) CutFlasmPostionSensor_Status = 3;	//����ֽ����
	else if((SignCutFlotasmPostion.SIO == 0) && (CutFlasmPostionSensor_Status == 3)) CutFlasmPostionSensor_Status = 0;	//��ֽ
}

//uint8_t readChamferPostionSensor(void)
//{
//	return PostionSensor_Status;
//}
void controlCutMachine(CutMotorns_Struct *Name)
{
	Machine_Status   = 0;
	Syncs_Status   = 0;
	Accel_Status   = 0;
	if(Name->Sequence == 0){
	 Name->Sequence = 1;
	}else{
		Machine_Status |= RELOAD_PAPER;	
	}

}
/*
  * ��������: ����Ӧ�ú���
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
void controlCutMachineSpeed(CutMotorns_Struct *Name)
{
		if(SignCutMachineFront.SIO == 1){
				STEPMOTOR_CerAxisMoveRel_CutTim9(CUT_ROLLER_STEP,INT32_MAX,100,5000,35); //ͬ��
				Syncs_Status = 1;
				Accel_Status = 0;
		}else{
				STEPMOTOR_CerAxisMoveRel_CutTim9(CUT_ROLLER_STEP,INT32_MAX,100,5000,70); //����
				Accel_Status = 1;
				Syncs_Status = 0;
			}
}

static void CutMachineSpecialHandle(CutMotorns_Struct *Name)
{
			if((SignCutMachineFront.SIO == 1) && (needCutting_Status != 1) && (Name->Sequence != 0)){
				if(!Syncs_Status){
					STEPMOTOR_CerAxisMoveRel_CutTim9(CUT_ROLLER_STEP,INT32_MAX,100,5000,35); //ͬ��
					Syncs_Status = 1;
					Accel_Status = 0;
					Machine_Status &= ~BEHIND_TRUE;
				}else{
					//Syncs_Status = 1;
					
				}
			}
		if(SignCutMachineBehind.SIO == 1){
					//������ 2020.4.13
				 Machine_Status |= BEHIND_TRUE;
		}
}

void runCutMachine(CutMotorns_Struct *Name)
{
	if(Name->Sequence > 1){
		if(cerMotionStatus[PLASTIC_STEP] == 0 ){
			if(cerCutMotionStatus[CUT_ROLLER_STEP] == 1){ 
				listCutStepStopCmd[CUT_ROLLER_STEP] = True;
				Name->PressingStatus = 1; 
			}else{}
			
			if(needCutting_Status == 0){
					return;
				}
		}else{
			if(Name->PressingStatus == 1){
				listCutStepStopCmd[CUT_ROLLER_STEP] = False;
				controlCutMachineSpeed(Name);
				Name->PressingStatus = 0; 
			}
		}
	}
	CutMachineSpecialHandle(Name);
			switch(Name->Sequence)
			{
				case 0u:
					break;
				case 1u:
				if(cerMotionStatus[PLASTIC_STEP] == 1){
					controlCutMachineSpeed(Name);
					Name->Sequence = 2u;
				}
				break;
				case 2u:
				if(CutFlasmPostionSensor_Status == 2){//ͷ����
					listCutStepStopCmd[CUT_ROLLER_STEP] = True;
					needCutting_Status = 1;
					Name->Sequence = 3u;
				}
				break;
				case 3u:
				if(cerCutMotionStatus[CUT_ROLLER_STEP] == 0){
					controlFlotasmCutMotor(Name,0,2);
					Name->Sequence = 4u;
				}
				break;
				case 4://��һ��ͷ
					if(readFlotasmCutMotor(Name) == 1){
						needCutting_Status = 0;
						//STEPMOTOR_CerAxisMoveRel_CutTim9(CUT_ROLLER_STEP,INT32_MAX,100,3000,45);
						controlCutMachineSpeed(Name);
						printf("��һ��ͷ\r\n");
						listCutStep_count[CUT_ROLLER_STEP] = 0;
						Name->Sequence = 5u;
					}
					break;
					
					
					
					
				case 5:
				if(CutFlasmPostionSensor_Status == 0){ //β��
					TailStep_countPre = listCutStep_count[CUT_ROLLER_STEP];
					//printf("listCutStep_countt[CUT_ROLLER_STEP]=%d",listCutStep_count[CUT_ROLLER_STEP]);
					//408 ---- 5����
					if((listCutStep_count[CUT_ROLLER_STEP] - HeadStep_countPre) > 4076){//������С��֤�����
						pushBack(&CutStepList, 734 + listCutStep_count[CUT_ROLLER_STEP],CutFlotasm);
						printf("β��\r\n");
						showList(&CutStepList);
						if(machinePaperCount > 0){
						machinePaperCount --;
					}
					}else{
						printf("������ʧ��\r\n");	
					}
	
						Name->Sequence = 6u;
				}
					break;
				case 6:
				if(CutFlasmPostionSensor_Status == 2){ //��һ�ŵ�ͷ
					HeadStep_countPre = listCutStep_count[CUT_ROLLER_STEP];
					if((listCutStep_count[CUT_ROLLER_STEP] - TailStep_countPre) > 978){//����10����
						pushBack(&CutStepList, 0 + listCutStep_count[CUT_ROLLER_STEP],CutFlotasm); 
						showList(&CutStepList);
						printf("��һ�ŵ�ͷ����β�ʹ���10����\r\n");
						Name->Sequence = 	5u;
					}else{
						printf("��һ�ŵ�ͷ����β��С��10����\r\n");
						showList(&CutStepList);
						Name->Sequence = 	5u;
					}						
				}
					break;
				case 7:
					break;
	    }
			
				//��˳�����У����ñ�����ȡ��һ��Ӧ�ÿ���
			if((CutStepList.nodeCount != 0) && (cerCutMotionStatus[CUT_ROLLER_STEP])){
				Node *p = CutStepList.headNode->next;				
				if(listCutStep_count[CUT_ROLLER_STEP] >= p->StopPostion){
					listCutStepStopCmd[CUT_ROLLER_STEP] = True;
					Name->ActionSeq = 1;
					popFront(&CutStepList);
				}else{//2020.1.13����Ϊ�գ���������
				 //clearEnconder();
				}
			}	
				//���������ͣ�����󣬶�����صĶ���
				if(cerCutMotionStatus[CUT_ROLLER_STEP] == 0){
					switch(Name->ActionSeq)
					{
						case 0:
							break;
						case 1:
								controlFlotasmCutMotor(Name,0,2);
								needCutting_Status = 1; 
								Name->ActionSeq = 2;
							break;
						case 2:
								if(readFlotasmCutMotor(Name) == 1){
									listCutStepStopCmd[CUT_ROLLER_STEP] = False;
								//	STEPMOTOR_CerAxisMoveRel_CutTim9(CUT_ROLLER_STEP,INT32_MAX,100,3000,45);
									 controlCutMachineSpeed(Name);
							  	 needCutting_Status = 0; 									
									Name->ActionSeq = 0;
								}
							break;
					}
				}

			//runChamferMotor(Name);
			runFlotasmCutMotor(Name);
			 
			runCutFlasmPostionSensor();
			signalStability(&SignCutFlotasmPostion,sensorCutFlotasmPostion);
			signalStability(&SignCutLeftFlotasmCut,sensorCutLeftFlotasm);
			signalStability(&SignCutRightFlotasmCut,sensorCutRightFlotasm);
			//signalStability(&SignCutChamferMotor,sensorCutChamferMotor);
			//signalStability(&SignCutChamferPostion,sensorCutChamferPostion);
			signalStability(&SignCutMachineFront,sensorCutMachineFront);
			signalStability(&SignCutMachineBehind,sensorCutMachineBehind);
			
}

/*
  * ��������: �����ֶ���Ĥ���ƺ���
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
void manualCutMem(CutMotorns_Struct *Name)
{
	if(Name->Sequence != 0)return;
		while(cerCutMotionStatus[CUT_ROLLER_STEP]);
	//	STEPMOTOR_CerAxisMoveRel(CUT_FLASM_STEP,215,10,10,50); //215 -- 2mm
	STEPMOTOR_CerAxisMoveRel_CutTim9(CUT_ROLLER_STEP,215,10,10,45);
}

/*
  * ��������: ����������Ĥ���ƺ���
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
void continuousCutMem(CutMotorns_Struct *Name)
{
	if(Name->Sequence != 0)return;
	    while(cerCutMotionStatus[CUT_ROLLER_STEP]);
			STEPMOTOR_CerAxisMoveRel_CutTim9(CUT_ROLLER_STEP,INT32_MAX,10,10,45);
			Name->ManualStatus = 1;
}

/*
  * ��������: ����ֹͣ��Ĥ���ƺ���
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
void StopCutMem(CutMotorns_Struct *Name)
{
		if(Name->Sequence != 0)return;
		if(Name->ManualStatus == 1){
			Name->ManualStatus = 0;
			listCutStepStopCmd[CUT_ROLLER_STEP] = True;
		}
}
