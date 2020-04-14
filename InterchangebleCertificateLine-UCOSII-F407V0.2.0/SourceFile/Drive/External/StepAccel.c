
/*-------------------------------<< Ä£¿é >>-----------------------------------*/
/*
 * - :CRJ-
 * - :201700721.001.00.1
 * - System->APP->Drive->CPU£¬×¢Òâ°üº¬Ë³Ðò
 * - 4ÀàÐÍº¯Êý£¬³õÊ¼»¯ÅäÖÃ£¬¿ØÖÆ£¬×´Ì¬£¬Ö´ÐÐ
 * - 
 * - :
 **/
/*___________________________________END______________________________________*/
#include "StepAccel.h"
#include <math.h>
#include <string.h>
#include "io.h"
#include "Resources.h"

speedRampData srdStepBox               = {STOP,CW,0,0,0,0,0};         // ¼Ó¼õËÙÇúÏß±äÁ¿
speedRampData srdStepMotor               = {STOP,CW,0,0,0,0,0};         // ¼Ó¼õËÙÇúÏß±äÁ¿

__IO int32_t  stepbox_position     = 0;           // µ±Ç°Î»ÖÃ
__IO int32_t  stepmotor_position     = 0;           // µ±Ç°Î»Ö
int32_t  stpdecel   = 200;       // ¼±Í£¼õËÙ¶È
uint32_t accel                  = 5;       // ¼ÓËÙ¶È
uint32_t decel                  = 5;       // ¼õËÙ¶È
int32_t  HomePos                = 0;           // ÁãµãÎ»ÖÃ
__IO uint8_t  stepBoxMotionStatus      = 0;           //Ö½ºÐÊÇ·ñÔÚÔË¶¯£¿0£ºÍ£Ö¹£¬1£ºÔË¶¯
__IO uint8_t  stepMotorMotionStatus      = 0;  				//¿ªÄ¤²½½ø
__IO uint16_t prescaler=1000;

bool boxStopCmd       = False; // Ö½ºÐÍ£Ö¹ÃüÁî
bool bLmtPos        = False; // ÕýÏÞÎ»µ±Ç°×´Ì¬
bool bLmtNeg        = False; // ¸ºÏÞÎ»µ±Ç°×´Ì¬
bool bStopCmd       = False; // Í£Ö¹ÃüÁî
bool bEmgStopping   = False; // ÊÇ·ñÅöÏÞÎ»¼±Í£
bool bEnableSoftLmt = False; // ÊÇ·ñÔÊÐíÈí¼þÏÞÎ»
bool bHomeOK        = False; // µ±Ç°ÊÇ·ñ´¦ÓÚÁãµã×´Ì¬
bool bZeroCapture   = False; // ÁãµãÑ°ÕÒ

int32_t  ZeroDir           = POSITIVE;	  //»ØÁã·½Ïò
int32_t  ZeroOffset        = 0;           //ÁãµãÆ«ÒÆ
uint32_t ZeroBackDistance  = 6400;        //»ØÁã»ØÍË¾àÀë 
uint8_t  ZeroStep          = FASTSEEK;	
uint32_t HomeFastSpeed     = 200;        //»ØÁã¿ìËÑËÙ¶È
uint32_t HomeSlowSpeed     = 100;         //»ØÁãÂýËÑËÙ¶È

uint8_t  ZeroStepMotor      = FORW;	
uint8_t  ZeroStepBoxMotor   = FORW;
/*--------------------------Sign---------------------------------*/
static void signalStabilityBox(Sign *Name,N8 (* fun)(void))
{
        N8 temp = fun();

		Name->Detect = 5;

        if(temp != Name->SIOB) {
                Name->DetectV	= 0;
                Name->SIOB		= temp;
        }
        if(Name->DetectV < Name->Detect) Name->DetectV ++;
        else {Name->DetectV = 255;Name->SIO = Name->SIOB;}
}

static void signalStabilityStep(Sign *Name,N8 (* fun)(void))
{
        N8 temp = fun();

		Name->Detect = 5;

        if(temp != Name->SIOB) {
                Name->DetectV	= 0;
                Name->SIOB		= temp;
        }
        if(Name->DetectV < Name->Detect) Name->DetectV ++;
        else {Name->DetectV = 255;Name->SIO = Name->SIOB;}
}

//¿ª·½
uint32_t STEP_MOTOR_BOX_sqrt(uint32_t x)
{
  register uint32_t xr;  // result register
  register uint32_t q2;  // scan-bit register
  register uint8_t f;   // flag (one bit)

  xr = 0;                     // clear result
  q2 = 0x40000000L;           // higest possible result bit
  do
  {
    if((xr + q2) <= x)
    {
      x -= xr + q2;
      f = 1;                  // set flag
    }
    else{
      f = 0;                  // clear flag
    }
    xr >>= 1;
    if(f){
      xr += q2;               // test flag
    }
  } while(q2 >>= 2);          // shift twice
  if(xr < x){
    return xr +1;             // add for rounding
  }
  else{
    return xr;
	  }
}

/*
 * TIMxCLK/CK_PSC --> TIMxCNT --> TIMx_ARR --> TIMxCNT ÖØÐÂ¼ÆÊý
 *                    TIMx_CCR(µçÆ½·¢Éú±ä»¯)
 * ÐÅºÅÖÜÆÚ=(TIMx_ARR +1 ) * Ê±ÖÓÖÜÆÚ
 * Õ¼¿Õ±È=TIMx_CCR/(TIMx_ARR +1)
 */
static void StepMotor_GPIO_Configuration(void)
{
	  GPIO_InitTypeDef GPIO_InitStructure;
  
	RCC_APB2PeriphClockCmd(STEP_MOTOR_BOX_PULSE_CLK|STEP_MOTOR_DIR_CLK
                         |RCC_APB2Periph_AFIO, ENABLE);

  GPIO_InitStructure.GPIO_Pin = STEP_MOTOR_BOX_PULSE_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(STEP_MOTOR_BOX_PULSEPORT, &GPIO_InitStructure);
	
  GPIO_InitStructure.GPIO_Pin = STEP_MOTOR_PULSE_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(STEP_MOTOR_BOX_PULSEPORT, &GPIO_InitStructure);
	
  GPIO_InitStructure.GPIO_Pin = STEP_MOTOR_BOX_DIR_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(STEP_MOTOR_BOX_DIRPORT, &GPIO_InitStructure);
  
	GPIO_InitStructure.GPIO_Pin = STEP_MOTOR_DIR_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(STEP_MOTOR_DIRPORT, &GPIO_InitStructure);
  
}

static void StepMotor_NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

  /* Enable the STEP_MOTOR_BOX_TIMx Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = STEP_MOTOR_BOX_TIMx_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = STEP_MOTOR_TIMx_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

void StepMotor_TIMx_Init(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_OCInitTypeDef  TIM_OCInitStructure;

  STEP_MOTOR_BOX_TIMX_CLOCKCMD(STEP_MOTOR_BOX_TIMx_RCC_CLK, ENABLE);
  STEP_MOTOR_TIMX_CLOCKCMD(STEP_MOTOR_TIMx_RCC_CLK,ENABLE);
  StepMotor_GPIO_Configuration();
  StepMotor_NVIC_Configuration();
 // StepMotor_EXTILine_Config();  
  
  TIM_TimeBaseStructure.TIM_Period =0xFFFF;
  TIM_TimeBaseStructure.TIM_Prescaler =35;
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
  TIM_TimeBaseInit(STEP_MOTOR_BOX_TIMx, &TIM_TimeBaseStructure);
  TIM_TimeBaseInit(STEP_MOTOR_TIMx, &TIM_TimeBaseStructure);

  /* Output Compare Active Mode configuration: Channel1 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Active;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 100;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
  STEP_MOTOR_BOX_TIMx_OCnInit(STEP_MOTOR_BOX_TIMx, &TIM_OCInitStructure);
	STEP_MOTOR_TIMx_OCnInit(STEP_MOTOR_TIMx, &TIM_OCInitStructure);
	
	TIM_ClearFlag(STEP_MOTOR_BOX_TIMx, TIM_FLAG_Update);
	TIM_ClearFlag(STEP_MOTOR_TIMx, TIM_FLAG_Update);
  /* TIM IT enable */
//  TIM_ITConfig(STEP_MOTOR_BOX_TIMx, TIM_IT_Update, ENABLE);
  TIM_ITConfig(STEP_MOTOR_BOX_TIMx, TIM_IT_CC3, ENABLE);
	TIM_ITConfig(STEP_MOTOR_TIMx, TIM_IT_CC2, ENABLE);
}

/**
  * º¯Êý¹¦ÄÜ: Ïà¶ÔÎ»ÖÃÔË¶¯£ºÔË¶¯¸ø¶¨µÄ²½Êý
  * ÊäÈë²ÎÊý: step£ºÒÆ¶¯µÄ²½Êý (ÕýÊýÎªË³Ê±Õë£¬¸ºÊýÎªÄæÊ±Õë).
              accel  ¼ÓËÙ¶È,Êµ¼ÊÖµÎªaccel*rad/sec^2
              decel  ¼õËÙ¶È,Êµ¼ÊÖµÎªdecel*rad/sec^2
              speed  ×î´óËÙ¶È,Êµ¼ÊÖµÎªspeed*rad/sec
  * ·µ »Ø Öµ: ÎÞ
  * Ëµ    Ã÷: ÒÔ¸ø¶¨µÄ²½ÊýÒÆ¶¯²½½øµç»ú£¬ÏÈ¼ÓËÙµ½×î´óËÙ¶È£¬È»ºóÔÚºÏÊÊÎ»ÖÃ¿ªÊ¼
  *           ¼õËÙÖÁÍ£Ö¹£¬Ê¹µÃÕû¸öÔË¶¯¾àÀëÎªÖ¸¶¨µÄ²½Êý¡£Èç¹û¼Ó¼õËÙ½×¶ÎºÜ¶Ì²¢ÇÒ
  *           ËÙ¶ÈºÜÂý£¬ÄÇ»¹Ã»´ïµ½×î´óËÙ¶È¾ÍÒª¿ªÊ¼¼õËÙ
  */

void StepMotorBoxAxisRel(int32_t step, uint32_t accel, uint32_t decel, uint32_t speed)
{  
  __IO uint16_t tim_count;
  // ´ïµ½×î´óËÙ¶ÈÊ±µÄ²½Êý
  __IO uint32_t max_s_lim;
  // ±ØÐëÒª¿ªÊ¼¼õËÙµÄ²½Êý£¨Èç¹û¼ÓËÙÃ»ÓÐ´ïµ½×î´óËÙ¶È£©
  __IO uint32_t accel_lim;
	float ftemp=0.0;

  if(step < 0) // ²½ÊýÎª¸ºÊý
  {
    srdStepBox.dir = CCW; // ÄæÊ±Õë·½ÏòÐý×ª
    STEP_MOTOR_BOX_SETDIR_CCW();
    step =-step;   // »ñÈ¡²½Êý¾ø¶ÔÖµ
  }
  else
  {
    srdStepBox.dir = CW; // Ë³Ê±Õë·½ÏòÐý×ª
    STEP_MOTOR_BOX_SETDIR_CW();
  }
  
  if(step == 1)
  {
    srdStepBox.accel_count = -1; // Move one step...
    srdStepBox.run_state = DECEL;// ...in DECEL state.
    srdStepBox.step_delay = 1000;	// Just a short delay so main() can act on 'running'.	   
    STEP_MOTOR_BOX_TIM_SetCompareN(STEP_MOTOR_BOX_TIMx,100);
    TIM_SetAutoreload(STEP_MOTOR_BOX_TIMx,100);
		stepBoxMotionStatus = 1;
		TIM_Cmd(STEP_MOTOR_BOX_TIMx, ENABLE);			 
  }
  else if(step != 0)  // Only move if number of steps to move is not zero.
  {
    // Refer to documentation for detailed information about these calculations.
    // Set max speed limit, by calc min_delay to use in timer.
    // min_delay = (alpha / tt)/ w
    srdStepBox.min_delay = T1_FREQ/speed/2;

    // Set accelration by calc the first (c0) step delay .
    // step_delay = 1/tt * STEP_MOTOR_BOX_sqrt(2*alpha/accel)
    // step_delay = ( tfreq*0.676/100 )*100 * STEP_MOTOR_BOX_sqrt( (2*alpha*10000000000) / (accel*100) )/10000
    srdStepBox.step_delay = ((long)T1_FREQ*0.676* STEP_MOTOR_BOX_sqrt(2000000 / accel))/1000/2;
    // Find out after how many steps does the speed hit the max speed limit.
    // max_s_lim = speed^2 / (2*alpha*accel)
    max_s_lim = speed*speed/(2*accel);
    // If we hit max speed limit before 0,5 step it will round to 0.
    // But in practice we need to move atleast 1 step to get any speed at all.
    if(max_s_lim == 0){
      max_s_lim = 1;
    }

    // Find out after how many steps we must start deceleration.
    // n1 = (n1+n2)decel / (accel + decel)
    if((accel+decel)>step)
		{
//			accel_lim = step*decel/(accel+decel);
			ftemp=(float)decel/(float)(accel+decel);
			accel_lim = (float)step*ftemp;
		}
		else
		{
			accel_lim = step/(accel+decel)*decel;
		}
    // We must accelrate at least 1 step before we can start deceleration.
    if(accel_lim == 0){
      accel_lim = 1;
    }

    // Use the limit we hit first to calc decel.
    if(accel_lim <= max_s_lim){
      srdStepBox.decel_val = accel_lim - step;
    }
    else{
      srdStepBox.decel_val =-(int32_t)(max_s_lim*accel/decel);
    }
    // We must decelrate at least 1 step to stop.
    if(srdStepBox.decel_val == 0){
      srdStepBox.decel_val = -1;
    }

    // Find step to start decleration.
    srdStepBox.decel_start = step + srdStepBox.decel_val;

    // If the maximum speed is so low that we dont need to go via accelration state.
    if(srdStepBox.step_delay <= srdStepBox.min_delay)
	 {
      srdStepBox.step_delay = srdStepBox.min_delay;
      srdStepBox.run_state = RUN;
    }
    else{
      srdStepBox.run_state = ACCEL;
    }

    // Reset counter.
    srdStepBox.accel_count = 0;
    stepBoxMotionStatus = 1;
    STEP_MOTOR_BOX_TIM_SetCompareN(STEP_MOTOR_BOX_TIMx,100);
    TIM_SetAutoreload(STEP_MOTOR_BOX_TIMx,100);	
    // Set Timer/Counter to divide clock by 8
	  TIM_Cmd(STEP_MOTOR_BOX_TIMx, ENABLE);
  }
}

//¿ªÄ¤µç»úÏà¶ÔÔË¶¯
void StepMotorAxisRel(int32_t step, uint32_t accel, uint32_t decel, uint32_t speed)
{  
  __IO uint16_t tim_count;
  // ´ïµ½×î´óËÙ¶ÈÊ±µÄ²½Êý
  __IO uint32_t max_s_lim;
  // ±ØÐëÒª¿ªÊ¼¼õËÙµÄ²½Êý£¨Èç¹û¼ÓËÙÃ»ÓÐ´ïµ½×î´óËÙ¶È£©
  __IO uint32_t accel_lim;
	float ftemp=0.0;

  if(step < 0) // ²½ÊýÎª¸ºÊý
  {
    srdStepMotor.dir = CCW; // ÄæÊ±Õë·½ÏòÐý×ª
    STEP_MOTOR_SETDIR_CCW();
    step =-step;   // »ñÈ¡²½Êý¾ø¶ÔÖµ
  }
  else
  {
    srdStepMotor.dir = CW; // Ë³Ê±Õë·½ÏòÐý×ª
    STEP_MOTOR_SETDIR_CW();
  }
  
  if(step == 1)
  {
    srdStepMotor.accel_count = -1; // Move one step...
    srdStepMotor.run_state = DECEL;// ...in DECEL state.
    srdStepMotor.step_delay = 1000;	// Just a short delay so main() can act on 'running'.	   
    STEP_MOTOR_TIM_SetCompareN(STEP_MOTOR_TIMx,100);
    TIM_SetAutoreload(STEP_MOTOR_TIMx,100);
		stepMotorMotionStatus = 1;
		TIM_Cmd(STEP_MOTOR_TIMx, ENABLE);			 
  }
  else if(step != 0)  // Only move if number of steps to move is not zero.
  {
    // Refer to documentation for detailed information about these calculations.
    // Set max speed limit, by calc min_delay to use in timer.
    // min_delay = (alpha / tt)/ w
    srdStepMotor.min_delay = T1_FREQ/speed/2;

    // Set accelration by calc the first (c0) step delay .
    // step_delay = 1/tt * STEP_MOTOR_BOX_sqrt(2*alpha/accel)
    // step_delay = ( tfreq*0.676/100 )*100 * STEP_MOTOR_BOX_sqrt( (2*alpha*10000000000) / (accel*100) )/10000
    srdStepMotor.step_delay = ((long)T1_FREQ*0.676* STEP_MOTOR_BOX_sqrt(2000000 / accel))/1000/2;
    // Find out after how many steps does the speed hit the max speed limit.
    // max_s_lim = speed^2 / (2*alpha*accel)
    max_s_lim = speed*speed/(2*accel);
    // If we hit max speed limit before 0,5 step it will round to 0.
    // But in practice we need to move atleast 1 step to get any speed at all.
    if(max_s_lim == 0){
      max_s_lim = 1;
    }

    // Find out after how many steps we must start deceleration.
    // n1 = (n1+n2)decel / (accel + decel)
    if((accel+decel)>step)
		{
//			accel_lim = step*decel/(accel+decel);
			ftemp=(float)decel/(float)(accel+decel);
			accel_lim = (float)step*ftemp;
		}
		else
		{
			accel_lim = step/(accel+decel)*decel;
		}
    // We must accelrate at least 1 step before we can start deceleration.
    if(accel_lim == 0){
      accel_lim = 1;
    }

    // Use the limit we hit first to calc decel.
    if(accel_lim <= max_s_lim){
      srdStepMotor.decel_val = accel_lim - step;
    }
    else{
      srdStepMotor.decel_val =-(int32_t)(max_s_lim*accel/decel);
    }
    // We must decelrate at least 1 step to stop.
    if(srdStepMotor.decel_val == 0){
      srdStepMotor.decel_val = -1;
    }

    // Find step to start decleration.
    srdStepMotor.decel_start = step + srdStepMotor.decel_val;

    // If the maximum speed is so low that we dont need to go via accelration state.
    if(srdStepMotor.step_delay <= srdStepMotor.min_delay)
	 {
      srdStepMotor.step_delay = srdStepMotor.min_delay;
      srdStepMotor.run_state = RUN;
    }
    else{
      srdStepMotor.run_state = ACCEL;
    }

    // Reset counter.
    srdStepMotor.accel_count = 0;
    stepMotorMotionStatus = 1;
    STEP_MOTOR_TIM_SetCompareN(STEP_MOTOR_TIMx,100);
    TIM_SetAutoreload(STEP_MOTOR_TIMx,100);	
    // Set Timer/Counter to divide clock by 8
	  TIM_Cmd(STEP_MOTOR_TIMx, ENABLE);
  }
}

/*! \brief Move the stepper motor a given number of steps.
 *
 *  Makes the stepper motor move the given number of steps.
 *  It accelrate with given accelration up to maximum speed and decelerate
 *  with given deceleration so it stops at the given step.
 *  If accel/decel is to small and steps to move is to few, speed might not
 *  reach the max speed limit before deceleration starts.
 *
 *  \param step  Number of steps to move (pos - CW, neg - CCW).
 *  \param accel  Accelration to use, in 0.01*rad/sec^2. µ¥Î» 0.01»¹ÊÇ1 rad/sec^2
 *  \param decel  Decelration to use, in 0.01*rad/sec^2.
 *  \param speed  Max speed, in 0.01*rad/sec.
 */
 /*signed int range:-2147483648---21474 83647
 **unsiged int range:0---4294967295u
 */

void StepMotorBoxAxisAbs(int32_t step, uint32_t accel, uint32_t decel, uint32_t speed)
{  
  __IO uint16_t tim_count;
  // ´ïµ½×î´óËÙ¶ÈÊ±µÄ²½Êý
  __IO uint32_t max_s_lim;
  // ±ØÐëÒª¿ªÊ¼¼õËÙµÄ²½Êý£¨Èç¹û¼ÓËÙÃ»ÓÐ´ïµ½×î´óËÙ¶È£©
  __IO uint32_t accel_lim;
	float ftemp=0.0;
	step = step - stepbox_position; //¾ø¶ÔÎ»ÖÃ
	
  if(step < 0) // ²½ÊýÎª¸ºÊý
  {
    srdStepBox.dir = CCW; // ÄæÊ±Õë·½ÏòÐý×ª
    STEP_MOTOR_BOX_SETDIR_CCW();
    step =-step;   // »ñÈ¡²½Êý¾ø¶ÔÖµ
  }
  else
  {
    srdStepBox.dir = CW; // Ë³Ê±Õë·½ÏòÐý×ª
    STEP_MOTOR_BOX_SETDIR_CW();
  }
  
  if(step == 1)
  {
    srdStepBox.accel_count = -1; // Move one step...
    srdStepBox.run_state = DECEL;// ...in DECEL state.
    srdStepBox.step_delay = 1000;	// Just a short delay so main() can act on 'running'.	   
    STEP_MOTOR_BOX_TIM_SetCompareN(STEP_MOTOR_BOX_TIMx,100);
    TIM_SetAutoreload(STEP_MOTOR_BOX_TIMx,100);
		stepBoxMotionStatus = 1;
		TIM_Cmd(STEP_MOTOR_BOX_TIMx, ENABLE);			 
  }
  else if(step != 0)  // Only move if number of steps to move is not zero.
  {
    // Refer to documentation for detailed information about these calculations.
    // Set max speed limit, by calc min_delay to use in timer.
    // min_delay = (alpha / tt)/ w
    srdStepBox.min_delay = T1_FREQ/speed/2;

    // Set accelration by calc the first (c0) step delay .
    // step_delay = 1/tt * STEP_MOTOR_BOX_sqrt(2*alpha/accel)
    // step_delay = ( tfreq*0.676/100 )*100 * STEP_MOTOR_BOX_sqrt( (2*alpha*10000000000) / (accel*100) )/10000
    srdStepBox.step_delay = ((long)T1_FREQ*0.676* STEP_MOTOR_BOX_sqrt(2000000 / accel))/1000/2;
    // Find out after how many steps does the speed hit the max speed limit.
    // max_s_lim = speed^2 / (2*alpha*accel)
    max_s_lim = speed*speed/(2*accel);
    // If we hit max speed limit before 0,5 step it will round to 0.
    // But in practice we need to move atleast 1 step to get any speed at all.
    if(max_s_lim == 0){
      max_s_lim = 1;
    }

    // Find out after how many steps we must start deceleration.
    // n1 = (n1+n2)decel / (accel + decel)
    if((accel+decel)>step)
		{
//			accel_lim = step*decel/(accel+decel);
			ftemp=(float)decel/(float)(accel+decel);
			accel_lim = (float)step*ftemp;
		}
		else
		{
			accel_lim = step/(accel+decel)*decel;
		}
    // We must accelrate at least 1 step before we can start deceleration.
    if(accel_lim == 0){
      accel_lim = 1;
    }

    // Use the limit we hit first to calc decel.
    if(accel_lim <= max_s_lim){
      srdStepBox.decel_val = accel_lim - step;
    }
    else{
      srdStepBox.decel_val =-(int32_t)(max_s_lim*accel/decel);
    }
    // We must decelrate at least 1 step to stop.
    if(srdStepBox.decel_val == 0){
      srdStepBox.decel_val = -1;
    }

    // Find step to start decleration.
    srdStepBox.decel_start = step + srdStepBox.decel_val;

    // If the maximum speed is so low that we dont need to go via accelration state.
    if(srdStepBox.step_delay <= srdStepBox.min_delay)
	 {
      srdStepBox.step_delay = srdStepBox.min_delay;
      srdStepBox.run_state = RUN;
    }
    else{
      srdStepBox.run_state = ACCEL;
    }

    // Reset counter.
    srdStepBox.accel_count = 0;
    stepBoxMotionStatus = 1;
    STEP_MOTOR_BOX_TIM_SetCompareN(STEP_MOTOR_BOX_TIMx,100);
    TIM_SetAutoreload(STEP_MOTOR_BOX_TIMx,100);	
    // Set Timer/Counter to divide clock by 8
	  TIM_Cmd(STEP_MOTOR_BOX_TIMx, ENABLE);
  }
}



//¾ø¶ÔÔË¶¯
void StepMotorAxisAbs(int32_t step, uint32_t accel, uint32_t decel, uint32_t speed)
{  
  __IO uint16_t tim_count;
  // ´ïµ½×î´óËÙ¶ÈÊ±µÄ²½Êý
  __IO uint32_t max_s_lim;
  // ±ØÐëÒª¿ªÊ¼¼õËÙµÄ²½Êý£¨Èç¹û¼ÓËÙÃ»ÓÐ´ïµ½×î´óËÙ¶È£©
  __IO uint32_t accel_lim;
	float ftemp=0.0;
	step = step - stepmotor_position; //¾ø¶ÔÎ»ÖÃ

  if(step < 0) // ²½ÊýÎª¸ºÊý
  {
    srdStepMotor.dir = CCW; // ÄæÊ±Õë·½ÏòÐý×ª
    STEP_MOTOR_SETDIR_CCW();
    step =  -step;   // »ñÈ¡²½Êý¾ø¶ÔÖµ
  }
  else
  {
    srdStepMotor.dir = CW; // Ë³Ê±Õë·½ÏòÐý×ª
    STEP_MOTOR_SETDIR_CW();
  }
  
  if(step == 1)
  {
    srdStepMotor.accel_count = -1; // Move one step...
    srdStepMotor.run_state = DECEL;// ...in DECEL state.
    srdStepMotor.step_delay = 1000;	// Just a short delay so main() can act on 'running'.	   
    STEP_MOTOR_TIM_SetCompareN(STEP_MOTOR_TIMx,100);
    TIM_SetAutoreload(STEP_MOTOR_TIMx,100);
		stepMotorMotionStatus = 1;
		TIM_Cmd(STEP_MOTOR_TIMx, ENABLE);			 
  }
  else if(step != 0)  // Only move if number of steps to move is not zero.
  {
    // Refer to documentation for detailed information about these calculations.
    // Set max speed limit, by calc min_delay to use in timer.
    // min_delay = (alpha / tt)/ w
    srdStepMotor.min_delay = T1_FREQ/speed/2;

    // Set accelration by calc the first (c0) step delay .
    // step_delay = 1/tt * STEP_MOTOR_BOX_sqrt(2*alpha/accel)
    // step_delay = ( tfreq*0.676/100 )*100 * STEP_MOTOR_BOX_sqrt( (2*alpha*10000000000) / (accel*100) )/10000
    srdStepMotor.step_delay = ((long)T1_FREQ*0.676* STEP_MOTOR_BOX_sqrt(2000000 / accel))/1000/2;
    // Find out after how many steps does the speed hit the max speed limit.
    // max_s_lim = speed^2 / (2*alpha*accel)
    max_s_lim = speed*speed/(2*accel);
    // If we hit max speed limit before 0,5 step it will round to 0.
    // But in practice we need to move atleast 1 step to get any speed at all.
    if(max_s_lim == 0){
      max_s_lim = 1;
    }

    // Find out after how many steps we must start deceleration.
    // n1 = (n1+n2)decel / (accel + decel)
    if((accel+decel)>step)
		{
//			accel_lim = step*decel/(accel+decel);
			ftemp=(float)decel/(float)(accel+decel);
			accel_lim = (float)step*ftemp;
		}
		else
		{
			accel_lim = step/(accel+decel)*decel;
		}
    // We must accelrate at least 1 step before we can start deceleration.
    if(accel_lim == 0){
      accel_lim = 1;
    }

    // Use the limit we hit first to calc decel.
    if(accel_lim <= max_s_lim){
      srdStepMotor.decel_val = accel_lim - step;
    }
    else{
      srdStepMotor.decel_val =-(int32_t)(max_s_lim*accel/decel);
    }
    // We must decelrate at least 1 step to stop.
    if(srdStepMotor.decel_val == 0){
      srdStepMotor.decel_val = -1;
    }

    // Find step to start decleration.
    srdStepMotor.decel_start = step + srdStepMotor.decel_val;

    // If the maximum speed is so low that we dont need to go via accelration state.
    if(srdStepMotor.step_delay <= srdStepMotor.min_delay)
	 {
      srdStepMotor.step_delay = srdStepMotor.min_delay;
      srdStepMotor.run_state = RUN;
    }
    else{
      srdStepMotor.run_state = ACCEL;
    }

    // Reset counter.
    srdStepMotor.accel_count = 0;
    stepMotorMotionStatus = 1;
    STEP_MOTOR_TIM_SetCompareN(STEP_MOTOR_TIMx,100);
    TIM_SetAutoreload(STEP_MOTOR_TIMx,100);	
    // Set Timer/Counter to divide clock by 8
	  TIM_Cmd(STEP_MOTOR_TIMx, ENABLE);
  }
}

/*--------------------------------µç»ú¸´Î»»ØÁã----------------------------------------*/
void StepMotorBoxReset(void)
{
	switch(ZeroStep)
	{
		case IDEL:
			break;
				case FASTSEEK:
			if(!stepBoxMotionStatus)   //ÊÇ·ñÔÚÔË¶¯£¿0£ºÍ£Ö¹£¬1£ºÔË¶¯
			{
          ZeroStep=FASTSEEKSTOP;
					StepMotorBoxAxisRel(3600, 50, 50, 100);
			}
			break;
		case FASTSEEKSTOP:
			if(stepBoxMotionStatus){
				ZeroStep=FASTSEEKBACK;
			}
			break;
		case FASTSEEKBACK:
			if(!stepBoxMotionStatus)   //ÊÇ·ñÔÚÔË¶¯£¿0£ºÍ£Ö¹£¬1£ºÔË¶¯
			{

          ZeroStep=SLOWSEEK;
          //StepMotorAxisRel(-3600, 5, 5, 20);

			}
			break;
			
	}
}
void controlResetStepMotor(void)
{
 ZeroStepMotor = FORW;
}
void controlStepMotorBack(void)
{
	ZeroStepMotor = BACK;
}
void controlResetBoxStepMotor(void)
{
	ZeroStepBoxMotor = FORW;
}
void controlBoxStepMotorBack(void)
{
	ZeroStepBoxMotor = BACK;
}	

/*--------------------------------ÖÐ¶Ï´¦Àíº¯Êý----------------------------------------*/
void StepMotorBox_TIMx_IRQHandler(void)
{
  // Holds next delay period.
  uint16_t new_step_delay;
  // Remember the last step delay used when accelrating.
  static uint16_t last_accel_delay;
  // Counting steps when moving.
  static uint32_t step_count = 0;
  // Keep track of remainder from new_step-delay calculation to incrase accurancy
  static int32_t rest = 0;
  static uint8_t i=0;

  if (TIM_GetITStatus(STEP_MOTOR_BOX_TIMx, TIM_IT_CC3) != RESET)
  {
    /* Clear STEP_MOTOR_BOX_TIMx Capture Compare1 interrupt pending bit*/
    TIM_ClearITPendingBit(STEP_MOTOR_BOX_TIMx, TIM_IT_CC3);
    TIM_ClearFlag(STEP_MOTOR_BOX_TIMx,TIM_FLAG_CC3);

  STEP_MOTOR_BOX_TIM_SetCompareN(STEP_MOTOR_BOX_TIMx,srdStepBox.step_delay);
  TIM_SetAutoreload(STEP_MOTOR_BOX_TIMx,srdStepBox.step_delay);

  if(srdStepBox.run_state)
  {
	  if(GPIO_ReadOutputDataBit(STEP_MOTOR_BOX_PULSEPORT, STEP_MOTOR_BOX_PULSE_1))
	  {
		  GPIO_ResetBits(STEP_MOTOR_BOX_PULSEPORT, STEP_MOTOR_BOX_PULSE_1);
		}
	  else
	  {
			GPIO_SetBits(STEP_MOTOR_BOX_PULSEPORT, STEP_MOTOR_BOX_PULSE_1);
	  }
	}
	  i++;
  if(i==2){
	  i=0;
		switch(srdStepBox.run_state) 
    {
      case STOP:
				step_count = 0;
				rest = 0;
				TIM_Cmd(STEP_MOTOR_BOX_TIMx, DISABLE);	//Ê¹ÓÃÁ½¸ö²½½øµÄ»°²»ÄÜÔÚÕâ¸ö¹Ø		
			  stepBoxMotionStatus = 0;
				break;
			case ACCEL:
				step_count++;      // ²½Êý¼Ó1
				if(srdStepBox.dir==CW)
				{
				stepbox_position++; // ¾ø¶ÔÎ»ÖÃ¼Ó1
				}
				else
				{
				stepbox_position--; // ¾ø¶ÔÎ»ÖÃ¼õ1
				}
				srdStepBox.accel_count++; // ¼ÓËÙ¼ÆÊýÖµ¼Ó1
				new_step_delay = srdStepBox.step_delay - (((2 *srdStepBox.step_delay) + rest)/(4 * srdStepBox.accel_count + 1));//¼ÆËãÐÂ(ÏÂ)Ò»²½Âö³åÖÜÆÚ(Ê±¼ä¼ä¸ô)
				rest = ((2 * srdStepBox.step_delay)+rest)%(4 * srdStepBox.accel_count + 1);// ¼ÆËãÓàÊý£¬ÏÂ´Î¼ÆËã²¹ÉÏÓàÊý£¬¼õÉÙÎó²î
				
				if(step_count >= srdStepBox.decel_start || boxStopCmd)// ¼ì²éÊÇ¹»Ó¦¸Ã¿ªÊ¼¼õËÙ
				{
					if(boxStopCmd){
						 boxStopCmd=False;
						srdStepBox.accel_count = T1_FREQ/2/stpdecel*T1_FREQ/srdStepBox.step_delay/srdStepBox.step_delay*-1;
					}else{
						srdStepBox.accel_count = srdStepBox.decel_val; // ¼ÓËÙ¼ÆÊýÖµÎª¼õËÙ½×¶Î¼ÆÊýÖµµÄ³õÊ¼Öµ
					}
				srdStepBox.run_state = DECEL;           // ÏÂ¸öÂö³å½øÈë¼õËÙ½×¶Î
				}
				else if(new_step_delay <= srdStepBox.min_delay) // ¼ì²éÊÇ·ñµ½´ïÆÚÍûµÄ×î´óËÙ¶È
				{
				last_accel_delay = new_step_delay; // ±£´æ¼ÓËÙ¹ý³ÌÖÐ×îºóÒ»´ÎÑÓÊ±£¨Âö³åÖÜÆÚ£©
				new_step_delay = srdStepBox.min_delay;    // Ê¹ÓÃmin_delay£¨¶ÔÓ¦×î´óËÙ¶Èspeed£©
				rest = 0;                          // ÇåÁãÓàÖµ
				srdStepBox.run_state = RUN;               // ÉèÖÃÎªÔÈËÙÔËÐÐ×´Ì¬
				}
				break;
			case RUN:
				step_count++;  // ²½Êý¼Ó1
				if(srdStepBox.dir==CW)
				{	  	
				stepbox_position++; // ¾ø¶ÔÎ»ÖÃ¼Ó1
				}
				else
				{
				stepbox_position--; // ¾ø¶ÔÎ»ÖÃ¼õ1
				}
				new_step_delay = srdStepBox.min_delay;     // Ê¹ÓÃmin_delay£¨¶ÔÓ¦×î´óËÙ¶Èspeed£©
				if(step_count >= srdStepBox.decel_start || boxStopCmd)   // ÐèÒª¿ªÊ¼¼õËÙ
				{
					if(boxStopCmd){
						boxStopCmd = False;
						srdStepBox.accel_count = T1_FREQ/2/stpdecel*T1_FREQ/srdStepBox.step_delay/srdStepBox.step_delay*-1;
					}else{
						srdStepBox.accel_count = srdStepBox.decel_val;  // ¼õËÙ²½Êý×öÎª¼ÓËÙ¼ÆÊýÖµ
						}
				new_step_delay = last_accel_delay;// ¼Ó½×¶Î×îºóµÄÑÓÊ±×öÎª¼õËÙ½×¶ÎµÄÆðÊ¼ÑÓÊ±(Âö³åÖÜÆÚ)
				srdStepBox.run_state = DECEL;            // ×´Ì¬¸Ä±äÎª¼õËÙ
			 }
				break;
			case DECEL:
				step_count++;  // ²½Êý¼Ó1
				if(srdStepBox.dir==CW)
				{	  	
				stepbox_position++; // ¾ø¶ÔÎ»ÖÃ¼Ó1
				}
				else
				{
				stepbox_position--; // ¾ø¶ÔÎ»ÖÃ¼õ1
				}
				srdStepBox.accel_count++;
				new_step_delay = srdStepBox.step_delay - (((2 * srdStepBox.step_delay) + rest)/(4 * srdStepBox.accel_count + 1)); //¼ÆËãÐÂ(ÏÂ)Ò»²½Âö³åÖÜÆÚ(Ê±¼ä¼ä¸ô)
				rest = ((2 * srdStepBox.step_delay)+rest)%(4 * srdStepBox.accel_count + 1);// ¼ÆËãÓàÊý£¬ÏÂ´Î¼ÆËã²¹ÉÏÓàÊý£¬¼õÉÙÎó²î
				if(boxStopCmd){
				 boxStopCmd=False;
				srdStepBox.accel_count = T1_FREQ/2/stpdecel*T1_FREQ/srdStepBox.step_delay/srdStepBox.step_delay*-1;
				}
				//¼ì²éÊÇ·ñÎª×îºóÒ»²½
				if(srdStepBox.accel_count >= 0)
				{
				srdStepBox.run_state = STOP;
				}
				break;
		}
	srdStepBox.step_delay = new_step_delay; // ÎªÏÂ¸ö(ÐÂµÄ)ÑÓÊ±(Âö³åÖÜÆÚ)¸³Öµ				
	 }
	}
}

void StepMotor_TIMx_IRQHandler(void)
{
  // Holds next delay period.
  uint16_t new_step_delay;
  // Remember the last step delay used when accelrating.
  static uint16_t last_accel_delay;
  // Counting steps when moving.
  static uint32_t step_count = 0;
  // Keep track of remainder from new_step-delay calculation to incrase accurancy
  static int32_t rest = 0;
  static uint8_t i=0;	
  if (TIM_GetITStatus(STEP_MOTOR_TIMx, TIM_IT_CC2) != RESET)
  {
    /* Clear STEP_MOTOR_BOX_TIMx Capture Compare1 interrupt pending bit*/
    TIM_ClearITPendingBit(STEP_MOTOR_TIMx, TIM_IT_CC2);
    TIM_ClearFlag(STEP_MOTOR_TIMx,TIM_FLAG_CC2);

  STEP_MOTOR_TIM_SetCompareN(STEP_MOTOR_TIMx,srdStepMotor.step_delay);
  TIM_SetAutoreload(STEP_MOTOR_TIMx,srdStepMotor.step_delay);
 if(srdStepMotor.run_state)
  {
	  if(GPIO_ReadOutputDataBit(STEP_MOTOR_PULSEPORT, STEP_MOTOR_PULSE_1))
	  {
		  GPIO_ResetBits(STEP_MOTOR_PULSEPORT, STEP_MOTOR_PULSE_1);
		}
	  else
	  {
			GPIO_SetBits(STEP_MOTOR_PULSEPORT, STEP_MOTOR_PULSE_1);
	  }
	}
	  i++;
  if(i==2){
	  i=0;
		switch(srdStepMotor.run_state) 
    {
      case STOP:
				step_count = 0;
				rest = 0;
				TIM_Cmd(STEP_MOTOR_TIMx, DISABLE);	//Ê¹ÓÃÁ½¸ö²½½øµÄ»°²»ÄÜÔÚÕâ¸ö¹Ø		
			  stepMotorMotionStatus = 0;
				break;
			case ACCEL:
				step_count++;      // ²½Êý¼Ó1
				if(srdStepMotor.dir==CW)
				{	  	
				stepmotor_position++; // ¾ø¶ÔÎ»ÖÃ¼Ó1
				}
				else
				{
				stepmotor_position--; // ¾ø¶ÔÎ»ÖÃ¼õ1
				}
				srdStepMotor.accel_count++; // ¼ÓËÙ¼ÆÊýÖµ¼Ó1
				new_step_delay = srdStepMotor.step_delay - (((2 *srdStepMotor.step_delay) + rest)/(4 * srdStepMotor.accel_count + 1));//¼ÆËãÐÂ(ÏÂ)Ò»²½Âö³åÖÜÆÚ(Ê±¼ä¼ä¸ô)
				rest = ((2 * srdStepMotor.step_delay)+rest)%(4 * srdStepMotor.accel_count + 1);// ¼ÆËãÓàÊý£¬ÏÂ´Î¼ÆËã²¹ÉÏÓàÊý£¬¼õÉÙÎó²î
				if(step_count >= srdStepMotor.decel_start)// ¼ì²éÊÇ¹»Ó¦¸Ã¿ªÊ¼¼õËÙ
				{
				srdStepMotor.accel_count = srdStepMotor.decel_val; // ¼ÓËÙ¼ÆÊýÖµÎª¼õËÙ½×¶Î¼ÆÊýÖµµÄ³õÊ¼Öµ
				srdStepMotor.run_state = DECEL;           // ÏÂ¸öÂö³å½øÈë¼õËÙ½×¶Î
				}
				else if(new_step_delay <= srdStepMotor.min_delay) // ¼ì²éÊÇ·ñµ½´ïÆÚÍûµÄ×î´óËÙ¶È
				{
				last_accel_delay = new_step_delay; // ±£´æ¼ÓËÙ¹ý³ÌÖÐ×îºóÒ»´ÎÑÓÊ±£¨Âö³åÖÜÆÚ£©
				new_step_delay = srdStepMotor.min_delay;    // Ê¹ÓÃmin_delay£¨¶ÔÓ¦×î´óËÙ¶Èspeed£©
				rest = 0;                          // ÇåÁãÓàÖµ
				srdStepMotor.run_state = RUN;               // ÉèÖÃÎªÔÈËÙÔËÐÐ×´Ì¬
				}
				break;
			case RUN:
				step_count++;  // ²½Êý¼Ó1
				if(srdStepMotor.dir==CW)
				{	  	
				stepmotor_position++; // ¾ø¶ÔÎ»ÖÃ¼Ó1
				}
				else
				{
				stepmotor_position--; // ¾ø¶ÔÎ»ÖÃ¼õ1
				}
				new_step_delay = srdStepMotor.min_delay;     // Ê¹ÓÃmin_delay£¨¶ÔÓ¦×î´óËÙ¶Èspeed£©
				if(step_count >= srdStepMotor.decel_start)   // ÐèÒª¿ªÊ¼¼õËÙ
				{
				srdStepMotor.accel_count = srdStepMotor.decel_val;  // ¼õËÙ²½Êý×öÎª¼ÓËÙ¼ÆÊýÖµ
				new_step_delay = last_accel_delay;// ¼Ó½×¶Î×îºóµÄÑÓÊ±×öÎª¼õËÙ½×¶ÎµÄÆðÊ¼ÑÓÊ±(Âö³åÖÜÆÚ)
				srdStepMotor.run_state = DECEL;            // ×´Ì¬¸Ä±äÎª¼õËÙ
				}
				break;
			case DECEL:
				step_count++;  // ²½Êý¼Ó1
				if(srdStepMotor.dir==CW)
				{	  	
				stepmotor_position++; // ¾ø¶ÔÎ»ÖÃ¼Ó1
				}
				else
				{
				stepmotor_position--; // ¾ø¶ÔÎ»ÖÃ¼õ1
				}
				srdStepMotor.accel_count++;
				new_step_delay = srdStepMotor.step_delay - (((2 * srdStepMotor.step_delay) + rest)/(4 * srdStepMotor.accel_count + 1)); //¼ÆËãÐÂ(ÏÂ)Ò»²½Âö³åÖÜÆÚ(Ê±¼ä¼ä¸ô)
				rest = ((2 * srdStepMotor.step_delay)+rest)%(4 * srdStepMotor.accel_count + 1);// ¼ÆËãÓàÊý£¬ÏÂ´Î¼ÆËã²¹ÉÏÓàÊý£¬¼õÉÙÎó²î

				//¼ì²éÊÇ·ñÎª×îºóÒ»²½
				if(srdStepMotor.accel_count >= 0)
				{
				srdStepMotor.run_state = STOP;
				}
				break;
		}
			srdStepMotor.step_delay = new_step_delay; // ÎªÏÂ¸ö(ÐÂµÄ)ÑÓÊ±(Âö³åÖÜÆÚ)¸³Öµ				
	 }
	}
}
