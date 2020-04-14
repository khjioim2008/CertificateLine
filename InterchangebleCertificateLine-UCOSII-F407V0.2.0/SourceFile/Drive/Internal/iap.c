#include "iap.h"
#include "io.h"
#include "Resources.h"
#define FLASH_APP1_ADDR		0x08002000  	//第一个应用程序起始地址(存放在FLASH)
											//保留的空间为IAP使用



//设置app固化配置
void iap_set_flag_s(void)
{
	Test_Write(APP_CONFIG_ADDR,APP_CONFIG_SET_VALUE);
	//printf("ok\r\n");
}

//清除app固化配置
void iap_clear_flag(void)
{
	Test_Write(APP_CONFIG_ADDR,APP_CONFIG_CLEAR_VALUE);
	//printf("ok\r\n");
}
















