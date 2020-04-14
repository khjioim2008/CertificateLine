#include "cpu.h"
#include "Configure.h"
#include "ram.h"



/*  
    ÆÁ ×Ö¿â  RFID  WIFI ERROR RGB 
    
    
*/

void InitializeSPI(void)
{
//        spiMode(3,0,0,0,7);
//        spiSwitch(TRUE);
        SPI1_Init();
}
N8 stm32spi(N8 Name,N8 RW,N8 Wait,N16 Size,char *BufferAddr)
{
#define errorTime	1000
//        N32 RGKJR;
	N8 temp = 0;
	N16 i=0;
	static N16 spiError=0;
	if((RW != READ)&&(RW != WRITE)) return 1;
	if(Size == 0) return 2;
	if((N32 )BufferAddr < 0X20000000) return 3; 
	//SCSnLine(TRUE);

	if((Wait == 0)||(Wait == 1)) spiCS(Name);

	for(i=0;i<Size;i++)
	{	
		spiError=0;
		while(spiWritable())
		{
			if(spiError<errorTime) spiError++;
			else return 6;
		}  
		if(RW==READ) {//¶Á
			spiDR = 0;
			spiError=0;
			while(spiReadable())
			{
				if(spiError<errorTime) spiError++;
				else return 7;
			}
			*(BufferAddr+i) = spiDR;
		} else if(RW==WRITE) {//Ð´
			spiDR = *(BufferAddr+i);
			//temp = spiDR;
			//                        RGKJR=250;
			//                        while(RGKJR --)
			spiError=0;
			while(spiReadable())
			{
				if(spiError<errorTime) spiError++;
				else return 8;
			}
			temp = spiDR;
		}
	}
	//SCSnLine(FALSE); 
	if((Wait == 0)||(Wait == 2)) spiCS(0);
	return 0;
}
//N8 readWriteW5500(N8 Name,N8 Mode,N8 RW,N16 ADDR,N16 Size,N8 *BufferAddr)
//{
//#define errorTime	1000
//    	N8 Cmd[3],temp;
//	N16 i=0;
//    	static N16 spiError=0;
//    	temp=Name%4;
//	//if((temp<2)&&(Size>6)) return 9;  
//	if((temp==0)&&(Name>0)) return 1;
//	if(Name>31) return 1;
//	if(Mode>3) return 2;
//	if(RW>1) return 3;
//	
//	w5500Cmd(Cmd)->Addr=ADDR;
//	w5500Cmd(Cmd)->Value.BSB=Name;
//	w5500Cmd(Cmd)->Value.OMB=Mode;
//	w5500Cmd(Cmd)->Value.RWB=RW;
//	
////#define CMDRAM		0
////#define SPIR		6
////#define SPIT		700+SPIR
////#define BUFSIZE		1023	
//	SCSnLine(TRUE);
//	for(i=0;i<3;i++)
//	{
//		spiError=0;
//                while(spiWritable())
//                {
//			if(spiError<errorTime) spiError++;
//			else return 4;
//		}
//		spiDR = Cmd[i];
//		spiError=0;
//                while(spiReadable())
//                {
//			if(spiError<errorTime) spiError++;
//			else return 5;
//		}
//                Cmd[i]=spiDR;
//	}
//	for(i=0;i<Size;i++)
//	{	
//		spiError=0;
//		while(spiWritable())
//                {
//			if(spiError<errorTime) spiError++;
//			else return 6;
//		}  
//                if(RW==0) {//¶Á
//		    	spiDR = 0;
//    			spiError=0;
//                        while(spiReadable())
//			{
//				if(spiError<errorTime) spiError++;
//				else return 7;
//			}
//                        //*(BufferAddr+i) = spiDR;
//			if(temp<2) *((BufferAddr+CMDRAM)+i) = spiDR;
//			else if(Size > (SPIT - SPIR)) *(BufferAddr + SPIR + i) = spiDR;
//			else *((BufferAddr + SPIR) + ((i + Dhcp.spiR) % (SPIT - SPIR))) = spiDR;
//                } else if(RW==1) {//Ð´
//		    	//spiDR = *(BufferAddr+i);
//			if(temp<2) spiDR = *((BufferAddr+CMDRAM)+i);
//			else if(Size > (BUFSIZE - SPIT)) spiDR = *(BufferAddr + SPIT + i);
//                        else spiDR = *((BufferAddr + SPIT) + ((i + Dhcp.spiT) % (BUFSIZE - SPIT)));		    
//                        spiError=0;
//                        while(spiReadable())
//			{
//				if(spiError<errorTime) spiError++;
//				else return 8;
//			}
//                        Cmd[0] = spiDR;
//                }
//	}
//	SCSnLine(FALSE);
//	return 0;
//}



