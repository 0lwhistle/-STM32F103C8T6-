#ifndef __SYSTICK_H
#define __SYSTICK_H

#include "stm32f10x.h"

static uint64_t systick_count = 0;//用于时钟计数
static uint64_t  MY_SYSTICK_BEGIN = 0;//用于记录开始计数时的systick_count
static uint64_t MY_SYSTICK_END = 0;//用于记录结束计数时的systick_count
//只要导入该头，默认这三个静态变量。

typedef enum Delay_Unit
{
	MS = 1000,
	US = 1000000,
}DELAY_UNIT;
//使用枚举，可选择计量时间的单位大小。

static uint8_t systick_init_flag = 0;
void SysTick_Init(DELAY_UNIT D);
//初始化时钟，输入参数计量时间单位大小。

void SysTick_CountPlus(void);
//放在systick_handel中，每次时钟中断时，让systick_count加一。

uint64_t SysTick_GetCount(void);
//返回当前状态的systick_count值。

void My_SysTick_Delay(uint64_t time);
//自动计数延迟，延迟时间的单位由初始化时确定，该函数的输入参数为时间大小。

uint64_t Get_Begin(void);
//给MY_SYSTICK_BEGIN赋值当前的systick_count，并返回。
uint64_t Get_End(void);
//给MY_SYSTICK_END赋值当前的systick_count，并返回。
uint64_t Get_During(void);
//返回当前 MY_SYSTICK_END - MY_SYSTICK_BEGIN 的差值。

#endif //__SYSTICK_H
