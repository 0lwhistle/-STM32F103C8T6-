#include "roatry_encoder.h"
/**
 * @brief  初始化控制 ENCIPHEROR 的IO
 * @param  无
 * @retval 无
 */
void ENCIPHEROR_GPIO_Config(void)
{
    /* 定义一个 GPIO 结构体 */
    GPIO_InitTypeDef GPIO_InitStructure = {0};
   
    /*开启 ENCIPHEROR 相关的GPIO外设/端口时钟*/
    RCC_APB2PeriphClockCmd(ENCIPHEROR_A_GPIO_CLK_PORT,ENABLE);
    RCC_APB2PeriphClockCmd(ENCIPHEROR_B_GPIO_CLK_PORT,ENABLE);
    RCC_APB2PeriphClockCmd(ENCIPHEROR_SW_GPIO_CLK_PORT,ENABLE);
    
    /*选择要控制的GPIO引脚、设置GPIO模式为 上拉输入、设置GPIO速率为50MHz*/
    GPIO_InitStructure.GPIO_Mode    =   GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin     =   ENCIPHEROR_A_GPIO_PIN;
    GPIO_InitStructure.GPIO_Speed   =   GPIO_Speed_50MHz;
    GPIO_Init(ENCIPHEROR_A_GPIO_PORT,&GPIO_InitStructure);
    
    /*选择要控制的GPIO引脚、设置GPIO模式为 上拉输入、设置GPIO速率为50MHz*/
    GPIO_InitStructure.GPIO_Pin     =   ENCIPHEROR_B_GPIO_PIN;
    GPIO_Init(ENCIPHEROR_B_GPIO_PORT,&GPIO_InitStructure);
    
    /*选择要控制的GPIO引脚、设置GPIO模式为 上拉输入、设置GPIO速率为50MHz*/
    GPIO_InitStructure.GPIO_Pin     =   ENCIPHEROR_SW_GPIO_PIN;
    GPIO_Init(ENCIPHEROR_SW_GPIO_PORT,&GPIO_InitStructure);
    
}


/**
  * @brief  配置 ENCIPHEROR_TIM 模式配置
  * @param  无
  * @retval 无
  */
void ENCIPHEROR_TIM_Mode_Config(void)
{
    /* 定义一个 TIM 结构体 */
	TIM_TimeBaseInitTypeDef  tim_timebasestructure = {0};		
    
    /* 定义一个 TIM输入配置 结构体 */
    TIM_ICInitTypeDef  tim_icinitstructure = {0};	

    /*开启 TIM 相关的外设/端口时钟*/
    ENCIPHEROR_TIM_APBXCLKCMD(ENCIPHEROR_TIM_CLK_PORT, ENABLE); 	    //使能TIMx时钟
    
    /* 通用定时器配置 */		 
    tim_timebasestructure.TIM_Period            = ENCIPHEROR_TIM_PERIOD;          //自动重装载寄存器的值，累计TIM_Period个频率后产生一个更新或者中断
    tim_timebasestructure.TIM_Prescaler         = 0;	    	                    //设置预分频，计数器的时钟频率CK_CNT等于fCK_PSC/(TIM_Prescaler+1)。
    tim_timebasestructure.TIM_ClockDivision     = TIM_CKD_DIV1 ;	                //设置时钟分频系数：不分频(这里用不到)
    tim_timebasestructure.TIM_CounterMode       = TIM_CounterMode_Up;  	        //向上计数模式
    tim_timebasestructure.TIM_RepetitionCounter = 0;                              //重复计数器的值，没用到不用管
    TIM_TimeBaseInit(ENCIPHEROR_TIM, &tim_timebasestructure);                   // 初始化定时器
 
    tim_icinitstructure.TIM_Channel = ENCIPHEROR_A_CHANNEL|ENCIPHEROR_B_CHANNEL;
    tim_icinitstructure.TIM_ICFilter = 0;
    
    TIM_ICInit(ENCIPHEROR_TIM, &tim_icinitstructure);
    
    /* 配置TIM编码器接口 */
    TIM_EncoderInterfaceConfig(ENCIPHEROR_TIM,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising); 
}

/**
 * @brief  ENCIPHEROR_TIM 初始化
 * @param  无
 * @retval 无
 */
void ENCIPHEROR_TIM_Init(void)
{
    /* 对应的 GPIO 的配置 */
    ENCIPHEROR_GPIO_Config();
    
    /* 配置 PWM_LED_TIM 模式 */
		ENCIPHEROR_TIM_Mode_Config();
    
    /* 使能 TIM */
    TIM_Cmd(ENCIPHEROR_TIM,ENABLE);
	
		ENCODER_INIT_FLAG = 1;
}

void sw_Scan(LEDS led)
{
		if(!ENCODER_INIT_FLAG) ENCIPHEROR_TIM_Init();
    sw.laststate = sw.nowstate;
    sw.nowstate = GPIO_ReadInputDataBit(ENCIPHEROR_SW_GPIO_PORT, ENCIPHEROR_SW_GPIO_PIN);
    if(sw.laststate == SET && sw.nowstate == RESET) //检测按键按下
    {
        sw.key_flag = PRESS;
				gpio_led_control(led, Turn_on);
    }
    else if(sw.laststate == RESET && sw.nowstate == SET) //检测按键松开
    {
				gpio_led_control(led, Turn_off);
        sw.key_flag = RELEASE;
    }
}

/**
  * @brief  获取编码器圈数
  * @param  无
  * @retval 当前累计圈数
	*备注：每次中断获取当前定时器的计数值，并计算旋转方向和角度，然后将计算值归零，方便下次中断计算。
  */

//每次中断获取当前定时器的计数值，并计算旋转方向和角度，然后将计算值归零，方便下次中断计算。
float Get_Encipheror_Value(void)
{
		if(!ENCODER_INIT_FLAG) ENCIPHEROR_TIM_Init();

    int16_t temp;
    
    /* 获取当前计数值 */
    temp = TIM_GetCounter(ENCIPHEROR_TIM);
    
    /* 圈数累加 */
    encipheror_task.laps_times += (float)temp/ 80;
    
    if(temp == 0)   //静止
    {
        encipheror_task.positive_direction_flag = 0;
        encipheror_task.reverse_direction_flag = 0; 
    }
    else if(temp >= 0)  //正方向转
    {
        encipheror_task.positive_direction_flag = 1;
        encipheror_task.reverse_direction_flag = 0; 
    }
    else if(temp <= 0)  //反方向转
    {
        encipheror_task.positive_direction_flag = 0;
        encipheror_task.reverse_direction_flag = 1; 
    }
    
    /* 清零CNT */
    TIM_SetCounter(ENCIPHEROR_TIM,0);//这步很重要，不归零下次中断无法正常计算。
    return encipheror_task.laps_times;
}

/**
 * @brief  Encipheror 任务
 * @param  无
 * @retval 无
 */
void Encoder_print()
{
				if(!ENCODER_INIT_FLAG) ENCIPHEROR_TIM_Init();

				/* 获取圈数 */
        Get_Encipheror_Value();
        printf("\r\n当前已转过圈数: %0.2f r \r\n",encipheror_task.laps_times);
        
        /* 检查方向标志位 */
        if(encipheror_task.positive_direction_flag == 1)
        {
						brightness +=5;
            printf("当前旋转方向:正转");
            encipheror_task.positive_direction_flag = 0;
        }
        else if(encipheror_task.reverse_direction_flag == 1)
        {
						brightness -=5;
            printf("当前旋转方向:反转");
            encipheror_task.reverse_direction_flag = 0;
        }
        else
        {
//            printf("当前旋转方向:未旋转");
						printf("当前未旋转\n");
        }
        
        /* 检查编码器按键标志位 */
        if(sw.key_flag)      
        {      
            switch((uint8_t)sw.key_flag)
            {
                case 1:
                    printf("\r\n编码器按键按下\r\n");
                    break;
                case 2:
                    printf("\r\n编码器按键松开\r\n");
                    break;
                default:
                    break;
            }
            sw.key_flag = STILLNESS;
        }  
}

void Led_brightness_control(Breath_LEDx Ledx, uint8_t brightness)
{
	uint8_t* tmp1 = Get_task_init_flag(Ledx);
	if (!*tmp1) BreathLight_TaskInit(Ledx);
	
	uint8_t* tmp2 = Get_pwm_init_flag(Ledx);
	if (!*tmp2) PWM_LED_TIM_Init(Ledx);
	
	BreathLight_TaskInfo* breath_lightx_task = Get_Task_ID(Ledx);
	
	uint8_t corrected_brightness = Gamma_Correct(brightness);
	PWM_LED_CycleConfig(LED_MAX_BRIGHTNESS);
	PWM_LED_PulseConfig(breath_lightx_task->id, corrected_brightness);

}

void roatry_controler(Breath_LEDx Ledx)
{

	if(brightness < 0) brightness = 0;
	if(brightness > 100) brightness = 100;
	
	printf("brightness = %d", brightness);

	Led_brightness_control(Ledx, brightness);
}
