#ifndef __StepAccel_H__
#define __StepAccel_H__

/* °üº¬Í·ÎÄ¼þ ----------------------------------------------------------------*/
#include <stm32f10x.h>
#include "ram.h"

/* ÀàÐÍ¶¨Òå ------------------------------------------------------------------*/
typedef enum {False = 0, True = !False} bool;

enum // »ØÁã×´Ì¬
{
 	IDEL=0,       // ¿ÕÏÐ×´Ì¬
 	FASTSEEK,     // ¿ìËÙÑ°ÕÒ×´Ì¬
	FASTSEEKSTOP, // ¿ìËÙÑ°ÕÒÍ£Ö¹×´Ì¬
	FASTSEEKBACK, // ¿ªÊ¼·µ»ØÑ°ÕÒ×´Ì¬
	SLOWSEEK,     // ÂýËÙÑ°ÕÒ
	SLOWSEEKSTOP, // ÂýËÙÑ°ÕÒÍ£Ö¹×´Ì¬
	MOVETOZERO    // ÒÑ»ØÁã
};
enum {
	 	START=0,      
		FORW,
		BACK,
		ZERO
};
typedef struct {
  uint8_t run_state ;//! What part of the speed ramp we are in. Ðý×ª×´Ì¬
  uint8_t dir ;//! Direction stepper motor should move. Ðý×ª·½Ïò
  int32_t step_delay;//! Peroid of next timer delay. At start this value set the accelration rate. 
  uint32_t decel_start;//! What step_pos to start decelaration ¼õËÙÆô¶¯
  int32_t decel_val;//! Sets deceleration rate. ÉèÖÃ¼õËÙËÙÂÊ
  int32_t min_delay;//! Minimum time delay (max speed)×îÐ¡ÑÓÊ±Ê±¼ä(¶ÔÓ¦×î´óËÙ¶È)
  int32_t accel_count;//! Counter used when accelerateing/decelerateing to calculate step_delay.¼Ó¼õËÙ½×¶Î¼ÆÊýÖµ
}speedRampData;

/* ºê¶¨Òå --------------------------------------------------------------------*/
//Ö½ºÐ
#define STEP_MOTOR_BOX_TIMX_CLOCKCMD           RCC_APB1PeriphClockCmd
#define STEP_MOTOR_BOX_TIMx_RCC_CLK            RCC_APB1Periph_TIM5
#define STEP_MOTOR_BOX_TIMx                    TIM5
#define STEP_MOTOR_BOX_TIMx_IRQn               TIM5_IRQn
#define STEP_MOTOR_BOX_TIMx_IRQHandler         TIM5_IRQHandler
#define STEP_MOTOR_BOX_TIMx_OCnInit            TIM_OC3Init
#define STEP_MOTOR_BOX_TIM_SetCompareN         TIM_SetCompare3

#define STEP_MOTOR_BOX_PULSE_CLK               RCC_APB2Periph_GPIOA
#define STEP_MOTOR_BOX_PULSE_1                 GPIO_Pin_2        // Ö½ºÐÂö³åÊä³öÒý½Å
#define STEP_MOTOR_BOX_PULSEPORT                GPIOA

#define STEP_MOTOR_BOX_DIR_CLK                 RCC_APB2Periph_GPIOA
#define STEP_MOTOR_BOX_DIR_1                   GPIO_Pin_5        // ·½Ïò¿ØÖÆÒý½Å
#define STEP_MOTOR_BOX_DIRPORT                 GPIOA
#define STEP_MOTOR_BOX_SETDIR_CW()             GPIO_SetBits(STEP_MOTOR_BOX_DIRPORT, STEP_MOTOR_BOX_DIR_1)
#define STEP_MOTOR_BOX_SETDIR_CCW()            GPIO_ResetBits(STEP_MOTOR_BOX_DIRPORT, STEP_MOTOR_BOX_DIR_1)

//¿ªÄ¤²½½øµç»ú
#define STEP_MOTOR_TIMX_CLOCKCMD           RCC_APB1PeriphClockCmd
#define STEP_MOTOR_TIMx_RCC_CLK            RCC_APB1Periph_TIM2
#define STEP_MOTOR_TIMx                    TIM2
#define STEP_MOTOR_TIMx_IRQn               TIM2_IRQn
#define STEP_MOTOR_TIMx_IRQHandler         TIM2_IRQHandler
#define STEP_MOTOR_TIMx_OCnInit            TIM_OC2Init
#define STEP_MOTOR_TIM_SetCompareN         TIM_SetCompare2

#define STEP_MOTOR_PULSE_CLK               RCC_APB2Periph_GPIOA
#define STEP_MOTOR_PULSE_1                 GPIO_Pin_1        // ¿ªÄ¤²½½øÂö³åÊä³öÒý½Å
#define STEP_MOTOR_PULSEPORT               GPIOA

#define STEP_MOTOR_DIR_CLK                 RCC_APB2Periph_GPIOC
#define STEP_MOTOR_DIR_1                   GPIO_Pin_4        // ·½Ïò¿ØÖÆÒý½Å
#define STEP_MOTOR_DIRPORT                 GPIOC
#define STEP_MOTOR_SETDIR_CW()             GPIO_SetBits(STEP_MOTOR_DIRPORT, STEP_MOTOR_DIR_1)
#define STEP_MOTOR_SETDIR_CCW()            GPIO_ResetBits(STEP_MOTOR_DIRPORT, STEP_MOTOR_DIR_1)



/* ºê¶¨Òå --------------------------------------------------------------------*/
#define TB6600_TIMx                        TIM2
#define TB6600_TIM_APBxClock_FUN           RCC_APB1PeriphClockCmd
#define TB6600_TIM_CLK                     RCC_APB1Periph_TIM2

#define TB6600_TIM_GPIO_APBxClock_FUN      RCC_APB1PeriphClockCmd
#define TB6600_TIM_GPIO_CLK                (RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO )
#define TB6600_TIM_CH1_PORT                GPIOA                 // ¶ÔÓ¦TB6600µÄPUL-£¬
#define TB6600_TIM_CH1_PIN                 GPIO_Pin_1            // ¶øPLU+Ö±½Ó½Ó¿ª·¢°åµÄ+5v(»òÕß3.3V)

#define TB6600_DIR_GPIO_CLK                RCC_APB2Periph_GPIOC  // µç»úÐý×ª·½Ïò¿ØÖÆ
#define TB6600_DIR_PORT                    GPIOC                 // ¶ÔÓ¦TB6600µÄDIR-£¬
#define TB6600_DIR_PIN                     GPIO_Pin_4           // ¶øDIR+Ö±½Ó½Ó¿ª·¢°åµÄ+5v(»òÕß3.3V)
#define STEPMOTOR_DIR_FORWARD()            GPIO_SetBits(TB6600_DIR_PORT,TB6600_DIR_PIN)
#define STEPMOTOR_DIR_REVERSAL()           GPIO_ResetBits(TB6600_DIR_PORT,TB6600_DIR_PIN)

#define STEPMOTOR_OUTPUT_ENABLE()          GPIO_SetBits(TB6600_ENA_PORT,TB6600_ENA_PIN)
#define STEPMOTOR_OUTPUT_DISABLE()          GPIO_ResetBits(TB6600_ENA_PORT,TB6600_ENA_PIN)


// ¶¨Òå¶¨Ê±Æ÷Ô¤·ÖÆµ£¬¶¨Ê±Æ÷Êµ¼ÊÊ±ÖÓÆµÂÊÎª£º72MHz/£¨STEPMOTOR_TIMx_PRESCALER+1£©
#define STEPMOTOR_TIM_PRESCALER               3  // ²½½øµç»úÇý¶¯Æ÷Ï¸·ÖÉèÖÃÎª£º   32  Ï¸·Ö
//#define STEPMOTOR_TIM_PRESCALER               7  // ²½½øµç»úÇý¶¯Æ÷Ï¸·ÖÉèÖÃÎª£º   16  Ï¸·Ö
//#define STEPMOTOR_TIM_PRESCALER               15  // ²½½øµç»úÇý¶¯Æ÷Ï¸·ÖÉèÖÃÎª£º   8  Ï¸·Ö
//#define STEPMOTOR_TIM_PRESCALER               31  // ²½½øµç»úÇý¶¯Æ÷Ï¸·ÖÉèÖÃÎª£º   4  Ï¸·Ö
//#define STEPMOTOR_TIM_PRESCALER               63  // ²½½øµç»úÇý¶¯Æ÷Ï¸·ÖÉèÖÃÎª£º   2  Ï¸·Ö
//#define STEPMOTOR_TIM_PRESCALER               127  // ²½½øµç»úÇý¶¯Æ÷Ï¸·ÖÉèÖÃÎª£º   1  Ï¸·Ö¡

// ¶¨Òå¶¨Ê±Æ÷ÖÜÆÚ£¬Êä³ö±È½ÏÄ£Ê½ÖÜÆÚÉèÖÃÎª0xFFFF
#define STEPMOTOR_TIM_PERIOD                  0xFFFF

#define FALSE                                 0
#define TRUE                                  1
#define CW                                    0 // Ë³Ê±Õë
#define CCW                                   1 // ÄæÊ±Õë

#define STOP                                  0 // ¼Ó¼õËÙÇúÏß×´Ì¬£ºÍ£Ö¹
#define ACCEL                                 1 // ¼Ó¼õËÙÇúÏß×´Ì¬£º¼ÓËÙ½×¶Î
#define DECEL                                 2 // ¼Ó¼õËÙÇúÏß×´Ì¬£º¼õËÙ½×¶Î
#define RUN                                   3 // ¼Ó¼õËÙÇúÏß×´Ì¬£ºÔÈËÙ½×¶Î
//#define T1_FREQ                               (SystemCoreClock/(STEPMOTOR_TIM_PRESCALER+1)) // ÆµÂÊftÖµ
#define T1_FREQ                               10000
#define FSPR                                  200         //²½½øµç»úµ¥È¦²½Êý
#define MICRO_STEP                            32          // ²½½øµç»úÇý¶¯Æ÷Ï¸·ÖÊý
#define SPR                                   (FSPR*MICRO_STEP)   // Ðý×ªÒ»È¦ÐèÒªµÄÂö³åÊý

// ÊýÑ§³£Êý
#define ALPHA                                 ((float)(2*3.14159/SPR))       // ¦Á= 2*pi/spr
#define A_T_x10                               ((float)(10*ALPHA*T1_FREQ))
#define T1_FREQ_148                           ((float)((T1_FREQ*0.676)/10)) // 0.676ÎªÎó²îÐÞÕýÖµ
#define A_SQ                                  ((float)(2*100000*ALPHA)) 
#define A_x200                                ((float)(200*ALPHA))

/*---------------------º¯ÊýÉùÃ÷ -------------------------------------------------------------*/
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
extern __IO uint8_t  stepBoxMotionStatus;           //Ö½ºÐÊÇ·ñÔÚÔË¶¯£¿0£ºÍ£Ö¹£¬1£ºÔË¶¯
extern __IO uint8_t  stepMotorMotionStatus;  				//¿ªÄ¤²½½ø
extern __IO int32_t  stepbox_position;        
extern __IO int32_t  stepmotor_position;
extern bool boxStopCmd;
#endif	
