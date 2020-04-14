#ifndef __ENCODER_TIM_H__
#define __ENCODER_TIM_H__

/* 包含头文件 ----------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* 类型定义 ------------------------------------------------------------------*/
/* 宏定义 --------------------------------------------------------------------*/
#define ENCODER_TIMx                        TIM5
#define ENCODER_TIM_RCC_CLK_ENABLE()        __HAL_RCC_TIM5_CLK_ENABLE()
#define ENCODER_TIM_RCC_CLK_DISABLE()       __HAL_RCC_TIM5_CLK_DISABLE()

#define ENCODER_TIM_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOA_CLK_ENABLE()
#define ENCODER_TIM_CH1_PIN                 GPIO_PIN_0
#define ENCODER_TIM_CH1_GPIO                GPIOA
#define ENCODER_TIM_CH2_PIN                 GPIO_PIN_1
#define ENCODER_TIM_CH2_GPIO                GPIOA
#define GPIO_CH1_AFx_TIMx                   GPIO_AF2_TIM5
#define GPIO_CH2_AFx_TIMx                   GPIO_AF2_TIM5

#define TIM_ENCODERMODE_TIx                 TIM_ENCODERMODE_TI12

#define ENCODER_TIM_IRQn                    TIM5_IRQn
#define ENCODER_TIM_IRQHANDLER              TIM5_IRQHandler

// 定义定时器预分频，定时器实际时钟频率为：84MHz/（ENCODER_TIMx_PRESCALER+1）
#define ENCODER_TIM_PRESCALER               0  // 

// 定义定时器周期，当定时器开始计数到ENCODER_TIMx_PERIOD值是更新定时器并生成对应事件和中断
#define ENCODER_TIM_PERIOD                  0xFFFF



//新增
#define ENCODER_TWO_TIMx                        TIM3
#define ENCODER_TWO_TIM_RCC_CLK_ENABLE()        __HAL_RCC_TIM3_CLK_ENABLE()
#define ENCODER_TWO_TIM_RCC_CLK_DISABLE()       __HAL_RCC_TIM3_CLK_DISABLE()

#define ENCODER_TWO_TIM_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOA_CLK_ENABLE()
#define ENCODER_TWO_TIM_CH1_PIN                 GPIO_PIN_6
#define ENCODER_TWO_TIM_CH1_GPIO                GPIOA
#define ENCODER_TWO_TIM_CH2_PIN                 GPIO_PIN_7
#define ENCODER_TWO_TIM_CH2_GPIO                GPIOA
#define ENCODER_TWO_GPIO_CH1_AFx_TIMx                   GPIO_AF2_TIM3
#define ENCODER_TWO_GPIO_CH2_AFx_TIMx                   GPIO_AF2_TIM3

#define TIM_ENCODERTWO_MODE_TIx                 TIM_ENCODERMODE_TI12

#define ENCODER_TWO_TIM_IRQn                    TIM3_IRQn
#define ENCODER_TWO_TIM_IRQHANDLER              TIM3_IRQHandler

// 定义定时器预分频，定时器实际时钟频率为：84MHz/（ENCODER_TIMx_PRESCALER+1）
#define ENCODER_TWO_TIM_PRESCALER               0  // 

// 定义定时器周期，当定时器开始计数到ENCODER_TIMx_PERIOD值是更新定时器并生成对应事件和中断
#define ENCODER_TWO_TIM_PERIOD                  0xFFFF





/* 扩展变量 ------------------------------------------------------------------*/
extern TIM_HandleTypeDef htimx_Encoder;
extern TIM_HandleTypeDef    htimx_EncoderTwo;

/* 函数声明 ------------------------------------------------------------------*/
void ENCODER_TIMx_Init(void);

#endif	/* __ENCODER_TIM_H__ */
/******************* (C) COPYRIGHT 2015-2020 硬石嵌入式开发团队 *****END OF FILE****/
