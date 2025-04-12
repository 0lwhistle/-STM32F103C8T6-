#ifndef __ASYNC_TASK_H
#define __ASYNC_TASK_H

#include "stm32f10x.h"
#include "systick.h"
#include "usart.h"
#include "stdarg.h"
#define MAX_TASK_NUM 10
#define TASK_NUM MAX_TASK_NUM

typedef void (*operation)(void);

typedef struct
{
	operation op;
	
	uint8_t exist;
	
	uint8_t flag;
	
	uint32_t circle;
	
	uint32_t timer;
	
//	va_list args;
//	
//	int arg_count;
//	
//	char* arg_types;
}ASYNC_TASKS;

static ASYNC_TASKS async_processes[TASK_NUM];
static ErrorStatus Error_List[TASK_NUM];
static uint8_t Current_Task_Num = 0;
static uint8_t RUNABLE_FLAG = 1;

ErrorStatus init_processes(int32_t time, operation op);
void run_processes(void);
void systick_task(void);
//void parse_args(ASYNC_TASKS* task);

#endif //__ASYNC_TASK_H
