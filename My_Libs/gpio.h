#ifndef __GPIO_H
#define __GPIO_H

#include "stm32f10x.h"
#include <stm32f10x_gpio.h>
#include <stdio.h>

typedef enum LEDS
{
	R = 0,
	G,
	B,
	OUT,
}LEDS;

typedef enum CONTROLER
{
	Turn_off = 0,
	Turn_on = 1,
	Reverse = 2,
}CONTROLER;

typedef struct
{
	uint8_t LED_R;
	uint8_t LED_G;
	uint8_t LED_B;
	uint8_t LED_OUT;
	uint8_t BEEP;
	uint8_t heat_sensitive_sensor;
	uint8_t photosensitive_sensor;
	uint8_t reflective_sensor;
}Init_List;

static Init_List init_list = {0};

static GPIO_InitTypeDef gpio_initstruct = {0};//默认gpio初始化结构体

void gpio_led_init(LEDS led);//用于初始化选择的led灯的gpio口，输入参数为LEDS枚举。
void gpio_led_control(LEDS led, CONTROLER status);//控制led灯的行为，输入参数为LEDS枚举与CONTROLER行为枚举。

void gpio_beep_init(void);//蜂鸣器初始化。
void gpio_beep_control(CONTROLER status);//控制蜂鸣器行为，输入参数为CONTROLER行为枚举。

void gpio_heat_sensitive_sensor_init(void);//热敏传感器初始化。
uint8_t Heat_Scan(void);//热敏检测，检测到过热返回1，否则返回0。

void gpio_photosensitive_sensor_init(void);//光敏传感器初始化。
uint8_t photosensitive_Scan(void);//光敏检测，检测到光返回1，否则返回0。

void gpio_reflective_sensor_init(void);//反射传感器初始化。
uint8_t reflective_Scan(void);//距离检测，检测到物体返回1，否则返回0。

#endif //__GPIO_H
