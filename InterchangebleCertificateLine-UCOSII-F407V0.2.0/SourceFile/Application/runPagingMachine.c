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
//#define SAMPLING                  0x01    // 采样标记
//__IO uint16_t time_count = 0;              // 时间计数，每1ms增加一(与滴答定时器频率有关)
//__IO uint8_t Time_Flag   = 0;              // 任务时间标记

#define PLASTIC_STEP_SPEED     20 //过塑滚轮速度 单位是mm/s
#define ROLLER_STEP_SPEED     100 //追逐滚轮的速度 单位是mm/s
#define ROLLER_AND_PLASTIC_DIS    26300 //两个轮子之间的距离  单位0.001mm 26.3mm

extern __IO uint16_t Machine_Status;
extern OS_EVENT *CutFullSem; // 钢印机堆满信号量
extern int32_t PagingCaptureNumber;     // 输入捕获数
extern int16_t OverflowCountTwo;        //编码器计数溢出 计数器
extern int32_t CaptureNumber;
extern int16_t OverflowCount;

float S_plastic;
float S_target = 0; //单位0.001mm 
uint16_t S_waitTime;  //等待时间
uint16_t S_chaseTime; //追逐时间
uint8_t firstStatus;

__IO uint16_t machinePaperCount = 0; 

/*
  * 函数功能: 分页一张纸
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
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
				//	cerStepSrd[FRICTION_STEP].decel_start = UINT32_MAX;	//修改参数值,使得电机在运动过程中不会减速,这句指令必须在STEPMOTOR_AxisMoveRel函数之后指执行
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
				else if(cerMotionStatus[FRICTION_STEP] == 0){ //转了一转都没纸
					Name->PagingSeq = 8;
				}
				break;
				case 5:
			  break;
				case 6:
				STEPMOTOR_CerAxisMoveRel(FRICTION_STEP,12800,5000,5000,FRICTION_STEP_SPEED);
				cerStepSrd[FRICTION_STEP].decel_start = UINT32_MAX;	//修改参数值,使得电机在运动过程中不会减速,这句指令必须在STEPMOTOR_AxisMoveRel函数之后指执行				break;
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
  * 函数功能: 抽出纸张 
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
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
				if(Name->FirstPaperStatus == 0){//36 约等于 45mm/s
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
					STEPMOTOR_CerAxisMoveRel(PLASTIC_STEP,INT32_MAX,100, 100,35); //过塑轮20mm/s
					listStopCmd[PLASTIC_STEP] = False;
					
					}
						//纸张 SignPagingRollerOut 到加热片后轮的距离是86.3MM
					//(125mm + 86.3mm )/20mm = 10.565s
					Name->PagingHeatingCount = 10565;
					if(Name->FirstPaperStatus == 0){
						Name->FirstPaperStatus = 1;
						//cerStepSrd[ROLLER_STEP].medle_delay = (int32_t)(A_T_x10/ROLLER_SYNC_SPEED);//回归速度的定时器值
						//cerStepSrd[ROLLER_STEP].decel_start = 0;	//设置开始减速的位置
						//cerStepSrd[ROLLER_STEP].run_state = RUN;	//这句指令必须在STEPMOTOR_AxisMoveRel函数之后指执行					
						
						machinePaperCount ++;  //纸张加一 2020.4.11
						Name->RollerSeq = 6; //第一张
					}else{
					//	printf("计数值:%d\r\n",Name->DisTimeCount);
						/*
						第二张在第一张尾巴消失的时候，到达传感器的时间t1
						过塑轮的速度为恒定Vs = 20mm/s
						第二张与第一张的间距S= Vs * t1 = 20 * 0.4 = 8 mm;
						实际上是10mm左右
						*/
						S_plastic = PLASTIC_STEP_SPEED * Name->DisTimeCount;  //20*0.4 = 8  S_plastic 的单位是0.001 mm
						if(S_plastic < S_target){
						 cerStepSrd[ROLLER_STEP].run_state = STOP;	
						 S_waitTime = (S_target - S_plastic) / PLASTIC_STEP_SPEED; 
						 machinePaperCount ++;  //纸张加一 2020.4.11
						 Name->RollerSeq = 4;
						}
						else{//else if(S_plastic >= S_target){ //100mm/s 速度追逐							
						if ((S_plastic - S_target) >= ROLLER_AND_PLASTIC_DIS){//追不上 26300是机械结构决定
						//追逐时间Tmax = 26.3mm / 100mm/s = 0.263s = 263ms	
						//ROLLER_AND_PLASTIC_DIS / ROLLER_STEP_SPEED = 263ms
								//S_chaseTime = 263;
								 	S_chaseTime = 263; //2020.4.3
							 }else{//追得上
									S_chaseTime = (S_plastic - S_target) / ROLLER_STEP_SPEED; //100mm/s
							 }
							 machinePaperCount ++;  //纸张加一 2020.4.11
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
						cerStepSrd[ROLLER_STEP].medle_delay = (int32_t)(A_T_x10/ROLLER_SYNC_SPEED);//回归速度的定时器值
						cerStepSrd[ROLLER_STEP].decel_start = 0;	//设置开始减速的位置
						cerStepSrd[ROLLER_STEP].run_state = RUN;	//这句指令必须在STEPMOTOR_AxisMoveRel函数之后指执行								S_waitTime = 0;
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
	Name->PagingTryCount = 0; //参数清零
	
	Machine_Status &= ~NO_PAPER_TRUE; //有纸
	
	PagingMachine.Sequence   = 1;	
	controlHobMotor(&PagingMachine,0,3);
	
 controlRollerPaper(&PagingMachine);	
	
}

/*
  * 函数功能: 分页应用函数
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */
void runPagingMachine(PagingMotorns_Struct * Name)
{

	switch(Name->Sequence)
	{
		case 0:
			break;
		case 1:		
		if(SignPagingRollerOut.SIO == 0){//需要纸
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
			}else{//开始的时候有纸,在前面解决
				controlPagingConveyerMotor(Name,0,3);
				//controlRollerPaper(Name);
			}
		}else{//尝试5次判断为无纸
			Name->Sequence = 8;
		}
			break;
		case 6:
			if(readPagingPaper(Name) == 5){ //第二步
					if(!(Machine_Status & BEHIND_TRUE)){
						Name->Sequence = 7;
						Name->PagingTryCount = 0;  //有纸即可清零
					}
			}else if(readPagingPaper(Name) == 8){ //继续抽
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
						STEPMOTOR_CerAxisMoveRel(PLASTIC_STEP,INT32_MAX,100, 100,35); //过塑轮20mm/s
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
  * 函数功能: 手动穿膜控制函数
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */
void manualPlasticMem(PagingMotorns_Struct *Name)
{
		if(Name->Sequence != 0)return;
		while(cerMotionStatus[PLASTIC_STEP]);
		STEPMOTOR_CerAxisMoveRel(PLASTIC_STEP,215,10,10,50);//215 -- 2mm
}

/*
  * 函数功能: 连续穿膜控制函数
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */
void continuousPlasticMem(PagingMotorns_Struct *Name)
{
		if(Name->Sequence != 0)return;
		while(cerMotionStatus[PLASTIC_STEP]);
		STEPMOTOR_CerAxisMoveRel(PLASTIC_STEP,INT32_MAX,10,10,50); //215 -- 2mm
		Name->ManualStatus = 1;
}

/*
  * 函数功能:
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */
void stopPlasticMem(PagingMotorns_Struct *Name)
{
			if(Name->Sequence != 0)return;
			if(Name->ManualStatus == 1){
				Name->ManualStatus = 0;
			  listStopCmd[PLASTIC_STEP] = True;
			}
}
