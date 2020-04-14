#include "Configure.h"
#include "Resources.h"
#include "DCMotor/bsp_BDCMotor.h"
#include "StepMotor/bsp_STEPMOTOR.h" 
#include "Sensor/bsp_Sensor.h"
//#include "cpucrj.h"

/*-------------------------------<< 模块 >>-----------------------------------*/
/*
 * - :CRJ-
 * - :201700721.001.00.1
 * - System->APP->Drive->CPU，注意包含顺序
 * - 4类型函数，初始化配置，控制，状态，执行
 * - 
 * - :
 **/
/*___________________________________END______________________________________*/



/*--------------------------------推膜电机初始化----------------------------------------*/
//static void initPushMotor(CutMotorns_Struct * Name)
//{
//  Name->PushMotor.Status.Status = 0;

//  Name->PushMotor.Status.Error  = 0;

//  Name->PushMotor.Control.Time  = 0;

//  Name->PushMotor.Run.Fault     = 0;

//  Name->PushMotor.Run.Time      = 0;
//}

///*--------------------------------推膜电机控制----------------------------------------*/
//void controlPushMotor(CutMotorns_Struct * Name, uint16_t Time, uint8_t Function)
//{
//    if(Function == 5) return;
//    Name->PushMotor.Status.Status = Function;
//    if(Function == 0)
//    {
//      PushMotorIO(STOP,0);
//    }
//    if(Function == 4)
//    {
//      PushMotorIO(POSITIVE, Name->MotorFlash.flashSep.PushMotorDuty);
//      Name->PushMotor.Run.Time = Time;
//    }
//}

//uint8_t readPushMotor(CutMotorns_Struct * Name)
//{
//	return Name->PushMotor.Status.Status;
//}
///*--------------------------------推膜电机执行----------------------------------------*/
//void runPushMotor(CutMotorns_Struct * Name)
//{
//     if(Name->PushMotor.Status.Status == 4) 
//     {
//       if(Name->PushMotor.Run.Time > 0)
//           {
//           Name->PushMotor.Run.Time --;
//           }
//           else{ 
//             PushMotorIO(STOP,0);
//					 }
//					 }

//}
// /*--------------------------------初始化膜盒皮带电机----------------------------------------*/
//static void initMembraneMotor(CutMotorns_Struct * Name)
//{
//  Name->MembraneMotor.Status.Status = 0;
//  
//  Name->MembraneMotor.Status.Error  = 0;
//  
//  Name->MembraneMotor.Control.Time  = 0;
//  
//  Name->MembraneMotor.Run.Fault     = 0;
// 
//  Name->MembraneMotor.Run.Time      = 0;
//}

///*--------------------------------控制膜盒皮带电机----------------------------------------*/
//void controlMembraneMotor(CutMotorns_Struct *Name,uint16_t Time,uint8_t Function)//??	
//{
//    if(Function == 5) return;
//    Name->MembraneMotor.Status.Status = Function;
//    if(Function == 0)
//    {
//      MembraneMotorIO(STOP,0);
//    }
//    else if(Function == 1)
//    {
//      Name->MembraneMotor.Run.Time = Time;
//     MembraneMotorIO(POSITIVE,Name->MotorFlash.flashSep.MembraneMotorDuty);
//    }
//    else if(Function == 2)
//    {
//      MembraneMotorIO(POSITIVE,Name->MotorFlash.flashSep.MembraneMotorDuty);
////      Name->MembraneMotor.Run.Fault = 10000;
//    }
//    else if(Function == 3)
//    {
//      MembraneMotorIO(POSITIVE,Name->MotorFlash.flashSep.MembraneMotorDuty);
////      Name->MembraneMotor.Run.Fault = 10000;
//    }
//    else if(Function == 4)
//    {
//      Name->MembraneMotor.Run.Time = Time;
//      MembraneMotorIO(POSITIVE,Name->MotorFlash.flashSep.MembraneMotorDuty);
//    }
//    
//}

//uint8_t readMembraneMotor(CutMotorns_Struct *Name)
//{
//	return Name->MembraneMotor.Status.Status;
//}

///*--------------------------------膜盒皮带执行----------------------------------------*/
//void runMembraneMotor(CutMotorns_Struct *Name)
//{
//    if(Name->MembraneMotor.Status.Status == 0)
//    {
//    }
//    else if(Name->MembraneMotor.Status.Status == 3)
//    {
//          if(Name->MembraneMotor.Run.Fault > 0) 
//          Name->MembraneMotor.Run.Fault--;
//          else
//          {
////            controlMembraneMotor(Name,0,0);    
//           // Name->MachineErr.MembraneMotorTimeOut = 1;
//          }
//    }
//    else if(Name->MembraneMotor.Status.Status == 4)
//    {
//       if(Name->MembraneMotor.Run.Time > 0)Name->MembraneMotor.Run.Time --;
//       else MembraneMotorIO(STOP,0);
//    }
//}

/*--------------------------------舵机----------------------------------------*/
//void initializeServo(CutMotorns_Struct *Name)
//{
//	Name->Servo.Status.temp		= 0;
//	Name->Servo.Control.location	= 0;
//	Name->Servo.Run.count		= 0;
//	Name->Servo.Run.series		= 38u;
//	Name->Servo.Run.width		= 6u;//取值16 - 27
//}

//void controlServo(CutMotorns_Struct *Name,H8 Location)
//{
//    //6 7 8 9 0 1 2 3 4 5 6  012345
//    //7 8 9 0 1 2 3 4 5 6 7 bsssssm

//				Name->Servo.Control.location	= Location + 6;
//        Name->Servo.Control.FreeTime = 20000;
//        Name->Servo.Run.FreeTime    = 0;
//}

//void runSensor(CutMotorns_Struct *Name)
//{
//        if(Name->Servo.Run.FreeTime < Name->Servo.Control.FreeTime){
//           Name->Servo.Run.FreeTime ++;
//        }
//        else{
//         return;
//         }
//  	if(Name->Servo.Run.count > Name->Servo.Run.series) {
//          Name->Servo.Run.count = 0;
//          Name->Servo.Run.width = Name->Servo.Control.location;
//          servoMotorIO(TRUE);
//	} else {
//          Name->Servo.Run.count ++;
//        }
//	if(Name->Servo.Run.count == Name->Servo.Run.width){
//          servoMotorIO(FALSE);
//	}
//}
static void initChamferMotor(CutMotorns_Struct * Name)
{
  Name->ChamferMotor.Status.Status = 0;

  Name->ChamferMotor.Status.Error  = 0;

  Name->ChamferMotor.Control.Time  = 0;

  Name->ChamferMotor.Run.Fault     = 0;

  Name->ChamferMotor.Run.Time      = 0;
}

void controlChamferMotor(CutMotorns_Struct * Name, uint16_t Time, uint8_t Function)
{
	Name->ChamferMotor.Status.Status  = 2;
	Name->ChamferMotor.Run.Fault = 3000;
}

void runChamferMotor(CutMotorns_Struct * Name)
{
	switch(Name->ChamferMotor.Status.Status)
	{
		case 0:
			if(SignCutChamferMotor.SIO == 1){
				chamferMotorIO(STOP,0);
	  	Name->ChamferMotor.Status.Status  = 1;
			}
			break;
		case 1:
			break;
		case 2:
			chamferMotorIO(POSITIVE,99);
		Name->ChamferMotor.Status.Status = 3;
			break;
		case 3:
			if(SignCutChamferMotor.SIO == 0){
		Name->ChamferMotor.Status.Status  = 0;
			}
			break;
	}
		if(Name->ChamferMotor.Status.Status != 1){
		Name->ChamferMotor.Run.Fault --;
		if(Name->ChamferMotor.Run.Fault <= 0){
			Name->ChamferMotor.Status.Error = 1;
			chamferMotorIO(STOP,0);
		}
	}
}

uint8_t readChamferMotor(CutMotorns_Struct * Name)
{
	return Name->ChamferMotor.Status.Status;
}

/*-------------------------------废料裁切刀电机----------------------------------------*/
static void initFlotasmCutMotor(CutMotorns_Struct * Name)
{
  Name->FlotasmCutMotor.Status.Status = 0;

  Name->FlotasmCutMotor.Status.Error  = 0;

  Name->FlotasmCutMotor.Control.Time  = 0;

  Name->FlotasmCutMotor.Run.Fault     = 0;

  Name->FlotasmCutMotor.Run.Time      = 0;
}

void controlFlotasmCutMotor(CutMotorns_Struct * Name, uint16_t Time, uint8_t Function)
{
//    if(Function == 5) return;
//    Name->FlotasmCutMotor.Status.Status = Function;
//    if(Function == 0){
//      flotasmCutMotorIO(STOP,0);
//    }
//    else if(Function == 1){
//      Name->FlotasmCutMotor.Run.Time = Time;
//			flotasmCutMotorIO(REVERSE,90);
//    }
//		else if(Function == 2){
//			Name->FlotasmCutMotor.Run.Fault = 65535;
//		  flotasmCutMotorIO(REVERSE,90);
//		}
//		else if(Function == 3){
//			Name->FlotasmCutMotor.Run.Fault = 65535;
//		  flotasmCutMotorIO(POSITIVE,90);
//		}else if(Function == 4){
//			Name->FlotasmCutMotor.Run.Time = Time;
//			flotasmCutMotorIO(POSITIVE,90);
//		}
		Name->FlotasmCutMotor.Run.Fault = 10000;
		Name->FlotasmCutMotor.Status.Status = Function;
}

uint8_t readFlotasmCutMotor(CutMotorns_Struct * Name)
{
	return Name->FlotasmCutMotor.Status.Status;
}

void runFlotasmCutMotor(CutMotorns_Struct * Name)
{
  if(Name->FlotasmCutMotor.Run.Fault <= 0 && Name->FlotasmCutMotor.Status.Status != 0 ){
	  //flotasmCutMotorIO(STOP,0);
	  cerStepSrd[CUT_FLASM_STEP].run_state = STOP;
		Name->FlotasmCutMotor.Status.Error = 1;
	return;
	}
	
		switch(Name->FlotasmCutMotor.Status.Status)
		{
			case 0:
				break;
			case 1://完成
				break;
			case 2:
				if(SignCutLeftFlotasmCut.SIO == 1
						&& (SignCutRightFlotasmCut.SIO == 1)){
						Name->FlotasmCutMotor.Status.Error = 2;		
					}else if(SignCutLeftFlotasmCut.SIO == 1 ){//在左边
					//	flotasmCutMotorIO(REVERSE,90);
						STEPMOTOR_CerAxisMoveRel(CUT_FLASM_STEP,-6400*20,500,500,1000);
						HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);

						//cerStepSrd[CUT_FLASM_STEP].decel_start = UINT32_MAX;
						Name->FlotasmCutMotor.Status.Status = 3;
						//printf("3-1\r\n");
					}else if(SignCutRightFlotasmCut.SIO == 1 ){//在右边
						//flotasmCutMotorIO(POSITIVE,90);
						STEPMOTOR_CerAxisMoveRel(CUT_FLASM_STEP,6400*20,500,500,1000);
					//cerStepSrd[CUT_FLASM_STEP].decel_start = UINT32_MAX;
						HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);

						Name->FlotasmCutMotor.Status.Status = 4;
						//printf("3-2\r\n");
					}
					else{//不确定位置,先往右走
					   //flotasmCutMotorIO(REVERSE,90);
						STEPMOTOR_CerAxisMoveRel(CUT_FLASM_STEP,-6400*20,500,500,1000);
						//cerStepSrd[CUT_FLASM_STEP].decel_start = UINT32_MAX;
						HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_SET);
						Name->FlotasmCutMotor.Status.Status = 5;
						//printf("3-3\r\n");
					}
				break;
			case 3:
				if(SignCutRightFlotasmCut.SIO == 1){
					//flotasmCutMotorIO(STOP,0);
					cerStepSrd[CUT_FLASM_STEP].run_state = STOP;	
					Name->FlotasmCutMotor.Status.Status = 1;
				}
				Name->FlotasmCutMotor.Run.Fault --;
				break;
		  case 4:
				if(SignCutLeftFlotasmCut.SIO == 1){
					//flotasmCutMotorIO(STOP,0);
					cerStepSrd[CUT_FLASM_STEP].run_state = STOP;
					Name->FlotasmCutMotor.Status.Status = 1;
				}
				Name->FlotasmCutMotor.Run.Fault --;
				break;
			case 5:
				if(SignCutRightFlotasmCut.SIO == 1){
					//flotasmCutMotorIO(POSITIVE,90);
					//STEPMOTOR_CerAxisMoveRel(CUT_FLASM_STEP,INT32_MAX,200,200,1000);
					//cerStepSrd[CUT_FLASM_STEP].decel_start = UINT32_MAX;
					cerStepSrd[CUT_FLASM_STEP].run_state = STOP;
					Name->FlotasmCutMotor.Status.Status = 1;
				}
				break;
			case 6:
				if(cerMotionStatus[CUT_FLASM_STEP] == 0){
						STEPMOTOR_CerAxisMoveRel(CUT_FLASM_STEP,6400*20,500,500,1000);
					//cerStepSrd[CUT_FLASM_STEP].decel_start = UINT32_MAX;
						HAL_GPIO_WritePin(GPIOD,GPIO_PIN_2,GPIO_PIN_RESET);
					Name->FlotasmCutMotor.Status.Status = 7;
				}
				break;
			case 7:
				if(SignCutLeftFlotasmCut.SIO == 1){
					cerStepSrd[CUT_FLASM_STEP].run_state = STOP;
					Name->FlotasmCutMotor.Status.Status = 1;
				}
				break;
		}
		
}
static void initStampACMmtor(StampMotorns_Struct * Name)
{
  Name->StampACMotor.Status.Status = 0;

  Name->StampACMotor.Status.Error  = 0;

  Name->StampACMotor.Control.Time  = 0;

  Name->StampACMotor.Run.Fault     = 0;

  Name->StampACMotor.Run.Time      = 0;
}

/*-------------------------钢印头控制------------------------------*/
void controlStampACMotor(StampMotorns_Struct *Name,uint32_t Time,uint8_t Function)
{
		Name->StampACMotor.Run.Fault = Time;
		Name->StampACMotor.Status.Status = Function;
}

/*-------------------------钢印头应用------------------------------*/
void runStampACMotor(StampMotorns_Struct *Name)
{
  if(Name->StampACMotor.Run.Fault <= 0 && Name->StampACMotor.Status.Status != 0){
	  StampACMotorIO(STOP);
	  Name->StampACMotor.Status.Error = 1;
	return;
	}
			switch(Name->StampACMotor.Status.Status)
		{
				case 0:
					break;
				case 1://完成
					break;
				case 2:
					StampACMotorIO(POSITIVE);//开始
					Name->StampACMotor.Status.Status = 3;
					break;
				case 3:
					if(SignStampACMotor.SIO == 0) //正在动作
					Name->StampACMotor.Status.Status = 4;
					break;
				case 4:
					if(SignStampACMotor.SIO == 1){ //等待复位
					StampACMotorIO(STOP);
					Name->StampACMotor.Status.Status = 1;
					}
					break;
		}
}
/*-------------------------钢印头状态读取------------------------------*/
uint8_t readStampACMotor(StampMotorns_Struct *Name)
{
	return Name->StampACMotor.Status.Status;
}

/*-------------------------分页机运输带------------------------------*/
static void initPagingConveyerMotor(PagingMotorns_Struct * Name)
{
  Name->ConveyerMotor.Status.Status = 0;

  Name->ConveyerMotor.Status.Error  = 0;

  Name->ConveyerMotor.Control.Time  = 0;

  Name->ConveyerMotor.Run.Fault     = 0;

  Name->ConveyerMotor.Run.Time      = 0;
}

void controlPagingConveyerMotor(PagingMotorns_Struct * Name,uint16_t Time,uint8_t Function)
{
    if(Function == 5) return;
    Name->ConveyerMotor.Status.Status = Function;
    if(Function == 0){  //先用左倒角电机的输出，/2020.1.3
			pagingConveyerMotorIO(STOP,0);
    }
    else if(Function == 1){
      Name->ConveyerMotor.Run.Time = Time;
			pagingConveyerMotorIO(REVERSE,70);
    }
		else if(Function == 2){
			Name->ConveyerMotor.Run.Fault = 65535;
		  pagingConveyerMotorIO(REVERSE,70);
		}
		else if(Function == 3){
			Name->ConveyerMotor.Run.Fault = 65535;
		  pagingConveyerMotorIO(POSITIVE,70);
		}else if(Function == 4){
			Name->ConveyerMotor.Run.Time = Time;
			pagingConveyerMotorIO(POSITIVE,70);
		}
}

uint8_t readPagingConveyerMotor(PagingMotorns_Struct *Name)
{
	return Name->ConveyerMotor.Status.Status;
}

void runPagingConveyerMotor(PagingMotorns_Struct * Name)
{
	if(Name->ConveyerMotor.Status.Status == 0)return;
	
	else if(Name->ConveyerMotor.Status.Status == 1){
			 if(Name->ConveyerMotor.Run.Time > 0)
       {
         Name->ConveyerMotor.Run.Time --;
       }
       else {
				 pagingConveyerMotorIO(STOP,0);
			 }
	}
	else if(Name->ConveyerMotor.Status.Status == 2){
			 if(Name->ConveyerMotor.Run.Fault > 0){
					Name->ConveyerMotor.Run.Fault --;
			 }else{
					Name->ConveyerMotor.Status.Error = 2;
				// pagingConveyerMotorIO(STOP,0);
			 }
	}
	else if(Name->ConveyerMotor.Status.Status == 3){
			 if(Name->ConveyerMotor.Run.Fault > 0){
					Name->ConveyerMotor.Run.Fault --;
			 }else{
					Name->ConveyerMotor.Status.Error = 3;
				 // pagingConveyerMotorIO(STOP,0);
			 }

	}
	else if(Name->ConveyerMotor.Status.Status == 4){
			 if(Name->ConveyerMotor.Run.Time > 0)
       {
         Name->ConveyerMotor.Run.Time --;
       }
       else {
				 pagingConveyerMotorIO(STOP,0);
			 }
	}
}

/*-------------------------分页机滚刀------------------------------*/
static void initHobMotor(PagingMotorns_Struct * Name)
{
  Name->HobMotor.Status.Status = 0;

  Name->HobMotor.Status.Error  = 0;

  Name->HobMotor.Control.Time  = 0;

  Name->HobMotor.Run.Fault     = 0;

  Name->HobMotor.Run.Time      = 0;
}

void controlHobMotor(PagingMotorns_Struct * Name,uint16_t Time,uint8_t Function)
{
    if(Function == 5) return;
    Name->HobMotor.Status.Status = Function;
    if(Function == 0){  //先用左倒角电机的输出，/2020.1.3
			hobMotorIO(STOP,0);
    }
    else if(Function == 1){
      Name->HobMotor.Run.Time = Time;
			hobMotorIO(REVERSE,95);
    }
		else if(Function == 2){
			Name->HobMotor.Run.Fault = 65535;
		  hobMotorIO(REVERSE,95);
		}
		else if(Function == 3){
			Name->HobMotor.Run.Fault = 65535;
		  hobMotorIO(POSITIVE,95);
		}else if(Function == 4){
			Name->HobMotor.Run.Time = Time;
			hobMotorIO(POSITIVE,95);
		}
}

uint8_t readHobMotor(PagingMotorns_Struct *Name)
{
	return Name->HobMotor.Status.Status;
}

void runHobMotor(PagingMotorns_Struct * Name)
{
	if(Name->HobMotor.Status.Status == 0)return;
	
	else if(Name->HobMotor.Status.Status == 1){
			 if(Name->HobMotor.Run.Time > 0)
       {
         Name->HobMotor.Run.Time --;
       }
       else {
				 hobMotorIO(STOP,0);
			 }
	}
	else if(Name->HobMotor.Status.Status == 2){
			 if(Name->HobMotor.Run.Fault > 0){
					Name->HobMotor.Run.Fault --;
			 }else{
					Name->HobMotor.Status.Error = 2;
				// hobMotorIO(STOP,0);
			 }
	}
	else if(Name->HobMotor.Status.Status == 3){
			 if(Name->HobMotor.Run.Fault > 0){
					Name->HobMotor.Run.Fault --;
			 }else{
					Name->HobMotor.Status.Error = 3;
				 // hobMotorIO(STOP,0);
			 }

	}
	else if(Name->HobMotor.Status.Status == 4){
			 if(Name->HobMotor.Run.Time > 0)
       {
         Name->HobMotor.Run.Time --;
       }
       else {
				 hobMotorIO(STOP,0);
			 }
	}
}

void initCutAllMotor(CutMotorns_Struct * Name)
{

	initChamferMotor(Name);
	initFlotasmCutMotor(Name);
	
}

void initStampAllMotor(StampMotorns_Struct *Name)
{
	initStampACMmtor(Name);
}

void initPagingAllMotor(PagingMotorns_Struct *Name)
{
	initPagingConveyerMotor(Name);
	initHobMotor(Name);
}
