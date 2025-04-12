#ifndef __BSP_SOFTI2C_H
#define	__BSP_SOFTI2C_H

#include "stm32f10x.h"

/* 定义 SOFT_IIC 连接的GPIO端口, 用户只需要修改下面的代码即可改变控制的 SOFT_IIC 引脚 */

#define SOFT_IIC_SCL_GPIO_PORT    			    GPIOB			                /* 对应GPIO端口 */
#define SOFT_IIC_SCL_GPIO_CLK_PORT 	            RCC_APB2Periph_GPIOB			/* 对应GPIO端口时钟位 */
#define SOFT_IIC_SCL_GPIO_PIN			        GPIO_Pin_6	       				/* 对应PIN脚 */

#define SOFT_IIC_SDA_GPIO_PORT    			    GPIOB			                /* 对应GPIO端口 */
#define SOFT_IIC_SDA_GPIO_CLK_PORT 	            RCC_APB2Periph_GPIOB			/* 对应GPIO端口时钟位 */
#define SOFT_IIC_SDA_GPIO_PIN			        GPIO_Pin_7	       				/* 对应PIN脚 */


#define SOFT_IIC_SDA_IN()           GPIO_ReadInputDataBit(SOFT_IIC_SDA_GPIO_PORT,SOFT_IIC_SDA_GPIO_PIN)  

#define SOFT_IIC_SCL_OUT(VALUE)     if(VALUE)    GPIO_SetBits(SOFT_IIC_SCL_GPIO_PORT,SOFT_IIC_SCL_GPIO_PIN);\
                                    else       GPIO_ResetBits(SOFT_IIC_SCL_GPIO_PORT,SOFT_IIC_SCL_GPIO_PIN);

#define SOFT_IIC_SDA_OUT(VALUE)     if(VALUE)    GPIO_SetBits(SOFT_IIC_SDA_GPIO_PORT,SOFT_IIC_SDA_GPIO_PIN);\
                                    else       GPIO_ResetBits(SOFT_IIC_SDA_GPIO_PORT,SOFT_IIC_SDA_GPIO_PIN); 

#define SOFT_IIC_DATA_READ          SOFT_IIC_SDA_IN() 

static uint8_t soft_init_flag = 0; 

typedef enum
{
    SOFT_IIC_ACK = 0,
    SOFT_IIC_NACK = 1,
}SOFT_IIC_Respond_TypeDef;
//用作反映i2c外设是否有解释道信息。

typedef enum
{
    SOFT_IIC_WRITE = 0,
    SOFT_IIC_READ = 1
}SOFT_IIC_Direction_TypeDef;
//写入模式与读取模式设置枚举。

void SOFT_IIC_GPIO_Config(void);
//初始化GPIO引脚，设置一个SCL和一个SDL。

void SOFT_IIC_Init(void);
//调用上面的函数进行初始化。

void SOFT_IIC_DELAY_US(uint32_t time);
//调用DWT是延迟作为自己的延迟函数。

void SOFT_IIC_Start(void);
//发送下降沿作为信号开始发送的标志。

void SOFT_IIC_Stop(void);
//发送上升沿作为信号发送结束的标志。

uint8_t SOFT_IIC_WaitAck(void);
//每发送完1个字节数据后，调用该函数，检查外设是否接受到数据。接收到数据返回0，否则返回1。

ErrorStatus SOFT_IIC_SendData(uint8_t data);
//发送一个字节。自动调用起始函数和终止函数。发送成功返回SUCCESS，否则返回ERROR。

ErrorStatus SOFT_IIC_AddressMatching(uint8_t slave_addr,SOFT_IIC_Direction_TypeDef direction);
//调用上面字节发送函数，发送一个字节包括外设地址与读写位。0为读，1为写。发送成功返回SUCCESS，否则返回ERROR。

ErrorStatus SOFT_IIC_CheckDevice(uint8_t slave_addr);//调用上面地址匹配函数检查对应地址的外设是否存在（连接成功）。存在（连接成功）成功返回SUCCESS，否则返回ERROR。

#endif /* __BSP_SOFTI2C_H  */
