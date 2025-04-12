// 使用说明：
// 1、默认使用 TIM2 作为ADC的触发源。
// 2、默认配置NVIC，启用ADC中断响应函数。
// 3、默认配置并启用DMA1进行数据运输。
// 4、默认开启ADC的6分频 72MHz / 6 = 12MHz。
// 5、默认ADC采样周期使用 ADC_SampleTime_55Cycles5，兼容速度与精度。
// 6、STM32F103C8T6 核心板的ADC2不支持独立DMA传输，故在此板上所有ADC2对应代码不可用。
// 7、电压计数公式 ： Voltage = ADCx_Value * (3.3f / 4095)。
#ifndef __ADC_H
#define __ADC_H

#define ADC_NUM 10

#include "stm32f10x.h"
#include "gpio.h"
#include "usart.h"
#include "systick.h"

static GPIO_InitTypeDef ADC_gpio_initstruct = {0};

typedef struct 
{	
	uint32_t RCC_APB2Periph;
	GPIO_TypeDef* GPIOx;
	uint16_t GPIO_Pin;
	uint8_t exist;
}ADC_TASKS;

static uint8_t CLOCK_INIT_FLAG = 0;

static uint8_t DMA1_COMPLEATE = 0;
static uint8_t DMA2_COMPLEATE = 0;

static uint8_t CURRENT_ADC1_TASKS_NUM = 0;
static uint8_t CURRENT_ADC2_TASKS_NUM = 0;

static ADC_TASKS adc1_tasks[ADC_NUM] = {0};
static ADC_TASKS adc2_tasks[ADC_NUM] = {0};

volatile static uint16_t adc1_values[ADC_NUM] = {0}; // DMA存储ADC原始值
volatile static uint16_t adc2_values[ADC_NUM] = {0}; // DMA存储ADC原始值
volatile static uint8_t ADC1_TASK_INDEX = 0;
volatile static uint8_t ADC2_TASK_INDEX = 0;

volatile static uint8_t ADC1_CHANNEL_BASE_ADD = 0;
volatile static uint8_t ADC2_CHANNEL_BASE_ADD = 0;

void CLOCKS_INIT(void);

void ADC1_TASK_INIT(uint32_t RCC_APB2Periph, GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void ADC2_TASK_INIT(uint32_t RCC_APB2Periph, GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);

void  ADC1_INIT(void);
void  ADC2_INIT(void);

void ADC1_RUN(void);
void ADC2_RUN(void);

void TIM2_INIT(void);
void NVIC_ACD1_INIT(void);
void NVIC_ACD2_INIT(void);
void DMA_ADC1_INIT(void);
void DMA_ADC2_INIT(void);

uint8_t Get_DMA_Flag(uint8_t x);
void Reset_DMA_Flag(uint8_t x);

void print_adc1_data(void);
void print_adc2_data(void);

uint16_t Get_ADC1_DATA(uint8_t index);
uint16_t Get_ADC1_DATA(uint8_t index);

#endif //__ADC_H
