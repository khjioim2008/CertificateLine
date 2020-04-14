/* 包含头文件 ----------------------------------------------------------------*/
#include "EncoderTIM/bsp_EncoderTIM.h"
/* 私有类型定义 --------------------------------------------------------------*/
/* 私有宏定义 ----------------------------------------------------------------*/
/* 私有变量 ------------------------------------------------------------------*/
int16_t OverflowCount = 0;//定时器溢出次数
int16_t OverflowCountTwo = 0;
/* Timer handler declaration */
TIM_HandleTypeDef    htimx_Encoder;
TIM_HandleTypeDef    htimx_EncoderTwo;
/* Timer Encoder Configuration Structure declaration */
TIM_Encoder_InitTypeDef sEncoderConfig;
TIM_Encoder_InitTypeDef sEncoderConfigTwo;

/* 扩展变量 ------------------------------------------------------------------*/
/* 私有函数原形 --------------------------------------------------------------*/
/* 函数体 --------------------------------------------------------------------*/
/**
  * 函数功能: 通用定时器初始化并配置通道PWM输出
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  */
void ENCODER_TIMx_Init(void)
{    
  htimx_Encoder.Instance = ENCODER_TIMx;
  htimx_Encoder.Init.Prescaler = ENCODER_TIM_PRESCALER;
  htimx_Encoder.Init.CounterMode = TIM_COUNTERMODE_UP;
  htimx_Encoder.Init.Period = ENCODER_TIM_PERIOD;
  htimx_Encoder.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
  //HAL_TIM_Base_Init(&htimx_Encoder);
  	
	//新增
	htimx_EncoderTwo.Instance = ENCODER_TWO_TIMx;
  htimx_EncoderTwo.Init.Prescaler = ENCODER_TWO_TIM_PRESCALER;
  htimx_EncoderTwo.Init.CounterMode = TIM_COUNTERMODE_UP;
  htimx_EncoderTwo.Init.Period = ENCODER_TWO_TIM_PERIOD;
  htimx_EncoderTwo.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
	//HAL_TIM_Base_Init(&htimx_EncoderTwo);
	
  sEncoderConfig.EncoderMode        = TIM_ENCODERMODE_TIx;  
  
  sEncoderConfig.IC1Polarity        = TIM_ICPOLARITY_RISING;   
  sEncoderConfig.IC1Selection       = TIM_ICSELECTION_DIRECTTI;  
  sEncoderConfig.IC1Prescaler       = TIM_ICPSC_DIV1; 
  sEncoderConfig.IC1Filter          = 0;
  
  sEncoderConfig.IC2Polarity        = TIM_ICPOLARITY_RISING;   
  sEncoderConfig.IC2Selection       = TIM_ICSELECTION_DIRECTTI;  
  sEncoderConfig.IC2Prescaler       = TIM_ICPSC_DIV1; 
  sEncoderConfig.IC2Filter          = 0;
	
	
	//新增
	sEncoderConfigTwo.EncoderMode        = TIM_ENCODERTWO_MODE_TIx;  
  
  sEncoderConfigTwo.IC1Polarity        = TIM_ICPOLARITY_RISING;   
  sEncoderConfigTwo.IC1Selection       = TIM_ICSELECTION_DIRECTTI;  
  sEncoderConfigTwo.IC1Prescaler       = TIM_ICPSC_DIV1; 
  sEncoderConfigTwo.IC1Filter          = 0;
  
  sEncoderConfigTwo.IC2Polarity        = TIM_ICPOLARITY_RISING;   
  sEncoderConfigTwo.IC2Selection       = TIM_ICSELECTION_DIRECTTI;  
  sEncoderConfigTwo.IC2Prescaler       = TIM_ICPSC_DIV1; 
  sEncoderConfigTwo.IC2Filter          = 0;
	
	
  __HAL_TIM_SET_COUNTER(&htimx_Encoder,0);
  
  HAL_TIM_Encoder_Init(&htimx_Encoder, &sEncoderConfig);
	
  
	__HAL_TIM_SET_COUNTER(&htimx_EncoderTwo,0);
  
  HAL_TIM_Encoder_Init(&htimx_EncoderTwo, &sEncoderConfigTwo);
  
  //
  __HAL_TIM_CLEAR_IT(&htimx_Encoder, TIM_IT_UPDATE);  //清除更新中断标志位
  __HAL_TIM_URS_ENABLE(&htimx_Encoder);               //仅允许计数器溢出才产生更新中断
  __HAL_TIM_ENABLE_IT(&htimx_Encoder,TIM_IT_UPDATE);  //使能更新中断
 
	  __HAL_TIM_CLEAR_IT(&htimx_EncoderTwo, TIM_IT_UPDATE);  //清除更新中断标志位
  __HAL_TIM_URS_ENABLE(&htimx_EncoderTwo);               //仅允许计数器溢出才产生更新中断
  __HAL_TIM_ENABLE_IT(&htimx_EncoderTwo,TIM_IT_UPDATE);  //使能更新中断
   
	
  HAL_NVIC_SetPriority(ENCODER_TIM_IRQn, 1, 1);
  HAL_NVIC_EnableIRQ(ENCODER_TIM_IRQn);
	
	HAL_NVIC_SetPriority(ENCODER_TWO_TIM_IRQn, 1, 2);
  HAL_NVIC_EnableIRQ(ENCODER_TWO_TIM_IRQn);
}

/**
  * 函数功能: 基本定时器硬件初始化配置
  * 输入参数: htim_base：基本定时器句柄类型指针
  * 返 回 值: 无
  * 说    明: 该函数被HAL库内部调用
  */
void HAL_TIM_Encoder_MspInit(TIM_HandleTypeDef* htim_base)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  if(htim_base->Instance==ENCODER_TIMx)
  {
    /* 基本定时器外设时钟使能 */
    ENCODER_TIM_RCC_CLK_ENABLE();
    ENCODER_TIM_GPIO_CLK_ENABLE();

    /* 定时器通道1功能引脚IO初始化 */
    GPIO_InitStruct.Pin = ENCODER_TIM_CH1_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull=GPIO_PULLUP;
    GPIO_InitStruct.Alternate = GPIO_CH1_AFx_TIMx;
    HAL_GPIO_Init(ENCODER_TIM_CH1_GPIO, &GPIO_InitStruct);
    
    /* 定时器通道2功能引脚IO初始化 */
    GPIO_InitStruct.Pin = ENCODER_TIM_CH2_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Alternate = GPIO_CH2_AFx_TIMx;
    HAL_GPIO_Init(ENCODER_TIM_CH2_GPIO, &GPIO_InitStruct);
  }
	  if(htim_base->Instance==ENCODER_TWO_TIMx)
  {
    ENCODER_TWO_TIM_RCC_CLK_ENABLE();
    ENCODER_TWO_TIM_GPIO_CLK_ENABLE();

    GPIO_InitStruct.Pin = ENCODER_TWO_TIM_CH1_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Alternate = ENCODER_TWO_GPIO_CH1_AFx_TIMx;
    HAL_GPIO_Init(ENCODER_TWO_TIM_CH1_GPIO, &GPIO_InitStruct);
    
    GPIO_InitStruct.Pin = ENCODER_TWO_TIM_CH2_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Alternate = ENCODER_TWO_GPIO_CH2_AFx_TIMx;
    HAL_GPIO_Init(ENCODER_TWO_TIM_CH2_GPIO, &GPIO_InitStruct);
	}

	
}

/**
  * 函数功能: 基本定时器硬件反初始化配置
  * 输入参数: htim_base：基本定时器句柄类型指针
  * 返 回 值: 无
  * 说    明: 该函数被HAL库内部调用
  */
void HAL_TIM_Encoder_MspDeInit(TIM_HandleTypeDef* htim_base)
{
  if(htim_base->Instance==ENCODER_TIMx)
  {
    /* 基本定时器外设时钟禁用 */
    ENCODER_TIM_RCC_CLK_DISABLE();
    
    HAL_GPIO_DeInit(ENCODER_TIM_CH1_GPIO, ENCODER_TIM_CH1_PIN);
    HAL_GPIO_DeInit(ENCODER_TIM_CH2_GPIO, ENCODER_TIM_CH2_PIN);
  }
	  if(htim_base->Instance==ENCODER_TWO_TIMx)
  {
    ENCODER_TWO_TIM_RCC_CLK_ENABLE();
    
    HAL_GPIO_DeInit(ENCODER_TWO_TIM_CH1_GPIO, ENCODER_TWO_TIM_CH1_PIN);
    HAL_GPIO_DeInit(ENCODER_TWO_TIM_CH2_GPIO, ENCODER_TWO_TIM_CH2_PIN);
  }
} 




void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
if(htim == &htimx_Encoder){
  if(__HAL_TIM_IS_TIM_COUNTING_DOWN(&htimx_Encoder)){
    OverflowCount--;       //向下计数溢出
	}
  else{
    OverflowCount++;       //向上计数溢出
	}
}

if(htim == &htimx_EncoderTwo){

	if(__HAL_TIM_IS_TIM_COUNTING_DOWN(&htimx_EncoderTwo)){
			OverflowCountTwo--;       //向下计数溢出
		}
		else{
			OverflowCountTwo++;       //向上计数溢出
		}
		
	}


}
/******************* (C) COPYRIGHT 2015-2020 硬石嵌入式开发团队 *****END OF FILE****/
