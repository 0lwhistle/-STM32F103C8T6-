#ifndef __BSP_SPI_H
#define	__BSP_SPI_H

#include "stm32f10x.h"

/* 定义 HARD_SPI 连接的GPIO端口, 用户只需要修改下面的代码即可改变控制的 HARD_SPI 引脚 */

#define SPI_NUM 1

#define SPI_BUFFER_SIZE 512                     // 你的数据缓冲区大小

#if (SPI_NUM == 1)

    #define SPI_SPIX                            SPI1
    #define SPI_SPIX_CLK_PORT                   RCC_APB2Periph_SPI1
    #define SPI_APBXCLKCMD   			        RCC_APB2PeriphClockCmd	        /* 对应SPI外设时钟 */

    #define SPI_CS_GPIO_PORT    			    GPIOA			                /* 对应GPIO端口 */
    #define SPI_CS_GPIO_CLK_PORT 	            RCC_APB2Periph_GPIOA			/* 对应GPIO端口时钟位 */
    #define SPI_CS_GPIO_PIN			            GPIO_Pin_4	       				/* 对应PIN脚 */
            
    #define SPI_SCK_GPIO_PORT    			    GPIOA			                /* 对应GPIO端口 */
    #define SPI_SCK_GPIO_CLK_PORT 	            RCC_APB2Periph_GPIOA			/* 对应GPIO端口时钟位 */
    #define SPI_SCK_GPIO_PIN			        GPIO_Pin_5	       				/* 对应PIN脚 */
    
    #define SPI_MISO_GPIO_PORT    			    GPIOA			                /* 对应GPIO端口 */
    #define SPI_MISO_GPIO_CLK_PORT 	            RCC_APB2Periph_GPIOA			/* 对应GPIO端口时钟位 */
    #define SPI_MISO_GPIO_PIN			        GPIO_Pin_6	       				/* 对应PIN脚 */
    
    #define SPI_MOSI_GPIO_PORT    			    GPIOA			                /* 对应GPIO端口 */
    #define SPI_MOSI_GPIO_CLK_PORT 	            RCC_APB2Periph_GPIOA			/* 对应GPIO端口时钟位 */
    #define SPI_MOSI_GPIO_PIN			        GPIO_Pin_7	       				/* 对应PIN脚 */

#endif

#define SPI_CS_ENABLE()       GPIO_ResetBits(SPI_CS_GPIO_PORT, SPI_CS_GPIO_PIN);
#define SPI_CS_DISABLE()        GPIO_SetBits(SPI_CS_GPIO_PORT, SPI_CS_GPIO_PIN); 

void SPI_PinConfig(void);
void SPI_Mode_Config(void);
void My_SPI_Init(void);
uint8_t SPI_TransferData(uint8_t data);


#endif /* __BSP_SPI_H  */
                                        
