/**
  ******************************************************************************
  * @file    stm32f4xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  *
  * COPYRIGHT(c) 2017 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"
#include "stm32f4xx_it.h"

/* USER CODE BEGIN 0 */
#include "StepMotor/bsp_STEPMOTOR.h"
#include "usartx/bsp_usartx.h"
#include "EncoderTIM/bsp_EncoderTIM.h"
/* USER CODE END 0 */
#include "includes.h"
/* External variables --------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Interruption and Exception Handlers         */ 
/******************************************************************************/

/**
* @brief This function handles Non maskable interrupt.
*/
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */

  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
* @brief This function handles Hard fault interrupt.
*/
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
		
  }
  /* USER CODE BEGIN HardFault_IRQn 1 */

  /* USER CODE END HardFault_IRQn 1 */
}

/**
* @brief This function handles Memory management fault.
*/
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
  }
  /* USER CODE BEGIN MemoryManagement_IRQn 1 */

  /* USER CODE END MemoryManagement_IRQn 1 */
}

/**
* @brief This function handles Pre-fetch fault, memory access fault.
*/
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
  }
  /* USER CODE BEGIN BusFault_IRQn 1 */

  /* USER CODE END BusFault_IRQn 1 */
}

/**
* @brief This function handles Undefined instruction or illegal state.
*/
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
  }
  /* USER CODE BEGIN UsageFault_IRQn 1 */

  /* USER CODE END UsageFault_IRQn 1 */
}

/**
* @brief This function handles System service call via SWI instruction.
*/
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVCall_IRQn 0 */

  /* USER CODE END SVCall_IRQn 0 */
  /* USER CODE BEGIN SVCall_IRQn 1 */

  /* USER CODE END SVCall_IRQn 1 */
}

/**
* @brief This function handles Debug monitor.
*/
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
* @brief This function handles Pendable request for system service.
*/
//void PendSV_Handler(void)
//{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
//}

/**
* @brief This function handles System tick timer.
*/
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  HAL_SYSTICK_IRQHandler();
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
	

}
/* USER CODE BEGIN 1 */
//void STEPMOTOR_TIMx_IRQHandler(void)
//{
//  HAL_TIM_IRQHandler(&htimx_STEPMOTOR);
//}

/**
* @brief This function handles DMA1 channel4 global interrupt.
*/
void DMAx_STREAMx_IRQHANDLER(void)
{
  /* USER CODE BEGIN DMA1_Channel4_IRQn 0 */

  /* USER CODE END DMA1_Channel4_IRQn 0 */
	OSIntEnter();
  HAL_DMA_IRQHandler(&hdma_usartx_tx);
  OSIntExit();

  /* USER CODE BEGIN DMA1_Channel4_IRQn 1 */

  /* USER CODE END DMA1_Channel4_IRQn 1 */
}
/**
* @brief This function handles USART1 global interrupt.
*/
void USARTx_IRQHANDLER(void)
{
  /* USER CODE BEGIN USART1_IRQn 0 */

  /* USER CODE END USART1_IRQn 0 */
	
	OSIntEnter();
  HAL_UART_IRQHandler(&husartx);
  OSIntExit();
	/* USER CODE BEGIN USART1_IRQn 1 */

  /* USER CODE END USART1_IRQn 1 */
}
void ENCODER_TIM_IRQHANDLER(void)
{
			OSIntEnter();
    HAL_TIM_IRQHandler(&htimx_Encoder);
		  OSIntExit();

}

void ENCODER_TWO_TIM_IRQHANDLER(void)
{
			OSIntEnter();
  HAL_TIM_IRQHandler(&htimx_EncoderTwo);
		  OSIntExit();

}

void STEPMOTOR_TIMx_IRQHandler(void)
{
		OSIntEnter();

  TIM_HandleTypeDef *htim = &htimx_STEPMOTOR;
    /* Capture compare 1 event */
  if(__HAL_TIM_GET_FLAG(htim, TIM_FLAG_CC1) != RESET)
  {   
      __HAL_TIM_CLEAR_IT(htim, TIM_IT_CC1);
      /* Output compare event */
      //HAL_TIM_OC_Callback(AXIS_X); 
	  	HAL_TIM_OC_Callback(CUT_FLASM_STEP);
  }
  /* Capture compare 2 event */
  if(__HAL_TIM_GET_FLAG(htim, TIM_FLAG_CC2) != RESET)
  {
      __HAL_TIM_CLEAR_IT(htim, TIM_IT_CC2);
      /* Output compare event */
      HAL_TIM_OC_Callback(PLASTIC_STEP);
  }
  /* Capture compare 3 event */
  if(__HAL_TIM_GET_FLAG(htim, TIM_FLAG_CC3) != RESET)
  {
      __HAL_TIM_CLEAR_IT(htim, TIM_IT_CC3);
      /* Output compare event */
//      HAL_TIM_OC_Callback(AXIS_Z);
		   HAL_TIM_OC_Callback(FRICTION_STEP);
  }
	
  /* Capture compare 4 event */
  if(__HAL_TIM_GET_FLAG(htim, TIM_FLAG_CC4) != RESET)
  {
      __HAL_TIM_CLEAR_IT(htim, TIM_IT_CC4);
      // Output compare event 
//      HAL_TIM_OC_Callback(AXIS_R);
	  	HAL_TIM_OC_Callback(ROLLER_STEP);
		 // HAL_TIM_OC_DelayElapsedCallback(htim);
  }

	  OSIntExit();
}


void CUT_MACHINE_TIMx_IRQHandler(void)
{
	OSIntEnter();
  TIM_HandleTypeDef *htim = &cutMachine_STEPMOTOR;
    /* Capture compare 1 event */
  if(__HAL_TIM_GET_FLAG(htim, TIM_FLAG_CC1) != RESET)
  {   
      __HAL_TIM_CLEAR_IT(htim, TIM_IT_CC1);
      /* Output compare event */
      //HAL_TIM_OC_Callback(AXIS_X); 
	  	HAL_TIM_OC_Callback_TIM9(0);
  }
  /* Capture compare 2 event */
  if(__HAL_TIM_GET_FLAG(htim, TIM_FLAG_CC2) != RESET)
  {
      __HAL_TIM_CLEAR_IT(htim, TIM_IT_CC2);
      /* Output compare event */
      HAL_TIM_OC_Callback_TIM9(1);
  }
  OSIntExit();
}
/******************************************************************************/
/* STM32F4xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f4xx.s).                    */
/******************************************************************************/

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
