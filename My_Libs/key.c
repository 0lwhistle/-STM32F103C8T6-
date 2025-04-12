#include "key.h"

void key_init(KEYx keyx)
{
	switch(keyx)
	{
		case KEY_1:
			RCC_APB2PeriphClockCmd(KEY_1_GPIO_RCC_PORT, ENABLE);
			
			GPIO_InitStruct.GPIO_Pin = KEY_1_PIN;
			GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
			GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
			GPIO_Init(KEY_1_GPIO_PORT, &GPIO_InitStruct);
			KEY1_INIT_FLAG = 1;
			break;
		
		case KEY_2:
			RCC_APB2PeriphClockCmd(KEY_2_GPIO_RCC_PORT, ENABLE);
			
			GPIO_InitStruct.GPIO_Pin = KEY_2_PIN;
			GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
			//GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
			GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
			GPIO_Init(KEY_2_GPIO_PORT, &GPIO_InitStruct);
			KEY2_INIT_FLAG = 1;
			break;
		
		case KEY_3:
			RCC_APB2PeriphClockCmd(KEY_3_GPIO_RCC_PORT, ENABLE);
			
			GPIO_InitStruct.GPIO_Pin = KEY_3_PIN;
			GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
			GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
			GPIO_Init(KEY_3_GPIO_PORT, &GPIO_InitStruct);
			KEY3_INIT_FLAG = 1;
			break;
	}
}

KEY_Status key_scan(KEYx keyx, KEY_TriggerLevel key_pressstatus, Key_Sacn_status key_scan_status)
{
	switch (keyx)
	{
		case KEY_1:
			if(!KEY1_INIT_FLAG) key_init(KEY_1);
			if(GPIO_ReadInputDataBit(KEY_1_GPIO_PORT, KEY_1_PIN) == key_pressstatus)
			{
					My_SysTick_Delay(20);
					if(key_scan_status) while(GPIO_ReadInputDataBit(KEY_1_GPIO_PORT, KEY_1_PIN) == key_pressstatus);
					My_SysTick_Delay(20);
					return KEY_DOWN;
			}
			
			else
			{
					return KEY_UP;
			}
		
		case KEY_2:
			if(!KEY2_INIT_FLAG) key_init(KEY_2);
			if(GPIO_ReadInputDataBit(KEY_2_GPIO_PORT, KEY_2_PIN) == key_pressstatus)
			{
					My_SysTick_Delay(20);
					if(key_scan_status) while(GPIO_ReadInputDataBit(KEY_2_GPIO_PORT, KEY_2_PIN) == key_pressstatus);
					My_SysTick_Delay(20);
					return KEY_DOWN;
			}
			
			else
			{
					return KEY_UP;
			}
		
		case KEY_3:
			if(!KEY3_INIT_FLAG) key_init(KEY_3);
			if(GPIO_ReadInputDataBit(KEY_3_GPIO_PORT, KEY_3_PIN) == key_pressstatus)
			{
					My_SysTick_Delay(20);
					if(key_scan_status) while(GPIO_ReadInputDataBit(KEY_3_GPIO_PORT, KEY_3_PIN) == key_pressstatus);
					My_SysTick_Delay(20);
					return KEY_DOWN;
			}
			
			else
			{
					return KEY_UP;
			}
	}
	
	return KEY_UP;
}
