/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "EncoderTIM/bsp_EncoderTIM.h"
/* ˽�����Ͷ��� --------------------------------------------------------------*/
/* ˽�к궨�� ----------------------------------------------------------------*/
/* ˽�б��� ------------------------------------------------------------------*/
int16_t OverflowCount = 0;//��ʱ���������
int16_t OverflowCountTwo = 0;
/* Timer handler declaration */
TIM_HandleTypeDef    htimx_Encoder;
TIM_HandleTypeDef    htimx_EncoderTwo;
/* Timer Encoder Configuration Structure declaration */
TIM_Encoder_InitTypeDef sEncoderConfig;
TIM_Encoder_InitTypeDef sEncoderConfigTwo;

/* ��չ���� ------------------------------------------------------------------*/
/* ˽�к���ԭ�� --------------------------------------------------------------*/
/* ������ --------------------------------------------------------------------*/
/**
  * ��������: ͨ�ö�ʱ����ʼ��������ͨ��PWM���
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  */
void ENCODER_TIMx_Init(void)
{    
  htimx_Encoder.Instance = ENCODER_TIMx;
  htimx_Encoder.Init.Prescaler = ENCODER_TIM_PRESCALER;
  htimx_Encoder.Init.CounterMode = TIM_COUNTERMODE_UP;
  htimx_Encoder.Init.Period = ENCODER_TIM_PERIOD;
  htimx_Encoder.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
  //HAL_TIM_Base_Init(&htimx_Encoder);
  	
	//����
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
	
	
	//����
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
  __HAL_TIM_CLEAR_IT(&htimx_Encoder, TIM_IT_UPDATE);  //��������жϱ�־λ
  __HAL_TIM_URS_ENABLE(&htimx_Encoder);               //���������������Ų��������ж�
  __HAL_TIM_ENABLE_IT(&htimx_Encoder,TIM_IT_UPDATE);  //ʹ�ܸ����ж�
 
	  __HAL_TIM_CLEAR_IT(&htimx_EncoderTwo, TIM_IT_UPDATE);  //��������жϱ�־λ
  __HAL_TIM_URS_ENABLE(&htimx_EncoderTwo);               //���������������Ų��������ж�
  __HAL_TIM_ENABLE_IT(&htimx_EncoderTwo,TIM_IT_UPDATE);  //ʹ�ܸ����ж�
   
	
  HAL_NVIC_SetPriority(ENCODER_TIM_IRQn, 1, 1);
  HAL_NVIC_EnableIRQ(ENCODER_TIM_IRQn);
	
	HAL_NVIC_SetPriority(ENCODER_TWO_TIM_IRQn, 1, 2);
  HAL_NVIC_EnableIRQ(ENCODER_TWO_TIM_IRQn);
}

/**
  * ��������: ������ʱ��Ӳ����ʼ������
  * �������: htim_base��������ʱ���������ָ��
  * �� �� ֵ: ��
  * ˵    ��: �ú�����HAL���ڲ�����
  */
void HAL_TIM_Encoder_MspInit(TIM_HandleTypeDef* htim_base)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  if(htim_base->Instance==ENCODER_TIMx)
  {
    /* ������ʱ������ʱ��ʹ�� */
    ENCODER_TIM_RCC_CLK_ENABLE();
    ENCODER_TIM_GPIO_CLK_ENABLE();

    /* ��ʱ��ͨ��1��������IO��ʼ�� */
    GPIO_InitStruct.Pin = ENCODER_TIM_CH1_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull=GPIO_PULLUP;
    GPIO_InitStruct.Alternate = GPIO_CH1_AFx_TIMx;
    HAL_GPIO_Init(ENCODER_TIM_CH1_GPIO, &GPIO_InitStruct);
    
    /* ��ʱ��ͨ��2��������IO��ʼ�� */
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
  * ��������: ������ʱ��Ӳ������ʼ������
  * �������: htim_base��������ʱ���������ָ��
  * �� �� ֵ: ��
  * ˵    ��: �ú�����HAL���ڲ�����
  */
void HAL_TIM_Encoder_MspDeInit(TIM_HandleTypeDef* htim_base)
{
  if(htim_base->Instance==ENCODER_TIMx)
  {
    /* ������ʱ������ʱ�ӽ��� */
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
    OverflowCount--;       //���¼������
	}
  else{
    OverflowCount++;       //���ϼ������
	}
}

if(htim == &htimx_EncoderTwo){

	if(__HAL_TIM_IS_TIM_COUNTING_DOWN(&htimx_EncoderTwo)){
			OverflowCountTwo--;       //���¼������
		}
		else{
			OverflowCountTwo++;       //���ϼ������
		}
		
	}


}
/******************* (C) COPYRIGHT 2015-2020 ӲʯǶ��ʽ�����Ŷ� *****END OF FILE****/
