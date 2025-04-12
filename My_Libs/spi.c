#include "spi.h"
#include "dwt.h"  

/**
  * @brief  配置 HARD_SPI 用到的I/O口
  * @param  无  
  * @note   无
  * @retval 无
  */
void SPI_PinConfig(void)
{
    /* 定义一个 GPIO 结构体 */
    GPIO_InitTypeDef gpio_initstruct = {0};
   
#if 1    
    
    /* 开启 HARD_SPI 相关的GPIO外设/端口时钟 */
    RCC_APB2PeriphClockCmd(SPI_CS_GPIO_CLK_PORT,ENABLE);
    
    GPIO_SetBits(SPI_CS_GPIO_PORT,SPI_CS_GPIO_PIN);
    
    /*选择要控制的GPIO引脚、设置GPIO模式为 推挽输出、设置GPIO速率为2MHz*/
    gpio_initstruct.GPIO_Mode   = GPIO_Mode_Out_PP;
    gpio_initstruct.GPIO_Pin    = SPI_CS_GPIO_PIN;
    gpio_initstruct.GPIO_Speed  = GPIO_Speed_2MHz;
    GPIO_Init(SPI_CS_GPIO_PORT,&gpio_initstruct); 

#endif    
    
#if 1 
    
    /* 开启 HARD_SPI 相关的GPIO外设/端口时钟 */
    RCC_APB2PeriphClockCmd(SPI_SCK_GPIO_CLK_PORT,ENABLE);
    
    GPIO_SetBits(SPI_SCK_GPIO_PORT,SPI_SCK_GPIO_PIN);
    
    /*选择要控制的GPIO引脚、设置GPIO模式为 推挽复用输出、设置GPIO速率为2MHz*/
    gpio_initstruct.GPIO_Mode   = GPIO_Mode_AF_PP;
    gpio_initstruct.GPIO_Pin    = SPI_SCK_GPIO_PIN;
    gpio_initstruct.GPIO_Speed  = GPIO_Speed_2MHz;
    GPIO_Init(SPI_SCK_GPIO_PORT,&gpio_initstruct); 
    
#endif 

#if 1 

    /* 开启 HARD_SPI 相关的GPIO外设/端口时钟 */
    RCC_APB2PeriphClockCmd(SPI_MISO_GPIO_CLK_PORT,ENABLE);
    
    GPIO_SetBits(SPI_MISO_GPIO_PORT,SPI_MISO_GPIO_PIN);
        
    /*选择要控制的GPIO引脚、设置GPIO模式为 上拉输入、设置GPIO速率为2MHz*/
    gpio_initstruct.GPIO_Mode   = GPIO_Mode_IPU;
    gpio_initstruct.GPIO_Pin    = SPI_MISO_GPIO_PIN;
    gpio_initstruct.GPIO_Speed  = GPIO_Speed_2MHz;
    GPIO_Init(SPI_MISO_GPIO_PORT,&gpio_initstruct); 
    
#endif 

#if 1 

    /* 开启 HARD_SPI 相关的GPIO外设/端口时钟 */
    RCC_APB2PeriphClockCmd(SPI_MOSI_GPIO_CLK_PORT,ENABLE);
    
    GPIO_SetBits(SPI_MOSI_GPIO_PORT,SPI_MOSI_GPIO_PIN);
    
    /*选择要控制的GPIO引脚、设置GPIO模式为 推挽复用输出、设置GPIO速率为2MHz*/
    gpio_initstruct.GPIO_Mode   = GPIO_Mode_AF_PP;
    gpio_initstruct.GPIO_Pin    = SPI_MOSI_GPIO_PIN;
    gpio_initstruct.GPIO_Speed  = GPIO_Speed_2MHz;
    GPIO_Init(SPI_MOSI_GPIO_PORT,&gpio_initstruct); 
    
#endif     
}

/**
  * @brief  HARD_SPI 工作模式配置
  * @param  无
  * @retval 无
  */
void SPI_Mode_Config(void)
{
    /* 定义一个 HARD_SPI 结构体 */
    SPI_InitTypeDef spi_initstruct = {0};
    
    /* 开启 HARD_SPI 相关的GPIO外设/端口时钟 */
    SPI_APBXCLKCMD(SPI_SPIX_CLK_PORT,ENABLE);
    
    spi_initstruct.SPI_BaudRatePrescaler    = SPI_BaudRatePrescaler_4;         //波特率分频，选择4分频
    spi_initstruct.SPI_CPOL                 = SPI_CPOL_High;				    //SPI极性，选择低极性
		spi_initstruct.SPI_CPHA                 = SPI_CPHA_2Edge;			        //SPI相位，选择第一个时钟边沿采样，极性和相位决定选择SPI模式0
		spi_initstruct.SPI_CRCPolynomial        = 7;				                //CRC多项式，暂时用不到，给默认值7
    spi_initstruct.SPI_DataSize             = SPI_DataSize_8b;		            //数据宽度，选择为8位
    spi_initstruct.SPI_Direction            = SPI_Direction_2Lines_FullDuplex;	//方向，选择2线全双工
    spi_initstruct.SPI_FirstBit             = SPI_FirstBit_MSB;		            //先行位，选择高位先行
    spi_initstruct.SPI_Mode                 = SPI_Mode_Master;			        //模式，选择为SPI主模式
    spi_initstruct.SPI_NSS                  = SPI_NSS_Soft;				        //NSS，选择由软件控制
    
    SPI_Init(SPI_SPIX,&spi_initstruct);//将结构体变量交给SPI_Init，配置SPI_SPIX
}

/**
  * @brief  HARD_SPI 初始化 
  * @param  无
  * @retval 无
  */
void My_SPI_Init(void)
{
    /* 对应的 GPIO 的配置 */
    SPI_PinConfig();
    
    /* 对应的配置模式 */
    SPI_Mode_Config();
    
    /* 使能 HARD_SPI */
    SPI_Cmd(SPI_SPIX,ENABLE);
}

/**
  * @brief  SPI 全双工模式传输一个字节
  * @param  data: 要传输的字节
  * @retval 接收的字节数据(若没有接受数据，可以不处理返回的数据)
  */
uint8_t SPI_TransferData(uint8_t data)
{
	while (SPI_I2S_GetFlagStatus(SPI_SPIX, SPI_I2S_FLAG_TXE) == RESET);	//等待发送数据寄存器空
	
	SPI_I2S_SendData(SPI_SPIX, data);//写入数据到发送数据寄存器，开始产生时序
    
    while(SPI_I2S_GetFlagStatus(SPI_SPIX,SPI_I2S_FLAG_TXE) == RESET);
	
	while (SPI_I2S_GetFlagStatus(SPI_SPIX, SPI_I2S_FLAG_RXNE) == RESET);//等待接收数据寄存器非空
	
	return SPI_I2S_ReceiveData(SPI_SPIX);//读取接收到的数据并返回
}

/*********************************************END OF FILE**********************/
