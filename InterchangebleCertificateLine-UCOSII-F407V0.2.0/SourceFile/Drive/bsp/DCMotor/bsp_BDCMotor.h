#ifndef __BDCMOTOR_TIM_H__
#define __BDCMOTOR_TIM_H__

/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* ���Ͷ��� ------------------------------------------------------------------*/
/* �궨�� --------------------------------------------------------------------*/

#define BDCMOTOR_TIMx                         TIM8
#define BDCMOTOR_TIM_RCC_CLK_ENABLE()         __HAL_RCC_TIM8_CLK_ENABLE()
#define BDCMOTOR_TIM_RCC_CLK_DISABLE()        __HAL_RCC_TIM8_CLK_DISABLE()

#define BDCMOTOR_TIM_CH1_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOC_CLK_ENABLE()     // ���PWM���������������ĵ�IN����
#define BDCMOTOR_TIM_CH1_PORT                 GPIOC                            // CH1��CH1N������������ʹ��
#define BDCMOTOR_TIM_CH1_PIN                  GPIO_PIN_6                       // ������������������OUT1��OUT2������
//#define BDCMOTOR_TIM_CH1N_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOB_CLK_ENABLE()     // CH1��CH1N��Ӧ����IN1��IN2
//#define BDCMOTOR_TIM_CH1N_PORT                GPIOB                            // ������������������OUT3��OUT4������
//#define BDCMOTOR_TIM_CH1N_PIN                 GPIO_PIN_13                      // CH1��CH1N��Ӧ����IN3��IN4

#define BDCMOTOR_TIM_CH2_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOC_CLK_ENABLE()     // ���PWM���������������ĵ�IN����
#define BDCMOTOR_TIM_CH2_PORT                 GPIOC                            // CH1��CH1N������������ʹ��
#define BDCMOTOR_TIM_CH2_PIN                  GPIO_PIN_7                       // ������������������OUT1��OUT2������

#define BDCMOTOR_TIM_CH3_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOC_CLK_ENABLE()     // ���PWM���������������ĵ�IN����
#define BDCMOTOR_TIM_CH3_PORT                 GPIOC                            // CH1��CH1N������������ʹ��
#define BDCMOTOR_TIM_CH3_PIN                  GPIO_PIN_8                       // ������������������OUT1��OUT2������

#define BDCMOTOR_TIM_CH4_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOC_CLK_ENABLE()     // ���PWM���������������ĵ�IN����
#define BDCMOTOR_TIM_CH4_PORT                 GPIOC                            // CH1��CH1N������������ʹ��
#define BDCMOTOR_TIM_CH4_PIN                  GPIO_PIN_9                       // ������������������OUT1��OUT2������

#define DCMOTOR_COMMON_TIMx                         TIM4
#define DCMOTOR_COMMON_TIM_RCC_CLK_ENABLE()         __HAL_RCC_TIM4_CLK_ENABLE()
#define DCMOTOR_COMMON_TIM_RCC_CLK_DISABLE()        __HAL_RCC_TIM4_CLK_DISABLE()

#define DCMOTOR_COMMON_TIM_CH1_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOB_CLK_ENABLE()     // ���PWM���������������ĵ�IN����
#define DCMOTOR_COMMON_TIM_CH1_PORT                 GPIOB                            // CH1��CH1N������������ʹ��
#define DCMOTOR_COMMON_TIM_CH1_PIN                  GPIO_PIN_6                       // ������������������OUT1��OUT2������

#define DCMOTOR_COMMON_TIM_CH2_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOB_CLK_ENABLE()     // ���PWM���������������ĵ�IN����
#define DCMOTOR_COMMON_TIM_CH2_PORT                 GPIOB                            // CH1��CH1N������������ʹ��
#define DCMOTOR_COMMON_TIM_CH2_PIN                  GPIO_PIN_7                       // ������������������OUT1��OUT2������

#define DCMOTOR_COMMON_TIM_CH3_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOB_CLK_ENABLE()     // ���PWM���������������ĵ�IN����
#define DCMOTOR_COMMON_TIM_CH3_PORT                 GPIOB                            // CH1��CH1N������������ʹ��
#define DCMOTOR_COMMON_TIM_CH3_PIN                  GPIO_PIN_8                       // ������������������OUT1��OUT2������

#define DCMOTOR_COMMON_TIM_CH4_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOB_CLK_ENABLE()     // ���PWM���������������ĵ�IN����
#define DCMOTOR_COMMON_TIM_CH4_PORT                 GPIOB                            // CH1��CH1N������������ʹ��
#define DCMOTOR_COMMON_TIM_CH4_PIN                  GPIO_PIN_9                       // ������������������OUT1��OUT2������

#define MOTOR_GPIO_AFx                        GPIO_AF3_TIM8
#define MOTOR_COMMON_GPIO_AFx                 GPIO_AF2_TIM4

// ���嶨ʱ��Ԥ��Ƶ����ʱ��ʵ��ʱ��Ƶ��Ϊ��168MHz/��BDCMOTOR_TIMx_PRESCALER+1��
#define BDCMOTOR_TIM_PRESCALER               			 1    // ʵ��ʱ��Ƶ��Ϊ��168/2 MHz
// ���嶨ʱ��Ԥ��Ƶ��TIM3��ʱ��ʵ��ʱ��Ƶ��Ϊ��84MHz/��BDCMOTOR_TIMx_PRESCALER+1��
#define COMMON_DCMOTOR_TIM_PRESCALER               0    // ʵ��ʱ��Ƶ��Ϊ��168/2 MHz
// ���嶨ʱ�����ڣ�PWMƵ��Ϊ��168MHz/��BDCMOTOR_TIMx_PRESCALER+1��/��BDCMOTOR_TIM_PERIOD+1��
#define BDCMOTOR_TIM_PERIOD                 			 4199  // PWMƵ��Ϊ84MHz/(4199+1)=20KHz
#define COMMON_DCMOTOR_TIM_PERIOD                  4199  // PWMƵ��Ϊ84MHz/(4199+1)=20KHz

// ����߼���ʱ���ظ������Ĵ���ֵ
// ʵ��PWMƵ��Ϊ��168MHz/��BDCMOTOR_TIMx_PRESCALER+1��/��BDCMOTOR_TIM_PERIOD+1��/��BDCMOTOR_TIM_REPETITIONCOUNTER+1��
#define BDCMOTOR_TIM_REPETITIONCOUNTER       0

#define LEFTHOB_MOTORIO_OUT_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOD_CLK_ENABLE()
#define LEFTHOB_MOTORIO_OUT_PIN                     GPIO_PIN_7
#define LEFTHOB_MOTORIO_PORT												 GPIOD


#define RIGHTHOB_MOTORIO_OUT_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOG_CLK_ENABLE()
#define RIGHTHOB_MOTORIO_OUT_PIN                     GPIO_PIN_9
#define RIGHTHOB_MOTORIO_PORT												 GPIOG

#define LEFTCHAMFER_MOTORIO_OUT_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOG_CLK_ENABLE()
#define LEFTCHAMFER_MOTORIO_OUT_PIN                     GPIO_PIN_10
#define LEFTCHAMFER_MOTORIO_PORT												GPIOG

#define RIGHTCHAMFER_MOTORIO_OUT_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOG_CLK_ENABLE()
#define RIGHTCHAMFER_MOTORIO_OUT_PIN                     GPIO_PIN_11
#define RIGHTCHAMFER_MOTORIO_PORT												 GPIOG

#define FLOTASMCUT_MOTORIO_OUT_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOE_CLK_ENABLE()
#define FLOTASMCUT_MOTORIO_OUT_PIN                     GPIO_PIN_14
#define FLOTASMCUT_MOTORIO_PORT												 GPIOG

#define STAMP_ACMOTORIO_OUT_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOE_CLK_ENABLE()
#define STAMP_ACMOTORIO_OUT_PIN                     GPIO_PIN_3
#define STAMP_ACMOTORIO_PORT												 GPIOE

#define PAGING_CONVEYER_MOTORIO_OUT_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOG_CLK_ENABLE()
#define PAGING_CONVEYER_MOTORIO_OUT_PIN                     GPIO_PIN_12
#define PAGING_CONVEYER_MOTORIO_PORT												 GPIOG

#define PAGING_HEATING_OUT_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOE_CLK_ENABLE()
#define PAGING_HEATING_OUT_PIN                     GPIO_PIN_4
#define PAGING_HEATING_PORT												 GPIOE


/* ��չ���� ------------------------------------------------------------------*/
extern TIM_HandleTypeDef htimx_BDCMOTOR;
extern TIM_HandleTypeDef htimx_COMMON_DCMOTOR;

extern __IO uint16_t PWM_Duty;

/* �������� ------------------------------------------------------------------*/

void initMotorPWM(void);
void initMotorIOPushOut(void);

void leftHobMotorIO(uint8_t Dir, uint8_t Per);
void rightHobMotorIO(uint8_t Dir, uint8_t Per);
void chamferMotorIO(uint8_t Dir, uint8_t Per);
void flotasmCutMotorIO(uint8_t Dir, uint8_t Per);
void StampACMotorIO(uint8_t status);
void pagingConveyerMotorIO(uint8_t Dir, uint8_t Per);
void hobMotorIO(uint8_t Dir, uint8_t Per);

#endif	/* __BDCMOTOR_TIM_H__ */
/******************* (C) COPYRIGHT 2015-2020 ӲʯǶ��ʽ�����Ŷ� *****END OF FILE****/
