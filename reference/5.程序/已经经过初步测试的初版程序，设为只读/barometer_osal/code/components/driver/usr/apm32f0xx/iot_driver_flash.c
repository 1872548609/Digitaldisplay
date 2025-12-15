#include "iot_driver_flash.h"
// 字读写===================================================================START
/**
 * @brief  从指定地址读取一个字 (32 位数据)
 * @param  faddr: 读取地址 (此地址必须为 4 的倍数!!)
 * @retval 读取到的 32 位数据
 */
uint32_t stmflash_read_word(uint32_t faddr)
{
    return *(volatile uint32_t *)faddr;  
}


/**
 * @brief  从指定地址读取多个字 (32 位数据)
 * @param  raddr: 起始地址 (必须为 4 的倍数!!)
 * @param  pbuf: 存储读取数据的缓冲区 (uint32_t 数组)
 * @param  length: 要读取的字数 (即 32 位数据的个数)
 * @retval 无
 */
void stmflash_read_words(uint32_t raddr, uint32_t *pbuf, uint16_t length)
{
    for (uint16_t i = 0; i < length; i++)
    {
        pbuf[i] = stmflash_read_word(raddr); 
        raddr += 4;  
    }
}

/**
 * @brief  不检查的写入多个字 (32 位数据)
 *         假设目标扇区已擦除，直接写入
 * @param  waddr: 起始地址 (必须为 4 的倍数!!)
 * @param  pbuf: 要写入的数据缓冲区 (uint32_t 数组)
 * @param  length: 要写入的字数 (即 32 位数据的个数)
 * @retval 无
 */
void stmflash_write_words_nocheck(uint32_t waddr, uint32_t *pbuf, uint16_t length)
{
    for (uint16_t i = 0; i < length; i++)
    {
       FMC_ProgramWord(waddr, pbuf[i]);  
        waddr += 4; 
    }
}

/**
 * @brief  在 FLASH 指定位置写入数据（自动擦除，字单位）
 * @note   先检测扇区是否已擦除（全 0xFFFFFFFF），未擦除则先擦除再写入。
 *         数据长度不足扇区时，仅影响目标扇区。
 * @param  waddr: 起始地址 (此地址必须为 4 的倍数!!)
 * @param  pbuf: 数据指针 (uint32_t 数组)
 * @param  length: 要写入的字数 (32 位数据的个数)
 * @retval 无
 */
uint32_t g_flashbuf[STM32_SECTOR_SIZE / 4]; // 缓冲区（用于暂存扇区数据，字单位）

void stmflash_write_words(uint32_t waddr, uint32_t *pbuf, uint16_t length)
{
    uint32_t secpos;   
    uint16_t secoff;    
    uint16_t secremain; 
    uint16_t i;
    uint32_t offaddr;   

    if (waddr < STM32_FLASH_BASE || (waddr >= (STM32_FLASH_BASE + 1024 * STM32_FLASH_SIZE)))
    {
        return; 
    }

    if (waddr % 4 != 0)
    {
        return; 
    }

    FMC_Unlock(); 

    offaddr = waddr - STM32_FLASH_BASE; 
    secpos = offaddr / STM32_SECTOR_SIZE; 
    secoff = (offaddr % STM32_SECTOR_SIZE) / 4; 
    secremain = STM32_SECTOR_SIZE / 4 - secoff; 


    if (length <= secremain)
    {
        secremain = length;
    }

    while (1) 
    {
        stmflash_read_words(secpos * STM32_SECTOR_SIZE + STM32_FLASH_BASE, g_flashbuf, STM32_SECTOR_SIZE / 4);

      
        for (i = 0; i < secremain; i++)
        {
            if (g_flashbuf[secoff + i] != 0XFFFFFFFF)
            {
                break; 
            }
        }

        if (i < secremain)
        {

            FMC_ErasePage(secpos * STM32_SECTOR_SIZE + STM32_FLASH_BASE);

            for (i = 0; i < secremain; i++)
            {
                g_flashbuf[i + secoff] = pbuf[i];
            }

            stmflash_write_words_nocheck(secpos * STM32_SECTOR_SIZE + STM32_FLASH_BASE, g_flashbuf, STM32_SECTOR_SIZE / 4);
        }
        else 
        {
            stmflash_write_words_nocheck(waddr, pbuf, secremain);
        }

        
        if (length == secremain)
        {
            break; 
        }
        else 
        {
            secpos++;        
            secoff = 0;     
            pbuf += secremain; 
            waddr += secremain * 4; 
            length -= secremain; 

            if (length > (STM32_SECTOR_SIZE / 4))
            {
                secremain = STM32_SECTOR_SIZE / 4;
            }
            else
            {
                secremain = length;
            }
        }
    }

    FMC_Lock(); 
}


#define FLASH_PAGE_SIZE       STM32_SECTOR_SIZE  // 1KB/页
#define FLASH_PAGE_COUNT      (STM32_FLASH_SIZE / FLASH_PAGE_SIZE)  // 总页数

/**
 * @brief 将数据写入指定页（循环追加写入）
 * @param page_idx 页索引（0-based）
 * @param data 要写入的数据数组
 * @param length 数据长度（字数）
 * @return 0成功，-1失败
 */
int flash_write_page(uint32_t page_idx, uint32_t *data, uint16_t length)
{
    if (page_idx >= FLASH_PAGE_COUNT || length == 0 || 
        length > (FLASH_PAGE_SIZE / 4)) {
        return -1; 
    }

    uint32_t page_addr = STM32_FLASH_BASE + page_idx * FLASH_PAGE_SIZE;
    uint32_t page_end = page_addr + FLASH_PAGE_SIZE;
    uint32_t current_addr = page_addr;
    uint32_t used_size = 0;
    
   
    while (current_addr + 4 <= page_end) {
        uint32_t word;
        stmflash_read_words(current_addr, &word, 1);
        
        
        if (word == 0xFFFFFFFF) {
            break;
        }
        current_addr += 4;
        used_size += 4;
    }
    
    
    if (used_size + length * 4 > FLASH_PAGE_SIZE) {
        // 空间不足，擦除页
        FMC_Unlock();
        FMC_ErasePage(page_addr);
        FMC_Lock();
        current_addr = page_addr;
    }
    
    // 写入数据
    FMC_Unlock();
    stmflash_write_words_nocheck(current_addr, data, length);
    FMC_Lock();
    
    return 0;
}

/**
 * @brief 读取指定页中最新写入的数据
 * @param page_idx 页索引（0-based）
 * @param buf 接收缓冲区
 * @param max_length 缓冲区最大容量（字数）
 * @return 实际读取的数据长度，0表示无数据，-1表示错误
 */
int flash_read_latest_in_page(uint32_t page_idx, uint32_t *buf, uint16_t max_length ,uint32_t endflag)
{
    if (page_idx >= FLASH_PAGE_COUNT || max_length == 0) {
        return -1;
    }

    uint32_t page_addr = STM32_FLASH_BASE + page_idx * FLASH_PAGE_SIZE;
    uint32_t page_end = page_addr + FLASH_PAGE_SIZE;
    uint32_t current_addr = page_end;
    //uint32_t data_found = 0;
    uint16_t words_to_read = 0;
    
  
    while (current_addr > page_addr) {
        current_addr -= 4;
        uint32_t word;
        stmflash_read_words(current_addr, &word, 1);
        
		if (word == endflag) {
			
            if (words_to_read > 0) {
				current_addr +=4;
                break; // 已经找到数据，且找到结束
            }
			
			words_to_read++;
        } 
		else if (word != 0xFFFFFFFF){
            words_to_read++;
        }

    }
    
    if (words_to_read == 0) {
        return 0; // 无有效数据
    }
    
  
    uint16_t read_len = (words_to_read < max_length) ? words_to_read : max_length;
    
   
    uint32_t read_addr = current_addr;
    stmflash_read_words(read_addr, buf, read_len);
    
    return read_len;
}

// 字读写===================================================================END



