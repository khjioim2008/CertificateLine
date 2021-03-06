#include "Type.h"

/******************************************************
*函数名称:CRC16CCITT
*输   入:pszBuf  要校验的数据
        unLength 校验数据的长
*输   出:校验值
*功   能:循环冗余校验-16
         （CCITT标准-0x1021） //sizeof()
*******************************************************/

N16 CRC16(char * pszBuf, N16 unLength)
{
	N32 i, j;
	N16 CrcReg = 0xFFFF;
	N16 CurVal;
	for (i = 0; i < unLength; i++) 
	{
		CurVal = pszBuf[i] << 8;

		for (j = 0; j < 8; j++) 
		{ 
			if ((short)(CrcReg ^ CurVal) < 0)
				CrcReg = (CrcReg << 1) ^ 0x1021;
			else 
				CrcReg <<= 1; 
			CurVal <<= 1;            
		}
	}
	return CrcReg;
}

N8 BCC8(char * pszBuf, N16 unLength)
{
        N16 Value = 0,i = 0;
        
        for (i = 0; i < unLength; i++) 
        {
                Value += pszBuf[i];
        }
        
        return ~(N8 )Value;
}

//u8 hexDigits[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
//char ToHexString(u8 bytes[])
//{
//	char *chars;
//	u8 i;
//	int b;
//	for(i=0;i<14;i++)
//	{
//		b=bytes[i];
//		
//		chars[i*2]=hexDigits[b >> 4];
//		chars[i*2  + 1] = hexDigits[b & 0xF];
//	}
//	return *chars;
//}

//void Copy_Mem(unsigned char *P1, const unsigned char *P2,unsigned int Count)
//{  
//   unsigned int i ;
//   for(i=0;i<Count;i++)*P1++=*P2++; 
//}

//// * 函数名：Get_ChipID
//// * 芯片的唯一ID为: 33FFDA05-464E3431-18750543
//// * 芯片flash的容量为: 256K 
// 
//void Get_ChipID(void)
//{  
//	u32 temp0,temp1,temp2;
////	u32 ChipUniqueID[3];
//	u8 temp[14];//MCU型号
//	u16 CurVal;
//	temp0=*(__IO u32*)(0x1FFFF7E8);    //产品唯一身份标识寄存器（96位）
//	temp1=*(__IO u32*)(0x1FFFF7EC);
//	temp2=*(__IO u32*)(0x1FFFF7F0);
//	temp[0] = (u8)(temp0 & 0x000000FF);
//	temp[1] = (u8)((temp0 & 0x0000FF00)>>8);
//	temp[2] = (u8)((temp0 & 0x00FF0000)>>16);
//	temp[3] = (u8)((temp0 & 0xFF000000)>>24);
//	temp[4] = (u8)(temp1 & 0x000000FF);
//	temp[5] = (u8)((temp1 & 0x0000FF00)>>8);
//	temp[6] = (u8)((temp1 & 0x00FF0000)>>16);
//	temp[7] = (u8)((temp1 & 0xFF000000)>>24);
//	temp[8] = (u8)(temp2 & 0x000000FF);
//	temp[9] = (u8)((temp2 & 0x0000FF00)>>8);
//	temp[10] = (u8)((temp2 & 0x00FF0000)>>16);
//	temp[11] = (u8)((temp2 & 0xFF000000)>>24);
////	ChipUniqueID[0] = *(__IO u32 *)(0X1FFFF7F0); // 高譕0纸�
////	ChipUniqueID[1] = *(__IO u32 *)(0X1FFFF7EC); // 
////	ChipUniqueID[2] = *(__IO u32 *)(0X1FFFF7E8); // 低字节
//
//	CurVal=CRC16CCITT(temp,12);
//	temp[12]=CurVal>>8;
//	temp[13]=CurVal&0XFF;
		
////	RS485_Send_Data(temp,14);
//}
