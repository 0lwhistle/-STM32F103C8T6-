#include "adc.h"

void CLOCKS_INIT(void)
{
 // 使能必要时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_ADC2, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1 | RCC_AHBPeriph_DMA2, ENABLE);
	RCC_ADCCLKConfig(RCC_PCLK2_Div6); // ADC时钟72MHz/6=12MHz
	CLOCK_INIT_FLAG = 1;
}

void ADC1_TASK_INIT(uint32_t RCC_APB2Periph, GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
	if(!CLOCK_INIT_FLAG) CLOCKS_INIT();
	for(uint8_t i = 0; i < ADC_NUM; ++i)
	{
		if(!adc1_tasks[i].exist)
		{
			adc1_tasks[i].exist = 1;
			adc1_tasks[i].GPIOx = GPIOx;
			adc1_tasks[i].GPIO_Pin = GPIO_Pin;
			adc1_tasks[i].RCC_APB2Periph = RCC_APB2Periph;
			
			RCC_APB2PeriphClockCmd(RCC_APB2Periph, ENABLE);
			
			ADC_gpio_initstruct.GPIO_Pin = GPIO_Pin;
			ADC_gpio_initstruct.GPIO_Mode = GPIO_Mode_AIN;
			
			++CURRENT_ADC1_TASKS_NUM;
			break;
		}
	}
}

void ADC2_TASK_INIT(uint32_t RCC_APB2Periph, GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
	if(!CLOCK_INIT_FLAG) CLOCKS_INIT();
	for(uint8_t i = 0; i < ADC_NUM; ++i)
	{
		if(!adc2_tasks[i].exist)
		{
			adc2_tasks[i].exist = 1;
			adc2_tasks[i].GPIOx = GPIOx;
			adc2_tasks[i].GPIO_Pin = GPIO_Pin;
			adc2_tasks[i].RCC_APB2Periph = RCC_APB2Periph;
			
			RCC_APB2PeriphClockCmd(RCC_APB2Periph, ENABLE);
			
			ADC_gpio_initstruct.GPIO_Pin = GPIO_Pin;
			ADC_gpio_initstruct.GPIO_Mode = GPIO_Mode_AIN;
			++CURRENT_ADC2_TASKS_NUM;
			break;
		}
	}
}

uint8_t add_to_index_func(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
	switch(GPIO_Pin)
		{
			case(GPIO_Pin_0):
				if(GPIOx == GPIOA)
					return 0;
				else 
					return 8;
			
			case(GPIO_Pin_1):
				if(GPIOx == GPIOA)
					return 1;
				else 
					return 9;
			
			case(GPIO_Pin_2):
				return 2;
			
			case(GPIO_Pin_3):
				return 3;
			
			case(GPIO_Pin_4):
				return 4;
			
			case(GPIO_Pin_5):
				return 5;
			
			case(GPIO_Pin_6):
				return 6;
			
			case(GPIO_Pin_7):
				return 7;
		}
		
		while(1);// 未知错误，进入死循环方便Debug
		
}

void  ADC1_INIT(void)
{
	ADC_InitTypeDef ADC_InitStructure;

	// ADC1配置
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE; // 扫描模式
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE; // 单次转换模式（由定时器触发）
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T2_CC2; // 默认TIM2_CC2触发
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = CURRENT_ADC1_TASKS_NUM;
	ADC_Init(ADC1, &ADC_InitStructure);

//	// 配置规则通道
//	ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 1, ADC_SampleTime_55Cycles5); // PA4
//	ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 2, ADC_SampleTime_55Cycles5); // PA5
	for(uint8_t i = 0; i < ADC_NUM; ++i)
	{
		if(adc1_tasks[i].exist)
			{
				ADC1_TASK_INDEX = add_to_index_func(adc1_tasks[i].GPIOx, adc1_tasks[i].GPIO_Pin);
				ADC_RegularChannelConfig(ADC1, ADC1_CHANNEL_BASE_ADD + ADC1_TASK_INDEX, i + 1, ADC_SampleTime_55Cycles5);
			}
	}


	DMA_ADC1_INIT();
	TIM2_INIT();
	NVIC_ACD1_INIT();
	ADC1_RUN();
	My_SysTick_Delay(100);// 延迟100ms等待第一波采样结果处理，防止过早启动。
}

void  ADC2_INIT(void)
{
	ADC_InitTypeDef ADC_InitStructure;

	// ADC2配置
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE; // 扫描模式
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE; // 单次转换模式（由定时器触发）
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T2_CC2; // 默认TIM2_CC2触发
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = CURRENT_ADC2_TASKS_NUM;
	ADC_Init(ADC2, &ADC_InitStructure);

//	// 配置规则通道
//	ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 1, ADC_SampleTime_55Cycles5); // PA4
//	ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 2, ADC_SampleTime_55Cycles5); // PA5
	for(uint8_t i = 0; i < ADC_NUM; ++i)
	{
		if(adc1_tasks[i].exist)
			{
				ADC2_TASK_INDEX = add_to_index_func(adc2_tasks[i].GPIOx, adc2_tasks[i].GPIO_Pin);
				ADC_RegularChannelConfig(ADC2, ADC2_CHANNEL_BASE_ADD + ADC2_TASK_INDEX, i + 1, ADC_SampleTime_55Cycles5);
			}
	}
	
	DMA_ADC2_INIT();
	TIM2_INIT();
	NVIC_ACD2_INIT();
	ADC2_RUN();
	My_SysTick_Delay(100);// 延迟100ms等待第一波采样结果处理，防止过早启动。
}

void ADC1_RUN(void)
{
	// 使能DMA和ADC
	ADC_DMACmd(ADC1, ENABLE);
	ADC_Cmd(ADC1, ENABLE);

	// 校准ADC
	ADC_ResetCalibration(ADC1);
	while(ADC_GetResetCalibrationStatus(ADC1));
	ADC_StartCalibration(ADC1);
	while(ADC_GetCalibrationStatus(ADC1));
	
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

void ADC2_RUN(void)
{
	// 使能DMA和ADC
	
	ADC_Cmd(ADC2, ENABLE);

	// 校准ADC
	ADC_ResetCalibration(ADC2);
	while(ADC_GetResetCalibrationStatus(ADC2));
	ADC_StartCalibration(ADC2);
	while(ADC_GetCalibrationStatus(ADC2));
	
	ADC_DMACmd(ADC2, ENABLE);
	
	ADC_SoftwareStartConvCmd(ADC2, ENABLE);
}

void DMA_ADC1_INIT(void)
{
	DMA_InitTypeDef DMA_InitStructure;

	// DMA1通道1配置（ADC1_DR -> Memory）
	DMA_DeInit(DMA1_Channel1);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)adc1_values;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = CURRENT_ADC1_TASKS_NUM;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular; // 循环模式
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);

	// 使能DMA传输完成中断
	DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE);
	DMA_Cmd(DMA1_Channel1, ENABLE);
}

void DMA_ADC2_INIT(void)
{
	DMA_InitTypeDef DMA_InitStructure;

	// DMA1通道2配置（ADC1_DR -> Memory）
	DMA_DeInit(DMA1_Channel2);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC2->DR;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)adc2_values;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = CURRENT_ADC2_TASKS_NUM; 
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular; // 循环模式
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel2, &DMA_InitStructure);

	// 使能DMA传输完成中断
	DMA_ITConfig(DMA1_Channel2, DMA_IT_TC, ENABLE);
	DMA_Cmd(DMA1_Channel2, ENABLE);
}

void TIM2_INIT(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;

	// TIM2配置（PWM输出触发ADC）
	TIM_TimeBaseStructure.TIM_Period = 999; // 自动重装载值
	TIM_TimeBaseStructure.TIM_Prescaler = 71; // 72MHz/(71+1)=1MHz
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

	// 配置通道2为PWM模式
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 500; // 50%占空比
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);

	// 选择TRGO输出源为OC2REF
	TIM_SelectOutputTrigger(TIM2, TIM_TRGOSource_OC2Ref);
	TIM_Cmd(TIM2, ENABLE);
}

void NVIC_ACD1_INIT(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	// 设置NVIC优先级分组
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	// 配置DMA中断
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void NVIC_ACD2_INIT(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	// 设置NVIC优先级分组
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	// 配置DMA中断
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void DMA1_Channel1_IRQHandler(void)
{
    if(DMA_GetITStatus(DMA1_IT_TC1))
    {
			//gpio_led_control(G, Reverse);
			DMA1_COMPLEATE = 1;
			DMA_ClearITPendingBit(DMA1_IT_TC1); // 清除中断标志
    }
}
void DMA1_Channel2_IRQHandler(void)
{
    if(DMA_GetITStatus(DMA1_IT_TC2))
    {
			//gpio_led_control(B, Reverse);
			DMA2_COMPLEATE = 1;
	    DMA_ClearITPendingBit(DMA1_IT_TC2); // 清除中断标志
    }
}

uint8_t Get_DMA_Flag(uint8_t x)
{
	if(x == 1)
		return DMA1_COMPLEATE;
	else 
		return DMA2_COMPLEATE;
}

void Reset_DMA_Flag(uint8_t x)
{
	if(x == 1)
		DMA1_COMPLEATE = 0;
	else 
		DMA2_COMPLEATE = 0;
}

void print_adc1_data(void)
{
	for(uint8_t i = 0; i < ADC_NUM; ++i)
			{
				if(adc1_tasks[i].exist)
					printf("DATA [%d]: %.4f V, ", i, adc1_values[i] * 3.3 / 4095);
			}
			printf("\r\n");
			Reset_DMA_Flag(1);				
}

void print_adc2_data(void)
{
	for(uint8_t i = 0; i < ADC_NUM; ++i)
			{
				if(adc1_tasks[i].exist)
					printf("DATA [%d]: %.4f V, ", i, adc2_values[i] * 3.3 / 4095);
			}
			printf("\r\n");
			Reset_DMA_Flag(2);				
}

uint16_t Get_ADC1_DATA(uint8_t index)
{	
	return adc1_values[index];
}

uint16_t Get_ADC2_DATA(uint8_t index)
{	
	return adc2_values[index];
}
