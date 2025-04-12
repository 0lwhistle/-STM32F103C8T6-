#include "systick.h"

void SysTick_Init(DELAY_UNIT D)
{
    uint32_t reloadValue;
		
		switch(D)
		{
        case MS:
            reloadValue = SystemCoreClock / 1000; // 1毫秒中断一次
						systick_init_flag = 1;
            break;
        case US:
            reloadValue = SystemCoreClock / 1000000; // 1微秒中断一次
						systick_init_flag = 1;
            break;
        default:
            // 处理未知单位或错误值
            while(1); // 出错时进入死循环
		}
		
		 if (SysTick_Config(reloadValue))
    {
        while(1); // 初始化失败后进入死循环
    }
    
}

void SysTick_close()
{
	if(!systick_init_flag) SysTick_Init(MS); //如无初始化，默认初始化毫秒模式。
	SysTick->CTRL  &= ~SysTick_CTRL_ENABLE_Msk;
}

void SysTick_CountPlus(void)
{
	++systick_count;
}

uint64_t SysTick_GetCount(void)
{
	return systick_count; 
}

void My_SysTick_Delay(uint64_t time){
	if(!systick_init_flag) SysTick_Init(MS); //如无初始化，默认初始化毫秒模式。
	//SystemCoreClock 默认等于72mMHz，即每72000000次操作。
	//SystemCoreClock / 1000 = 72000， 计时器每次操作减去1直到为0重置，总共需要1s/1000 = 1ms
	//同理 SystemCoreClock / 10000 = 7200 为1微妙， SystemCoreClock / 1 = 72000000 为1秒。
	uint64_t tick_start = SysTick_GetCount();
	
	while(SysTick_GetCount() - tick_start < time);
}

uint64_t Get_Begin(void)
{
	if(!systick_init_flag) SysTick_Init(MS); //如无初始化，默认初始化毫秒模式。
	MY_SYSTICK_BEGIN = SysTick_GetCount();
	return MY_SYSTICK_BEGIN;
}
uint64_t Get_End(void)
{
	if(!systick_init_flag) SysTick_Init(MS); //如无初始化，默认初始化毫秒模式。
	MY_SYSTICK_END = SysTick_GetCount();
	return MY_SYSTICK_END;
}
uint64_t Get_During(void)
{
	if(!systick_init_flag) SysTick_Init(MS); //如无初始化，默认初始化毫秒模式。
	return MY_SYSTICK_END - MY_SYSTICK_BEGIN;
}

