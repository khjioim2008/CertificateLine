
/*-------------------------------<< 模块 >>-----------------------------------*/
/*
 * - :CRJ-
 * - :201700721.001.00.1
 * - System->APP->Drive->CPU，注意包含顺序
 * - 4类型函数，初始化配置，控制，状态，执行
 * - 
 * - :
 **/
/*___________________________________END______________________________________*/
#include "Resources.h"
#include "max6675.h"

/**
  * 函数功能: 初始化
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
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
  * 函数功能: 读取Max6675
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
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
	selectHc4053(channel);//2选1
		
	tmp = readMax6675(); //读
	flagMax6675 = tmp & 0x04;		 	//读出数据的D2位是热电偶掉线标志位，该位为1表示掉线，该位为0表示连接
	flagMax6675 = flagMax6675 >> 2; 	//MAX6675是否在线
	
	if(flagMax6675 == 1){
		nowTemp = -1;//error，测不了负温的，所以用-1 
	}else{
		tmp = tmp << 1;					   	//读出来的数据的D3~D14是温度值
		tmp = tmp >> 4;
		nowTemp = tmp/4;				//测得的温度单位是0.25，所以要乘以0.25（即除以4）才能得到以度为单位的温度值
	}
		return nowTemp;

}
