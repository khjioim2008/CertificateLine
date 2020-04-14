#ifndef __BSP_KEY_H__
#define __BSP_KEY_H__

/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "stdint.h"
#include "string.h"

/* �궨�� --------------------------------------------------------------------*/
#define KEY1_RCC_CLK_ENABLE           __HAL_RCC_GPIOF_CLK_ENABLE
#define KEY1_GPIO_PIN                 GPIO_PIN_10
#define KEY1_GPIO                     GPIOF
#define KEY1_DOWN_LEVEL               0  /* ����ԭ��ͼ��ƣ�KEY1����ʱ����Ϊ�ߵ�ƽ��������������Ϊ1 */

#define KEY2_RCC_CLK_ENABLE           __HAL_RCC_GPIOF_CLK_ENABLE
#define KEY2_GPIO_PIN                 GPIO_PIN_9
#define KEY2_GPIO                     GPIOF
#define KEY2_DOWN_LEVEL               0  /* ����ԭ��ͼ��ƣ�KEY2����ʱ����Ϊ�͵�ƽ��������������Ϊ0 */

#define KEY3_RCC_CLK_ENABLE           __HAL_RCC_GPIOE_CLK_ENABLE
#define KEY3_GPIO_PIN                 GPIO_PIN_10
#define KEY3_GPIO                     GPIOE
#define KEY3_DOWN_LEVEL               0  /* ����ԭ��ͼ��ƣ�KEY2����ʱ����Ϊ�͵�ƽ��������������Ϊ0 */

#define KEY4_RCC_CLK_ENABLE           __HAL_RCC_GPIOE_CLK_ENABLE
#define KEY4_GPIO_PIN                 GPIO_PIN_8
#define KEY4_GPIO                     GPIOE
#define KEY4_DOWN_LEVEL               0  /* ����ԭ��ͼ��ƣ�KEY2����ʱ����Ϊ�͵�ƽ��������������Ϊ0 */


#define LED_ONE_OUT_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOD_CLK_ENABLE()
#define LED_ONE_OUT_PIN                     GPIO_PIN_0
#define LED_ONE_PORT												 GPIOD

#define LED_TWO_OUT_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOD_CLK_ENABLE()
#define LED_TWO_OUT_PIN                     GPIO_PIN_1
#define LED_TWO_PORT												 GPIOD




//According to your need to modify the constants.
#define TICKS_INTERVAL    5	//ms
#define DEBOUNCE_TICKS    4	//MAX 8
#define SHORT_TICKS       (300 /TICKS_INTERVAL)
#define LONG_TICKS        (1000 /TICKS_INTERVAL)


typedef void (*BtnCallback)(void*);

typedef enum {
	PRESS_DOWN = 0,  //���£�ÿ�ΰ��¶�����
	PRESS_UP,				//��������ÿ���ɿ�������
	PRESS_REPEAT,		//�ظ����³�����repeat������������
	SINGLE_CLICK,     //����
	DOUBLE_CLICK,			//˫��
	LONG_RRESS_START, //������ʼ����һ��
	LONG_PRESS_HOLD, //����һֱ����
	number_of_event,
	NONE_PRESS
}PressEvent;

typedef struct Button {
	uint16_t ticks;
	uint8_t  repeat : 4;
	uint8_t  event : 4;
	uint8_t  state : 3;
	uint8_t  debounce_cnt : 3; 
	uint8_t  active_level : 1;
	uint8_t  button_level : 1;
	uint8_t  (*hal_button_Level)(void);
	BtnCallback  cb[number_of_event];
	struct Button* next;
}Button;

void button_init(struct Button* handle, uint8_t(*pin_level)(void), uint8_t active_level);
void button_attach(struct Button* handle, PressEvent event, BtnCallback cb);
PressEvent get_button_event(struct Button* handle);
int  button_start(struct Button* handle);
void button_stop(struct Button* handle);
void button_ticks(void);


void KEY_GPIO_Init(void);
uint8_t readCutButtonIO(void); 
uint8_t readStampButtonIO(void); 
uint8_t readWorkFinishButtonIO(void);
uint8_t readPaperExitButtonIO(void); 

#endif

