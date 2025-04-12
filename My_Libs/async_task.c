#include "async_task.h"

//void parse_args(ASYNC_TASKS* task)
//{
//    va_list args;
//    va_copy(args, task->args);
//    
//    for (int i = 0; i < task->arg_count; i++) {
//        switch (task->arg_types[i]) {
//            case 'i': {
//                int val = va_arg(args, int);
//                // 处理int类型参数（如传递给任务函数）
//                break;
//            }
//            case 'f': {
//                float val = va_arg(args, double); // float会提升为double[6](@ref)
//                break;
//            }
//            // 支持更多类型...
//        }
//    }
//    va_end(args);
//}


ErrorStatus init_processes(int32_t time, operation op)
{
	if(!systick_init_flag) SysTick_Init(MS); //如无初始化，默认初始化毫秒模式。
	for(uint8_t i = 0; i < TASK_NUM; ++i)
	{
		if(!async_processes[i].exist)
		{
			async_processes[i].op = op;
			async_processes[i].circle = time;
			async_processes[i].flag = 0;
			async_processes[i].timer = async_processes[i].circle;
			async_processes[i].exist = 1;
			Error_List[i] = SUCCESS;
			++Current_Task_Num;
			
//			va_list args;
//			va_start(args, arg_types);
//			va_copy(async_processes[i].args, args);
//			async_processes[i].arg_count = arg_count;
//			async_processes[i].arg_types = arg_types;
//			va_end(args);
			
			return SUCCESS;
		}
	} 
		
	return ERROR;
}

void run_processes()
{
	if(RUNABLE_FLAG)
	{
		for(uint8_t i = 0; i < Current_Task_Num; ++i)
		{
			if(!Error_List[i])
			{
				printf("第%d个多线程任务初始化失败！\n", i);
				RUNABLE_FLAG = 0;
				return;
			}
		}
		
		for(uint8_t i = 0; i < TASK_NUM; ++i)
		{
			if(async_processes[i].exist && async_processes[i].flag)
			{
				//parse_args(&async_processes[i]);
				async_processes[i].op();
				async_processes[i].timer = async_processes[i].circle;
				async_processes[i].flag = 0;
			}
		}
	}
}

void systick_task()
{
	for(uint8_t i = 0; i < TASK_NUM; ++i)
	{
		if(async_processes[i].exist)
		{
			if(async_processes[i].timer > 0x00)
			{
			async_processes[i].timer--;
			if(async_processes[i].timer == 0) async_processes[i].flag = 1;
			}
		}
	}
}
