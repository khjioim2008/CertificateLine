#ifndef __BSP_Sensor_H__
#define __BSP_Sensor_H__

/* 包含头文件 ----------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "Resources.h"
#include "BasicTypes.h"
#include <stdio.h>
#include <string.h>
/* 类型定义 ------------------------------------------------------------------*/
/* 宏定义 --------------------------------------------------------------------*/

/*Paging Machine****/
#define EXIST_PAPER_SENSORIO_IN_GPIO_CLK_ENABLE()       						__HAL_RCC_GPIOD_CLK_ENABLE()
#define EXIST_PAPER_OUT_SENSORIO_IN_PIN                     						GPIO_PIN_11
#define EXIST_PAPER_OUT_SENSORIO_PORT																	GPIOD

#define FRICTION_ORIGIN_SENSORIO_IN_GPIO_CLK_ENABLE()       						__HAL_RCC_GPIOG_CLK_ENABLE()
#define FRICTION_ORIGIN_SENSORIO_IN_PIN                     						GPIO_PIN_0
#define FRICTION_ORIGIN_SENSORIO_PORT																	GPIOG

#define FRICTION_OUT_SENSORIO_IN_GPIO_CLK_ENABLE()       						__HAL_RCC_GPIOG_CLK_ENABLE()
#define FRICTION_OUT_SENSORIO_IN_PIN                     						GPIO_PIN_0
#define FRICTION_OUT_SENSORIO_PORT																	GPIOG

#define FRICTION_OUT_ONE_SENSORIO_IN_GPIO_CLK_ENABLE()       						__HAL_RCC_GPIOD_CLK_ENABLE()
#define FRICTION_OUT_ONE_SENSORIO_IN_PIN                     						GPIO_PIN_10
#define FRICTION_OUT_ONE_SENSORIO_PORT																	GPIOD

#define FRICTION_OUT_TWO_SENSORIO_IN_GPIO_CLK_ENABLE()       						__HAL_RCC_GPIOD_CLK_ENABLE()
#define FRICTION_OUT_TWO_SENSORIO_IN_PIN                     						GPIO_PIN_9
#define FRICTION_OUT_TWO_SENSORIO_PORT																	GPIOD

#define FRICTION_OUT_THREE_SENSORIO_IN_GPIO_CLK_ENABLE()       						__HAL_RCC_GPIOD_CLK_ENABLE()
#define FRICTION_OUT_THREE_SENSORIO_IN_PIN                     						GPIO_PIN_8
#define FRICTION_OUT_THREE_SENSORIO_PORT																	GPIOD

#define FRICTION_OUT_FOUR_SENSORIO_IN_GPIO_CLK_ENABLE()       						__HAL_RCC_GPIOC_CLK_ENABLE()
#define FRICTION_OUT_FOUR_SENSORIO_IN_PIN                     						GPIO_PIN_0
#define FRICTION_OUT_FOUR_SENSORIO_PORT																	GPIOC

#define ROLLER_OUT_SENSORIO_IN_GPIO_CLK_ENABLE()       						__HAL_RCC_GPIOB_CLK_ENABLE()
#define ROLLER_OUT_SENSORIO_IN_PIN                     						GPIO_PIN_0
#define ROLLER_OUT_SENSORIO_PORT																	GPIOB


//CutMachine
#define CUTFLOTASM_POSTION_SENSORIO_IN_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOC_CLK_ENABLE()
#define CUTFLOTASM_POSTION_SENSORIO_IN_PIN                     GPIO_PIN_5
#define CUTFLOTASM_POSTION_SENSORIO_PORT												GPIOC

#define LEFT_FLOTASMCUT_SENSORIO_IN_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOA_CLK_ENABLE()
#define LEFT_FLOTASMCUT_SENSORIO_IN_PIN                     GPIO_PIN_4
#define LEFT_FLOTASMCUT_SENSORIO_PORT												GPIOA

#define RIGHT_FLOTASMCUT_SENSORIO_IN_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOA_CLK_ENABLE()
#define RIGHT_FLOTASMCUT_SENSORIO_IN_PIN                     GPIO_PIN_3
#define RIGHT_FLOTASMCUT_SENSORIO_PORT												GPIOA

#define CHAMFER_POSTION_SENSORIO_IN_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOF_CLK_ENABLE()
#define CHAMFER_POSTION_SENSORIO_IN_PIN                     GPIO_PIN_15
#define CHAMFER_POSTION_SENSORIO_PORT												GPIOF

#define CHAMFER_MOTOR_SENSORIO_IN_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOG_CLK_ENABLE()
#define CHAMFER_MOTOR_SENSORIO_IN_PIN                     GPIO_PIN_1
#define CHAMFER_MOTOR_SENSORIO_PORT												GPIOG

#define CUTMACHINE_FRONT_SENSORIO_IN_GPIO_CLK_ENABLE()       						__HAL_RCC_GPIOE_CLK_ENABLE()
#define CUTMACHINE_FRONT_SENSORIO_IN_PIN                     						GPIO_PIN_13
#define CUTMACHINE_FRONT_SENSORIO_PORT																	GPIOE

#define CUTMACHINE_BEHIND_SENSORIO_IN_GPIO_CLK_ENABLE()       						__HAL_RCC_GPIOE_CLK_ENABLE()
#define CUTMACHINE_BEHIND_SENSORIO_IN_PIN                     						GPIO_PIN_12
#define CUTMACHINE_BEHIND_SENSORIO_PORT																	GPIOE


//StampMachine
#define STAMPPOSTION_SENSORIO_IN_GPIO_CLK_ENABLE()       						__HAL_RCC_GPIOB_CLK_ENABLE()
#define STAMPPOSTION_SENSORIO_IN_PIN                     						GPIO_PIN_10
#define STAMPPOSTION_SENSORIO_PORT																	GPIOD

#define STAMPACMOTOR_SENSORIO_IN_GPIO_CLK_ENABLE()       						__HAL_RCC_GPIOD_CLK_ENABLE()
#define STAMPACMOTOR_SENSORIO_IN_PIN                     						GPIO_PIN_11
#define STAMPACMOTOR_SENSORIO_PORT																	GPIOD

#define STAMPWAIT_UP_SENSORIO_IN_GPIO_CLK_ENABLE()       						__HAL_RCC_GPIOD_CLK_ENABLE()
#define STAMPWAIT_UP_SENSORIO_IN_PIN                     						GPIO_PIN_8
#define STAMPWAIT_UP_SENSORIO_PORT																	GPIOD

#define STAMPWAIT_DOWN_SENSORIO_IN_GPIO_CLK_ENABLE()       						__HAL_RCC_GPIOD_CLK_ENABLE()
#define STAMPWAIT_DOWN_SENSORIO_IN_PIN                     						GPIO_PIN_9
#define STAMPWAIT_DOWN_SENSORIO_PORT																	GPIOD


void initSensorIO(void);

void signalStability(Sign *Name,N8 (* fun)(void));

void signalStabilityT(Sign *Name,N8 (* fun)(void),N16 DeTemp);

uint8_t sensorPagingExistPaper(void);
uint8_t sensorPagingFrictionOrigin(void);
uint8_t sensorPagingFrictionOut(void);
uint8_t sensorPagingFrictionOutOne(void);
uint8_t sensorPagingFrictionOutTwo(void);
uint8_t sensorPagingFrictionOutThree(void);
uint8_t sensorPagingFrictionOutFour(void);
uint8_t sensorPagingRollerOut(void);


uint8_t sensorCutFlotasmPostion(void);
uint8_t sensorCutLeftFlotasm(void);
uint8_t sensorCutRightFlotasm(void);
uint8_t sensorCutChamferMotor(void);
uint8_t sensorCutChamferPostion(void);
uint8_t sensorCutMachineFront(void);
uint8_t sensorCutMachineBehind(void);



uint8_t sensorStampPostion(void);
uint8_t sensorStampACMotor(void);
uint8_t sensorStampWaitUp(void);
uint8_t sensorStampWaitDown(void);

#endif 
