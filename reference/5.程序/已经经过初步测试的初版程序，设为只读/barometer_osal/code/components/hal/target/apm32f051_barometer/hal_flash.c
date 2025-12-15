/* ------------------------------------------------------------------------------------------------
 *                                          Includes
 * ------------------------------------------------------------------------------------------------
 */

#include "hal_board_cfg.h"
#include "hal_flash.h"
#include "hal_mcu.h"
#include "hal_types.h"

/**************************************************************************************************
 * @fn          HalFlashRead
 *
 * @brief       This function reads 'cnt' bytes from the internal flash.
 *
 * input parameters
 *
 * @param       pg - A valid flash page number.
 * @param       offset - A valid offset into the page.
 * @param       buf - A valid buffer space at least as big as the 'cnt' parameter.
 * @param       cnt - A valid number of bytes to read.
 *
 * output parameters
 *
 * None.
 *
 * @return      None.
 **************************************************************************************************
 */
void HalFlashRead(uint8 pg, uint16 offset, uint8 *buf, uint16 cnt)
{
    uint32_t raddr = HAL_FLASH_PAGE_MAP + pg * HAL_FLASH_PAGE_SIZE + offset;

    for (uint16_t i = 0; i < cnt; i++)
    {
        //*buf++ = *(volatile uint32_t *)raddr++;
        *buf++ = *(((volatile uint8_t *) (uint32_t)raddr) + i);
    }
}

/**************************************************************************************************
 * @fn          HalFlashWrite
 *
 * @brief       This function writes 'cnt' bytes to the internal flash.
 *
 * input parameters
 *
 * @param       addr - Valid HAL flash write address: actual addr / 4 and quad-aligned.
 * @param       buf - Valid buffer space at least as big as 'cnt' X 4.
 * @param       cnt - Number of 4-byte blocks to write.
 *
 * output parameters
 *
 * None.
 *
 * @return      None.
 **************************************************************************************************
 */
void HalFlashWrite(uint16 addr, uint8 *buf, uint16 cnt)
{
    uint32_t flash_addr = HAL_FLASH_PAGE_MAP + addr * HAL_FLASH_WORD_SIZE;
    uint16_t words;
    uint16_t bytes;
    uint32_t word_write = 0;
    FMC_STATE_T fmc_state;

    cnt *= HAL_FLASH_WORD_SIZE;
    words = cnt / HAL_FLASH_WORD_SIZE;
    bytes = cnt % HAL_FLASH_WORD_SIZE;
    
    
    for (uint16_t i = 0; i < words; i++)
    {
        for (uint8_t j = 0; j < HAL_FLASH_WORD_SIZE; j++)
        {
            word_write <<= 8;
            word_write |= buf[HAL_FLASH_WORD_SIZE - j - 1];
        }

        FMC_Unlock();
        fmc_state = FMC_ProgramWord(flash_addr, word_write);
        FMC_Lock();

        buf += HAL_FLASH_WORD_SIZE;
        flash_addr += HAL_FLASH_WORD_SIZE;
    }

    if (fmc_state != FMC_STATE_COMPLETE)
    {
        return;
    }

    if (bytes)
    {
        word_write = 0;
        for (uint8_t i = 0; i < bytes; i++)
        {
            word_write <<= 8;
            word_write |= buf[bytes - i - 1];
        }

        for (uint8_t i = 0; i < (4-bytes); i++)
        {
            word_write <<= 8;
            word_write |= 0;
        }

        FMC_Unlock();
        fmc_state = FMC_ProgramWord(flash_addr, word_write);
        FMC_Lock();
    }
}

/**************************************************************************************************
 * @fn          HalFlashErase
 *
 * @brief       This function erases the specified page of the internal flash.
 *
 * input parameters
 *
 * @param       pg - A valid flash page number to erase.
 *
 * output parameters
 *
 * None.
 *
 * @return      None.
 **************************************************************************************************
 */
void HalFlashErase(uint8 pg)
{
    uint32_t page_addr = HAL_FLASH_PAGE_MAP + pg * HAL_FLASH_PAGE_SIZE;
    
    FMC_Unlock();
    FMC_ErasePage(page_addr);
    FMC_Lock();
}

/**************************************************************************************************
*/
