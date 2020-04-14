#ifndef __BDCMOTOR_TIM_H__
#define __BDCMOTOR_TIM_H__

/* 包含头文件 ----------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* 类型定义 ------------------------------------------------------------------*/
/* 宏定义 --------------------------------------------------------------------*/

#define BDCMOTOR_TIMx                         TIM8
#define BDCMOTOR_TIM_RCC_CLK_ENABLE()         __HAL_RCC_TIM8_CLK_ENABLE()
#define BDCMOTOR_TIM_RCC_CLK_DISABLE()        __HAL_RCC_TIM8_CLK_DISABLE()

#define BDCMOTOR_TIM_CH1_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOC_CLK_ENABLE()     // 输出PWM脉冲给电机控制器的的IN引脚
#define BDCMOTOR_TIM_CH1_PORT                 GPIOC                            // CH1和CH1N两个引脚配套使用
#define BDCMOTOR_TIM_CH1_PIN                  GPIO_PIN_6                       // 如果电机接在驱动器的OUT1和OUT2端子上
//#define BDCMOTOR_TIM_CH1N_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOB_CLK_ENABLE()     // CH1和CH1N对应接在IN1和IN2
//#define BDCMOTOR_TIM_CH1N_PORT                GPIOB                            // 如果电机接在驱动器的OUT3和OUT4端子上
//#define BDCMOTOR_TIM_CH1N_PIN                 GPIO_PIN_13                      // CH1和CH1N对应接在IN3和IN4

#define BDCMOTOR_TIM_CH2_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOC_CLK_ENABLE()     // 输出PWM脉冲给电机控制器的的IN引脚
#define BDCMOTOR_TIM_CH2_PORT                 GPIOC                            // CH1和CH1N两个引脚配套使用
#define BDCMOTOR_TIM_CH2_PIN                  GPIO_PIN_7                       // 如果电机接在驱动器的OUT1和OUT2端子上

#define BDCMOTOR_TIM_CH3_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOC_CLK_ENABLE()     // 输出PWM脉冲给电机控制器的的IN引脚
#define BDCMOTOR_TIM_CH3_PORT                 GPIOC                            // CH1和CH1N两个引脚配套使用
#define BDCMOTOR_TIM_CH3_PIN                  GPIO_PIN_8                       // 如果电机接在驱动器的OUT1和OUT2端子上

#define BDCMOTOR_TIM_CH4_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOC_CLK_ENABLE()     // 输出PWM脉冲给电机控制器的的IN引脚
#define BDCMOTOR_TIM_CH4_PORT                 GPIOC                            // CH1和CH1N两个引脚配套使用
#define BDCMOTOR_TIM_CH4_PIN                  GPIO_PIN_9                       // 如果电机接在驱动器的OUT1和OUT2端子上

#define DCMOTOR_COMMON_TIMx                         TIM4
#define DCMOTOR_COMMON_TIM_RCC_CLK_ENABLE()         __HAL_RCC_TIM4_CLK_ENABLE()
#define DCMOTOR_COMMON_TIM_RCC_CLK_DISABLE()        __HAL_RCC_TIM4_CLK_DISABLE()

#define DCMOTOR_COMMON_TIM_CH1_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOB_CLK_ENABLE()     // 输出PWM脉冲给电机控制器的的IN引脚
#define DCMOTOR_COMMON_TIM_CH1_PORT                 GPIOB                            // CH1和CH1N两个引脚配套使用
#define DCMOTOR_COMMON_TIM_CH1_PIN                  GPIO_PIN_6                       // 如果电机接在驱动器的OUT1和OUT2端子上

#define DCMOTOR_COMMON_TIM_CH2_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOB_CLK_ENABLE()     // 输出PWM脉冲给电机控制器的的IN引脚
#define DCMOTOR_COMMON_TIM_CH2_PORT                 GPIOB                            // CH1和CH1N两个引脚配套使用
#define DCMOTOR_COMMON_TIM_CH2_PIN                  GPIO_PIN_7                       // 如果电机接在驱动器的OUT1和OUT2端子上

#define DCMOTOR_COMMON_TIM_CH3_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOB_CLK_ENABLE()     // 输出PWM脉冲给电机控制器的的IN引脚
#define DCMOTOR_COMMON_TIM_CH3_PORT                 GPIOB                            // CH1和CH1N两个引脚配套使用
#define DCMOTOR_COMMON_TIM_CH3_PIN                  GPIO_PIN_8                       // 如果电机接在驱动器的OUT1和OUT2端子上

#define DCMOTOR_COMMON_TIM_CH4_GPIO_CLK_ENABLE()    __HAL_RCC_GPIOB_CLK_ENABLE()     // 输出PWM脉冲给电机控制器的的IN引脚
#define DCMOTOR_COMMON_TIM_CH4_PORT                 GPIOB                            // CH1和CH1N两个引脚配套使用
#define DCMOTOR_COMMON_TIM_CH4_PIN                  GPIO_PIN_9                       // 如果电机接在驱动器的OUT1和OUT2端子上

#define MOTOR_GPIO_AFx                        GPIO_AF3_TIM8
#define MOTOR_COMMON_GPIO_AFx                 GPIO_AF2_TIM4

// 定义定时器预分频，定时器实际时钟频率为：168MHz/（BDCMOTOR_TIMx_PRESCALER+1）
#define BDCMOTOR_TIM_PRESCALER               			 1    // 实际时钟频率为：168/2 MHz
// 定义定时器预分频，TIM3定时器实际时钟频率为：84MHz/（BDCMOTOR_TIMx_PRESCALER+1）
#define COMMON_DCMOTOR_TIM_PRESCALER               0    // 实际时钟频率为：168/2 MHz
// 定义定时器周期，PWM频率为：168MHz/（BDCMOTOR_TIMx_PRESCALER+1）/（BDCMOTOR_TIM_PERIOD+1）
#define BDCMOTOR_TIM_PERIOD                 			 4199  // PWM频率为84MHz/(4199+1)=20KHz
#define COMMON_DCMOTOR_TIM_PERIOD                  4199  // PWM频率为84MHz/(4199+1)=20KHz

// 定义高级定时器重复计数寄存器值
// 实际PWM频率为：168MHz/（BDCMOTOR_TIMx_PRESCALER+1）/（BDCMOTOR_TIM_PERIOD+1）/（BDCMOTOR_TIM_REPETITIONCOUNTER+1）
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


/* 扩展变量 ------------------------------------------------------------------*/
extern TIM_HandleTypeDef htimx_BDCMOTOR;
extern TIM_HandleTypeDef htimx_COMMON_DCMOTOR;

extern __IO uint16_t PWM_Duty;

/* 函数声明 ------------------------------------------------------------------*/

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
/******************* (C) COPYRIGHT 2015-2020 硬石嵌入式开发团队 *****END OF FILE****/
