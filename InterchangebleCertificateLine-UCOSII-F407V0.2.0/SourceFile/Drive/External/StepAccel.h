#ifndef __StepAccel_H__
#define __StepAccel_H__

/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include <stm32f10x.h>
#include "ram.h"

/* ���Ͷ��� ------------------------------------------------------------------*/
typedef enum {False = 0, True = !False} bool;

enum // ����״̬
{
 	IDEL=0,       // ����״̬
 	FASTSEEK,     // ����Ѱ��״̬
	FASTSEEKSTOP, // ����Ѱ��ֹͣ״̬
	FASTSEEKBACK, // ��ʼ����Ѱ��״̬
	SLOWSEEK,     // ����Ѱ��
	SLOWSEEKSTOP, // ����Ѱ��ֹͣ״̬
	MOVETOZERO    // �ѻ���
};
enum {
	 	START=0,      
		FORW,
		BACK,
		ZERO
};
typedef struct {
  uint8_t run_state ;//! What part of the speed ramp we are in. ��ת״̬
  uint8_t dir ;//! Direction stepper motor should move. ��ת����
  int32_t step_delay;//! Peroid of next timer delay. At start this value set the accelration rate. 
  uint32_t decel_start;//! What step_pos to start decelaration ��������
  int32_t decel_val;//! Sets deceleration rate. ���ü�������
  int32_t min_delay;//! Minimum time delay (max speed)��С��ʱʱ��(��Ӧ����ٶ�)
  int32_t accel_count;//! Counter used when accelerateing/decelerateing to calculate step_delay.�Ӽ��ٽ׶μ���ֵ
}speedRampData;

/* �궨�� --------------------------------------------------------------------*/
//ֽ��
#define STEP_MOTOR_BOX_TIMX_CLOCKCMD           RCC_APB1PeriphClockCmd
#define STEP_MOTOR_BOX_TIMx_RCC_CLK            RCC_APB1Periph_TIM5
#define STEP_MOTOR_BOX_TIMx                    TIM5
#define STEP_MOTOR_BOX_TIMx_IRQn               TIM5_IRQn
#define STEP_MOTOR_BOX_TIMx_IRQHandler         TIM5_IRQHandler
#define STEP_MOTOR_BOX_TIMx_OCnInit            TIM_OC3Init
#define STEP_MOTOR_BOX_TIM_SetCompareN         TIM_SetCompare3

#define STEP_MOTOR_BOX_PULSE_CLK               RCC_APB2Periph_GPIOA
#define STEP_MOTOR_BOX_PULSE_1                 GPIO_Pin_2        // ֽ�������������
#define STEP_MOTOR_BOX_PULSEPORT                GPIOA

#define STEP_MOTOR_BOX_DIR_CLK                 RCC_APB2Periph_GPIOA
#define STEP_MOTOR_BOX_DIR_1                   GPIO_Pin_5        // �����������
#define STEP_MOTOR_BOX_DIRPORT                 GPIOA
#define STEP_MOTOR_BOX_SETDIR_CW()             GPIO_SetBits(STEP_MOTOR_BOX_DIRPORT, STEP_MOTOR_BOX_DIR_1)
#define STEP_MOTOR_BOX_SETDIR_CCW()            GPIO_ResetBits(STEP_MOTOR_BOX_DIRPORT, STEP_MOTOR_BOX_DIR_1)

//��Ĥ�������
#define STEP_MOTOR_TIMX_CLOCKCMD           RCC_APB1PeriphClockCmd
#define STEP_MOTOR_TIMx_RCC_CLK            RCC_APB1Periph_TIM2
#define STEP_MOTOR_TIMx                    TIM2
#define STEP_MOTOR_TIMx_IRQn               TIM2_IRQn
#define STEP_MOTOR_TIMx_IRQHandler         TIM2_IRQHandler
#define STEP_MOTOR_TIMx_OCnInit            TIM_OC2Init
#define STEP_MOTOR_TIM_SetCompareN         TIM_SetCompare2

#define STEP_MOTOR_PULSE_CLK               RCC_APB2Periph_GPIOA
#define STEP_MOTOR_PULSE_1                 GPIO_Pin_1        // ��Ĥ���������������
#define STEP_MOTOR_PULSEPORT               GPIOA

#define STEP_MOTOR_DIR_CLK                 RCC_APB2Periph_GPIOC
#define STEP_MOTOR_DIR_1                   GPIO_Pin_4        // �����������
#define STEP_MOTOR_DIRPORT                 GPIOC
#define STEP_MOTOR_SETDIR_CW()             GPIO_SetBits(STEP_MOTOR_DIRPORT, STEP_MOTOR_DIR_1)
#define STEP_MOTOR_SETDIR_CCW()            GPIO_ResetBits(STEP_MOTOR_DIRPORT, STEP_MOTOR_DIR_1)



/* �궨�� --------------------------------------------------------------------*/
#define TB6600_TIMx                        TIM2
#define TB6600_TIM_APBxClock_FUN           RCC_APB1PeriphClockCmd
#define TB6600_TIM_CLK                     RCC_APB1Periph_TIM2

#define TB6600_TIM_GPIO_APBxClock_FUN      RCC_APB1PeriphClockCmd
#define TB6600_TIM_GPIO_CLK                (RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO )
#define TB6600_TIM_CH1_PORT                GPIOA                 // ��ӦTB6600��PUL-��
#define TB6600_TIM_CH1_PIN                 GPIO_Pin_1            // ��PLU+ֱ�ӽӿ������+5v(����3.3V)

#define TB6600_DIR_GPIO_CLK                RCC_APB2Periph_GPIOC  // �����ת�������
#define TB6600_DIR_PORT                    GPIOC                 // ��ӦTB6600��DIR-��
#define TB6600_DIR_PIN                     GPIO_Pin_4           // ��DIR+ֱ�ӽӿ������+5v(����3.3V)
#define STEPMOTOR_DIR_FORWARD()            GPIO_SetBits(TB6600_DIR_PORT,TB6600_DIR_PIN)
#define STEPMOTOR_DIR_REVERSAL()           GPIO_ResetBits(TB6600_DIR_PORT,TB6600_DIR_PIN)

#define STEPMOTOR_OUTPUT_ENABLE()          GPIO_SetBits(TB6600_ENA_PORT,TB6600_ENA_PIN)
#define STEPMOTOR_OUTPUT_DISABLE()          GPIO_ResetBits(TB6600_ENA_PORT,TB6600_ENA_PIN)


// ���嶨ʱ��Ԥ��Ƶ����ʱ��ʵ��ʱ��Ƶ��Ϊ��72MHz/��STEPMOTOR_TIMx_PRESCALER+1��
#define STEPMOTOR_TIM_PRESCALER               3  // �������������ϸ������Ϊ��   32  ϸ��
//#define STEPMOTOR_TIM_PRESCALER               7  // �������������ϸ������Ϊ��   16  ϸ��
//#define STEPMOTOR_TIM_PRESCALER               15  // �������������ϸ������Ϊ��   8  ϸ��
//#define STEPMOTOR_TIM_PRESCALER               31  // �������������ϸ������Ϊ��   4  ϸ��
//#define STEPMOTOR_TIM_PRESCALER               63  // �������������ϸ������Ϊ��   2  ϸ��
//#define STEPMOTOR_TIM_PRESCALER               127  // �������������ϸ������Ϊ��   1  ϸ�֡

// ���嶨ʱ�����ڣ�����Ƚ�ģʽ��������Ϊ0xFFFF
#define STEPMOTOR_TIM_PERIOD                  0xFFFF

#define FALSE                                 0
#define TRUE                                  1
#define CW                                    0 // ˳ʱ��
#define CCW                                   1 // ��ʱ��

#define STOP                                  0 // �Ӽ�������״̬��ֹͣ
#define ACCEL                                 1 // �Ӽ�������״̬�����ٽ׶�
#define DECEL                                 2 // �Ӽ�������״̬�����ٽ׶�
#define RUN                                   3 // �Ӽ�������״̬�����ٽ׶�
//#define T1_FREQ                               (SystemCoreClock/(STEPMOTOR_TIM_PRESCALER+1)) // Ƶ��ftֵ
#define T1_FREQ                               10000
#define FSPR                                  200         //���������Ȧ����
#define MICRO_STEP                            32          // �������������ϸ����
#define SPR                                   (FSPR*MICRO_STEP)   // ��תһȦ��Ҫ��������

// ��ѧ����
#define ALPHA                                 ((float)(2*3.14159/SPR))       // ��= 2*pi/spr
#define A_T_x10                               ((float)(10*ALPHA*T1_FREQ))
#define T1_FREQ_148                           ((float)((T1_FREQ*0.676)/10)) // 0.676Ϊ�������ֵ
#define A_SQ                                  ((float)(2*100000*ALPHA)) 
#define A_x200                                ((float)(200*ALPHA))

/*---------------------�������� -------------------------------------------------------------*/
void StepMotor_TIMx_Init(void);
void StepMotorBoxAxisRel(int32_t step, uint32_t accel, uint32_t decel, uint32_t speed);
void StepMotorBoxAxisAbs(int32_t step, uint32_t accel, uint32_t decel, uint32_t speed);
void StepMotorAxisRel(int32_t step, uint32_t accel, uint32_t decel, uint32_t speed);
void StepMotorAxisAbs(int32_t step, uint32_t accel, uint32_t decel, uint32_t speed);
void StepMotorBox_TIMx_IRQHandler(void);
void StepMotor_TIMx_IRQHandler(void);
void StepMotorBoxReset(void);
void controlResetStepMotor(void);
void controlStepMotorBack(void);
void controlResetBoxStepMotor(void);
void controlBoxStepMotorBack(void);
void runResetStepMotorBox(void);
void runResetStepMotor(void);
u8 resetStepMotorAll(void);
u16 getstepBoxRunLength(void);
extern __IO uint8_t  stepBoxMotionStatus;           //ֽ���Ƿ����˶���0��ֹͣ��1���˶�
extern __IO uint8_t  stepMotorMotionStatus;  				//��Ĥ����
extern __IO int32_t  stepbox_position;        
extern __IO int32_t  stepmotor_position;
extern bool boxStopCmd;
#endif	
