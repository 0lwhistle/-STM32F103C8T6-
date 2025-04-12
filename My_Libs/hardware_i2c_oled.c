#include "HARDWARE_i2c_oled.h"
#include "HARDWARE_i2c.h"
#include "usart.h"
#include "string.h"
#include "font.h"

/**
  * @brief  检测I2C总线OLED从设备
  * @param  slave_addr：从设备地址
  * @retval SUCCESS 存在   ERROR：不存在
  */
ErrorStatus HARDWARE_OLED_CheckDevice(uint8_t slave_addr)
{
		if(!i2c_init_flag) IIC_Init();
    uint8_t i = 100;
    uint8_t success_num = 0;
    uint8_t error_num = 0;
    uint8_t error_num_temp = 0;
    
    while(i--)
    {
        if(IIC_CheckDevice(OLED_SLAVER_ARRD) == SUCCESS)
        {
            success_num++;
        }
        else
        {
            error_num++;
        }
    }
    
    if(IIC_CheckDevice(OLED_SLAVER_ARRD+5) != SUCCESS)
    {
        error_num_temp++;
    }
    
    if((success_num > 90) && (error_num_temp == 1))
    {
        printf("通信测试正常");
        return SUCCESS;
    }
    else
    {
        printf("通信测试失败");
        return ERROR;
    }   
}

/**
  * @brief  向OLED 单指令/数据写入操作
  *	@param	cmd： 写指令/数据模式选择
  *	@param	byte：具体指令/数据
  * @retval SUCCESS 写入成功   ERROR：写入失败
  */
ErrorStatus HARDWARE_OLED_WriteByte(uint8_t cmd,uint8_t byte)
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
    temp = IIC_AddressMatching(OLED_SLAVER_ARRD,IIC_WRITE);
    if(temp != SUCCESS)
    {
        printf("地址失败");
        
        /* 释放总线并发出停止信号 */
        IIC_Stop();
        return temp;
    }
    
    /* 写指令/数据*/
    temp = IIC_SendData(cmd);
    if(temp != SUCCESS)
    {
        printf("写指令/数据失败");
        
        /* 释放总线并发出停止信号 */
        IIC_Stop();
        return temp;
    }
    
    /* 具体指令/数据*/
    temp = IIC_SendData(byte);
    if(temp != SUCCESS)
    {
        printf("具体指令/数据失败");
        /* 释放总线并发出停止信号 */
        IIC_Stop();
        return temp;
    }
    
    /* 释放总线并发出停止信号 */
    IIC_Stop();
    return SUCCESS;  
}

/**
  * @brief  向OLED 多指令/数据写入操作
  *	@param	cmd：   写指令/数据模式选择
  *         buffer：缓冲区指针
  *		    num：   操作字节数
  * @retval SUCCESS 写入成功   ERROR：写入失败
  */
ErrorStatus HARDWARE_OLED_WriteBuffer(uint8_t cmd,uint8_t* buffer,uint32_t num)
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
    temp = IIC_AddressMatching(OLED_SLAVER_ARRD,IIC_WRITE);
    if(temp != SUCCESS)
    {
        printf("地址失败");
        
        /* 释放总线并发出停止信号 */
        IIC_Stop();
        return temp;
    }
    
    /* 写指令/数据*/
    temp = IIC_SendData(cmd);
    if(temp != SUCCESS)
    {
        printf("写指令/数据失败");
        
        /* 释放总线并发出停止信号 */
        IIC_Stop();
        return temp;
    }
 
    /* 具体指令/数据 */
    for(uint32_t i = 0;i<num;i++)
    {
        temp = IIC_SendData(*buffer++);
        if(temp != SUCCESS)
        {
            printf("具体指令/数据失败");
            return ERROR;
        }
    }
    
    /* 释放总线并发出停止信号 */
    IIC_Stop();
    return SUCCESS;  
}

/**
  * @brief  设置光标
  * @param  y：光标y位置以左上角为原点，向下方向的坐标，范围：0~7
  *         x：光标x位置以左上角为原点，向右方向的坐标，范围：0~127
  * @retval 无
  */
void HARDWARE_OLED_SetPos(uint8_t y,uint8_t x) //设置起始点像素点坐标
{ 
    uint8_t data_buffer_temp[] =  {0xB0+y,((x&0xF0)>>4)|0x10,(x&0x0F)|0x00};//{设置Y位置,设置X位置高4位,设置X位置低4位}
    
    HARDWARE_OLED_WriteBuffer(OLED_WR_CMD,data_buffer_temp,OLED_ARRAY_SIZE(data_buffer_temp));
}



/**
  * @brief  填充整个屏幕
  * @param  fill_data:要填充的数据
  * @retval 无
  */
void HARDWARE_OLED_Fill(uint8_t fill_data)
{
    uint8_t data_buffer_temp[128] = {0};
		memset(data_buffer_temp, fill_data, 128);

    for(uint8_t m=0;m<8;m++)
	{
        HARDWARE_OLED_SetPos(m,0);
        HARDWARE_OLED_WriteBuffer(OLED_WR_DATA,data_buffer_temp,OLED_ARRAY_SIZE(data_buffer_temp));
    }
    
}


/**
  * @brief  初始化OLED
  * @param  无
  * @retval 无
  */
void HARDWARE_OLED_Init(T_ turn)
{
		if(!i2c_init_flag) IIC_Init();
    IIC_DELAY_US(1000000); // 1s,这里的延时很重要,上电后延时，没有错误的冗余设计
    
    while(HARDWARE_OLED_CheckDevice(OLED_SLAVER_ARRD) == ERROR);
    
    /* 控制显示 */
    HARDWARE_OLED_WriteByte(OLED_WR_CMD,0xAE);//设置显示打开/关闭(AFh/AEh)
    
    /* 控制内存寻址模式 */
    HARDWARE_OLED_WriteByte(OLED_WR_CMD,0x20);//设置内存寻址模式(20h)
    HARDWARE_OLED_WriteByte(OLED_WR_CMD,0x02);//00b，水平寻址模式;01b，垂直寻址模式;10b，页面寻址模式(RESET);11，无效
    
    /* 页起始地址 */ 
    HARDWARE_OLED_WriteByte(OLED_WR_CMD,0xB0);//设置页面寻址模式的页面起始地址，0-7(B0-B7)(PAGE0-PAGE7)

    /* COM输出扫描方向 */
    HARDWARE_OLED_WriteByte(OLED_WR_CMD,0xA1);//设置左右方向，0xA1正常 0xA0左右反置
		HARDWARE_OLED_WriteByte(OLED_WR_CMD,0xC8);//设置上下方向，0xC8正常 0xC0上下反置
    
    /* 页内列起始地址 */
		HARDWARE_OLED_WriteByte(OLED_WR_CMD,0x00);//设置列地址低位0-7
		HARDWARE_OLED_WriteByte(OLED_WR_CMD,0x10);//设置列地址高位0-F(10h-1Fh) 列序号=列地址低位*列地址高位(最高位不参与乘积)
    
    /* 页内行起始地址 */
		HARDWARE_OLED_WriteByte(OLED_WR_CMD,0x40);//设置起始行地址
    
    /* 设置对比度 */
		HARDWARE_OLED_WriteByte(OLED_WR_CMD,0x81);//设置对比度控制寄存器(81h)
		HARDWARE_OLED_WriteByte(OLED_WR_CMD,0xff);//0x00~0xff
    
    /* 设置显示方向 */
		HARDWARE_OLED_WriteByte(OLED_WR_CMD,0xA1);//将列地址0映射到SEG0(A0h)/将列地址127映射到SEG0(A1h)
		HARDWARE_OLED_WriteByte(OLED_WR_CMD,0xA6);//设置正常显示(A6h)/倒转显示(A7h)
    
    /* 设置多路复用率 */
		HARDWARE_OLED_WriteByte(OLED_WR_CMD,0xA8);//多路复用率(A8h)
		HARDWARE_OLED_WriteByte(OLED_WR_CMD,0x3F);//(1 ~ 64)
    
    /* 全屏显示 */
		HARDWARE_OLED_WriteByte(OLED_WR_CMD,0xA4);//设置整个显示打开/关闭(A4恢复到RAM内容显示,输出遵循RAM内容/A5全屏显示,输出忽略RAM内容)
    
    /*设置显示偏移量*/
		HARDWARE_OLED_WriteByte(OLED_WR_CMD,0xd3);//设置显示偏移量(D3h)
		HARDWARE_OLED_WriteByte(OLED_WR_CMD,0x00);
    
    /* 设置显示时钟分频比/振荡器频率 */
		HARDWARE_OLED_WriteByte(OLED_WR_CMD,0xD5);//设置显示时钟分频比/振荡器频率
		HARDWARE_OLED_WriteByte(OLED_WR_CMD,0xf0);//设定分割比
    
    /* 设置预充期 */
		HARDWARE_OLED_WriteByte(OLED_WR_CMD,0xD9);//设置预充期
		HARDWARE_OLED_WriteByte(OLED_WR_CMD,0x22);
    
    /* 设置com引脚硬件配置 */
		HARDWARE_OLED_WriteByte(OLED_WR_CMD,0xDA);//设置com引脚硬件配置
		HARDWARE_OLED_WriteByte(OLED_WR_CMD,0x12);
    
    /* 设置VCOMH取消选择级别 */
		HARDWARE_OLED_WriteByte(OLED_WR_CMD,0xDB);//设置VCOMH取消选择级别
		HARDWARE_OLED_WriteByte(OLED_WR_CMD,0x20);//0x20,0.77xVcc
    
		HARDWARE_OLED_WriteByte(OLED_WR_CMD,0x8D);//设置DC-DC使能
		HARDWARE_OLED_WriteByte(OLED_WR_CMD,0x14);
    
    /* 显示打开 */
		HARDWARE_OLED_WriteByte(OLED_WR_CMD,0xAF);//设置显示打开/关闭(AFh/AEh)
    		
		hardward_i2c_oled_init_flag = 1;
		
    HARDWARE_OLED_CLS(turn);
		

}

/**
  * @brief  清屏
  * @param  无
  * @retval 无
  */
void HARDWARE_OLED_CLS(T_ turn)//清屏
{
	if(!hardward_i2c_oled_init_flag) HARDWARE_OLED_Init(HARDWARE_I2C_OLED_CLS_DEFAULT);
	if(turn) HARDWARE_OLED_Fill(0xff);
	else HARDWARE_OLED_Fill(0x00);
}

/**
  * @brief  将OLED从休眠中唤醒
  * @param  无
  * @retval 无
  */
void HARDWARE_OLED_ON(void)
{
    uint8_t data_buffer_temp[] = {0x8D,0x14,0xAF};//{设置电荷泵,开启电荷泵,设置显示打开}
    HARDWARE_OLED_WriteBuffer(OLED_WR_CMD,data_buffer_temp,OLED_ARRAY_SIZE(data_buffer_temp));
}

/**
  * @brief  让OLED休眠 -- 休眠模式下,OLED功耗不到10uA
  * @param  无
  * @retval 无
  */
void HARDWARE_OLED_OFF(void)
{
    uint8_t data_buffer_temp[] = {0x8D,0x10,0xAE};//{设置电荷泵,关闭电荷泵,设置显示关闭}
    HARDWARE_OLED_WriteBuffer(OLED_WR_CMD,data_buffer_temp,OLED_ARRAY_SIZE(data_buffer_temp));
}


/**
  * @brief  OLED_ShowCN，显示的汉字,16*16点阵
  * @param  y：以左上角为原点，向下方向的坐标，范围：0~7   (每次操作8个像素点)
  *         x：以左上角为原点，向右方向的坐标，范围：0~127 (每次操作1个像素点)
  *         n:汉字在的索引
  *         data_cn 中文LIB
  *					
  * @retval 无
  */
void HARDWARE_OLED_ShowChinese(uint8_t y,uint8_t x,uint8_t n,uint8_t *data_cn)
{
		if(!hardward_i2c_oled_init_flag) HARDWARE_OLED_Init(HARDWARE_I2C_OLED_CLS_DEFAULT);
    uint32_t addr=32*n;
    
    HARDWARE_OLED_SetPos(y,x);
    for(uint8_t i=0;i<16;i++)
    {
        HARDWARE_OLED_WriteByte(OLED_WR_DATA,data_cn[addr]);
        addr += 1;
    }
    
    HARDWARE_OLED_SetPos(y+1,x);
    for(uint8_t i=0;i<16;i++)
    {
        HARDWARE_OLED_WriteByte(OLED_WR_DATA,data_cn[addr]);
        addr += 1;
    }
}

/**
  * @brief  显示的汉字(大小为F16X16),16*16点阵
  * @param  line：  以左上角为原点，向下方向的坐标，范围：0~3   (每次操作16个像素点)
  *         offset：以左上角为原点，向右方向的坐标，范围：0~7   (每次操作16个像素点)
  *         n:汉字在 chinese_library_16x16 中文LIB的索引
  *         data_cn 中文LIB	
  * @retval 无
  */
void HARDWARE_OLED_ShowChinese_F16X16(uint8_t line,uint8_t offset,uint8_t n)
{
    HARDWARE_OLED_ShowChinese(line*TEXTSIZE_F16X16/8,offset*TEXTSIZE_F16X16, n,(uint8_t *)chinese_library_16x16);
}

/**
  * @brief  OLED显示一个字符
  * @param  y：以左上角为原点，向下方向的坐标，范围：0~7   (每次操作8个像素点)
  *         x：以左上角为原点，向右方向的坐标，范围：0~127 (每次操作1个像素点)
  *         char_data 要显示的一个字符，范围：ASCII可见字符
  *         textsize : 字符字体大小
  * @retval 无
  */
void HARDWARE_OLED_ShowChar(uint8_t y, uint8_t x, uint8_t char_data,uint8_t textsize)
{
		if(!hardward_i2c_oled_init_flag) HARDWARE_OLED_Init(HARDWARE_I2C_OLED_CLS_DEFAULT);
    uint32_t addr = char_data-32;
    
    switch(textsize)
    {
        case TEXTSIZE_F6X8:
            HARDWARE_OLED_SetPos(y,x);
            for(uint8_t i=0;i<6;i++)
            {
                HARDWARE_OLED_WriteByte(OLED_WR_DATA,(uint8_t)ascll_code_6x8[addr][i]);
            }
            break;
        case TEXTSIZE_F8X16:
            HARDWARE_OLED_SetPos(y,x);
            for(uint8_t i=0;i<8;i++)
            {
                HARDWARE_OLED_WriteByte(OLED_WR_DATA,(uint8_t)ascll_code_8x16[addr][i]);
            }
            HARDWARE_OLED_SetPos(y+1,x);
            for(uint8_t i=0;i<8;i++)
            {
                HARDWARE_OLED_WriteByte(OLED_WR_DATA,(uint8_t)ascll_code_8x16[addr][i+8]);
            }
            break;
        default:
            break;
    }
}

/**
  * @brief  OLED显示字符串
  * @param  y：以左上角为原点，向下方向的坐标，范围：0~7   (每次操作8个像素点)
  *         x：以左上角为原点，向右方向的坐标，范围：0~127 (每次操作1个像素点)
  *         string_data 要显示的字符串，范围：ASCII可见字符
  *         textsize : 字符字体大小
  * @retval 无
  */
void HARDWARE_OLED_ShowString(uint8_t y, uint8_t x, uint8_t *string_data,uint8_t textsize)
{
    for(uint8_t i=0;*string_data !='\0';i++)
    {
        HARDWARE_OLED_ShowChar(y,x+i*textsize,*string_data++,textsize);
    }
}

/**
  * @brief  OLED显示字符串(大小为F8X16)
  * @param  line：  以左上角为原点，向下方向的坐标，范围：0~3   (每次操作16个像素点)
  *         offset：以左上角为原点，向右方向的坐标，范围：0~15 (每次操作8个像素点)
  *         string_data 要显示的字符串，范围：ASCII可见字符
  * @retval 无
  */
void HARDWARE_OLED_ShowString_F8X16(uint8_t line, uint8_t offset, uint8_t *string_data)
{
    HARDWARE_OLED_ShowString(line*TEXTSIZE_F8X16/8*2,offset*TEXTSIZE_F8X16,string_data,TEXTSIZE_F8X16);
}

void HARDWARE_OLED_DrawBitMap(uint8_t x,uint8_t y,uint8_t x_length,uint8_t y_length,uint8_t *raw)
{  
		if(!hardward_i2c_oled_init_flag) HARDWARE_OLED_Init(HARDWARE_I2C_OLED_CLS_DEFAULT);
    uint32_t j=0;
    uint8_t y_page_start;
    uint8_t x_col_start,x_col_end;
    uint32_t raw_size=0;
    
    /*寻找合适的page进行开始*/
    if((y+1)%8 !=0 && y != 0 )
    {
        y = (y/8+1)*8;
    }
    y_page_start =  y/8;
    x_col_start  =  x;
    
    if(y_page_start>7 || x_col_start>127 || y_length>64 || x_length>128)
    {
       return;
    }
    
    /* 计算实际page数 */
    
    if(y_length%8 != 0)
    {
        y_length = y_length/8+1; 
    }
    else
    {
        y_length = y_length/8; 
    }
    
    /* 裁剪出范围内的page数 */
    if(y_page_start+y_length>8)
    {
        y_length = 8-y_page_start;
    }
    
    /* 裁剪出范围内的列数 */
    if(x_col_start+x_length>128)
    {
        x_length = 128-x_col_start;
    }
    
    raw_size = x_length * y_length; 
    
    if(raw_size >= 1024)
    {
        raw_size =  1024; 
    }
    
    x_col_end = x_col_start+x_length;
    
    for(uint8_t y_temp=y_page_start;;y_temp++)
    {
        HARDWARE_OLED_SetPos(y_temp,x_col_start);
        
        for(uint8_t x_temp = x_col_start;x_temp<x_col_end;x_temp++)
        {
            HARDWARE_OLED_WriteByte(OLED_WR_DATA,(uint8_t)raw[j++]);
            if(j == raw_size)
            {
                return;
            }
        }
    }
}

/*********************************************END OF FILE**********************/
