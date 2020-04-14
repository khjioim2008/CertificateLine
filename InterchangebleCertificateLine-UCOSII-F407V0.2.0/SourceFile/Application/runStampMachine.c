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

#include "includes.h"

#include  <stdlib.h>
#include  <string.h>
#include <stdio.h>


__IO uint8_t StampPostion_Status; // ��ӡλ����ر�־
__IO uint8_t StampWait_Status   = 0; //���ϱ�־
__IO uint8_t StampNeed_Status   = 0;//��Ҫ��ӡ���

extern OS_EVENT *CutFullSem; // ��ӡ�������ź���

/*--------------------------����״̬---------------------------------*/
void runStampPostionSensor (void)
{
	if((SignStampPostion.SIO == 1) && (StampPostion_Status == 0)) StampPostion_Status = 1;	//ֽ����
	else if((SignStampPostion.SIO == 1) && (StampPostion_Status == 1)) StampPostion_Status = 2;	//��ֽ
	else if((SignStampPostion.SIO == 0) && (StampPostion_Status == 2)) StampPostion_Status = 3;	//����ֽ����
	else if((SignStampPostion.SIO == 0) && (StampPostion_Status == 3)) StampPostion_Status = 0;	//��ֽ
}

/**
  * ��������: ��ӡ��Ӧ�ú���
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
void runStampMachine(StampMotorns_Struct *Name)
{	
//	uint8_t err; 
		if(SignStampWaitDown.SIO == 1 && (Name->Sequence != 0)){ //ֹͣ
			listStopCmd[PLASTIC_STEP] = True;
			StampWait_Status = 1;
		}else{
			if((StampWait_Status == 1) && (SignStampWaitUp.SIO == 1)){//�ָ�
		 // if(StampWait_Status == 1 ){//�ָ�
				StampWait_Status = 0;
				listStopCmd[PLASTIC_STEP] = False;
				STEPMOTOR_CerAxisMoveRel(PLASTIC_STEP,INT32_MAX,10,10,STAMP_STEP_SPEED);
				//OSSemPost(CutFullSem);
				//OSSemSet(CutFullSem,10,&err);
			}
		}
  	switch(Name->Sequence)
		{
			case 0:
			//	Name->Sequence = 1;
				break;
			case 1:
				STEPMOTOR_CerAxisMoveRel(PLASTIC_STEP,INT32_MAX,10,10,STAMP_STEP_SPEED);
				Name->Sequence = 2;
				break;
			case 2:
				if(StampPostion_Status == 2){ //��һ��
					listStep_count[PLASTIC_STEP] = 0;
					printf("listStep_count = %d\r\n",listStep_count[PLASTIC_STEP]);	
					pushBack(&StampStepList,DIS_STAMP_AND_SENSOR,StampAC);
					showList(&StampStepList);
					Name->Sequence = 3;
				}
				break;
			case 3:
				if(StampPostion_Status == 0){
					Name->Sequence = 4;
				}
			break;
			case 4:
				if(StampPostion_Status == 2){//��һ��
					if(StampStepList.nodeCount != 0){
					//uint32_t ȡֵ��Χ0~4,294,967,295 
						if(listStep_count[PLASTIC_STEP] < (UINT32_MAX - DIS_STAMP_AND_SENSOR)){
							pushBack(&StampStepList,DIS_STAMP_AND_SENSOR + listStep_count[PLASTIC_STEP],StampAC);
							printf("listStep_count = %d\r\n",listStep_count[PLASTIC_STEP]);
							showList(&StampStepList);
						}else{//�������
							showList(&StampStepList);
							overFlowSetList(&StampStepList,(float)listStep_count[PLASTIC_STEP]); 
							showList(&StampStepList);
							}
					}else{
						listStep_count[PLASTIC_STEP] = 0;
						printf("listStep_count = %d\r\n",listStep_count[PLASTIC_STEP]);
						pushBack(&StampStepList,DIS_STAMP_AND_SENSOR,StampAC);
						showList(&StampStepList);
					}
					Name->Sequence = 3;
				}
			break;
		}
		
		if(StampStepList.nodeCount != 0){
			Node *p = StampStepList.headNode->next;
			//ǿ��ת��ȡ���Ƚϣ�Ӧ��û������
			if(listStep_count[PLASTIC_STEP] >= (uint32_t) p->StopPostion){
				StampNeed_Status = 1;
				listStopCmd[PLASTIC_STEP] = True;
			}
		}else{//2020.1.13����Ϊ�գ���������
			listStep_count[PLASTIC_STEP] = 0;
		}
//		if(StampStepList.nodeCount != 0){
		if((StampStepList.nodeCount != 0) && (StampNeed_Status == 1)){
			if(cerMotionStatus[PLASTIC_STEP] == 0){
				switch(Name->StepSeq)
				{
					case 0:
						controlStampACMotor(Name,10000,2);
					  Name->StepSeq = 1;
						break;
					case 1:
						if(readStampACMotor(Name) == 1){
							popFront(&StampStepList);
							printf("listStep_count = %d\r\n",listStep_count[PLASTIC_STEP]);
							showList(&StampStepList);
							listStopCmd[PLASTIC_STEP] = False;
							STEPMOTOR_CerAxisMoveRel(PLASTIC_STEP,INT32_MAX,10,10,STAMP_STEP_SPEED);
							StampNeed_Status = 0;
							Name->StepSeq = 0;
						}
						break;
			 }
			}
		}
		runStampACMotor(Name);
		runStampPostionSensor();
		signalStability(&SignStampPostion,sensorStampPostion);
		signalStability(&SignStampACMotor,sensorStampACMotor);
		signalStability(&SignStampWaitUp,sensorStampWaitUp);
		signalStability(&SignStampWaitDown,sensorStampWaitDown);
}
