#ifndef __BSP_DWT_H
#define __BSP_DWT_H

#include "stm32f10x.h"

/*
 在Cortex-M里面有一个外设叫DWT(Data Watchpoint and Trace)，该外设有一个32位的寄存器叫CYCCNT，它是一个向上的计数器，记录的是内核时钟运行的个数，
 假设内核频率为72M，内核跳一次的时间大概为1/72M=14ns（最长能记录的时间为：60s=2 的32 次方/72000000）。当CYCCNT溢出之后，会清0重新开始向上计数。
 使能CYCCNT计数的操作步骤：
 
 1、先使能DWT外设准备，这个由另外内核调试寄存器DEMCR的位24控制，写1使能
 2、使能CYCCNT寄存器之前，先清0
 3、使能CYCCNT计数器，这个由DWT_CTRL(代码上宏定义为DWT_CR)的位0控制，写1使能
 4、CYCCNT和us互相转换：1/SystemCoreClock*CYCCNT（S）  = 1000/SystemCoreClock*CYCCNT（MS）= 1000000/SystemCoreClock*CYCCNT（US） = (X)US

*/

/* DWT时间戳相关寄存器定义 */

#define DEMCR           *(uint32_t *)(0xE000EDFC)
#define DWT_CTRL        *(uint32_t *)(0xE0001000)
#define DWT_CYCCNT      *(uint32_t *)(0xE0001004)

#define DEMCR_TRCENA            (1<<24)
#define DWT_CTRL_CYCCNTENA      (1<<0)

static uint8_t Init_flag = 0;
void DWT_Init(void);
//dwt初始化

uint32_t DWT_GetTick(void);
//返回当前的dwt寄存器中DWT_CYCCNT的计数值。

uint32_t DWT_TickToMicrosecond(uint32_t tick,uint32_t frequency);
//将当前的tick计数值转化为时间计数值。输入参数tick，当前内核频率。

void DWT_DelayUs(uint32_t time);
//延时函数，输入参数为延迟时间大小。

void DWT_DelayMs(uint32_t time);
//延时函数，输入参数为延迟时间大小。

void DWT_DelayS(uint32_t time);
//延时函数，输入参数为延迟时间大小。

#endif /* __BSP_DWT_H  */
