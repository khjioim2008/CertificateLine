
/* 包含头文件 ----------------------------------------------------------------*/
#include "usartx/bsp_usartx.h"
#include <stdio.h>

/*--------------------------STMCOM---------------------------------*/
typedef struct{
	uint8_t *outBufStartPtr;	
	uint8_t *inBufStartPtr;
	uint8_t *inBufRePtr;	
	uint8_t *inBufWrPtr;	
	uint32_t inDataNum;	
	uint16_t inBufLen;		
	uint8_t comSeq;		
	Com_RecCallback recCallBack; 
}STMCOM_QUEUE_INFO;

#define COM_NUM_MAX			1 

#define COM_INBUF0_LENGTH		128                
#define COM_OUTBUF0_LENGTH	128  

#define COM_INBUF1_LENGTH		0               
#define COM_OUTBUF1_LENGTH		0  

#define COM_INBUF2_LENGTH		0                
#define COM_OUTBUF2_LENGTH		0   

#define COM_INBUF3_LENGTH		0              
#define COM_OUTBUF3_LENGTH		0   

#define COM_INBUF4_LENGTH		0                
#define COM_OUTBUF4_LENGTH		0   

#if COM_INBUF0_LENGTH > 0
uint8_t g_comInBuf0[COM_INBUF0_LENGTH];
#endif
#if COM_OUTBUF0_LENGTH > 0
uint8_t g_comOutBuf0[COM_OUTBUF0_LENGTH];
#endif

#if COM_INBUF1_LENGTH > 0
uint8_t g_comInBuf1[COM_INBUF1_LENGTH];
#endif
#if COM_OUTBUF1_LENGTH > 0
uint8_t g_comOutBuf1[COM_OUTBUF1_LENGTH];
#endif

#if COM_INBUF2_LENGTH > 0
uint8_t g_comInBuf2[COM_INBUF2_LENGTH];
#endif
#if COM_OUTBUF2_LENGTH > 0
uint8_t g_comOutBuf2[COM_OUTBUF2_LENGTH];
#endif

#if COM_INBUF3_LENGTH > 0
uint8_t g_comInBuf3[COM_INBUF3_LENGTH];
#endif
#if COM_OUTBUF3_LENGTH > 0
uint8_t g_comOutBuf3[COM_OUTBUF3_LENGTH];
#endif

#if COM_INBUF4_LENGTH > 0
uint8_t g_comInBuf4[COM_INBUF4_LENGTH];
#endif
#if COM_OUTBUF4_LENGTH > 0
uint8_t g_comOutBuf4[COM_OUTBUF4_LENGTH];
#endif

STMCOM_QUEUE_INFO g_comQueue[COM_NUM_MAX];

UART_HandleTypeDef husartx;
DMA_HandleTypeDef hdma_usartx_tx;
/* 扩展变量 ------------------------------------------------------------------*/
/* 私有函数原形 --------------------------------------------------------------*/
/* 函数体 --------------------------------------------------------------------*/
/**
  * 函数功能: RS485通信功能引脚GPIO初始化
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */
void HAL_UART_MspInit(UART_HandleTypeDef *husart)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  if(husart->Instance == USARTx)
  {
    /* 串口外设时钟使能 */
    
    USARTx_Tx_GPIO_ClK_ENABLE();
    USARTx_Rx_GPIO_ClK_ENABLE();
    RCC_DMAx_CLK_ENABLE();
    
    /* 串口外设功能GPIO配置 */
    GPIO_InitStruct.Pin =  USARTx_Tx_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_SET_AF_USARTx; 
    HAL_GPIO_Init( USARTx_Tx_GPIO_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin =  USARTx_Rx_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_SET_AF_USARTx;
    HAL_GPIO_Init( USARTx_Rx_GPIO_PORT, &GPIO_InitStruct);
  }
}
/**
  * 函数功能: 串口硬件反初始化配置
  * 输入参数: huart：串口句柄类型指针
  * 返 回 值: 无
  * 说    明: 该函数被HAL库内部调用
  */
void HAL_UART_MspDeInit(UART_HandleTypeDef *husart)
{
  if(husart->Instance == USARTx)
  {
    /* 串口外设时钟禁用 */
    USART_RCC_CLK_DISABLE();
  
    /* 串口外设功能GPIO配置 */
    HAL_GPIO_DeInit(USARTx_Tx_GPIO_PORT, USARTx_Tx_GPIO_PIN);
    HAL_GPIO_DeInit(USARTx_Rx_GPIO_PORT, USARTx_Rx_GPIO_PIN);
        /* DMA外设反初始化 */
    HAL_DMA_DeInit(husart->hdmatx);
    /* 串口中断禁用 */
    HAL_NVIC_DisableIRQ(USARTx_IRQn);
  }
}
/**
  * 函数功能: NVIC配置
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  */
static void MX_NVIC_DMAx_Init(void)
{
    /* USART interrupt configuration */
  HAL_NVIC_SetPriority(USARTx_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(USARTx_IRQn);
  /* DMA interrupt configuration */
  HAL_NVIC_SetPriority(DMAx_STREAMx_IRQn, 2, 0);
  HAL_NVIC_EnableIRQ(DMAx_STREAMx_IRQn);  
}
/**
  * 函数功能: 串口参数配置.
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */
void  USARTx_Init(void)
{ 
  /* 使能串口功能引脚GPIO时钟 */
  USART_RCC_CLK_ENABLE();
  
  husartx.Instance = USARTx;
  husartx.Init.BaudRate = USARTx_BAUDRATE;
  husartx.Init.WordLength = UART_WORDLENGTH_8B;
  husartx.Init.StopBits = UART_STOPBITS_1;
  husartx.Init.Parity = UART_PARITY_NONE;
  husartx.Init.Mode = UART_MODE_TX_RX;
	husartx.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  husartx.Init.OverSampling = UART_OVERSAMPLING_16;
  HAL_UART_Init(&husartx);
  
  /* 初始化DMA数据流通道 */  
  hdma_usartx_tx.Instance = DMAx_STREAMx;
  hdma_usartx_tx.Init.Channel = DMAx_CHANNELx;
  hdma_usartx_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
  hdma_usartx_tx.Init.PeriphInc = DMA_PINC_DISABLE;
  hdma_usartx_tx.Init.MemInc = DMA_MINC_ENABLE;
  hdma_usartx_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
  hdma_usartx_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
  hdma_usartx_tx.Init.Mode = DMA_NORMAL;
  hdma_usartx_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
  hdma_usartx_tx.Init.Priority = DMA_PRIORITY_HIGH;
  HAL_DMA_Init(&hdma_usartx_tx);

  /* 链接串口与DMA通道 */
  __HAL_LINKDMA(&husartx,hdmatx,hdma_usartx_tx);
    
  MX_NVIC_DMAx_Init();
}

/**
  * 函数功能: 重定向c库函数printf到DEBUG_USARTx
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */
int fputc(int ch, FILE *f)
{

  HAL_UART_Transmit(&husartx, (uint8_t *)&ch, 1, 0xffff);
  return ch;
}

/**
  * 函数功能: 重定向c库函数getchar,scanf到DEBUG_USARTx
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */

int fgetc(FILE * f)
{
  uint8_t ch = 0;
  while(HAL_UART_Receive(&husartx,&ch, 1, 0xffff)!=HAL_OK);
  return ch;
}

/**
  * @brief ??COM???
  * @param  None
  * @retval None
  */
USART_TypeDef* StmCom_GetTypeDef(uint8_t _comSeq)
{
	USART_TypeDef *m_pTypeDef;
	
	switch(_comSeq)
	{
		case STMCOM_COMM0:
			m_pTypeDef = USART1;
			break;
		case STMCOM_COMM1:
			m_pTypeDef = USART2;
			break;
		case STMCOM_COMM2:
			m_pTypeDef = USART3;
			break;
		case STMCOM_COMM3:
			m_pTypeDef = UART4;
			break;
		case STMCOM_COMM4:
			m_pTypeDef = UART5;
			break;
		default:
			m_pTypeDef = 0;
			break;
	}
	return m_pTypeDef;
}


//static void StmCom_SendByte(USART_TypeDef *USARTx, uint8_t _Data)
//{
//	USART_SendData(USARTx, _Data); 
//	while(!(USARTx->SR & USART_FLAG_TXE));
//	while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) != SET);
//}

/**
 * @brief  ??COM?????????
 * @param  
 * @retval None
 */
void StmCom_ReceiveComData(uint8_t _comSeq, uint8_t _recData)
{
	uint8_t i;
	for(i = 0; i < COM_NUM_MAX; i++)
	{
		if(g_comQueue[i].comSeq == _comSeq)
		{
			*g_comQueue[i].inBufWrPtr = _recData;//??FIFO??
			++g_comQueue[i].inDataNum;
			if(++g_comQueue[i].inBufWrPtr >= (g_comQueue[i].inBufStartPtr + g_comQueue[i].inBufLen))
			g_comQueue[i].inBufWrPtr = g_comQueue[i].inBufStartPtr;
			break;
		}
	}
}

/**
  * @brief COM????
  * @param  None
  * @retval None
  */
static void StmCom_InScan(void)  
{
	uint8_t i;
	uint32_t m_recDataNum, m_sendNum;
	uint8_t *m_pRecBufPrt;
	STMCOM_QUEUE_INFO *m_pQueue;
 
	m_pQueue = &g_comQueue[0];	
	for(i = 0; i < COM_NUM_MAX; i++, m_pQueue++)
	{
		if(m_pQueue->inBufRePtr != m_pQueue->inBufWrPtr)
		{
			m_recDataNum = m_pQueue->inDataNum;
			m_pRecBufPrt = m_pQueue->inBufRePtr;
			m_pQueue->inDataNum = 0;
			m_pQueue->inBufRePtr = m_pQueue->inBufWrPtr;
			m_sendNum = m_pQueue->inBufLen - (m_pRecBufPrt - m_pQueue->inBufStartPtr);
			if(m_sendNum >= m_recDataNum)
			{
				m_sendNum = m_recDataNum;
				m_pQueue->recCallBack(m_pRecBufPrt, m_sendNum);//??????
			}
			else
			{
				m_pQueue->recCallBack(m_pRecBufPrt, m_sendNum);
				m_sendNum = m_recDataNum - m_sendNum;
				m_pQueue->recCallBack(m_pQueue->inBufStartPtr, m_sendNum);//??????
			}
		}
	}
}

/**
  * @brief COM?????
  * @param  None
  * @retval None
  */
static void StmCom_OutScan(void)  
{  

}

/**
  * @brief COM???
  * @param  None
  * @retval None
  */
void StmCom_Init(void)  
{
#if COM_INBUF0_LENGTH > 0
	g_comQueue[0].inBufStartPtr = g_comInBuf0;
	g_comQueue[0].inBufRePtr = g_comQueue[0].inBufStartPtr;
	g_comQueue[0].inBufWrPtr = g_comQueue[0].inBufStartPtr;
	g_comQueue[0].inDataNum = 0;
	g_comQueue[0].inBufLen = COM_INBUF0_LENGTH;
	g_comQueue[0].comSeq = STMCOM_COMM0;//COM???
	g_comQueue[0].comSeq = 0; 
#endif
#if COM_OUTBUF0_LENGTH > 0
	g_comQueue[0].outBufStartPtr = g_comOutBuf0;
#endif
#if COM_INBUF1_LENGTH > 0
	g_comQueue[1].inBufStartPtr = g_comInBuf1;
	g_comQueue[1].inBufRePtr = g_comQueue[1].inBufStartPtr;
	g_comQueue[1].inBufWrPtr = g_comQueue[1].inBufStartPtr;
	g_comQueue[1].inDataNum = 0;
	g_comQueue[1].inBufLen = COM_INBUF1_LENGTH;
	g_comQueue[1].comSeq = STMCOM_MAX;
#endif
#if COM_OUTBUF1_LENGTH > 0
	g_comQueue[1].outBufStartPtr = g_comOutBuf1;
#endif

#if COM_INBUF2_LENGTH > 0
	g_comQueue[2].inBufStartPtr = g_comInBuf2;
	g_comQueue[2].inBufRePtr = g_comQueue[2].inBufStartPtr;
	g_comQueue[2].inBufWrPtr = g_comQueue[2].inBufStartPtr;
	g_comQueue[2].inDataNum = 0;
	g_comQueue[2].inBufLen = COM_INBUF2_LENGTH;
	g_comQueue[2].comSeq = STMCOM_MAX;	
#endif
#if COM_OUTBUF2_LENGTH > 0
	g_comQueue[2].outBufStartPtr = g_comOutBuf2;
#endif

#if COM_INBUF3_LENGTH > 0
	g_comQueue[3].inBufStartPtr = g_comInBuf3;
	g_comQueue[3].inBufRePtr = g_comQueue[3].inBufStartPtr;
	g_comQueue[3].inBufWrPtr = g_comQueue[3].inBufStartPtr;
	g_comQueue[3].inDataNum = 0;
	g_comQueue[3].inBufLen = COM_INBUF3_LENGTH;
	g_comQueue[3].comSeq = STMCOM_MAX;
#endif
#if COM_OUTBUF3_LENGTH > 0
	g_comQueue[3].outBufStartPtr = g_comOutBuf3;
#endif

#if COM_INBUF4_LENGTH > 0
	g_comQueue[4].inBufStartPtr = g_comInBuf4;
	g_comQueue[4].inBufRePtr = g_comQueue[4].inBufStartPtr;
	g_comQueue[4].inBufWrPtr = g_comQueue[4].inBufStartPtr;
	g_comQueue[4].inDataNum = 0;
	g_comQueue[4].inBufLen = COM_INBUF4_LENGTH;
	g_comQueue[4].comSeq = STMCOM_MAX;
#endif
#if COM_OUTBUF4_LENGTH > 0
	g_comQueue[4].outBufStartPtr = g_comOutBuf4;
#endif
}
/**
  * @brief COM???
  * @param  None
  * @retval None
  */
void StmCom_ScanTask(void)  
{
	StmCom_InScan();
	//StmCom_OutScan();
}
/**
 * @brief  ??COM????????
 * @param  _recCallBack:??????
 * @retval None
 */
void StmCom_SetRecDataCallBack(uint8_t _comSeq, Com_RecCallback _recCallBack)
{
	uint8_t i;
	for(i = 0; i < COM_NUM_MAX; i++)
	{
		if(g_comQueue[i].comSeq == _comSeq)
		{
			g_comQueue[i].recCallBack = _recCallBack;
			break;
		}
	}
}
/**
 * @brief  ????COM?
 * @param  
 * @retval None
 */
 void StmCom_SendDataToComm(uint8_t _comSeq, uint8_t *_pBuf, uint32_t _len)
{
	uint32_t i;
	USART_TypeDef *m_pTypeDef;
	
	m_pTypeDef = StmCom_GetTypeDef(_comSeq);
	if(m_pTypeDef == 0)
		return;
	for(i = 0; i < _len; i++)
	{
//		StmCom_SendByte(m_pTypeDef, *_pBuf);
		_pBuf += 1;
	}
}

/**
  * @brief COM
  * @param  None
  * @retval None
  */
void StmCom_RxIRQHandler(uint8_t data)
{
//	USART_TypeDef *m_pTypeDef;
	
//	m_pTypeDef = StmCom_GetTypeDef(_comSeq);
//	if(m_pTypeDef == 0)
//		return;
////	if(USART_GetFlagStatus(m_pTypeDef, USART_FLAG_ORE) != RESET) //???USART_GetITStatus??????????
////	{
////		USART_ClearFlag(m_pTypeDef, USART_FLAG_ORE);
////		USART_ReceiveData(m_pTypeDef);
////	}
//	else if(USART_GetITStatus(m_pTypeDef, USART_IT_RXNE) != RESET)
//	{
//		USART_ClearITPendingBit(m_pTypeDef, USART_IT_RXNE);
// 		StmCom_ReceiveComData(_comSeq, (uint8_t)m_pTypeDef->DR);
//	}
}
