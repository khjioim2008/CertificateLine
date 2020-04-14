#include "pid.h"
#include "StepMotor/bsp_STEPMOTOR.h" 
#include "EncoderTIM/bsp_EncoderTIM.h"

/*-------------------------------<< ģ�� >>-----------------------------------*/
/*
 * - :CRJ-
 * - :201700721.001.00.1
 * - System->APP->Drive->CPU��ע�����˳��
 * - 4���ͺ�������ʼ�����ã����ƣ�״̬��ִ��
 * - 
 * - :
 **/
/*___________________________________END______________________________________*/
#define MAX_SPEED                   200 //����ٶ�
#define abs(x)    ((x)<0?(-x):(x))			//�����ֵ

PID vPID;  //����PID����  vPID �ٶ�
float Exp_Val;     // PID�������������ֵ
float Vel_Target;

int16_t MSF = 0;                  // ��������ٶ�
int16_t SUM_Pulse;
int32_t CaptureNumber=0;     // ���벶����
int32_t Last_CaptureNumber = 0;// ��һ�β���ֵ
int16_t SUM_Pulse;

int16_t MSF_2 = 0;                  // ��������ٶ�
float SUM_Pulse_2; 
int32_t PagingCaptureNumber = 0;     // ���벶����
int32_t Last_PagingCaptureNumber =0;// ��һ�β���ֵ


uint8_t Motion_Dir = 0;           // ����˶�����
extern int16_t OverflowCount;              //������������� ������
extern int16_t OverflowCountTwo;              //������������� ������

/**
  * �������ܣ�����ʽPID�ٶȻ�����
  * ���������NextPoint     �ɱ������õ����ٶ�ֵ 
  *           TargetVal    Ŀ��ֵ
  * �� �� ֵ������PID����õ�������ֵ
  * ˵    ��������ʽ PID �ٶȻ��������,����õ��Ľ����Ȼ���ٶ�ֵ
  */
float IncPIDCalc(int NextPoint,float TargetVal)       //��ʱ����,����ֵ 
{
  float iError = 0,iIncpid = 0;                       //��ǰ���
  iError = TargetVal - NextPoint;                     // ��������
  if((iError<0.5f)&&(iError>-0.5f))
    iError = 0;                                       // |e| < 0.5,��������
  iIncpid=(vPID.Proportion * iError)                  // E[k]��
              -(vPID.Integral * vPID.LastError)       // E[k-1]��
              +(vPID.Derivative * vPID.PrevError);    // E[k-2]��
  
  vPID.PrevError=vPID.LastError;                      // �洢�������´μ���
  vPID.LastError = iError;
  return(iIncpid);                                    // ��������ֵ
	
}

/**
  * �������ܣ�PID������ʼ��
  * �����������
  * �� �� ֵ����
  * ˵    ������
*/
void Init_PIDStruct(void)
{
	Vel_Target = (vPID.SetPoint * P_PERIOD);//ÿ��λ���������ڵ�������(Ƶ��)
  vPID.SetPoint   = 45;       // Ŀ��ֵ  ��λ:mm/s
  vPID.Proportion = 0.11f;    // Kpϵ��
  vPID.Derivative = 0.03f;    // Kiϵ��
  vPID.Integral   = 0.12f;    // Kdϵ��
  vPID.LastError  = 0;
  vPID.PrevError  = 0;
  vPID.SumError   = 0; 
	

  /* Ŀ��ֵ��λΪ:mm/s,������Ҫת��ΪƵ��
     f = v*PPM;
     PPM��ÿmm�ı�����������,�õ���F����ÿ��������� (Pulse/s,Ҳ����Ƶ��:Hz)
     Vel_Target = f/(1000/20);�õ�ÿ����������(20ms)��������(Pulse/ms)
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
  * �������ܣ��������PID����
  * �����������
  * �� �� ֵ����
  * ˵    ������
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
  * �������ܣ�PIDִ�к���
  * �����������
  * �� �� ֵ����
  * ˵    ������
*/

void runPIDPagingMachineStep(void)
{
      //��ñ�����������ֵ
      CaptureNumber = OverflowCount * 65535 + __HAL_TIM_GET_COUNTER(&htimx_Encoder);

      //M�� ���ٶ�
      MSF = CaptureNumber  - Last_CaptureNumber;
      Last_CaptureNumber = CaptureNumber;
      MSF = abs(MSF);
      //���ٶȽ����ۼ�,�õ�1s�ڵ�������
      SUM_Pulse += MSF;
      Exp_Val += IncPIDCalc(MSF,Vel_Target);
      Exp_Val = abs(Exp_Val);
      /* ����PID����õ��Ľ���Ǳ��������������ֵ������,
         ��Ҫת��Ϊ��������Ŀ�����(Ƶ��ֵ),�������һ��ϵ��6400/2400
      */
      STEPMOTOR_Motion_Ctrl(CW,Exp_Val*FEEDBACK_CONST);//����һ��ϵ��,6400/2400,��PID������ת��Ϊ���������Ƶ��(�ٶ�)
}

void measuringSpeed(void)
{
	float SUM_PulseSpeed;
	CaptureNumber = OverflowCount * 65535 + __HAL_TIM_GET_COUNTER(&htimx_Encoder);

	//M�� ���ٶ�
	MSF = CaptureNumber  - Last_CaptureNumber;
	Last_CaptureNumber = CaptureNumber;
	MSF = abs(MSF);
	//���ٶȽ����ۼ�,�õ�1s�ڵ�������
	SUM_Pulse += MSF;
	
	PagingCaptureNumber = OverflowCountTwo * 65535 + __HAL_TIM_GET_COUNTER(&htimx_EncoderTwo);
  MSF_2 = PagingCaptureNumber - Last_PagingCaptureNumber;
	Last_PagingCaptureNumber = PagingCaptureNumber;
	MSF_2 = abs(MSF_2);
//���ٶȽ����ۼ�,�õ�1s�ڵ�������
	SUM_Pulse_2 =  MSF_2;

	printf("���ϵ����ֲ���ֵ:%d\r\n",CaptureNumber);
  printf("SUM_Pulse:%d\r\n",SUM_Pulse);
	SUM_PulseSpeed = MPP * SUM_Pulse;
	printf("���ϵ������ٶ�:%d mm/s\r\n", (int)SUM_PulseSpeed);
	//printf("���ǵ����ֲ���ֵ:%d\r\n",PagingCaptureNumber);
//  printf("ǰ��ҳ�����ٶ�:%.2f mm/s\r\n",SUM_Pulse_2 * MPP);
	
	SUM_Pulse = 0;
	SUM_Pulse_2 = 0;
}
