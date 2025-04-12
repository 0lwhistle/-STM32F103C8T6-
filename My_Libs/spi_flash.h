#ifndef __BSP_SPI_FLASH_H
#define	__BSP_SPI_FLASH_H

#include "stm32f10x.h"

//#define  WFLASH_ID              0xEF3015   //W25X16
//#define  WFLASH_ID              0xEF4015	 //W25Q16
//#define  WFLASH_ID              0XEF4018   //W25Q128
#define  WFLASH_ID              0XEF4017    //W25Q64

#define W25Q64_SPI_FLASH_PAGESIZE       256
#define W25Q64_SPI_FLASH_SIZE           (W25Q64_SPI_FLASH_PAGESIZE*8)

#define W25Q64_MANUFACTDEVICEID_ADDRESS	    0x000000 
#define W25Q64_DUMMY_BYTE	                0xFF

/* WIP(busy)标志，FLASH内部正在写入 */
#define WIP_FLAG                  0x01

/********************************命令定义*******************************/

#define W25Q64_WRITE_ENABLE							0x06
#define W25Q64_WRITE_DISABLE						0x04
#define W25Q64_READ_STATUS_REGISTER_1				0x05
#define W25Q64_READ_STATUS_REGISTER_2				0x35
#define W25Q64_WRITE_STATUS_REGISTER				0x01
#define W25Q64_PAGE_PROGRAM							0x02
#define W25Q64_QUAD_PAGE_PROGRAM					0x32
#define W25Q64_BLOCK_ERASE_64KB						0xD8
#define W25Q64_BLOCK_ERASE_32KB						0x52
#define W25Q64_SECTOR_ERASE_4KB						0x20
#define W25Q64_CHIP_ERASE							0xC7
#define W25Q64_ERASE_SUSPEND						0x75
#define W25Q64_ERASE_RESUME							0x7A
#define W25Q64_POWER_DOWN							0xB9
#define W25Q64_HIGH_PERFORMANCE_MODE				0xA3
#define W25Q64_CONTINUOUS_READ_MODE_RESET			0xFF
#define W25Q64_RELEASE_POWER_DOWN_HPM_DEVICE_ID		0xAB
#define W25Q64_MANUFACTURER_DEVICE_ID				0x90
#define W25Q64_READ_UNIQUE_ID						0x4B
#define W25Q64_JEDEC_ID								0x9F
#define W25Q64_READ_DATA							0x03
#define W25Q64_FAST_READ							0x0B
#define W25Q64_FAST_READ_DUAL_OUTPUT				0x3B
#define W25Q64_FAST_READ_DUAL_IO					0xBB
#define W25Q64_FAST_READ_QUAD_OUTPUT				0x6B
#define W25Q64_FAST_READ_QUAD_IO					0xEB
#define W25Q64_OCTAL_WORD_READ_QUAD_IO				0xE3

uint16_t SPI_FLASH_ReadDeviceID(void);
uint32_t SPI_FLASH_JEDEC_ID(void);
void SPI_FLASH_WriteEnable(void);
void SPI_FLASH_WaitForWriteEnd(void);
void SPI_FLASH_PowerDown(void);
void SPI_FLASH_WakeUp(void);
void SPI_FLASH_BulkErase(void);
void SPI_FLASH_SectorErase(uint32_t sector_addr);
void SPI_FLASH_ReadBuffer(uint32_t read_addr, uint8_t* buffer, uint32_t size);
void SPI_FLASH_WritePage(uint32_t write_addr, uint8_t* buffer, uint16_t size);
void SPI_FLASH_WriteBuffer(uint32_t write_addr, uint8_t* buffer, uint32_t size);
#endif /* __BSP_SPI_FLASH_H  */
                                        
