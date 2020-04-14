#include "cpu.h"
#include "Configure.h"

//#define IIC_READ         1
//#define IIC_WRITE        0
#define IIC_NACK         1
#define IIC_ACK          0

typedef struct
{
        N8 id   :4;
        N8 addr :3;
        N8 rw   :1;
}IIC_CMD;
#define IICCMD(ADDR)  ((IIC_CMD *) ADDR)








N8 ack(N8 rw,N8 Type,N8 Port)//static 
{
        N8 iicError = 0;

        
        //sdaWrite(0,Port);
        if(rw == READ) {
                //sdaLine(1);
                
                setSDA(READ,Port);//setIO(IOB,I_D,(1<<7));
                while(sdaRead(Port) != IIC_ACK) 
                {
                        if(iicError<255) iicError ++;
                        else break;
                }
                setSDA(WRITE,Port);//setIO(IOB,O_G_P_50M,(1<<7));
                if(iicError > 0) {
                
                        return iicError;
                }
                
                
        } else if(rw == WRITE) {
                sdaWrite(Type,Port); 
                
        }
        sclLine(1,Port);
        //testDelay(10000);
        sclLine(0,Port);//testDelay(10000);
        sdaWrite(0,Port);//testDelay(10000);
        return 0;        
}
void writeByte(N8 Value,N8 Port)//static 
{
        N8 temp = Value;
        N8 i = 0;
        sclLine(0,Port);//testDelay(10000);
        for(i = 0;i < 8; i ++)
        {
                //sclLine(0,Port);
                if((temp & (1 << (7 - i))) == 0) sdaWrite(0,Port);
                else sdaWrite(1,Port); 
                sclLine(1,Port);//testDelay(10000); 
                sclLine(0,Port);//testDelay(10000);
                                
        }
}
/*       1 2 3 4 5 6 7 8  9  1 2 3 4 5 6 7 8  9  1 2 3 4 5 6 7 8  9
SCL-HH-L-HLHLHLHLHLHLHLHL-H-L-HLHLHLHLHLHLHLHL-H-L-HLHLHLHLHLHLHLHL-HH
SDA-HL- X X X X X X X X R T  X X X X X X X X R T  X X X X X X X X R LH
       1 2 3 4 5 6 7 8  9  1 2 3 4 5 6 7 8  9  1 2 3 4 5 6 7 8 9
SCL-HH-LHLHLHLHLHLHLHLH-LH-LHLHLHLHLHLHLHLH-LH-LHLHLHLHLHLHLHLHL-HH
SDA-HL-X X X X X X X X -RT-X X X X X X X X -RT-X X X X X X X X R LH
*/  
N8 readByte(N8 Port)//static 
{
        N8 temp = 0;
        N8 i = 0; 
        //sdaLine(1);
        sclLine(0,Port);//testDelay(10000);
        setSDA(READ,Port);//setIO(IOB,I_D,(1<<7));
        for(;i < 8; i ++)
        {
                
                sclLine(1,Port);//testDelay(10000);
                if(sdaRead(Port) != 0) temp |= (1 << (7 - i));
                
                sclLine(0,Port);//testDelay(10000);
        } 
        setSDA(WRITE,Port);//setIO(IOB,O_G_P_50M,(1<<7));
        return temp;
}

N8 iicCmd(N8 Size,char *Buffer,N8 Port)
{       
        N8 i = 0;

        if(Size > 3) return 1;        

        for(i = 0;i<Size;i++)
        {
                writeByte(*(Buffer + i),Port);
                if(ack(READ,0,Port) != 0) return 2;
        }        
        return 0;
}
N8 iicData(N8 RW,N16 addr,N16 Size,char *Buffer,N8 Port)
{       

        N16 i=0;
        if(Size == 0) return 2;
        if(RW == READ) {
                for(i=0;i<Size;i++)
                {
                        *(Buffer + addr + i) = readByte(Port);
                        if(i < (Size - 1)) ack(WRITE,0,Port);
                        else ack(WRITE,1,Port);
                }
                
        } else if(RW == WRITE) {
                for(i = 0;i < Size;i ++)
                {
                        writeByte(*(Buffer + addr + i),Port);
                        if(ack(READ,0,Port) != 0) return 1;
                } 
        }
        
        return 0;
}
void iicStart(N8 Port)
{
//        if((RPB & (1<<6))==0) sclLine(1);//SCL
//        if((RPB & (1<<7))==0) sdaLine(1);//SDA
        sdaWrite(1,Port);//testDelay(10000);
        sclLine(1,Port);//testDelay(10000);
        
        sdaWrite(0,Port);//testDelay(10000);
        sclLine(0,Port);//testDelay(10000);
        
}
void iicEnd(N8 Port)
{
//        if(((RPB & (1<<6))==0)&&((RPB & (1<<7))==0)) {//SCL
//        
//        } else if(((RPB & (1<<6))==1)&&((RPB & (1<<7))==0)) {//SCL
//        
//        } else if(((RPB & (1<<6))==0)&&((RPB & (1<<7))==1)) {//SCL
//        
//        } else if(((RPB & (1<<6))==1)&&((RPB & (1<<7))==1)) {//SCL
//        
//        }
        sdaWrite(0,Port);//testDelay(10000);
        sclLine(1,Port); //testDelay(10000);
        sdaWrite(1,Port);//testDelay(10000);
}
void iicReset(N8 Port)
{        //SCL 高 锁定, 低 可变
        N8 i = 0;
       // initializeIICIO();
        
        sdaWrite(1,Port);//testDelay(10000);
        sclLine(1,Port);//testDelay(10000);
        for(i = 0;i < 8;i ++)
        {
                sclLine(0,Port);
                sclLine(1,Port);        
        }  
           
}




typedef struct
{
        
        N16 rw   :1;
        N16 page :3;
        N16 id   :4;
        N16 addr :8;
}fm24c04_cmd;
#define fm24c04Cmd(ADDR) ((fm24c04_cmd *)ADDR)

N8 IIC(N8 Name,N8 RW,N16 Addr,N16 Size,char *Buffer)
{
        N8 errorV = 1,errorC = 0;
        N8 i = 0,temp_page_Start = 0,temp_page_End = 0;
        N16 temp_size = 0,temp_addr = 0;
        char temp[2];

        if(RW > READ) return 1;
        if(Addr > FM24C04_PAGE * FM24C04_SIZE -1) return 2;
        if(Size > FM24C04_PAGE * FM24C04_SIZE - Addr) return 3;
        if(Size == 0) return 4;
        if(Addr >= FM24C04_PAGE * FM24C04_SIZE) return 5;

        temp_page_Start = Addr / FM24C04_SIZE;
        if(((Size + Addr) % FM24C04_SIZE) == 0) temp_page_End = (Size + Addr) / FM24C04_SIZE;
        else temp_page_End = (Size + Addr) / FM24C04_SIZE + 1;

        while (errorV != 0)
        {
                temp_addr = 0;
                temp_size = 0;
                for(i = temp_page_Start;i < temp_page_End;i ++)
                {   
                        temp_addr += temp_size;
                        
                        fm24c04Cmd(temp)->addr = 0;
                        if(i == temp_page_Start) {
                                if((temp_page_End - temp_page_Start) > 1) {
                                        temp_size = FM24C04_SIZE - (Addr % FM24C04_SIZE);
                                } else {
                                        temp_size = Size;                                    
                                }
                                fm24c04Cmd(temp)->addr = Addr % FM24C04_SIZE; 
                        } else if(i == (temp_page_End - 1)) {
                                temp_size = (Size + Addr) - FM24C04_SIZE * i;
                        } else temp_size = FM24C04_SIZE;                        

                        fm24c04Cmd(temp)->id    = Name;
                        fm24c04Cmd(temp)->page  = i;

                        if(RW == READ) {
                                iicStart(FM24C04_PORT);
                                fm24c04Cmd(temp)->rw    = WRITE;                        
                                errorV = iicCmd(2,temp,FM24C04_PORT);
                                iicEnd(FM24C04_PORT);
                                if(errorV != 0) return 7;                  
                        } 
                        fm24c04Cmd(temp)->rw    = RW;
                        iicStart(FM24C04_PORT);
                        if(RW == READ) {
                                errorV = iicCmd(1,temp,FM24C04_PORT);
                        } else {
                                errorV = iicCmd(2,temp,FM24C04_PORT);
                        }
                        if(errorV != 0) break;
                        errorV = iicData(RW,temp_addr,temp_size,Buffer,FM24C04_PORT);
                        if(errorV != 0) {break;}
                        iicEnd(FM24C04_PORT);
                }
                if(errorV != 0) iicEnd(FM24C04_PORT);
                if(errorV != 0) {
                        iicReset(FM24C04_PORT);
                        if(errorC < 3) errorC ++;
                        else return 6;                
                }                
        }
        return 0;
}

void initializeIIC(void)
{
        iicReset(0);
        iicReset(1);        
}
/*
          1 2 3 4 5 6 7 8 9
SCL-  HL HLHLHLHLHLHLHLHLHL HLHLHLHLHLHLHLHLHL
SDA-   H                L H
*/
//N8 iic(N8 cmdSize,N16 addr,N16 Size,N8 *Buffer)
//{       
//        N8 i = 0;
//        N16 ii=0;
//        if(cmdSize > 3) return 1;        
//        start();
//        for(i = 0;i<cmdSize;i++)
//        {
//                writeByte(*(Buffer + i));
//                if(ack(READ,0) != 0) return 2;
//        }        
//        if(IICCMD(Buffer+0)->rw == READ) {
//                for(ii=0;ii<Size;ii++)
//                {
//                        *(Buffer + addr + ii) = readByte();
//                        ack(WRITE,0);
//                }
//        } else if(IICCMD(Buffer+0)->rw == WRITE) {
//                for(ii = cmdSize;ii < Size + cmdSize;ii ++)
//                {
//                        writeByte(*(Buffer + addr + ii));
//                        if(ack(READ,0) != 0) return 3;
//                }        
//        }
//        end();
//        return 0;
//}
//void iic(N8 Device,N8 RW,N16 addr,N16 Size)
/*
        SCL=1
        SDA=1
        
        SCL=1
        SDA=0

        SCL=0
        SDA=X
        SCL=1

        SCL=1
        SDA=1 

        SCL=0 等待 
        主机SDA=1 响应
        
*/

 //typedef union {
//        IIC_CMD cmdA;
//        N8      cmdB;
//}cmd;
