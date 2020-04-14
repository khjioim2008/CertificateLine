#ifndef __PID_H
#define	__PID_H

#include <stdint.h>
#include "stm32f4xx_hal.h"

typedef struct 
{
  __IO float SetPoint;    // 目标值  单位:mm/s
  __IO int LastError;     // 前一次误差    
  __IO int PrevError;     // 前两次误差
  __IO long SumError;     // 累计误差
  __IO double Proportion; // Kp系数
  __IO double Integral;   // Ki系数
  __IO double Derivative; // Kd系数
}PID;
extern float Exp_Val;     // PID计算出来的期望值
extern float Vel_Target;


extern float S_target; //乘上0.001mm 即是6mm

extern PID vPID;

extern float IncPIDCalc(int NextPoint,float TargetVal);

extern void Init_PIDStruct(void);

extern void controlPIDStepMotor(float vSetPoint, float sSetPoint);

extern void controlPIDPagingMachineStep(void);

extern void runPIDPagingMachineStep(void);

extern void clearEnconder(void);

extern void measuringSpeed(void);

#endif
