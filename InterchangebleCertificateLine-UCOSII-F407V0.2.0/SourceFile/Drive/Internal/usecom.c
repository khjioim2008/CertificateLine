#include "usecom.h"

#include "usartx/bsp_usartx.h"



CertificateCom_RecCallback g_recCallBack; 

/**
  * @brief 	
  * @param  
  * @retval None
  */
static void CertificateCom_ReceiveMessage(uint8_t *_pMsgInfo, uint32_t _len)	
{
	g_recCallBack(_pMsgInfo, _len);
}

/**
  * @brief  	
  * @param
  * @retval
  */
void CertificateCom_Init(void)
{
//	STMCOM_CFG_INFO m_comCfg;
//	m_comCfg.baudRate = 115200;
//	m_comCfg.wordLength = STMCOM_WORDLEN_8B;
//	m_comCfg.stopBits = STMCOM_STOPBITS_1;
//	m_comCfg.parity = STMCOM_PARITY_NO;
//	StmCom_Config(STMCOM_COMM0, &m_comCfg);    
	StmCom_SetRecDataCallBack(STMCOM_COMM0, CertificateCom_ReceiveMessage);
	StmCom_Init();
}

/**   
  * @brief  
  * @param  
  * @retval None
  */
	void CertificateCom_SendMessage(uint8_t *_pMsgInfo, uint16_t _len)	
{
	//StmCom_SendDataToComm(STMCOM_COMM0, _pMsgInfo, _len);
}

/**
  * @brief 
  * @param  
  * @retval None
  */
void CertificateCom_RecMsgCallback(CertificateCom_RecCallback _recCallBack)
{
	g_recCallBack = _recCallBack;
}
/**
  * @brief 
  * @param  
  * @retval None
  */
void CertificateCom_Scan(void)
{
	  StmCom_ScanTask();
}
