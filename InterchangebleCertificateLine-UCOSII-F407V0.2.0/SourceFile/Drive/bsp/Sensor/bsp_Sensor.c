/*-------------------------------<< >>-----------------------------------*/
/*
 * - :CRJ--
 * - :20170000.000.00.1  
 * - 
 * - IC 
 * - Level:0 ,1
 * - :STM32F407-
 **/
/*___________________________________END______________________________________*/

#include "Sensor/bsp_Sensor.h"

void initSensorIO(void)
{
	GPIO_InitTypeDef GPIO_InitStruct; 
	
	/******·ÖÒ³******/
	EXIST_PAPER_SENSORIO_IN_GPIO_CLK_ENABLE();
	FRICTION_ORIGIN_SENSORIO_IN_GPIO_CLK_ENABLE();
	FRICTION_OUT_SENSORIO_IN_GPIO_CLK_ENABLE();
	FRICTION_OUT_ONE_SENSORIO_IN_GPIO_CLK_ENABLE();
	FRICTION_OUT_TWO_SENSORIO_IN_GPIO_CLK_ENABLE();
	FRICTION_OUT_THREE_SENSORIO_IN_GPIO_CLK_ENABLE();
	FRICTION_OUT_FOUR_SENSORIO_IN_GPIO_CLK_ENABLE();
	ROLLER_OUT_SENSORIO_IN_GPIO_CLK_ENABLE();
	
	/******²ÃÇÐ******/
	
	CUTFLOTASM_POSTION_SENSORIO_IN_GPIO_CLK_ENABLE();
	LEFT_FLOTASMCUT_SENSORIO_IN_GPIO_CLK_ENABLE();
	RIGHT_FLOTASMCUT_SENSORIO_IN_GPIO_CLK_ENABLE();
	CHAMFER_POSTION_SENSORIO_IN_GPIO_CLK_ENABLE();
	CHAMFER_MOTOR_SENSORIO_IN_GPIO_CLK_ENABLE();
	CUTMACHINE_FRONT_SENSORIO_IN_GPIO_CLK_ENABLE();
	CUTMACHINE_BEHIND_SENSORIO_IN_GPIO_CLK_ENABLE();
	
	/*
	STAMPPOSTION_SENSORIO_IN_GPIO_CLK_ENABLE();
	STAMPACMOTOR_SENSORIO_IN_GPIO_CLK_ENABLE();
	STAMPWAIT_UP_SENSORIO_IN_GPIO_CLK_ENABLE();
	STAMPWAIT_DOWN_SENSORIO_IN_GPIO_CLK_ENABLE();
	*/
	
		/******·ÖÒ³******/
		
	GPIO_InitStruct.Pin = EXIST_PAPER_OUT_SENSORIO_IN_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  HAL_GPIO_Init(EXIST_PAPER_OUT_SENSORIO_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = FRICTION_ORIGIN_SENSORIO_IN_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  HAL_GPIO_Init(FRICTION_ORIGIN_SENSORIO_PORT, &GPIO_InitStruct);
  
	GPIO_InitStruct.Pin = FRICTION_OUT_SENSORIO_IN_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  HAL_GPIO_Init(FRICTION_OUT_SENSORIO_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = FRICTION_OUT_ONE_SENSORIO_IN_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  HAL_GPIO_Init(FRICTION_OUT_ONE_SENSORIO_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = FRICTION_OUT_TWO_SENSORIO_IN_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  HAL_GPIO_Init(FRICTION_OUT_TWO_SENSORIO_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = FRICTION_OUT_THREE_SENSORIO_IN_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  HAL_GPIO_Init(FRICTION_OUT_THREE_SENSORIO_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = FRICTION_OUT_FOUR_SENSORIO_IN_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  HAL_GPIO_Init(FRICTION_OUT_FOUR_SENSORIO_PORT, &GPIO_InitStruct);
			
	GPIO_InitStruct.Pin = ROLLER_OUT_SENSORIO_IN_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  HAL_GPIO_Init(ROLLER_OUT_SENSORIO_PORT, &GPIO_InitStruct);
	
	/******²ÃÇÐ******/
	GPIO_InitStruct.Pin = CUTFLOTASM_POSTION_SENSORIO_IN_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  HAL_GPIO_Init(CUTFLOTASM_POSTION_SENSORIO_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = LEFT_FLOTASMCUT_SENSORIO_IN_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  HAL_GPIO_Init(LEFT_FLOTASMCUT_SENSORIO_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = RIGHT_FLOTASMCUT_SENSORIO_IN_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  HAL_GPIO_Init(RIGHT_FLOTASMCUT_SENSORIO_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = CHAMFER_POSTION_SENSORIO_IN_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  HAL_GPIO_Init(CHAMFER_POSTION_SENSORIO_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = CHAMFER_MOTOR_SENSORIO_IN_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  HAL_GPIO_Init(CHAMFER_MOTOR_SENSORIO_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = CUTMACHINE_FRONT_SENSORIO_IN_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  HAL_GPIO_Init(CUTMACHINE_FRONT_SENSORIO_PORT, &GPIO_InitStruct);
	
	
	GPIO_InitStruct.Pin = CUTMACHINE_BEHIND_SENSORIO_IN_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  HAL_GPIO_Init(CUTMACHINE_BEHIND_SENSORIO_PORT, &GPIO_InitStruct);
	
	/*
	GPIO_InitStruct.Pin = STAMPPOSTION_SENSORIO_IN_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  HAL_GPIO_Init(STAMPPOSTION_SENSORIO_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = STAMPACMOTOR_SENSORIO_IN_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  HAL_GPIO_Init(STAMPACMOTOR_SENSORIO_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = STAMPWAIT_UP_SENSORIO_IN_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  HAL_GPIO_Init(STAMPWAIT_UP_SENSORIO_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = STAMPWAIT_DOWN_SENSORIO_IN_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  HAL_GPIO_Init(STAMPWAIT_DOWN_SENSORIO_PORT, &GPIO_InitStruct);
	*/
	
	
}


/*--------------------------Sign---------------------------------*/
void signalStability(Sign *Name,N8 (* fun)(void))
{
        N8 temp = fun();

		Name->Detect = 5;

        if(temp != Name->SIOB) {
                Name->DetectV	= 0;
                Name->SIOB		= temp;
        }
        if(Name->DetectV < Name->Detect) Name->DetectV ++;
        else {Name->DetectV = 255;Name->SIO = Name->SIOB;}
}

/*--------------------------Sign---------------------------------*/
void signalStabilityT(Sign *Name,N8 (* fun)(void),N16 DeTemp)
{
				N8 temp = fun();
				Name->Detect = DeTemp;

        if(temp != Name->SIOB) {
                Name->DetectV	= 0;
                Name->SIOB		= temp;
        }
        if(Name->DetectV < Name->Detect) Name->DetectV ++;
        else {Name->DetectV = 255;Name->SIO = Name->SIOB;}
}

/******·ÖÒ³******/
uint8_t sensorPagingExistPaper(void)
{
		return !!(EXIST_PAPER_OUT_SENSORIO_PORT->IDR & EXIST_PAPER_OUT_SENSORIO_IN_PIN);
}

uint8_t sensorPagingFrictionOrigin(void)
{
		return !(FRICTION_ORIGIN_SENSORIO_PORT->IDR & FRICTION_ORIGIN_SENSORIO_IN_PIN);
}

uint8_t sensorPagingFrictionOut(void)
{
		return !!(FRICTION_OUT_SENSORIO_PORT->IDR & FRICTION_OUT_SENSORIO_IN_PIN);
}

uint8_t sensorPagingFrictionOutOne(void)
{
		return !!(FRICTION_OUT_ONE_SENSORIO_PORT->IDR & FRICTION_OUT_ONE_SENSORIO_IN_PIN);
}

uint8_t sensorPagingFrictionOutTwo(void)
{
		return !!(FRICTION_OUT_TWO_SENSORIO_PORT->IDR & FRICTION_OUT_TWO_SENSORIO_IN_PIN);
}

uint8_t sensorPagingFrictionOutThree(void)
{
		return !!(FRICTION_OUT_THREE_SENSORIO_PORT->IDR & FRICTION_OUT_THREE_SENSORIO_IN_PIN);
}

uint8_t sensorPagingFrictionOutFour(void)
{
		return !!(FRICTION_OUT_FOUR_SENSORIO_PORT->IDR & FRICTION_OUT_FOUR_SENSORIO_IN_PIN);
}

uint8_t sensorPagingRollerOut(void)
{
		return !(ROLLER_OUT_SENSORIO_PORT->IDR & ROLLER_OUT_SENSORIO_IN_PIN);
}


/******²ÃÇÐ******/

uint8_t sensorCutFlotasmPostion(void)
{
	return !(CUTFLOTASM_POSTION_SENSORIO_PORT->IDR & CUTFLOTASM_POSTION_SENSORIO_IN_PIN);

}

uint8_t sensorCutLeftFlotasm(void)
{
	return !(LEFT_FLOTASMCUT_SENSORIO_PORT->IDR & LEFT_FLOTASMCUT_SENSORIO_IN_PIN);

}

uint8_t sensorCutRightFlotasm(void)
{
	return !(RIGHT_FLOTASMCUT_SENSORIO_PORT->IDR & RIGHT_FLOTASMCUT_SENSORIO_IN_PIN);

}

uint8_t sensorCutChamferMotor(void)
{
	return !(CHAMFER_MOTOR_SENSORIO_PORT->IDR & CHAMFER_MOTOR_SENSORIO_IN_PIN);

}

uint8_t sensorCutChamferPostion(void)
{
	return !(CHAMFER_POSTION_SENSORIO_PORT->IDR & CHAMFER_POSTION_SENSORIO_IN_PIN);

}

uint8_t sensorCutMachineFront(void)
{
	return !(CUTMACHINE_FRONT_SENSORIO_PORT->IDR & CUTMACHINE_FRONT_SENSORIO_IN_PIN);

}

uint8_t sensorCutMachineBehind(void)
{
	return !(CUTMACHINE_BEHIND_SENSORIO_PORT->IDR & CUTMACHINE_BEHIND_SENSORIO_IN_PIN);

}


/******¸ÖÓ¡******/

uint8_t sensorStampPostion(void)
{
	return !(STAMPPOSTION_SENSORIO_PORT->IDR & STAMPPOSTION_SENSORIO_IN_PIN);
}

uint8_t sensorStampACMotor(void)
{
		return !(STAMPACMOTOR_SENSORIO_PORT->IDR & STAMPACMOTOR_SENSORIO_IN_PIN);
}

uint8_t sensorStampWaitUp(void)
{
		return !(STAMPWAIT_UP_SENSORIO_PORT->IDR & STAMPWAIT_UP_SENSORIO_IN_PIN);
}

uint8_t sensorStampWaitDown(void)
{
		return !(STAMPWAIT_DOWN_SENSORIO_PORT->IDR & STAMPWAIT_DOWN_SENSORIO_IN_PIN);
}

