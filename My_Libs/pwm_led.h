#ifndef __PWM_BEATHE_CIRCULATION_H
#define __PWM_BEATHE_CIRCULATION_H

#include "stm32f10x.h"
#include "math.h"
#include "usart.h"

/* 定义 PWM_LED 连接的GPIO端口, 用户只需要修改下面的代码即可改变控制的 PWM_LED 引脚 */

#define PWM_RLED 1
#define PWM_GLED 2
#define PWM_BLED 3

// PWM_RLED
#define PWM_RLED_GPIO_PORT    			    GPIOA			                /* GPIO端口 */
#define PWM_RLED_GPIO_CLK_PORT 	            RCC_APB2Periph_GPIOA			/* GPIO端口时钟 */
#define PWM_RLED_GPIO_PIN			        GPIO_Pin_1	       				/* 连接到GPIO */
        
#define PWM_RLED_TIM_OCX_INIT 	            TIM_OC2Init			    //通道
#define PWM_RLED_TIM_OCXPRELOAD_CONFIG 	    TIM_OC2PreloadConfig    //预装载

// PWM_GLED
#define PWM_GLED_GPIO_PORT    			    GPIOA			                /* GPIO端口 */
#define PWM_GLED_GPIO_CLK_PORT 	            RCC_APB2Periph_GPIOA			/* GPIO端口时钟 */
#define PWM_GLED_GPIO_PIN			        GPIO_Pin_2	       				/* 连接到GPIO */
        
#define PWM_GLED_TIM_OCX_INIT 	            TIM_OC3Init			    //通道
#define PWM_GLED_TIM_OCXPRELOAD_CONFIG 	    TIM_OC3PreloadConfig    //预装载


// PWM_BLED
#define PWM_BLED_GPIO_PORT    			    GPIOA			                /* GPIO端口 */
#define PWM_BLED_GPIO_CLK_PORT 	            RCC_APB2Periph_GPIOA			/* GPIO端口时钟 */
#define PWM_BLED_GPIO_PIN			        GPIO_Pin_3	       				/* 连接到GPIO */
        
#define PWM_BLED_TIM_OCX_INIT 	            TIM_OC4Init			    //通道
#define PWM_BLED_TIM_OCXPRELOAD_CONFIG 	    TIM_OC4PreloadConfig    //预装载


#define PWM_LED_TIM   			            TIM2   			
#define PWM_LED_TIM_CLK_PORT 	            RCC_APB1Periph_TIM2 		
#define PWM_LED_TIM_APBXCLKCMD   		    RCC_APB1PeriphClockCmd  
                                            
#define PWM_LED_TIM_IRQ                     TIM2_IRQn         
#define PWM_LED_TIM_IRQHANDLER              TIM2_IRQHandler  


#define PWM_LED_PERIOD (1000-1)                 //初始周期
#define PWM_LED_PULSE  0                    //初始脉宽

#define LED_MAX_BRIGHTNESS  100         // LED最大亮度
#define GAMMA_CORRECTION    1.5         // 伽马矫正系数，可以根据实际情况调整


typedef struct
{
    uint8_t  flag;
    uint8_t  id;
    int16_t  direction;     //呼吸方向，1为增加，-1为减少,0表示不变
    uint16_t brightness;    //  亮度值
}BreathLight_TaskInfo;

typedef enum BL
{
	B_R = 0,
	B_G,
	B_B,
}Breath_LEDx;


void PWM_LED_GPIO_Config(Breath_LEDx Ledx);
void PWM_LED_TIM_Mode_Config(Breath_LEDx Ledx);
void PWM_LED_TIM_Init(Breath_LEDx Ledx);
void PWM_LED_PulseConfig(uint16_t servo_num,uint16_t pwm_pulse);
void PWM_LED_CycleConfig(uint16_t pwm_cycle);

extern BreathLight_TaskInfo breath_light1_task;
extern BreathLight_TaskInfo breath_light2_task;
extern BreathLight_TaskInfo breath_light3_task;

static uint8_t breath_lightr_task_init_flag = 0;
static uint8_t breath_lightg_task_init_flag = 0;
static uint8_t breath_lightb_task_init_flag = 0;

static uint8_t breath_lightr_pwm_init_flag = 0;
static uint8_t breath_lightg_pwm_init_flag = 0;
static uint8_t breath_lightb_pwm_init_flag = 0;

void BreathLight_TaskInit(Breath_LEDx Ledx);
void BreathLight_Process(BreathLight_TaskInfo* breath_light_task);
uint8_t Gamma_Correct(uint8_t brightness);
void BreathLight(Breath_LEDx Ledx);
BreathLight_TaskInfo* Get_Task_ID(Breath_LEDx Ledx);
void control_pwm_init_flag(Breath_LEDx Ledx);
uint8_t* Get_pwm_init_flag(Breath_LEDx Ledx);
uint8_t* Get_task_init_flag(Breath_LEDx Ledx);
#endif //__PWM_BEATHE_CIRCULATION_H
