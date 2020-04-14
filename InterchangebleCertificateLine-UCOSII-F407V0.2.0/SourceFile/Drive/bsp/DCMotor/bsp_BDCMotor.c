
/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "DCMotor/bsp_BDCMotor.h"
#include "Configure.h"

/* ˽�����Ͷ��� --------------------------------------------------------------*/
/* ˽�к궨�� ----------------------------------------------------------------*/
/* ˽�б��� ------------------------------------------------------------------*/
TIM_HandleTypeDef htimx_BDCMOTOR;
TIM_HandleTypeDef htimx_COMMON_DCMOTOR;
__IO uint16_t PWM_Duty=500;         // ռ�ձȣ�PWM_Duty/BDCMOTOR_TIM_PERIOD*100%

/* ��չ���� ------------------------------------------------------------------*/
/* ˽�к���ԭ�� --------------------------------------------------------------*/
/* ������ --------------------------------------------------------------------*/
/**
  * ��������: ������ʱ��Ӳ����ʼ������
  * �������: htim_base��������ʱ���������ָ��
  * �� �� ֵ: ��
  * ˵    ��: BDCMOTOR���GPIO��ʼ������,�ú�����HAL���ڲ�����.
  */
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
  /* BDCMOTOR���GPIO��ʼ������ */
  if(htim == &htimx_BDCMOTOR)
  {
	 BDCMOTOR_TIM_CH1_GPIO_CLK_ENABLE();
	 DCMOTOR_COMMON_TIM_CH1_GPIO_CLK_ENABLE();
  }
}

void initMotorPWM(void)
{
	/***htimx_BDCMOTOR TIM1***/
  TIM_ClockConfigTypeDef sClockSourceConfig;             // ��ʱ��ʱ��
  TIM_OC_InitTypeDef sConfigOC;                          // ��ʱ��ͨ���Ƚ����
  TIM_BreakDeadTimeConfigTypeDef  sBDTConfig;            // ��ʱ������ʱ��Ƚ����
  /* ������ʱ������ʱ��ʹ�� */
  BDCMOTOR_TIM_RCC_CLK_ENABLE();
	
  /* ��ʱ�������������� */
  htimx_BDCMOTOR.Instance = BDCMOTOR_TIMx;                                 // ��ʱ�����
  htimx_BDCMOTOR.Init.Prescaler = BDCMOTOR_TIM_PRESCALER;                  // ��ʱ��Ԥ��Ƶ��
  htimx_BDCMOTOR.Init.CounterMode = TIM_COUNTERMODE_UP;                  // �����������ϼ���
  htimx_BDCMOTOR.Init.Period = BDCMOTOR_TIM_PERIOD;                        // ��ʱ������
  htimx_BDCMOTOR.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;              // ʱ�ӷ�Ƶ
  htimx_BDCMOTOR.Init.RepetitionCounter = BDCMOTOR_TIM_REPETITIONCOUNTER;  // �ظ�������
  /* ��ʼ����ʱ���Ƚ�������� */
  HAL_TIM_PWM_Init(&htimx_BDCMOTOR);

  /* ��ʱ��ʱ��Դ���� */
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;       // ʹ���ڲ�ʱ��Դ
  HAL_TIM_ConfigClockSource(&htimx_BDCMOTOR, &sClockSourceConfig);

  /* ����ɲ������,������Ч��ƽ�ǵ͵�ƽ */
  sBDTConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE ;
  sBDTConfig.BreakPolarity = TIM_BREAKPOLARITY_LOW ;
  sBDTConfig.BreakState = TIM_BREAK_DISABLE ;
  sBDTConfig.DeadTime = 0 ;
  sBDTConfig.LockLevel = TIM_LOCKLEVEL_OFF ;
  sBDTConfig.OffStateIDLEMode= TIM_OSSI_DISABLE ;
  sBDTConfig.OffStateRunMode = TIM_OSSR_ENABLE ;
  HAL_TIMEx_ConfigBreakDeadTime(&htimx_BDCMOTOR,&sBDTConfig);

  /* ��ʱ���Ƚ�������� */
  sConfigOC.OCMode = TIM_OCMODE_PWM1;                  // �Ƚ����ģʽ��PWM1ģʽ
  sConfigOC.Pulse =  PWM_Duty;                         // ռ�ձ�
  sConfigOC.OCPolarity = TIM_OCPOLARITY_LOW;          // �������
// sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
 sConfigOC.OCNPolarity = TIM_OCNPOLARITY_LOW;        // ����ͨ���������
// sConfigOC.OCNPolarity = TIM_OCPOLARITY_HIGH;        // ����ͨ���������

  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;           // ����ģʽ
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;       // ���е�ƽ
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;     // ����ͨ�����е�ƽ
  HAL_TIM_PWM_ConfigChannel(&htimx_BDCMOTOR, &sConfigOC, TIM_CHANNEL_1);
	HAL_TIM_PWM_ConfigChannel(&htimx_BDCMOTOR, &sConfigOC, TIM_CHANNEL_2);
	HAL_TIM_PWM_ConfigChannel(&htimx_BDCMOTOR, &sConfigOC, TIM_CHANNEL_3);
	HAL_TIM_PWM_ConfigChannel(&htimx_BDCMOTOR, &sConfigOC, TIM_CHANNEL_4);
  HAL_TIM_Base_Start(&htimx_BDCMOTOR);
	
	
	
	
	
  /***htimx_COMMON_DCMOTOR TIM4***/
  /* ������ʱ������ʱ��ʹ�� */
  DCMOTOR_COMMON_TIM_RCC_CLK_ENABLE();
	  /* ��ʱ�������������� */
  htimx_COMMON_DCMOTOR.Instance = DCMOTOR_COMMON_TIMx;                                 // ��ʱ�����
  htimx_COMMON_DCMOTOR.Init.Prescaler = COMMON_DCMOTOR_TIM_PRESCALER;                  // ��ʱ��Ԥ��Ƶ��
  htimx_COMMON_DCMOTOR.Init.CounterMode = TIM_COUNTERMODE_UP;                  // �����������ϼ���
  htimx_COMMON_DCMOTOR.Init.Period = COMMON_DCMOTOR_TIM_PERIOD;                        // ��ʱ������
  htimx_COMMON_DCMOTOR.Init.ClockDivision= TIM_CLOCKDIVISION_DIV1;              // ʱ�ӷ�Ƶ
 // htimx_COMMON_DCMOTOR.Init.RepetitionCounter = BDCMOTOR_TIM_REPETITIONCOUNTER;  // �ظ�������
	HAL_TIM_PWM_Init(&htimx_COMMON_DCMOTOR);
	  /* ��ʱ��ʱ��Դ���� */
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;       // ʹ���ڲ�ʱ��Դ
  HAL_TIM_ConfigClockSource(&htimx_COMMON_DCMOTOR, &sClockSourceConfig);
 
	sConfigOC.OCMode = TIM_OCMODE_PWM1;                  // �Ƚ����ģʽ��PWM1ģʽ
  sConfigOC.Pulse =  PWM_Duty;                         // ռ�ձ�
  sConfigOC.OCPolarity = TIM_OCPOLARITY_LOW;          // �������
 // sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_LOW;        // ����ͨ���������
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;           // ����ģʽ
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;       // ���е�ƽ
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;     // ����ͨ�����е�ƽ
 
 HAL_TIM_PWM_ConfigChannel(&htimx_COMMON_DCMOTOR, &sConfigOC, TIM_CHANNEL_1);
	HAL_TIM_PWM_ConfigChannel(&htimx_COMMON_DCMOTOR, &sConfigOC, TIM_CHANNEL_2);
  HAL_TIM_PWM_ConfigChannel(&htimx_COMMON_DCMOTOR, &sConfigOC, TIM_CHANNEL_3);
  HAL_TIM_PWM_ConfigChannel(&htimx_COMMON_DCMOTOR, &sConfigOC, TIM_CHANNEL_3);

	HAL_TIM_Base_Start(&htimx_COMMON_DCMOTOR);
	
	
	
	
	
  HAL_TIM_PWM_Stop(&htimx_BDCMOTOR,TIM_CHANNEL_1);
	
  HAL_TIMEx_PWMN_Stop(&htimx_BDCMOTOR,TIM_CHANNEL_1);         // ֹͣ���

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
	
	
	
	 /**��ʼ��PWM**/
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
  * ��������: �����������
  * ���뺯��: Dir,���ת������ STOP
	*						Per �ٷֱ� 0%-100%
  * �� �� ֵ: ��
  * ˵    ��: ��
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
  * ��������: �ҹ���������
  * ���뺯��: Dir,���ת������ STOP
	*						Per �ٷֱ� 0%-100%
  * �� �� ֵ: ��
  * ˵    ��: ��
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
  * ��������: �󵹽ǵ�����
  * ���뺯��: Dir,���ת������ STOP
	*						Per �ٷֱ� 0%-100%
  * �� �� ֵ: ��
  * ˵    ��: ��
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
  * ��������: �ҵ��ǵ�����
  * ���뺯��: Dir,���ת������ STOP
	*						Per �ٷֱ� 0%-100%
  * �� �� ֵ: ��
  * ˵    ��: ��
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
  * ��������: 
  * ���뺯��: Dir,���ת������ STOP
	*						Per �ٷֱ� 0%-100%
  * �� �� ֵ: ��
  * ˵    ��: ��
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
  * ��������: ��ҳ�������������
  * ���뺯��: Dir,���ת������ STOP
	*						Per �ٷֱ� 0%-100%
  * �� �� ֵ: ��
  * ˵    ��: 2020.1.3 �����������IO
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
  * ��������: ����������
  * ���뺯��: Dir,���ת������ STOP
	*						Per �ٷֱ� 0%-100%
  * �� �� ֵ: ��
  * ˵    ��: ��
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
