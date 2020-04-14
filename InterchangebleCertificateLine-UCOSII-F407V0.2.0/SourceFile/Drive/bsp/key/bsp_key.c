
/* 包含头文件 ----------------------------------------------------------------*/
#include "key/bsp_key.h"
#include "includes.h"

#define EVENT_CB(ev)   if(handle->cb[ev])handle->cb[ev]((Button*)handle)
	
//button handle list head.
static struct Button* head_handle = NULL;


/**
  * 函数功能: 板载按键IO引脚初始化.
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：使用宏定义方法代替具体引脚号，方便程序移植，只要简单修改bsp_key.h
  *           文件相关宏定义就可以方便修改引脚。
  */
void KEY_GPIO_Init(void)
{
   /* 定义IO硬件初始化结构体变量 */
  GPIO_InitTypeDef GPIO_InitStruct;
	
	/* 使能(开启)KEY引脚对应IO端口时钟 */  
  KEY1_RCC_CLK_ENABLE(); //F
	KEY3_RCC_CLK_ENABLE(); //E
  LED_ONE_OUT_GPIO_CLK_ENABLE();
	
  /* 配置KEY1 GPIO:输入上拉模式 */
  GPIO_InitStruct.Pin = KEY1_GPIO_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Alternate = GPIO_AF0_TRACE;       // GPIO引脚用做系统默认功能
  HAL_GPIO_Init(KEY1_GPIO, &GPIO_InitStruct);  
  
  /* 配置KEY2 GPIO:输入上拉模式 */
  GPIO_InitStruct.Pin = KEY2_GPIO_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Alternate = GPIO_AF0_TRACE;       // GPIO引脚用做系统默认功能
  HAL_GPIO_Init(KEY2_GPIO, &GPIO_InitStruct);

  /* 配置KEY3 GPIO:输入上拉模式 */
  GPIO_InitStruct.Pin = KEY3_GPIO_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Alternate = GPIO_AF0_TRACE;       // GPIO引脚用做系统默认功能
  HAL_GPIO_Init(KEY3_GPIO, &GPIO_InitStruct);

  /* 配置KEY4 GPIO:输入上拉模式 */
  GPIO_InitStruct.Pin = KEY4_GPIO_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Alternate = GPIO_AF0_TRACE;       // GPIO引脚用做系统默认功能
  HAL_GPIO_Init(KEY4_GPIO, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = LED_ONE_OUT_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Alternate = GPIO_AF0_TRACE;       // GPIO引脚用做系统默认功能
  HAL_GPIO_Init(LED_ONE_PORT, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = LED_TWO_OUT_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Alternate = GPIO_AF0_TRACE;       // GPIO引脚用做系统默认功能
  HAL_GPIO_Init(LED_TWO_PORT, &GPIO_InitStruct);

}

uint8_t readCutButtonIO(void) 
{
	return !!(KEY1_GPIO->IDR & KEY1_GPIO_PIN);
}

uint8_t readStampButtonIO(void) 
{
	return !!(KEY2_GPIO->IDR & KEY2_GPIO_PIN);
}
uint8_t readWorkFinishButtonIO(void) 
{
	return !!(KEY3_GPIO->IDR & KEY3_GPIO_PIN);
}

uint8_t readPaperExitButtonIO(void) 
{
	return !!(KEY4_GPIO->IDR & KEY4_GPIO_PIN);
}

/**
  * @brief  Initializes the button struct handle.
  * @param  handle: the button handle strcut.
  * @param  pin_level: read the HAL GPIO of the connet button level.
  * @param  active_level: pressed GPIO level.
  * @retval None                                                                       
  */
void button_init(struct Button* handle, uint8_t(*pin_level)(void), uint8_t active_level)
{
	memset(handle, 0, sizeof(struct Button));
	handle->event = (uint8_t)NONE_PRESS;
	handle->hal_button_Level = pin_level;
	handle->button_level = handle->hal_button_Level();
	handle->active_level = active_level;
}

/**
  * @brief  Attach the button event callback function.
  * @param  handle: the button handle strcut.
  * @param  event: trigger event type.
  * @param  cb: callback function.
  * @retval None
  */
void button_attach(struct Button* handle, PressEvent event, BtnCallback cb)
{
	handle->cb[event] = cb;
}

/**
  * @brief  Inquire the button event happen.
  * @param  handle: the button handle strcut.
  * @retval button event.
  */
PressEvent get_button_event(struct Button* handle)
{
	return (PressEvent)(handle->event);
}

/**
  * @brief  Button driver core function, driver state machine.
  * @param  handle: the button handle strcut.
  * @retval None
  */
void button_handler(struct Button* handle)
{
	uint8_t read_gpio_level = handle->hal_button_Level();

	//ticks counter working..
	if((handle->state) > 0) handle->ticks++;

	/*------------button debounce handle---------------*/
	if(read_gpio_level != handle->button_level) { //not equal to prev one
		//continue read 3 times same new level change
		if(++(handle->debounce_cnt) >= DEBOUNCE_TICKS) {
			handle->button_level = read_gpio_level;
			handle->debounce_cnt = 0;
		}
	} else { //leved not change ,counter reset.
		handle->debounce_cnt = 0;
	}

	/*-----------------State machine-------------------*/
	switch (handle->state) {
	case 0:
		if(handle->button_level == handle->active_level) {	//start press down
			handle->event = (uint8_t)PRESS_DOWN;
			EVENT_CB(PRESS_DOWN);
			handle->ticks = 0;
			handle->repeat = 1;
			handle->state = 1;
		} else {
			handle->event = (uint8_t)NONE_PRESS;
		}
		break;

	case 1:
		if(handle->button_level != handle->active_level) { //released press up
			handle->event = (uint8_t)PRESS_UP;
			EVENT_CB(PRESS_UP);
			handle->ticks = 0;
			handle->state = 2;

		} else if(handle->ticks > LONG_TICKS) {
			handle->event = (uint8_t)LONG_RRESS_START;
			EVENT_CB(LONG_RRESS_START);
			handle->state = 5;
		}
		break;

	case 2:
		if(handle->button_level == handle->active_level) { //press down again
			handle->event = (uint8_t)PRESS_DOWN;
			EVENT_CB(PRESS_DOWN);
			handle->repeat++;			
			EVENT_CB(PRESS_REPEAT); // repeat hit
			handle->ticks = 0;
			handle->state = 3;
		} else if(handle->ticks > SHORT_TICKS) { //released timeout
			if(handle->repeat == 1) {
				handle->event = (uint8_t)SINGLE_CLICK;
				EVENT_CB(SINGLE_CLICK);
			} else if(handle->repeat == 2) {
				handle->event = (uint8_t)DOUBLE_CLICK;
				EVENT_CB(DOUBLE_CLICK); // repeat hit
			}
			handle->state = 0;
		}
		break;

	case 3:
		if(handle->button_level != handle->active_level) { //released press up
			handle->event = (uint8_t)PRESS_UP;
			EVENT_CB(PRESS_UP);
			if(handle->ticks < SHORT_TICKS) {
				handle->ticks = 0;
				handle->state = 2; //repeat press
			} else {
				handle->state = 0;
			}
		}
		break;

	case 5:
		if(handle->button_level == handle->active_level) {
			//continue hold trigger
			handle->event = (uint8_t)LONG_PRESS_HOLD;
			EVENT_CB(LONG_PRESS_HOLD);

		} else { //releasd
			handle->event = (uint8_t)PRESS_UP;
			EVENT_CB(PRESS_UP);
			handle->state = 0; //reset
		}
		break;
	}
}

/**
  * @brief  Start the button work, add the handle into work list.
  * @param  handle: target handle strcut.
  * @retval 0: succeed. -1: already exist.
  */
int button_start(struct Button* handle)
{
	struct Button* target = head_handle;
	while(target) {
		if(target == handle) return -1;	//already exist.
		target = target->next;
	}
	handle->next = head_handle;
	head_handle = handle;
	return 0;
}

/**
  * @brief  Stop the button work, remove the handle off work list.
  * @param  handle: target handle strcut.
  * @retval None
  */
void button_stop(struct Button* handle)
{
	struct Button** curr;
	for(curr = &head_handle; *curr; ) {
		struct Button* entry = *curr;
		if (entry == handle) {
			*curr = entry->next;
//			free(entry);
		} else
			curr = &entry->next;
	}
}

/**
  * @brief  background ticks, timer repeat invoking interval 5ms.
  * @param  None.
  * @retval None
  */
void button_ticks(void)
{
	struct Button* target;
	for(target=head_handle; target; target=target->next) {
		button_handler(target);
	}
}

