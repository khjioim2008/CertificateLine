#ifndef __PID_H
#define	__PID_H

#include <stdint.h>
#include "stm32f4xx_hal.h"

typedef struct 
{
  __IO float SetPoint;    // Ŀ��ֵ  ��λ:mm/s
  __IO int LastError;     // ǰһ�����    
  __IO int PrevError;     // ǰ�������
  __IO long SumError;     // �ۼ����
  __IO double Proportion; // Kpϵ��
  __IO double Integral;   // Kiϵ��
  __IO double Derivative; // Kdϵ��
}PID;
extern float Exp_Val;     // PID�������������ֵ
extern float Vel_Target;


extern float S_target; //����0.001mm ����6mm

extern PID vPID;

extern float IncPIDCalc(int NextPoint,float TargetVal);

extern void Init_PIDStruct(void);

extern void controlPIDStepMotor(float vSetPoint, float sSetPoint);

extern void controlPIDPagingMachineStep(void);

extern void runPIDPagingMachineStep(void);

extern void clearEnconder(void);

extern void measuringSpeed(void);

#endif
