#ifndef __STEPMOTOR_TIM_H__
#define __STEPMOTOR_TIM_H__

/* 包含头文件 ----------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* 类型定义 ------------------------------------------------------------------*/
typedef enum {False = 0, True = !False} bool;

typedef struct {
  __IO uint8_t  run_state ;  // 电机旋转状态
  __IO uint8_t  dir ;        // 电机旋转方向
  __IO int32_t  step_delay;  // 下个脉冲周期（时间间隔），启动时为加速度
  __IO uint32_t decel_start; // 启动减速位置
  __IO int32_t  decel_val;   // 减速阶段步数
  __IO int32_t  min_delay;   // 最小脉冲周期(最大速度，即匀速段速度)
  __IO int32_t  accel_count; // 加减速阶段计数值
	__IO int32_t  medle_delay;

}speedRampData;

typedef struct {
  uint16_t  Pulse_Pin ; 	// 定时器脉冲输出引脚
  uint32_t  Channel;		  // 定时器通道
  uint32_t  IT_CCx ;  		// 定时器通道中断使能位
  uint32_t  Flag_CCx ;    // 定时器SR中断标记位
}Tim;

typedef struct{\
  uint16_t  Ena_Pin ;     //电机使能引脚编号
  uint16_t  Dir_Pin ;     //电机方向引脚编号
  GPIO_TypeDef *Dir_Port; //电机方向引脚端口
  GPIO_TypeDef *Ena_Port; //电机使能引脚端口
}StepMotor;

typedef struct{\
  uint16_t  Ena_Pin ;     //电机使能引脚编号
  GPIO_TypeDef *Ena_Port; //电机使能引脚端口
}CutStepMotor;

/* 宏定义 --------------------------------------------------------------------*/
#define STEPMOTOR_TIMx                        TIM1
#define STEPMOTOR_TIM_RCC_CLK_ENABLE()        __HAL_RCC_TIM1_CLK_ENABLE()
#define STEPMOTOR_TIM_RCC_CLK_DISABLE()       __HAL_RCC_TIM1_CLK_DISABLE()
#define STEPMOTOR_TIM_IT_CCx                  TIM_IT_CC1
#define STEPMOTOR_TIM_FLAG_CCx                TIM_FLAG_CC1
#define STEPMOTOR_TIMx_IRQn                   TIM1_CC_IRQn
#define STEPMOTOR_TIMx_IRQHandler             TIM1_CC_IRQHandler

#define STEPMOTOR_TIM_CHANNEL_x               TIM_CHANNEL_1
#define STEPMOTOR_TIM_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOA_CLK_ENABLE()     // 输出控制脉冲给电机驱动器
#define STEPMOTOR_TIM_PUL_PORT                GPIOA                            // 对应驱动器的PUL-（驱动器使用共阳接法）
#define STEPMOTOR_TIM_PUL_PIN                 GPIO_PIN_8                       // 而PLU+直接接开发板的VCC
#define GPIO_AFx_TIMx                         GPIO_AF1_TIM1

#define STEPMOTOR_DIR_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOC_CLK_ENABLE()     // 电机旋转方向控制，如果悬空不接默认正转
#define STEPMOTOR_DIR_PORT                    GPIOC                            // 对应驱动器的DIR-（驱动器使用共阳接法）
#define STEPMOTOR_DIR_PIN                     GPIO_PIN_14                       // 而DIR+直接接开发板的VCC
#define GPIO_PIN_AF_AS_SYS                    GPIO_AF0_RTC_50Hz                // 引脚不作为复用功能使用

#define STEPMOTOR_ENA_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOC_CLK_ENABLE()     // 电机脱机使能控制，如果悬空不接默认使能电机
#define STEPMOTOR_ENA_PORT                    GPIOC                            // 对应驱动器的ENA-（驱动器使用共阳接法）
#define STEPMOTOR_ENA_PIN                     GPIO_PIN_13                       // 而ENA+直接接开发板的VCC


//PagingMachineStep PM=PagingMachine
#define PM_FRICTION_STEP_TIMx                        TIM1
#define PM_FRICTION_STEP_TIM_RCC_CLK_ENABLE()        __HAL_RCC_TIM1_CLK_ENABLE()
#define PM_FRICTION_STEP_TIM_RCC_CLK_DISABLE()       __HAL_RCC_TIM1_CLK_DISABLE()
#define PM_FRICTION_STEP_TIM_IT_CCx                  TIM_IT_CC3
#define PM_FRICTION_STEP_TIM_FLAG_CCx                TIM_FLAG_CC3
#define PM_FRICTION_STEP_TIMx_IRQn                   TIM1_CC_IRQn
#define PM_FRICTION_STEP_TIMx_IRQHandler             TIM1_CC_IRQHandler

#define PM_FRICTION_STEP_TIM_CHANNEL_x               TIM_CHANNEL_3
#define PM_FRICTION_STEP_TIM_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOA_CLK_ENABLE()     // 输出控制脉冲给电机驱动器
#define PM_FRICTION_STEP_TIM_PUL_PORT                GPIOA                            // 对应驱动器的PUL-（驱动器使用共阳接法）
#define PM_FRICTION_STEP_TIM_PUL_PIN                 GPIO_PIN_10                       // 而PLU+直接接开发板的VCC
#define PM_FRICTION_STEP_GPIO_AFx_TIMx               GPIO_AF1_TIM1

#define PM_FRICTION_STEP_DIR_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOF_CLK_ENABLE()     // 电机旋转方向控制，如果悬空不接默认正转
#define PM_FRICTION_STEP_DIR_PORT                    GPIOF                            // 对应驱动器的DIR-（驱动器使用共阳接法）
#define PM_FRICTION_STEP_DIR_PIN                     GPIO_PIN_0                       // 而DIR+直接接开发板的VCC
#define PM_FRICTION_STEP_GPIO_PIN_AF_AS_SYS                    GPIO_AF0_RTC_50Hz                // 引脚不作为复用功能使用

#define PM_FRICTION_STEP_ENA_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOC_CLK_ENABLE()     // 电机脱机使能控制，如果悬空不接默认使能电机
#define PM_FRICTION_STEP_ENA_PORT                    GPIOC                            // 对应驱动器的ENA-（驱动器使用共阳接法）
#define PM_FRICTION_STEP_ENA_PIN                     GPIO_PIN_15                       // 而ENA+直接接开发板的VCC

#define PM_ROLLER_STEP_TIMx                        TIM1
#define PM_ROLLER_STEP_TIM_RCC_CLK_ENABLE()        __HAL_RCC_TIM1_CLK_ENABLE()
#define PM_ROLLER_STEP_TIM_RCC_CLK_DISABLE()       __HAL_RCC_TIM1_CLK_DISABLE()
#define PM_ROLLER_STEP_TIM_IT_CCx                  TIM_IT_CC4
#define PM_ROLLER_STEP_TIM_FLAG_CCx                TIM_FLAG_CC4
#define PM_ROLLER_STEP_TIMx_IRQn                   TIM1_CC_IRQn
#define PM_ROLLER_STEP_TIMx_IRQHandler             TIM1_CC_IRQHandler

#define PM_ROLLER_STEP_TIM_CHANNEL_x               TIM_CHANNEL_4
#define PM_ROLLER_STEP_TIM_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOA_CLK_ENABLE()     // 输出控制脉冲给电机驱动器
#define PM_ROLLER_STEP_TIM_PUL_PORT                GPIOA                            // 对应驱动器的PUL-（驱动器使用共阳接法）
#define PM_ROLLER_STEP_TIM_PUL_PIN                 GPIO_PIN_11                       // 而PLU+直接接开发板的VCC
#define PM_ROLLER_STEP_GPIO_AFx_TIMx               GPIO_AF1_TIM1

#define PM_ROLLER_STEP_DIR_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOC_CLK_ENABLE()     // 电机旋转方向控制，如果悬空不接默认正转
#define PM_ROLLER_STEP_DIR_PORT                    GPIOC                            // 对应驱动器的DIR-（驱动器使用共阳接法）
#define PM_ROLLER_STEP_DIR_PIN                     GPIO_PIN_14                       // 而DIR+直接接开发板的VCC
#define PM_ROLLER_STEP_GPIO_PIN_AF_AS_SYS                    GxPIO_AF0_RTC_50Hz                // 引脚不作为复用功能使用

#define PM_ROLLER_STEP_ENA_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOC_CLK_ENABLE()     // 电机脱机使能控制，如果悬空不接默认使能电机
#define PM_ROLLER_STEP_ENA_PORT                    GPIOC                            // 对应驱动器的ENA-（驱动器使用共阳接法）
#define PM_ROLLER_STEP_ENA_PIN                     GPIO_PIN_13                       // 而ENA+直接接开发板的VCC

//
#define SM_ROLLER_STEP_TIMx                        TIM1
#define SM_STEP_TIM_RCC_CLK_ENABLE()        __HAL_RCC_TIM1_CLK_ENABLE()
#define SM_STEP_TIM_RCC_CLK_DISABLE()       __HAL_RCC_TIM1_CLK_DISABLE()
#define SM_STEP_TIM_IT_CCx                  TIM_IT_CC2
#define SM_STEP_TIM_FLAG_CCx                TIM_FLAG_CC2
#define SM_STEP_TIMx_IRQn                   TIM1_CC_IRQn
#define SM_STEP_TIMx_IRQHandler             TIM1_CC_IRQHandler

#define SM_TIM_CHANNEL_x               TIM_CHANNEL_2
#define SM_TIM_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOA_CLK_ENABLE()     // 输出控制脉冲给电机驱动器
#define SM_TIM_PUL_PORT                GPIOA                            // 对应驱动器的PUL-（驱动器使用共阳接法）
#define SM_TIM_PUL_PIN                 GPIO_PIN_9                       // 而PLU+直接接开发板的VCC
#define SM_GPIO_AFx_TIMx               GPIO_AF1_TIM1

#define SM_STEP_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOF_CLK_ENABLE()     // 电机旋转方向控制，如果悬空不接默认正转
#define SM_STEP_DIR_PORT                    GPIOF                            // 对应驱动器的DIR-（驱动器使用共阳接法）
#define SM_STEP_DIR_PIN                     GPIO_PIN_4                       // 而DIR+直接接开发板的VCC
#define SM_STEP_GPIO_PIN_AF_AS_SYS                    GPIO_AF0_RTC_50Hz                // 引脚不作为复用功能使用

#define SM_STEP_ENA_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOF_CLK_ENABLE()     // 电机脱机使能控制，如果悬空不接默认使能电机
#define SM_STEP_ENA_PORT                    GPIOF                            // 对应驱动器的ENA-（驱动器使用共阳接法）
#define SM_STEP_ENA_PIN                     GPIO_PIN_3                       // 而ENA+直接接开发板的VCC

#define CER_STEPMOTOR_DIR_FORWARD(Axis) \
HAL_GPIO_WritePin(Stepmotor[Axis].Dir_Port,Stepmotor[Axis].Dir_Pin,GPIO_PIN_RESET)//设置电机方向,参数Axis:当前活动轴
#define CER_STEPMOTOR_DIR_REVERSAL(Axis)\
HAL_GPIO_WritePin(Stepmotor[Axis].Dir_Port,Stepmotor[Axis].Dir_Pin,GPIO_PIN_SET)

#define CER_STEPMOTOR_OUTPUT_ENABLE(Axis)\
HAL_GPIO_WritePin(Stepmotor[Axis].Ena_Port,Stepmotor[Axis].Ena_Pin,GPIO_PIN_SET)//电机使能控制 参数Axis:当前活动轴
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
#define CUT_CHAMFER_STEP_TIM_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOE_CLK_ENABLE()     // 输出控制脉冲给电机驱动器
#define CUT_CHAMFER_STEP_TIM_PUL_PORT                GPIOE                            // 对应驱动器的PUL-（驱动器使用共阳接法）
#define CUT_CHAMFER_STEP_TIM_PUL_PIN                 GPIO_PIN_5                       // 而PLU+直接接开发板的VCC
#define CUT_CHAMFER_STEP_GPIO_AFx_TIMx               GPIO_AF3_TIM9

#define CUT_CHAMFER_STEP_ENA_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOG_CLK_ENABLE()     // 电机脱机使能控制，如果悬空不接默认使能电机
#define CUT_CHAMFER_STEP_ENA_PORT                    GPIOG                            // 对应驱动器的ENA-（驱动器使用共阳接法）
#define CUT_CHAMFER_STEP_ENA_PIN                     GPIO_PIN_5                       // 而ENA+直接接开发板的VCC

#define CUT_ROLLER_STEP_TIMx                        TIM9
#define CUT_ROLLER_STEP_TIM_RCC_CLK_ENABLE()        __HAL_RCC_TIM9_CLK_ENABLE()
#define CUT_ROLLER_STEP_TIM_RCC_CLK_DISABLE()       __HAL_RCC_TIM9_CLK_DISABLE()
#define CUT_ROLLER_STEP_TIM_IT_CCx                  TIM_IT_CC2
#define CUT_ROLLER_STEP_TIM_FLAG_CCx                TIM_FLAG_CC2
#define CUT_ROLLER_STEP_TIMx_IRQn                   TIM1_BRK_TIM9_IRQn
#define CUT_ROLLER_STEP_TIMx_IRQHandler             TIM1_BRK_TIM9_IRQHandler

#define CUT_ROLLER_STEP_TIM_CHANNEL_x               TIM_CHANNEL_2
#define CUT_ROLLER_STEP_TIM_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOE_CLK_ENABLE()     // 输出控制脉冲给电机驱动器
#define CUT_ROLLER_STEP_TIM_PUL_PORT                GPIOE                            // 对应驱动器的PUL-（驱动器使用共阳接法）
#define CUT_ROLLER_STEP_TIM_PUL_PIN                 GPIO_PIN_6                       // 而PLU+直接接开发板的VCC
#define CUT_ROLLER_STEP_GPIO_AFx_TIMx               GPIO_AF3_TIM9

#define CUT_ROLLER_STEP_ENA_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOG_CLK_ENABLE()     // 电机脱机使能控制，如果悬空不接默认使能电机
#define CUT_ROLLER_STEP_ENA_PORT                    GPIOG                            // 对应驱动器的ENA-（驱动器使用共阳接法）
#define CUT_ROLLER_STEP_ENA_PIN                     GPIO_PIN_6                       // 而ENA+直接接开发板的VCC

#define CER_CUT_STEPMOTOR_OUTPUT_ENABLE(Axis)\
HAL_GPIO_WritePin(CutStepmotor[Axis].Ena_Port,CutStepmotor[Axis].Ena_Pin,GPIO_PIN_SET)//电机使能控制 参数Axis:当前活动轴
#define CER_CUT_STEPMOTOR_OUTPUT_DISABLE(Axis)\
HAL_GPIO_WritePin(CutStepmotor[Axis].Ena_Port,CutStepmotor[Axis].Ena_Pin,GPIO_PIN_RESET)



// 定义定时器预分频，定时器实际时钟频率为：168MHz/（STEPMOTOR_TIMx_PRESCALER+1）
// 用于步进电机位置环控制需要将定时器频率降低,为了避免计数溢出
#define STEPMOTOR_TIM_PRESCALER               7  // 步进电机驱动器细分设置为：   32  细分

// 2020.3.28#define STEPMOTOR_TIM_PRESCALER               5  // 步进电机驱动器细分设置为：   32  细分

//#define STEPMOTOR_TIM_PRESCALER               15  // 步进电机驱动器细分设置为：   16  细分
//#define STEPMOTOR_TIM_PRESCALER               31  // 步进电机驱动器细分设置为：   8  细分
//#define STEPMOTOR_TIM_PRESCALER               63  // 步进电机驱动器细分设置为：   4  细分
//#define STEPMOTOR_TIM_PRESCALER               127  // 步进电机驱动器细分设置为：   2  细分
//#define STEPMOTOR_TIM_PRESCALER               255 // 步进电机驱动器细分设置为：   1  细分


// 定义定时器周期，输出比较模式周期设置为0xFFFF
#define STEPMOTOR_TIM_PERIOD                   0xFFFF
// 定义高级定时器重复计数寄存器值
#define STEPMOTOR_TIM_REPETITIONCOUNTER       0

//编码器及丝杠的一些参数
#define SECOND                                1000  // 常数:1s == 1000 ms
#define SAMPLING_PERIOD                       20    // 编码器采样周期 单位ms
#define M_PERIOD                              1000    // 编码器采样周期 单位ms

#define FREQ_UINT                             (T1_FREQ_TIM9/(SECOND/SAMPLING_PERIOD))    //对定时器的频率做单位换算,避免数值太大溢出

#define T1_FREQ_TIM9                          (SystemCoreClock/(279+1)) // 频率ft值
#define ENCODER_SPR              			        (500*4)     		// 编码器单圈线数*4倍频
#define MPR                         		      (78.5)       		    // 步进电机旋转一圈,丝杠的距离;单位：mm/r 
#define PPM      							                (ENCODER_SPR/MPR)  		// 每mm内编码器的脉冲数;单位:Pules/mm
#define P_PERIOD                              ((float)PPM/(SECOND/SAMPLING_PERIOD))  // 每个采样周期对应的脉冲数 
#define MPP                                   ((float)(MPR)/ENCODER_SPR) // 编码器单步步进距离

//// 步进电机及驱动器的一些参数
#define T1_FREQ                               (SystemCoreClock/(STEPMOTOR_TIM_PRESCALER+1)) // 定时器1频率F值
#define FSPR                                  200         //步进电机单圈步数
#define MICRO_STEP                            32          // 步进电机驱动器细分数
#define SPR                                   ((float)(FSPR*MICRO_STEP))   // 旋转一圈需要的脉冲数

#define FEEDBACK_CONST                        (SPR/ENCODER_SPR)   //编码器和步进电机驱动器的比值

#define CCW                                   1
#define CW                                    0


//梯形加减速
#define STOP                                  0 // 加减速曲线状态：停止
#define ACCEL                                 1 // 加减速曲线状态：加速阶段
#define DECEL                                 2 // 加减速曲线状态：减速阶段
#define RUN                                   3 // 加减速曲线状态：匀速阶段
#define T1_FREQ                               (SystemCoreClock/(STEPMOTOR_TIM_PRESCALER+1)) // 频率ft值
#define FSPR                                  200         //步进电机单圈步数
#define MICRO_STEP                            32          // 步进电机驱动器细分数
//#define SPR                                   (FSPR*MICRO_STEP)   // 旋转一圈需要的脉冲数

// 数学常数
#define ALPHA                                 ((float)(2*3.14159/SPR))       // α= 2*pi/spr
#define A_T_x10                               ((float)(10*ALPHA*T1_FREQ))
#define T1_FREQ_148                           ((float)((T1_FREQ*0.676)/10)) // 0.676为误差修正值
#define A_SQ                                  ((float)(2*100000*ALPHA)) 
#define A_x200                                ((float)(200*ALPHA))

#define   CUT_FLASM_STEP      0 
#define 	PLASTIC_STEP   		  1
#define 	FRICTION_STEP  		  2   //步进电机标号
#define 	ROLLER_STEP       	3
//TIM9
#define   CUT_CHAMFER_STEP	  0
#define   CUT_ROLLER_STEP		  1

/* 扩展变量 ------------------------------------------------------------------*/
extern TIM_HandleTypeDef htimx_STEPMOTOR;
extern TIM_HandleTypeDef cutMachine_STEPMOTOR; 
extern __IO uint16_t Toggle_Pulse;
extern __IO uint8_t  MotionStatus;           //是否在运动？0：停止，1：运动
extern bool stopCmd;
extern __IO uint32_t listStep_count[4];
extern bool listStopCmd[4]; // 纸盒停止命令
extern bool listCutStepStopCmd[2];
extern __IO uint8_t  cerMotionStatus[4];        // 是否在运动？0：停止，1：运动
extern __IO uint8_t  cerCutMotionStatus[2];
extern speedRampData cerStepSrd[4]; 
extern speedRampData cerCutStepSrd[2];
extern __IO uint32_t listCutStep_count[2];
/* 函数声明 ------------------------------------------------------------------*/
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
