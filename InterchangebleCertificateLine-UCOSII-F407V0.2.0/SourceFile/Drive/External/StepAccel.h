#ifndef __StepAccel_H__
#define __StepAccel_H__

/* 包含头文件 ----------------------------------------------------------------*/
#include <stm32f10x.h>
#include "ram.h"

/* 类型定义 ------------------------------------------------------------------*/
typedef enum {False = 0, True = !False} bool;

enum // 回零状态
{
 	IDEL=0,       // 空闲状态
 	FASTSEEK,     // 快速寻找状态
	FASTSEEKSTOP, // 快速寻找停止状态
	FASTSEEKBACK, // 开始返回寻找状态
	SLOWSEEK,     // 慢速寻找
	SLOWSEEKSTOP, // 慢速寻找停止状态
	MOVETOZERO    // 已回零
};
enum {
	 	START=0,      
		FORW,
		BACK,
		ZERO
};
typedef struct {
  uint8_t run_state ;//! What part of the speed ramp we are in. 旋转状态
  uint8_t dir ;//! Direction stepper motor should move. 旋转方向
  int32_t step_delay;//! Peroid of next timer delay. At start this value set the accelration rate. 
  uint32_t decel_start;//! What step_pos to start decelaration 减速启动
  int32_t decel_val;//! Sets deceleration rate. 设置减速速率
  int32_t min_delay;//! Minimum time delay (max speed)最小延时时间(对应最大速度)
  int32_t accel_count;//! Counter used when accelerateing/decelerateing to calculate step_delay.加减速阶段计数值
}speedRampData;

/* 宏定义 --------------------------------------------------------------------*/
//纸盒
#define STEP_MOTOR_BOX_TIMX_CLOCKCMD           RCC_APB1PeriphClockCmd
#define STEP_MOTOR_BOX_TIMx_RCC_CLK            RCC_APB1Periph_TIM5
#define STEP_MOTOR_BOX_TIMx                    TIM5
#define STEP_MOTOR_BOX_TIMx_IRQn               TIM5_IRQn
#define STEP_MOTOR_BOX_TIMx_IRQHandler         TIM5_IRQHandler
#define STEP_MOTOR_BOX_TIMx_OCnInit            TIM_OC3Init
#define STEP_MOTOR_BOX_TIM_SetCompareN         TIM_SetCompare3

#define STEP_MOTOR_BOX_PULSE_CLK               RCC_APB2Periph_GPIOA
#define STEP_MOTOR_BOX_PULSE_1                 GPIO_Pin_2        // 纸盒脉冲输出引脚
#define STEP_MOTOR_BOX_PULSEPORT                GPIOA

#define STEP_MOTOR_BOX_DIR_CLK                 RCC_APB2Periph_GPIOA
#define STEP_MOTOR_BOX_DIR_1                   GPIO_Pin_5        // 方向控制引脚
#define STEP_MOTOR_BOX_DIRPORT                 GPIOA
#define STEP_MOTOR_BOX_SETDIR_CW()             GPIO_SetBits(STEP_MOTOR_BOX_DIRPORT, STEP_MOTOR_BOX_DIR_1)
#define STEP_MOTOR_BOX_SETDIR_CCW()            GPIO_ResetBits(STEP_MOTOR_BOX_DIRPORT, STEP_MOTOR_BOX_DIR_1)

//开膜步进电机
#define STEP_MOTOR_TIMX_CLOCKCMD           RCC_APB1PeriphClockCmd
#define STEP_MOTOR_TIMx_RCC_CLK            RCC_APB1Periph_TIM2
#define STEP_MOTOR_TIMx                    TIM2
#define STEP_MOTOR_TIMx_IRQn               TIM2_IRQn
#define STEP_MOTOR_TIMx_IRQHandler         TIM2_IRQHandler
#define STEP_MOTOR_TIMx_OCnInit            TIM_OC2Init
#define STEP_MOTOR_TIM_SetCompareN         TIM_SetCompare2

#define STEP_MOTOR_PULSE_CLK               RCC_APB2Periph_GPIOA
#define STEP_MOTOR_PULSE_1                 GPIO_Pin_1        // 开膜步进脉冲输出引脚
#define STEP_MOTOR_PULSEPORT               GPIOA

#define STEP_MOTOR_DIR_CLK                 RCC_APB2Periph_GPIOC
#define STEP_MOTOR_DIR_1                   GPIO_Pin_4        // 方向控制引脚
#define STEP_MOTOR_DIRPORT                 GPIOC
#define STEP_MOTOR_SETDIR_CW()             GPIO_SetBits(STEP_MOTOR_DIRPORT, STEP_MOTOR_DIR_1)
#define STEP_MOTOR_SETDIR_CCW()            GPIO_ResetBits(STEP_MOTOR_DIRPORT, STEP_MOTOR_DIR_1)



/* 宏定义 --------------------------------------------------------------------*/
#define TB6600_TIMx                        TIM2
#define TB6600_TIM_APBxClock_FUN           RCC_APB1PeriphClockCmd
#define TB6600_TIM_CLK                     RCC_APB1Periph_TIM2

#define TB6600_TIM_GPIO_APBxClock_FUN      RCC_APB1PeriphClockCmd
#define TB6600_TIM_GPIO_CLK                (RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO )
#define TB6600_TIM_CH1_PORT                GPIOA                 // 对应TB6600的PUL-，
#define TB6600_TIM_CH1_PIN                 GPIO_Pin_1            // 而PLU+直接接开发板的+5v(或者3.3V)

#define TB6600_DIR_GPIO_CLK                RCC_APB2Periph_GPIOC  // 电机旋转方向控制
#define TB6600_DIR_PORT                    GPIOC                 // 对应TB6600的DIR-，
#define TB6600_DIR_PIN                     GPIO_Pin_4           // 而DIR+直接接开发板的+5v(或者3.3V)
#define STEPMOTOR_DIR_FORWARD()            GPIO_SetBits(TB6600_DIR_PORT,TB6600_DIR_PIN)
#define STEPMOTOR_DIR_REVERSAL()           GPIO_ResetBits(TB6600_DIR_PORT,TB6600_DIR_PIN)

#define STEPMOTOR_OUTPUT_ENABLE()          GPIO_SetBits(TB6600_ENA_PORT,TB6600_ENA_PIN)
#define STEPMOTOR_OUTPUT_DISABLE()          GPIO_ResetBits(TB6600_ENA_PORT,TB6600_ENA_PIN)


// 定义定时器预分频，定时器实际时钟频率为：72MHz/（STEPMOTOR_TIMx_PRESCALER+1）
#define STEPMOTOR_TIM_PRESCALER               3  // 步进电机驱动器细分设置为：   32  细分
//#define STEPMOTOR_TIM_PRESCALER               7  // 步进电机驱动器细分设置为：   16  细分
//#define STEPMOTOR_TIM_PRESCALER               15  // 步进电机驱动器细分设置为：   8  细分
//#define STEPMOTOR_TIM_PRESCALER               31  // 步进电机驱动器细分设置为：   4  细分
//#define STEPMOTOR_TIM_PRESCALER               63  // 步进电机驱动器细分设置为：   2  细分
//#define STEPMOTOR_TIM_PRESCALER               127  // 步进电机驱动器细分设置为：   1  细分�

// 定义定时器周期，输出比较模式周期设置为0xFFFF
#define STEPMOTOR_TIM_PERIOD                  0xFFFF

#define FALSE                                 0
#define TRUE                                  1
#define CW                                    0 // 顺时针
#define CCW                                   1 // 逆时针

#define STOP                                  0 // 加减速曲线状态：停止
#define ACCEL                                 1 // 加减速曲线状态：加速阶段
#define DECEL                                 2 // 加减速曲线状态：减速阶段
#define RUN                                   3 // 加减速曲线状态：匀速阶段
//#define T1_FREQ                               (SystemCoreClock/(STEPMOTOR_TIM_PRESCALER+1)) // 频率ft值
#define T1_FREQ                               10000
#define FSPR                                  200         //步进电机单圈步数
#define MICRO_STEP                            32          // 步进电机驱动器细分数
#define SPR                                   (FSPR*MICRO_STEP)   // 旋转一圈需要的脉冲数

// 数学常数
#define ALPHA                                 ((float)(2*3.14159/SPR))       // α= 2*pi/spr
#define A_T_x10                               ((float)(10*ALPHA*T1_FREQ))
#define T1_FREQ_148                           ((float)((T1_FREQ*0.676)/10)) // 0.676为误差修正值
#define A_SQ                                  ((float)(2*100000*ALPHA)) 
#define A_x200                                ((float)(200*ALPHA))

/*---------------------函数声明 -------------------------------------------------------------*/
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
extern __IO uint8_t  stepBoxMotionStatus;           //纸盒是否在运动？0：停止，1：运动
extern __IO uint8_t  stepMotorMotionStatus;  				//开膜步进
extern __IO int32_t  stepbox_position;        
extern __IO int32_t  stepmotor_position;
extern bool boxStopCmd;
#endif	
