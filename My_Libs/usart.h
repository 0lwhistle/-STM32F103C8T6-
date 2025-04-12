#ifndef __USART_H
#define __USART_H

#include "stm32f10x.h"
#include <stdio.h>

#define usart_init_default(void) usart_init(RCC_APB2Periph_GPIOA, GPIOA, RCC_APB2Periph_USART1, USART1, GPIO_Pin_9, GPIO_Pin_10)
//如无特殊要求，可直接复制上面的代码进行初始化。
void My_USART_SendBytes(USART_TypeDef *USARTx, uint8_t *pData, uint16_t Size);


int fputc(int ch, FILE* f);


void usart_init(uint32_t rcc_gpio, GPIO_TypeDef* gpio_port, uint32_t rcc_usartx, USART_TypeDef* usartx, uint16_t tx_pin, uint16_t tr_pin);

//while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);
//uint8_t recv = USART_ReceiveData(USART1);
//接受信息用这两个

//输出打印信息直接printf()

#endif //__USART_H
