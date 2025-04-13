#include "dhtll.h"

void Dhtll_GPIO_Config(void)
{    
		GPIO_InitTypeDef gpio_initstruct = {0};
    /* 开启 DHT11 相关的GPIO外设/端口时钟 */
    RCC_APB2PeriphClockCmd(DHT11_DATA_GPIO_CLK_PORT,ENABLE);
    
    /*选择要控制的GPIO引脚、设置GPIO模式为 上拉输入、设置GPIO速率为50MHz*/
    gpio_initstruct.GPIO_Pin    = DHT11_DATA_GPIO_PIN;
    gpio_initstruct.GPIO_Mode   = GPIO_Mode_IPD;
    gpio_initstruct.GPIO_Speed  = GPIO_Speed_50MHz;
    GPIO_Init(DHT11_DATA_GPIO_PORT,&gpio_initstruct);
}

void DHTll_DataPinModeConfig(GPIOMode_TypeDef mode)
{
		GPIO_InitTypeDef gpio_initstruct = {0};
		gpio_initstruct.GPIO_Pin    = DHT11_DATA_GPIO_PIN;
    gpio_initstruct.GPIO_Mode = mode;
		gpio_initstruct.GPIO_Speed  = GPIO_Speed_50MHz;
    GPIO_Init(DHT11_DATA_GPIO_PORT,&gpio_initstruct);
}

uint8_t DHT11_ReadByte(void)
{
	uint8_t dhtll_readbyte_temp = 0;
	
	for(uint8_t i = 0; i < 8; ++i)
	{
		while(DHTll_DATA_IN() == Bit_RESET);
		
		DWT_DelayUs(40);
		if(DHTll_DATA_IN() == Bit_RESET)
		{
			dhtll_readbyte_temp &= (uint8_t) (~(0x1<<(7-i)));
		}
		
		else 
		{
			while(DHTll_DATA_IN() == Bit_SET);
			dhtll_readbyte_temp |= (uint8_t) (0x1<<(7-i));
		}
	}
	
	return dhtll_readbyte_temp;
}

ErrorStatus DHTll_ReadData(DHTll_DATA_TYPEDEF* dhtll_data)
{
	uint8_t count_timer_temp = 0;
	
	DHTll_DataPinModeConfig(GPIO_Mode_Out_OD);
	
	DHTll_DATA_OUT(0);
	DWT_DelayMs(20);
	DHTll_DATA_OUT(1);
	
	DHTll_DataPinModeConfig(GPIO_Mode_IPU);
	
	DWT_DelayUs(20);
	
	if(DHTll_DATA_IN() == Bit_RESET)
	{
		count_timer_temp = 0;
		
		while(DHTll_DATA_IN() == Bit_RESET)
		{
			if(count_timer_temp++ > 83) 
			{		
					//printf("3\n");
					return ERROR;
			}
			DWT_DelayUs(1);
		}
		
		count_timer_temp = 0;
		
		while(DHTll_DATA_IN() == Bit_SET)
		{
			if(count_timer_temp++ > 87) 
			{
				//printf("4\n");
				return ERROR;
			}
			DWT_DelayUs(1);
		}
		
		dhtll_data->humi_int = DHT11_ReadByte();        //湿度高8位
		dhtll_data->humi_deci = DHT11_ReadByte();       //湿度低8位
		dhtll_data->temp_int = DHT11_ReadByte();        //温度高8位
		dhtll_data->temp_deci = DHT11_ReadByte();       //温度低8位
		dhtll_data->check_sum = DHT11_ReadByte();       //校验和
		
		DWT_DelayUs(54);
		//printf("%d\n", dhtll_data->check_sum == dhtll_data->humi_int + dhtll_data->humi_deci + dhtll_data->temp_int + dhtll_data->temp_deci);
		if(dhtll_data->check_sum == dhtll_data->humi_int + dhtll_data->humi_deci + dhtll_data->temp_int + dhtll_data->temp_deci)
		{
		return SUCCESS;
		}
		else
		{
			//printf("1\n");
			return ERROR;
		}
	}
	
	else
	{
		//printf("2\n");
		return ERROR;
	}
}

void DHTll_print()
{
	if(DHTll_ReadData(&dhtll_data) == SUCCESS)
			{
				if(dhtll_data.humi_deci&0x80) printf("Humidity -%d.%d ％\r\n",dhtll_data.humi_int,dhtll_data.humi_deci);
				else printf("Humidity %d.%d ％ \n",dhtll_data.humi_int,dhtll_data.humi_deci);
			
				if(dhtll_data.temp_deci&0x80) printf("temperature -%d.%d ％\r\n",dhtll_data.temp_int,dhtll_data.temp_deci);
				else printf("temperature %d.%d ％ \n",dhtll_data.temp_int,dhtll_data.temp_deci);
			}
			
			else printf("READ_DHT11_DATA ERROR!\r\n");
}

ErrorStatus Get_DHTll_DATA(DHTll_DATA_TYPEDEF* dhtll_data_temp)
{
	if(DHTll_ReadData(dhtll_data_temp) == SUCCESS)
	{
		return SUCCESS;
	}
	
	else 
		return ERROR;
}
