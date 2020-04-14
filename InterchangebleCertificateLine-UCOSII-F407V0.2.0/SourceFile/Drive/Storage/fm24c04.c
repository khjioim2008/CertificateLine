#include "cpu.h"
#include "Configure.h"
#include "iic.h"

//#define FM24C04_PAGE    2
//#define FM24C04_SIZE    256


typedef struct
{
        
        N16 rw   :1;
        N16 page :3;
        N16 id   :4;
        N16 addr :8;
}fm24c04_cmd;
#define fm24c04Cmd(ADDR) ((fm24c04_cmd *)ADDR)

N8 fm24c04(N8 RW,N16 Addr,N16 Size,char *Buffer)
{
        N8 errorV = 1,errorC = 0;
        N8 i = 0,temp_page_Start = 0,temp_page_End = 0;
        N16 temp_size = 0,temp_addr = 0;//,temp_size_first = 0,temp_size_tail = 0;
        char temp[2];
//copyRAM
        if(RW > READ) return 1;
        if(Addr > FM24C04_PAGE * FM24C04_SIZE -1) return 2;
        if(Size > FM24C04_PAGE * FM24C04_SIZE - Addr) return 3;
        if(Size == 0) return 4;
        if(Addr >= FM24C04_PAGE * FM24C04_SIZE) return 5;
        
        
        
        
        temp_page_Start = Addr / FM24C04_SIZE;
        if(((Size + Addr) % FM24C04_SIZE) == 0) temp_page_End = (Size + Addr) / FM24C04_SIZE;
        else temp_page_End = (Size + Addr) / FM24C04_SIZE + 1;

//        temp_size_first = Addr % FM24C04_SIZE;
//        temp_size_tail  = (Size + Addr) % FM24C04_SIZE;
        
        while (errorV != 0)
        {
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
                                //temp_size = ((Addr + Size) % (FM24C04_SIZE + 1)) - (Addr % FM24C04_SIZE);
                                fm24c04Cmd(temp)->addr = Addr % FM24C04_SIZE; 
                        } else if(i == (temp_page_End - 1)) {
                                temp_size = (Size + Addr) - FM24C04_SIZE * i;
                        } else temp_size = FM24C04_SIZE;                        

                        fm24c04Cmd(temp)->id    = 0X0A;
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
                        if(errorV != 0) break;
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







