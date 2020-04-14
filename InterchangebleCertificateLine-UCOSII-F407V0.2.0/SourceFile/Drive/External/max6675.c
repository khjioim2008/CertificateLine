
/*-------------------------------<< ģ�� >>-----------------------------------*/
/*
 * - :CRJ-
 * - :201700721.001.00.1
 * - System->APP->Drive->CPU��ע�����˳��
 * - 4���ͺ�������ʼ�����ã����ƣ�״̬��ִ��
 * - 
 * - :
 **/
/*___________________________________END______________________________________*/
#include "Resources.h"
#include "max6675.h"

/**
  * ��������: ��ʼ��
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  */

static void initMax6675(void)
{
	GPIO_InitTypeDef GPIO_InitStruct; 

	SO_MAX6675IO_IN_GPIO_CLK_ENABLE();
	CS_MAX6675IO_OUT_GPIO_CLK_ENABLE();
	SCK_MAX6675IO_OUT_GPIO_CLK_ENABLE();
	
	GPIO_InitStruct.Pin = SO_MAX6675IO_IN_PIN; 
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT; 
  HAL_GPIO_Init(SO_MAX6675IO_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = CS_MAX6675IO_OUT_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  HAL_GPIO_Init(CS_MAX6675IO_PORT, &GPIO_InitStruct);
		
	GPIO_InitStruct.Pin = SCK_MAX6675IO_OUT_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  HAL_GPIO_Init(SCK_MAX6675IO_PORT, &GPIO_InitStruct);
}

static void initHC4053(void)
{
	GPIO_InitTypeDef GPIO_InitStruct; 
	SELCET_HC4053IO_OUT_GPIO_CLK_ENABLE();
	
	GPIO_InitStruct.Pin = SELCET_HC4053IO_OUT_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  HAL_GPIO_Init(SELCET_HC4053IO_PORT, &GPIO_InitStruct);
	
}

void initReadTemperature(void)
{
	initMax6675();
	
	initHC4053();
}
/**
  * ��������: ��ȡMax6675
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  */

static uint16_t readMax6675(void)
{
		uint8_t i;
		uint16_t dat =0;
	//CS  = 0; 
  HAL_GPIO_WritePin(CS_MAX6675IO_PORT,CS_MAX6675IO_OUT_PIN,GPIO_PIN_RESET);

	//SCK = 0;     
	HAL_GPIO_WritePin(SCK_MAX6675IO_PORT,SCK_MAX6675IO_OUT_PIN,GPIO_PIN_RESET);

	for(i=0; i<16; i++)		//get D15-D0 from 6675  
	{      
		//SCK = 1; 
    HAL_GPIO_WritePin(SCK_MAX6675IO_PORT,SCK_MAX6675IO_OUT_PIN,GPIO_PIN_SET);
		dat = dat<<1;    
		if(HAL_GPIO_ReadPin(SO_MAX6675IO_PORT,SO_MAX6675IO_IN_PIN) == 1)
		dat = dat|0x01;   
		//SCK = 0;
    HAL_GPIO_WritePin(SCK_MAX6675IO_PORT,SCK_MAX6675IO_OUT_PIN,GPIO_PIN_RESET);
	} 
	
	//CS = 1;   
	HAL_GPIO_WritePin(CS_MAX6675IO_PORT,CS_MAX6675IO_OUT_PIN,GPIO_PIN_SET);
  
	return dat;   
}

static void selectHc4053(uint8_t channel)
{
	if(channel == 1){
		HAL_GPIO_WritePin(SELCET_HC4053IO_PORT,SELCET_HC4053IO_OUT_PIN,GPIO_PIN_SET);
	
	}else{
		HAL_GPIO_WritePin(SELCET_HC4053IO_PORT,SELCET_HC4053IO_OUT_PIN,GPIO_PIN_RESET);
	
	}
}

int16_t readTemperature(uint8_t channel)
{
	uint16_t tmp = 0;
	uint8_t flagMax6675 = 0;
	int16_t nowTemp = 0;
	selectHc4053(channel);//2ѡ1
		
	tmp = readMax6675(); //��
	flagMax6675 = tmp & 0x04;		 	//�������ݵ�D2λ���ȵ�ż���߱�־λ����λΪ1��ʾ���ߣ���λΪ0��ʾ����
	flagMax6675 = flagMax6675 >> 2; 	//MAX6675�Ƿ�����
	
	if(flagMax6675 == 1){
		nowTemp = -1;//error���ⲻ�˸��µģ�������-1 
	}else{
		tmp = tmp << 1;					   	//�����������ݵ�D3~D14���¶�ֵ
		tmp = tmp >> 4;
		nowTemp = tmp/4;				//��õ��¶ȵ�λ��0.25������Ҫ����0.25��������4�����ܵõ��Զ�Ϊ��λ���¶�ֵ
	}
		return nowTemp;

}
