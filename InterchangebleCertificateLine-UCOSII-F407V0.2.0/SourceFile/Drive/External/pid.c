#include "pid.h"
#include "StepMotor/bsp_STEPMOTOR.h" 
#include "EncoderTIM/bsp_EncoderTIM.h"

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
#define MAX_SPEED                   200 //最大速度
#define abs(x)    ((x)<0?(-x):(x))			//求绝对值

PID vPID;  //定义PID变量  vPID 速度
float Exp_Val;     // PID计算出来的期望值
float Vel_Target;

int16_t MSF = 0;                  // 电机反馈速度
int16_t SUM_Pulse;
int32_t CaptureNumber=0;     // 输入捕获数
int32_t Last_CaptureNumber = 0;// 上一次捕获值
int16_t SUM_Pulse;

int16_t MSF_2 = 0;                  // 电机反馈速度
float SUM_Pulse_2; 
int32_t PagingCaptureNumber = 0;     // 输入捕获数
int32_t Last_PagingCaptureNumber =0;// 上一次捕获值


uint8_t Motion_Dir = 0;           // 电机运动方向
extern int16_t OverflowCount;              //编码器计数溢出 计数器
extern int16_t OverflowCountTwo;              //编码器计数溢出 计数器

/**
  * 函数功能：增量式PID速度环计算
  * 输入参数：NextPoint     由编码器得到的速度值 
  *           TargetVal    目标值
  * 返 回 值：经过PID运算得到的增量值
  * 说    明：增量式 PID 速度环控制设计,计算得到的结果仍然是速度值
  */
float IncPIDCalc(int NextPoint,float TargetVal)       //临时变量,期望值 
{
  float iError = 0,iIncpid = 0;                       //当前误差
  iError = TargetVal - NextPoint;                     // 增量计算
  if((iError<0.5f)&&(iError>-0.5f))
    iError = 0;                                       // |e| < 0.5,不做调整
  iIncpid=(vPID.Proportion * iError)                  // E[k]项
              -(vPID.Integral * vPID.LastError)       // E[k-1]项
              +(vPID.Derivative * vPID.PrevError);    // E[k-2]项
  
  vPID.PrevError=vPID.LastError;                      // 存储误差，用于下次计算
  vPID.LastError = iError;
  return(iIncpid);                                    // 返回增量值
	
}

/**
  * 函数功能：PID参数初始化
  * 输入参数：无
  * 返 回 值：无
  * 说    明：无
*/
void Init_PIDStruct(void)
{
	Vel_Target = (vPID.SetPoint * P_PERIOD);//每单位采样周期内的脉冲数(频率)
  vPID.SetPoint   = 45;       // 目标值  单位:mm/s
  vPID.Proportion = 0.11f;    // Kp系数
  vPID.Derivative = 0.03f;    // Ki系数
  vPID.Integral   = 0.12f;    // Kd系数
  vPID.LastError  = 0;
  vPID.PrevError  = 0;
  vPID.SumError   = 0; 
	

  /* 目标值单位为:mm/s,这里需要转换为频率
     f = v*PPM;
     PPM是每mm的编码器脉冲数,得到的F就是每秒的脉冲数 (Pulse/s,也就是频率:Hz)
     Vel_Target = f/(1000/20);得到每个采样周期(20ms)的脉冲数(Pulse/ms)
   */
  
}


void clearEnconder(void)
{
		__HAL_TIM_SET_COMPARE(&htimx_Encoder,TIM_CHANNEL_1,0);
		__HAL_TIM_SET_COMPARE(&htimx_Encoder,TIM_CHANNEL_2,0);
		__HAL_TIM_SET_COUNTER(&htimx_Encoder,0);
		OverflowCount = 0;
		CaptureNumber = 0;
		Last_CaptureNumber = 0;
		SUM_Pulse = 0;
		MSF = 0;
}

float readStepMotorPostionNow(void)
{
	float PostionStep = 0;
	PostionStep = (OverflowCount * 65535 + __HAL_TIM_GET_COUNTER(&htimx_Encoder)) * MPP;
  
	return PostionStep;
}
/**
  * 函数功能：步进电机PID控制
  * 输入参数：无
  * 返 回 值：无
  * 说    明：无
*/

void controlPIDStepMotor(float vSetPoint, float sSetPoint)
{

//	__HAL_TIM_SET_COMPARE(&htimx_Encoder,TIM_CHANNEL_1,0);
//	__HAL_TIM_SET_COMPARE(&htimx_Encoder,TIM_CHANNEL_2,0);
//	__HAL_TIM_SET_COUNTER(&htimx_Encoder,0);
//	OverflowCount = 0;
//  CaptureNumber = 0;
//	Last_CaptureNumber = 0;
//	SUM_Pulse = 0;
//	MSF = 0;
}

void controlPIDPagingMachineStep(void)
{
	Exp_Val = 0;
	Init_PIDStruct();
	HAL_TIM_OC_Start_IT(&cutMachine_STEPMOTOR,TIM_CHANNEL_1);
}

/**
  * 函数功能：PID执行函数
  * 输入参数：无
  * 返 回 值：无
  * 说    明：无
*/

void runPIDPagingMachineStep(void)
{
      //获得编码器的脉冲值
      CaptureNumber = OverflowCount * 65535 + __HAL_TIM_GET_COUNTER(&htimx_Encoder);

      //M法 测速度
      MSF = CaptureNumber  - Last_CaptureNumber;
      Last_CaptureNumber = CaptureNumber;
      MSF = abs(MSF);
      //对速度进行累计,得到1s内的脉冲数
      SUM_Pulse += MSF;
      Exp_Val += IncPIDCalc(MSF,Vel_Target);
      Exp_Val = abs(Exp_Val);
      /* 经过PID计算得到的结果是编码器的输出期望值的增量,
         需要转换为步进电机的控制量(频率值),这里乘上一个系数6400/2400
      */
      STEPMOTOR_Motion_Ctrl(CW,Exp_Val*FEEDBACK_CONST);//乘上一个系数,6400/2400,将PID计算结果转换为步进电机的频率(速度)
}

void measuringSpeed(void)
{
	float SUM_PulseSpeed;
	CaptureNumber = OverflowCount * 65535 + __HAL_TIM_GET_COUNTER(&htimx_Encoder);

	//M法 测速度
	MSF = CaptureNumber  - Last_CaptureNumber;
	Last_CaptureNumber = CaptureNumber;
	MSF = abs(MSF);
	//对速度进行累计,得到1s内的脉冲数
	SUM_Pulse += MSF;
	
	PagingCaptureNumber = OverflowCountTwo * 65535 + __HAL_TIM_GET_COUNTER(&htimx_EncoderTwo);
  MSF_2 = PagingCaptureNumber - Last_PagingCaptureNumber;
	Last_PagingCaptureNumber = PagingCaptureNumber;
	MSF_2 = abs(MSF_2);
//对速度进行累计,得到1s内的脉冲数
	SUM_Pulse_2 =  MSF_2;

	printf("废料刀滚轮捕获值:%d\r\n",CaptureNumber);
  printf("SUM_Pulse:%d\r\n",SUM_Pulse);
	SUM_PulseSpeed = MPP * SUM_Pulse;
	printf("废料刀滚轮速度:%d mm/s\r\n", (int)SUM_PulseSpeed);
	//printf("倒角刀滚轮捕获值:%d\r\n",PagingCaptureNumber);
//  printf("前分页滚轮速度:%.2f mm/s\r\n",SUM_Pulse_2 * MPP);
	
	SUM_Pulse = 0;
	SUM_Pulse_2 = 0;
}
