#ifndef __IAP_H_
#define __IAP_H_
#include "flash.h"



//0x08000000 flash起始地址
//0x2000 iap代码长度
//0x1ffc-0x1fff作为固化配置存在

#define APP_CONFIG_ADDR 		0X08001FFC	//配置地址
#define APP_CONFIG_SET_VALUE	0X5555	//设置值
#define APP_CONFIG_CLEAR_VALUE	0XFFFF	//清零值


//设置app固化配置
void iap_set_flag_s(void);

//清除app固化配置
void iap_clear_flag(void);


#endif


















