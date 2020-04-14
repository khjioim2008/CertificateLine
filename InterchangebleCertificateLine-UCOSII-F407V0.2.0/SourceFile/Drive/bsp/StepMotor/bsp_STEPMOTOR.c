/* 包含头文件 ----------------------------------------------------------------*/
#include "StepMotor/bsp_STEPMOTOR.h" 
#include <math.h>
#include <string.h>

/* 私有类型定义 --------------------------------------------------------------*/

/* 私有宏定义 ----------------------------------------------------------------*/

/* 私有变量 ------------------------------------------------------------------*/
TIM_HandleTypeDef htimx_STEPMOTOR;
TIM_HandleTypeDef cutMachine_STEPMOTOR; 

__IO uint16_t Toggle_Pulse = 3000;         // 比较输出周期，值越小输出频率越快
__IO uint8_t i = 0;                         //步进电机辅助输出脉冲计数器
__IO uint8_t iCut = 0;                         //步进电机辅助输出脉冲计数器


// 总移动步数计数器


__IO uint32_t listStep_count[4] = {0,0,0,0};
bool listStopCmd[4]       = {False,False,False,False}; // 纸盒停止命令

__IO uint32_t listCutStep_count[2] = {0,0};
bool listCutStepStopCmd[2]       = {False,False}; // 纸盒停止命令

int32_t  stpdecel   = 5000;       // 急停减速度

speedRampData cerStepSrd[4] ={\
 {STOP,CW,0,0,0,0,0},   // 加减速曲线变量
 {STOP,CW,0,0,0,0,0},
 {STOP,CW,0,0,0,0,0},
 {STOP,CW,0,0,0,0,0},
};

speedRampData cerCutStepSrd[2] ={\
 {STOP,CW,0,0,0,0,0},   // 加减速曲线变量
 {STOP,CW,0,0,0,0,0},
};



/* 定时器脉冲输出引脚,需要修改直接在stepmotor.h头文件修改即可*/
const Tim Timer[4]={\
{STEPMOTOR_TIM_PUL_PIN, STEPMOTOR_TIM_CHANNEL_x, STEPMOTOR_TIM_IT_CCx, STEPMOTOR_TIM_FLAG_CCx},
{SM_TIM_PUL_PIN, SM_TIM_CHANNEL_x, SM_STEP_TIM_IT_CCx, SM_STEP_TIM_FLAG_CCx},
{PM_FRICTION_STEP_TIM_PUL_PIN, PM_FRICTION_STEP_TIM_CHANNEL_x, PM_FRICTION_STEP_TIM_IT_CCx, PM_FRICTION_STEP_TIM_FLAG_CCx},
{PM_ROLLER_STEP_TIM_PUL_PIN, PM_ROLLER_STEP_TIM_CHANNEL_x, PM_ROLLER_STEP_TIM_IT_CCx, PM_ROLLER_STEP_TIM_FLAG_CCx},
};

/* 定时器脉冲输出引脚,需要修改直接在stepmotor.h头文件修改即可*/
const Tim Timer9[2]={\
{CUT_CHAMFER_STEP_TIM_PUL_PIN, CUT_CHAMFER_STEP_TIM_CHANNEL_x, CUT_CHAMFER_STEP_TIM_IT_CCx, CUT_CHAMFER_STEP_TIM_FLAG_CCx},
{CUT_ROLLER_STEP_TIM_PUL_PIN, CUT_ROLLER_STEP_TIM_CHANNEL_x, CUT_ROLLER_STEP_TIM_IT_CCx, CUT_ROLLER_STEP_TIM_FLAG_CCx},
};

/* 步进电机控制引脚*/
const StepMotor Stepmotor[4]={\
{STEPMOTOR_ENA_PIN,					STEPMOTOR_DIR_PIN,								STEPMOTOR_DIR_PORT,       STEPMOTOR_ENA_PORT},
{SM_STEP_ENA_PIN,						SM_STEP_DIR_PIN,									SM_STEP_DIR_PORT,	        SM_STEP_ENA_PORT},
{PM_FRICTION_STEP_ENA_PIN,	PM_FRICTION_STEP_DIR_PIN,					PM_FRICTION_STEP_DIR_PORT,PM_FRICTION_STEP_ENA_PORT},
{PM_ROLLER_STEP_ENA_PIN,		PM_ROLLER_STEP_DIR_PIN,						PM_ROLLER_STEP_DIR_PORT,  PM_ROLLER_STEP_ENA_PORT},
};

/* 步进电机控制引脚*/
const CutStepMotor CutStepmotor[2]={\
{CUT_CHAMFER_STEP_ENA_PIN,CUT_CHAMFER_STEP_ENA_PORT},
{CUT_ROLLER_STEP_ENA_PIN,	 CUT_ROLLER_STEP_ENA_PORT},
};

__IO uint8_t  cerMotionStatus[4]   = {STOP,STOP,STOP,STOP};        // 是否在运动？0：停止，1：运动
__IO int32_t  pmStep_position[4]  = {0,0,0,0};           // 当前位置

__IO uint8_t  cerCutMotionStatus[2]   = {STOP,STOP};        // 是否在运动？0：停止，1：运动
__IO int32_t  pmCutStep_position[2]  = {0,0};           // 当前位置

/* 扩展变量 ------------------------------------------------------------------*/

/* 私有函数原形 --------------------------------------------------------------*/

/* 函数体 --------------------------------------------------------------------*/

/**
  * 函数功能: 驱动器相关GPIO初始化配置
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  */
static void STEPMOTOR_GPIO_Init(void)
{
	uint8_t i;
  GPIO_InitTypeDef GPIO_InitStruct; 
  
  /* 引脚端口时钟使能 */
  STEPMOTOR_TIM_GPIO_CLK_ENABLE();
  STEPMOTOR_DIR_GPIO_CLK_ENABLE();
  STEPMOTOR_ENA_GPIO_CLK_ENABLE();
  
	 for(i=0; i<4; i++)
  {
		/* 步进电机驱动器：脉冲输出 */
    GPIO_InitStruct.Pin = Timer[i].Pulse_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
		//GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF1_TIM1;
		HAL_GPIO_Init(PM_FRICTION_STEP_TIM_PUL_PORT, &GPIO_InitStruct);
    
    /* 步进电机驱动器：方向控制 */
    CER_STEPMOTOR_DIR_FORWARD(i);//默认设置为顺时针方向
    GPIO_InitStruct.Pin = Stepmotor[i].Dir_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
		//GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    //GPIO_InitStruct.Alternate = GPIO_AF0_TRACE;
    HAL_GPIO_Init(Stepmotor[i].Dir_Port, &GPIO_InitStruct);
  
    /* 步进电机驱动器：使能控制 */
    GPIO_InitStruct.Pin = Stepmotor[i].Ena_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
		//GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; //有隔离用PP
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
  * 函数功能: 驱动器定时器初始化
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  */
void STEPMOTOR_TIMx_Init(void)
{
		uint8_t i = 0;
  TIM_ClockConfigTypeDef sClockSourceConfig;             // 定时器时钟
  TIM_OC_InitTypeDef sConfigOC;                          // 定时器通道比较输出
  
  STEPMOTOR_TIM_RCC_CLK_ENABLE();
	
  /* STEPMOTOR相关GPIO初始化配置 */
  STEPMOTOR_GPIO_Init();
  
  /* 定时器基本环境配置 */
  htimx_STEPMOTOR.Instance = STEPMOTOR_TIMx;                          // 定时器编号
  htimx_STEPMOTOR.Init.Prescaler = STEPMOTOR_TIM_PRESCALER;           // 定时器预分频器
  htimx_STEPMOTOR.Init.CounterMode = TIM_COUNTERMODE_UP;              // 计数方向：向上计数
  htimx_STEPMOTOR.Init.Period = STEPMOTOR_TIM_PERIOD;                 // 定时器周期
  htimx_STEPMOTOR.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;          // 时钟分频
  HAL_TIM_Base_Init(&htimx_STEPMOTOR);
	
	
  /* 定时器时钟源配置 */
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;       		// 使用内部时钟源
  HAL_TIM_ConfigClockSource(&htimx_STEPMOTOR, &sClockSourceConfig);

  /* 定时器比较输出配置 */
  sConfigOC.OCMode = TIM_OCMODE_TOGGLE;                // 比较输出模式：反转输出
  sConfigOC.Pulse = 0xFFFF;                            // 脉冲数
  sConfigOC.OCPolarity = TIM_OCPOLARITY_LOW;           // 输出极性
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_LOW;         // 互补通道输出极性
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;           // 快速模式
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;       // 空闲电平
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;     // 互补通道空闲电平
  
	for (i=0; i <4; i++)
	{
		HAL_TIM_OC_Stop_IT (&htimx_STEPMOTOR,Timer[i].Channel); 
    HAL_TIM_OC_ConfigChannel(&htimx_STEPMOTOR, &sConfigOC, Timer[i].Channel);
		TIM_CCxChannelCmd(STEPMOTOR_TIMx, Timer[i].Channel, TIM_CCx_DISABLE);
	}
	
  /* 配置定时器中断优先级并使能 */
  HAL_NVIC_SetPriority(STEPMOTOR_TIMx_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(STEPMOTOR_TIMx_IRQn);
  
  /* Enable the main output */
  __HAL_TIM_MOE_ENABLE(&htimx_STEPMOTOR);  
  HAL_TIM_Base_Start(&htimx_STEPMOTOR);// 使能定时器
}

static void PagingMachinePIDStep_GPIO_Init(void)
{
	
	
	  GPIO_InitTypeDef GPIO_InitStruct; 
	//CUT_CHAMFER_STEP_TIM_GPIO_CLK_ENABLE();
	CUT_ROLLER_STEP_TIM_RCC_CLK_ENABLE();
	//CUT_CHAMFER_STEP_ENA_GPIO_CLK_ENABLE();
	CUT_ROLLER_STEP_ENA_GPIO_CLK_ENABLE();
	
			/* 步进电机驱动器：脉冲输出 */
    GPIO_InitStruct.Pin = CUT_CHAMFER_STEP_TIM_PUL_PIN;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
		//GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = CUT_CHAMFER_STEP_GPIO_AFx_TIMx;
		HAL_GPIO_Init(CUT_CHAMFER_STEP_TIM_PUL_PORT, &GPIO_InitStruct);
 
			/* 步进电机驱动器：脉冲输出 */
    GPIO_InitStruct.Pin = CUT_ROLLER_STEP_TIM_PUL_PIN;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
		//GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = CUT_ROLLER_STEP_GPIO_AFx_TIMx;
		HAL_GPIO_Init(CUT_ROLLER_STEP_TIM_PUL_PORT, &GPIO_InitStruct);
    	
	    /* 步进电机驱动器：使能控制 */
    GPIO_InitStruct.Pin = CUT_CHAMFER_STEP_ENA_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
		//GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; //有隔离用PP
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    //GPIO_InitStruct.Alternate = GPIO_AF0_TRACE;
    HAL_GPIO_Init(CUT_CHAMFER_STEP_ENA_PORT, &GPIO_InitStruct);
		
			    /* 步进电机驱动器：使能控制 */
    GPIO_InitStruct.Pin = CUT_ROLLER_STEP_ENA_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
		//GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; //有隔离用PP
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    //GPIO_InitStruct.Alternate = GPIO_AF0_TRACE;
    HAL_GPIO_Init(CUT_ROLLER_STEP_ENA_PORT, &GPIO_InitStruct);
		
}

void cutMachineStep_TIM9_Init(void)
{
  TIM_ClockConfigTypeDef sClockSourceConfig;             // 定时器时钟
  TIM_OC_InitTypeDef sConfigOC;                          // 定时器通道比较输出
    
	CUT_MACHINE_TIM_RCC_CLK_ENABLE();
	/* STEPMOTOR相关GPIO初始化配置 */
	 PagingMachinePIDStep_GPIO_Init();

  /* 定时器基本环境配置 */
  cutMachine_STEPMOTOR.Instance = CUT_MACHINE_TIMx;                                 // 定时器编号
  cutMachine_STEPMOTOR.Init.Prescaler = STEPMOTOR_TIM_PRESCALER;                  // 定时器预分频器
  cutMachine_STEPMOTOR.Init.CounterMode = TIM_COUNTERMODE_UP;                     // 计数方向：向上计数
  cutMachine_STEPMOTOR.Init.Period = STEPMOTOR_TIM_PERIOD;                        // 定时器周期
  cutMachine_STEPMOTOR.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;                 // 时钟分频
 // cutMachine_STEPMOTOR.Init.RepetitionCounter = STEPMOTOR_TIM_REPETITIONCOUNTER;  // 重复计数器
  HAL_TIM_Base_Init(&cutMachine_STEPMOTOR);

  /* 定时器时钟源配置 */
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;                 // 使用内部时钟源
  HAL_TIM_ConfigClockSource(&cutMachine_STEPMOTOR, &sClockSourceConfig);

  /* 定时器比较输出配置 */
  sConfigOC.OCMode = TIM_OCMODE_TOGGLE;                // 比较输出模式：反转输出
  sConfigOC.Pulse = 0xFFFF;                      // 脉冲数
  sConfigOC.OCPolarity = TIM_OCPOLARITY_LOW;           // 输出极性
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_LOW;         // 互补通道输出极性
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;           // 快速模式
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;       // 空闲电平
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;     // 互补通道空闲电平
  
	//HAL_TIM_OC_ConfigChannel(&cutMachine_STEPMOTOR, &sConfigOC, PAGINGPID_TIM_CHANNEL_x);
	for (i=0; i <2; i++)
	{
		HAL_TIM_OC_Stop_IT (&cutMachine_STEPMOTOR,Timer9[i].Channel); 
    HAL_TIM_OC_ConfigChannel(&cutMachine_STEPMOTOR, &sConfigOC, Timer9[i].Channel);
		TIM_CCxChannelCmd(CUT_MACHINE_TIMx, Timer9[i].Channel, TIM_CCx_DISABLE);
	}
	
  
  /* 配置定时器中断优先级并使能 */
  HAL_NVIC_SetPriority(CUT_MACHINE_TIMx_IRQn, 1, 1);
  HAL_NVIC_EnableIRQ(CUT_MACHINE_TIMx_IRQn);
  
  /* Enable the main output */
  __HAL_TIM_MOE_ENABLE(&cutMachine_STEPMOTOR);  
  HAL_TIM_Base_Start(&cutMachine_STEPMOTOR);// 使能定时器

}
/**
  * 函数功能: 基本定时器硬件初始化配置
  * 输入参数: htim_base：基本定时器句柄类型指针
  * 返 回 值: 无
  * 说    明: 该函数被HAL库内部调用
  */
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* htim_base)
{

  if(htim_base->Instance==STEPMOTOR_TIMx)
  {
    /* 基本定时器外设时钟使能 */
    STEPMOTOR_TIM_RCC_CLK_ENABLE();
  }
	  if(htim_base->Instance==CUT_MACHINE_TIMx)
		{
		CUT_MACHINE_TIM_RCC_CLK_ENABLE();
		}																													
}	

/**
  * 函数功能: 基本定时器硬件反初始化配置
  * 输入参数: htim_base：基本定时器句柄类型指针
  * 返 回 值: 无
  * 说    明: 该函数被HAL库内部调用
  */
void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* htim_base)
{

  if(htim_base->Instance==STEPMOTOR_TIMx)
  {
    /* 基本定时器外设时钟禁用 */
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
  * 函数功能: 相对位置运动：运动给定的步数
  * 输入参数: Axis   活动轴
  *           step   移动的步数 (正数为顺时针，负数为逆时针).
  *           accel  加速度,实际值为accel*0.025*rad/sec^2
  *           decel  减速度,实际值为decel*0.025*rad/sec^2
  *           speed  最大速度,实际值为speed*0.05*rad/sec
  * 返 回 值: 无
  * 说    明: 以给定的步数移动步进电机，先加速到最大速度，然后在合适位置开始
  *           减速至停止，使得整个运动距离为指定的步数。如果加减速阶段很短并且
  *           速度很慢，那还没达到最大速度就要开始减速
  */
void STEPMOTOR_CerAxisMoveRel(uint8_t Axis,int32_t step,uint32_t accel, uint32_t decel, uint32_t speed)
{  
		__IO uint16_t tim_count;//获取定时器的计数值
  // 达到最大速度时的步数
	__IO uint32_t max_s_lim;
  // 必须要开始减速的步数（如果加速没有达到最大速度）
	__IO uint32_t accel_lim;
  
	//在电机运动过程一般不能再继续操作,除了在回原点过程中
//	if((cerMotionStatus[Axis] != STOP)&&(ZeroStep[Axis] == IDLE))	

	//	if(cerMotionStatus[Axis] != STOP)
//	{
//		return ;
//	}
	
  	/* 方向设定 */
	if(step < 0) // 步数为负数
	{
		cerStepSrd[Axis].dir = CCW; // 逆时针方向旋转
		CER_STEPMOTOR_DIR_REVERSAL(Axis);
		step = -step;   // 获取步数绝对值
	}
	else
	{
		cerStepSrd[Axis].dir = CW; // 顺时针方向旋转
		CER_STEPMOTOR_DIR_FORWARD(Axis);
	}

	if(step == 1)    // 步数为1
	{
		cerStepSrd[Axis].accel_count = -1;   // 只移动一步
		cerStepSrd[Axis].run_state = DECEL;  // 减速状态.
		cerStepSrd[Axis].step_delay = 1000;	// 短延时	
	}
		else if(step != 0)  // 如果目标运动步数不为0
	{

		// 设置最大速度极限, 计算得到min_delay用于定时器的计数器的值。
		// min_delay = (alpha / tt)/ w
		cerStepSrd[Axis].min_delay = (int32_t)(A_T_x10/speed);

		// 通过计算第一个(c0) 的步进延时来设定加速度，其中accel单位为0.1rad/sec^2
		// step_delay = 1/tt * sqrt(2*alpha/accel)
		// step_delay = ( tfreq*0.676/10 )*10 * sqrt( (2*alpha*100000) / (accel*10) )/100
		cerStepSrd[Axis].step_delay = (int32_t)((T1_FREQ_148 * sqrt(A_SQ / accel))/10);//C0,初始速度的定时器值
			

		/*计算加减速需要的参数*/
		// 计算多少步之后达到最大速度的限制
		// max_s_lim = speed^2 / (2*alpha*accel)
		max_s_lim = (uint32_t)(speed*speed/(A_x200*accel/10));
		// 如果达到最大速度小于0.5步，我们将四舍五入为0
		// 但实际我们必须移动至少一步才能达到想要的速度
		if(max_s_lim == 0)
		{
		  max_s_lim = 1;
		}
			
		// 计算多少步之后我们必须开始减速
		// n1 = (n1+n2)decel / (accel + decel)
		accel_lim = (uint32_t)(step*decel/(accel+decel));
		// 我们必须加速至少1步才能才能开始减速.
		if(accel_lim == 0)
		{
		  accel_lim = 1;
		}
		// 使用限制条件我们可以计算出减速阶段步数
		if(accel_lim <= max_s_lim)
		{
		  cerStepSrd[Axis].decel_val = accel_lim - step;
		}
		else{
		  cerStepSrd[Axis].decel_val = -(max_s_lim*accel/decel);
		}
		// 当只剩下一步我们必须减速
		if(cerStepSrd[Axis].decel_val == 0)
		{
		  cerStepSrd[Axis].decel_val = -1;
		}

		// 计算开始减速时的步数
		cerStepSrd[Axis].decel_start = step + cerStepSrd[Axis].decel_val;

		// 如果最大速度很慢，我们就不需要进行加速运动
		if(cerStepSrd[Axis].step_delay <= cerStepSrd[Axis].min_delay)
		{
			cerStepSrd[Axis].step_delay = cerStepSrd[Axis].min_delay;
			cerStepSrd[Axis].run_state = RUN;
		}
		else
		{
			cerStepSrd[Axis].run_state = ACCEL;
		}    
		// 复位加速度计数值
		cerStepSrd[Axis].accel_count = 0;
	}
	
	cerMotionStatus[Axis] = 1;
	tim_count=__HAL_TIM_GET_COUNTER(&htimx_STEPMOTOR);
 	__HAL_TIM_SET_COMPARE(&htimx_STEPMOTOR,Timer[Axis].Channel,tim_count + cerStepSrd[Axis].step_delay); // 设置定时器比较值
  HAL_TIM_OC_Start_IT (&htimx_STEPMOTOR,Timer[Axis].Channel);
	TIM_CCxChannelCmd(STEPMOTOR_TIMx, Timer[Axis].Channel, TIM_CCx_ENABLE);// 使能定时器通道 
	CER_STEPMOTOR_OUTPUT_ENABLE(Axis);
}


void STEPMOTOR_CerAxisMoveRel_CutTim9(uint8_t Axis,int32_t step,uint32_t accel, uint32_t decel, uint32_t speed)
{  
		__IO uint16_t tim_count_cut;//获取定时器的计数值
  // 达到最大速度时的步数
	__IO uint32_t max_s_lim_cut;
  // 必须要开始减速的步数（如果加速没有达到最大速度）
	__IO uint32_t accel_lim_cut;
  
	//在电机运动过程一般不能再继续操作,除了在回原点过程中
//	if((cerMotionStatus[Axis] != STOP)&&(ZeroStep[Axis] == IDLE))	

	//	if(cerMotionStatus[Axis] != STOP)
//	{
//		return ;
//	}
	
  	/* 方向设定 */
	if(step < 0) // 步数为负数
	{
		cerCutStepSrd[Axis].dir = CCW; // 逆时针方向旋转
		//CER_STEPMOTOR_DIR_REVERSAL(Axis);
		step = -step;   // 获取步数绝对值
	}
	else
	{
		cerCutStepSrd[Axis].dir = CW; // 顺时针方向旋转
		//CER_STEPMOTOR_DIR_FORWARD(Axis);
	}

	if(step == 1)    // 步数为1
	{
		cerCutStepSrd[Axis].accel_count = -1;   // 只移动一步
		cerCutStepSrd[Axis].run_state = DECEL;  // 减速状态.
		cerCutStepSrd[Axis].step_delay = 1000;	// 短延时	
	}
		else if(step != 0)  // 如果目标运动步数不为0
	{

		// 设置最大速度极限, 计算得到min_delay用于定时器的计数器的值。
		// min_delay = (alpha / tt)/ w
		cerCutStepSrd[Axis].min_delay = (int32_t)(A_T_x10/speed);

		// 通过计算第一个(c0) 的步进延时来设定加速度，其中accel单位为0.1rad/sec^2
		// step_delay = 1/tt * sqrt(2*alpha/accel)
		// step_delay = ( tfreq*0.676/10 )*10 * sqrt( (2*alpha*100000) / (accel*10) )/100
		cerCutStepSrd[Axis].step_delay = (int32_t)((T1_FREQ_148 * sqrt(A_SQ / accel))/10);//C0,初始速度的定时器值
			

		/*计算加减速需要的参数*/
		// 计算多少步之后达到最大速度的限制
		// max_s_lim = speed^2 / (2*alpha*accel)
		max_s_lim_cut = (uint32_t)(speed*speed/(A_x200*accel/10));
		// 如果达到最大速度小于0.5步，我们将四舍五入为0
		// 但实际我们必须移动至少一步才能达到想要的速度
		if(max_s_lim_cut == 0)
		{
		  max_s_lim_cut = 1;
		}
			
		// 计算多少步之后我们必须开始减速
		// n1 = (n1+n2)decel / (accel + decel)
		accel_lim_cut = (uint32_t)(step*decel/(accel+decel));
		// 我们必须加速至少1步才能才能开始减速.
		if(accel_lim_cut == 0)
		{
		  accel_lim_cut = 1;
		}
		// 使用限制条件我们可以计算出减速阶段步数
		if(accel_lim_cut <= max_s_lim_cut)
		{
		  cerCutStepSrd[Axis].decel_val = accel_lim_cut - step;
		}
		else{
		  cerCutStepSrd[Axis].decel_val = -(max_s_lim_cut*accel/decel);
		}
		// 当只剩下一步我们必须减速
		if(cerCutStepSrd[Axis].decel_val == 0)
		{
		  cerCutStepSrd[Axis].decel_val = -1;
		}

		// 计算开始减速时的步数
		cerCutStepSrd[Axis].decel_start = step + cerCutStepSrd[Axis].decel_val;

		// 如果最大速度很慢，我们就不需要进行加速运动
		if(cerCutStepSrd[Axis].step_delay <= cerCutStepSrd[Axis].min_delay)
		{
			cerCutStepSrd[Axis].step_delay = cerCutStepSrd[Axis].min_delay;
			cerCutStepSrd[Axis].run_state = RUN;
		}
		else
		{
			cerCutStepSrd[Axis].run_state = ACCEL;
		}    
		// 复位加速度计数值
		cerCutStepSrd[Axis].accel_count = 0;
	}
	
	cerCutMotionStatus[Axis] = 1;
	tim_count_cut=__HAL_TIM_GET_COUNTER(&cutMachine_STEPMOTOR);
	__HAL_TIM_SET_COMPARE(&cutMachine_STEPMOTOR,Timer9[Axis].Channel,tim_count_cut + cerCutStepSrd[Axis].step_delay); // 设置定时器比较值
  HAL_TIM_OC_Start_IT (&cutMachine_STEPMOTOR,Timer9[Axis].Channel);
	TIM_CCxChannelCmd(CUT_MACHINE_TIMx, Timer9[Axis].Channel, TIM_CCx_ENABLE);// 使能定时器通道 
	CER_CUT_STEPMOTOR_OUTPUT_ENABLE(Axis);
}


void STEPMOTOR_SM_AxisMoveRel(uint8_t Axis,int32_t step,uint32_t accel, uint32_t decel, uint32_t speed)
{
	STEPMOTOR_CerAxisMoveRel(Axis,step,accel,decel,speed);
	cerStepSrd[Axis].run_state = RUN;
}

/**
  * 函数功能: 定时器中断回调函数
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 实现加减速过程
  */
void HAL_TIM_OC_Callback(uint8_t Axis)
{
  __IO static uint16_t tim_count = 0;
  __IO  uint32_t new_step_delay = 0;
  __IO static uint8_t  i[4]={0,0,0,0};
	__IO static uint16_t last_accel_delay[4] = {0,0,0,0};
	// 总移动步数计数器
	__IO static uint32_t step_count[4] = {0,0,0,0};
	// 记录new_step_delay中的余数，提高下一步计算的精度
	__IO static int32_t rest[4] = {0,0,0,0};
	//定时器使用翻转模式，需要进入两次中断才输出一个完整脉冲

  // 设置比较值
  tim_count = __HAL_TIM_GET_COUNTER(&htimx_STEPMOTOR);
  __HAL_TIM_SET_COMPARE(&htimx_STEPMOTOR,Timer[Axis].Channel,tim_count+cerStepSrd[Axis].step_delay);

  i[Axis]++;       // 定时器中断次数计数值
  if(i[Axis] == 2) // 2次，说明已经输出一个完整脉冲
  {
    i[Axis] = 0;   // 清零定时器中断次数计数值

    switch(cerStepSrd[Axis].run_state) // 加减速曲线阶段
    {
      case STOP:
    
        // 关闭通道
        TIM_CCxChannelCmd(STEPMOTOR_TIMx,Timer[Axis].Channel,TIM_CCx_DISABLE);       
        __HAL_TIM_CLEAR_FLAG(&htimx_STEPMOTOR, Timer[Axis].Flag_CCx);
        HAL_TIM_OC_Stop_IT (&htimx_STEPMOTOR,Timer[Axis].Channel);
       // CER_STEPMOTOR_OUTPUT_DISABLE(Axis);
      
        cerMotionStatus[Axis] = STOP;  		//  电机为停止状态
        step_count[Axis] = 0;  // 清零步数计数器
			  rest[Axis] = 0;        // 清零余值
        last_accel_delay[Axis] = 0;
        cerStepSrd[Axis].accel_count = 0;
        cerStepSrd[Axis].step_delay = 0;
        cerStepSrd[Axis].min_delay = 0;
        break;
      case ACCEL:
        step_count[Axis] ++;      		// 步数加1
				listStep_count[Axis] ++;  
			if(cerStepSrd[Axis].dir == CW)
        {		  	
          pmStep_position[Axis]++; 	  // 绝对位置加1
        }
        else
        {
          pmStep_position[Axis]--; 	  // 绝对位置减1
        }
        cerStepSrd[Axis].accel_count++; 	  // 加速计数值加1
        
        new_step_delay = cerStepSrd[Axis].step_delay - (((2 *cerStepSrd[Axis].step_delay) + rest[Axis])/(4 * cerStepSrd[Axis].accel_count + 1));//计算新(下)一步脉冲周期(时间间隔)
        rest[Axis] = ((2 * cerStepSrd[Axis].step_delay)+rest[Axis])%(4 * cerStepSrd[Axis].accel_count + 1);// 计算余数，下次计算补上余数，减少误差
        
        if(step_count[Axis] >= cerStepSrd[Axis].decel_start || listStopCmd[Axis])		// 检查是否应该开始减速
        {
					if(listStopCmd[Axis]){
					 listStopCmd[Axis] = False;
						cerStepSrd[Axis].accel_count = T1_FREQ/2/stpdecel*T1_FREQ/cerStepSrd[Axis].step_delay/cerStepSrd[Axis].step_delay*-1;
					}else{
					cerStepSrd[Axis].accel_count = cerStepSrd[Axis].decel_val; 	// 加速计数值为减速阶段计数值的初始值
					}
          cerStepSrd[Axis].run_state = DECEL;           	      // 下个脉冲进入减速阶段
        }
        else if(new_step_delay <= cerStepSrd[Axis].min_delay)  // 检查是否到达期望的最大速度
        {
          cerStepSrd[Axis].accel_count = cerStepSrd[Axis].decel_val; 	// 加速计数值为减速阶段计数值的初始值
          last_accel_delay[Axis] = new_step_delay; 	// 保存加速过程中最后一次延时（脉冲周期）
          new_step_delay = cerStepSrd[Axis].min_delay;    	// 使用min_delay（对应最大速度speed）
          rest[Axis] = 0;                          	// 清零余值
          cerStepSrd[Axis].run_state = RUN;               	// 设置为匀速运行状态
        }
        last_accel_delay[Axis] = new_step_delay; 	  // 保存加速过程中最后一次延时（脉冲周期）
        break;
        
      case RUN:
        step_count[Axis]++; 		 // 步数加1
        listStep_count[Axis] ++; 
			if(cerStepSrd[Axis].dir==CW)
        {	  	
          pmStep_position[Axis]++; // 绝对位置加1
        }
        else
        {
          pmStep_position[Axis]--; // 绝对位置减1
        }
            
        new_step_delay = cerStepSrd[Axis].min_delay;       // 使用min_delay（对应最大速度speed）
            
        if(step_count[Axis] >= cerStepSrd[Axis].decel_start|| listStopCmd[Axis])   // 需要开始减速
        {
					if(listStopCmd[Axis]){
						listStopCmd[Axis] = False;
						cerStepSrd[Axis].accel_count = T1_FREQ/2/stpdecel*T1_FREQ/cerStepSrd[Axis].step_delay/cerStepSrd[Axis].step_delay*-1;
					}else{
          cerStepSrd[Axis].accel_count = cerStepSrd[Axis].decel_val;  // 减速步数做为加速计数值
          }
					new_step_delay = last_accel_delay[Axis];// 加阶段最后的延时做为减速阶段的起始延时(脉冲周期)
          cerStepSrd[Axis].run_state = DECEL;            // 状态改变为减速
        }
        break;
        
      case DECEL:
        step_count[Axis]++; 		 // 步数加1
       listStep_count[Axis] ++; 
			if(cerStepSrd[Axis].dir == CW)
        {		  	
          pmStep_position[Axis]++; // 绝对位置加1
        }
        else
        {
          pmStep_position[Axis]--; // 绝对位置减1
        }
        cerStepSrd[Axis].accel_count++;
        new_step_delay = cerStepSrd[Axis].step_delay - (((2 * cerStepSrd[Axis].step_delay) + rest[Axis])/(4 * cerStepSrd[Axis].accel_count + 1)); //计算新(下)一步脉冲周期(时间间隔)
        rest[Axis] = ((2 * cerStepSrd[Axis].step_delay)+rest[Axis])%(4 * cerStepSrd[Axis].accel_count + 1);// 计算余数，下次计算补上余数，减少误差
				if(listStopCmd[Axis]){
					listStopCmd[Axis] = False;
					cerStepSrd[Axis].accel_count = T1_FREQ/2/stpdecel*T1_FREQ/cerStepSrd[Axis].step_delay/cerStepSrd[Axis].step_delay*-1;
				}
        //检查是否为最后一步
        if(cerStepSrd[Axis].accel_count >= 0)
        {
            cerStepSrd[Axis].run_state = STOP;
        }
				if(cerStepSrd[Axis].medle_delay != 0) 
          {
            if(new_step_delay >= cerStepSrd[Axis].medle_delay)		//检查是否到达爬行速度
            {
              cerStepSrd[Axis].min_delay = cerStepSrd[Axis].medle_delay;//装载爬行速度
              new_step_delay = cerStepSrd[Axis].medle_delay;     //以爬行速度为下个延时
              cerStepSrd[Axis].decel_start = UINT32_MAX;       //在达到decel_start之前保持匀速 
              rest[Axis] = 0;       
              cerStepSrd[Axis].run_state = RUN;
            }		
          }
        break;
      default :break;
    }      
    if(new_step_delay>0xFFFF)
      new_step_delay=0xFFFF;
    cerStepSrd[Axis].step_delay = new_step_delay; // 为下个(新的)延时(脉冲周期)赋值
  }
}

void HAL_TIM_OC_Callback_TIM9(uint8_t Axis)
{
  __IO static uint16_t tim_count_cut = 0;
  __IO  uint32_t new_step_delay_cut = 0;
  __IO static uint8_t  i_cut[2]={0,0};
	__IO static uint16_t last_accel_delay_cut[2] = {0,0};
	// 总移动步数计数器
	__IO static uint32_t step_count_cut[2] = {0,0};
	// 记录new_step_delay中的余数，提高下一步计算的精度
	__IO static int32_t rest_cut[2] = {0,0};
	//定时器使用翻转模式，需要进入两次中断才输出一个完整脉冲

  // 设置比较值
  tim_count_cut = __HAL_TIM_GET_COUNTER(&cutMachine_STEPMOTOR);
  __HAL_TIM_SET_COMPARE(&cutMachine_STEPMOTOR,Timer9[Axis].Channel,tim_count_cut+cerCutStepSrd[Axis].step_delay);

  i_cut[Axis]++;       // 定时器中断次数计数值
  if(i_cut[Axis] == 2) // 2次，说明已经输出一个完整脉冲
  {
    i_cut[Axis] = 0;   // 清零定时器中断次数计数值

    switch(cerCutStepSrd[Axis].run_state) // 加减速曲线阶段
    {
      case STOP:
    
        // 关闭通道
        TIM_CCxChannelCmd(CUT_MACHINE_TIMx,Timer9[Axis].Channel,TIM_CCx_DISABLE);       
        __HAL_TIM_CLEAR_FLAG(&cutMachine_STEPMOTOR, Timer9[Axis].Flag_CCx);
        HAL_TIM_OC_Stop_IT (&cutMachine_STEPMOTOR,Timer9[Axis].Channel);
       // CER_STEPMOTOR_OUTPUT_DISABLE(Axis);
      
        cerCutMotionStatus[Axis] = STOP;  		//  电机为停止状态
        step_count_cut[Axis] = 0;  // 清零步数计数器
			  rest_cut[Axis] = 0;        // 清零余值
        last_accel_delay_cut[Axis] = 0;
        cerCutStepSrd[Axis].accel_count = 0;
        cerCutStepSrd[Axis].step_delay = 0;
        cerCutStepSrd[Axis].min_delay = 0;
        break;
      case ACCEL:
        step_count_cut[Axis] ++;      		// 步数加1
				listCutStep_count[Axis] ++;  
			if(cerCutStepSrd[Axis].dir == CW)
        {		  	
          pmCutStep_position[Axis]++; 	  // 绝对位置加1
        }
        else
        {
          pmCutStep_position[Axis]--; 	  // 绝对位置减1
        }
        cerCutStepSrd[Axis].accel_count++; 	  // 加速计数值加1
        
        new_step_delay_cut = cerCutStepSrd[Axis].step_delay - (((2 *cerCutStepSrd[Axis].step_delay) + rest_cut[Axis])/(4 * cerCutStepSrd[Axis].accel_count + 1));//计算新(下)一步脉冲周期(时间间隔)
        rest_cut[Axis] = ((2 * cerCutStepSrd[Axis].step_delay)+rest_cut[Axis])%(4 * cerCutStepSrd[Axis].accel_count + 1);// 计算余数，下次计算补上余数，减少误差
        
        if(step_count_cut[Axis] >= cerCutStepSrd[Axis].decel_start || listCutStepStopCmd[Axis])		// 检查是否应该开始减速
        {
					if(listCutStepStopCmd[Axis]){
					 listCutStepStopCmd[Axis] = False;
						cerCutStepSrd[Axis].accel_count = T1_FREQ/2/stpdecel*T1_FREQ/cerCutStepSrd[Axis].step_delay/cerCutStepSrd[Axis].step_delay*-1;
					}else{
					cerCutStepSrd[Axis].accel_count = cerCutStepSrd[Axis].decel_val; 	// 加速计数值为减速阶段计数值的初始值
					}
          cerCutStepSrd[Axis].run_state = DECEL;           	      // 下个脉冲进入减速阶段
        }
        else if(new_step_delay_cut <= cerCutStepSrd[Axis].min_delay)  // 检查是否到达期望的最大速度
        {
          cerCutStepSrd[Axis].accel_count = cerCutStepSrd[Axis].decel_val; 	// 加速计数值为减速阶段计数值的初始值
          last_accel_delay_cut[Axis] = new_step_delay_cut; 	// 保存加速过程中最后一次延时（脉冲周期）
          new_step_delay_cut = cerCutStepSrd[Axis].min_delay;    	// 使用min_delay（对应最大速度speed）
          rest_cut[Axis] = 0;                          	// 清零余值
          cerCutStepSrd[Axis].run_state = RUN;               	// 设置为匀速运行状态
        }
        last_accel_delay_cut[Axis] = new_step_delay_cut; 	  // 保存加速过程中最后一次延时（脉冲周期）
        break;
        
      case RUN:
        step_count_cut[Axis]++; 		 // 步数加1
        listCutStep_count[Axis] ++; 
			if(cerCutStepSrd[Axis].dir==CW)
        {	  	
          pmCutStep_position[Axis]++; // 绝对位置加1
        }
        else
        {
          pmCutStep_position[Axis]--; // 绝对位置减1
        }
            
        new_step_delay_cut = cerCutStepSrd[Axis].min_delay;       // 使用min_delay（对应最大速度speed）
            
        if(step_count_cut[Axis] >= cerCutStepSrd[Axis].decel_start|| listCutStepStopCmd[Axis])   // 需要开始减速
        {
					if(listCutStepStopCmd[Axis]){
						listCutStepStopCmd[Axis] = False;
						cerCutStepSrd[Axis].accel_count = T1_FREQ/2/stpdecel*T1_FREQ/cerCutStepSrd[Axis].step_delay/cerCutStepSrd[Axis].step_delay*-1;
					}else{
          cerCutStepSrd[Axis].accel_count = cerCutStepSrd[Axis].decel_val;  // 减速步数做为加速计数值
          }
					new_step_delay_cut = last_accel_delay_cut[Axis];// 加阶段最后的延时做为减速阶段的起始延时(脉冲周期)
          cerCutStepSrd[Axis].run_state = DECEL;            // 状态改变为减速
        }
        break;
        
      case DECEL:
        step_count_cut[Axis]++; 		 // 步数加1
       listCutStep_count[Axis] ++; 
			if(cerCutStepSrd[Axis].dir == CW)
        {		  	
          pmCutStep_position[Axis]++; // 绝对位置加1
        }
        else
        {
          pmCutStep_position[Axis]--; // 绝对位置减1
        }
        cerCutStepSrd[Axis].accel_count++;
        new_step_delay_cut = cerCutStepSrd[Axis].step_delay - (((2 * cerCutStepSrd[Axis].step_delay) + rest_cut[Axis])/(4 * cerCutStepSrd[Axis].accel_count + 1)); //计算新(下)一步脉冲周期(时间间隔)
        rest_cut[Axis] = ((2 * cerCutStepSrd[Axis].step_delay)+rest_cut[Axis])%(4 * cerCutStepSrd[Axis].accel_count + 1);// 计算余数，下次计算补上余数，减少误差
				if(listStopCmd[Axis]){
					listStopCmd[Axis] = False;
					cerCutStepSrd[Axis].accel_count = T1_FREQ/2/stpdecel*T1_FREQ/cerCutStepSrd[Axis].step_delay/cerCutStepSrd[Axis].step_delay*-1;
				}
        //检查是否为最后一步
        if(cerCutStepSrd[Axis].accel_count >= 0)
        {
            cerCutStepSrd[Axis].run_state = STOP;
        }
				if(cerCutStepSrd[Axis].medle_delay != 0) 
          {
            if(new_step_delay_cut >= cerCutStepSrd[Axis].medle_delay)		//检查是否到达爬行速度
            {
              cerCutStepSrd[Axis].min_delay = cerCutStepSrd[Axis].medle_delay;//装载爬行速度
              new_step_delay_cut = cerCutStepSrd[Axis].medle_delay;     //以爬行速度为下个延时
              cerCutStepSrd[Axis].decel_start = UINT32_MAX;       //在达到decel_start之前保持匀速 
              rest_cut[Axis] = 0;       
              cerCutStepSrd[Axis].run_state = RUN;
            }		
          }
        break;
      default :break;
    }      
    if(new_step_delay_cut>0xFFFF)
      new_step_delay_cut=0xFFFF;
    cerCutStepSrd[Axis].step_delay = new_step_delay_cut; // 为下个(新的)延时(脉冲周期)赋值
  }
}



static void RollerStepOutPutControl(uint8_t status)
{
	if(status == 0){ //高电平使能还是低电平使能
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
  uint16_t Step_Delay = 0;  //步进延时

  if(Frequency == 0)
    RollerStepOutPutControl(0);
  else
  {
    if(Dir==CCW)
    {
      RollerStepDirControl(CCW);
    }
    else RollerStepDirControl(CW);//方向控制
    /*
      步进电机速度由定时器输出脉冲频率(f)决定,
        f = c/F;c是计数器的计数值,F是定时器频率
      推导过程:(T是定时器输出脉冲周期)
        T=c*t => T=c*F => f = 1/T = F/c;
    
    */
    Step_Delay = (uint16_t)(FREQ_UINT/Frequency);
    
    Toggle_Pulse = Step_Delay>>1;//算出来的结果是周期,这里除以2,半周期翻转一次
  }
}


/**
  * 函数功能: 定时器比较输出中断回调函数
  * 输入参数: htim：定时器句柄指针
  * 返 回 值: 无
  * 说    明: 无
  */
//void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim)
//{
//  uint32_t count;
//  uint32_t tmp;
//  count=__HAL_TIM_GET_COUNTER(&cutMachine_STEPMOTOR);
//  tmp = STEPMOTOR_TIM_PERIOD & (count + Toggle_Pulse);
//  __HAL_TIM_SET_COMPARE(&cutMachine_STEPMOTOR,TIM_CHANNEL_1,tmp);
//}
