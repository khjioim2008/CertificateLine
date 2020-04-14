#ifndef __BSP_USARTX_H__
#define __BSP_USARTX_H__

#include "stm32f4xx_hal.h"
#include <stdio.h>
#include <string.h>


#define USARTx                                 USART2
#define USARTx_BAUDRATE                        115200
#define USART_RCC_CLK_ENABLE()                 __HAL_RCC_USART2_CLK_ENABLE()
#define USART_RCC_CLK_DISABLE()                __HAL_RCC_USART2_CLK_DISABLE()

#define USARTx_Tx_GPIO_ClK_ENABLE()            __HAL_RCC_GPIOD_CLK_ENABLE()
#define USARTx_Tx_GPIO_PORT                    GPIOD
#define USARTx_Tx_GPIO_PIN                     GPIO_PIN_5
#define GPIO_SET_AF_USARTx                     GPIO_AF7_USART2

#define USARTx_Rx_GPIO_ClK_ENABLE()            __HAL_RCC_GPIOD_CLK_ENABLE()
#define USARTx_Rx_GPIO_PORT                    GPIOD
#define USARTx_Rx_GPIO_PIN                     GPIO_PIN_6
#define GPIO_SET_AF_USARTx                     GPIO_AF7_USART2


#define USARTx_IRQHANDLER                      USART2_IRQHandler
#define USARTx_IRQn                            USART2_IRQn

// ??DMA??
#define DMAx_STREAMx                           DMA1_Stream6
#define DMAx_CHANNELx                          DMA_CHANNEL_4
#define RCC_DMAx_CLK_ENABLE()                  __HAL_RCC_DMA1_CLK_ENABLE()
#define DMAx_STREAMx_IRQn                      DMA1_Stream6_IRQn
#define DMAx_STREAMx_IRQHANDLER                DMA1_Stream6_IRQHandler



enum STMCOM_COM_SEQ 
{
	STMCOM_COMM0 = 0,
	STMCOM_COMM1 = 1,
	STMCOM_COMM2 = 2,
	STMCOM_COMM3 = 3,	
	STMCOM_COMM4 = 4,
	STMCOM_MAX  = 5,
};

//enum STMCOM_WORDLEN_CFG
//{
//	STMCOM_WORDLEN_8B = USART_WordLength_8b,	
//	STMCOM_WORDLEN_9B = USART_WordLength_9b,	
//};

//enum STMCOM_STOPBITS_CFG
//{
//	STMCOM_STOPBITS_1 = USART_StopBits_1,	
//	STMCOM_STOPBITS_0_5 = USART_StopBits_0_5,
//	STMCOM_STOPBITS_2 = USART_StopBits_2,
//	STMCOM_STOPBITS_1_5 = USART_StopBits_1_5,
//};

//enum STMCOM_PARITY_CFG
//{
//	STMCOM_PARITY_NO = USART_Parity_No,	
//	STMCOM_PARITY_EVEN = USART_Parity_Even,	
//	STMCOM_PARITY_ODD = USART_Parity_Odd,	
//};

typedef struct{  
	uint32_t baudRate;	
	uint16_t wordLength;
	uint16_t stopBits;	
	uint16_t parity;
}STMCOM_CFG_INFO;


typedef void (*Com_RecCallback)(uint8_t *_pbuf, uint32_t _len);		

extern void StmCom_Init(void);

extern void StmCom_ScanTask(void);

extern void StmCom_SetRecDataCallBack(uint8_t _comSeq, Com_RecCallback _recCallBack);

extern void StmCom_SendDataToComm(uint8_t _comSeq, uint8_t *_pBuf, uint32_t _len);

extern void StmCom_RxIRQHandler(uint8_t _comSeq);


extern USART_TypeDef* StmCom_GetTypeDef(uint8_t _comSeq);

extern void StmCom_ReceiveComData(uint8_t _comSeq, uint8_t _recData);
/* 扩展变量 ------------------------------------------------------------------*/
extern UART_HandleTypeDef husartx;

extern DMA_HandleTypeDef hdma_usartx_tx;
/* 函数声明 ------------------------------------------------------------------*/
void  USARTx_Init(void);


#endif  /* __BSP_USARTX_ H__ */

/******************* (C) COPYRIGHT 2015-2020 硬石嵌入式开发团队 *****END OF FILE****/
