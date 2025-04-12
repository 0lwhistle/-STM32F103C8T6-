#ifndef __KEY_H
#define __KEY_H

#include "stm32f10x.h"
#include "systick.h"
#include "usart.h"

#define KEY_1_GPIO_PORT 						GPIOA
#define KEY_1_GPIO_RCC_PORT 				RCC_APB2Periph_GPIOA
#define KEY_1_PIN 									GPIO_Pin_0

#define KEY_2_GPIO_PORT 						GPIOC
#define KEY_2_GPIO_RCC_PORT 				RCC_APB2Periph_GPIOC
#define KEY_2_PIN 									GPIO_Pin_13

#define KEY_3_GPIO_PORT 						GPIOB
#define KEY_3_GPIO_RCC_PORT 				RCC_APB2Periph_GPIOB
#define KEY_3_PIN 									GPIO_Pin_15

typedef enum
{
	KEY_1 = 0,
	KEY_2,
	KEY_3,
}KEYx;

typedef enum 
{
	KEY_UP = 0, 
	KEY_DOWN = 1,
}KEY_Status;

typedef enum 
{
	KEY_LOW_TRIGGER = 0, 
	KEY_HIGH_TRIGGER = 1,
}KEY_TriggerLevel;

typedef enum 
{
	synchronous = 0,
	asynchronous,
}Key_Sacn_status;

static GPIO_InitTypeDef GPIO_InitStruct = {0};

static uint8_t KEY1_INIT_FLAG = 0;
static uint8_t KEY2_INIT_FLAG = 0;
static uint8_t KEY3_INIT_FLAG = 0;


void key_init(KEYx keyx);
KEY_Status key_scan(KEYx keyx, KEY_TriggerLevel key_pressstatus, Key_Sacn_status key_scan_status);

#endif // __KEY_H
