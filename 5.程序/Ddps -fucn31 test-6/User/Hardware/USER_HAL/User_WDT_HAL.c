#include "User_WDT_HAL.h"

void HAL_IWDT_Init(void)
{
   /* clear IWDTRST Flag*/
    if (RCM_ReadStatusFlag(RCM_FLAG_IWDTRST) != RESET)      //清除复位标志
    {
        RCM_ClearStatusFlag();
    }
    /* set IWDT Write Access */
    IWDT_EnableWriteAccess();   //写入0x5555解锁

    /* set IWDT Divider*/
    IWDT_ConfigDivider(IWDT_DIV_64);    //分频

    /* set IWDT Reloader*/
    IWDT_ConfigReload(40000 / 16);   //重载值  40khz 计数结果4s

    /* Refresh*/
    IWDT_Refresh();             //写0xaaaa  将重载值写入

    /* Enable IWDT*/
    IWDT_Enable();             //使能看门狗
				
			DBG_DisableAPB1Periph(DBG_APB1_PER_IWDT_STOP); //调试时关闭看门狗
}


void HAL_IWDT_Feed(void)
{
    IWDT_Refresh();
}
