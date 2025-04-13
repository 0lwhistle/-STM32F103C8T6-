#include "usart.h"

int fputc(int ch, FILE* f){//printf()函数的使用前重载
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
	
	USART_SendData(USART1, (uint8_t) ch);
	
	return ch;
}

int fgetc(FILE *f)
{
		while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(USART1);
}


void My_USART_SendBytes(USART_TypeDef *USARTx, uint8_t *pData, uint16_t Size){
	for(uint32_t i = 0; i < Size; ++i){
		while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);//判断输出寄存器是否为空
		
		USART_SendData(USARTx, pData[i]);
	}
	while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);//判断是否传输完成
}

void My_USART_SendByte(USART_TypeDef *USARTx, uint8_t pData){
	
		while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);//判断输出寄存器是否为空
	
		USART_SendData(USARTx, pData);
	
		while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);//判断是否传输完成
}

void usart_init(uint32_t rcc_gpio, GPIO_TypeDef* gpio_port, uint32_t rcc_usartx, USART_TypeDef* usartx, uint16_t tx_pin, uint16_t tr_pin)
{
	RCC_APB2PeriphClockCmd(rcc_gpio, ENABLE);
	GPIO_InitTypeDef GI = {0};
	
	GI.GPIO_Pin = tx_pin;
	GI.GPIO_Mode = GPIO_Mode_AF_PP;
	GI.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(gpio_port, &GI);
	
	GI.GPIO_Pin = tr_pin;
	GI.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(gpio_port, &GI);
	
	RCC_APB2PeriphClockCmd(rcc_usartx, ENABLE);//打开USART1时钟
	
	USART_InitTypeDef UI = {0};//输入初始化信息
	
	UI.USART_BaudRate = 115200;
	UI.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	UI.USART_WordLength = USART_WordLength_8b;
	UI.USART_Parity = USART_Parity_No;
	UI.USART_StopBits = USART_StopBits_1;
	USART_Init(usartx, &UI);//初始化
	USART_Cmd(usartx, ENABLE);//打开时钟连接分频器的总开关
}

