
/*-----------------------------<< STM32内核SystemTick函数 >>--------------------------------*/
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


#define delay_osrunning		OSRunning			//OS是否运行标记,0,不运行;1,在运行
#define delay_ostickspersec	OSCfg_TickRate_Hz	//OS时钟节拍,即每秒调度次数
#define delay_osintnesting 	OSIntNestingCtr		//中断嵌套级别,即中断嵌套次数


void InitializeSysTickOS(void)
{
	N32 reload;
	//SystemClock72M();
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);	//选择外部时钟  HCLK/8
	reload = SystemCoreClock / 8000000;				//每秒钟的计数次数 单位为K	   
	reload *= 1000000 / delay_ostickspersec;		//根据delay_ostickspersec设定溢出时间
												//reload为24位寄存器,最大值:16777216,在72M下,约合1.86s左右	
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;   	//开启SYSTICK中断
	SysTick->LOAD = reload; 						//每1/delay_ostickspersec秒中断一次	
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;   	//开启SYSTICK    
}			

//ucosSysTick_Handler中断服务函数
void SysTick_Handler(void)
{
	if(delay_osrunning==1)						//OS开始跑了,才执行正常的调度处理
	{
		OSIntEnter();							//进入中断
		OSTimeTick();       					//调用ucos的时钟服务程序               
		OSIntExit();       	 					//触发任务切换软中断
	}
}
