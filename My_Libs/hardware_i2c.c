#include "hardware_i2c.h"
#include "dwt.h"  
#include "usart.h"

/**
  * @brief  配置 HARD_IIC 用到的I/O口
  * @param  无  
  * @note   无
  * @retval 无
  */
void IIC_PinConfig(void)
{
    /* 定义一个 GPIO 结构体 */
    GPIO_InitTypeDef gpio_initstruct = {0};
   
    /* 开启 HARD_IIC 相关的GPIO外设/端口时钟 */
    RCC_APB2PeriphClockCmd(IIC_SCL_GPIO_CLK_PORT,ENABLE);

    IIC_SCL_OUT(1);
    
    /*选择要控制的GPIO引脚、设置GPIO模式为 开漏复用输出、设置GPIO速率为50MHz*/
    gpio_initstruct.GPIO_Mode   = GPIO_Mode_AF_OD;
    gpio_initstruct.GPIO_Pin    = IIC_SCL_GPIO_PIN;
    gpio_initstruct.GPIO_Speed  = GPIO_Speed_50MHz;
    GPIO_Init(IIC_SCL_GPIO_PORT,&gpio_initstruct); 
    
    /* 开启 IIC 相关的GPIO外设/端口时钟 */
    RCC_APB2PeriphClockCmd(IIC_SDA_GPIO_CLK_PORT,ENABLE);

    IIC_SDA_OUT(1);
    
    /*选择要控制的GPIO引脚、设置GPIO模式为 开漏复用输出、设置GPIO速率为50MHz*/
    gpio_initstruct.GPIO_Mode   = GPIO_Mode_AF_OD;
    gpio_initstruct.GPIO_Pin    = IIC_SDA_GPIO_PIN;
    gpio_initstruct.GPIO_Speed  = GPIO_Speed_50MHz;
    GPIO_Init(IIC_SDA_GPIO_PORT,&gpio_initstruct); 
    
}

/**
  * @brief  HARD_IIC 工作模式配置
  * @param  无
  * @retval 无
  */
void IIC_Mode_Config(void)
{
    /* 定义一个 HARD_IIC 结构体 */
    I2C_InitTypeDef i2c_initstruct = {0};
    
    /* 开启 HARD_IIC 相关的GPIO外设/端口时钟 */
    RCC_APB1PeriphClockCmd(IIC_I2CX_CLK_PORT,ENABLE);
    
    i2c_initstruct.I2C_Mode                 = I2C_Mode_I2C;
    i2c_initstruct.I2C_DutyCycle            = I2C_DutyCycle_2;                 //高电平数据稳定，低电平数据变化 SCL 时钟线的占空比
    i2c_initstruct.I2C_OwnAddress1          = IIC_I2CX_OWN_ADDRESS7;
    i2c_initstruct.I2C_Ack                  = I2C_Ack_Enable;
    i2c_initstruct.I2C_AcknowledgedAddress  = I2C_AcknowledgedAddress_7bit;      
    i2c_initstruct.I2C_ClockSpeed           = IIC_SPEED;                        //通信速率
    
    I2C_Init(IIC_I2CX, &i2c_initstruct);
}

/**
  * @brief  HARD_IIC 初始化
  * @param  无
  * @retval 无
  */
void IIC_Init(void)
{
    /* 对应的 GPIO 的配置 */
    IIC_PinConfig();
    
    /* 对应的配置模式 */
    IIC_Mode_Config();
    
    /* 使能 HARD_IIC */
    I2C_Cmd(IIC_I2CX,ENABLE);
		
		usart_init_default();
	
		i2c_init_flag = 1;
}

/**
  * @brief  HARD_IIC 延时接口(单位：us)
  * @param  无
  * @retval 无
  */
void IIC_DELAY_US(uint32_t time)
{
    DWT_DelayUs(time);
}

/**
  * @brief  发起 HARD_IIC 总线开始信号
  * @param  无
  * @retval ERROR:失败 ;   SUCCESS:成功
  */
ErrorStatus IIC_Start(void)
{
    uint32_t check_times = CHECK_TIMES;
    
    /* 检测总线是否繁忙 */
    while(I2C_GetFlagStatus(IIC_I2CX,I2C_FLAG_BUSY))
    {
        check_times--;
        IIC_DELAY_US(1);
        if(check_times == 0)
        {
            return ERROR;
        }
    }
    
    /* 产生I2C 起始信号*/
    I2C_GenerateSTART(IIC_I2CX,ENABLE);
    check_times = CHECK_TIMES;
    
    while(I2C_CheckEvent(IIC_I2CX,I2C_EVENT_MASTER_MODE_SELECT) == ERROR)
    {
        check_times--;
        IIC_DELAY_US(1);
        if(check_times == 0)
        {
            /* 发送停止信号方便下次通信使用*/
            IIC_Stop();
            return ERROR;
        }
    }
    return SUCCESS;
}

/**
 * @brief  发起 IIC 总线停止信号
 * @param  无
 * @retval 无
 */
void IIC_Stop(void) 
{
    I2C_GenerateSTOP(IIC_I2CX,ENABLE);
}

/**
  * @brief  通过 IIC外设 地址验证
  * @param  slave_addr: 从设备地址
  * @param  direction: 读写操作(传输方向)
  * @retval ERROR:失败 ;   SUCCESS:成功
  */
ErrorStatus IIC_AddressMatching(uint8_t slave_addr,IIC_Direction_TypeDef direction)
{
		if(!i2c_init_flag) IIC_Init();
	
    uint32_t check_times    =  CHECK_TIMES;
    uint32_t event_temp     =  I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED;
    
    if(direction == IIC_WRITE)
    {
        event_temp = I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED;
    }
    else
    {
        event_temp = I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED;
    }

    I2C_Send7bitAddress(IIC_I2CX,(slave_addr<<1)|direction,I2C_Direction_Transmitter);  //本质与I2C_SendData(IIC_I2CX, (slave_addr<<1)|读写操作位)一样

    while(I2C_CheckEvent(IIC_I2CX,event_temp) == ERROR)
    {
        check_times--;
        IIC_DELAY_US(1);
        if(check_times == 0)
        {
            /* 发送停止信号方便下次通信使用*/
            IIC_Stop();
            return ERROR;
        }
    }
    return SUCCESS;
}

/**
  * @brief  检测IIC总线从设备
  * @param  slave_addr：从设备地址
  * @retval SUCCESS 检测通过   其他：不通过
  */
ErrorStatus IIC_CheckDevice(uint8_t slave_addr)
{
		if(!i2c_init_flag) IIC_Init();
	
    ErrorStatus temp = ERROR;
    
    /* 检测总线是否繁忙和发出开始信号*/
    temp = IIC_Start();
    if(temp != SUCCESS)
    {
        return temp;
    }
   
    /* 呼叫从机,地址配对*/
    temp = IIC_AddressMatching(slave_addr,IIC_WRITE);
    if(temp != SUCCESS)
    {
        return temp;
    }
    
    /* 发送停止信号方便下次通信使用*/
    IIC_Stop();
    return SUCCESS;  
}

ErrorStatus IIC_SendData(uint8_t data)
{
    uint32_t check_times = CHECK_TIMES;
    I2C_SendData(IIC_I2CX,data);
    while(I2C_CheckEvent(IIC_I2CX,I2C_EVENT_MASTER_BYTE_TRANSMITTED) == ERROR)
    {
        check_times--;
        IIC_DELAY_US(1);
        if(check_times == 0)
        {
            /* 发送停止信号方便下次通信使用*/
            IIC_Stop();
            return ERROR;
        }
    }
    return SUCCESS;
}


/*********************************************END OF FILE**********************/
