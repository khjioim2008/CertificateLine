
/*-----------------------------<< STM32�ں�SystemTick���� >>--------------------------------*/
/*
 * - :CRJ
 * - :20150330.002.00.1
 * - delay(void)

 **/
/*___________________________________END______________________________________*/
#include "Configure.h"
#include "Resources.h"
#include "cpucrj.h"
#include "includes.h"//OS


#define delay_osrunning		OSRunning			//OS�Ƿ����б��,0,������;1,������
#define delay_ostickspersec	OSCfg_TickRate_Hz	//OSʱ�ӽ���,��ÿ����ȴ���
#define delay_osintnesting 	OSIntNestingCtr		//�ж�Ƕ�׼���,���ж�Ƕ�״���


void InitializeSysTickOS(void)
{
	N32 reload;
	//SystemClock72M();
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);	//ѡ���ⲿʱ��  HCLK/8
	reload = SystemCoreClock / 8000000;				//ÿ���ӵļ������� ��λΪK	   
	reload *= 1000000 / delay_ostickspersec;		//����delay_ostickspersec�趨���ʱ��
												//reloadΪ24λ�Ĵ���,���ֵ:16777216,��72M��,Լ��1.86s����	
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;   	//����SYSTICK�ж�
	SysTick->LOAD = reload; 						//ÿ1/delay_ostickspersec���ж�һ��	
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;   	//����SYSTICK    
}			

//ucosSysTick_Handler�жϷ�����
void SysTick_Handler(void)
{
	if(delay_osrunning==1)						//OS��ʼ����,��ִ�������ĵ��ȴ���
	{
		OSIntEnter();							//�����ж�
		OSTimeTick();       					//����ucos��ʱ�ӷ������               
		OSIntExit();       	 					//���������л����ж�
	}
}
