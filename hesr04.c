#include "hesr04.h"

STRUCT_CAPTURE TIM_ICUserValueStructure = {0,0,0,0};


// 配置中断
static void TIM_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);		
	
    NVIC_InitStructure.NVIC_IRQChannel = GENERAL_TIM_IRQn ;	
	
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	 
	
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

// 配置ECHO引脚
void TIM_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure = {0};
	
	// 开启ECHO引脚的时钟
	RCC_APB2PeriphClockCmd(ECHO_RCC_GPIO_CLK, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin =  ECHO_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_Init(ECHO_GPIO_PORT, &GPIO_InitStructure);	
	
	//进行TIM2的重映射，因为的TIM2的默认通道引脚为PA0和PA1， 但实际需要PB3
	GPIO_PinRemapConfig(GPIO_FullRemap_TIM2, ENABLE); 
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE); 
}

// 配置定时器
static void GENERAL_TIM_Mode_Config(void)
{
    // 开启定时器的时钟
    GENERAL_TIM_APBxClock_FUN(GENERAL_TIM_CLK,ENABLE);
    
/*--------------------时基单元初始化-------------------------*/	
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	TIM_TimeBaseStructure.TIM_Period=GENERAL_TIM_PERIOD;	

	TIM_TimeBaseStructure.TIM_Prescaler= GENERAL_TIM_PRESCALER;	
	
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;		
	
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;		
	
	TIM_TimeBaseStructure.TIM_RepetitionCounter=0;	
	
	TIM_TimeBaseInit(GENERAL_TIMx, &TIM_TimeBaseStructure);

	/*--------------------配置输入捕获通道2-------------------*/	
	TIM_ICInitTypeDef TIM_ICInitStructure;
	
	TIM_ICInitStructure.TIM_Channel = GENERAL_TIM_CHANNELx;
	
	TIM_ICInitStructure.TIM_ICPolarity = GENERAL_TIM_STRAT_ICPolarity;
	
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	
	TIM_ICInitStructure.TIM_ICFilter = 0;
	
	TIM_ICInit(GENERAL_TIMx, &TIM_ICInitStructure);
	
	// 清除定时器的更新标志位和通道2的捕获标志位
  TIM_ClearFlag(GENERAL_TIMx, TIM_FLAG_Update|GENERAL_TIM_IT_CCx);	
  // 配置定时器的更新中断和通道2捕获中断
	TIM_ITConfig (GENERAL_TIMx, TIM_IT_Update | GENERAL_TIM_IT_CCx, ENABLE );
	
	//开启定时器
	TIM_Cmd(GENERAL_TIMx, ENABLE);
    
}
 
// 配置TRIG引脚
static void CS100A_TRIG_GPIO_Config(void)
{		
		
    GPIO_InitTypeDef  GPIO_InitStruct = {0};

    RCC_APB2PeriphClockCmd(TRIG_RCC_GPIO_CLK, ENABLE);
  														   
    GPIO_InitStruct.GPIO_Pin = TRIG_GPIO_PIN;	

    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_Out_PP;  

    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(TRIG_GPIO_PORT, &GPIO_InitStruct);	
    
}

// 超声波模块测量触发函数
void CS100A_TRIG(void)
{
	GPIO_SetBits(TRIG_GPIO_PORT,TRIG_GPIO_PIN);
	
	DWT_DelayUs(30);// 必须大于10us
	
	GPIO_ResetBits(TRIG_GPIO_PORT,TRIG_GPIO_PIN);
	
}

/**
  * @brief  初始化的集合
  * @param  
  * @retval 
  */
void CS100A_Init(void)
{
    
    TIM_GPIO_Config();
    TIM_NVIC_Config();
    CS100A_TRIG_GPIO_Config();
    GENERAL_TIM_Mode_Config();
}


//配置中断响应函数
void GENERAL_TIM_IRQHANDLER(void)
{
	// 如何中断是由定时器溢出更新触发的，说明距离较远，超声波信号仍在返回途中。
	if ( TIM_GetITStatus ( GENERAL_TIMx, TIM_IT_Update) != RESET )               
	{	
		TIM_ICUserValueStructure.usPeriod ++;		
		TIM_ClearITPendingBit ( GENERAL_TIMx, TIM_FLAG_Update ); 		
	}

	// 如果中断是由捕获通道触发的，
	// 上升沿捕获中断，说明超声波信号刚被发送出去，因为超声波信号发送时ECHO会发送上升沿。
	// 下降沿捕获中断，说明超声波信号已经走完往返路程，并被被接收器接收到了，ECHO发送下降沿信号。
	if ( TIM_GetITStatus (GENERAL_TIMx, GENERAL_TIM_IT_CCx ) != RESET)
	{
		// 接受上升沿信号
		if ( TIM_ICUserValueStructure.ucStartFlag == 0 )
		{
			TIM_SetCounter ( GENERAL_TIMx, 0 );

			TIM_ICUserValueStructure.usPeriod = 0;
           	
			TIM_ICUserValueStructure.usCtr = 0;

			// 配置下一次中断由下降沿触发
			GENERAL_TIM_OCxPolarityConfig_FUN(GENERAL_TIMx, TIM_ICPolarity_Falling);
      // 捕获开始标志位置一     		
			TIM_ICUserValueStructure.ucStartFlag = 1;			
		}
		
		// 接受下降沿信号
		else
		{
			TIM_ICUserValueStructure.usCtr = GENERAL_TIM_GetCapturex_FUN (GENERAL_TIMx);

			GENERAL_TIM_OCxPolarityConfig_FUN(GENERAL_TIMx, TIM_ICPolarity_Rising);
      // 捕获开始标志位置零
			TIM_ICUserValueStructure.ucStartFlag = 0;
      // 捕获结束标志位置一		
			TIM_ICUserValueStructure.ucFinishFlag = 1;		
		}
		// 清除中断标志位
		TIM_ClearITPendingBit (GENERAL_TIMx,GENERAL_TIM_IT_CCx);	    
	}		
}

// 返回超声波信号一次往返的耗时，单位us
uint32_t Get_time(void)
{
	uint32_t time = TIM_ICUserValueStructure .usPeriod * GENERAL_TIM_PERIOD + TIM_ICUserValueStructure .usCtr;
	return time;
}

float Get_distance(uint32_t time)
{	
	if(!GET_TEMP_FLAG)
		temp_status = Get_DHTll_DATA(&dhtll_data_hesr04);
	
	if (temp_status)
	{
		GET_TEMP_FLAG = 1;
		return (float)time*(331.4 + 0.6 * dhtll_data_hesr04.temp_int)/2000000.00;
	}
	
	else 
	{
		GET_TEMP_FLAG = 0;
		return (float)time*340/2000000.00;
	}
}

void print_distance(void)
{
	uint32_t time = Get_time();
	if(GET_TEMP_FLAG)
		printf("%.4f M(温度参数:%d)\r\n", Get_distance(time), dhtll_data_hesr04.temp_int);
	else
		printf("%.4f M(无温度参数)\r\n", Get_distance(time));
		
}
