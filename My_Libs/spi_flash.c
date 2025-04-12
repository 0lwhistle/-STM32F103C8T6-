#include "spi_flash.h"
#include "spi.h"

/**
  * @brief  读取 Manufacturer/Device ID
  * @param  无  
  * @note   无
  * @retval Manufacturer/Device ID
  */
uint16_t SPI_FLASH_ReadDeviceID(void)
{
    uint16_t m_id = 0;          //定义用于存放 Manufacturer/Device_ID 号的变量
    
    SPI_CS_ENABLE();            //SPI起始
    SPI_TransferData(W25Q64_MANUFACTURER_DEVICE_ID);     //发送读取 Manufacturer/Device ID 的指令                                              
    SPI_TransferData((uint8_t)((W25Q64_MANUFACTDEVICEID_ADDRESS>>8)&0xFF));
    SPI_TransferData((uint8_t)((W25Q64_MANUFACTDEVICEID_ADDRESS>>4)&0xFF));
    SPI_TransferData((uint8_t)((W25Q64_MANUFACTDEVICEID_ADDRESS>>0)&0xFF));
    m_id = SPI_TransferData(W25Q64_DUMMY_BYTE);          //接收 Manufacturer_ID (JEDEC为Winbond分配的制造商ID字节(EFh))，通过输出参数返回
    m_id = m_id<<8|SPI_TransferData(W25Q64_DUMMY_BYTE);  //接收 Device_ID ，通过输出参数返回
    
    SPI_CS_DISABLE();   //SPI终止
    
    return m_id;
}

/**
  * @brief  读取 JEDEC_ID
  * @param  无  
  * @note   无
  * @retval JEDEC_ID
  */
uint32_t SPI_FLASH_JEDEC_ID(void)
{
    uint32_t jedec_id = 0;      //定义用于存放 JEDEC_ID 号的变量
    
    SPI_CS_ENABLE();    //SPI起始

    SPI_TransferData(W25Q64_JEDEC_ID);                  //发送读取 JEDEC_ID 的指令
    jedec_id = SPI_TransferData(W25Q64_DUMMY_BYTE);	    //接收 JEDEC_ID 高8位(JEDEC为Winbond分配的制造商ID字节(EFh))，通过输出参数返回
    jedec_id <<=8;                                      //留出低8位
    jedec_id |= SPI_TransferData(W25Q64_DUMMY_BYTE);	//接收 JEDEC_ID 的中8位（内存类型/容量等信息），通过输出参数返回
    jedec_id <<=8;                                      //留出低8位
    jedec_id |= SPI_TransferData(W25Q64_DUMMY_BYTE);	//接收 JEDEC_ID 的低8位（内存类型/容量等信息），通过输出参数返回
    
    SPI_CS_DISABLE();   //SPI终止
    
    return jedec_id;
}

/**
  * @brief  向FLASH发送 写使能 命令
  * @param  无  
  * @note   无
  * @retval 无
  */
void SPI_FLASH_WriteEnable(void)
{
    SPI_CS_ENABLE();                        //SPI起始
    SPI_TransferData(W25Q64_WRITE_ENABLE);  //发送写使能命令
    SPI_CS_DISABLE();                       //SPI终止
}

/**
  * @brief  等待WIP(BUSY)标志被置0，即等待到FLASH内部数据写入完毕
  * @param  无  
  * @note   无
  * @retval JEDEC_ID
  */
void SPI_FLASH_WaitForWriteEnd(void)
{
    SPI_CS_ENABLE();   //SPI起始
    SPI_TransferData(W25Q64_READ_STATUS_REGISTER_1);                    //发送 读状态寄存器 命令
    while((SPI_TransferData(W25Q64_DUMMY_BYTE)&WIP_FLAG) == SET);       //若FLASH忙碌，则等待
    SPI_CS_DISABLE();  //SPI终止
}

/**
  * @brief  FLASH进入掉电模式
  * @param  无  
  * @note   无
  * @retval JEDEC_ID
  */
void SPI_FLASH_PowerDown(void)
{
    SPI_CS_ENABLE();   //SPI起始
    SPI_TransferData(W25Q64_POWER_DOWN);//发送 掉电 命令
    SPI_CS_DISABLE();  //SPI终止
}

/**
  * @brief  唤醒FLASH
  * @param  无  
  * @note   无
  * @retval JEDEC_ID
  */
void SPI_FLASH_WakeUp(void)
{
    SPI_CS_ENABLE();   //SPI起始
    SPI_TransferData(W25Q64_RELEASE_POWER_DOWN_HPM_DEVICE_ID);//发送 上电 命令
    SPI_CS_DISABLE();  //SPI终止
}

/**
  * @brief  擦除FLASH扇区，整片擦除
  * @param  无  
  * @note   无
  * @retval 无
  */
void SPI_FLASH_BulkErase(void)
{
    SPI_FLASH_WriteEnable();    //发送FLASH写使能命令
    
    SPI_CS_ENABLE();   //SPI起始
    SPI_TransferData(W25Q64_CHIP_ERASE);//发送整块擦除指令
    SPI_CS_DISABLE();  //SPI终止
    
    /* 等待擦除完毕*/
    SPI_FLASH_WaitForWriteEnd();
}

/**
  * @brief  擦除FLASH扇区
  * @param  sector_addr：要擦除的扇区地址 
  * @note   无
  * @retval 无
  */
void SPI_FLASH_SectorErase(uint32_t sector_addr)
{
    SPI_FLASH_WriteEnable();    //发送FLASH写使能命令
    
    SPI_CS_ENABLE();   //SPI起始
    SPI_TransferData(W25Q64_SECTOR_ERASE_4KB);//发送扇区擦除指令
    SPI_TransferData((sector_addr & 0xFF0000) >> 16 );//发送擦除扇区地址的高位
    SPI_TransferData((sector_addr & 0xFF00  ) >> 8  );//发送擦除扇区地址的中位
    SPI_TransferData((sector_addr & 0xFF    )       );//发送擦除扇区地址的低位
    SPI_CS_DISABLE();  //SPI终止
    
    /* 等待擦除完毕*/
    SPI_FLASH_WaitForWriteEnd();
}

/**
  * @brief  读取FLASH数据
  * @param  start_addr：读取地址 
  * @param  buffer：存储读出数据的指针 
  * @param  num：读取数据长度 
  * @note   无
  * @retval 无
  */
void SPI_FLASH_ReadBuffer(uint32_t read_addr, uint8_t* buffer, uint32_t size)
{
    uint32_t  read_num = size;                  //待写入总数量
    uint32_t  read_start_addr = read_addr;    //写入数据的目的首地址
    
    SPI_CS_ENABLE();   //SPI起始
    SPI_TransferData(W25Q64_READ_DATA);//发送 读 指令
    SPI_TransferData((read_start_addr & 0xFF0000) >> 16 );//发送地址的高位
    SPI_TransferData((read_start_addr & 0xFF00  ) >> 8  );//发送地址的中位
    SPI_TransferData((read_start_addr & 0xFF    )       );//发送地址的低位
    
    /* 读取数据 */
    while(read_num--)
    {
        *buffer = SPI_TransferData(W25Q64_DUMMY_BYTE);  //读取一个字节
        buffer++;                                       //指向下一个字节缓冲区
    }
    SPI_CS_DISABLE();  //SPI终止
}

/**
  * @brief  对FLASH按页写入数据，调用本函数写入数据前需要先擦除扇区（单页写入）
  * @param  start_addr：写入首地址 
  * @param  buffer：存储写入数据的指针 
  * @param  num：写入数据长度 
  * @note   写入的地址范围不能跨页
  * @retval 无
  */
void SPI_FLASH_WritePage(uint32_t write_addr, uint8_t* buffer, uint16_t size)
{
    uint16_t  write_num = size;                  //待写入总数量
    uint32_t  write_start_addr = write_addr;    //写入数据的目的首地址
    
    SPI_FLASH_WriteEnable();    //发送FLASH写使能命令
    
    SPI_CS_ENABLE();   //SPI起始
    SPI_TransferData(W25Q64_PAGE_PROGRAM);//发送 页编程 指令
    SPI_TransferData((write_start_addr & 0xFF0000) >> 16 );//发送地址的高位
    SPI_TransferData((write_start_addr & 0xFF00  ) >> 8 );//发送地址的中位
    SPI_TransferData((write_start_addr & 0xFF    )       );//发送地址的低位
    
    /* 检验写入数据是否跨页 */
    if((write_num + (write_start_addr%W25Q64_SPI_FLASH_PAGESIZE))>W25Q64_SPI_FLASH_PAGESIZE )
    {
        return;
    }

    /* 写入数据 */
    for (uint16_t i = 0; i < write_num; i++)
    {
        SPI_TransferData(buffer[i]);  //写入一个字节
    }
    
    SPI_CS_DISABLE();  //SPI终止
    
    /* 等待擦除完毕*/
    SPI_FLASH_WaitForWriteEnd();
}

/**
  * @brief  对FLASH按页写入数据，调用本函数写入数据前需要先擦除扇区（跨页连续写入）
  * @param  start_addr：写入首地址 
  * @param  buffer：存储写入数据的指针 
  * @param  num：写入数据长度 
  * @retval 无
  */
void SPI_FLASH_WriteBuffer(uint32_t write_addr, uint8_t* buffer, uint32_t size)
{
    uint32_t  write_num = size;                                     //待写入总数量
    uint32_t  write_start_addr = write_addr;                        //写入数据的目的首地址
    uint32_t  offset_addr = write_addr % W25Q64_SPI_FLASH_PAGESIZE; //页内偏移地址
    
    /* 检验首地址是否在页且存在跨页 */
    if((write_start_addr%W25Q64_SPI_FLASH_PAGESIZE) != 0)
    {
        uint32_t  page_remaining = W25Q64_SPI_FLASH_PAGESIZE-offset_addr;   //单页剩余空间
        if(write_num > page_remaining)
        {
            /* 如果写入数据大于首页剩余空间则写满当前空余空间 */
            SPI_FLASH_WritePage(write_start_addr, buffer, page_remaining);  //写入首页剩余的所有空间
            write_start_addr = write_start_addr + page_remaining;           //下一次数据的写入首地址进行偏移
            buffer += page_remaining;                                       //下一次数据的缓存数据指针地址进行偏移
            write_num -= page_remaining;                                    //待写入总数量更新
        }
        else
        {
            /* 如果写入数据不大于首页剩余空间则写完后直接退出函数 */
            SPI_FLASH_WritePage(write_start_addr, buffer, write_num);       //写入首页剩余的空间中
            return;
        }
    }
    while(write_num > 0)
    {
        /* 写入数量大于页最大容量则单次写入数量为最大页容量，否则单次写入数量为待写入数量 */
        uint16_t write_chunk  = (write_num > W25Q64_SPI_FLASH_PAGESIZE) ? W25Q64_SPI_FLASH_PAGESIZE : write_num;  //准备立即写入数量
        SPI_FLASH_WritePage(write_start_addr,buffer,write_chunk);   //写入数据
        write_start_addr += write_chunk ;                           //下一次数据的写入首地址进行偏移
        buffer += write_chunk ;                                     //下一次数据的缓存数据指针地址进行偏移
        write_num -= write_chunk ;                                  //待写入总数量更新
    } 
}

/*********************************************END OF FILE**********************/
