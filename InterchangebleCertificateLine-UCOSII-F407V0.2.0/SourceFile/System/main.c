/*-------------------------------<< 主函数>>-----------------------------------*/
/*
 * - :CRJ--
 * - :2020.0.0.0.1  
 * - void main(void)
 * - :STM32F407-CRJ-UCOSII版本
 **/
/*___________________________________END______________________________________*/

#include "Resources.h"
#include "Drives.h"
#include "Configure.h"

#include "key/bsp_key.h"
#include "usartx/bsp_usartx.h"
#include "EncoderTIM/bsp_EncoderTIM.h"
#include "DCMotor/bsp_BDCMotor.h"
#include "Sensor/bsp_Sensor.h"
#include "StepMotor/bsp_STEPMOTOR.h" 
#include "stepLinkedList.h"

//#include "runAllTask.h"
#include "runCutMachine.h"
#include "runStampMachine.h"
#include "runPagingMachine.h"

#include "includes.h"
#define SAMPLING                  0x01    // 采样标记
#define BUTTONLING                 0x02 
#define MESUINGLING                 0x04 

uint16_t time_count = 0;              // 时间计数，每1ms增加一(与滴答定时器频率有关)
uint8_t Time_Flag   = 0;              // 任务时间标记
 
uint8_t aTxBuffer;
uint8_t aRxBuffer;                         // 接收数据

__IO uint16_t Machine_Status   = 0;
extern int32_t CaptureNumber;     // 输入捕获数
extern int32_t PagingCaptureNumber;
extern int32_t SUM_Pulse;

//设置任务优先级
#define  APP_TASK_START_PRIO           20			//开始任务的优先级为最低
#define CUT_TASK_PRIO			             11
#define STAMP_TASK_PRIO			           12
#define PAGING_TASK_PRIO			         13
#define COM_TASK_PRIO                  19

//设置任务堆栈大小
#define  APP_TASK_START_STK_SIZE       256
#define CUT_STK_SIZE			             256
#define STAMP_STK_SIZE			           256
#define PAGING_STK_SIZE			           1024
#define COM_STK_SIZE			             1024

//任务堆栈
OS_STK appTaskStartStk[CUT_STK_SIZE];
OS_STK CUT_TASK_STK[CUT_STK_SIZE];
OS_STK STAMP_TASK_STK[STAMP_STK_SIZE];
OS_STK PAGING_TASK_STK[PAGING_STK_SIZE];
OS_STK COM_TASK_STK[COM_STK_SIZE];

//任务函数
void appTaskStart (void* p_arg);
void cutTask(void* p_arg);
void stampTask(void *p_arg);
void pagingTask(void *p_arg);
void comTask(void *p_arg);

//按键回调函数
void CUTBTN_PRESS_DOWN_Handler(void* btn);
void CUTBTN_LONG_PRESS_Handler(void* btn);
void CUTBTN_PRESS_UP_Handler(void* btn);
void PLASTICBTN_PRESS_DOWN_Handler(void* btn);
void PLASTICBTN_LONG_PRESS_Handler(void* btn);
void PLASTICBBTN_PRESS_UP_Handler(void* btn);
void WORKFINISH_PRESS_DOWN_Handler(void* btn);
void PAPEREXITBTN_PRESS_DOWN_Handler(void* btn);

OS_EVENT *CutFullSem; // 钢印机堆满信号量
/*SYSTICK 配置， 1ms一次systick中断，产生ucos 调度节拍*/
void OS_CPU_SysTickInit (void)
{
  INT32U  cpu_clk_freq;
  INT32U  cnts;
  cpu_clk_freq = HAL_RCC_GetHCLKFreq();  
	cnts  = cpu_clk_freq / (INT32U)OS_TICKS_PER_SEC;
	SysTick_Config (cnts);
}

static void SystemClock_Config()
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
 
  __HAL_RCC_PWR_CLK_ENABLE();                                     // 使能PWR时钟

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);  // 设置调压器输出电压级别1

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;      // 外部晶振，8MHz
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;                        // 打开HSE 
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;                    // 打开PLL
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;            // PLL时钟源选择HSE
  RCC_OscInitStruct.PLL.PLLM = 8;                                 // 8分频MHz
  RCC_OscInitStruct.PLL.PLLN = 336;                               // 336倍频
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;                     // 2分频，得到168MHz主时钟
  RCC_OscInitStruct.PLL.PLLQ = 7;                                 // USB/SDIO/随机数产生器等的主PLL分频系数
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;       // 系统时钟：168MHz
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;              // AHB时钟： 168MHz
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;               // APB1时钟：42MHz
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;               // APB2时钟：84MHz
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);

 // HAL_RCC_EnableCSS();                                            // 使能CSS功能，优先使用外部晶振，内部时钟源为备用
  
 	// HAL_RCC_GetHCLKFreq()/1000    1ms中断一次
	// HAL_RCC_GetHCLKFreq()/100000	 10us中断一次
	// HAL_RCC_GetHCLKFreq()/1000000 1us中断一次
//  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);                 // 配置并启动系统滴答定时器
  /* 系统滴答定时器时钟源 */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* 系统滴答定时器中断优先级配置 */
 HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
	
}

static void initializeHardWare(void)
{
  HAL_Init();
  /* 配置系统时钟 */
  SystemClock_Config();
  /* 板载按键初始化 */
   KEY_GPIO_Init();
	 button_init(&CutBtn,readCutButtonIO,0);
	 button_init(&PlasticBtn,readStampButtonIO,0);
	 button_init(&WorkFinishBtn,readWorkFinishButtonIO,0);
	 button_init(&PaperExitBtn,readPaperExitButtonIO,0);
	
	 button_attach(&CutBtn, PRESS_DOWN,CUTBTN_PRESS_DOWN_Handler);
	 button_attach(&CutBtn, LONG_RRESS_START,CUTBTN_LONG_PRESS_Handler);
	 button_attach(&CutBtn, PRESS_UP,CUTBTN_PRESS_UP_Handler);
	 
	 button_attach(&PlasticBtn, PRESS_DOWN,PLASTICBTN_PRESS_DOWN_Handler);
	 button_attach(&PlasticBtn, LONG_RRESS_START,PLASTICBTN_LONG_PRESS_Handler);
	 button_attach(&PlasticBtn, PRESS_UP,PLASTICBBTN_PRESS_UP_Handler);
   
	 button_attach(&WorkFinishBtn, PRESS_DOWN,WORKFINISH_PRESS_DOWN_Handler);
	 button_attach(&PaperExitBtn, PRESS_DOWN,PAPEREXITBTN_PRESS_DOWN_Handler);
	
	 button_start(&CutBtn);
	 button_start(&PlasticBtn);
	 button_start(&WorkFinishBtn);
	 button_start(&PaperExitBtn);
	
	initMotorPWM();
		  /* 初始化PID参数结构体 */
    Init_PIDStruct();
  /* 调试串口初始化 */
  USARTx_Init(); 
	
  CommandCallBackInit();//回调设置
  
	CertificateCom_Init();//串口FIFO队列设置
	
	initSensorIO();//传感器IO
	
	initReadTemperature();
	
	/* PWM输出功能 */
	initMotorPWM();
	
  initMotorIOPushOut();	
 
	/* 编码器定时器初始化并配置输入捕获功能 */
   ENCODER_TIMx_Init();
  /* 启动编码器接口 */
  HAL_TIM_Encoder_Start(&htimx_Encoder, TIM_CHANNEL_ALL);
  HAL_TIM_Encoder_Start(&htimx_EncoderTwo, TIM_CHANNEL_ALL);
	
  /* 步进电机定时器初始化*/
  STEPMOTOR_TIMx_Init();

	cutMachineStep_TIM9_Init(); //TIM9
	
	//initCutAllMotor(&CutMachine);
  /* 使能接收，进入中断回调函数 */
  HAL_UART_Receive_IT(&husartx,&aRxBuffer,1);
	
	Vel_Target = (vPID.SetPoint*P_PERIOD);
		
		
	listInit(&CutStepList); //裁切
 // listInit(&StampStepList);//钢印
	
}

int main(void)
{

  initializeHardWare();// 初始化硬件 

	OSInit();  //UCOS初始化

	CutFullSem = OSSemCreate(1);  //初始值
	
	OSTaskCreate (appTaskStart,	  		  		//指向任务代码的指针
								(void *) 0,								  		//任务开始执行时，传递给任务的参数的指针
								(OS_STK *) &appTaskStartStk[APP_TASK_START_STK_SIZE - 1],	//分配给任务的堆栈的栈顶指针   从顶向下递减
								(INT8U) APP_TASK_START_PRIO);

		OSStart();
}

void appTaskStart (void* p_arg)
{
	(void) p_arg;
		OS_CPU_SR cpu_sr=0;

	/*初始化ucos时钟节拍*/
	  OS_CPU_SysTickInit();

	/*使能ucos 的统计任务*/
#if (OS_TASK_STAT_EN > 0)
	/*统计任务初始化函*/
	OSStatInit();               
#endif

	OS_ENTER_CRITICAL();  //进入临界区(关闭中断)
	OSTaskCreate(cutTask,(void*)0,(OS_STK*)&CUT_TASK_STK[CUT_STK_SIZE-1],CUT_TASK_PRIO);//裁切
	//OSTaskCreate(stampTask,(void*)0,(OS_STK*)&STAMP_TASK_STK[STAMP_STK_SIZE-1],STAMP_TASK_PRIO);//钢印
	OSTaskCreate(pagingTask,(void*)0,(OS_STK*)&PAGING_TASK_STK[PAGING_STK_SIZE-1],PAGING_TASK_PRIO);//分页
	OSTaskCreate(comTask,(void*)0,(OS_STK*)&COM_TASK_STK[COM_STK_SIZE-1],COM_TASK_PRIO);//通讯扫描
	OSTaskSuspend(APP_TASK_START_PRIO);//挂起开始任务
	OS_EXIT_CRITICAL();  //退出临界区(开中断)

	//OSTimeDlyHMSM (0, 0, 0, 1);
}

void cutTask(void *p_arg)
{
   p_arg = p_arg;
	while(1)
	{
		runCutMachine(&CutMachine);
		OSTimeDly(1);
	}	
}

void stampTask(void *p_arg)
{
 p_arg = p_arg;
	while(1)
	{
		runStampMachine(&StampMachine);
		OSTimeDly(1);
	}
}

void pagingTask(void *p_arg)
{
	p_arg = p_arg;
	

  while(1)
	{
		runPagingMachine(&PagingMachine);
		OSTimeDly(1);
	}
}

void comTask(void *p_arg)
{
	uint8_t aTxBuffer[256];
	int16_t tempRead;
	static u8 tempChoose = W1;
	
		
	while(1)
	{   
	time_count ++;
		if(time_count % 5 == 0)
		{
			Time_Flag |= BUTTONLING;
		}
		
		if(time_count % (SAMPLING_PERIOD) == 0)//20ms读取一次编码器数值
		{
			Time_Flag |= SAMPLING;
		}
		

		if(time_count % (M_PERIOD) == 0)//20ms读取一次编码器数值
		{
			Time_Flag |= MESUINGLING;
		}
				if(Time_Flag & BUTTONLING){
			button_ticks();
		  Time_Flag &= ~BUTTONLING;
		}
		
		
		// 采样和控制周期为20ms
		if(Time_Flag & SAMPLING){
		//	runPIDPagingMachineStep();
		//	measuringSpeed();
			Time_Flag &= ~SAMPLING; 
		}
		if(Time_Flag & MESUINGLING){	


//			tempChoose = !tempChoose;
//			tempRead = readTemperature(tempChoose);
//			if(tempRead == -1){
//			//	printf("K型热偶连接错误\r\n");
//			}else{
//				//printf("K型热偶%d\r\n",tempRead);
//			}
			
			CertificateCom_Scan();
			
			//printf("分页捕获值:%d\r\n",PagingCaptureNumber);
			
		//	measuringSpeed();
//			sprintf((char*)aTxBuffer,"捕获值:%d 当前位置:%.2fmm\r\n",CaptureNumber,(float)(CaptureNumber*MPP));
	//		sprintf((char *)aTxBuffer,"捕获值:%d--速度:%.1f mm/s\r\n",CaptureNumber,(float)SUM_Pulse*MPP);
     // sprintf((char *)aTxBuffer+strlen((const char*)aTxBuffer),"1s内编码器计数值:%d\r\n",SUM_Pulse);
			//sprintf((char*)aTxBuffer+strlen((const char*)aTxBuffer),"CPU使用率:%d\r\n",OSCPUUsage);	
//				printf("捕获值:%d--速度:%.1f mm/s\r\n",CaptureNumber,(float)SUM_Pulse*MPP);
		 // HAL_UART_Transmit_DMA(&husartx, aTxBuffer, strlen((const char*)aTxBuffer));
		  
			
			Time_Flag &= ~MESUINGLING; 

		}
			OSTimeDly(1);
	}
}

/**
  * 函数功能: 串口接收完成回调函数
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)
{
  //HAL_UART_Transmit(&husartx,&aRxBuffer,1,0);
	StmCom_ReceiveComData(0,aRxBuffer); //存入FIFO
  HAL_UART_Receive_IT(&husartx,&aRxBuffer,1);
}

/**
  * 函数功能: 按键1按下去的回调函数
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */
void CUTBTN_PRESS_DOWN_Handler(void* btn)
{
 //	HAL_GPIO_TogglePin(LED_ONE_PORT,LED_ONE_OUT_PIN);
	//穿膜2mm
	manualCutMem(&CutMachine);
 // controlPIDPagingMachineStep();
}

void CUTBTN_LONG_PRESS_Handler(void* btn)
{
 	//HAL_GPIO_TogglePin(LED_ONE_PORT,LED_ONE_OUT_PIN);
	continuousCutMem(&CutMachine);
}

void CUTBTN_PRESS_UP_Handler(void* btn)
{
//	HAL_GPIO_TogglePin(LED_ONE_PORT,LED_ONE_OUT_PIN);
		StopCutMem(&CutMachine);
}
/**
  * 函数功能: 按键2
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */
void PLASTICBTN_PRESS_DOWN_Handler(void* btn)
{
	//HAL_GPIO_TogglePin(LED_TWO_PORT,LED_TWO_OUT_PIN);
	manualPlasticMem(&PagingMachine);
}

void PLASTICBTN_LONG_PRESS_Handler(void* btn)
{
	//HAL_GPIO_TogglePin(LED_TWO_PORT,LED_TWO_OUT_PIN);
	continuousPlasticMem(&PagingMachine);
}

void PLASTICBBTN_PRESS_UP_Handler(void* btn)
{
	//HAL_GPIO_TogglePin(LED_TWO_PORT,LED_TWO_OUT_PIN);
	stopPlasticMem(&PagingMachine);
}
/**
  * 函数功能: 按键3 
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：工作结束按下按键最后一张出来，不然就暂停节省耗材
  */
void WORKFINISH_PRESS_DOWN_Handler(void* btn)
{
			Machine_Status |= WORK_FINISH;
}

/**
  * 函数功能: 按键4 
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：有纸按下启动
  */
void PAPEREXITBTN_PRESS_DOWN_Handler(void* btn)
{
	 controlPagingMachine(&PagingMachine);
	 controlCutMachine(&CutMachine);
	 S_target = 8000; //按按键就默认值
}
