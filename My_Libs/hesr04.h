#ifndef __HESR04_H
#define __HESR04_H
#include "stm32f10x.h"
#include "dwt.h"
#include "dhtll.h"
#include "usart.h"

// 定时器的预分频
#define GENERAL_TIM_PRESCALER               71

// 定时器的周期
#define GENERAL_TIM_PERIOD                  0xFFFF

#define GENERAL_TIMx                        TIM2
#define GENERAL_TIM_APBxClock_FUN           RCC_APB1PeriphClockCmd
#define GENERAL_TIM_CLK                     RCC_APB1Periph_TIM2
#define GENERAL_TIM_CHANNELx                TIM_Channel_2
#define GENERAL_TIM_IT_CCx                  TIM_IT_CC2


#define GENERAL_TIM_IRQn                    TIM2_IRQn
#define GENERAL_TIM_IRQHANDLER              TIM2_IRQHandler

#define ECHO_RCC_GPIO_CLK                   RCC_APB2Periph_GPIOB
#define ECHO_GPIO_PIN                       GPIO_Pin_3
#define ECHO_GPIO_PORT                      GPIOB


#define TRIG_RCC_GPIO_CLK                   RCC_APB2Periph_GPIOA
#define TRIG_GPIO_PIN                       GPIO_Pin_15
#define TRIG_GPIO_PORT                      GPIOA

// 获取捕获寄存器的值的函数的宏定义
#define            GENERAL_TIM_GetCapturex_FUN                 TIM_GetCapture2
// 捕获信号极性配置函数的宏
#define            GENERAL_TIM_OCxPolarityConfig_FUN           TIM_OC2PolarityConfig

// 测量起始边沿
#define            GENERAL_TIM_STRAT_ICPolarity        TIM_ICPolarity_Rising 
// 测量结束边沿
#define            GENERAL_TIM_END_ICPolarity          TIM_ICPolarity_Falling 

typedef struct              
{   
	uint8_t   ucFinishFlag;   // 捕获结束标志
	uint8_t   ucStartFlag;    // 捕获开始标志
	uint16_t  usCtr;          // 捕获寄存器的值
	uint16_t  usPeriod;       // 自动重装寄存器的更新标志 
}STRUCT_CAPTURE; 

//----------------------------------------------------------------
extern STRUCT_CAPTURE TIM_ICUserValueStructure;
static DHTll_DATA_TYPEDEF dhtll_data_hesr04 = {0};//超声波模块测温专用
//----------------------------------------------------------------

static uint8_t GET_TEMP_FLAG = 0;
static ErrorStatus temp_status;

void CS100A_TRIG(void);
//超声波模块的测量触发函数。

void CS100A_Init(void);
//超声波模块初始化。

void TIM_GPIO_Config(void);
//ECHO引脚的初始化。

void TIM_NVIC_Config(void);
//配置定时器中断。

void TRIG_GPIO_Config(void);
//TRIG引脚的初始化。

void TIM_Mode_Config(void);
//定时器的配置。

uint32_t Get_time(void);
float Get_distance(uint32_t time);

void print_distance(void);

#endif // __HESR04_H
