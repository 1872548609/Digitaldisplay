#ifndef __USER_FLASH_HAL_H
#define __USER_FLASH_HAL_H

#include "superhead.h"


void stmflash_write(uint32_t waddr, uint16_t *pbuf, uint16_t length);

void stmflash_read(uint32_t raddr, uint16_t *pbuf, uint16_t length);

#endif
