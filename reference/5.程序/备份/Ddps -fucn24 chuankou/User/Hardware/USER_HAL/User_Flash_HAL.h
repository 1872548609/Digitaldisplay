#ifndef __USER_FLASH_HAL_H
#define __USER_FLASH_HAL_H

#include "superhead.h"


/* FLASH 起始地址 */
#define STM32_FLASH_BASE 0x08000000 /* STM32 FLASH 起始地址 */
#define STM32_FLASH_SIZE 0x10000    /* STM32 FLASH 64k总大小 */

/*  扇区大小 */
#define STM32_SECTOR_SIZE 1024 /* 页区大小为 1K 字节 */

// Flash 操作状态
typedef enum {
    FLASH_OK = 0,
    FLASH_ERROR,
    FLASH_ERROR_ADDR,
    FLASH_ERROR_WRITE,
    FLASH_ERROR_ERASE,
    FLASH_ERROR_PAGE_FULL  // 页已满，需要擦除
} Flash_Status;


void stmflash_write(uint32_t waddr, uint16_t *pbuf, uint16_t length);

void stmflash_read(uint32_t raddr, uint16_t *pbuf, uint16_t length);



#endif
