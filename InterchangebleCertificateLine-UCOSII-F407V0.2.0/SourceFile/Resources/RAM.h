#ifndef __RAM_H
#define __RAM_H
#include "stm32f4xx_hal.h"

typedef struct Status8bit_struct
{
        uint8_t bit0 :1;
        uint8_t bit1 :1;
        uint8_t bit2 :1;
        uint8_t bit3 :1;
        uint8_t bit4 :1;
        uint8_t bit5 :1;
        uint8_t bit6 :1;
        uint8_t bit7 :1;
}Status8bit;

typedef struct systic_struct
{
        uint16_t             _base;
        uint8_t             _1ms;
				uint8_t             _1msb;
				uint16_t            _1s;          
}systicStruct;

typedef struct UpdateUI_struct
{
        uint8_t UI1 :1;
        uint8_t UI2 :1;
        uint8_t UI3 :1;
        uint8_t UI4 :1;
        uint8_t UI5 :1;
        uint8_t UI6 :1;
        uint8_t UI7 :1;
        uint8_t UI8 :1;
}updateUIStruct;
typedef struct control_struct
{
        uint8_t mode;//手动，自动
        uint8_t time;
        uint8_t loop;
        uint8_t Motor;
        uint8_t State;
        uint8_t Page;
        uint16_t pageTime;
        updateUIStruct Update;
}controlStruct;
/*------------------------------Motor-------------------------------------*/
typedef struct Update_struct
{
        uint8_t M1 :1;
        uint8_t M2 :1;
        uint8_t M3 :1;
        uint8_t M4 :1;
        uint8_t M5 :1;
        uint8_t M6 :1;
        uint8_t M7 :1;
        uint8_t M8 :1;
}updateStruct;
typedef struct  External_control_struct
{
  uint8_t  New; 
  uint8_t  Number; 
	uint8_t  Direction;//
	uint8_t  Location;
	uint16_t Value;
  uint8_t  ZigBeeStatus;//
}External_control;

typedef struct  motor_Info_struct
{
        uint8_t State;
        uint8_t Value;
}motorInfoStruct;
typedef struct motor_struct
{
        updateStruct Update;
        motorInfoStruct M[6];
        motorInfoStruct S[6];
}motorStruct;

/*------------------stepmotor步进电机---------------------------*/
typedef struct _tStep_Status
{
        uint8_t error;
}Step_Status;
typedef struct _tStep_Control
{
  uint16_t Steps;
	uint8_t Speed; 
	uint8_t Speed2;
}Step_Control;
typedef struct  _tStep_Run
{
    	uint16_t Fault;
    	uint16_t Steps;
    	uint8_t Direction;
			uint8_t HL;
			uint8_t Speed;
	    uint8_t Speed2;
}Step_Run;

typedef struct _tStep_Struct
{
	Step_Status	Status;
	Step_Control	Control;   
	Step_Run	Run; 
	uint8_t		Step;
}Step_Struct;

/*------------------倒角刀电机---------------------------*/
typedef struct _tLeftChamferMotor_Status
{
        uint8_t Error;
        uint8_t Status;
        uint16_t Count;
}ChamferMotor_Status;

typedef struct _tLeftChamferMotor_Control
{
        uint16_t Time;
}ChamferMotor_Control;

typedef struct _tLeftChamferMotor_Run
{
        uint16_t Fault;
        uint16_t Time;
}ChamferMotor_Run;

typedef struct _tLeftChamferMotor_Struct
{
  ChamferMotor_Status  Status;
  ChamferMotor_Control Control;
  ChamferMotor_Run     Run;
}ChamferMotor_Struct;

/*------------------废料裁切电机---------------------------*/
typedef struct _tFlotasmCutMotor_Status
{
        uint8_t Error;
        uint8_t Status;
        uint16_t Count;
}FlotasmCutMotor_Status;

typedef struct _tFlotasmCutMotor_Control
{
        uint16_t Time;
}FlotasmCutMotor_Control;

typedef struct _tFlotasmCutMotor_Run
{
        uint16_t Fault;
        uint16_t Time;
}FlotasmCutMotor_Run;

typedef struct _tFlotasmCutMotor_Struct
{
  FlotasmCutMotor_Status  Status;
  FlotasmCutMotor_Control Control;
  FlotasmCutMotor_Run     Run;
}FlotasmCutMotor_Struct;

/*------------------钢印头---------------------------*/
typedef struct _tStampACMotor_Status
{
        uint8_t Error;
        uint8_t Status;
        uint16_t Count;
}StampACMotor_Status;

typedef struct _tStampACMotor_Control
{
        uint16_t Time;
}StampACMotor_Control;

typedef struct _tStampACMotor_Run
{
        uint16_t Fault;
        uint16_t Time;
}StampACMotor_Run;

typedef struct _tStampACMotor_Struct
{
  StampACMotor_Status  Status;
  StampACMotor_Control Control;
  StampACMotor_Run     Run;
}StampACMotor_Struct;

/*------------------张力推拉直流电机--------------------------*/
typedef struct _tStampPullMotor_Status
{
        uint8_t Error;
        uint8_t Status;
        uint16_t Count;
}StampPullMotor_Status;

typedef struct _tStampPullMotor_Control
{
        uint16_t Time;
}StampPullMotor_Control;

typedef struct _tStampPullMotor_Run
{
        uint16_t Fault;
        uint16_t Time;
}StampPullMotor_Run;

typedef struct _tStampPullMotor_Struct
{
  StampPullMotor_Status  Status;
  StampPullMotor_Control Control;
  StampPullMotor_Run     Run;
}StampPullMotor_Struct;

/*------------------分页机运输带直流电机--------------------------*/
typedef struct _tConveyerMotor_Status
{
        uint8_t Error;
        uint8_t Status;
        uint16_t Count;
}ConveyerMotor_Status;

typedef struct _tConveyerMotor_Control
{
        uint16_t Time;
}ConveyerMotor_Control;

typedef struct _tConveyerMotor_Run
{
        uint16_t Fault;
        uint16_t Time;
}ConveyerMotor_Run;

typedef struct _tConveyerMotor_Struct
{
  ConveyerMotor_Status  Status;
  ConveyerMotor_Control Control;
  ConveyerMotor_Run     Run;
}ConveyerMotor_Struct;


/*------------------分页机滚刀--------------------------*/
typedef struct _tHobMotor_Status
{
        uint8_t Error;
        uint8_t Status;
        uint16_t Count;
}HobMotor_Status;

typedef struct _tHobMotor_Control
{
        uint16_t Time;
}HobMotor_Control;

typedef struct _tHobMotor_Run
{
        uint16_t Fault;
        uint16_t Time;
}HobMotor_Run;

typedef struct _tHobMotor_Struct
{
  HobMotor_Status  Status;
  HobMotor_Control Control;
  HobMotor_Run     Run;
}HobMotor_Struct;

/*------------------Sign---------------------------*/
typedef struct _tSign
{
        uint8_t SIOB;
        uint16_t DetectV;
        
        uint8_t SIO;
        uint16_t Detect;  
}Sign;

typedef struct _tHalt_Struct
{
			uint8_t HaltSe;
			uint8_t HaltStatus;
	    uint8_t HaltAny;
}Halt_Struct;

typedef struct _tCount_Struct{
		 uint16_t WaitPaper;
}Count_Struct;

/*------------------Motor-All---------------------------*/
typedef struct _tMotorns_Struct
{
	ChamferMotor_Struct    ChamferMotor;
	FlotasmCutMotor_Struct     FlotasmCutMotor;
	Count_Struct               Count;
	uint8_t                    Sequence;
	uint8_t                    SelfResult;
	uint8_t                    ActionSeq;
	uint8_t                    WorkFinishSeq;
	uint8_t                    ManualStatus;
	uint8_t                    PressingStatus;
}CutMotorns_Struct;

typedef struct _tStampMotorns_Struct
{
	StampACMotor_Struct        StampACMotor;
	StampPullMotor_Struct      StampPullMotor;
	uint8_t                    Sequence;
	uint8_t                    StepSeq;
	uint8_t                    ManualStatus;
}StampMotorns_Struct;

typedef struct _tPagingMotorns_Struct
{
	ConveyerMotor_Struct        ConveyerMotor;
	HobMotor_Struct             HobMotor;
	uint8_t                    ManualStatus;
	uint8_t                     Sequence;
	uint8_t                     PagingSeq;
	uint8_t                     RollerSeq;
	uint8_t                     FirstPaperStatus;
	uint8_t                     PagingTryCount;
	uint16_t                    PagingHeatingCount;
	uint16_t                    DisTimeCount;
	uint16_t                    FirstPaperWaitCount;
	uint16_t                    CloseWaitCount;
}PagingMotorns_Struct;

/*------------------状态查询---------------------------*/
typedef struct _tInfo
{
		 uint8_t   InfoData[6];
}Info_Struct;

#endif
