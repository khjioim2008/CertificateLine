#ifndef __spi_d_H
#define __spi_d_H

#include "BasicTypes.h"

extern void InitializeSPI(void);

extern N8 stm32spi(N8 Name,N8 RW,N8 Wait,N16 Size,char *BufferAddr);





#endif

