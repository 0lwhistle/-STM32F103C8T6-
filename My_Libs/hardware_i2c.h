#ifndef __BSP_I2C_H
#define	__BSP_I2C_H

#include "stm32f10x.h"

/* 定义 IIC 连接的GPIO端口, 用户只需要修改下面的代码即可改变控制的 IIC 引脚 */

#define IIC_NUM 1//硬件支持I2C一般只支持两对GPIO引脚，灵活性不及软件I2C，但也由于硬件支持性能相对更好。

#if (IIC_NUM == 1)

    #define IIC_I2CX                            I2C1
    #define IIC_I2CX_CLK_PORT                   RCC_APB1Periph_I2C1
    
    #define IIC_SCL_GPIO_PORT    			    GPIOB			                /* 对应GPIO端口 */
    #define IIC_SCL_GPIO_CLK_PORT 	            RCC_APB2Periph_GPIOB			/* 对应GPIO端口时钟位 */
    #define IIC_SCL_GPIO_PIN			        GPIO_Pin_6	       				/* 对应PIN脚 */

    #define IIC_SDA_GPIO_PORT    			    GPIOB			                /* 对应GPIO端口 */
    #define IIC_SDA_GPIO_CLK_PORT 	            RCC_APB2Periph_GPIOB			/* 对应GPIO端口时钟位 */
    #define IIC_SDA_GPIO_PIN			        GPIO_Pin_7	       				/* 对应PIN脚 */

#elif (IIC_NUM == 2)

    #define IIC_I2CX                            I2C2
    #define IIC_I2CX_CLK_PORT                   RCC_APB1Periph_I2C2
    
    #define IIC_SCL_GPIO_PORT    			    GPIOB			                /* 对应GPIO端口 */
    #define IIC_SCL_GPIO_CLK_PORT 	            RCC_APB2Periph_GPIOB			/* 对应GPIO端口时钟位 */
    #define IIC_SCL_GPIO_PIN			        GPIO_Pin_10	       				/* 对应PIN脚 */

    #define IIC_SDA_GPIO_PORT    			    GPIOB			                /* 对应GPIO端口 */
    #define IIC_SDA_GPIO_CLK_PORT 	            RCC_APB2Periph_GPIOB			/* 对应GPIO端口时钟位 */
    #define IIC_SDA_GPIO_PIN			        GPIO_Pin_11	       				/* 对应PIN脚 */

#endif

#define IIC_SDA_IN()           GPIO_ReadInputDataBit(IIC_SDA_GPIO_PORT,IIC_SDA_GPIO_PIN)  

#define IIC_SCL_OUT(VALUE)     if(VALUE)    GPIO_SetBits(IIC_SCL_GPIO_PORT,IIC_SCL_GPIO_PIN);\
                               else       GPIO_ResetBits(IIC_SCL_GPIO_PORT,IIC_SCL_GPIO_PIN);

#define IIC_SDA_OUT(VALUE)     if(VALUE)    GPIO_SetBits(IIC_SDA_GPIO_PORT,IIC_SDA_GPIO_PIN);\
                               else       GPIO_ResetBits(IIC_SDA_GPIO_PORT,IIC_SDA_GPIO_PIN); 

#define IIC_DATA_READ          IIC_SDA_IN() 

//#define IIC_SCL_OUT(VALUE)     (VALUE) ? GPIO_SetBits(IIC_SCL_GPIO_PORT,IIC_SCL_GPIO_PIN):GPIO_ResetBits(IIC_SCL_GPIO_PORT,IIC_SCL_GPIO_PIN)
//#define IIC_SDA_OUT(VALUE)     (VALUE) ? GPIO_SetBits(IIC_SDA_GPIO_PORT,IIC_SDA_GPIO_PIN):GPIO_ResetBits(IIC_SDA_GPIO_PORT,IIC_SDA_GPIO_PIN)
//上面宏定义的三元表达式写法。

/* 这个是stm32主机地址，只要与外接的I2C外设地址不一样即可 */
#define IIC_I2CX_OWN_ADDRESS7      0x0A  

/* IIC 速度模式（标准模式） */  
#define IIC_SPEED              400000

/*等待超时时间，单位us*/
#define CHECK_TIMES         10

static uint8_t i2c_init_flag = 0;//检查是否初始化。

typedef enum
{
    IIC_WRITE = 0,
    IIC_READ = 1
}IIC_Direction_TypeDef;
//读写枚举。

void IIC_PinConfig(void);
//初始化对应的GPIO引脚，设置SCL和SDL。

void IIC_Mode_Config(void);
//初始化I2C时钟与属性（模式、占空比、主机地址、ACK使能、应答地址大小、传输速度）。

void IIC_Init(void);
//调用上面两个函数，对I2C进行初始化，并打开I2C总开关。

void IIC_DELAY_US(uint32_t time);
//调用DWT的us级延迟作为自己的延迟函数，输入延迟时间。

ErrorStatus IIC_Start(void);
//发送起始信号。发送前检查总线是否繁忙（1us检查一次，检查10次），繁忙返回ERROR。发送后检查主机是否成功控制总线，失败返回ERROR。全部成功返回SUCCESS。

void IIC_Stop(void);
//发送上升沿结束信号。

ErrorStatus IIC_AddressMatching(uint8_t slave_addr,IIC_Direction_TypeDef direction);
//检查I2C外设是否存在（连接成功），不存在（连接失败）返回ERROR，反之返回SUCCESS。输入外设地址与读写模式，0为写，1为读。

ErrorStatus IIC_CheckDevice(uint8_t slave_addr);
//检查I2C外设是否存在（连接成功），不存在（连接失败）返回ERROR，反之返回SUCCESS。输入外设地址。

ErrorStatus IIC_SendData(uint8_t data);
#endif /* __BSP_I2C_H  */
                                        
