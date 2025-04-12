#ifndef __DHTLL_H
#define __DHTLL_H

#include "dwt.h"
#include "usart.h"

#define DHT11_DATA_GPIO_PORT          GPIOB                           /* GPIO端口 */
#define DHT11_DATA_GPIO_CLK_PORT      RCC_APB2Periph_GPIOB            /* GPIO端口时钟 */
#define DHT11_DATA_GPIO_PIN           GPIO_Pin_12                     /* 对应PIN脚 */


#define DHTll_DATA_IN()         GPIO_ReadInputDataBit(DHT11_DATA_GPIO_PORT,DHT11_DATA_GPIO_PIN)
#define DHTll_DATA_OUT(VALUE)   if(VALUE)   GPIO_SetBits(DHT11_DATA_GPIO_PORT,DHT11_DATA_GPIO_PIN);\
																else      GPIO_ResetBits(DHT11_DATA_GPIO_PORT,DHT11_DATA_GPIO_PIN);
																	
typedef struct
{
	 uint8_t humi_int;        //湿度的整数部分
   uint8_t humi_deci;       //湿度的小数部分
   uint8_t temp_int;        //温度的整数部分
   uint8_t temp_deci;       //温度的小数部分
   uint8_t check_sum;       //校验和                              
}DHTll_DATA_TYPEDEF; 

static DHTll_DATA_TYPEDEF dhtll_data = {0};
																
void Dhtll_GPIO_Config(void);
void DHTll_DataPinModeConfig(GPIOMode_TypeDef mode);
uint8_t DHT11_ReadByte(void);
ErrorStatus DHTll_ReadData(DHTll_DATA_TYPEDEF* dhtll_data);
void DHTll_print(void);

#endif //__DHTLL_H
