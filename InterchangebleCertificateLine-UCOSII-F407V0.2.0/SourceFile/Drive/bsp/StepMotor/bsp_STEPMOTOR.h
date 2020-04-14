#ifndef __STEPMOTOR_TIM_H__
#define __STEPMOTOR_TIM_H__

/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* ���Ͷ��� ------------------------------------------------------------------*/
typedef enum {False = 0, True = !False} bool;

typedef struct {
  __IO uint8_t  run_state ;  // �����ת״̬
  __IO uint8_t  dir ;        // �����ת����
  __IO int32_t  step_delay;  // �¸��������ڣ�ʱ������������ʱΪ���ٶ�
  __IO uint32_t decel_start; // ��������λ��
  __IO int32_t  decel_val;   // ���ٽ׶β���
  __IO int32_t  min_delay;   // ��С��������(����ٶȣ������ٶ��ٶ�)
  __IO int32_t  accel_count; // �Ӽ��ٽ׶μ���ֵ
	__IO int32_t  medle_delay;

}speedRampData;

typedef struct {
  uint16_t  Pulse_Pin ; 	// ��ʱ�������������
  uint32_t  Channel;		  // ��ʱ��ͨ��
  uint32_t  IT_CCx ;  		// ��ʱ��ͨ���ж�ʹ��λ
  uint32_t  Flag_CCx ;    // ��ʱ��SR�жϱ��λ
}Tim;

typedef struct{\
  uint16_t  Ena_Pin ;     //���ʹ�����ű��
  uint16_t  Dir_Pin ;     //����������ű��
  GPIO_TypeDef *Dir_Port; //����������Ŷ˿�
  GPIO_TypeDef *Ena_Port; //���ʹ�����Ŷ˿�
}StepMotor;

typedef struct{\
  uint16_t  Ena_Pin ;     //���ʹ�����ű��
  GPIO_TypeDef *Ena_Port; //���ʹ�����Ŷ˿�
}CutStepMotor;

/* �궨�� --------------------------------------------------------------------*/
#define STEPMOTOR_TIMx                        TIM1
#define STEPMOTOR_TIM_RCC_CLK_ENABLE()        __HAL_RCC_TIM1_CLK_ENABLE()
#define STEPMOTOR_TIM_RCC_CLK_DISABLE()       __HAL_RCC_TIM1_CLK_DISABLE()
#define STEPMOTOR_TIM_IT_CCx                  TIM_IT_CC1
#define STEPMOTOR_TIM_FLAG_CCx                TIM_FLAG_CC1
#define STEPMOTOR_TIMx_IRQn                   TIM1_CC_IRQn
#define STEPMOTOR_TIMx_IRQHandler             TIM1_CC_IRQHandler

#define STEPMOTOR_TIM_CHANNEL_x               TIM_CHANNEL_1
#define STEPMOTOR_TIM_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOA_CLK_ENABLE()     // ���������������������
#define STEPMOTOR_TIM_PUL_PORT                GPIOA                            // ��Ӧ��������PUL-��������ʹ�ù����ӷ���
#define STEPMOTOR_TIM_PUL_PIN                 GPIO_PIN_8                       // ��PLU+ֱ�ӽӿ������VCC
#define GPIO_AFx_TIMx                         GPIO_AF1_TIM1

#define STEPMOTOR_DIR_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOC_CLK_ENABLE()     // �����ת������ƣ�������ղ���Ĭ����ת
#define STEPMOTOR_DIR_PORT                    GPIOC                            // ��Ӧ��������DIR-��������ʹ�ù����ӷ���
#define STEPMOTOR_DIR_PIN                     GPIO_PIN_14                       // ��DIR+ֱ�ӽӿ������VCC
#define GPIO_PIN_AF_AS_SYS                    GPIO_AF0_RTC_50Hz                // ���Ų���Ϊ���ù���ʹ��

#define STEPMOTOR_ENA_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOC_CLK_ENABLE()     // ����ѻ�ʹ�ܿ��ƣ�������ղ���Ĭ��ʹ�ܵ��
#define STEPMOTOR_ENA_PORT                    GPIOC                            // ��Ӧ��������ENA-��������ʹ�ù����ӷ���
#define STEPMOTOR_ENA_PIN                     GPIO_PIN_13                       // ��ENA+ֱ�ӽӿ������VCC


//PagingMachineStep PM=PagingMachine
#define PM_FRICTION_STEP_TIMx                        TIM1
#define PM_FRICTION_STEP_TIM_RCC_CLK_ENABLE()        __HAL_RCC_TIM1_CLK_ENABLE()
#define PM_FRICTION_STEP_TIM_RCC_CLK_DISABLE()       __HAL_RCC_TIM1_CLK_DISABLE()
#define PM_FRICTION_STEP_TIM_IT_CCx                  TIM_IT_CC3
#define PM_FRICTION_STEP_TIM_FLAG_CCx                TIM_FLAG_CC3
#define PM_FRICTION_STEP_TIMx_IRQn                   TIM1_CC_IRQn
#define PM_FRICTION_STEP_TIMx_IRQHandler             TIM1_CC_IRQHandler

#define PM_FRICTION_STEP_TIM_CHANNEL_x               TIM_CHANNEL_3
#define PM_FRICTION_STEP_TIM_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOA_CLK_ENABLE()     // ���������������������
#define PM_FRICTION_STEP_TIM_PUL_PORT                GPIOA                            // ��Ӧ��������PUL-��������ʹ�ù����ӷ���
#define PM_FRICTION_STEP_TIM_PUL_PIN                 GPIO_PIN_10                       // ��PLU+ֱ�ӽӿ������VCC
#define PM_FRICTION_STEP_GPIO_AFx_TIMx               GPIO_AF1_TIM1

#define PM_FRICTION_STEP_DIR_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOF_CLK_ENABLE()     // �����ת������ƣ�������ղ���Ĭ����ת
#define PM_FRICTION_STEP_DIR_PORT                    GPIOF                            // ��Ӧ��������DIR-��������ʹ�ù����ӷ���
#define PM_FRICTION_STEP_DIR_PIN                     GPIO_PIN_0                       // ��DIR+ֱ�ӽӿ������VCC
#define PM_FRICTION_STEP_GPIO_PIN_AF_AS_SYS                    GPIO_AF0_RTC_50Hz                // ���Ų���Ϊ���ù���ʹ��

#define PM_FRICTION_STEP_ENA_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOC_CLK_ENABLE()     // ����ѻ�ʹ�ܿ��ƣ�������ղ���Ĭ��ʹ�ܵ��
#define PM_FRICTION_STEP_ENA_PORT                    GPIOC                            // ��Ӧ��������ENA-��������ʹ�ù����ӷ���
#define PM_FRICTION_STEP_ENA_PIN                     GPIO_PIN_15                       // ��ENA+ֱ�ӽӿ������VCC

#define PM_ROLLER_STEP_TIMx                        TIM1
#define PM_ROLLER_STEP_TIM_RCC_CLK_ENABLE()        __HAL_RCC_TIM1_CLK_ENABLE()
#define PM_ROLLER_STEP_TIM_RCC_CLK_DISABLE()       __HAL_RCC_TIM1_CLK_DISABLE()
#define PM_ROLLER_STEP_TIM_IT_CCx                  TIM_IT_CC4
#define PM_ROLLER_STEP_TIM_FLAG_CCx                TIM_FLAG_CC4
#define PM_ROLLER_STEP_TIMx_IRQn                   TIM1_CC_IRQn
#define PM_ROLLER_STEP_TIMx_IRQHandler             TIM1_CC_IRQHandler

#define PM_ROLLER_STEP_TIM_CHANNEL_x               TIM_CHANNEL_4
#define PM_ROLLER_STEP_TIM_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOA_CLK_ENABLE()     // ���������������������
#define PM_ROLLER_STEP_TIM_PUL_PORT                GPIOA                            // ��Ӧ��������PUL-��������ʹ�ù����ӷ���
#define PM_ROLLER_STEP_TIM_PUL_PIN                 GPIO_PIN_11                       // ��PLU+ֱ�ӽӿ������VCC
#define PM_ROLLER_STEP_GPIO_AFx_TIMx               GPIO_AF1_TIM1

#define PM_ROLLER_STEP_DIR_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOC_CLK_ENABLE()     // �����ת������ƣ�������ղ���Ĭ����ת
#define PM_ROLLER_STEP_DIR_PORT                    GPIOC                            // ��Ӧ��������DIR-��������ʹ�ù����ӷ���
#define PM_ROLLER_STEP_DIR_PIN                     GPIO_PIN_14                       // ��DIR+ֱ�ӽӿ������VCC
#define PM_ROLLER_STEP_GPIO_PIN_AF_AS_SYS                    GxPIO_AF0_RTC_50Hz                // ���Ų���Ϊ���ù���ʹ��

#define PM_ROLLER_STEP_ENA_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOC_CLK_ENABLE()     // ����ѻ�ʹ�ܿ��ƣ�������ղ���Ĭ��ʹ�ܵ��
#define PM_ROLLER_STEP_ENA_PORT                    GPIOC                            // ��Ӧ��������ENA-��������ʹ�ù����ӷ���
#define PM_ROLLER_STEP_ENA_PIN                     GPIO_PIN_13                       // ��ENA+ֱ�ӽӿ������VCC

//
#define SM_ROLLER_STEP_TIMx                        TIM1
#define SM_STEP_TIM_RCC_CLK_ENABLE()        __HAL_RCC_TIM1_CLK_ENABLE()
#define SM_STEP_TIM_RCC_CLK_DISABLE()       __HAL_RCC_TIM1_CLK_DISABLE()
#define SM_STEP_TIM_IT_CCx                  TIM_IT_CC2
#define SM_STEP_TIM_FLAG_CCx                TIM_FLAG_CC2
#define SM_STEP_TIMx_IRQn                   TIM1_CC_IRQn
#define SM_STEP_TIMx_IRQHandler             TIM1_CC_IRQHandler

#define SM_TIM_CHANNEL_x               TIM_CHANNEL_2
#define SM_TIM_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOA_CLK_ENABLE()     // ���������������������
#define SM_TIM_PUL_PORT                GPIOA                            // ��Ӧ��������PUL-��������ʹ�ù����ӷ���
#define SM_TIM_PUL_PIN                 GPIO_PIN_9                       // ��PLU+ֱ�ӽӿ������VCC
#define SM_GPIO_AFx_TIMx               GPIO_AF1_TIM1

#define SM_STEP_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOF_CLK_ENABLE()     // �����ת������ƣ�������ղ���Ĭ����ת
#define SM_STEP_DIR_PORT                    GPIOF                            // ��Ӧ��������DIR-��������ʹ�ù����ӷ���
#define SM_STEP_DIR_PIN                     GPIO_PIN_4                       // ��DIR+ֱ�ӽӿ������VCC
#define SM_STEP_GPIO_PIN_AF_AS_SYS                    GPIO_AF0_RTC_50Hz                // ���Ų���Ϊ���ù���ʹ��

#define SM_STEP_ENA_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOF_CLK_ENABLE()     // ����ѻ�ʹ�ܿ��ƣ�������ղ���Ĭ��ʹ�ܵ��
#define SM_STEP_ENA_PORT                    GPIOF                            // ��Ӧ��������ENA-��������ʹ�ù����ӷ���
#define SM_STEP_ENA_PIN                     GPIO_PIN_3                       // ��ENA+ֱ�ӽӿ������VCC

#define CER_STEPMOTOR_DIR_FORWARD(Axis) \
HAL_GPIO_WritePin(Stepmotor[Axis].Dir_Port,Stepmotor[Axis].Dir_Pin,GPIO_PIN_RESET)//���õ������,����Axis:��ǰ���
#define CER_STEPMOTOR_DIR_REVERSAL(Axis)\
HAL_GPIO_WritePin(Stepmotor[Axis].Dir_Port,Stepmotor[Axis].Dir_Pin,GPIO_PIN_SET)

#define CER_STEPMOTOR_OUTPUT_ENABLE(Axis)\
HAL_GPIO_WritePin(Stepmotor[Axis].Ena_Port,Stepmotor[Axis].Ena_Pin,GPIO_PIN_SET)//���ʹ�ܿ��� ����Axis:��ǰ���
#define CER_STEPMOTOR_OUTPUT_DISABLE(Axis)\
HAL_GPIO_WritePin(Stepmotor[Axis].Ena_Port,Stepmotor[Axis].Ena_Pin,GPIO_PIN_RESET)


//PagingMachinePIDStep

#define CUT_MACHINE_TIMx                        TIM9
#define CUT_MACHINE_TIM_RCC_CLK_ENABLE()        __HAL_RCC_TIM9_CLK_ENABLE()
#define CUT_MACHINE_TIM_RCC_CLK_DISABLE()       __HAL_RCC_TIM9_CLK_DISABLE()
#define CUT_MACHINE_TIMx_IRQn                   TIM1_BRK_TIM9_IRQn
#define CUT_MACHINE_TIMx_IRQHandler             TIM1_BRK_TIM9_IRQHandler

#define CUT_CHAMFER_STEP_TIMx                        TIM9
#define CUT_CHAMFER_STEP_TIM_RCC_CLK_ENABLE()        __HAL_RCC_TIM9_CLK_ENABLE()
#define CUT_CHAMFER_STEP_TIM_RCC_CLK_DISABLE()       __HAL_RCC_TIM9_CLK_DISABLE()
#define CUT_CHAMFER_STEP_TIM_IT_CCx                  TIM_IT_CC1
#define CUT_CHAMFER_STEP_TIM_FLAG_CCx                TIM_FLAG_CC1
#define CUT_CHAMFER_STEP_TIMx_IRQn                   TIM1_BRK_TIM9_IRQn
#define CUT_CHAMFER_STEP_TIMx_IRQHandler             TIM1_BRK_TIM9_IRQHandler

#define CUT_CHAMFER_STEP_TIM_CHANNEL_x               TIM_CHANNEL_1
#define CUT_CHAMFER_STEP_TIM_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOE_CLK_ENABLE()     // ���������������������
#define CUT_CHAMFER_STEP_TIM_PUL_PORT                GPIOE                            // ��Ӧ��������PUL-��������ʹ�ù����ӷ���
#define CUT_CHAMFER_STEP_TIM_PUL_PIN                 GPIO_PIN_5                       // ��PLU+ֱ�ӽӿ������VCC
#define CUT_CHAMFER_STEP_GPIO_AFx_TIMx               GPIO_AF3_TIM9

#define CUT_CHAMFER_STEP_ENA_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOG_CLK_ENABLE()     // ����ѻ�ʹ�ܿ��ƣ�������ղ���Ĭ��ʹ�ܵ��
#define CUT_CHAMFER_STEP_ENA_PORT                    GPIOG                            // ��Ӧ��������ENA-��������ʹ�ù����ӷ���
#define CUT_CHAMFER_STEP_ENA_PIN                     GPIO_PIN_5                       // ��ENA+ֱ�ӽӿ������VCC

#define CUT_ROLLER_STEP_TIMx                        TIM9
#define CUT_ROLLER_STEP_TIM_RCC_CLK_ENABLE()        __HAL_RCC_TIM9_CLK_ENABLE()
#define CUT_ROLLER_STEP_TIM_RCC_CLK_DISABLE()       __HAL_RCC_TIM9_CLK_DISABLE()
#define CUT_ROLLER_STEP_TIM_IT_CCx                  TIM_IT_CC2
#define CUT_ROLLER_STEP_TIM_FLAG_CCx                TIM_FLAG_CC2
#define CUT_ROLLER_STEP_TIMx_IRQn                   TIM1_BRK_TIM9_IRQn
#define CUT_ROLLER_STEP_TIMx_IRQHandler             TIM1_BRK_TIM9_IRQHandler

#define CUT_ROLLER_STEP_TIM_CHANNEL_x               TIM_CHANNEL_2
#define CUT_ROLLER_STEP_TIM_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOE_CLK_ENABLE()     // ���������������������
#define CUT_ROLLER_STEP_TIM_PUL_PORT                GPIOE                            // ��Ӧ��������PUL-��������ʹ�ù����ӷ���
#define CUT_ROLLER_STEP_TIM_PUL_PIN                 GPIO_PIN_6                       // ��PLU+ֱ�ӽӿ������VCC
#define CUT_ROLLER_STEP_GPIO_AFx_TIMx               GPIO_AF3_TIM9

#define CUT_ROLLER_STEP_ENA_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOG_CLK_ENABLE()     // ����ѻ�ʹ�ܿ��ƣ�������ղ���Ĭ��ʹ�ܵ��
#define CUT_ROLLER_STEP_ENA_PORT                    GPIOG                            // ��Ӧ��������ENA-��������ʹ�ù����ӷ���
#define CUT_ROLLER_STEP_ENA_PIN                     GPIO_PIN_6                       // ��ENA+ֱ�ӽӿ������VCC

#define CER_CUT_STEPMOTOR_OUTPUT_ENABLE(Axis)\
HAL_GPIO_WritePin(CutStepmotor[Axis].Ena_Port,CutStepmotor[Axis].Ena_Pin,GPIO_PIN_SET)//���ʹ�ܿ��� ����Axis:��ǰ���
#define CER_CUT_STEPMOTOR_OUTPUT_DISABLE(Axis)\
HAL_GPIO_WritePin(CutStepmotor[Axis].Ena_Port,CutStepmotor[Axis].Ena_Pin,GPIO_PIN_RESET)



// ���嶨ʱ��Ԥ��Ƶ����ʱ��ʵ��ʱ��Ƶ��Ϊ��168MHz/��STEPMOTOR_TIMx_PRESCALER+1��
// ���ڲ������λ�û�������Ҫ����ʱ��Ƶ�ʽ���,Ϊ�˱���������
#define STEPMOTOR_TIM_PRESCALER               7  // �������������ϸ������Ϊ��   32  ϸ��

// 2020.3.28#define STEPMOTOR_TIM_PRESCALER               5  // �������������ϸ������Ϊ��   32  ϸ��

//#define STEPMOTOR_TIM_PRESCALER               15  // �������������ϸ������Ϊ��   16  ϸ��
//#define STEPMOTOR_TIM_PRESCALER               31  // �������������ϸ������Ϊ��   8  ϸ��
//#define STEPMOTOR_TIM_PRESCALER               63  // �������������ϸ������Ϊ��   4  ϸ��
//#define STEPMOTOR_TIM_PRESCALER               127  // �������������ϸ������Ϊ��   2  ϸ��
//#define STEPMOTOR_TIM_PRESCALER               255 // �������������ϸ������Ϊ��   1  ϸ��


// ���嶨ʱ�����ڣ�����Ƚ�ģʽ��������Ϊ0xFFFF
#define STEPMOTOR_TIM_PERIOD                   0xFFFF
// ����߼���ʱ���ظ������Ĵ���ֵ
#define STEPMOTOR_TIM_REPETITIONCOUNTER       0

//��������˿�ܵ�һЩ����
#define SECOND                                1000  // ����:1s == 1000 ms
#define SAMPLING_PERIOD                       20    // �������������� ��λms
#define M_PERIOD                              1000    // �������������� ��λms

#define FREQ_UINT                             (T1_FREQ_TIM9/(SECOND/SAMPLING_PERIOD))    //�Զ�ʱ����Ƶ������λ����,������ֵ̫�����

#define T1_FREQ_TIM9                          (SystemCoreClock/(279+1)) // Ƶ��ftֵ
#define ENCODER_SPR              			        (500*4)     		// ��������Ȧ����*4��Ƶ
#define MPR                         		      (78.5)       		    // ���������תһȦ,˿�ܵľ���;��λ��mm/r 
#define PPM      							                (ENCODER_SPR/MPR)  		// ÿmm�ڱ�������������;��λ:Pules/mm
#define P_PERIOD                              ((float)PPM/(SECOND/SAMPLING_PERIOD))  // ÿ���������ڶ�Ӧ�������� 
#define MPP                                   ((float)(MPR)/ENCODER_SPR) // ������������������

//// �����������������һЩ����
#define T1_FREQ                               (SystemCoreClock/(STEPMOTOR_TIM_PRESCALER+1)) // ��ʱ��1Ƶ��Fֵ
#define FSPR                                  200         //���������Ȧ����
#define MICRO_STEP                            32          // �������������ϸ����
#define SPR                                   ((float)(FSPR*MICRO_STEP))   // ��תһȦ��Ҫ��������

#define FEEDBACK_CONST                        (SPR/ENCODER_SPR)   //�������Ͳ�������������ı�ֵ

#define CCW                                   1
#define CW                                    0


//���μӼ���
#define STOP                                  0 // �Ӽ�������״̬��ֹͣ
#define ACCEL                                 1 // �Ӽ�������״̬�����ٽ׶�
#define DECEL                                 2 // �Ӽ�������״̬�����ٽ׶�
#define RUN                                   3 // �Ӽ�������״̬�����ٽ׶�
#define T1_FREQ                               (SystemCoreClock/(STEPMOTOR_TIM_PRESCALER+1)) // Ƶ��ftֵ
#define FSPR                                  200         //���������Ȧ����
#define MICRO_STEP                            32          // �������������ϸ����
//#define SPR                                   (FSPR*MICRO_STEP)   // ��תһȦ��Ҫ��������

// ��ѧ����
#define ALPHA                                 ((float)(2*3.14159/SPR))       // ��= 2*pi/spr
#define A_T_x10                               ((float)(10*ALPHA*T1_FREQ))
#define T1_FREQ_148                           ((float)((T1_FREQ*0.676)/10)) // 0.676Ϊ�������ֵ
#define A_SQ                                  ((float)(2*100000*ALPHA)) 
#define A_x200                                ((float)(200*ALPHA))

#define   CUT_FLASM_STEP      0 
#define 	PLASTIC_STEP   		  1
#define 	FRICTION_STEP  		  2   //����������
#define 	ROLLER_STEP       	3
//TIM9
#define   CUT_CHAMFER_STEP	  0
#define   CUT_ROLLER_STEP		  1

/* ��չ���� ------------------------------------------------------------------*/
extern TIM_HandleTypeDef htimx_STEPMOTOR;
extern TIM_HandleTypeDef cutMachine_STEPMOTOR; 
extern __IO uint16_t Toggle_Pulse;
extern __IO uint8_t  MotionStatus;           //�Ƿ����˶���0��ֹͣ��1���˶�
extern bool stopCmd;
extern __IO uint32_t listStep_count[4];
extern bool listStopCmd[4]; // ֽ��ֹͣ����
extern bool listCutStepStopCmd[2];
extern __IO uint8_t  cerMotionStatus[4];        // �Ƿ����˶���0��ֹͣ��1���˶�
extern __IO uint8_t  cerCutMotionStatus[2];
extern speedRampData cerStepSrd[4]; 
extern speedRampData cerCutStepSrd[2];
extern __IO uint32_t listCutStep_count[2];
/* �������� ------------------------------------------------------------------*/
void STEPMOTOR_TIMx_Init(void);
void cutMachineStep_TIM9_Init(void);
void STEPMOTOR_Motion_Ctrl(uint8_t Dir , float Frequency);
void STEPMOTOR_AxisMoveRel( int32_t step, uint32_t accel, uint32_t decel, uint32_t speed);
void HAL_TIM_OC_Callback(uint8_t Axis);
void STEPMOTOR_CerAxisMoveRel(uint8_t Axis,int32_t step,uint32_t accel, uint32_t decel, uint32_t speed);

void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim);

void HAL_TIM_OC_Callback_TIM9(uint8_t Axis);
void STEPMOTOR_CerAxisMoveRel_CutTim9(uint8_t Axis,int32_t step,uint32_t accel, uint32_t decel, uint32_t speed);

#endif	/* __STEPMOTOR_TIM_H__ */
