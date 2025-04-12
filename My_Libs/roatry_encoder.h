#ifndef __ROATRY_ENCODER
#define __ROATRY_ENCODER

#include "stm32f10x.h"
#include "usart.h"
#include "gpio.h"
#include "pwm_led.h"

/* 定义 ENCIPHEROR 连接的GPIO端口, 用户只需要修改下面的代码即可改变控制的 ENCIPHEROR 引脚 */

// ENCIPHEROR_A
#define ENCIPHEROR_A_GPIO_PORT    			GPIOA			                /* GPIO端口 */
#define ENCIPHEROR_A_GPIO_CLK_PORT 	        RCC_APB2Periph_GPIOA			/* GPIO端口时钟 */
#define ENCIPHEROR_A_GPIO_PIN			    GPIO_Pin_6	       				/* 连接到GPIO */

// ENCIPHEROR_B
#define ENCIPHEROR_B_GPIO_PORT    			GPIOA			                /* GPIO端口 */
#define ENCIPHEROR_B_GPIO_CLK_PORT 	        RCC_APB2Periph_GPIOA			/* GPIO端口时钟 */
#define ENCIPHEROR_B_GPIO_PIN			    GPIO_Pin_7	       				/* 连接到GPIO */

// ENCIPHEROR_SW
#define ENCIPHEROR_SW_GPIO_PORT    			GPIOA			                /* GPIO端口 */
#define ENCIPHEROR_SW_GPIO_CLK_PORT 	    RCC_APB2Periph_GPIOA			/* GPIO端口时钟 */
#define ENCIPHEROR_SW_GPIO_PIN			    GPIO_Pin_8	       				/* 连接到GPIO */

#define ENCIPHEROR_TIM   			        TIM3   			
#define ENCIPHEROR_TIM_CLK_PORT 	        RCC_APB1Periph_TIM3 		
#define ENCIPHEROR_TIM_APBXCLKCMD   		RCC_APB1PeriphClockCmd  
                                 
#define ENCIPHEROR_TIM_IRQ                  TIM3_IRQn         
#define ENCIPHEROR_TIM_IRQHANDLER           TIM3_IRQHandler  

#define ENCIPHEROR_A_CHANNEL                TIM_Channel_1
#define ENCIPHEROR_B_CHANNEL                TIM_Channel_2

#define ENCIPHEROR_TIM_PERIOD (65536-1)             //初始周期
#define ENCIPHEROR_TIM_PULSE  0                     //初始脉宽

typedef enum
{
	STILLNESS = 0,
	PRESS,
	RELEASE,
}SW_Status;

typedef struct
{
    uint8_t laststate;
    uint8_t nowstate;
    uint8_t key_scan_time;
    SW_Status key_flag;
}KEY_STATE_INFO;

static KEY_STATE_INFO sw = {0};

static uint8_t ENCODER_INIT_FLAG = 0;

static int brightness = 0;

void ENCIPHEROR_GPIO_Config(void);
void ENCIPHEROR_TIM_Mode_Config(void);
void ENCIPHEROR_TIM_Init(void);
void sw_Scan(LEDS led);

typedef struct
{
    uint8_t  positive_direction_flag;
    uint8_t  reverse_direction_flag;
    float   laps_times;         // 圈数
}Encipheror_TaskInfo;

static Encipheror_TaskInfo encipheror_task = {0};

void Encoder_print(void);
float Get_Encipheror_Value(void);
void Led_brightness_control(Breath_LEDx ledx, uint8_t brightness);
void roatry_controler(Breath_LEDx Ledx);

#endif //__ROATRY_ENCODER
