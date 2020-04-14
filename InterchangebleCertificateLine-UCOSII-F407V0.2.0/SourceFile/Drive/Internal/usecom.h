#ifndef __USECOM_H
#define	__USECOM_H

#include <stdint.h>
#include "stm32f4xx_hal.h"

typedef void (*CertificateCom_RecCallback)(uint8_t *_pbuf, uint32_t _len);	

void CertificateCom_Init(void);
void CertificateCom_SendMessage(uint8_t *_pMsgInfo, uint16_t _len)	;
void CertificateCom_RecMsgCallback(CertificateCom_RecCallback _recCallBack);
void CertificateCom_Scan(void);

#endif
