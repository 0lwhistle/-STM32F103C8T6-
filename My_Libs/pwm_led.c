#include "pwm_led.h"

BreathLight_TaskInfo breath_light1_task = {0};
BreathLight_TaskInfo breath_light2_task = {0};
BreathLight_TaskInfo breath_light3_task = {0};

/**
 * @brief  呼吸灯 任务初始化
 * @param  breath_light_task_cycle: 任务轮询周期 单位ms(可修改系统节拍定时器)
 * @retval 无
 */
void BreathLight_TaskInit(Breath_LEDx Ledx)
{ 
	switch (Ledx)
	{
		case B_R:
			breath_light1_task.id      = PWM_RLED;
			breath_light1_task.direction      = 1;
			breath_lightr_task_init_flag = 1;
			break;
	
		case B_G:
			breath_light2_task.id      = PWM_GLED;
		  breath_light2_task.direction      = 1;
			breath_lightg_task_init_flag = 1;
			break;
		
		case B_B:
    breath_light3_task.id      = PWM_BLED;
    breath_light3_task.direction      = 1;
		breath_lightb_task_init_flag = 1;
    break;
	}
	
    PWM_LED_CycleConfig(PWM_LED_PERIOD);
		//PWM_LED_CycleConfig(LED_MAX_BRIGHTNESS);    
}

/**
  * @brief  伽马矫正函数
  * @param  brightness: 矫正值输入
  * @retval 矫正值输出
  */
uint8_t Gamma_Correct(uint8_t brightness) 
{
    return (uint8_t)(pow((double)brightness / LED_MAX_BRIGHTNESS, GAMMA_CORRECTION) * LED_MAX_BRIGHTNESS);
}

/* 设置使得整个呼吸过程为3秒左右即可达到很好的效果 */	
//要求：
//TIM_Period：与PWM表中数值范围一致
//TIM_Prescaler：越小越好，可减轻闪烁现象
/**
  * @brief  呼吸灯效果函数
  * @param  无
  * @retval 无
  */
void BreathLight_Process(BreathLight_TaskInfo* breath_light_task) 
{
    // 更新亮度值
    breath_light_task->brightness += breath_light_task->direction;
    
    /* 确定方向 */
    if (breath_light_task->brightness == 0 || breath_light_task->brightness == LED_MAX_BRIGHTNESS) 
    {
        // 改变呼吸方向
        breath_light_task->direction = -breath_light_task->direction;
    }
 
    /* 对亮度进行伽马矫正 */
    uint8_t corrected_brightness = Gamma_Correct(breath_light_task->brightness);
		//uint8_t corrected_brightness = Gamma_Correct(1000);
    /* 配置LED亮度最大亮度  */
    PWM_LED_CycleConfig(LED_MAX_BRIGHTNESS);
    /* 控制LED亮度 */
    PWM_LED_PulseConfig(breath_light_task->id,corrected_brightness);
}

/**
 * @brief  初始化控制 PWM_LED 的IO
 * @param  无
 * @retval 无
 */
void PWM_LED_GPIO_Config(Breath_LEDx Ledx)
{
    /* 定义一个 GPIO 结构体 */
    GPIO_InitTypeDef GPIO_InitStructure = {0};

/**************************** 核心板载LED灯 *****************************/
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	if (Ledx == B_R)    
	{
    /*开启 PWM_RLED 相关的GPIO外设/端口时钟*/
    RCC_APB2PeriphClockCmd(PWM_RLED_GPIO_CLK_PORT,ENABLE);

    /* IO输出状态初始化控制 */
    GPIO_ResetBits(PWM_RLED_GPIO_PORT,PWM_RLED_GPIO_PIN);
    
    /*选择要控制的GPIO引脚、设置GPIO模式为通用 复用推挽输出、设置GPIO速率为50MHz*/
    GPIO_InitStructure.GPIO_Mode    =   GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin     =   PWM_RLED_GPIO_PIN;
    GPIO_InitStructure.GPIO_Speed   =   GPIO_Speed_50MHz;
    GPIO_Init(PWM_RLED_GPIO_PORT,&GPIO_InitStructure); 
    
	}
    
	if (Ledx == B_G)    
	{
    /*开启 PWM_GLED 相关的GPIO外设/端口时钟*/
    RCC_APB2PeriphClockCmd(PWM_GLED_GPIO_CLK_PORT,ENABLE);

    /* IO输出状态初始化控制 */
    GPIO_ResetBits(PWM_GLED_GPIO_PORT,PWM_GLED_GPIO_PIN);
    
    /*选择要控制的GPIO引脚、设置GPIO模式为通用 复用推挽输出、设置GPIO速率为50MHz*/
    GPIO_InitStructure.GPIO_Mode    =   GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin     =   PWM_GLED_GPIO_PIN;
    GPIO_InitStructure.GPIO_Speed   =   GPIO_Speed_50MHz;
    GPIO_Init(PWM_GLED_GPIO_PORT,&GPIO_InitStructure); 
    
	}

	if (Ledx == B_B)    
	{
    /*开启 PWM_BLED 相关的GPIO外设/端口时钟*/
    RCC_APB2PeriphClockCmd(PWM_BLED_GPIO_CLK_PORT,ENABLE);

    /* IO输出状态初始化控制 */
    GPIO_ResetBits(PWM_BLED_GPIO_PORT,PWM_BLED_GPIO_PIN);
    
    /*选择要控制的GPIO引脚、设置GPIO模式为通用 复用推挽输出、设置GPIO速率为50MHz*/
    GPIO_InitStructure.GPIO_Mode    =   GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin     =   PWM_BLED_GPIO_PIN;
    GPIO_InitStructure.GPIO_Speed   =   GPIO_Speed_50MHz;
    GPIO_Init(PWM_BLED_GPIO_PORT,&GPIO_InitStructure); 
    
	}

}

/**
  * @brief  配置 PWM_LED_TIM 模式配置
  * @param  无
  * @retval 无
  */
void PWM_LED_TIM_Mode_Config(Breath_LEDx Ledx)
{
    /* 定义一个 GENERALTIM 结构体 */
	TIM_TimeBaseInitTypeDef  tim_timebasestructure = {0};		
    
    /* 定义一个 PWM输出配置 结构体 */
    TIM_OCInitTypeDef  tim_ocinitstructure = {0};	

    /*开启 GENERALTIM 相关的外设/端口时钟*/
    PWM_LED_TIM_APBXCLKCMD(PWM_LED_TIM_CLK_PORT, ENABLE); 	    //使能TIMx时钟
    
    /* 通用定时器配置 */		 
    tim_timebasestructure.TIM_Period            = PWM_LED_PERIOD;       		    //自动重装载寄存器的值，累计TIM_Period个频率后产生一个更新或者中断
    tim_timebasestructure.TIM_Prescaler         = (72-1);	    	    //设置预分频，计数器的时钟频率CK_CNT等于fCK_PSC/(TIM_Prescaler+1)。
    tim_timebasestructure.TIM_ClockDivision     = TIM_CKD_DIV1 ;	    //设置时钟分频系数：不分频(这里用不到)
    tim_timebasestructure.TIM_CounterMode       = TIM_CounterMode_Up;  	//向上计数模式
    tim_timebasestructure.TIM_RepetitionCounter = 0;                    //重复计数器的值，没用到不用管
    TIM_TimeBaseInit(PWM_LED_TIM, &tim_timebasestructure);            // 初始化定时器
 
/*    
    例如向上计数时:
        PWM模式1下，TIMx_CNT<TIMx_CCRn时，输出有效电平;TIMx_CNT>TIMx_CCRn时，输出无效电平 
        PWM模式2下，TIMx_CNT<TIMx_CCRn时，输出无效电平;TIMx_CNT>TIMx_CCRn时，输出有效电平
*/    
    /* PWM模式配置 */
    tim_ocinitstructure.TIM_OCMode      = TIM_OCMode_PWM1;	    		//配置为PWM模式1
    tim_ocinitstructure.TIM_OutputState = TIM_OutputState_Enable;	    //使能输出
    tim_ocinitstructure.TIM_Pulse       = PWM_LED_PULSE;				//设置初始PWM脉冲宽度	
    tim_ocinitstructure.TIM_OCPolarity  = TIM_OCPolarity_Low;  	        //当定时器计数值小于CCR_Val时为低电平

    /* 使能通道和预装载 */
		if(Ledx == B_R)
		{
    PWM_RLED_TIM_OCX_INIT(PWM_LED_TIM, &tim_ocinitstructure);								
    PWM_RLED_TIM_OCXPRELOAD_CONFIG(PWM_LED_TIM, TIM_OCPreload_Enable);
		}
		
		if(Ledx == B_G)
		{
    PWM_GLED_TIM_OCX_INIT(PWM_LED_TIM, &tim_ocinitstructure);								
    PWM_GLED_TIM_OCXPRELOAD_CONFIG(PWM_LED_TIM, TIM_OCPreload_Enable);
		}
		
		if(Ledx == B_B)
		{
    PWM_BLED_TIM_OCX_INIT(PWM_LED_TIM, &tim_ocinitstructure);								
    PWM_BLED_TIM_OCXPRELOAD_CONFIG(PWM_LED_TIM, TIM_OCPreload_Enable);
		}
    
    /* 使能重载寄存器ARR */
    TIM_ARRPreloadConfig(PWM_LED_TIM, ENABLE);//使能重载寄存器ARR
    
}

/**
 * @brief  PWM_LED_TIM 初始化
 * @param  无
 * @retval 无
 */
void PWM_LED_TIM_Init(Breath_LEDx Ledx)
{
    /* 对应的 GPIO 的配置 */
    PWM_LED_GPIO_Config(Ledx);
    
    /* 配置 PWM_LED_TIM 模式 */
		PWM_LED_TIM_Mode_Config(Ledx);
    
    /* 使能 TIM */
    TIM_Cmd(PWM_LED_TIM,ENABLE);
	
		control_pwm_init_flag(Ledx);
		//printf("LED %d PWM Init done", Ledx);
}

/**
  * @brief  配置 PWM_LED_TIM 输出的PWM信号的脉宽
  * @param  led_num：led接口号
  * @param  pwm_pulse：脉冲长度
  * @retval 无
  */
void PWM_LED_PulseConfig(uint16_t led_num,uint16_t pwm_pulse)
{	
    if (led_num == PWM_RLED)
    {
        TIM_SetCompare2(PWM_LED_TIM, pwm_pulse);
    }
    if (led_num == PWM_GLED)
    {
        TIM_SetCompare3(PWM_LED_TIM, pwm_pulse);
    }
    if (led_num == PWM_BLED)
    {
        TIM_SetCompare4(PWM_LED_TIM, pwm_pulse);
    }
}

/**
  * @brief  配置 PWM_LED_TIM 输出的PWM信号的周期
  * @param  pwm_cycle（单位：1/SystemCoreClock*TIM_Prescaler S）
  * @retval 无
  */
void PWM_LED_CycleConfig(uint16_t pwm_cycle)
{
    TIM_SetAutoreload(PWM_LED_TIM, pwm_cycle);	
}

void BreathLight(Breath_LEDx Ledx)
{
	switch(Ledx)
	{
		case B_R:
			if (!breath_lightr_pwm_init_flag) PWM_LED_TIM_Init(Ledx);
			if(!breath_lightr_task_init_flag) BreathLight_TaskInit(Ledx);
			BreathLight_Process(&breath_light1_task);
			break;
		
		case B_G:
			if (!breath_lightg_pwm_init_flag) PWM_LED_TIM_Init(Ledx);
			if(!breath_lightg_task_init_flag) BreathLight_TaskInit(Ledx);	
			BreathLight_Process(&breath_light2_task);
			break;
		
		case B_B:
			if (!breath_lightb_pwm_init_flag) PWM_LED_TIM_Init(Ledx);
			if(!breath_lightb_task_init_flag) BreathLight_TaskInit(Ledx);	
			BreathLight_Process(&breath_light3_task);
			break;
	}
}

BreathLight_TaskInfo* Get_Task_ID(Breath_LEDx Ledx)
{
	if(Ledx == B_R) return &breath_light1_task;
	if(Ledx == B_G) return &breath_light2_task;
	else return &breath_light3_task;
}

void control_pwm_init_flag(Breath_LEDx Ledx)
{
	if(Ledx == B_R) breath_lightr_pwm_init_flag = 1;
	if(Ledx == B_G) breath_lightg_pwm_init_flag = 1;
	else breath_lightb_pwm_init_flag = 1;
}

uint8_t* Get_pwm_init_flag(Breath_LEDx Ledx)
{
	if(Ledx == B_R) return &breath_lightr_pwm_init_flag;
	if(Ledx == B_G) return &breath_lightg_pwm_init_flag;
	else return &breath_lightb_pwm_init_flag;
}

uint8_t* Get_task_init_flag(Breath_LEDx Ledx)
{
	if(Ledx == B_R) return &breath_lightr_task_init_flag;
	if(Ledx == B_G) return &breath_lightg_task_init_flag;
	else return &breath_lightb_task_init_flag;
}

/*********************************************END OF FILE**********************/

