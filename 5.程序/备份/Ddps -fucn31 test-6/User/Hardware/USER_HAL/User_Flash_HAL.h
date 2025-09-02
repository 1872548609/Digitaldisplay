#ifndef __USER_FLASH_HAL_H
#define __USER_FLASH_HAL_H

#include "superhead.h"


/* FLASH 起始地址 */
#define STM32_FLASH_BASE 0x08000000 /* STM32 FLASH 起始地址 */
#define STM32_FLASH_SIZE 0x10000    /* STM32 FLASH 64k总大小 */

#define STM32_FLASH_TOTAL_PAGES   0x40 //总页数

/*  扇区大小 */
#define STM32_SECTOR_SIZE 0x400 /* 页区大小为 1K 字节 */

#define STM32_START_PAGE 63 //起始页索引

// Flash 操作状态
typedef enum {
    FLASH_OK = 0,           //操作完成
    FLASH_ERROR,             //操作失败
    FLASH_ERROR_ADDR,      //写地址错误
    FLASH_ERROR_WRITE,
    FLASH_ERROR_ERASE,
    FLASH_ERROR_PAGE_FULL  // 页已满，需要擦除
} Flash_Status;


typedef struct {
    uint32_t current_page;  // 当前使用的页（0 ~ FLASH_TOTAL_PAGES-1）
    uint32_t write_offset;  // 当前页的写入偏移（字节）
				uint8_t is_page_full;   // 当前页是否已满（1=满，0=未满）
} Flash_Page_Manager;

extern Flash_Page_Manager manager;

//正点原子
void stmflash_write(uint32_t waddr, uint16_t *pbuf, uint16_t length);

void stmflash_read(uint32_t raddr, uint16_t *pbuf, uint16_t length);


//页面管理
void Flash_Page_Init(Flash_Page_Manager *manager) ;
void Flash_Page_Set(Flash_Page_Manager *manager,uint32_t currentpage,uint32_t offset) ;
Flash_Status Flash_Page_EraseAndReset(Flash_Page_Manager *manager, uint32_t page_index) ;
Flash_Status Flash_Page_Write(Flash_Page_Manager *manager, const void *data, uint32_t size) ;
Flash_Status Flash_Page_SafeWrite(Flash_Page_Manager *manager, const void *data, uint32_t size)  ;
Flash_Status Flash_Page_Read(uint32_t page_index, void *buffer, uint32_t offset, uint32_t size); 
		
	
	
//用例
//int main() {
//    Flash_Page_Manager manager;
//    Flash_Page_Init(&manager);
// 
//    // 示例数据（9 个 float）
//    float data[9] = {1.1f, 2.2f, 3.3f, 4.4f, 5.5f, 6.6f, 7.7f, 8.8f, 9.9f};
// 
//    // 安全写入（自动处理页满）
//    Flash_Status status = Flash_Page_SafeWrite(&manager, data, sizeof(data));
//    if (status != FLASH_OK) {
//        // 错误处理
//        while (1);
//    }
// 
//    return 0;
//}

//void Read_Data_Example() {
//    Flash_Page_Manager manager;
//    Flash_Page_Init(&manager);
// 
//    float buffer[9];
//    uint32_t page_index = 0; // 从第 0 页读取
//    uint32_t offset = 0;     // 从页开头读取
// 
//    Flash_Status status = Flash_Page_Read(page_index, buffer, offset, sizeof(buffer));
//    if (status != FLASH_OK) {
//        // 错误处理
//        while (1);
//    }
// 
//    // 使用 buffer 中的数据...
//}

#endif
