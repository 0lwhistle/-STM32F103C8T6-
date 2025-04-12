#include "soft_i2c.h"
#include "dwt.h"  
#include "usart.h"

uint32_t i2c_delay_time = 1;

/**
  * @brief  配置 SOFT_IIC 用到的I/O口
  * @param  无  
  * @note   无
  * @retval 无
  */
void SOFT_IIC_GPIO_Config(void)
{
    /* 定义一个 GPIO 结构体 */
    GPIO_InitTypeDef gpio_initstruct = {0};
   
    /* 开启 SOFT_IIC 相关的GPIO外设/端口时钟 */
    RCC_APB2PeriphClockCmd(SOFT_IIC_SCL_GPIO_CLK_PORT,ENABLE);

    SOFT_IIC_SCL_OUT(1);
    
    /*选择要控制的GPIO引脚、设置GPIO模式为 开漏输出、设置GPIO速率为50MHz*/
    gpio_initstruct.GPIO_Mode   = GPIO_Mode_Out_OD;
    gpio_initstruct.GPIO_Pin    = SOFT_IIC_SCL_GPIO_PIN;
    gpio_initstruct.GPIO_Speed  = GPIO_Speed_50MHz;
    GPIO_Init(SOFT_IIC_SCL_GPIO_PORT,&gpio_initstruct); 
    
    /* 开启 SOFT_IIC 相关的GPIO外设/端口时钟 */
    RCC_APB2PeriphClockCmd(SOFT_IIC_SDA_GPIO_CLK_PORT,ENABLE);

    SOFT_IIC_SDA_OUT(1);
    
    /*选择要控制的GPIO引脚、设置GPIO模式为 开漏输出、设置GPIO速率为50MHz*/
    gpio_initstruct.GPIO_Mode   = GPIO_Mode_Out_OD;
    gpio_initstruct.GPIO_Pin    = SOFT_IIC_SDA_GPIO_PIN;
    gpio_initstruct.GPIO_Speed  = GPIO_Speed_50MHz;
    GPIO_Init(SOFT_IIC_SDA_GPIO_PORT,&gpio_initstruct); 
    
		soft_init_flag = 1;
}

/**
  * @brief  SOFT_IIC 初始化
  * @param  无
  * @retval 无
  */
void SOFT_IIC_Init(void)
{
    /* 对应的GPIO的配置 */
    SOFT_IIC_GPIO_Config();
		usart_init_default();
}

/**
  * @brief  SOFT_IIC 延时接口(单位：us)
  * @param  无
  * @retval 无
  */
void SOFT_IIC_DELAY_US(uint32_t time)
{
    DWT_DelayUs(time);
}

/**
  * @brief  发起 SOFT_IIC 总线开始信号
  * @param  无
  * @retval 无
  */
void SOFT_IIC_Start(void)
{
    /* 当SCL高电平时，SDA出现一个下跳沿表示I2C总线启动信号 */
    SOFT_IIC_SDA_OUT(1);
		SOFT_IIC_SCL_OUT(1);
    SOFT_IIC_DELAY_US(i2c_delay_time);
    SOFT_IIC_SDA_OUT(0);
    SOFT_IIC_DELAY_US(i2c_delay_time);
    SOFT_IIC_SCL_OUT(0);
    SOFT_IIC_DELAY_US(i2c_delay_time);
}

/**
 * @brief  发起 SOFT_IIC 总线停止信号
 * @param  无
 * @retval 无
 */
void SOFT_IIC_Stop(void)
{
		
    /* 当SCL高电平时，SDA出现一个上跳沿表示I2C总线停止信号 */
    SOFT_IIC_SDA_OUT(0);
		SOFT_IIC_SCL_OUT(1);
    SOFT_IIC_DELAY_US(i2c_delay_time);
    SOFT_IIC_SDA_OUT(1);
    SOFT_IIC_DELAY_US(i2c_delay_time);

}

/**
 * @brief  产生一个时钟，并读取器件的ACK应答信号
 * @param  无
 * @retval SOFT_IIC_ACK:应答信号   SOFT_IIC_NACK：非应答信号
 */
uint8_t SOFT_IIC_WaitAck(void)
{
    uint8_t ret = SOFT_IIC_NACK;
    SOFT_IIC_SDA_OUT(1);	        /* 释放SDA总线 */
    SOFT_IIC_DELAY_US(i2c_delay_time);
    
    SOFT_IIC_SCL_OUT(1);	        /* 驱动SCL = 1, 此时器件会返回ACK应答 */
    
    if(SOFT_IIC_DATA_READ == SOFT_IIC_NACK)	        /* 读取SDA口线状态 */
    {
        ret = SOFT_IIC_NACK;
    }
    else
    {
        ret = SOFT_IIC_ACK;
    }
    SOFT_IIC_SCL_OUT(0);
    SOFT_IIC_DELAY_US(i2c_delay_time);
    
    return ret;
}

/**
  * @brief  通过 SOFT_IIC 发送一个数据字节.
  * @param  data: 要传输的字节
  * @retval 无
  */
ErrorStatus SOFT_IIC_SendData(uint8_t data)
{
		if(!soft_init_flag) SOFT_IIC_Init();
    for(uint8_t i = 0; i < 8; i ++)//循环8次，主机依次发送数据的每一位
    {
        SOFT_IIC_SDA_OUT(data&(0x80>>i));	        //使用掩码的方式取出Byte的指定一位数据并写入到SDA线
        SOFT_IIC_DELAY_US(i2c_delay_time);
        SOFT_IIC_SCL_OUT(1);						//释放SCL，从机在SCL高电平期间读取SDA
        SOFT_IIC_DELAY_US(i2c_delay_time);
        SOFT_IIC_SCL_OUT(0);						//拉低SCL，主机开始发送下一位数据
    }
    
    if(SOFT_IIC_WaitAck())
    {
        /* 发送停止条件 */
        SOFT_IIC_Stop();     //方便下次通信使用
        return ERROR;
    }
    return SUCCESS;
}

/**
  * @brief  通过 SOFT_IIC 地址验证.
  * @param  slave_addr: 从设备地址
  * @param  direction: 读写操作(传输方向)
  * @retval 其他:失败 ;   SUCCESS:成功
  */
ErrorStatus SOFT_IIC_AddressMatching(uint8_t slave_addr,SOFT_IIC_Direction_TypeDef direction)
{
    return SOFT_IIC_SendData((slave_addr<<1)|direction);//从设备地址+读\写操作(高7位为：从设备地址,低1位为：读写操作位

}

/**
  * @brief  检测IIC总线从设备
  * @param  slave_addr：从设备地址
  * @retval SUCCESS 检测通过   其他：不通过
  */
ErrorStatus SOFT_IIC_CheckDevice(uint8_t slave_addr)
{
		if(!soft_init_flag) SOFT_IIC_Init();
    ErrorStatus temp = ERROR;
    
    /* 发出开始信号 */
    SOFT_IIC_Start();
    
    /* 呼叫从机,地址配对*/
    temp = SOFT_IIC_AddressMatching(slave_addr,SOFT_IIC_WRITE);
    if(temp != SUCCESS)
    {
        return temp;
    }
    
    /* 释放总线并发出停止信号 */
    SOFT_IIC_Stop();
    return SUCCESS;  
}
/*****************************END OF FILE***************************************/
