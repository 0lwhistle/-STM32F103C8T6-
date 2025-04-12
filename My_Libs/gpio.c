#include "gpio.h"

void gpio_led_init(LEDS led)
{
switch(led)
		{
        case R:
						/* 开启 LED 相关的GPIO外设/端口时钟 */
						RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
    
						/* IO输出状态初始化控制 */
						GPIO_SetBits(GPIOA,GPIO_Pin_1);
    
						/*选择要控制的GPIO引脚、设置GPIO模式为 推挽模式、设置GPIO速率为2MHz*/
						gpio_initstruct.GPIO_Pin    = GPIO_Pin_1;
						gpio_initstruct.GPIO_Mode   = GPIO_Mode_Out_PP;
						gpio_initstruct.GPIO_Speed  = GPIO_Speed_2MHz;
						GPIO_Init(GPIOA,&gpio_initstruct);
						init_list.LED_R = 1;
            break;
        case G:
						/* 开启 LED 相关的GPIO外设/端口时钟 */
						RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
    
						/* IO输出状态初始化控制 */
						GPIO_SetBits(GPIOA,GPIO_Pin_2);
    
						/*选择要控制的GPIO引脚、设置GPIO模式为 推挽模式、设置GPIO速率为2MHz*/
						gpio_initstruct.GPIO_Pin    = GPIO_Pin_2;
						gpio_initstruct.GPIO_Mode   = GPIO_Mode_Out_PP;
						gpio_initstruct.GPIO_Speed  = GPIO_Speed_2MHz;
						GPIO_Init(GPIOA,&gpio_initstruct);
						init_list.LED_G = 1;
            break;
				case B:
						/* 开启 LED 相关的GPIO外设/端口时钟 */
						RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
    
						/* IO输出状态初始化控制 */
						GPIO_SetBits(GPIOA,GPIO_Pin_3);
    
						/*选择要控制的GPIO引脚、设置GPIO模式为 推挽模式、设置GPIO速率为2MHz*/
						gpio_initstruct.GPIO_Pin    = GPIO_Pin_3;
						gpio_initstruct.GPIO_Mode   = GPIO_Mode_Out_PP;
						gpio_initstruct.GPIO_Speed  = GPIO_Speed_2MHz;
						GPIO_Init(GPIOA,&gpio_initstruct);
						init_list.LED_B = 1;
            break;
				case OUT:
						/* 开启 LED 相关的GPIO外设/端口时钟 */
						RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
    
						/* IO输出状态初始化控制 */
						GPIO_SetBits(GPIOB,GPIO_Pin_5);
    
						/*选择要控制的GPIO引脚、设置GPIO模式为 推挽模式、设置GPIO速率为2MHz*/
						gpio_initstruct.GPIO_Pin    = GPIO_Pin_5;
						gpio_initstruct.GPIO_Mode   = GPIO_Mode_Out_PP;
						gpio_initstruct.GPIO_Speed  = GPIO_Speed_2MHz;
						GPIO_Init(GPIOB,&gpio_initstruct);
						init_list.LED_OUT = 1;
            break;
				
        default:
            // 处理未知单位或错误值
            while(1); // 出错时进入死循环
		}
}

void gpio_led_control(LEDS led, CONTROLER status)
{
switch(led)
		{
        case R:
						switch(status)
								{
										case Turn_on:
											if(init_list.LED_R == 0) gpio_led_init(R);											
											GPIO_ResetBits(GPIOA,GPIO_Pin_1);			
											break;
										
										case Turn_off:
											if(init_list.LED_R == 0) gpio_led_init(R);											
											GPIO_SetBits(GPIOA,GPIO_Pin_1);			
											break;
										
										case Reverse:
											if(init_list.LED_R == 0) gpio_led_init(R);	
											GPIOA->ODR ^= GPIO_Pin_1;
											break;
										default:
											// 处理未知单位或错误值
											while(1); // 出错时进入死循环							
								}
            break;
								
        case G:
						switch(status)
								{
										case Turn_on:
											if(init_list.LED_G == 0) gpio_led_init(G);											
											GPIO_ResetBits(GPIOA,GPIO_Pin_2);			
											break;
										
										case Turn_off:
											if(init_list.LED_G == 0) gpio_led_init(G);											
											GPIO_SetBits(GPIOA,GPIO_Pin_2);			
											break;
										
										case Reverse:
											if(init_list.LED_G == 0) gpio_led_init(G);	
											GPIOA->ODR ^= GPIO_Pin_2;
											break;	
										default:
											// 处理未知单位或错误值
											while(1); // 出错时进入死循环										
								}
            break;
								
				case B:
						switch(status)
								{
										case Turn_on:
											if(init_list.LED_B == 0) gpio_led_init(B);											
											GPIO_ResetBits(GPIOA,GPIO_Pin_3);			
											break;
										
										case Turn_off:
											if(init_list.LED_B == 0) gpio_led_init(B);											
											GPIO_SetBits(GPIOA,GPIO_Pin_3);			
											break;
										
										case Reverse:
											if(init_list.LED_B == 0) gpio_led_init(B);	
											GPIOA->ODR ^= GPIO_Pin_3;
											break;	
										default:
											// 处理未知单位或错误值
											while(1); // 出错时进入死循环										
								}
            break;
								
				case OUT:
						switch(status)
								{
										case Turn_on:
											if(init_list.LED_OUT == 0) gpio_led_init(OUT);											
											GPIO_ResetBits(GPIOB,GPIO_Pin_5);			
											break;
										
										case Turn_off:
											if(init_list.LED_OUT == 0) gpio_led_init(OUT);											
											GPIO_SetBits(GPIOB,GPIO_Pin_5);			
											break;
										
										case Reverse:
											if(init_list.LED_OUT == 0) gpio_led_init(OUT);	
											GPIOB->ODR ^= GPIO_Pin_5;
											break;
										default:
											// 处理未知单位或错误值
											while(1); // 出错时进入死循环
								}
            break;
								
        default:
            // 处理未知单位或错误值
            while(1); // 出错时进入死循环
		}
}

void gpio_beep_init()
{
    /* 开启 BEEP 相关的GPIO外设/端口时钟 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
    
    /* IO输出状态初始化控制 */
    GPIO_ResetBits(GPIOA,GPIO_Pin_6);
    
    /*选择要控制的GPIO引脚、设置GPIO模式为 推挽模式、设置GPIO速率为2MHz*/
    gpio_initstruct.GPIO_Pin    = GPIO_Pin_6;
    gpio_initstruct.GPIO_Mode   = GPIO_Mode_Out_PP;
    gpio_initstruct.GPIO_Speed  = GPIO_Speed_2MHz;
    GPIO_Init(GPIOA,&gpio_initstruct);
		
		init_list.BEEP = 1;
}
void gpio_beep_control(CONTROLER status)
{
	switch(status)
			{
					case Turn_off:
						if(init_list.BEEP == 0) gpio_beep_init();											
						GPIO_ResetBits(GPIOA,GPIO_Pin_6);			
						break;
					
					case Turn_on:
						if(init_list.BEEP == 0) gpio_beep_init();											
						GPIO_SetBits(GPIOA,GPIO_Pin_6);			
						break;
					
					case Reverse:
						if(init_list.BEEP == 0) gpio_beep_init();	
						GPIOA->ODR ^= GPIO_Pin_6;
						break;
					default:
						// 处理未知单位或错误值
						while(1); // 出错时进入死循环
			}
}

void gpio_heat_sensitive_sensor_init()
{
		/* 开启 LIGHT 相关的GPIO外设/端口时钟 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
    
    /* IO输出状态初始化控制 */
    GPIO_SetBits(GPIOA,GPIO_Pin_11);
    
    /*选择要控制的GPIO引脚、设置GPIO模式为 浮空输入、设置GPIO速率为2MHz*/
    gpio_initstruct.GPIO_Pin    = GPIO_Pin_11;
    gpio_initstruct.GPIO_Mode   = GPIO_Mode_IN_FLOATING;
    gpio_initstruct.GPIO_Speed  = GPIO_Speed_2MHz;
    GPIO_Init(GPIOA,&gpio_initstruct);
	init_list.heat_sensitive_sensor = 1;
}

uint8_t Heat_Scan()
{
	if(init_list.heat_sensitive_sensor == 0) gpio_heat_sensitive_sensor_init();
		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_11) == Bit_RESET)
    {
        return 1;//检测到过热返回1
    }
    else
    {
        return 0;//没检测到热源返回0
    }
}

void gpio_photosensitive_sensor_init()
{
		/* 开启 LIGHT 相关的GPIO外设/端口时钟 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
    
    /* IO输出状态初始化控制 */
    GPIO_SetBits(GPIOA,GPIO_Pin_11);
    
    /*选择要控制的GPIO引脚、设置GPIO模式为 浮空输入、设置GPIO速率为2MHz*/
    gpio_initstruct.GPIO_Pin    = GPIO_Pin_11;
    gpio_initstruct.GPIO_Mode   = GPIO_Mode_IN_FLOATING;
    gpio_initstruct.GPIO_Speed  = GPIO_Speed_2MHz;
    GPIO_Init(GPIOA,&gpio_initstruct);
	init_list.photosensitive_sensor = 1;
}

uint8_t photosensitive_Scan()
{
		if(init_list.photosensitive_sensor == 0) gpio_photosensitive_sensor_init();
		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_11) == Bit_RESET)
    {
        return 1;//检测到光返回1
    }
    else
    {
        return 0;//没检测到光返回0
    }
}

void gpio_reflective_sensor_init()
	{
		/* 开启 LIGHT 相关的GPIO外设/端口时钟 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
    
    /* IO输出状态初始化控制 */
    GPIO_SetBits(GPIOA,GPIO_Pin_11);
    
    /*选择要控制的GPIO引脚、设置GPIO模式为 浮空输入、设置GPIO速率为2MHz*/
    gpio_initstruct.GPIO_Pin    = GPIO_Pin_11;
    gpio_initstruct.GPIO_Mode   = GPIO_Mode_IN_FLOATING;
    gpio_initstruct.GPIO_Speed  = GPIO_Speed_2MHz;
    GPIO_Init(GPIOA,&gpio_initstruct);
	init_list.reflective_sensor = 1;
}
	
uint8_t reflective_Scan()
{
		if(init_list.reflective_sensor == 0) gpio_reflective_sensor_init();
		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_11) == Bit_RESET)
    {
        return 1;//检测到物体返回1
    }
    else
    {
        return 0;//没检测到物体返回0
    }
}
