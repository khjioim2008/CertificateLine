#include "iap.h"
#include "io.h"
#include "Resources.h"
#define FLASH_APP1_ADDR		0x08002000  	//��һ��Ӧ�ó�����ʼ��ַ(�����FLASH)
											//�����Ŀռ�ΪIAPʹ��



//����app�̻�����
void iap_set_flag_s(void)
{
	Test_Write(APP_CONFIG_ADDR,APP_CONFIG_SET_VALUE);
	//printf("ok\r\n");
}

//���app�̻�����
void iap_clear_flag(void)
{
	Test_Write(APP_CONFIG_ADDR,APP_CONFIG_CLEAR_VALUE);
	//printf("ok\r\n");
}
















