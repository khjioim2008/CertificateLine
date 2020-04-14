/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "StepMotor/bsp_STEPMOTOR.h" 
#include <math.h>
#include <string.h>

/* ˽�����Ͷ��� --------------------------------------------------------------*/

/* ˽�к궨�� ----------------------------------------------------------------*/

/* ˽�б��� ------------------------------------------------------------------*/
TIM_HandleTypeDef htimx_STEPMOTOR;
TIM_HandleTypeDef cutMachine_STEPMOTOR; 

__IO uint16_t Toggle_Pulse = 3000;         // �Ƚ�������ڣ�ֵԽС���Ƶ��Խ��
__IO uint8_t i = 0;                         //�����������������������
__IO uint8_t iCut = 0;                         //�����������������������


// ���ƶ�����������


__IO uint32_t listStep_count[4] = {0,0,0,0};
bool listStopCmd[4]       = {False,False,False,False}; // ֽ��ֹͣ����

__IO uint32_t listCutStep_count[2] = {0,0};
bool listCutStepStopCmd[2]       = {False,False}; // ֽ��ֹͣ����

int32_t  stpdecel   = 5000;       // ��ͣ���ٶ�

speedRampData cerStepSrd[4] ={\
 {STOP,CW,0,0,0,0,0},   // �Ӽ������߱���
 {STOP,CW,0,0,0,0,0},
 {STOP,CW,0,0,0,0,0},
 {STOP,CW,0,0,0,0,0},
};

speedRampData cerCutStepSrd[2] ={\
 {STOP,CW,0,0,0,0,0},   // �Ӽ������߱���
 {STOP,CW,0,0,0,0,0},
};



/* ��ʱ�������������,��Ҫ�޸�ֱ����stepmotor.hͷ�ļ��޸ļ���*/
const Tim Timer[4]={\
{STEPMOTOR_TIM_PUL_PIN, STEPMOTOR_TIM_CHANNEL_x, STEPMOTOR_TIM_IT_CCx, STEPMOTOR_TIM_FLAG_CCx},
{SM_TIM_PUL_PIN, SM_TIM_CHANNEL_x, SM_STEP_TIM_IT_CCx, SM_STEP_TIM_FLAG_CCx},
{PM_FRICTION_STEP_TIM_PUL_PIN, PM_FRICTION_STEP_TIM_CHANNEL_x, PM_FRICTION_STEP_TIM_IT_CCx, PM_FRICTION_STEP_TIM_FLAG_CCx},
{PM_ROLLER_STEP_TIM_PUL_PIN, PM_ROLLER_STEP_TIM_CHANNEL_x, PM_ROLLER_STEP_TIM_IT_CCx, PM_ROLLER_STEP_TIM_FLAG_CCx},
};

/* ��ʱ�������������,��Ҫ�޸�ֱ����stepmotor.hͷ�ļ��޸ļ���*/
const Tim Timer9[2]={\
{CUT_CHAMFER_STEP_TIM_PUL_PIN, CUT_CHAMFER_STEP_TIM_CHANNEL_x, CUT_CHAMFER_STEP_TIM_IT_CCx, CUT_CHAMFER_STEP_TIM_FLAG_CCx},
{CUT_ROLLER_STEP_TIM_PUL_PIN, CUT_ROLLER_STEP_TIM_CHANNEL_x, CUT_ROLLER_STEP_TIM_IT_CCx, CUT_ROLLER_STEP_TIM_FLAG_CCx},
};

/* ���������������*/
const StepMotor Stepmotor[4]={\
{STEPMOTOR_ENA_PIN,					STEPMOTOR_DIR_PIN,								STEPMOTOR_DIR_PORT,       STEPMOTOR_ENA_PORT},
{SM_STEP_ENA_PIN,						SM_STEP_DIR_PIN,									SM_STEP_DIR_PORT,	        SM_STEP_ENA_PORT},
{PM_FRICTION_STEP_ENA_PIN,	PM_FRICTION_STEP_DIR_PIN,					PM_FRICTION_STEP_DIR_PORT,PM_FRICTION_STEP_ENA_PORT},
{PM_ROLLER_STEP_ENA_PIN,		PM_ROLLER_STEP_DIR_PIN,						PM_ROLLER_STEP_DIR_PORT,  PM_ROLLER_STEP_ENA_PORT},
};

/* ���������������*/
const CutStepMotor CutStepmotor[2]={\
{CUT_CHAMFER_STEP_ENA_PIN,CUT_CHAMFER_STEP_ENA_PORT},
{CUT_ROLLER_STEP_ENA_PIN,	 CUT_ROLLER_STEP_ENA_PORT},
};

__IO uint8_t  cerMotionStatus[4]   = {STOP,STOP,STOP,STOP};        // �Ƿ����˶���0��ֹͣ��1���˶�
__IO int32_t  pmStep_position[4]  = {0,0,0,0};           // ��ǰλ��

__IO uint8_t  cerCutMotionStatus[2]   = {STOP,STOP};        // �Ƿ����˶���0��ֹͣ��1���˶�
__IO int32_t  pmCutStep_position[2]  = {0,0};           // ��ǰλ��

/* ��չ���� ------------------------------------------------------------------*/

/* ˽�к���ԭ�� --------------------------------------------------------------*/

/* ������ --------------------------------------------------------------------*/

/**
  * ��������: ���������GPIO��ʼ������
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  */
static void STEPMOTOR_GPIO_Init(void)
{
	uint8_t i;
  GPIO_InitTypeDef GPIO_InitStruct; 
  
  /* ���Ŷ˿�ʱ��ʹ�� */
  STEPMOTOR_TIM_GPIO_CLK_ENABLE();
  STEPMOTOR_DIR_GPIO_CLK_ENABLE();
  STEPMOTOR_ENA_GPIO_CLK_ENABLE();
  
	 for(i=0; i<4; i++)
  {
		/* ���������������������� */
    GPIO_InitStruct.Pin = Timer[i].Pulse_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
		//GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF1_TIM1;
		HAL_GPIO_Init(PM_FRICTION_STEP_TIM_PUL_PORT, &GPIO_InitStruct);
    
    /* ���������������������� */
    CER_STEPMOTOR_DIR_FORWARD(i);//Ĭ������Ϊ˳ʱ�뷽��
    GPIO_InitStruct.Pin = Stepmotor[i].Dir_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
		//GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    //GPIO_InitStruct.Alternate = GPIO_AF0_TRACE;
    HAL_GPIO_Init(Stepmotor[i].Dir_Port, &GPIO_InitStruct);
  
    /* ���������������ʹ�ܿ��� */
    GPIO_InitStruct.Pin = Stepmotor[i].Ena_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
		//GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; //�и�����PP
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    //GPIO_InitStruct.Alternate = GPIO_AF0_TRACE;
    HAL_GPIO_Init(Stepmotor[i].Ena_Port, &GPIO_InitStruct);
		CER_STEPMOTOR_OUTPUT_ENABLE(i);
	}
	
//	GPIO_InitStruct.Pin = GPIO_PIN_13;
//	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//	HAL_GPIO_Init(GPIOC,&GPIO_InitStruct);
//	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_SET);

//	GPIO_InitStruct.Pin = GPIO_PIN_15;
//	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//	HAL_GPIO_Init(GPIOC,&GPIO_InitStruct);
//	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_15,GPIO_PIN_SET);
//	
//	GPIO_InitStruct.Pin = GPIO_PIN_3;
//	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//	HAL_GPIO_Init(GPIOF,&GPIO_InitStruct);
//	HAL_GPIO_WritePin(GPIOF,GPIO_PIN_3,GPIO_PIN_SET);
//			
//	GPIO_InitStruct.Pin = GPIO_PIN_1;
//	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//	HAL_GPIO_Init(GPIOF,&GPIO_InitStruct);
//	HAL_GPIO_WritePin(GPIOF,GPIO_PIN_1,GPIO_PIN_SET);
//	
}

/**
  * ��������: ��������ʱ����ʼ��
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  */
void STEPMOTOR_TIMx_Init(void)
{
		uint8_t i = 0;
  TIM_ClockConfigTypeDef sClockSourceConfig;             // ��ʱ��ʱ��
  TIM_OC_InitTypeDef sConfigOC;                          // ��ʱ��ͨ���Ƚ����
  
  STEPMOTOR_TIM_RCC_CLK_ENABLE();
	
  /* STEPMOTOR���GPIO��ʼ������ */
  STEPMOTOR_GPIO_Init();
  
  /* ��ʱ�������������� */
  htimx_STEPMOTOR.Instance = STEPMOTOR_TIMx;                          // ��ʱ�����
  htimx_STEPMOTOR.Init.Prescaler = STEPMOTOR_TIM_PRESCALER;           // ��ʱ��Ԥ��Ƶ��
  htimx_STEPMOTOR.Init.CounterMode = TIM_COUNTERMODE_UP;              // �����������ϼ���
  htimx_STEPMOTOR.Init.Period = STEPMOTOR_TIM_PERIOD;                 // ��ʱ������
  htimx_STEPMOTOR.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;          // ʱ�ӷ�Ƶ
  HAL_TIM_Base_Init(&htimx_STEPMOTOR);
	
	
  /* ��ʱ��ʱ��Դ���� */
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;       		// ʹ���ڲ�ʱ��Դ
  HAL_TIM_ConfigClockSource(&htimx_STEPMOTOR, &sClockSourceConfig);

  /* ��ʱ���Ƚ�������� */
  sConfigOC.OCMode = TIM_OCMODE_TOGGLE;                // �Ƚ����ģʽ����ת���
  sConfigOC.Pulse = 0xFFFF;                            // ������
  sConfigOC.OCPolarity = TIM_OCPOLARITY_LOW;           // �������
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_LOW;         // ����ͨ���������
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;           // ����ģʽ
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;       // ���е�ƽ
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;     // ����ͨ�����е�ƽ
  
	for (i=0; i <4; i++)
	{
		HAL_TIM_OC_Stop_IT (&htimx_STEPMOTOR,Timer[i].Channel); 
    HAL_TIM_OC_ConfigChannel(&htimx_STEPMOTOR, &sConfigOC, Timer[i].Channel);
		TIM_CCxChannelCmd(STEPMOTOR_TIMx, Timer[i].Channel, TIM_CCx_DISABLE);
	}
	
  /* ���ö�ʱ���ж����ȼ���ʹ�� */
  HAL_NVIC_SetPriority(STEPMOTOR_TIMx_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(STEPMOTOR_TIMx_IRQn);
  
  /* Enable the main output */
  __HAL_TIM_MOE_ENABLE(&htimx_STEPMOTOR);  
  HAL_TIM_Base_Start(&htimx_STEPMOTOR);// ʹ�ܶ�ʱ��
}

static void PagingMachinePIDStep_GPIO_Init(void)
{
	
	
	  GPIO_InitTypeDef GPIO_InitStruct; 
	//CUT_CHAMFER_STEP_TIM_GPIO_CLK_ENABLE();
	CUT_ROLLER_STEP_TIM_RCC_CLK_ENABLE();
	//CUT_CHAMFER_STEP_ENA_GPIO_CLK_ENABLE();
	CUT_ROLLER_STEP_ENA_GPIO_CLK_ENABLE();
	
			/* ���������������������� */
    GPIO_InitStruct.Pin = CUT_CHAMFER_STEP_TIM_PUL_PIN;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
		//GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = CUT_CHAMFER_STEP_GPIO_AFx_TIMx;
		HAL_GPIO_Init(CUT_CHAMFER_STEP_TIM_PUL_PORT, &GPIO_InitStruct);
 
			/* ���������������������� */
    GPIO_InitStruct.Pin = CUT_ROLLER_STEP_TIM_PUL_PIN;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
		//GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = CUT_ROLLER_STEP_GPIO_AFx_TIMx;
		HAL_GPIO_Init(CUT_ROLLER_STEP_TIM_PUL_PORT, &GPIO_InitStruct);
    	
	    /* ���������������ʹ�ܿ��� */
    GPIO_InitStruct.Pin = CUT_CHAMFER_STEP_ENA_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
		//GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; //�и�����PP
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    //GPIO_InitStruct.Alternate = GPIO_AF0_TRACE;
    HAL_GPIO_Init(CUT_CHAMFER_STEP_ENA_PORT, &GPIO_InitStruct);
		
			    /* ���������������ʹ�ܿ��� */
    GPIO_InitStruct.Pin = CUT_ROLLER_STEP_ENA_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
		//GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; //�и�����PP
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    //GPIO_InitStruct.Alternate = GPIO_AF0_TRACE;
    HAL_GPIO_Init(CUT_ROLLER_STEP_ENA_PORT, &GPIO_InitStruct);
		
}

void cutMachineStep_TIM9_Init(void)
{
  TIM_ClockConfigTypeDef sClockSourceConfig;             // ��ʱ��ʱ��
  TIM_OC_InitTypeDef sConfigOC;                          // ��ʱ��ͨ���Ƚ����
    
	CUT_MACHINE_TIM_RCC_CLK_ENABLE();
	/* STEPMOTOR���GPIO��ʼ������ */
	 PagingMachinePIDStep_GPIO_Init();

  /* ��ʱ�������������� */
  cutMachine_STEPMOTOR.Instance = CUT_MACHINE_TIMx;                                 // ��ʱ�����
  cutMachine_STEPMOTOR.Init.Prescaler = STEPMOTOR_TIM_PRESCALER;                  // ��ʱ��Ԥ��Ƶ��
  cutMachine_STEPMOTOR.Init.CounterMode = TIM_COUNTERMODE_UP;                     // �����������ϼ���
  cutMachine_STEPMOTOR.Init.Period = STEPMOTOR_TIM_PERIOD;                        // ��ʱ������
  cutMachine_STEPMOTOR.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;                 // ʱ�ӷ�Ƶ
 // cutMachine_STEPMOTOR.Init.RepetitionCounter = STEPMOTOR_TIM_REPETITIONCOUNTER;  // �ظ�������
  HAL_TIM_Base_Init(&cutMachine_STEPMOTOR);

  /* ��ʱ��ʱ��Դ���� */
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;                 // ʹ���ڲ�ʱ��Դ
  HAL_TIM_ConfigClockSource(&cutMachine_STEPMOTOR, &sClockSourceConfig);

  /* ��ʱ���Ƚ�������� */
  sConfigOC.OCMode = TIM_OCMODE_TOGGLE;                // �Ƚ����ģʽ����ת���
  sConfigOC.Pulse = 0xFFFF;                      // ������
  sConfigOC.OCPolarity = TIM_OCPOLARITY_LOW;           // �������
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_LOW;         // ����ͨ���������
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;           // ����ģʽ
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;       // ���е�ƽ
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;     // ����ͨ�����е�ƽ
  
	//HAL_TIM_OC_ConfigChannel(&cutMachine_STEPMOTOR, &sConfigOC, PAGINGPID_TIM_CHANNEL_x);
	for (i=0; i <2; i++)
	{
		HAL_TIM_OC_Stop_IT (&cutMachine_STEPMOTOR,Timer9[i].Channel); 
    HAL_TIM_OC_ConfigChannel(&cutMachine_STEPMOTOR, &sConfigOC, Timer9[i].Channel);
		TIM_CCxChannelCmd(CUT_MACHINE_TIMx, Timer9[i].Channel, TIM_CCx_DISABLE);
	}
	
  
  /* ���ö�ʱ���ж����ȼ���ʹ�� */
  HAL_NVIC_SetPriority(CUT_MACHINE_TIMx_IRQn, 1, 1);
  HAL_NVIC_EnableIRQ(CUT_MACHINE_TIMx_IRQn);
  
  /* Enable the main output */
  __HAL_TIM_MOE_ENABLE(&cutMachine_STEPMOTOR);  
  HAL_TIM_Base_Start(&cutMachine_STEPMOTOR);// ʹ�ܶ�ʱ��

}
/**
  * ��������: ������ʱ��Ӳ����ʼ������
  * �������: htim_base��������ʱ���������ָ��
  * �� �� ֵ: ��
  * ˵    ��: �ú�����HAL���ڲ�����
  */
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* htim_base)
{

  if(htim_base->Instance==STEPMOTOR_TIMx)
  {
    /* ������ʱ������ʱ��ʹ�� */
    STEPMOTOR_TIM_RCC_CLK_ENABLE();
  }
	  if(htim_base->Instance==CUT_MACHINE_TIMx)
		{
		CUT_MACHINE_TIM_RCC_CLK_ENABLE();
		}																													
}	

/**
  * ��������: ������ʱ��Ӳ������ʼ������
  * �������: htim_base��������ʱ���������ָ��
  * �� �� ֵ: ��
  * ˵    ��: �ú�����HAL���ڲ�����
  */
void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* htim_base)
{

  if(htim_base->Instance==STEPMOTOR_TIMx)
  {
    /* ������ʱ������ʱ�ӽ��� */
    STEPMOTOR_TIM_RCC_CLK_DISABLE();
    HAL_GPIO_DeInit(STEPMOTOR_TIM_PUL_PORT,STEPMOTOR_TIM_PUL_PIN);
    HAL_GPIO_DeInit(STEPMOTOR_DIR_PORT,STEPMOTOR_DIR_PIN);
    HAL_GPIO_DeInit(STEPMOTOR_ENA_PORT,STEPMOTOR_ENA_PIN);
    
    HAL_NVIC_DisableIRQ(STEPMOTOR_TIMx_IRQn);
  }
	  if(htim_base->Instance == CUT_MACHINE_TIMx)
  {
		CUT_MACHINE_TIM_RCC_CLK_ENABLE();
		
		HAL_GPIO_DeInit(CUT_CHAMFER_STEP_TIM_PUL_PORT,CUT_CHAMFER_STEP_TIM_PUL_PIN);
    HAL_GPIO_DeInit(CUT_CHAMFER_STEP_ENA_PORT,CUT_CHAMFER_STEP_ENA_PIN);
    
		HAL_GPIO_DeInit(CUT_ROLLER_STEP_TIM_PUL_PORT,CUT_ROLLER_STEP_TIM_PUL_PIN);
    HAL_GPIO_DeInit(CUT_ROLLER_STEP_ENA_PORT,CUT_ROLLER_STEP_ENA_PIN);
 		
		HAL_NVIC_DisableIRQ(CUT_MACHINE_TIMx_IRQn);
		
		
	}
} 

/**
  * ��������: ���λ���˶����˶������Ĳ���
  * �������: Axis   ���
  *           step   �ƶ��Ĳ��� (����Ϊ˳ʱ�룬����Ϊ��ʱ��).
  *           accel  ���ٶ�,ʵ��ֵΪaccel*0.025*rad/sec^2
  *           decel  ���ٶ�,ʵ��ֵΪdecel*0.025*rad/sec^2
  *           speed  ����ٶ�,ʵ��ֵΪspeed*0.05*rad/sec
  * �� �� ֵ: ��
  * ˵    ��: �Ը����Ĳ����ƶ�����������ȼ��ٵ�����ٶȣ�Ȼ���ں���λ�ÿ�ʼ
  *           ������ֹͣ��ʹ�������˶�����Ϊָ���Ĳ���������Ӽ��ٽ׶κ̲ܶ���
  *           �ٶȺ������ǻ�û�ﵽ����ٶȾ�Ҫ��ʼ����
  */
void STEPMOTOR_CerAxisMoveRel(uint8_t Axis,int32_t step,uint32_t accel, uint32_t decel, uint32_t speed)
{  
		__IO uint16_t tim_count;//��ȡ��ʱ���ļ���ֵ
  // �ﵽ����ٶ�ʱ�Ĳ���
	__IO uint32_t max_s_lim;
  // ����Ҫ��ʼ���ٵĲ������������û�дﵽ����ٶȣ�
	__IO uint32_t accel_lim;
  
	//�ڵ���˶�����һ�㲻���ټ�������,�����ڻ�ԭ�������
//	if((cerMotionStatus[Axis] != STOP)&&(ZeroStep[Axis] == IDLE))	

	//	if(cerMotionStatus[Axis] != STOP)
//	{
//		return ;
//	}
	
  	/* �����趨 */
	if(step < 0) // ����Ϊ����
	{
		cerStepSrd[Axis].dir = CCW; // ��ʱ�뷽����ת
		CER_STEPMOTOR_DIR_REVERSAL(Axis);
		step = -step;   // ��ȡ��������ֵ
	}
	else
	{
		cerStepSrd[Axis].dir = CW; // ˳ʱ�뷽����ת
		CER_STEPMOTOR_DIR_FORWARD(Axis);
	}

	if(step == 1)    // ����Ϊ1
	{
		cerStepSrd[Axis].accel_count = -1;   // ֻ�ƶ�һ��
		cerStepSrd[Axis].run_state = DECEL;  // ����״̬.
		cerStepSrd[Axis].step_delay = 1000;	// ����ʱ	
	}
		else if(step != 0)  // ���Ŀ���˶�������Ϊ0
	{

		// ��������ٶȼ���, ����õ�min_delay���ڶ�ʱ���ļ�������ֵ��
		// min_delay = (alpha / tt)/ w
		cerStepSrd[Axis].min_delay = (int32_t)(A_T_x10/speed);

		// ͨ�������һ��(c0) �Ĳ�����ʱ���趨���ٶȣ�����accel��λΪ0.1rad/sec^2
		// step_delay = 1/tt * sqrt(2*alpha/accel)
		// step_delay = ( tfreq*0.676/10 )*10 * sqrt( (2*alpha*100000) / (accel*10) )/100
		cerStepSrd[Axis].step_delay = (int32_t)((T1_FREQ_148 * sqrt(A_SQ / accel))/10);//C0,��ʼ�ٶȵĶ�ʱ��ֵ
			

		/*����Ӽ�����Ҫ�Ĳ���*/
		// ������ٲ�֮��ﵽ����ٶȵ�����
		// max_s_lim = speed^2 / (2*alpha*accel)
		max_s_lim = (uint32_t)(speed*speed/(A_x200*accel/10));
		// ����ﵽ����ٶ�С��0.5�������ǽ���������Ϊ0
		// ��ʵ�����Ǳ����ƶ�����һ�����ܴﵽ��Ҫ���ٶ�
		if(max_s_lim == 0)
		{
		  max_s_lim = 1;
		}
			
		// ������ٲ�֮�����Ǳ��뿪ʼ����
		// n1 = (n1+n2)decel / (accel + decel)
		accel_lim = (uint32_t)(step*decel/(accel+decel));
		// ���Ǳ����������1�����ܲ��ܿ�ʼ����.
		if(accel_lim == 0)
		{
		  accel_lim = 1;
		}
		// ʹ�������������ǿ��Լ�������ٽ׶β���
		if(accel_lim <= max_s_lim)
		{
		  cerStepSrd[Axis].decel_val = accel_lim - step;
		}
		else{
		  cerStepSrd[Axis].decel_val = -(max_s_lim*accel/decel);
		}
		// ��ֻʣ��һ�����Ǳ������
		if(cerStepSrd[Axis].decel_val == 0)
		{
		  cerStepSrd[Axis].decel_val = -1;
		}

		// ���㿪ʼ����ʱ�Ĳ���
		cerStepSrd[Axis].decel_start = step + cerStepSrd[Axis].decel_val;

		// �������ٶȺ��������ǾͲ���Ҫ���м����˶�
		if(cerStepSrd[Axis].step_delay <= cerStepSrd[Axis].min_delay)
		{
			cerStepSrd[Axis].step_delay = cerStepSrd[Axis].min_delay;
			cerStepSrd[Axis].run_state = RUN;
		}
		else
		{
			cerStepSrd[Axis].run_state = ACCEL;
		}    
		// ��λ���ٶȼ���ֵ
		cerStepSrd[Axis].accel_count = 0;
	}
	
	cerMotionStatus[Axis] = 1;
	tim_count=__HAL_TIM_GET_COUNTER(&htimx_STEPMOTOR);
 	__HAL_TIM_SET_COMPARE(&htimx_STEPMOTOR,Timer[Axis].Channel,tim_count + cerStepSrd[Axis].step_delay); // ���ö�ʱ���Ƚ�ֵ
  HAL_TIM_OC_Start_IT (&htimx_STEPMOTOR,Timer[Axis].Channel);
	TIM_CCxChannelCmd(STEPMOTOR_TIMx, Timer[Axis].Channel, TIM_CCx_ENABLE);// ʹ�ܶ�ʱ��ͨ�� 
	CER_STEPMOTOR_OUTPUT_ENABLE(Axis);
}


void STEPMOTOR_CerAxisMoveRel_CutTim9(uint8_t Axis,int32_t step,uint32_t accel, uint32_t decel, uint32_t speed)
{  
		__IO uint16_t tim_count_cut;//��ȡ��ʱ���ļ���ֵ
  // �ﵽ����ٶ�ʱ�Ĳ���
	__IO uint32_t max_s_lim_cut;
  // ����Ҫ��ʼ���ٵĲ������������û�дﵽ����ٶȣ�
	__IO uint32_t accel_lim_cut;
  
	//�ڵ���˶�����һ�㲻���ټ�������,�����ڻ�ԭ�������
//	if((cerMotionStatus[Axis] != STOP)&&(ZeroStep[Axis] == IDLE))	

	//	if(cerMotionStatus[Axis] != STOP)
//	{
//		return ;
//	}
	
  	/* �����趨 */
	if(step < 0) // ����Ϊ����
	{
		cerCutStepSrd[Axis].dir = CCW; // ��ʱ�뷽����ת
		//CER_STEPMOTOR_DIR_REVERSAL(Axis);
		step = -step;   // ��ȡ��������ֵ
	}
	else
	{
		cerCutStepSrd[Axis].dir = CW; // ˳ʱ�뷽����ת
		//CER_STEPMOTOR_DIR_FORWARD(Axis);
	}

	if(step == 1)    // ����Ϊ1
	{
		cerCutStepSrd[Axis].accel_count = -1;   // ֻ�ƶ�һ��
		cerCutStepSrd[Axis].run_state = DECEL;  // ����״̬.
		cerCutStepSrd[Axis].step_delay = 1000;	// ����ʱ	
	}
		else if(step != 0)  // ���Ŀ���˶�������Ϊ0
	{

		// ��������ٶȼ���, ����õ�min_delay���ڶ�ʱ���ļ�������ֵ��
		// min_delay = (alpha / tt)/ w
		cerCutStepSrd[Axis].min_delay = (int32_t)(A_T_x10/speed);

		// ͨ�������һ��(c0) �Ĳ�����ʱ���趨���ٶȣ�����accel��λΪ0.1rad/sec^2
		// step_delay = 1/tt * sqrt(2*alpha/accel)
		// step_delay = ( tfreq*0.676/10 )*10 * sqrt( (2*alpha*100000) / (accel*10) )/100
		cerCutStepSrd[Axis].step_delay = (int32_t)((T1_FREQ_148 * sqrt(A_SQ / accel))/10);//C0,��ʼ�ٶȵĶ�ʱ��ֵ
			

		/*����Ӽ�����Ҫ�Ĳ���*/
		// ������ٲ�֮��ﵽ����ٶȵ�����
		// max_s_lim = speed^2 / (2*alpha*accel)
		max_s_lim_cut = (uint32_t)(speed*speed/(A_x200*accel/10));
		// ����ﵽ����ٶ�С��0.5�������ǽ���������Ϊ0
		// ��ʵ�����Ǳ����ƶ�����һ�����ܴﵽ��Ҫ���ٶ�
		if(max_s_lim_cut == 0)
		{
		  max_s_lim_cut = 1;
		}
			
		// ������ٲ�֮�����Ǳ��뿪ʼ����
		// n1 = (n1+n2)decel / (accel + decel)
		accel_lim_cut = (uint32_t)(step*decel/(accel+decel));
		// ���Ǳ����������1�����ܲ��ܿ�ʼ����.
		if(accel_lim_cut == 0)
		{
		  accel_lim_cut = 1;
		}
		// ʹ�������������ǿ��Լ�������ٽ׶β���
		if(accel_lim_cut <= max_s_lim_cut)
		{
		  cerCutStepSrd[Axis].decel_val = accel_lim_cut - step;
		}
		else{
		  cerCutStepSrd[Axis].decel_val = -(max_s_lim_cut*accel/decel);
		}
		// ��ֻʣ��һ�����Ǳ������
		if(cerCutStepSrd[Axis].decel_val == 0)
		{
		  cerCutStepSrd[Axis].decel_val = -1;
		}

		// ���㿪ʼ����ʱ�Ĳ���
		cerCutStepSrd[Axis].decel_start = step + cerCutStepSrd[Axis].decel_val;

		// �������ٶȺ��������ǾͲ���Ҫ���м����˶�
		if(cerCutStepSrd[Axis].step_delay <= cerCutStepSrd[Axis].min_delay)
		{
			cerCutStepSrd[Axis].step_delay = cerCutStepSrd[Axis].min_delay;
			cerCutStepSrd[Axis].run_state = RUN;
		}
		else
		{
			cerCutStepSrd[Axis].run_state = ACCEL;
		}    
		// ��λ���ٶȼ���ֵ
		cerCutStepSrd[Axis].accel_count = 0;
	}
	
	cerCutMotionStatus[Axis] = 1;
	tim_count_cut=__HAL_TIM_GET_COUNTER(&cutMachine_STEPMOTOR);
	__HAL_TIM_SET_COMPARE(&cutMachine_STEPMOTOR,Timer9[Axis].Channel,tim_count_cut + cerCutStepSrd[Axis].step_delay); // ���ö�ʱ���Ƚ�ֵ
  HAL_TIM_OC_Start_IT (&cutMachine_STEPMOTOR,Timer9[Axis].Channel);
	TIM_CCxChannelCmd(CUT_MACHINE_TIMx, Timer9[Axis].Channel, TIM_CCx_ENABLE);// ʹ�ܶ�ʱ��ͨ�� 
	CER_CUT_STEPMOTOR_OUTPUT_ENABLE(Axis);
}


void STEPMOTOR_SM_AxisMoveRel(uint8_t Axis,int32_t step,uint32_t accel, uint32_t decel, uint32_t speed)
{
	STEPMOTOR_CerAxisMoveRel(Axis,step,accel,decel,speed);
	cerStepSrd[Axis].run_state = RUN;
}

/**
  * ��������: ��ʱ���жϻص�����
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ʵ�ּӼ��ٹ���
  */
void HAL_TIM_OC_Callback(uint8_t Axis)
{
  __IO static uint16_t tim_count = 0;
  __IO  uint32_t new_step_delay = 0;
  __IO static uint8_t  i[4]={0,0,0,0};
	__IO static uint16_t last_accel_delay[4] = {0,0,0,0};
	// ���ƶ�����������
	__IO static uint32_t step_count[4] = {0,0,0,0};
	// ��¼new_step_delay�е������������һ������ľ���
	__IO static int32_t rest[4] = {0,0,0,0};
	//��ʱ��ʹ�÷�תģʽ����Ҫ���������жϲ����һ����������

  // ���ñȽ�ֵ
  tim_count = __HAL_TIM_GET_COUNTER(&htimx_STEPMOTOR);
  __HAL_TIM_SET_COMPARE(&htimx_STEPMOTOR,Timer[Axis].Channel,tim_count+cerStepSrd[Axis].step_delay);

  i[Axis]++;       // ��ʱ���жϴ�������ֵ
  if(i[Axis] == 2) // 2�Σ�˵���Ѿ����һ����������
  {
    i[Axis] = 0;   // ���㶨ʱ���жϴ�������ֵ

    switch(cerStepSrd[Axis].run_state) // �Ӽ������߽׶�
    {
      case STOP:
    
        // �ر�ͨ��
        TIM_CCxChannelCmd(STEPMOTOR_TIMx,Timer[Axis].Channel,TIM_CCx_DISABLE);       
        __HAL_TIM_CLEAR_FLAG(&htimx_STEPMOTOR, Timer[Axis].Flag_CCx);
        HAL_TIM_OC_Stop_IT (&htimx_STEPMOTOR,Timer[Axis].Channel);
       // CER_STEPMOTOR_OUTPUT_DISABLE(Axis);
      
        cerMotionStatus[Axis] = STOP;  		//  ���Ϊֹͣ״̬
        step_count[Axis] = 0;  // ���㲽��������
			  rest[Axis] = 0;        // ������ֵ
        last_accel_delay[Axis] = 0;
        cerStepSrd[Axis].accel_count = 0;
        cerStepSrd[Axis].step_delay = 0;
        cerStepSrd[Axis].min_delay = 0;
        break;
      case ACCEL:
        step_count[Axis] ++;      		// ������1
				listStep_count[Axis] ++;  
			if(cerStepSrd[Axis].dir == CW)
        {		  	
          pmStep_position[Axis]++; 	  // ����λ�ü�1
        }
        else
        {
          pmStep_position[Axis]--; 	  // ����λ�ü�1
        }
        cerStepSrd[Axis].accel_count++; 	  // ���ټ���ֵ��1
        
        new_step_delay = cerStepSrd[Axis].step_delay - (((2 *cerStepSrd[Axis].step_delay) + rest[Axis])/(4 * cerStepSrd[Axis].accel_count + 1));//������(��)һ����������(ʱ����)
        rest[Axis] = ((2 * cerStepSrd[Axis].step_delay)+rest[Axis])%(4 * cerStepSrd[Axis].accel_count + 1);// �����������´μ��㲹���������������
        
        if(step_count[Axis] >= cerStepSrd[Axis].decel_start || listStopCmd[Axis])		// ����Ƿ�Ӧ�ÿ�ʼ����
        {
					if(listStopCmd[Axis]){
					 listStopCmd[Axis] = False;
						cerStepSrd[Axis].accel_count = T1_FREQ/2/stpdecel*T1_FREQ/cerStepSrd[Axis].step_delay/cerStepSrd[Axis].step_delay*-1;
					}else{
					cerStepSrd[Axis].accel_count = cerStepSrd[Axis].decel_val; 	// ���ټ���ֵΪ���ٽ׶μ���ֵ�ĳ�ʼֵ
					}
          cerStepSrd[Axis].run_state = DECEL;           	      // �¸����������ٽ׶�
        }
        else if(new_step_delay <= cerStepSrd[Axis].min_delay)  // ����Ƿ񵽴�����������ٶ�
        {
          cerStepSrd[Axis].accel_count = cerStepSrd[Axis].decel_val; 	// ���ټ���ֵΪ���ٽ׶μ���ֵ�ĳ�ʼֵ
          last_accel_delay[Axis] = new_step_delay; 	// ������ٹ��������һ����ʱ���������ڣ�
          new_step_delay = cerStepSrd[Axis].min_delay;    	// ʹ��min_delay����Ӧ����ٶ�speed��
          rest[Axis] = 0;                          	// ������ֵ
          cerStepSrd[Axis].run_state = RUN;               	// ����Ϊ��������״̬
        }
        last_accel_delay[Axis] = new_step_delay; 	  // ������ٹ��������һ����ʱ���������ڣ�
        break;
        
      case RUN:
        step_count[Axis]++; 		 // ������1
        listStep_count[Axis] ++; 
			if(cerStepSrd[Axis].dir==CW)
        {	  	
          pmStep_position[Axis]++; // ����λ�ü�1
        }
        else
        {
          pmStep_position[Axis]--; // ����λ�ü�1
        }
            
        new_step_delay = cerStepSrd[Axis].min_delay;       // ʹ��min_delay����Ӧ����ٶ�speed��
            
        if(step_count[Axis] >= cerStepSrd[Axis].decel_start|| listStopCmd[Axis])   // ��Ҫ��ʼ����
        {
					if(listStopCmd[Axis]){
						listStopCmd[Axis] = False;
						cerStepSrd[Axis].accel_count = T1_FREQ/2/stpdecel*T1_FREQ/cerStepSrd[Axis].step_delay/cerStepSrd[Axis].step_delay*-1;
					}else{
          cerStepSrd[Axis].accel_count = cerStepSrd[Axis].decel_val;  // ���ٲ�����Ϊ���ټ���ֵ
          }
					new_step_delay = last_accel_delay[Axis];// �ӽ׶�������ʱ��Ϊ���ٽ׶ε���ʼ��ʱ(��������)
          cerStepSrd[Axis].run_state = DECEL;            // ״̬�ı�Ϊ����
        }
        break;
        
      case DECEL:
        step_count[Axis]++; 		 // ������1
       listStep_count[Axis] ++; 
			if(cerStepSrd[Axis].dir == CW)
        {		  	
          pmStep_position[Axis]++; // ����λ�ü�1
        }
        else
        {
          pmStep_position[Axis]--; // ����λ�ü�1
        }
        cerStepSrd[Axis].accel_count++;
        new_step_delay = cerStepSrd[Axis].step_delay - (((2 * cerStepSrd[Axis].step_delay) + rest[Axis])/(4 * cerStepSrd[Axis].accel_count + 1)); //������(��)һ����������(ʱ����)
        rest[Axis] = ((2 * cerStepSrd[Axis].step_delay)+rest[Axis])%(4 * cerStepSrd[Axis].accel_count + 1);// �����������´μ��㲹���������������
				if(listStopCmd[Axis]){
					listStopCmd[Axis] = False;
					cerStepSrd[Axis].accel_count = T1_FREQ/2/stpdecel*T1_FREQ/cerStepSrd[Axis].step_delay/cerStepSrd[Axis].step_delay*-1;
				}
        //����Ƿ�Ϊ���һ��
        if(cerStepSrd[Axis].accel_count >= 0)
        {
            cerStepSrd[Axis].run_state = STOP;
        }
				if(cerStepSrd[Axis].medle_delay != 0) 
          {
            if(new_step_delay >= cerStepSrd[Axis].medle_delay)		//����Ƿ񵽴������ٶ�
            {
              cerStepSrd[Axis].min_delay = cerStepSrd[Axis].medle_delay;//װ�������ٶ�
              new_step_delay = cerStepSrd[Axis].medle_delay;     //�������ٶ�Ϊ�¸���ʱ
              cerStepSrd[Axis].decel_start = UINT32_MAX;       //�ڴﵽdecel_start֮ǰ�������� 
              rest[Axis] = 0;       
              cerStepSrd[Axis].run_state = RUN;
            }		
          }
        break;
      default :break;
    }      
    if(new_step_delay>0xFFFF)
      new_step_delay=0xFFFF;
    cerStepSrd[Axis].step_delay = new_step_delay; // Ϊ�¸�(�µ�)��ʱ(��������)��ֵ
  }
}

void HAL_TIM_OC_Callback_TIM9(uint8_t Axis)
{
  __IO static uint16_t tim_count_cut = 0;
  __IO  uint32_t new_step_delay_cut = 0;
  __IO static uint8_t  i_cut[2]={0,0};
	__IO static uint16_t last_accel_delay_cut[2] = {0,0};
	// ���ƶ�����������
	__IO static uint32_t step_count_cut[2] = {0,0};
	// ��¼new_step_delay�е������������һ������ľ���
	__IO static int32_t rest_cut[2] = {0,0};
	//��ʱ��ʹ�÷�תģʽ����Ҫ���������жϲ����һ����������

  // ���ñȽ�ֵ
  tim_count_cut = __HAL_TIM_GET_COUNTER(&cutMachine_STEPMOTOR);
  __HAL_TIM_SET_COMPARE(&cutMachine_STEPMOTOR,Timer9[Axis].Channel,tim_count_cut+cerCutStepSrd[Axis].step_delay);

  i_cut[Axis]++;       // ��ʱ���жϴ�������ֵ
  if(i_cut[Axis] == 2) // 2�Σ�˵���Ѿ����һ����������
  {
    i_cut[Axis] = 0;   // ���㶨ʱ���жϴ�������ֵ

    switch(cerCutStepSrd[Axis].run_state) // �Ӽ������߽׶�
    {
      case STOP:
    
        // �ر�ͨ��
        TIM_CCxChannelCmd(CUT_MACHINE_TIMx,Timer9[Axis].Channel,TIM_CCx_DISABLE);       
        __HAL_TIM_CLEAR_FLAG(&cutMachine_STEPMOTOR, Timer9[Axis].Flag_CCx);
        HAL_TIM_OC_Stop_IT (&cutMachine_STEPMOTOR,Timer9[Axis].Channel);
       // CER_STEPMOTOR_OUTPUT_DISABLE(Axis);
      
        cerCutMotionStatus[Axis] = STOP;  		//  ���Ϊֹͣ״̬
        step_count_cut[Axis] = 0;  // ���㲽��������
			  rest_cut[Axis] = 0;        // ������ֵ
        last_accel_delay_cut[Axis] = 0;
        cerCutStepSrd[Axis].accel_count = 0;
        cerCutStepSrd[Axis].step_delay = 0;
        cerCutStepSrd[Axis].min_delay = 0;
        break;
      case ACCEL:
        step_count_cut[Axis] ++;      		// ������1
				listCutStep_count[Axis] ++;  
			if(cerCutStepSrd[Axis].dir == CW)
        {		  	
          pmCutStep_position[Axis]++; 	  // ����λ�ü�1
        }
        else
        {
          pmCutStep_position[Axis]--; 	  // ����λ�ü�1
        }
        cerCutStepSrd[Axis].accel_count++; 	  // ���ټ���ֵ��1
        
        new_step_delay_cut = cerCutStepSrd[Axis].step_delay - (((2 *cerCutStepSrd[Axis].step_delay) + rest_cut[Axis])/(4 * cerCutStepSrd[Axis].accel_count + 1));//������(��)һ����������(ʱ����)
        rest_cut[Axis] = ((2 * cerCutStepSrd[Axis].step_delay)+rest_cut[Axis])%(4 * cerCutStepSrd[Axis].accel_count + 1);// �����������´μ��㲹���������������
        
        if(step_count_cut[Axis] >= cerCutStepSrd[Axis].decel_start || listCutStepStopCmd[Axis])		// ����Ƿ�Ӧ�ÿ�ʼ����
        {
					if(listCutStepStopCmd[Axis]){
					 listCutStepStopCmd[Axis] = False;
						cerCutStepSrd[Axis].accel_count = T1_FREQ/2/stpdecel*T1_FREQ/cerCutStepSrd[Axis].step_delay/cerCutStepSrd[Axis].step_delay*-1;
					}else{
					cerCutStepSrd[Axis].accel_count = cerCutStepSrd[Axis].decel_val; 	// ���ټ���ֵΪ���ٽ׶μ���ֵ�ĳ�ʼֵ
					}
          cerCutStepSrd[Axis].run_state = DECEL;           	      // �¸����������ٽ׶�
        }
        else if(new_step_delay_cut <= cerCutStepSrd[Axis].min_delay)  // ����Ƿ񵽴�����������ٶ�
        {
          cerCutStepSrd[Axis].accel_count = cerCutStepSrd[Axis].decel_val; 	// ���ټ���ֵΪ���ٽ׶μ���ֵ�ĳ�ʼֵ
          last_accel_delay_cut[Axis] = new_step_delay_cut; 	// ������ٹ��������һ����ʱ���������ڣ�
          new_step_delay_cut = cerCutStepSrd[Axis].min_delay;    	// ʹ��min_delay����Ӧ����ٶ�speed��
          rest_cut[Axis] = 0;                          	// ������ֵ
          cerCutStepSrd[Axis].run_state = RUN;               	// ����Ϊ��������״̬
        }
        last_accel_delay_cut[Axis] = new_step_delay_cut; 	  // ������ٹ��������һ����ʱ���������ڣ�
        break;
        
      case RUN:
        step_count_cut[Axis]++; 		 // ������1
        listCutStep_count[Axis] ++; 
			if(cerCutStepSrd[Axis].dir==CW)
        {	  	
          pmCutStep_position[Axis]++; // ����λ�ü�1
        }
        else
        {
          pmCutStep_position[Axis]--; // ����λ�ü�1
        }
            
        new_step_delay_cut = cerCutStepSrd[Axis].min_delay;       // ʹ��min_delay����Ӧ����ٶ�speed��
            
        if(step_count_cut[Axis] >= cerCutStepSrd[Axis].decel_start|| listCutStepStopCmd[Axis])   // ��Ҫ��ʼ����
        {
					if(listCutStepStopCmd[Axis]){
						listCutStepStopCmd[Axis] = False;
						cerCutStepSrd[Axis].accel_count = T1_FREQ/2/stpdecel*T1_FREQ/cerCutStepSrd[Axis].step_delay/cerCutStepSrd[Axis].step_delay*-1;
					}else{
          cerCutStepSrd[Axis].accel_count = cerCutStepSrd[Axis].decel_val;  // ���ٲ�����Ϊ���ټ���ֵ
          }
					new_step_delay_cut = last_accel_delay_cut[Axis];// �ӽ׶�������ʱ��Ϊ���ٽ׶ε���ʼ��ʱ(��������)
          cerCutStepSrd[Axis].run_state = DECEL;            // ״̬�ı�Ϊ����
        }
        break;
        
      case DECEL:
        step_count_cut[Axis]++; 		 // ������1
       listCutStep_count[Axis] ++; 
			if(cerCutStepSrd[Axis].dir == CW)
        {		  	
          pmCutStep_position[Axis]++; // ����λ�ü�1
        }
        else
        {
          pmCutStep_position[Axis]--; // ����λ�ü�1
        }
        cerCutStepSrd[Axis].accel_count++;
        new_step_delay_cut = cerCutStepSrd[Axis].step_delay - (((2 * cerCutStepSrd[Axis].step_delay) + rest_cut[Axis])/(4 * cerCutStepSrd[Axis].accel_count + 1)); //������(��)һ����������(ʱ����)
        rest_cut[Axis] = ((2 * cerCutStepSrd[Axis].step_delay)+rest_cut[Axis])%(4 * cerCutStepSrd[Axis].accel_count + 1);// �����������´μ��㲹���������������
				if(listStopCmd[Axis]){
					listStopCmd[Axis] = False;
					cerCutStepSrd[Axis].accel_count = T1_FREQ/2/stpdecel*T1_FREQ/cerCutStepSrd[Axis].step_delay/cerCutStepSrd[Axis].step_delay*-1;
				}
        //����Ƿ�Ϊ���һ��
        if(cerCutStepSrd[Axis].accel_count >= 0)
        {
            cerCutStepSrd[Axis].run_state = STOP;
        }
				if(cerCutStepSrd[Axis].medle_delay != 0) 
          {
            if(new_step_delay_cut >= cerCutStepSrd[Axis].medle_delay)		//����Ƿ񵽴������ٶ�
            {
              cerCutStepSrd[Axis].min_delay = cerCutStepSrd[Axis].medle_delay;//װ�������ٶ�
              new_step_delay_cut = cerCutStepSrd[Axis].medle_delay;     //�������ٶ�Ϊ�¸���ʱ
              cerCutStepSrd[Axis].decel_start = UINT32_MAX;       //�ڴﵽdecel_start֮ǰ�������� 
              rest_cut[Axis] = 0;       
              cerCutStepSrd[Axis].run_state = RUN;
            }		
          }
        break;
      default :break;
    }      
    if(new_step_delay_cut>0xFFFF)
      new_step_delay_cut=0xFFFF;
    cerCutStepSrd[Axis].step_delay = new_step_delay_cut; // Ϊ�¸�(�µ�)��ʱ(��������)��ֵ
  }
}



static void RollerStepOutPutControl(uint8_t status)
{
	if(status == 0){ //�ߵ�ƽʹ�ܻ��ǵ͵�ƽʹ��
		 HAL_GPIO_WritePin(PM_ROLLER_STEP_ENA_PORT,PM_ROLLER_STEP_ENA_PIN,GPIO_PIN_SET);
	}else{
		HAL_GPIO_WritePin(PM_ROLLER_STEP_ENA_PORT,PM_ROLLER_STEP_ENA_PIN,GPIO_PIN_RESET);
	}
	
}
void RollerStepDirControl(uint8_t Dir)
{
	if(Dir == CCW){
		 HAL_GPIO_WritePin(PM_ROLLER_STEP_DIR_PORT,PM_ROLLER_STEP_DIR_PIN,GPIO_PIN_SET);
	}else{
		 HAL_GPIO_WritePin(PM_ROLLER_STEP_DIR_PORT,PM_ROLLER_STEP_DIR_PIN,GPIO_PIN_RESET);
	}
}

void STEPMOTOR_Motion_Ctrl(uint8_t Dir , float Frequency)
{
  uint16_t Step_Delay = 0;  //������ʱ

  if(Frequency == 0)
    RollerStepOutPutControl(0);
  else
  {
    if(Dir==CCW)
    {
      RollerStepDirControl(CCW);
    }
    else RollerStepDirControl(CW);//�������
    /*
      ��������ٶ��ɶ�ʱ���������Ƶ��(f)����,
        f = c/F;c�Ǽ������ļ���ֵ,F�Ƕ�ʱ��Ƶ��
      �Ƶ�����:(T�Ƕ�ʱ�������������)
        T=c*t => T=c*F => f = 1/T = F/c;
    
    */
    Step_Delay = (uint16_t)(FREQ_UINT/Frequency);
    
    Toggle_Pulse = Step_Delay>>1;//������Ľ��������,�������2,�����ڷ�תһ��
  }
}


/**
  * ��������: ��ʱ���Ƚ�����жϻص�����
  * �������: htim����ʱ�����ָ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  */
//void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim)
//{
//  uint32_t count;
//  uint32_t tmp;
//  count=__HAL_TIM_GET_COUNTER(&cutMachine_STEPMOTOR);
//  tmp = STEPMOTOR_TIM_PERIOD & (count + Toggle_Pulse);
//  __HAL_TIM_SET_COMPARE(&cutMachine_STEPMOTOR,TIM_CHANNEL_1,tmp);
//}
