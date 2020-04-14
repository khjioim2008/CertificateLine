#ifndef __max6675_H
#define	__max6675_H

#include <stdint.h>
#include "stm32f4xx_hal.h"



#define W1 0  //°å¿¨Ë¿Ó¡²ã ÊÇW1

#define W2 1  //°å¿¨Ë¿Ó¡²ã ÊÇW2



#define SELCET_HC4053IO_OUT_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOF_CLK_ENABLE()
#define SELCET_HC4053IO_OUT_PIN                     GPIO_PIN_11
#define SELCET_HC4053IO_PORT												GPIOF

#define SO_MAX6675IO_IN_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOF_CLK_ENABLE()
#define SO_MAX6675IO_IN_PIN                     GPIO_PIN_14
#define SO_MAX6675IO_PORT												GPIOF

#define CS_MAX6675IO_OUT_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOF_CLK_ENABLE()
#define CS_MAX6675IO_OUT_PIN                     GPIO_PIN_13
#define CS_MAX6675IO_PORT												GPIOF

#define SCK_MAX6675IO_OUT_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOF_CLK_ENABLE()
#define SCK_MAX6675IO_OUT_PIN                     GPIO_PIN_12
#define SCK_MAX6675IO_PORT												GPIOF


void initReadTemperature(void);

int16_t readTemperature(uint8_t channel);

#endif
