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

extern OS_EVENT *StampFullSem; // 钢印机堆满信号量
extern int32_t PagingCaptureNumber;     // 输入捕获数
extern int16_t OverflowCountTwo;        //编码器计数溢出 计数器
extern int32_t CaptureNumber;
extern int16_t OverflowCount;

float S1;
float S_target = 0; //单位0.001mm 
uint16_t S_waitTime;  //等待时间
uint16_t S_chaseTime; //追逐时间
uint8_t firstStatus;
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
					STEPMOTOR_CerAxisMoveRel(FRICTION_STEP,12800,50,50,FRICTION_STEP_SPEED);
					cerStepSrd[FRICTION_STEP].decel_start = UINT32_MAX;	//修改参数值,使得电机在运动过程中不会减速,这句指令必须在STEPMOTOR_AxisMoveRel函数之后指执行
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
				break;
				case 5:
				//if(SignPagingFrictionOrigin.SIO == 1){
					//Name->PagingSeq = 1;
				  //cerStepSrd[FRICTION_STEP].run_state = STOP;
					//listStopCmd[FRICTION_STEP]  = True;
				//}
			  break;
				case 6:
				STEPMOTOR_CerAxisMoveRel(FRICTION_STEP,12800,50,50,FRICTION_STEP_SPEED);
				cerStepSrd[FRICTION_STEP].decel_start = UINT32_MAX;	//修改参数值,使得电机在运动过程中不会减速,这句指令必须在STEPMOTOR_AxisMoveRel函数之后指执行				break;
			  Name->PagingSeq = 7;
				break;
				case 7:
				if(SignPagingFrictionOrigin.SIO == 1){
					  cerStepSrd[FRICTION_STEP].run_state = STOP;
					}
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
		switch(Name->RollerSeq)
		{
			case 0:
				
				break;
			case 1: //6400*2 
				break;
			case 2:
				if(Name->FirstPaperStatus == 0){
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
				if(SignPagingRollerOut.SIO == 1){ //36 约等于 45mm/s
					if(Name->FirstPaperStatus == 0){
						Name->FirstPaperStatus = 1;
						//cerStepSrd[ROLLER_STEP].medle_delay = (int32_t)(A_T_x10/ROLLER_SYNC_SPEED);//回归速度的定时器值
						//cerStepSrd[ROLLER_STEP].decel_start = 0;	//设置开始减速的位置
						//cerStepSrd[ROLLER_STEP].run_state = RUN;	//这句指令必须在STEPMOTOR_AxisMoveRel函数之后指执行					
						Name->RollerSeq = 6; //第一张
					}else{
						printf("计数值:%d\r\n",Name->DisTimeCount);
						/*
						第二张在第一张尾巴消失的时候，到达传感器的时间t1
						过塑轮的速度为恒定Vs = 20mm/s
						第二张与第一张的间距S= Vs * t1 = 20 * 0.4 = 8 mm;
						实际上是10mm左右
						*/
						S1 = PLASTIC_STEP_SPEED * Name->DisTimeCount;  //20*0.4 = 8  S1 的单位是0.001 mm
						if(S1 < S_target){
						 cerStepSrd[ROLLER_STEP].run_state = STOP;	
						 S_waitTime = (S_target - S1) / PLASTIC_STEP_SPEED; 
						 Name->RollerSeq = 4;
						}
						else{//else if(S1 >= S_target){ //100mm/s 速度追逐							
						if ((S1 - S_target) >= ROLLER_AND_PLASTIC_DIS){//追不上 26300是机械结构决定
						//追逐时间Tmax = 26.3mm / 100mm/s = 0.263s = 263ms	
						//ROLLER_AND_PLASTIC_DIS / ROLLER_STEP_SPEED = 263ms
								//S_chaseTime = 263;
								 	S_chaseTime = 263; //2020.4.3
							 }else{//追得上
									S_chaseTime = (S1 - S_target) / ROLLER_STEP_SPEED; //100mm/s
							 }
							 Name->RollerSeq = 5;
						}
						printf("S1:%.2f\r\n",S1);
						printf("S_waitTime:%d\r\n",S_waitTime);
					  printf("S_chaseTime:%d\r\n",S_chaseTime);
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
}

static uint8_t readRollerPaper(PagingMotorns_Struct * Name)
{
	return Name->RollerSeq;
}

/*
  * 函数功能: 分页应用函数
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */
void runPagingMachine(PagingMotorns_Struct * Name)
{
//uint8_t err;
//	PagingCaptureNumber = OverflowCountTwo * 65535 + __HAL_TIM_GET_COUNTER(&htimx_EncoderTwo);
//	CaptureNumber = OverflowCount * 65535 + __HAL_TIM_GET_COUNTER(&htimx_Encoder);
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
		Name->Sequence = 4;
			break;
		case 4:
			if(SignPagingExistPaper.SIO == 1 && SignPagingExistPaper.SIOB == 1){
				Name->Sequence = 5;
			}
			break;
		case 5:
			if(SignPagingFrictionOutOne.SIO == 0){	
				controlPagingPaper(Name);
				controlPagingConveyerMotor(Name,0,3);
				Name->Sequence = 6;
			}else{//开始的时候有纸
				controlPagingConveyerMotor(Name,0,3);
				//controlRollerPaper(Name);
			//	Name->Sequence = 4; 
		//		Name->Sequence = 3;
			}
			break;
		case 6:
			if(readPagingPaper(Name) == 5){ //第二步
				if(SignPagingFrictionOutOne.SIO == 0){ //没纸继续抽
					Name->Sequence = 7;
				}else{ //有纸
					//controlPagingConveyerMotor(Name,0,0);
				  //controlPagingPaperStepTwo(Name);
					Name->Sequence = 7;
					Name->PaperCount ++;
				}
			}
			break;
		case 7:
		if((SignPagingFrictionOutTwo.SIO == 0) && (SignPagingFrictionOutThree.SIO == 0)
		&& (SignPagingFrictionOutFour.SIO == 0)){
			controlPagingPaperStepTwo(Name);
			Name->Sequence = 4;

		}
			break;
	}
/*
	switch(Name->Sequence)
	{
		case 0:

			break;
		case 1:
			if(SignPagingRollerOut.SIO == 0){//需要纸
				//controlPagingConveyerMotor(Name,0,3);
				Name->Sequence = 2;
			}else{
				
			}
			break;
		case 2:
			if((SignPagingFrictionOutTwo.SIO == 0) && (SignPagingFrictionOutThree.SIO == 0)
			&& (SignPagingFrictionOutFour.SIO == 0)){//需要纸
				controlPagingConveyerMotor(Name,0,3);
				Name->Sequence = 3;
			}else{
				//controlPagingConveyerMotor(Name,0,3);
			}
			break;
		case 3:
			if(firstStatus == 0 && SignPagingFrictionOutOne.SIO == 1){//刚开机有纸
				controlPagingPaperStepTwo(Name);
				Name->Sequence = 5;
			}else if(firstStatus == 0 && SignPagingFrictionOutOne.SIO == 0){//刚开机没纸
				controlPagingPaper(Name);
				Name->Sequence = 4;
			}else if(firstStatus == 1 && SignPagingFrictionOutOne.SIO == 1){//运行中有纸
				controlPagingPaperStepTwo(Name);
				Name->Sequence = 5;
			}else if(firstStatus == 1 && SignPagingFrictionOutOne.SIO == 0){//运行中没纸
				controlPagingPaper(Name);
				Name->Sequence = 4;
			}else{printf("error_2 waitting for you");}
			break;
		case 4: //FSignPagingFrictionOutOneSIO 没纸
		if(SignPagingExistPaper.SIO == 1 && SignPagingExistPaper.SIOB == 1){
				if(readPagingPaper(Name) == 5){
					if(SignPagingFrictionOutOne.SIO == 0){ //没纸继续抽
						 Name->Sequence = 3;
					}else{
						Name->Sequence = 5;
					}
				}else{
				
				}
			}
			break;
		case 5://SignPagingFrictionOutOneSIO 有纸
			Name->Sequence = 2; 
			break;
	}*/
		runPagingPaper(Name);
		runRollerPaper(Name);
		runHobMotor(Name);
		runPagingConveyerMotor(Name);
		signalStabilityT(&SignPagingExistPaper,sensorPagingExistPaper,2000);
		signalStability(&SignPagingFrictionOrigin,sensorPagingFrictionOrigin);
		
	//signalStability(&SignPagingFrictionOut,sensorPagingFrictionOut);
		
		signalStability(&SignPagingFrictionOutOne,sensorPagingFrictionOutOne);
		signalStability(&SignPagingFrictionOutTwo,sensorPagingFrictionOutTwo);
		signalStability(&SignPagingFrictionOutThree,sensorPagingFrictionOutThree);
		signalStability(&SignPagingFrictionOutFour,sensorPagingFrictionOutFour);
		signalStability(&SignPagingRollerOut,sensorPagingRollerOut);

}
