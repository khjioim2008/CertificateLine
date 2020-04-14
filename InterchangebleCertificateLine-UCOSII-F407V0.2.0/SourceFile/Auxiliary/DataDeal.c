/*-------------------------------<< >>-----------------------------------*/
/*
 * - :CRJ--
 * - :20170000.000.00.1  
 * - 
 * - IC 
 * - Level:0 ,1
 * - :STM32F103-CutMachine-UCOSIII
 **/
/*___________________________________END______________________________________*/

#include "Resources.h"
#include "includes.h"
#include "Configure.h"
#include "flash.h"

u16 getMoreTimesNum(u16*dataBuf,u16 len,u16 num)
{
int i,j,k,m;
u16 CurrentAppearingMoreTimesData=0;
	for(i=0; i<len; i++){//ð������С����
		for(j=0; j<len-1-i; j++){
			if(dataBuf[j] > dataBuf[j+1]){
				      u8 tmp = dataBuf[j+1];
				dataBuf[j+1] = dataBuf[j];
				dataBuf[j]   = tmp;
			}
		}
	}
	/*********�ҳ����������������**************/
	//CurrentAppearingMoreTimesData��¼��ǰ����������������
	//m��¼CurrentAppearingMoreTimesData���ֵĴ�����k��¼��ǰ�����ֵĴ���
	
	CurrentAppearingMoreTimesData = dataBuf[0];
	k = 1;
	m = 1;
	for(i=1; i<len; i++){
		if(dataBuf[i] != dataBuf[i-1]){//ǰ�����������
			if(k > m){
				m = k;//m��󣬱�֤m�ǳ��ִ����������Ĵ���
				CurrentAppearingMoreTimesData = dataBuf[i-1];
			}
			k = 1;//�µ�����k���±�Ϊ1����ѭ��
		}else{//ǰ����������ȣ�k��������
		k++;
		}
		if(k>m){//ѭ�������и��£���Ȼ��������һ�������࣬�޷�������һ��
		m = k;
		CurrentAppearingMoreTimesData = dataBuf[i-1];
		}
	}
	return CurrentAppearingMoreTimesData;
}

u16 analogAdaptAverage(u16 AnalogBuf[AnalogAverageLen])
{
	u8 i;
	u16 AnalogBufSum=0,AnlogBufAverage=0;
	for(i=0;i<3;i++){
	if((AnalogBuf[i] == 0x0000) || (AnalogBuf[i] == 0xffff))
		return 0;//û�г�ʼ�����˳�
	else{//�г�ʼ����
		AnalogBufSum += AnalogBuf[i];
	}
	}
	
	AnlogBufAverage = AnalogBufSum / AnalogAverageLen;
	return AnlogBufAverage;
}

void writeAnalogDataInflash(u16 AnalogBufper)
{
	if(AnalogPerBuf[AnalogBufper] < 65535){//������ݵ���Ч�ԣ�����Flash������
		AnalogPerBuf[AnalogBufper] ++;
		STMFLASH_Write(FlASH_SAVE_ANALOG_PER_ADDR,AnalogPerBuf,100);
	}else{
		u8 k;
		for(k=0;k<100;k++){
			AnalogPerBuf[k] = 0;
		}
		STMFLASH_Write(FlASH_SAVE_ANALOG_PER_ADDR,AnalogPerBuf,100);
	}
}

void writeAnalogDataClear(void)
{
			N8 k;
		for(k=0;k<100;k++){
			AnalogPerBuf[k] = 0;
		}
		STMFLASH_Write(FlASH_SAVE_ANALOG_PER_ADDR,AnalogPerBuf,100);
}

u8 writeAnalogAverageBufInflash(u16 AnalogBuf[AnalogAverageLen])
{
	u8 i=0;
	 for(i=0; i<AnalogAverageLen - 1; i++)
	{
		if(AnalogBuf[i]<=0 || AnalogBuf[i]>100){ //���������Ч��
			return 0;
		}
	}
	
	STMFLASH_Write(FLASH_SAVE_ANALOG_AVERAGE_ADDR,AnalogBuf,AnalogAverageLen);
	STMFLASH_Write(FLASH_SAVE_ANALOG_AVERAGE_ADDR_ONE,AnalogBuf,AnalogAverageLen);
	STMFLASH_Write(FLASH_SAVE_ANALOG_AVERAGE_ADDR_TOW,AnalogBuf,AnalogAverageLen);

	return 1;
}

void clearAnalogAverageBufInflash(void)
{
	u16 AnalogBuf[AnalogAverageLen];
	u8 i=0;
	 for(i=0; i<AnalogAverageLen - 1; i++)
	{
		AnalogBuf[i] =0x0000;

	}
	STMFLASH_Write(FLASH_SAVE_ANALOG_AVERAGE_ADDR,AnalogBuf,AnalogAverageLen);
	STMFLASH_Write(FLASH_SAVE_ANALOG_AVERAGE_ADDR_ONE,AnalogBuf,AnalogAverageLen);
	STMFLASH_Write(FLASH_SAVE_ANALOG_AVERAGE_ADDR_TOW,AnalogBuf,AnalogAverageLen);
}

void flashDefaultSet(Flash_Save_Struct * flashDefault)
{
	flashDefault->flashSep.MembraneMotorDuty  = 100;
	flashDefault->flashSep.MembraneRollerDuty = 100;
	flashDefault->flashSep.ReveresRollerDuty  = 80;
	flashDefault->flashSep.PushMotorDuty      = 100;
	flashDefault->flashSep.BrushRollerDuty    = 100;
	flashDefault->flashSep.PaperRollerDuty    = 100;
	flashDefault->flashSep.FlsamMotorDuty     = 100;
	flashDefault->flashSep.EndRollerDuty      = 100;
	flashDefault->flashSep.PosStepStick       = 5800;
	flashDefault->flashSep.posStepStickUp     = 4000;
	//STMFLASH_Write(FLASH_SAVE_ADDR,(u16 *)&flashDefault,sizeof(Flash_Save_Struct)/2);
//	STMFLASH_Write(FLASH_SAVE_ONE_ADDR,(u16 *)&flashDefault,sizeof(Flash_Save_Struct)/2);
//	STMFLASH_Write(FLASH_SAVE_TWO_ADDR,(u16 *)&flashDefault,sizeof(Flash_Save_Struct)/2);
//	STMFLASH_Write(FLASH_SAVE_THREE_ADDR,(u16 *)&flashDefault,sizeof(Flash_Save_Struct)/2);
//	flashDefault->flashSep.posBoxWaitIn = 6000;
//	flashDefault->flashSep.posBoxCheck = 16000;
//	flashDefault->flashSep.posBoxInsert = 25000;
}
