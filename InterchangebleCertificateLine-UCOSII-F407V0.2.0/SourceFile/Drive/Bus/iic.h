#ifndef __iic_H
#define __iic_H

#include "BasicTypes.h"


#define u8 N8
extern void iicStart(N8 Port);

extern void iicEnd(N8 Port);

extern N8 iicCmd(N8 Size,char *Buffer,N8 Port);

extern N8 iicData(N8 RW,N16 addr,N16 Size,char *Buffer,N8 Port);

extern void iicReset(N8 Port);

extern void initializeIIC(void);


extern void writeByte(N8 Value,N8 Port); 
extern  N8 readByte(N8 Port);
extern N8 ack(N8 rw,N8 Type,N8 Port); 

extern void sclLine(N8 Level,N8 Port);
extern void sdaWrite(N8 Level,N8 Port);
extern void setSDA(N8 wr,N8 Port);
   
extern N8 IIC(N8 Name,N8 RW,N16 Addr,N16 Size,char *Buffer); 
 
 
 
extern void write_byte(u8 daddr,u8 addr,u8 dat);

extern u8 read_byet(u8 daddr,u8 addr); 
 
#endif
