
/* 包含头文件 ----------------------------------------------------------------*/
#include "DCMotor/bsp_BDCMotor.h"
#include "Configure.h"

/* 私有类型定义 --------------------------------------------------------------*/
/* 私有宏定义 ----------------------------------------------------------------*/
/* 私有变量 ------------------------------------------------------------------*/
TIM_HandleTypeDef htimx_BDCMOTOR;
TIM_HandleTypeDef htimx_COMMON_DCMOTOR;
__IO uint16_t PWM_Duty=500;         // 占空比：PWM_Duty/BDCMOTOR_TIM_PERIOD*100%

/* 扩展变量 ------------------------------------------------------------------*/
/* 私有函数原形 --------------------------------------------------------------*/
/* 函数体 --------------------------------------------------------------------*/
/**
  * 函数功能: 基本定时器硬件初始化配置
  * 输入参数: htim_base：基本定时器句柄类型指针
  * 返 回 值: 无
  * 说    明: BDCMOTOR相关GPIO初始化配置,该函数被HAL库内部调用.
  */
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
  /* BDCMOTOR相关GPIO初始化配置 */
  if(htim == &htimx_BDCMOTOR)
  {
	 BDCMOTOR_TIM_CH1_GPIO_CLK_ENABLE();
	 DCMOTOR_COMMON_TIM_CH1_GPIO_CLK_ENABLE();
  }
}

void initMotorPWM(void)
{
	/***htimx_BDCMOTOR TIM1***/
  TIM_ClockConfigTypeDef sClockSourceConfig;             // 定时器时钟
  TIM_OC_InitTypeDef sConfigOC;                          // 定时器通道比较输出
  TIM_BreakDeadTimeConfigTypeDef  sBDTConfig;            // 定时器死区时间比较输出
  /* 基本定时器外设时钟使能 */
  BDCMOTOR_TIM_RCC_CLK_ENABLE();
	
  /* 定时器基本环境配置 */
  htimx_BDCMOTOR.Instance = BDCMOTOR_TIMx;                                 // 定时器编号
  htimx_BDCMOTOR.Init.Prescaler = BDCMOTOR_TIM_PRESCALER;                  // 定时器预分频器
  htimx_BDCMOTOR.Init.CounterMode = TIM_COUNTERMODE_UP;                  // 计数方向：向上计数
  htimx_BDCMOTOR.Init.Period = BDCMOTOR_TIM_PERIOD;                        // 定时器周期
  htimx_BDCMOTOR.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;              // 时钟分频
  htimx_BDCMOTOR.Init.RepetitionCounter = BDCMOTOR_TIM_REPETITIONCOUNTER;  // 重复计数器
  /* 初始化定时器比较输出环境 */
  HAL_TIM_PWM_Init(&htimx_BDCMOTOR);

  /* 定时器时钟源配置 */
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;       // 使用内部时钟源
  HAL_TIM_ConfigClockSource(&htimx_BDCMOTOR, &sClockSourceConfig);

  /* 死区刹车配置,配置有效电平是低电平 */
  sBDTConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE ;
  sBDTConfig.BreakPolarity = TIM_BREAKPOLARITY_LOW ;
  sBDTConfig.BreakState = TIM_BREAK_DISABLE ;
  sBDTConfig.DeadTime = 0 ;
  sBDTConfig.LockLevel = TIM_LOCKLEVEL_OFF ;
  sBDTConfig.OffStateIDLEMode= TIM_OSSI_DISABLE ;
  sBDTConfig.OffStateRunMode = TIM_OSSR_ENABLE ;
  HAL_TIMEx_ConfigBreakDeadTime(&htimx_BDCMOTOR,&sBDTConfig);

  /* 定时器比较输出配置 */
  sConfigOC.OCMode = TIM_OCMODE_PWM1;                  // 比较输出模式：PWM1模式
  sConfigOC.Pulse =  PWM_Duty;                         // 占空比
  sConfigOC.OCPolarity = TIM_OCPOLARITY_LOW;          // 输出极性
// sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
 sConfigOC.OCNPolarity = TIM_OCNPOLARITY_LOW;        // 互补通道输出极性
// sConfigOC.OCNPolarity = TIM_OCPOLARITY_HIGH;        // 互补通道输出极性

  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;           // 快速模式
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;       // 空闲电平
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;     // 互补通道空闲电平
  HAL_TIM_PWM_ConfigChannel(&htimx_BDCMOTOR, &sConfigOC, TIM_CHANNEL_1);
	HAL_TIM_PWM_ConfigChannel(&htimx_BDCMOTOR, &sConfigOC, TIM_CHANNEL_2);
	HAL_TIM_PWM_ConfigChannel(&htimx_BDCMOTOR, &sConfigOC, TIM_CHANNEL_3);
	HAL_TIM_PWM_ConfigChannel(&htimx_BDCMOTOR, &sConfigOC, TIM_CHANNEL_4);
  HAL_TIM_Base_Start(&htimx_BDCMOTOR);
	
	
	
	
	
  /***htimx_COMMON_DCMOTOR TIM4***/
  /* 基本定时器外设时钟使能 */
  DCMOTOR_COMMON_TIM_RCC_CLK_ENABLE();
	  /* 定时器基本环境配置 */
  htimx_COMMON_DCMOTOR.Instance = DCMOTOR_COMMON_TIMx;                                 // 定时器编号
  htimx_COMMON_DCMOTOR.Init.Prescaler = COMMON_DCMOTOR_TIM_PRESCALER;                  // 定时器预分频器
  htimx_COMMON_DCMOTOR.Init.CounterMode = TIM_COUNTERMODE_UP;                  // 计数方向：向上计数
  htimx_COMMON_DCMOTOR.Init.Period = COMMON_DCMOTOR_TIM_PERIOD;                        // 定时器周期
  htimx_COMMON_DCMOTOR.Init.ClockDivision= TIM_CLOCKDIVISION_DIV1;              // 时钟分频
 // htimx_COMMON_DCMOTOR.Init.RepetitionCounter = BDCMOTOR_TIM_REPETITIONCOUNTER;  // 重复计数器
	HAL_TIM_PWM_Init(&htimx_COMMON_DCMOTOR);
	  /* 定时器时钟源配置 */
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;       // 使用内部时钟源
  HAL_TIM_ConfigClockSource(&htimx_COMMON_DCMOTOR, &sClockSourceConfig);
 
	sConfigOC.OCMode = TIM_OCMODE_PWM1;                  // 比较输出模式：PWM1模式
  sConfigOC.Pulse =  PWM_Duty;                         // 占空比
  sConfigOC.OCPolarity = TIM_OCPOLARITY_LOW;          // 输出极性
 // sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_LOW;        // 互补通道输出极性
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;           // 快速模式
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;       // 空闲电平
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;     // 互补通道空闲电平
 
 HAL_TIM_PWM_ConfigChannel(&htimx_COMMON_DCMOTOR, &sConfigOC, TIM_CHANNEL_1);
	HAL_TIM_PWM_ConfigChannel(&htimx_COMMON_DCMOTOR, &sConfigOC, TIM_CHANNEL_2);
  HAL_TIM_PWM_ConfigChannel(&htimx_COMMON_DCMOTOR, &sConfigOC, TIM_CHANNEL_3);
  HAL_TIM_PWM_ConfigChannel(&htimx_COMMON_DCMOTOR, &sConfigOC, TIM_CHANNEL_3);

	HAL_TIM_Base_Start(&htimx_COMMON_DCMOTOR);
	
	
	
	
	
  HAL_TIM_PWM_Stop(&htimx_BDCMOTOR,TIM_CHANNEL_1);
	
  HAL_TIMEx_PWMN_Stop(&htimx_BDCMOTOR,TIM_CHANNEL_1);         // 停止输出

	GPIO_InitTypeDef GPIO_InitStruct;

	/* Set as PWM Output Pin*/
	GPIO_InitStruct.Pin = BDCMOTOR_TIM_CH1_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Alternate = MOTOR_GPIO_AFx;
	HAL_GPIO_Init(BDCMOTOR_TIM_CH1_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = BDCMOTOR_TIM_CH2_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Alternate = MOTOR_GPIO_AFx;
	HAL_GPIO_Init(BDCMOTOR_TIM_CH2_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = BDCMOTOR_TIM_CH3_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Alternate = MOTOR_GPIO_AFx;
	HAL_GPIO_Init(BDCMOTOR_TIM_CH3_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = BDCMOTOR_TIM_CH4_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Alternate = MOTOR_GPIO_AFx;
	HAL_GPIO_Init(BDCMOTOR_TIM_CH4_PORT, &GPIO_InitStruct);

	HAL_TIM_PWM_Start(&htimx_BDCMOTOR,TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htimx_BDCMOTOR,TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htimx_BDCMOTOR,TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htimx_BDCMOTOR,TIM_CHANNEL_4);
	
	
	
	GPIO_InitStruct.Pin = DCMOTOR_COMMON_TIM_CH1_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Alternate = MOTOR_COMMON_GPIO_AFx;
	HAL_GPIO_Init(DCMOTOR_COMMON_TIM_CH1_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = DCMOTOR_COMMON_TIM_CH2_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Alternate = MOTOR_COMMON_GPIO_AFx;
	HAL_GPIO_Init(DCMOTOR_COMMON_TIM_CH2_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = DCMOTOR_COMMON_TIM_CH3_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Alternate = MOTOR_COMMON_GPIO_AFx;
	HAL_GPIO_Init(DCMOTOR_COMMON_TIM_CH3_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = DCMOTOR_COMMON_TIM_CH4_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Alternate = MOTOR_COMMON_GPIO_AFx;
	HAL_GPIO_Init(DCMOTOR_COMMON_TIM_CH4_PORT, &GPIO_InitStruct);
	
	
	HAL_TIM_PWM_Start(&htimx_COMMON_DCMOTOR,TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htimx_COMMON_DCMOTOR,TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htimx_COMMON_DCMOTOR,TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htimx_COMMON_DCMOTOR,TIM_CHANNEL_4);
	
	
	
	 /**初始化PWM**/
  __HAL_TIM_SetCompare(&htimx_BDCMOTOR,TIM_CHANNEL_1,0);
	__HAL_TIM_SetCompare(&htimx_BDCMOTOR,TIM_CHANNEL_2,0);
	__HAL_TIM_SetCompare(&htimx_BDCMOTOR,TIM_CHANNEL_3,0);
	__HAL_TIM_SetCompare(&htimx_BDCMOTOR,TIM_CHANNEL_4,0);
	
	__HAL_TIM_SetCompare(&htimx_COMMON_DCMOTOR,TIM_CHANNEL_1,0);
	__HAL_TIM_SetCompare(&htimx_COMMON_DCMOTOR,TIM_CHANNEL_2,0);
	__HAL_TIM_SetCompare(&htimx_COMMON_DCMOTOR,TIM_CHANNEL_3,0);
	__HAL_TIM_SetCompare(&htimx_COMMON_DCMOTOR,TIM_CHANNEL_4,0);
	
	
	
}

void initMotorIOPushOut(void)
{
	GPIO_InitTypeDef GPIO_InitStruct; 
  
	RIGHTHOB_MOTORIO_OUT_GPIO_CLK_ENABLE();
	LEFTHOB_MOTORIO_OUT_GPIO_CLK_ENABLE();
	LEFTCHAMFER_MOTORIO_OUT_GPIO_CLK_ENABLE();
//	RIGHTCHAMFER_MOTORIO_OUT_GPIO_CLK_ENABLE();
//	FLOTASMCUT_MOTORIO_OUT_GPIO_CLK_ENABLE();
//	STAMP_ACMOTORIO_OUT_GPIO_CLK_ENABLE();
//	PAGING_CONVEYER_MOTORIO_OUT_GPIO_CLK_ENABLE();
//	PAGING_HEATING_OUT_GPIO_CLK_ENABLE();
	
	GPIO_InitStruct.Pin = RIGHTHOB_MOTORIO_OUT_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(RIGHTHOB_MOTORIO_PORT, &GPIO_InitStruct);
	HAL_GPIO_WritePin(RIGHTHOB_MOTORIO_PORT,RIGHTHOB_MOTORIO_OUT_PIN,GPIO_PIN_SET);
	
	GPIO_InitStruct.Pin = LEFTHOB_MOTORIO_OUT_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(LEFTHOB_MOTORIO_PORT, &GPIO_InitStruct);
	HAL_GPIO_WritePin(LEFTHOB_MOTORIO_PORT,LEFTHOB_MOTORIO_OUT_PIN,GPIO_PIN_SET);

	GPIO_InitStruct.Pin = LEFTCHAMFER_MOTORIO_OUT_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(LEFTCHAMFER_MOTORIO_PORT, &GPIO_InitStruct);
	HAL_GPIO_WritePin(LEFTCHAMFER_MOTORIO_PORT,LEFTCHAMFER_MOTORIO_OUT_PIN,GPIO_PIN_SET);

	GPIO_InitStruct.Pin = GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

//	GPIO_InitStruct.Pin = FLOTASMCUT_MOTORIO_OUT_PIN;
//  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
//  HAL_GPIO_Init(FLOTASMCUT_MOTORIO_PORT, &GPIO_InitStruct);
//	HAL_GPIO_WritePin(FLOTASMCUT_MOTORIO_PORT,FLOTASMCUT_MOTORIO_OUT_PIN,GPIO_PIN_SET);

//	GPIO_InitStruct.Pin = STAMP_ACMOTORIO_OUT_PIN;
//  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
//  HAL_GPIO_Init(STAMP_ACMOTORIO_PORT, &GPIO_InitStruct);
//	HAL_GPIO_WritePin(STAMP_ACMOTORIO_PORT,STAMP_ACMOTORIO_OUT_PIN,GPIO_PIN_RESET);
//	
//	GPIO_InitStruct.Pin = PAGING_CONVEYER_MOTORIO_OUT_PIN;
//  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
//  HAL_GPIO_Init(PAGING_CONVEYER_MOTORIO_PORT, &GPIO_InitStruct);
//	HAL_GPIO_WritePin(PAGING_CONVEYER_MOTORIO_PORT,PAGING_CONVEYER_MOTORIO_OUT_PIN,GPIO_PIN_SET);
//	
//	GPIO_InitStruct.Pin = PAGING_HEATING_OUT_PIN;
//  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
//  HAL_GPIO_Init(PAGING_HEATING_PORT, &GPIO_InitStruct);
//	HAL_GPIO_WritePin(PAGING_HEATING_PORT,PAGING_HEATING_OUT_PIN,GPIO_PIN_SET);
//	

}

/**
  * 函数功能: 左滚刀电机输出
  * 输入函数: Dir,电机转动方向 STOP
	*						Per 百分比 0%-100%
  * 返 回 值: 无
  * 说    明: 无
  */
void leftHobMotorIO(uint8_t Dir, uint8_t Per)
{
	uint16_t Value,DValue;
	Value = BDCMOTOR_TIM_PERIOD  / PRECISION * Per;
	DValue = BDCMOTOR_TIM_PERIOD - Value;
		if(Dir == POSITIVE){
		 __HAL_TIM_SetCompare(&htimx_BDCMOTOR,TIM_CHANNEL_1,DValue);
		 HAL_GPIO_WritePin(LEFTHOB_MOTORIO_PORT,LEFTHOB_MOTORIO_OUT_PIN,GPIO_PIN_RESET);
		}else if(Dir == REVERSE){
			__HAL_TIM_SetCompare(&htimx_BDCMOTOR,TIM_CHANNEL_1,Value);
			HAL_GPIO_WritePin(LEFTHOB_MOTORIO_PORT,LEFTHOB_MOTORIO_OUT_PIN,GPIO_PIN_SET);
		}
		else{
		  __HAL_TIM_SetCompare(&htimx_BDCMOTOR,TIM_CHANNEL_1,0);
		  HAL_GPIO_WritePin(LEFTHOB_MOTORIO_PORT,LEFTHOB_MOTORIO_OUT_PIN,GPIO_PIN_SET);

		}
}

/**
  * 函数功能: 右滚刀电机输出
  * 输入函数: Dir,电机转动方向 STOP
	*						Per 百分比 0%-100%
  * 返 回 值: 无
  * 说    明: 无
  */
void rightHobMotorIO(uint8_t Dir, uint8_t Per)
{
	uint16_t Value,DValue;
	Value = BDCMOTOR_TIM_PERIOD  / PRECISION * Per;
	DValue = BDCMOTOR_TIM_PERIOD - Value;
		if(Dir == POSITIVE){
		  __HAL_TIM_SetCompare(&htimx_BDCMOTOR,TIM_CHANNEL_2,DValue);
			HAL_GPIO_WritePin(RIGHTHOB_MOTORIO_PORT,RIGHTHOB_MOTORIO_OUT_PIN,GPIO_PIN_RESET);
		}else if(Dir == REVERSE){
			__HAL_TIM_SetCompare(&htimx_BDCMOTOR,TIM_CHANNEL_2,Value);
			HAL_GPIO_WritePin(RIGHTHOB_MOTORIO_PORT,RIGHTHOB_MOTORIO_OUT_PIN,GPIO_PIN_SET);
			
		}
		else{
		  __HAL_TIM_SetCompare(&htimx_BDCMOTOR,TIM_CHANNEL_2,0);
	  HAL_GPIO_WritePin(RIGHTHOB_MOTORIO_PORT,RIGHTHOB_MOTORIO_OUT_PIN,GPIO_PIN_SET);
		}
}

/**
  * 函数功能: 左倒角电机输出
  * 输入函数: Dir,电机转动方向 STOP
	*						Per 百分比 0%-100%
  * 返 回 值: 无
  * 说    明: 无
  */
void chamferMotorIO(uint8_t Dir, uint8_t Per)
{
	uint16_t Value,DValue;
	Value = BDCMOTOR_TIM_PERIOD  / PRECISION * Per;
	DValue = BDCMOTOR_TIM_PERIOD - Value;
		if(Dir == POSITIVE){
			HAL_GPIO_WritePin(LEFTCHAMFER_MOTORIO_PORT,LEFTCHAMFER_MOTORIO_OUT_PIN,GPIO_PIN_RESET);
	  	__HAL_TIM_SetCompare(&htimx_BDCMOTOR,TIM_CHANNEL_3,DValue);
		}else if(Dir == REVERSE){
			__HAL_TIM_SetCompare(&htimx_BDCMOTOR,TIM_CHANNEL_3,Value);
			HAL_GPIO_WritePin(LEFTCHAMFER_MOTORIO_PORT,LEFTCHAMFER_MOTORIO_OUT_PIN,GPIO_PIN_SET);
		}
		else{
	    HAL_GPIO_WritePin(LEFTCHAMFER_MOTORIO_PORT,LEFTCHAMFER_MOTORIO_OUT_PIN,GPIO_PIN_SET);
			
			__HAL_TIM_SetCompare(&htimx_BDCMOTOR,TIM_CHANNEL_3,0);

		}
}

/**
  * 函数功能: 右倒角电机输出
  * 输入函数: Dir,电机转动方向 STOP
	*						Per 百分比 0%-100%
  * 返 回 值: 无
  * 说    明: 无
  */
void rightChamferMotorIO(uint8_t Dir, uint8_t Per)
{
	uint16_t Value,DValue;
	Value = BDCMOTOR_TIM_PERIOD  / PRECISION * Per;
	DValue = BDCMOTOR_TIM_PERIOD - Value;
		if(Dir == POSITIVE){
		 __HAL_TIM_SetCompare(&htimx_BDCMOTOR,TIM_CHANNEL_4,DValue);
	    HAL_GPIO_WritePin(RIGHTCHAMFER_MOTORIO_PORT,RIGHTCHAMFER_MOTORIO_OUT_PIN,GPIO_PIN_RESET);

		}else if(Dir == REVERSE){
		 __HAL_TIM_SetCompare(&htimx_BDCMOTOR,TIM_CHANNEL_4,Value);
			HAL_GPIO_WritePin(RIGHTCHAMFER_MOTORIO_PORT,RIGHTCHAMFER_MOTORIO_OUT_PIN,GPIO_PIN_SET);
		}
		else{
		 __HAL_TIM_SetCompare(&htimx_BDCMOTOR,TIM_CHANNEL_4,0);
	  HAL_GPIO_WritePin(RIGHTCHAMFER_MOTORIO_PORT,RIGHTCHAMFER_MOTORIO_OUT_PIN,GPIO_PIN_SET);
		}
}

/**
  * 函数功能: 
  * 输入函数: Dir,电机转动方向 STOP
	*						Per 百分比 0%-100%
  * 返 回 值: 无
  * 说    明: 无
  */
void flotasmCutMotorIO(uint8_t Dir, uint8_t Per)
{
	uint16_t Value,DValue;
	Value = COMMON_DCMOTOR_TIM_PERIOD  / PRECISION * Per;
	DValue = COMMON_DCMOTOR_TIM_PERIOD - Value;
		if(Dir == POSITIVE){
		__HAL_TIM_SetCompare(&htimx_COMMON_DCMOTOR,TIM_CHANNEL_3,DValue);

			HAL_GPIO_WritePin(FLOTASMCUT_MOTORIO_PORT,FLOTASMCUT_MOTORIO_OUT_PIN,GPIO_PIN_RESET);
		}else if(Dir == REVERSE){
			__HAL_TIM_SetCompare(&htimx_COMMON_DCMOTOR,TIM_CHANNEL_3,Value);

			HAL_GPIO_WritePin(FLOTASMCUT_MOTORIO_PORT,FLOTASMCUT_MOTORIO_OUT_PIN,GPIO_PIN_SET);

		}
		else{
	  	__HAL_TIM_SetCompare(&htimx_COMMON_DCMOTOR,TIM_CHANNEL_3,0);
	   	  HAL_GPIO_WritePin(FLOTASMCUT_MOTORIO_PORT,FLOTASMCUT_MOTORIO_OUT_PIN,GPIO_PIN_SET);

		}
}

void StampACMotorIO(uint8_t status)
{
	if(status == 1){
		HAL_GPIO_WritePin(STAMP_ACMOTORIO_PORT,STAMP_ACMOTORIO_OUT_PIN,GPIO_PIN_SET);
	}else{
			HAL_GPIO_WritePin(STAMP_ACMOTORIO_PORT,STAMP_ACMOTORIO_OUT_PIN,GPIO_PIN_RESET);

	}
}

/**
  * 函数功能: 分页机运输带电机输出
  * 输入函数: Dir,电机转动方向 STOP
	*						Per 百分比 0%-100%
  * 返 回 值: 无
  * 说    明: 2020.1.3 先用左滚刀的IO
  */
void pagingConveyerMotorIO(uint8_t Dir, uint8_t Per)
{
	/*
	uint16_t Value,DValue;
	Value = BDCMOTOR_TIM_PERIOD  / PRECISION * Per;
	DValue = BDCMOTOR_TIM_PERIOD - Value;
		if(Dir == POSITIVE){
		 __HAL_TIM_SetCompare(&htimx_COMMON_DCMOTOR,TIM_CHANNEL_1,DValue);
		 HAL_GPIO_WritePin(PAGING_CONVEYER_MOTORIO_PORT,PAGING_CONVEYER_MOTORIO_OUT_PIN,GPIO_PIN_RESET);
		}else if(Dir == REVERSE){
			__HAL_TIM_SetCompare(&htimx_COMMON_DCMOTOR,TIM_CHANNEL_1,Value);
			HAL_GPIO_WritePin(PAGING_CONVEYER_MOTORIO_PORT,PAGING_CONVEYER_MOTORIO_OUT_PIN,GPIO_PIN_SET);
		}
		else{
		  __HAL_TIM_SetCompare(&htimx_COMMON_DCMOTOR,TIM_CHANNEL_1,0);
		  HAL_GPIO_WritePin(PAGING_CONVEYER_MOTORIO_PORT,PAGING_CONVEYER_MOTORIO_OUT_PIN,GPIO_PIN_SET);
		}
	*/
	uint16_t Value,DValue;
	Value = BDCMOTOR_TIM_PERIOD  / PRECISION * Per;
	DValue = BDCMOTOR_TIM_PERIOD - Value;
		if(Dir == POSITIVE){
		 __HAL_TIM_SetCompare(&htimx_BDCMOTOR,TIM_CHANNEL_1,DValue);
		 HAL_GPIO_WritePin(LEFTHOB_MOTORIO_PORT,LEFTHOB_MOTORIO_OUT_PIN,GPIO_PIN_RESET);
		}else if(Dir == REVERSE){
			__HAL_TIM_SetCompare(&htimx_BDCMOTOR,TIM_CHANNEL_1,Value);
			HAL_GPIO_WritePin(LEFTHOB_MOTORIO_PORT,LEFTHOB_MOTORIO_OUT_PIN,GPIO_PIN_SET);
		}
		else{
		  __HAL_TIM_SetCompare(&htimx_BDCMOTOR,TIM_CHANNEL_1,0);
		  HAL_GPIO_WritePin(LEFTHOB_MOTORIO_PORT,LEFTHOB_MOTORIO_OUT_PIN,GPIO_PIN_SET);

		}
}
/**
  * 函数功能: 滚刀电机输出
  * 输入函数: Dir,电机转动方向 STOP
	*						Per 百分比 0%-100%
  * 返 回 值: 无
  * 说    明: 无
  */
void hobMotorIO(uint8_t Dir, uint8_t Per)
{
	uint16_t Value,DValue;
	Value = BDCMOTOR_TIM_PERIOD  / PRECISION * Per;
	DValue = BDCMOTOR_TIM_PERIOD - Value;
		if(Dir == POSITIVE){
		  __HAL_TIM_SetCompare(&htimx_BDCMOTOR,TIM_CHANNEL_2,DValue);
			HAL_GPIO_WritePin(RIGHTHOB_MOTORIO_PORT,RIGHTHOB_MOTORIO_OUT_PIN,GPIO_PIN_RESET);
		}else if(Dir == REVERSE){
			__HAL_TIM_SetCompare(&htimx_BDCMOTOR,TIM_CHANNEL_2,Value);
			HAL_GPIO_WritePin(RIGHTHOB_MOTORIO_PORT,RIGHTHOB_MOTORIO_OUT_PIN,GPIO_PIN_SET);
		}
		else{
		  __HAL_TIM_SetCompare(&htimx_BDCMOTOR,TIM_CHANNEL_2,0);
	  HAL_GPIO_WritePin(RIGHTHOB_MOTORIO_PORT,RIGHTHOB_MOTORIO_OUT_PIN,GPIO_PIN_SET);
		}
}
