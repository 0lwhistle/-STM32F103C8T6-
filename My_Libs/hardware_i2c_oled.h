#ifndef __BSP_I2C_OLED_H
#define	__BSP_I2C_OLED_H

#include "stm32f10x.h"

#define OLED_SLAVER_ARRD   (0x3C)//OLED屏的地址

#define OLED_WR_CMD        (0x00)//指令模式
#define OLED_WR_DATA       (0x40)//数据模式

#define OLED_ARRAY_SIZE(A) sizeof(A)/sizeof(A[0])  
	
#define TEXTSIZE_F6X8       6  
#define TEXTSIZE_F8X16      8  
#define TEXTSIZE_F16X16     16   

#define HARDWARE_I2C_OLED_CLS_DEFAULT OFF

static uint8_t hardward_i2c_oled_init_flag = 0;

typedef enum OLED_CLS_TURN
{
	OFF = 0,
	ON = 1,
}T_;



void HARDWARE_OLED_Init(T_ turn);
ErrorStatus HARDWARE_OLED_CheckDevice(uint8_t slave_addr);
ErrorStatus HARDWARE_OLED_WriteByte(uint8_t cmd,uint8_t byte);
ErrorStatus HARDWARE_OLED_WriteBuffer(uint8_t cmd,uint8_t* buffer,uint32_t num);
void HARDWARE_OLED_SetPos(uint8_t y,uint8_t x);
void HARDWARE_OLED_SetPos(uint8_t y,uint8_t x);
void HARDWARE_OLED_Fill(uint8_t fill_data);
void HARDWARE_OLED_CLS(T_ turn);
void HARDWARE_OLED_ON(void);
void HARDWARE_OLED_OFF(void);

void HARDWARE_OLED_ShowChinese(uint8_t y,uint8_t x,uint8_t n,uint8_t *data_cn);
void HARDWARE_OLED_ShowChinese_F16X16(uint8_t line,uint8_t offset,uint8_t n);
void HARDWARE_OLED_ShowChar(uint8_t y, uint8_t x, uint8_t char_data,uint8_t textsize);
void HARDWARE_OLED_ShowString(uint8_t y, uint8_t x, uint8_t *string_data,uint8_t textsize);
void HARDWARE_OLED_ShowString_F8X16(uint8_t line, uint8_t offset, uint8_t *string_data);
void HARDWARE_OLED_DrawBitMap(uint8_t x,uint8_t y,uint8_t x_length,uint8_t y_length,uint8_t *raw);
#endif /* __BSP_I2C_OLED_H  */
                                        
