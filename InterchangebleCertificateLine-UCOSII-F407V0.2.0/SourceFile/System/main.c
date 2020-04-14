/*-------------------------------<< ������>>-----------------------------------*/
/*
 * - :CRJ--
 * - :2020.0.0.0.1  
 * - void main(void)
 * - :STM32F407-CRJ-UCOSII�汾
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
#define SAMPLING                  0x01    // �������
#define BUTTONLING                 0x02 
#define MESUINGLING                 0x04 

uint16_t time_count = 0;              // ʱ�������ÿ1ms����һ(��δ�ʱ��Ƶ���й�)
uint8_t Time_Flag   = 0;              // ����ʱ����
 
uint8_t aTxBuffer;
uint8_t aRxBuffer;                         // ��������

__IO uint16_t Machine_Status   = 0;
extern int32_t CaptureNumber;     // ���벶����
extern int32_t PagingCaptureNumber;
extern int32_t SUM_Pulse;

//�����������ȼ�
#define  APP_TASK_START_PRIO           20			//��ʼ��������ȼ�Ϊ���
#define CUT_TASK_PRIO			             11
#define STAMP_TASK_PRIO			           12
#define PAGING_TASK_PRIO			         13
#define COM_TASK_PRIO                  19

//���������ջ��С
#define  APP_TASK_START_STK_SIZE       256
#define CUT_STK_SIZE			             256
#define STAMP_STK_SIZE			           256
#define PAGING_STK_SIZE			           1024
#define COM_STK_SIZE			             1024

//�����ջ
OS_STK appTaskStartStk[CUT_STK_SIZE];
OS_STK CUT_TASK_STK[CUT_STK_SIZE];
OS_STK STAMP_TASK_STK[STAMP_STK_SIZE];
OS_STK PAGING_TASK_STK[PAGING_STK_SIZE];
OS_STK COM_TASK_STK[COM_STK_SIZE];

//������
void appTaskStart (void* p_arg);
void cutTask(void* p_arg);
void stampTask(void *p_arg);
void pagingTask(void *p_arg);
void comTask(void *p_arg);

//�����ص�����
void CUTBTN_PRESS_DOWN_Handler(void* btn);
void CUTBTN_LONG_PRESS_Handler(void* btn);
void CUTBTN_PRESS_UP_Handler(void* btn);
void PLASTICBTN_PRESS_DOWN_Handler(void* btn);
void PLASTICBTN_LONG_PRESS_Handler(void* btn);
void PLASTICBBTN_PRESS_UP_Handler(void* btn);
void WORKFINISH_PRESS_DOWN_Handler(void* btn);
void PAPEREXITBTN_PRESS_DOWN_Handler(void* btn);

OS_EVENT *CutFullSem; // ��ӡ�������ź���
/*SYSTICK ���ã� 1msһ��systick�жϣ�����ucos ���Ƚ���*/
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
 
  __HAL_RCC_PWR_CLK_ENABLE();                                     // ʹ��PWRʱ��

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);  // ���õ�ѹ�������ѹ����1

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;      // �ⲿ����8MHz
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;                        // ��HSE 
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;                    // ��PLL
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;            // PLLʱ��Դѡ��HSE
  RCC_OscInitStruct.PLL.PLLM = 8;                                 // 8��ƵMHz
  RCC_OscInitStruct.PLL.PLLN = 336;                               // 336��Ƶ
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;                     // 2��Ƶ���õ�168MHz��ʱ��
  RCC_OscInitStruct.PLL.PLLQ = 7;                                 // USB/SDIO/������������ȵ���PLL��Ƶϵ��
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;       // ϵͳʱ�ӣ�168MHz
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;              // AHBʱ�ӣ� 168MHz
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;               // APB1ʱ�ӣ�42MHz
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;               // APB2ʱ�ӣ�84MHz
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);

 // HAL_RCC_EnableCSS();                                            // ʹ��CSS���ܣ�����ʹ���ⲿ�����ڲ�ʱ��ԴΪ����
  
 	// HAL_RCC_GetHCLKFreq()/1000    1ms�ж�һ��
	// HAL_RCC_GetHCLKFreq()/100000	 10us�ж�һ��
	// HAL_RCC_GetHCLKFreq()/1000000 1us�ж�һ��
//  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);                 // ���ò�����ϵͳ�δ�ʱ��
  /* ϵͳ�δ�ʱ��ʱ��Դ */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* ϵͳ�δ�ʱ���ж����ȼ����� */
 HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
	
}

static void initializeHardWare(void)
{
  HAL_Init();
  /* ����ϵͳʱ�� */
  SystemClock_Config();
  /* ���ذ�����ʼ�� */
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
		  /* ��ʼ��PID�����ṹ�� */
    Init_PIDStruct();
  /* ���Դ��ڳ�ʼ�� */
  USARTx_Init(); 
	
  CommandCallBackInit();//�ص�����
  
	CertificateCom_Init();//����FIFO��������
	
	initSensorIO();//������IO
	
	initReadTemperature();
	
	/* PWM������� */
	initMotorPWM();
	
  initMotorIOPushOut();	
 
	/* ��������ʱ����ʼ�����������벶���� */
   ENCODER_TIMx_Init();
  /* �����������ӿ� */
  HAL_TIM_Encoder_Start(&htimx_Encoder, TIM_CHANNEL_ALL);
  HAL_TIM_Encoder_Start(&htimx_EncoderTwo, TIM_CHANNEL_ALL);
	
  /* ���������ʱ����ʼ��*/
  STEPMOTOR_TIMx_Init();

	cutMachineStep_TIM9_Init(); //TIM9
	
	//initCutAllMotor(&CutMachine);
  /* ʹ�ܽ��գ������жϻص����� */
  HAL_UART_Receive_IT(&husartx,&aRxBuffer,1);
	
	Vel_Target = (vPID.SetPoint*P_PERIOD);
		
		
	listInit(&CutStepList); //����
 // listInit(&StampStepList);//��ӡ
	
}

int main(void)
{

  initializeHardWare();// ��ʼ��Ӳ�� 

	OSInit();  //UCOS��ʼ��

	CutFullSem = OSSemCreate(1);  //��ʼֵ
	
	OSTaskCreate (appTaskStart,	  		  		//ָ����������ָ��
								(void *) 0,								  		//����ʼִ��ʱ�����ݸ�����Ĳ�����ָ��
								(OS_STK *) &appTaskStartStk[APP_TASK_START_STK_SIZE - 1],	//���������Ķ�ջ��ջ��ָ��   �Ӷ����µݼ�
								(INT8U) APP_TASK_START_PRIO);

		OSStart();
}

void appTaskStart (void* p_arg)
{
	(void) p_arg;
		OS_CPU_SR cpu_sr=0;

	/*��ʼ��ucosʱ�ӽ���*/
	  OS_CPU_SysTickInit();

	/*ʹ��ucos ��ͳ������*/
#if (OS_TASK_STAT_EN > 0)
	/*ͳ�������ʼ����*/
	OSStatInit();               
#endif

	OS_ENTER_CRITICAL();  //�����ٽ���(�ر��ж�)
	OSTaskCreate(cutTask,(void*)0,(OS_STK*)&CUT_TASK_STK[CUT_STK_SIZE-1],CUT_TASK_PRIO);//����
	//OSTaskCreate(stampTask,(void*)0,(OS_STK*)&STAMP_TASK_STK[STAMP_STK_SIZE-1],STAMP_TASK_PRIO);//��ӡ
	OSTaskCreate(pagingTask,(void*)0,(OS_STK*)&PAGING_TASK_STK[PAGING_STK_SIZE-1],PAGING_TASK_PRIO);//��ҳ
	OSTaskCreate(comTask,(void*)0,(OS_STK*)&COM_TASK_STK[COM_STK_SIZE-1],COM_TASK_PRIO);//ͨѶɨ��
	OSTaskSuspend(APP_TASK_START_PRIO);//����ʼ����
	OS_EXIT_CRITICAL();  //�˳��ٽ���(���ж�)

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
		
		if(time_count % (SAMPLING_PERIOD) == 0)//20ms��ȡһ�α�������ֵ
		{
			Time_Flag |= SAMPLING;
		}
		

		if(time_count % (M_PERIOD) == 0)//20ms��ȡһ�α�������ֵ
		{
			Time_Flag |= MESUINGLING;
		}
				if(Time_Flag & BUTTONLING){
			button_ticks();
		  Time_Flag &= ~BUTTONLING;
		}
		
		
		// �����Ϳ�������Ϊ20ms
		if(Time_Flag & SAMPLING){
		//	runPIDPagingMachineStep();
		//	measuringSpeed();
			Time_Flag &= ~SAMPLING; 
		}
		if(Time_Flag & MESUINGLING){	


//			tempChoose = !tempChoose;
//			tempRead = readTemperature(tempChoose);
//			if(tempRead == -1){
//			//	printf("K����ż���Ӵ���\r\n");
//			}else{
//				//printf("K����ż%d\r\n",tempRead);
//			}
			
			CertificateCom_Scan();
			
			//printf("��ҳ����ֵ:%d\r\n",PagingCaptureNumber);
			
		//	measuringSpeed();
//			sprintf((char*)aTxBuffer,"����ֵ:%d ��ǰλ��:%.2fmm\r\n",CaptureNumber,(float)(CaptureNumber*MPP));
	//		sprintf((char *)aTxBuffer,"����ֵ:%d--�ٶ�:%.1f mm/s\r\n",CaptureNumber,(float)SUM_Pulse*MPP);
     // sprintf((char *)aTxBuffer+strlen((const char*)aTxBuffer),"1s�ڱ���������ֵ:%d\r\n",SUM_Pulse);
			//sprintf((char*)aTxBuffer+strlen((const char*)aTxBuffer),"CPUʹ����:%d\r\n",OSCPUUsage);	
//				printf("����ֵ:%d--�ٶ�:%.1f mm/s\r\n",CaptureNumber,(float)SUM_Pulse*MPP);
		 // HAL_UART_Transmit_DMA(&husartx, aTxBuffer, strlen((const char*)aTxBuffer));
		  
			
			Time_Flag &= ~MESUINGLING; 

		}
			OSTimeDly(1);
	}
}

/**
  * ��������: ���ڽ�����ɻص�����
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)
{
  //HAL_UART_Transmit(&husartx,&aRxBuffer,1,0);
	StmCom_ReceiveComData(0,aRxBuffer); //����FIFO
  HAL_UART_Receive_IT(&husartx,&aRxBuffer,1);
}

/**
  * ��������: ����1����ȥ�Ļص�����
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
void CUTBTN_PRESS_DOWN_Handler(void* btn)
{
 //	HAL_GPIO_TogglePin(LED_ONE_PORT,LED_ONE_OUT_PIN);
	//��Ĥ2mm
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
  * ��������: ����2
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
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
  * ��������: ����3 
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ���������������°������һ�ų�������Ȼ����ͣ��ʡ�Ĳ�
  */
void WORKFINISH_PRESS_DOWN_Handler(void* btn)
{
			Machine_Status |= WORK_FINISH;
}

/**
  * ��������: ����4 
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������ֽ��������
  */
void PAPEREXITBTN_PRESS_DOWN_Handler(void* btn)
{
	 controlPagingMachine(&PagingMachine);
	 controlCutMachine(&CutMachine);
	 S_target = 8000; //��������Ĭ��ֵ
}
