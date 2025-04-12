#include "dwt.h"

/**
  * @brief  初始化DWT计数器
  * @param  无
  * @retval 无
  * @note   使用延时函数前，必须调用本函数
  */
void DWT_Init(void)
{
    /* 使能DWT外设 */
    DEMCR |= (uint32_t)DEMCR_TRCENA;
    
    /* DWT CYCCNT寄存器计数清0 */
    DWT_CYCCNT = (uint32_t)0U;      //使能CYCCNT寄存器之前，先清0
    
    /* 使能Cortex-M DWT CYCCNT寄存器 */
    DWT_CTRL  |=(uint32_t)DWT_CTRL_CYCCNTENA;
	
		Init_flag = 1;
}

/**
  * @brief  读取当前时间戳
  * @param  无
  * @retval 当前时间戳，即DWT_CYCCNT寄存器的值
  */
uint32_t DWT_GetTick(void)
{ 
		if(!Init_flag) DWT_Init();
    return ((uint32_t)DWT_CYCCNT);
}

/**
  * @brief  节拍数转化时间间隔(微妙单位)
  * @param  tick :需要转换的节拍数
  * @param  frequency :内核时钟频率
  * @retval 当前时间戳(微妙单位)
  */
uint32_t DWT_TickToMicrosecond(uint32_t tick,uint32_t frequency)
{ 
		if(!Init_flag) DWT_Init();
    return (uint32_t)(1000000.0/frequency*tick);
}

/**
  * @brief  DWT计数实现精确延时，32位计数器
  * @param  time : 延迟长度，单位1 us
  * @retval 无
  * @note   无
  */
void DWT_DelayUs(uint32_t time)
{
		if(!Init_flag) DWT_Init();
    /* 将微秒转化为对应的时钟计数值*/
    uint32_t tick_duration = time * (SystemCoreClock / 1000000) ;
    uint32_t tick_start = DWT_GetTick();         /* 刚进入时的计数器值 */
    
    while(DWT_GetTick() - tick_start < tick_duration);
}

/**
  * @brief  DWT计数实现精确延时，32位计数器
  * @param  time : 延迟长度，单位1 ms
  * @retval 无
  */
void DWT_DelayMs(uint32_t time)
{
		if(!Init_flag) DWT_Init();
    for(uint32_t i = 0; i < time; i++)
    {
        DWT_DelayUs(1000);
    }
}


/**
  * @brief  DWT计数实现精确延时，32位计数器
  * @param  time : 延迟长度，单位1 S
  * @retval 无
  */
void DWT_DelayS(uint32_t time)
{
		if(!Init_flag) DWT_Init();
    for(uint32_t i = 0; i < time; i++)
    {
        DWT_DelayMs(1000);
    }
}
