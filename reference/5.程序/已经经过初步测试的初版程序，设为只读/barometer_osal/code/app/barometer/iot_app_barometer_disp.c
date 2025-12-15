/**************************************************************************************************
Filename:       iot_app_barometer_disp.c
Revised:        $Date: 2019-8-20 15:35
Revision:       $Revision: V1.0.0

Description:

Copyright 2021 uwaycon. All rights reserved.
                                                
**************************************************-*****************^******************************/
#include "main.h"

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef _IOT_APP_BAROMETER_DISP_H
/*************************************************-*************************************************
*                                               NOTE
**************************************************-*****************^******************************/
//-- source insight use UTF-8 to encode, File->Reload As Encoding...->UTF-8
//-- source insight set default encode, Option->Preferences->Files->Default encoding->UTF-8

/*************************************************-*************************************************
*                                             INCLUDES
**************************************************-*****************^******************************/

/*************************************************-*************************************************
*                                               MACRO
**************************************************-*****************^******************************/

/*************************************************-*************************************************
*                                          CONSTANT DEFINED
**************************************************-*****************^******************************/

/*************************************************-*************************************************
*                                           STRUCT DEFINED
**************************************************-*****************^******************************/

/*************************************************-*************************************************
*                                          GLOBAL VARIABLES
**************************************************-*****************^******************************/

/*************************************************-*************************************************
*                                          LOCAL VARIABLES
**************************************************-*****************^******************************/
uint8_t iot_app_barometer_disp_task_id;                       // 任务ID，由OSAL分配，用于内部任务/事件处理

/*************************************************-*************************************************
*                                          LOCAL VARIABLES
**************************************************-*****************^******************************/
uint8_t iot_app_barometer_disp_event_display_state = IOT_APP_BAROMETER_DISP_EVENT_DISPLAY_STATE_NORMAL;     // 显示动画事件变量
uint8_t iot_app_barometer_disp_event_flash_state = IOT_APP_BAROMETER_DISP_EVENT_FLASH_STATE_NORMAL;         // 闪烁显示事件变量

uint8_t iot_app_barometer_disp_key_lock_flag = IOT_APP_BAROMETER_DISP_KEY_STATE_UNLOCK;                     // 按键锁定状态变量
uint8_t subsidiary_display_flag = 0;                                                                        // 标示进入编号和自定义修改
/*************************************************-*************************************************
*                                             FUNCTIONS
**************************************************-*****************^******************************/

/*************************************************-*************************************************
*                                             FUNCTIONS
**************************************************-*****************^******************************/
#ifdef APM32F051

// pro模式进入
void iot_app_menu_item_setting_pro_cb_enter(void *item)
{
    div_disp_by_string(MAIN_SCREEN, " PRO");
    div_disp_by_string(SECOND_SCREEN, "MODE");
}

// pro模式退出
void iot_app_menu_item_setting_pro_cb_exit(void *item)
{
}

//= pro模式设定自定义值
uint8_t iot_app_menu_item_setting_pro_cb_click_key(void *item, uint8_t keys)
{
    if (keys)
    {
        if (keys & HAL_KEY_MODE)
        {
            if (!subsidiary_display_flag)
            {
                iot_app_menu_enter_child_menu();
            }
            else if (subsidiary_display_flag == 1)
            {
                subsidiary_display_flag = 2;
                div_disp_num(2, iot_app_common_data_data_subsidiary_display_no_left);
                iot_app_barometer_disp_event_flash_state = IOT_APP_BAROMETER_DISP_EVENT_FLASH_STATE_NO_RIGHT;
                IOT_APP_BAROMETER_DISP_EVENT_FLASH_PERIOD_START();
            }
            else if (subsidiary_display_flag == 2)
            {
                subsidiary_display_flag = 0;
                iot_app_menu_stop();
                iot_app_common_data_system_state_set(IOT_APP_COMMON_DATA_SYSTEM_STATE_NORMAL);
                iot_app_barometer_disp_event_flash_state = IOT_APP_BAROMETER_DISP_EVENT_FLASH_STATE_NORMAL;
                IOT_APP_BAROMETER_DISP_EVENT_FLASH_STOP();
            }
        }
        if (iot_app_common_data_subsidiary_display_flag == IOT_APP_COMMON_DATA_SUBSIDIARY_DISPLAY_NO)
        {
            if (keys & (HAL_KEY_LEFT_UP | HAL_KEY_RIGHT_DOWN))
            {
                // 从pro进入时显示内容
                if (!subsidiary_display_flag)
                {
                    subsidiary_display_flag = 1;
                    div_disp_by_string(MAIN_SCREEN, "No  ");
                    div_disp_num(2, iot_app_common_data_data_subsidiary_display_no_left);
                    div_disp_num(3, iot_app_common_data_data_subsidiary_display_no_right);
                    div_disp_by_string(SECOND_SCREEN, " Sub");
                    div_disp_out1andpoint(P2, 1);
                    iot_app_barometer_disp_event_flash_state = IOT_APP_BAROMETER_DISP_EVENT_FLASH_STATE_NO_LEFT;
                    IOT_APP_BAROMETER_DISP_EVENT_FLASH_PERIOD_START();
                }
                else if (subsidiary_display_flag == 1)
                {
                    if (keys & HAL_KEY_LEFT_UP)
                    {
                        if (++iot_app_common_data_data_subsidiary_display_no_left > 9)
                        {
                            iot_app_common_data_data_subsidiary_display_no_left = 0;
                        }
                    }
                    if (keys & HAL_KEY_RIGHT_DOWN)
                    {
                        if (--iot_app_common_data_data_subsidiary_display_no_left == 255)
                        {
                            iot_app_common_data_data_subsidiary_display_no_left = 9;
                        }
                    }
                }
                else if (subsidiary_display_flag == 2)
                {
                    if (keys & HAL_KEY_LEFT_UP)
                    {
                        if (++iot_app_common_data_data_subsidiary_display_no_right > 9)
                        {
                            iot_app_common_data_data_subsidiary_display_no_right = 0;
                        }
                    }
                    if (keys & HAL_KEY_RIGHT_DOWN)
                    {
                        if (--iot_app_common_data_data_subsidiary_display_no_right == 255)
                        {
                            iot_app_common_data_data_subsidiary_display_no_right = 9;
                        }
                    }
                }
                iot_app_data_flash_store();
            }
        }
        return 1;
    }
    return 0;
}

// 副屏显示内容设置进入
void iot_app_menu_item_setting_subsidiary_display_cb_enter(void *item)
{
    switch (iot_app_common_data_subsidiary_display_flag)
    {
        case IOT_APP_COMMON_DATA_SUBSIDIARY_DISPLAY_STD:
        {
            div_disp_by_string(MAIN_SCREEN, " Std");
        }break;
        case IOT_APP_COMMON_DATA_SUBSIDIARY_DISPLAY_OFF:
        {
            div_disp_by_string(MAIN_SCREEN, " Off");
        }break;
        case IOT_APP_COMMON_DATA_SUBSIDIARY_DISPLAY_UNIT:
        {
            div_disp_by_string(MAIN_SCREEN, "UnIt");
        }break;
        case IOT_APP_COMMON_DATA_SUBSIDIARY_DISPLAY_NO:
        {
            div_disp_by_string(MAIN_SCREEN, "No**");
        }break;
        case IOT_APP_COMMON_DATA_SUBSIDIARY_DISPLAY_CUSTOM:
        {
            div_disp_by_string(MAIN_SCREEN, "Cust");
        }break;
        default:break;
    }
    if (iot_app_common_data_subsidiary_display_flag == IOT_APP_COMMON_DATA_SUBSIDIARY_DISPLAY_NO)
    {
        div_disp_out1andpoint(P2, 1);
    }
    else
    {
        div_disp_out1andpoint(P2, 0);
    }
    div_disp_by_string(SECOND_SCREEN, " Sub");
}

// 副屏显示内容设置退出
void iot_app_menu_item_setting_subsidiary_display_cb_exit(void *item)
{
    div_disp_out1andpoint(P2, 0);
}

// 副屏显示内容设置按键处理
uint8_t iot_app_menu_item_setting_subsidiary_display_cb_click_key(void *item, uint8_t keys)
{
    if (keys & (HAL_KEY_RIGHT_DOWN | HAL_KEY_LEFT_UP))
    {
        if (keys & HAL_KEY_LEFT_UP)
        {
            if (++iot_app_common_data_subsidiary_display_flag > 3)
            {
                iot_app_common_data_subsidiary_display_flag = 0;
            }
        }
        else if (keys & HAL_KEY_RIGHT_DOWN)
        {
            if (--iot_app_common_data_subsidiary_display_flag == 255)
            {
                iot_app_common_data_subsidiary_display_flag = 3;
            }
        }
        iot_app_data_flash_store();
        switch (iot_app_common_data_subsidiary_display_flag)
        {
            case IOT_APP_COMMON_DATA_SUBSIDIARY_DISPLAY_STD:
            {
                div_disp_by_string(MAIN_SCREEN, " Std");
            }break;
            case IOT_APP_COMMON_DATA_SUBSIDIARY_DISPLAY_OFF:
            {
                div_disp_by_string(MAIN_SCREEN, " Off");
            }break;
            case IOT_APP_COMMON_DATA_SUBSIDIARY_DISPLAY_UNIT:
            {
                div_disp_by_string(MAIN_SCREEN, "UnIt");
            }break;
            case IOT_APP_COMMON_DATA_SUBSIDIARY_DISPLAY_NO:
            {
                div_disp_by_string(MAIN_SCREEN, "No**");
            }break;
            case IOT_APP_COMMON_DATA_SUBSIDIARY_DISPLAY_CUSTOM:
            {
                div_disp_by_string(MAIN_SCREEN, "Cust");
            }break;
            default:break;
        }
        if (iot_app_common_data_subsidiary_display_flag == IOT_APP_COMMON_DATA_SUBSIDIARY_DISPLAY_NO)
        {
            div_disp_out1andpoint(P2, 1);
        }
        else
        {
            div_disp_out1andpoint(P2, 0);
        }
        div_disp_by_string(SECOND_SCREEN, " Sub");
        return 1;
    }
    return 0;
}

// 显示速度设置进入
void iot_app_menu_item_setting_display_speed_cb_enter(void *item)
{
    switch (iot_app_common_data_display_speed_flag)
    {
        case IOT_APP_COMMON_DATA_DISPLAY_SPEED_250MS:
        {
            div_disp_by_string(MAIN_SCREEN, " 250");
        }break;
        case IOT_APP_COMMON_DATA_DISPLAY_SPEED_500MS:
        {
            div_disp_by_string(MAIN_SCREEN, " 500");
        }break;
        case IOT_APP_COMMON_DATA_DISPLAY_SPEED_1S:
        {
            div_disp_by_string(MAIN_SCREEN, "1000");
        }break;
        default:break;
    }
    div_disp_by_string(SECOND_SCREEN, "D-SP");
}

// 显示速度设置退出
void iot_app_menu_item_setting_display_speed_cb_exit(void *item)
{
}

// 显示速度设置按键处理
uint8_t iot_app_menu_item_setting_display_speed_cb_click_key(void *item, uint8_t keys)
{
    if (keys & (HAL_KEY_RIGHT_DOWN | HAL_KEY_LEFT_UP))
    {
        if (keys & HAL_KEY_LEFT_UP)
        {
            if (++iot_app_common_data_display_speed_flag > 2)
            {
                iot_app_common_data_display_speed_flag = 0;
            }
        }
        else if (keys & HAL_KEY_RIGHT_DOWN)
        {
            if (--iot_app_common_data_display_speed_flag == 255)
            {
                iot_app_common_data_display_speed_flag = 2;
            }
        }
        iot_app_data_flash_store();
        switch (iot_app_common_data_display_speed_flag)
        {
            case IOT_APP_COMMON_DATA_DISPLAY_SPEED_250MS:
            {
                div_disp_by_string(MAIN_SCREEN, " 250");
            }break;
            case IOT_APP_COMMON_DATA_DISPLAY_SPEED_500MS:
            {
                div_disp_by_string(MAIN_SCREEN, " 500");
            }break;
            case IOT_APP_COMMON_DATA_DISPLAY_SPEED_1S:
            {
                div_disp_by_string(MAIN_SCREEN, "1000");
            }break;
            default:break;
        }
        div_disp_by_string(SECOND_SCREEN, "D-SP");
        return 1;
    }
    return 0;
}

// 应差级别设置进入
void iot_app_menu_item_setting_hysteresis_level_cb_enter(void *item)
{
    switch (iot_app_common_data_hysteresis_level)
    {
        case IOT_APP_COMMON_DATA_HYSTERESIS_LEVEL_0:
        {
            div_disp_by_string(MAIN_SCREEN, "   0");
        }break;
        case IOT_APP_COMMON_DATA_HYSTERESIS_LEVEL_1:
        {
            div_disp_by_string(MAIN_SCREEN, "   1");
        }break;
        case IOT_APP_COMMON_DATA_HYSTERESIS_LEVEL_2:
        {
            div_disp_by_string(MAIN_SCREEN, "   2");
        }break;
        case IOT_APP_COMMON_DATA_HYSTERESIS_LEVEL_3:
        {
            div_disp_by_string(MAIN_SCREEN, "   3");
        }break;
        case IOT_APP_COMMON_DATA_HYSTERESIS_LEVEL_4:
        {
            div_disp_by_string(MAIN_SCREEN, "   4");
        }break;
        case IOT_APP_COMMON_DATA_HYSTERESIS_LEVEL_5:
        {
            div_disp_by_string(MAIN_SCREEN, "   5");
        }break;
        case IOT_APP_COMMON_DATA_HYSTERESIS_LEVEL_6:
        {
            div_disp_by_string(MAIN_SCREEN, "   6");
        }break;
        case IOT_APP_COMMON_DATA_HYSTERESIS_LEVEL_7:
        {
            div_disp_by_string(MAIN_SCREEN, "   7");
        }break;
        case IOT_APP_COMMON_DATA_HYSTERESIS_LEVEL_8:
        {
            div_disp_by_string(MAIN_SCREEN, "   8");
        }break;
        default:break;
    }
    div_disp_by_string(SECOND_SCREEN, " HYS");
}

// 应差级别设置退出
void iot_app_menu_item_setting_hysteresis_level_cb_exit(void *item)
{
}

// 应差级别设置按键处理
uint8_t iot_app_menu_item_setting_hysteresis_level_cb_click_key(void *item, uint8_t keys)
{
    if (keys & (HAL_KEY_RIGHT_DOWN | HAL_KEY_LEFT_UP))
    {
        if (keys & HAL_KEY_LEFT_UP)
        {
            if (++iot_app_common_data_hysteresis_level > 8)
            {
                iot_app_common_data_hysteresis_level = 1;
            }
        }
        else if (keys & HAL_KEY_RIGHT_DOWN)
        {
            if (--iot_app_common_data_hysteresis_level == 255)
            {
                iot_app_common_data_hysteresis_level = 8;
            }
        }
        iot_app_data_flash_store();
        switch (iot_app_common_data_hysteresis_level)
        {
            case IOT_APP_COMMON_DATA_HYSTERESIS_LEVEL_0:
            {
                div_disp_by_string(MAIN_SCREEN, "   0");
            }break;
            case IOT_APP_COMMON_DATA_HYSTERESIS_LEVEL_1:
            {
                div_disp_by_string(MAIN_SCREEN, "   1");
            }break;
            case IOT_APP_COMMON_DATA_HYSTERESIS_LEVEL_2:
            {
                div_disp_by_string(MAIN_SCREEN, "   2");
            }break;
            case IOT_APP_COMMON_DATA_HYSTERESIS_LEVEL_3:
            {
                div_disp_by_string(MAIN_SCREEN, "   3");
            }break;
            case IOT_APP_COMMON_DATA_HYSTERESIS_LEVEL_4:
            {
                div_disp_by_string(MAIN_SCREEN, "   4");
            }break;
            case IOT_APP_COMMON_DATA_HYSTERESIS_LEVEL_5:
            {
                div_disp_by_string(MAIN_SCREEN, "   5");
            }break;
            case IOT_APP_COMMON_DATA_HYSTERESIS_LEVEL_6:
            {
                div_disp_by_string(MAIN_SCREEN, "   6");
            }break;
            case IOT_APP_COMMON_DATA_HYSTERESIS_LEVEL_7:
            {
                div_disp_by_string(MAIN_SCREEN, "   7");
            }break;
            case IOT_APP_COMMON_DATA_HYSTERESIS_LEVEL_8:
            {
                div_disp_by_string(MAIN_SCREEN, "   8");
            }break;
            default:break;
        }
        div_disp_by_string(SECOND_SCREEN, " HYS");
        return 1;
    }
    return 0;
}

// 颜色联动转换进入
void iot_app_menu_item_setting_display_color_linkage_cb_enter(void *item)
{
    if (!iot_app_common_data_display_color_linkage_flag)
    {
        div_disp_by_string(MAIN_SCREEN, "Out1");
    }
    else
    {
        div_disp_by_string(MAIN_SCREEN, "Out2");
    }
    div_disp_by_string(SECOND_SCREEN, "DISP");
}

// 颜色联动转换退出
void iot_app_menu_item_setting_display_color_linkage_cb_exit(void *item)
{
}

// 颜色联动转换按键处理
uint8_t iot_app_menu_item_setting_display_color_linkage_cb_click_key(void *item, uint8_t keys)
{
    if (keys & (HAL_KEY_RIGHT_DOWN | HAL_KEY_LEFT_UP))
    {
        iot_app_common_data_display_color_linkage_flag = !iot_app_common_data_display_color_linkage_flag;
        iot_app_data_flash_store();
        if (!iot_app_common_data_display_color_linkage_flag)
        {
            div_disp_by_string(MAIN_SCREEN, "Out1");
        }
        else
        {
            div_disp_by_string(MAIN_SCREEN, "Out2");
        }
        div_disp_by_string(SECOND_SCREEN, "DISP");
        return 1;
    }
    return 0;
}

// ECO模式进入
void iot_app_menu_item_setting_eco_cb_enter(void *item)
{
    switch (iot_app_common_data_eco_flag)
    {
        case IOT_APP_COMMON_DATA_ECO_OFF:
        {
            div_disp_by_string(MAIN_SCREEN, " Off");
        }break;
        case IOT_APP_COMMON_DATA_ECO_STD:
        {
            div_disp_by_string(MAIN_SCREEN, " Std");
        }break;
        case IOT_APP_COMMON_DATA_ECO_FULL:
        {
            div_disp_by_string(MAIN_SCREEN, "FUll");
        }break;
        default:break;
    }
    div_disp_by_string(SECOND_SCREEN, " ECO");
}

// ECO模式退出
void iot_app_menu_item_setting_eco_cb_exit(void *item)
{
}

// ECO模式按键处理
uint8_t iot_app_menu_item_setting_eco_cb_click_key(void *item, uint8_t keys)
{
    if (keys & (HAL_KEY_RIGHT_DOWN | HAL_KEY_LEFT_UP))
    {
        if (keys & HAL_KEY_LEFT_UP)
        {
            if (++iot_app_common_data_eco_flag > 2)
            {
                iot_app_common_data_eco_flag = 0;
            }
        }
        else if (keys & HAL_KEY_RIGHT_DOWN)
        {
            if (--iot_app_common_data_eco_flag == 255)
            {
                iot_app_common_data_eco_flag = 2;
            }
        }
        iot_app_data_flash_store();
        switch (iot_app_common_data_eco_flag)
        {
            case IOT_APP_COMMON_DATA_ECO_OFF:
            {
                div_disp_by_string(MAIN_SCREEN, " Off");
            }break;
            case IOT_APP_COMMON_DATA_ECO_STD:
            {
                div_disp_by_string(MAIN_SCREEN, " Std");
            }break;
            case IOT_APP_COMMON_DATA_ECO_FULL:
            {
                div_disp_by_string(MAIN_SCREEN, "FUll");
            }break;
            default:break;
        }
        div_disp_by_string(SECOND_SCREEN, " ECO");
        return 1;
    }
    return 0;
}

// 复制功能进入
void iot_app_menu_item_setting_copy_cb_enter(void *item)
{
    switch (iot_app_common_data_copy_flag)
    {
        case IOT_APP_COMMON_DATA_COPY_OFF:
        {
            div_disp_by_string(MAIN_SCREEN, " OFF");
        }break;
        case IOT_APP_COMMON_DATA_COPY_ON:
        {
            div_disp_by_string(MAIN_SCREEN, "  ON");
        }break;
        case IOT_APP_COMMON_DATA_COPY_ON_LOCK:
        {
            div_disp_by_string(MAIN_SCREEN, "ON-L");
        }break;
        case IOT_APP_COMMON_DATA_COPY_READY:
        {
            div_disp_by_string(MAIN_SCREEN, "REDY");
        }break;
        default:break;
    }
    div_disp_by_string(SECOND_SCREEN, "COPY");
}

// 复制功能退出
void iot_app_menu_item_setting_copy_cb_exit(void *item)
{
}

// 复制功能按键处理
uint8_t iot_app_menu_item_setting_copy_cb_click_key(void *item, uint8_t keys)
{
    if (keys & (HAL_KEY_RIGHT_DOWN | HAL_KEY_LEFT_UP))
    {
        if (keys & HAL_KEY_LEFT_UP)
        {
            if (++iot_app_common_data_copy_flag > 3)
            {
                iot_app_common_data_copy_flag = 0;
            }
        }
        else if (keys & HAL_KEY_RIGHT_DOWN)
        {
            if (--iot_app_common_data_copy_flag == 255)
            {
                iot_app_common_data_copy_flag = 3;
            }
        }
        iot_app_data_flash_store();
        switch (iot_app_common_data_copy_flag)
        {
            case IOT_APP_COMMON_DATA_COPY_OFF:
            {
                div_disp_by_string(MAIN_SCREEN, " OFF");
            }break;
            case IOT_APP_COMMON_DATA_COPY_ON:
            {
                div_disp_by_string(MAIN_SCREEN, "  ON");
            }break;
            case IOT_APP_COMMON_DATA_COPY_ON_LOCK:
            {
                div_disp_by_string(MAIN_SCREEN, "ON-L");
            }break;
            case IOT_APP_COMMON_DATA_COPY_READY:
            {
                div_disp_by_string(MAIN_SCREEN, "REDY");
            }break;
            default:break;
        }
        div_disp_by_string(SECOND_SCREEN, "COPY");
        return 1;
    }
    return 0;
}

// 恢复出厂设置进入
void iot_app_menu_item_setting_reset_cb_enter(void *item)
{
    iot_app_common_data_reset_flag = 0;
    div_disp_by_string(MAIN_SCREEN, " OFF");
    div_disp_by_string(SECOND_SCREEN, "RSET");
}

// 恢复出厂设置退出
void iot_app_menu_item_setting_reset_cb_exit(void *item)
{
}

// 恢复出厂设置按键处理
uint8_t iot_app_menu_item_setting_reset_cb_click_key(void *item, uint8_t keys)
{
    if (keys & (HAL_KEY_RIGHT_DOWN | HAL_KEY_LEFT_UP))
    {
        iot_app_common_data_reset_flag = !iot_app_common_data_reset_flag;
        iot_app_data_flash_store();
        if (!iot_app_common_data_reset_flag)
        {
            div_disp_by_string(MAIN_SCREEN, " OFF");
        }
        else
        {
            div_disp_by_string(MAIN_SCREEN, "  ON");
        }
        div_disp_by_string(SECOND_SCREEN, "RSET");
        return 1;
    }
    return 0;
}

// 菜单模式进入
void iot_app_menu_item_setting_normal_cb_enter(void *item)
{
    div_disp_by_string(MAIN_SCREEN, "NoRM");
    div_disp_by_string(SECOND_SCREEN, "MODE");
}

// 菜单模式退出
void iot_app_menu_item_setting_normal_cb_exit(void *item)
{
    
}

// 菜单模式按键处理
uint8_t iot_app_menu_item_setting_normal_cb_click_key(void *item, uint8_t keys)
{
    if (keys & HAL_KEY_MODE)
    {
        iot_app_menu_enter_child_menu();
        return 1;
    }
    return 0;
}

// out1mode进入
void iot_app_menu_item_setting_out1mode_cb_enter(void *item)
{
    switch (iot_app_common_data_output1_flag)
    {
        case IOT_APP_COMMON_DATA_OUTPUT1_EASY:
        {
            div_disp_by_string(MAIN_SCREEN, "EASY");
        }break;
        case IOT_APP_COMMON_DATA_OUTPUT1_HYSTERESIS:
        {
            div_disp_by_string(MAIN_SCREEN, " HYS");
        }break;
        case IOT_APP_COMMON_DATA_OUTPUT1_WINDOW_COMPARE:
        {
            div_disp_by_string(MAIN_SCREEN, "WCMP");
        }break;
        default:break;
    }
    div_disp_by_string(SECOND_SCREEN, "Out1");
}

// out1mode退出
void iot_app_menu_item_setting_out1mode_cb_exit(void *item)
{
    
}

// out1mode按键处理
uint8_t iot_app_menu_item_setting_out1mode_cb_click_key(void *item, uint8_t keys)
{
    if (keys & (HAL_KEY_RIGHT_DOWN | HAL_KEY_LEFT_UP))
    {
        if (keys & HAL_KEY_LEFT_UP)
        {
            if (++iot_app_common_data_output1_flag > 2)
            {
                iot_app_common_data_output1_flag = 0;
            }
        }
        else if (keys & HAL_KEY_RIGHT_DOWN)
        {
            if (--iot_app_common_data_output1_flag == 255)
            {
                iot_app_common_data_output1_flag = 2;
            }
        }
        iot_app_data_flash_store();
        switch(iot_app_common_data_output1_flag)
        {
            case IOT_APP_COMMON_DATA_OUTPUT1_EASY:
            {
                div_disp_by_string(MAIN_SCREEN, "EASY");
            }break;
            case IOT_APP_COMMON_DATA_OUTPUT1_HYSTERESIS:
            {
                div_disp_by_string(MAIN_SCREEN, " HYS");
            }break;
            case IOT_APP_COMMON_DATA_OUTPUT1_WINDOW_COMPARE:
            {
                div_disp_by_string(MAIN_SCREEN, "WCMP");
            }break;
            default:break;
        }
        div_disp_by_string(SECOND_SCREEN, "Out1");
        return 1;
    }
    return 0;
}

// out2mode进入
void iot_app_menu_item_setting_out2mode_cb_enter(void *item)
{
    switch (iot_app_common_data_output2_flag)
    {
        case IOT_APP_COMMON_DATA_OUTPUT2_OFF:
        {
            div_disp_by_string(MAIN_SCREEN, " OFF");
        }break;
        case IOT_APP_COMMON_DATA_OUTPUT2_EASY:
        {
            div_disp_by_string(MAIN_SCREEN, "EASY");
        }break;
        case IOT_APP_COMMON_DATA_OUTPUT2_HYSTERESIS:
        {
            div_disp_by_string(MAIN_SCREEN, " HYS");
        }break;
        case IOT_APP_COMMON_DATA_OUTPUT2_WINDOW_COMPARE:
        {
            div_disp_by_string(MAIN_SCREEN, "WCMP");
        }break;
        default:break;
    }
    div_disp_by_string(SECOND_SCREEN, "Out2");
}

// out2mode退出
void iot_app_menu_item_setting_out2mode_cb_exit(void *item)
{
    
}

// out2mode按键处理
uint8_t iot_app_menu_item_setting_out2mode_cb_click_key(void *item, uint8_t keys)
{
    if (keys & (HAL_KEY_RIGHT_DOWN | HAL_KEY_LEFT_UP))
    {
        if (keys & HAL_KEY_LEFT_UP)
        {
            if (++iot_app_common_data_output2_flag > 3)
            {
                iot_app_common_data_output2_flag = 0;
            }
        }
        else if (keys & HAL_KEY_RIGHT_DOWN)
        {
            if (--iot_app_common_data_output2_flag == 255)
            {
                iot_app_common_data_output2_flag = 3;
            }
        }
        iot_app_data_flash_store();
        switch (iot_app_common_data_output2_flag)
        {
            case IOT_APP_COMMON_DATA_OUTPUT2_OFF:
            {
                div_disp_by_string(MAIN_SCREEN, " OFF");
            }break;
            case IOT_APP_COMMON_DATA_OUTPUT2_EASY:
            {
                div_disp_by_string(MAIN_SCREEN, "EASY");
            }break;
            case IOT_APP_COMMON_DATA_OUTPUT2_HYSTERESIS:
            {
                div_disp_by_string(MAIN_SCREEN, " HYS");
            }break;
            case IOT_APP_COMMON_DATA_OUTPUT2_WINDOW_COMPARE:
            {
                div_disp_by_string(MAIN_SCREEN, "WCMP");
            }break;
            default:break;
        }
        div_disp_by_string(SECOND_SCREEN, "Out2");
        return 1;
    }
    return 0;
}

// nonc输出进入
void iot_app_menu_item_setting_nonc_output_cb_enter(void *item)
{
    if (iot_app_common_data_output2_flag)
    {
        switch (iot_app_common_data_nonc_output_flag)
        {
            case IOT_APP_COMMON_DATA_NONC_O2ON_NORMAL_OPEN:
            {
                div_disp_by_string(MAIN_SCREEN, "1o2o");
            }break;
            case IOT_APP_COMMON_DATA_NONC_O2ON_NORMAL_CLOSE:
            {
                div_disp_by_string(MAIN_SCREEN, "1c2c");
            }break;
            case IOT_APP_COMMON_DATA_NONC_O2ON_OPEN_CLOSE:
            {
                div_disp_by_string(MAIN_SCREEN, "1o2c");
            }break;
            case IOT_APP_COMMON_DATA_NONC_O2ON_CLOSE_OPEN:
            {
                div_disp_by_string(MAIN_SCREEN, "1c2o");
            }break;
            default:break;
        }
        div_disp_out1andpoint(P1,1);
        div_disp_out1andpoint(P3,1);
    }
    else
    {
        if (iot_app_common_data_nonc_output_flag == 2)
        {
            iot_app_common_data_nonc_output_flag = 0;
        }
        if (iot_app_common_data_nonc_output_flag == 3)
        {
            iot_app_common_data_nonc_output_flag = 1;
        }
        switch (iot_app_common_data_nonc_output_flag)
        {
            case IOT_APP_COMMON_DATA_NONC_O2OFF_NORMAL_OPEN:
            {
                div_disp_by_string(MAIN_SCREEN, " 1No");
            }break;
            case IOT_APP_COMMON_DATA_NONC_O2OFF_NORMAL_CLOSE:
            {
                div_disp_by_string(MAIN_SCREEN, " 1Nc");
            }break;
            default:break;
        }
        
        main_screen_disp_clear_point();
    }
    div_disp_by_string(SECOND_SCREEN, "NoNc");
    
    //= 立刻刷新nonc设置
    #ifdef _IOT_APP_DATA_PROCESSING_AND_OUTPUT_CONTROL_H
    out_put_nonc_set_output_flashonce();
    #endif
}

// nonc输出退出
void iot_app_menu_item_setting_nonc_output_cb_exit(void *item)
{
    if (iot_app_common_data_output2_flag)
    {
        div_disp_out1andpoint(P1,0);
        div_disp_out1andpoint(P3,0);
    }
    else
    {
        div_disp_out1andpoint(P3,0);
    }
}

// nonc按键处理
uint8_t iot_app_menu_item_setting_nonc_output_cb_click_key(void *item, uint8_t keys)
{
    if (keys & (HAL_KEY_LEFT_UP | HAL_KEY_RIGHT_DOWN))
    {
        if (keys & HAL_KEY_LEFT_UP)
        {
            if (iot_app_common_data_output2_flag)
            {
                if (++iot_app_common_data_nonc_output_flag > 3)
                {
                    iot_app_common_data_nonc_output_flag = 0;
                }
            }
            else
            {
                if (++iot_app_common_data_nonc_output_flag > 1)
                {
                    iot_app_common_data_nonc_output_flag = 0;
                }
            }
        }
        if (keys & HAL_KEY_RIGHT_DOWN)
        {
            if (iot_app_common_data_output2_flag)
            {
                if (--iot_app_common_data_nonc_output_flag == 255)
                {
                    iot_app_common_data_nonc_output_flag = 3;
                }
            }
            else
            {
                if (--iot_app_common_data_nonc_output_flag == 255)
                {
                    iot_app_common_data_nonc_output_flag = 1;
                }
            }
        }
        iot_app_data_flash_store();
        if (iot_app_common_data_output2_flag)
        {
            switch (iot_app_common_data_nonc_output_flag)
            {
                case IOT_APP_COMMON_DATA_NONC_O2ON_NORMAL_OPEN:
                {
                    div_disp_by_string(MAIN_SCREEN, "1o2o");
                }break;
                case IOT_APP_COMMON_DATA_NONC_O2ON_NORMAL_CLOSE:
                {
                    div_disp_by_string(MAIN_SCREEN, "1c2c");
                }break;
                case IOT_APP_COMMON_DATA_NONC_O2ON_OPEN_CLOSE:
                {
                    div_disp_by_string(MAIN_SCREEN, "1o2c");
                }break;
                case IOT_APP_COMMON_DATA_NONC_O2ON_CLOSE_OPEN:
                {
                    div_disp_by_string(MAIN_SCREEN, "1c2o");
                }break;
                default:break;
            }
            div_disp_out1andpoint(P1,1);
            div_disp_out1andpoint(P3,1);
        }
        else
        {
            switch (iot_app_common_data_nonc_output_flag)
            {
                case IOT_APP_COMMON_DATA_NONC_O2OFF_NORMAL_OPEN:
                {
                    div_disp_by_string(MAIN_SCREEN, " 1No");
                }break;
                case IOT_APP_COMMON_DATA_NONC_O2OFF_NORMAL_CLOSE:
                {
                    div_disp_by_string(MAIN_SCREEN, " 1Nc");
                }break;
                default:break;
            }
            main_screen_disp_clear_point();
        }
        
        //= 立刻刷新nonc设置
        #ifdef _IOT_APP_DATA_PROCESSING_AND_OUTPUT_CONTROL_H
        out_put_nonc_set_output_flashonce();
        #endif

        return 1;
    }
    return 0;
}

// 反应时间进入
void iot_app_menu_item_setting_response_time_cb_enter(void *item)
{
    switch (iot_app_common_data_response_time_flag)
    {
        case IOT_APP_COMMON_DATA_RESPONSE_TIME_2500US:
        {
            div_disp_by_string(MAIN_SCREEN, "  25");
        }break;
        case IOT_APP_COMMON_DATA_RESPONSE_TIME_5MS:
        {
            div_disp_by_string(MAIN_SCREEN, "   5");
        }break;
        case IOT_APP_COMMON_DATA_RESPONSE_TIME_10MS:
        {
            div_disp_by_string(MAIN_SCREEN, "  10");
        }break;
        case IOT_APP_COMMON_DATA_RESPONSE_TIME_25MS:
        {
            div_disp_by_string(MAIN_SCREEN, "  25");
        }break;
        case IOT_APP_COMMON_DATA_RESPONSE_TIME_50MS:
        {
            div_disp_by_string(MAIN_SCREEN, "  50");
        }break;
        case IOT_APP_COMMON_DATA_RESPONSE_TIME_100MS:
        {
            div_disp_by_string(MAIN_SCREEN, " 100");
        }break;
        case IOT_APP_COMMON_DATA_RESPONSE_TIME_250MS:
        {
            div_disp_by_string(MAIN_SCREEN, " 250");
        }break;
        case IOT_APP_COMMON_DATA_RESPONSE_TIME_500MS:
        {
            div_disp_by_string(MAIN_SCREEN, " 500");
        }break;
        case IOT_APP_COMMON_DATA_RESPONSE_TIME_1S:
        {
            div_disp_by_string(MAIN_SCREEN, "1000");
        }break;
        case IOT_APP_COMMON_DATA_RESPONSE_TIME_5S:
        {
            div_disp_by_string(MAIN_SCREEN, "5000");
        }break;
        default:break;
    }
    if (iot_app_common_data_response_time_flag)
    {
        div_disp_out1andpoint(P3, 0);
    }
    else
    {
        div_disp_out1andpoint(P3, 1);
    }
    div_disp_by_string(SECOND_SCREEN, " Res");
}

// 反应时间退出
void iot_app_menu_item_setting_response_time_cb_exit(void *item)
{
    div_disp_out1andpoint(P3, 0);
}

// 反应时间按键处理
uint8_t iot_app_menu_item_setting_response_time_cb_click_key(void *item, uint8_t keys)
{
    if (keys & (HAL_KEY_LEFT_UP | HAL_KEY_RIGHT_DOWN))
    {
        if (keys & HAL_KEY_LEFT_UP)
        {
            if (++iot_app_common_data_response_time_flag > 9)
            {
                iot_app_common_data_response_time_flag = 0;
            }
        }
        if (keys & HAL_KEY_RIGHT_DOWN)
        {
            if (--iot_app_common_data_response_time_flag == 255)
            {
                iot_app_common_data_response_time_flag = 9;
            }
        }
        iot_app_data_flash_store();
        switch (iot_app_common_data_response_time_flag)
        {
            case IOT_APP_COMMON_DATA_RESPONSE_TIME_2500US:
            {
                div_disp_by_string(MAIN_SCREEN, "  25");
            }break;
            case IOT_APP_COMMON_DATA_RESPONSE_TIME_5MS:
            {
                div_disp_by_string(MAIN_SCREEN, "   5");
            }break;
            case IOT_APP_COMMON_DATA_RESPONSE_TIME_10MS:
            {
                div_disp_by_string(MAIN_SCREEN, "  10");
            }break;
            case IOT_APP_COMMON_DATA_RESPONSE_TIME_25MS:
            {
                div_disp_by_string(MAIN_SCREEN, "  25");
            }break;
            case IOT_APP_COMMON_DATA_RESPONSE_TIME_50MS:
            {
                div_disp_by_string(MAIN_SCREEN, "  50");
            }break;
            case IOT_APP_COMMON_DATA_RESPONSE_TIME_100MS:
            {
                div_disp_by_string(MAIN_SCREEN, " 100");
            }break;
            case IOT_APP_COMMON_DATA_RESPONSE_TIME_250MS:
            {
                div_disp_by_string(MAIN_SCREEN, " 250");
            }break;
            case IOT_APP_COMMON_DATA_RESPONSE_TIME_500MS:
            {
                div_disp_by_string(MAIN_SCREEN, " 500");
            }break;
            case IOT_APP_COMMON_DATA_RESPONSE_TIME_1S:
            {
                div_disp_by_string(MAIN_SCREEN, "1000");
            }break;
            case IOT_APP_COMMON_DATA_RESPONSE_TIME_5S:
            {
                div_disp_by_string(MAIN_SCREEN, "5000");
            }break;
            default:break;
        }
        if (iot_app_common_data_response_time_flag)
        {
            div_disp_out1andpoint(P3, 0);
        }
        else
        {
            div_disp_out1andpoint(P3, 1);
        }
        div_disp_by_string(SECOND_SCREEN, " Res");
        return 1;
    }
    return 0;
}

// 屏幕文字颜色进入
void iot_app_menu_item_setting_screen_text_color_cb_enter(void *item)
{
    switch (iot_app_common_data_screen_text_color_flag)
    {
        case IOT_APP_COMMON_DATA_SCREEN_TEXT_COLOR_RON:
        {
            div_disp_by_string(MAIN_SCREEN, "R-ON");
        }break;
        case IOT_APP_COMMON_DATA_SCREEN_TEXT_COLOR_GON:
        {
            div_disp_by_string(MAIN_SCREEN, "G-ON");
        }break;
        case IOT_APP_COMMON_DATA_SCREEN_TEXT_COLOR_RED:
        {
            div_disp_by_string(MAIN_SCREEN, " RED");
        }break;
        case IOT_APP_COMMON_DATA_SCREEN_TEXT_COLOR_GREEN:
        {
            div_disp_by_string(MAIN_SCREEN, "GREN");
        }break;
        default:break;
    }
    div_disp_by_string(SECOND_SCREEN, "CLOR");
}

// 屏幕文字颜色退出
void iot_app_menu_item_setting_screen_text_color_cb_exit(void *item)
{
}

// 屏幕文字颜色按键处理
uint8_t iot_app_menu_item_setting_screen_text_color_cb_click_key(void *item, uint8_t keys)
{
    if (keys | (HAL_KEY_LEFT_UP | HAL_KEY_RIGHT_DOWN))
    {
        if (keys & HAL_KEY_LEFT_UP)
        {
            if (++iot_app_common_data_screen_text_color_flag > 3)
            {
                iot_app_common_data_screen_text_color_flag = 0;
            }
        }
        if (keys & HAL_KEY_RIGHT_DOWN)
        {
            if (--iot_app_common_data_screen_text_color_flag == 255)
            {
                iot_app_common_data_screen_text_color_flag = 3;
            }
        }
        iot_app_data_flash_store();
        switch (iot_app_common_data_screen_text_color_flag)
        {
            case IOT_APP_COMMON_DATA_SCREEN_TEXT_COLOR_RON:
            {
                div_disp_by_string(MAIN_SCREEN, "R-ON");
            }break;
            case IOT_APP_COMMON_DATA_SCREEN_TEXT_COLOR_GON:
            {
                div_disp_by_string(MAIN_SCREEN, "G-ON");
            }break;
            case IOT_APP_COMMON_DATA_SCREEN_TEXT_COLOR_RED:
            {
                div_disp_by_string(MAIN_SCREEN, " RED");
            }break;
            case IOT_APP_COMMON_DATA_SCREEN_TEXT_COLOR_GREEN:
            {
                div_disp_by_string(MAIN_SCREEN, "GREN");
            }break;
            default:break;
        }
        div_disp_by_string(SECOND_SCREEN, "CLOR");
        iot_app_data_flash_store();                     // 将变化写入flash
        return 1;
    }
    return 0;
}

// 单位转换进入
void iot_app_menu_item_setting_pressure_unit_cb_enter(void *item)
{
    switch (iot_app_common_data_pressure_unit_flag)
    {
        case IOT_APP_COMMON_DATA_PRESSURE_UNIT_KPA:
        {
            div_disp_by_string(MAIN_SCREEN, " KPA");
        }break;
        case IOT_APP_COMMON_DATA_PRESSURE_UNIT_KGF:
        {
            div_disp_by_string(MAIN_SCREEN, " KGF");
        }break;
        case IOT_APP_COMMON_DATA_PRESSURE_UNIT_BAR:
        {
            div_disp_by_string(MAIN_SCREEN, " bAR");
        }break;
        case IOT_APP_COMMON_DATA_PRESSURE_UNIT_PSI:
        {
            div_disp_by_string(MAIN_SCREEN, " PSI");
        }break;
        case IOT_APP_COMMON_DATA_PRESSURE_UNIT_MMHG:
        {
            div_disp_by_string(MAIN_SCREEN, "MMHG");
        }break;
        case IOT_APP_COMMON_DATA_PRESSURE_UNIT_INHG:
        {
            div_disp_by_string(MAIN_SCREEN, "1NHG");
        }break;
        case IOT_APP_COMMON_DATA_PRESSURE_UNIT_MPA:
        {
            div_disp_by_string(MAIN_SCREEN, " MPA");
        }break;
        default:break;
    }
    div_disp_by_string(SECOND_SCREEN, "Unit");
}

// 单位转换退出
void iot_app_menu_item_setting_pressure_unit_cb_exit(void *item)
{
}

// 单位转换按键处理
uint8_t iot_app_menu_item_setting_pressure_unit_cb_click_key(void *item, uint8_t keys)
{
    if (keys & (HAL_KEY_LEFT_UP | HAL_KEY_RIGHT_DOWN))
    {
        if (keys & HAL_KEY_LEFT_UP)
        {
            //= 修改此处为6，没有第七个单位
            if (++iot_app_common_data_pressure_unit_flag > 5)
            {
                iot_app_common_data_pressure_unit_flag = 0;
            }
        }
        if (keys & HAL_KEY_RIGHT_DOWN)
        {
            if (--iot_app_common_data_pressure_unit_flag == 255)
            {
                iot_app_common_data_pressure_unit_flag = 5;
            }
        }
        iot_app_data_flash_store();
        switch (iot_app_common_data_pressure_unit_flag)
        {
            case IOT_APP_COMMON_DATA_PRESSURE_UNIT_KPA:
            {
                div_disp_by_string(MAIN_SCREEN, " Kpa");
            }break;
            case IOT_APP_COMMON_DATA_PRESSURE_UNIT_KGF:
            {
                div_disp_by_string(MAIN_SCREEN, " Kgf");
            }break;
            case IOT_APP_COMMON_DATA_PRESSURE_UNIT_BAR:
            {
                div_disp_by_string(MAIN_SCREEN, " bar");
            }break;
            case IOT_APP_COMMON_DATA_PRESSURE_UNIT_PSI:
            {
                div_disp_by_string(MAIN_SCREEN, " PsI");
            }break;
            case IOT_APP_COMMON_DATA_PRESSURE_UNIT_MMHG:
            {
                div_disp_by_string(MAIN_SCREEN, "mmHG");
            }break;
            case IOT_APP_COMMON_DATA_PRESSURE_UNIT_INHG:
            {
                div_disp_by_string(MAIN_SCREEN, "1nHG");
            }break;
            case IOT_APP_COMMON_DATA_PRESSURE_UNIT_MPA:
            {
                div_disp_by_string(MAIN_SCREEN, " Mpa");
            }break;
            default:break;
        }
        div_disp_by_string(SECOND_SCREEN, "Unit");
        return 1;
    }
    return 0;
}

uint8_t iot_app_menu_item_level_2_cb_click_key(void *item, uint8_t keys)
{
    if (keys & HAL_KEY_MODE)
    {
        if (iot_app_menu.current == iot_app_menu_item_setting_pro)
        {
            if (!subsidiary_display_flag)
            {
                iot_app_menu_switch_next();
            }
        }
        else if (iot_app_menu.current == iot_app_menu_item_setting_eco)
        {
            switch (iot_app_common_data_eco_flag)
            {
                case IOT_APP_COMMON_DATA_ECO_OFF:
                {
                    iot_app_common_data_screen_set_brightness(100);
                    IOT_APP_BAROMETER_DISP_EVENT_ECO_FULL_STOP();
                }break;
                case IOT_APP_COMMON_DATA_ECO_STD:
                {
                    iot_app_common_data_screen_set_brightness(50);
                    IOT_APP_BAROMETER_DISP_EVENT_ECO_FULL_STOP();
                }break;
                case IOT_APP_COMMON_DATA_ECO_FULL:
                {
                    iot_app_common_data_screen_set_brightness(100);
                    iot_app_common_data_eco_count = 0;
                    IOT_APP_BAROMETER_DISP_EVENT_ECO_FULL_PERIOD_START();
                }break;
                default:break;
            }
            iot_app_menu_switch_next();
        }
        else if ((iot_app_menu.current == iot_app_menu_item_setting_pressure_unit) || (iot_app_menu.current == iot_app_menu_item_setting_reset))
        {
            //= 恢复出厂设置
            if (iot_app_menu.current == iot_app_menu_item_setting_reset && iot_app_common_data_reset_flag)
            {
                iot_app_data_flash_erase();
                iot_app_data_flash_restore();

                #ifdef _IOT_APP_DATA_PROCESSING_AND_OUTPUT_CONTROL_H
                flash_now_set();
                out_put_nonc_set_output_flashonce();
                
                zero_calibration_flag = 0;
                zero_calibration_value = 0.0;
                #endif

            }
            iot_app_menu_stop();
            iot_app_common_data_system_state_set(IOT_APP_COMMON_DATA_SYSTEM_STATE_NORMAL);
        }
        else
        {
            iot_app_menu_switch_next();
        }
        return 1;
    }
    if (keys & (HAL_KEY_RIGHT_DOWN | HAL_KEY_LEFT_UP))
    {
    }
    return 0;
}

uint8_t iot_app_menu_item_level_1_cb_click_key(void *item, uint8_t keys)
{
    return 0;
}

//= 按键自锁回调
uint8_t iot_app_barometer_disp_key_lock(uint8_t cur_keys, uint8_t click_keys, uint8_t hold_keys, uint8_t release_keys, uint32_t poll_time_milliseconds)
{
    static uint8_t hold_flag = 0;

    // 长按2s进入按键锁定
    if ((hold_keys & (HAL_KEY_MODE | HAL_KEY_RIGHT_DOWN)) && 
        (hal_key_press_time_count[HAL_KEY_MODE_INDEX] >= (2000/poll_time_milliseconds)) &&
        (hal_key_press_time_count[HAL_KEY_RIGHT_DOWN_INDEX] >= (2000/poll_time_milliseconds)) &&
        !(cur_keys & HAL_KEY_LEFT_UP))
    {
        if (!hold_flag)
        {
            hold_flag = 1;
            // 按键未锁时
            if (!iot_app_barometer_disp_key_lock_flag)
            {
                iot_app_common_data_system_state_set(IOT_APP_COMMON_DATA_SYSTEM_STATE_KEY_LOCK);
                
                iot_app_barometer_disp_key_lock_flag = IOT_APP_BAROMETER_DISP_KEY_STATE_LOCK;
                
                iot_app_barometer_disp_event_display_state = IOT_APP_BAROMETER_DISP_EVENT_DISPLAY_STATE_KEY_LOCK;
                IOT_APP_BAROMETER_DISP_EVENT_DISPLAY_PERIOD_START();
            }
            // 按键锁定时
            else
            {
                iot_app_barometer_disp_key_lock_flag = IOT_APP_BAROMETER_DISP_KEY_STATE_UNLOCK;
                
                iot_app_barometer_disp_event_display_state = IOT_APP_BAROMETER_DISP_EVENT_DISPLAY_STATE_KEY_LOCK;
                IOT_APP_BAROMETER_DISP_EVENT_DISPLAY_PERIOD_START();
            }
        }
    }
    else
    {
        hold_flag = 0;
    }
    if (iot_app_data_system_state == IOT_APP_COMMON_DATA_SYSTEM_STATE_KEY_LOCK || hold_flag)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}


//= 菜单回调
uint8_t iot_app_barometer_disp_menu(uint8_t cur_keys, uint8_t click_keys, uint8_t hold_keys, uint8_t release_keys, uint32_t poll_time_milliseconds)
{
    static uint8 iot_app_is_after_2spress = 0;
    static uint8 iot_app_is_after_4spress = 0;
    
    static uint8_t hold_flag = 0;

    if (iot_app_data_system_state == IOT_APP_COMMON_DATA_SYSTEM_STATE_NORMAL)
    {
        if(hold_keys)
        {
             if (!hold_flag)
            {
                //-- 非菜单模式，判断是否进入菜单模式
                if ((hold_keys & HAL_KEY_MODE) 
                    && (hal_key_press_time_count[HAL_KEY_MODE_INDEX] >= (1000/poll_time_milliseconds))
                    && !(cur_keys & (HAL_KEY_RIGHT_DOWN|HAL_KEY_LEFT_UP))
                    && !(iot_app_is_after_2spress))
                {

                    iot_app_is_after_2spress = 1;
                    
                    #ifdef HAL_BOARD_CFG_H
                    HAL_LED1_TURN_ON();
                    HAL_LED3_TURN_ON();
                    #endif
                    
                    #ifdef _IOT_APP_DATA_PROCESSING_AND_OUTPUT_CONTROL_H
                    second_screen_disp_clear_point();
                    main_screen_disp_clear_point();
                    #endif
                    
                    iot_app_menu_start();
                    
                    iot_app_menu_enter_child_menu();
                    
                    return 1;
                }
                else if ((hold_keys & HAL_KEY_MODE) && 
                    (hal_key_press_time_count[HAL_KEY_MODE_INDEX] >= (2500/poll_time_milliseconds)) &&
                    !(cur_keys & (HAL_KEY_RIGHT_DOWN|HAL_KEY_LEFT_UP)) && !(iot_app_is_after_4spress))
                {
                   
                    iot_app_is_after_4spress = 1;
                    
                    iot_app_menu_switch_next();

                    hold_flag = 1;
                    
                    return 1;
                }
            }
        }
        else
        {
            hold_flag = 0;
        }
        
        if (release_keys)
        {   
            if(release_keys & HAL_KEY_MODE)
            {
                if (iot_app_is_after_4spress)
                {
                    iot_app_common_data_system_state_set(IOT_APP_COMMON_DATA_SYSTEM_STATE_MENU);
                    
                    iot_app_is_after_2spress = 0;
                    iot_app_is_after_4spress = 0;
                }
                else if (iot_app_is_after_2spress)
                {
                    iot_app_common_data_system_state_set(IOT_APP_COMMON_DATA_SYSTEM_STATE_MENU);
                    
                    iot_app_is_after_2spress = 0;
                }
                else
                {
                    iot_app_is_after_2spress = 0;
                    iot_app_is_after_4spress = 0;
                }
            }
        }
    }
    

    if (iot_app_data_system_state == IOT_APP_COMMON_DATA_SYSTEM_STATE_MENU)
    {
        if ((hold_keys & HAL_KEY_MODE) && 
            (hal_key_press_time_count[HAL_KEY_MODE_INDEX] >= (1000/poll_time_milliseconds)) &&
            !(cur_keys & (HAL_KEY_RIGHT_DOWN|HAL_KEY_LEFT_UP)))
        {
            if (!hold_flag)
            {
                // 长按退出pro模式时保存eco设置
                if (iot_app_menu.current == iot_app_menu_item_setting_eco)
                {
                    switch (iot_app_common_data_eco_flag)
                    {
                        case IOT_APP_COMMON_DATA_ECO_OFF:
                        {
                            iot_app_common_data_screen_set_brightness(100);
                            IOT_APP_BAROMETER_DISP_EVENT_ECO_FULL_STOP();
                        }break;
                        case IOT_APP_COMMON_DATA_ECO_STD:
                        {
                            iot_app_common_data_screen_set_brightness(50);
                            IOT_APP_BAROMETER_DISP_EVENT_ECO_FULL_STOP();
                        }break;
                        case IOT_APP_COMMON_DATA_ECO_FULL:
                        {
                            iot_app_common_data_screen_set_brightness(100);
                            iot_app_common_data_eco_count = 0;
                            IOT_APP_BAROMETER_DISP_EVENT_ECO_FULL_PERIOD_START();
                        }break;
                        default:break;
                    }
                    
                    iot_app_menu_switch_next();
                    
                }
                
                //= 长按退出时打断pro设定的闪烁bug
                if (subsidiary_display_flag)
                {
                    subsidiary_display_flag = 0;
                    iot_app_menu_stop();
                    iot_app_common_data_system_state_set(IOT_APP_COMMON_DATA_SYSTEM_STATE_NORMAL);
                    iot_app_barometer_disp_event_flash_state = IOT_APP_BAROMETER_DISP_EVENT_FLASH_STATE_NORMAL;
                    IOT_APP_BAROMETER_DISP_EVENT_FLASH_STOP();
                }

                iot_app_menu_stop();
                
                iot_app_common_data_system_state_set(IOT_APP_COMMON_DATA_SYSTEM_STATE_NORMAL);
                
                hold_flag = 1;
                
                return 1;
            }
        }
        else
        {
            hold_flag = 0;
        }

        if (click_keys)
        {
            if (iot_app_menu.current->cb_click_keys_1)
            {
                iot_app_menu.current->cb_click_keys_1(iot_app_menu.current, click_keys);
            }

            if (iot_app_menu.current->cb_click_keys_2)
            {
                iot_app_menu.current->cb_click_keys_2(iot_app_menu.current, click_keys);
            }
        }

        if (hold_keys)
        {
            if (iot_app_menu.current->cb_hold_keys != NULL)
            {
                iot_app_menu.current->cb_hold_keys(iot_app_menu.current, hold_keys, hal_key_press_time_count, poll_time_milliseconds);
            }
        }

        if (release_keys)
        {
            if (iot_app_menu.current->cb_release_keys != NULL)
            {
                iot_app_menu.current->cb_release_keys(iot_app_menu.current, release_keys);
            }
            
            if(release_keys & HAL_KEY_MODE)
            {
                iot_app_is_after_2spress = 0;
            }
        }

       return 1;
    }
    if (iot_app_data_system_state == IOT_APP_COMMON_DATA_SYSTEM_STATE_MENU || hold_flag)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

//= 峰值谷值保持回调
uint8_t iot_app_barometer_disp_peak_bottom_hold(uint8_t cur_keys, uint8_t click_keys, uint8_t hold_keys, uint8_t release_keys, uint32_t poll_time_milliseconds)
{
    static uint8_t hold_flag = 0;

    if ((hold_keys & (HAL_KEY_MODE | HAL_KEY_LEFT_UP)) && 
        (hal_key_press_time_count[HAL_KEY_MODE_INDEX] >= (2000/poll_time_milliseconds)) &&
        (hal_key_press_time_count[HAL_KEY_LEFT_UP_INDEX] >= (2000/poll_time_milliseconds)) &&
        !(cur_keys & HAL_KEY_RIGHT_DOWN))
    {
        if (!hold_flag)
        {
            hold_flag = 1;
            if (iot_app_data_system_state == IOT_APP_COMMON_DATA_SYSTEM_STATE_NORMAL)
            {
                iot_app_common_data_pressure_max = iot_app_common_data_unit_conversion_pressure;
                iot_app_common_data_pressure_min = iot_app_common_data_unit_conversion_pressure;
                iot_app_common_data_system_state_set(IOT_APP_COMMON_DATA_SYSTEM_STATE_PEAK_BOTTOM_HOLD);
                iot_app_barometer_disp_event_flash_state = IOT_APP_BAROMETER_DISP_EVENT_FLASH_STATE_PEAK_BOTTOM_HOLD;
                IOT_APP_BAROMETER_DISP_EVENT_FLASH_PERIOD_START();
            }
            else
            {
                iot_app_barometer_disp_event_flash_state = IOT_APP_BAROMETER_DISP_EVENT_FLASH_STATE_NORMAL;
                IOT_APP_BAROMETER_DISP_EVENT_FLASH_STOP();
                iot_app_barometer_disp_event_display_state = IOT_APP_BAROMETER_DISP_EVENT_DISPLAY_STATE_PEAK_BOTTOM_HOLD;
                IOT_APP_BAROMETER_DISP_EVENT_DISPLAY_PERIOD_START();
            }
        }
    }
    else
    {
        hold_flag = 0;
    }
    if (iot_app_data_system_state == IOT_APP_COMMON_DATA_SYSTEM_STATE_PEAK_BOTTOM_HOLD || hold_flag)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

//= eco回调
uint8_t iot_app_barometer_disp_screen_off(uint8_t cur_keys, uint8_t click_keys, uint8_t hold_keys, uint8_t release_keys, uint32_t poll_time_milliseconds)
{
    if (cur_keys || click_keys || hold_keys || release_keys)
    {
        iot_app_common_data_eco_count = 0;
        if (!IOT_APP_BAROMETER_DISP_EVENT_ECO_FULL_IS_ACTIVE())
        {
            iot_app_common_data_screen_set_brightness(100);
            IOT_APP_BAROMETER_DISP_EVENT_ECO_FULL_PERIOD_START();
            return 1;
        }
    }
    return 0;
}
//= 校零回调
uint8_t iot_app_barometer_disp_zero_calibration(uint8_t cur_keys, uint8_t click_keys, uint8_t hold_keys, uint8_t release_keys, uint32_t poll_time_milliseconds)
{
    static uint8_t hold_flag = 0;

    if ((hold_keys & (HAL_KEY_LEFT_UP | HAL_KEY_RIGHT_DOWN)) && 
        (hal_key_press_time_count[HAL_KEY_LEFT_UP_INDEX] >= (2000/poll_time_milliseconds)) &&
        (hal_key_press_time_count[HAL_KEY_RIGHT_DOWN_INDEX] >= (2000/poll_time_milliseconds)) &&
        !(cur_keys & HAL_KEY_MODE))
    {
        if (!hold_flag)
        {
            hold_flag = 1;
            
            if(iot_app_common_data_unit_conversion_pressure<5&&iot_app_common_data_unit_conversion_pressure>-5)
            {
                iot_app_common_data_system_state_set(IOT_APP_COMMON_DATA_SYSTEM_STATE_ZERO_CALIBRATION);
                iot_app_barometer_disp_event_display_state = IOT_APP_BAROMETER_DISP_EVENT_DISPLAY_STATE_ZERO_CALIBRATION;
                IOT_APP_BAROMETER_DISP_EVENT_DISPLAY_PERIOD_START();
            }
           
        }
    }
    else
    {
        hold_flag = 0;
    }
    if (iot_app_data_system_state == IOT_APP_COMMON_DATA_SYSTEM_STATE_ZERO_CALIBRATION || hold_flag)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

//= 显示颜色联动转换
void iot_app_barometer_disp_screen_text_color(uint8_t enable)
{
    uint8 output1_noncstate = 0;
    uint8 output2_noncstate = 0;
    
    if (iot_app_common_data_output2_flag)
    {
        switch (iot_app_common_data_nonc_output_flag)
        {
            case OUTPUT_NONCSTATE_1NO2NO:
            {
                output1_noncstate = NO;
                output2_noncstate = NO;
            }break;
            case OUTPUT_NONCSTATE_1NC2NC:
            {
                output1_noncstate = NC;
                output2_noncstate = NC;
            }break;
            case OUTPUT_NONCSTATE_1NO2NC:
            {
                output1_noncstate = NO;
                output2_noncstate = NC;
            }break;
            case OUTPUT_NONCSTATE_1NC2NO:
            {
                output1_noncstate = NC;
                output2_noncstate = NO;
            }break;
            default:break;
        }
    }
    else
    {
        switch (iot_app_common_data_nonc_output_flag)
        {
            case OUTPUT_NONCSTATE_1NO:
            {
                output1_noncstate = NO;
            }break;
            case OUTPUT_NONCSTATE_1NC:
            {
                output1_noncstate = NC;
            }break;
            default:break;
        }
    }
    
    #ifdef HAL_BOARD_CFG_H
    if (enable)
    {
        switch (iot_app_common_data_screen_text_color_flag)
        {
            case IOT_APP_COMMON_DATA_SCREEN_TEXT_COLOR_RON:
            {
                // 当显示颜色联动转换选择out1通道时
                if (!iot_app_common_data_display_color_linkage_flag)
                {
                    // out1通道有输出
                    if (m_output.output_out1_state)
                    {
                         if(output1_noncstate)
                        {
                            HAL_LED1_TURN_ON();
                            HAL_LED3_TURN_OFF();
                        }
                        else
                        {
                            HAL_LED1_TURN_OFF();
                            HAL_LED3_TURN_ON();
                            
                        }
                    }
                    else
                    {
                         if(output1_noncstate)
                        {
                            HAL_LED1_TURN_OFF();
                            HAL_LED3_TURN_ON(); 
                        }
                        else
                        {
                            HAL_LED1_TURN_ON();
                            HAL_LED3_TURN_OFF();
                            
                        }
                    }
                }
                // 当显示颜色联动转换选择out2通道时
                else
                {
                     if (m_output.output_out2_state)
                    {
                         if(output2_noncstate)
                        {
                            HAL_LED1_TURN_ON();
                            HAL_LED3_TURN_OFF();
                        }
                        else
                        {
                            HAL_LED1_TURN_OFF();
                            HAL_LED3_TURN_ON();
                            
                        }
                    }
                    else
                    {
                         if(output2_noncstate)
                        {
                            HAL_LED1_TURN_OFF();
                            HAL_LED3_TURN_ON(); 
                        }
                        else
                        {
                            HAL_LED1_TURN_ON();
                            HAL_LED3_TURN_OFF();
                            
                        }
                    }
                }
            }break;
            case IOT_APP_COMMON_DATA_SCREEN_TEXT_COLOR_GON:
            {
                if (!iot_app_common_data_display_color_linkage_flag)
                {
                    if (m_output.output_out1_state)
                    {
                         if(output1_noncstate)
                        {
                            HAL_LED1_TURN_OFF();
                            HAL_LED3_TURN_ON();
                        }
                        else
                        {
                            HAL_LED1_TURN_ON();
                            HAL_LED3_TURN_OFF();
                            
                        }
                    }
                    else
                    {
                         if(output1_noncstate)
                        {
                            HAL_LED1_TURN_ON();
                            HAL_LED3_TURN_OFF();
                        }
                        else
                        {
                            HAL_LED1_TURN_OFF();
                            HAL_LED3_TURN_ON();
                            
                        }
                    }
                }
                else
                {
                    if (m_output.output_out2_state)
                    {
                         if(output2_noncstate)
                        {
                            HAL_LED1_TURN_OFF();
                            HAL_LED3_TURN_ON();  
                        }
                        else
                        {
                            HAL_LED1_TURN_ON();
                            HAL_LED3_TURN_OFF();
                            
                        }
                    }
                    else
                    {
                         if(output2_noncstate)
                        {
                            HAL_LED1_TURN_ON();
                            HAL_LED3_TURN_OFF();
                        }
                        else
                        {
                            HAL_LED1_TURN_OFF();
                            HAL_LED3_TURN_ON();
                            
                        }
                    }
                }
            }break;
            case IOT_APP_COMMON_DATA_SCREEN_TEXT_COLOR_RED:
            {
                HAL_LED1_TURN_ON();
                HAL_LED3_TURN_OFF();
            }break;
            case IOT_APP_COMMON_DATA_SCREEN_TEXT_COLOR_GREEN:
            {
                HAL_LED1_TURN_OFF();
                HAL_LED3_TURN_ON();
            }break;
            default:break;
        }
        HAL_LED2_TURN_ON();
        HAL_LED4_TURN_ON();
    }
    else
    {
        HAL_LED1_TURN_OFF();
        HAL_LED2_TURN_OFF();
        HAL_LED3_TURN_OFF();
        HAL_LED4_TURN_OFF();
    }
    if (iot_app_data_system_state == IOT_APP_COMMON_DATA_SYSTEM_STATE_MENU)
    {
        HAL_LED1_TURN_ON();
        HAL_LED2_TURN_ON();
        HAL_LED3_TURN_ON();
        HAL_LED4_TURN_ON();
    }
    #endif

}

uint8_t iot_app_barometer_disp_key_callback(uint8_t cur_keys, uint8_t pre_keys, uint32_t poll_time_milliseconds)
{
    uint8_t  k;
    uint8_t  key_mask = HAL_KEY_1;
    uint8_t  scan_flag = 1;
    uint8_t  click_keys = 0;      // 单击的按键
    uint8_t  hold_keys = 0;       // 长按超过1秒的按键
    uint8_t  release_keys = 0;

    cur_keys &= IOT_APP_BAROMETER_DISP_KEY_MASK;
    pre_keys &= IOT_APP_BAROMETER_DISP_KEY_MASK;

    //-- 此处判断按键的状态，单击或长按等等
    for (k = 0; k < 8; k++,key_mask<<=1)
    {
        IOT_WATCHDOG_RESET();

        if (!(key_mask & IOT_APP_BAROMETER_DISP_KEY_MASK))
        {
            continue;
        }
        
        if (cur_keys & key_mask)//-- 按键按下
        {
            if (hal_key_press_time_count[k] == 2)
            {
            
            }
            else if (hal_key_press_time_count[k] >= (1000/poll_time_milliseconds))  // 大于1s触发按键
            {
                hold_keys |= key_mask;
            }
        }
        else
        {
            if ((pre_keys & key_mask) && 
                //= 这里调整一下速度，按键判断太慢了
                (hal_key_press_time_count[k] > (10/poll_time_milliseconds)) &&
                (hal_key_press_time_count[k] < (1000/poll_time_milliseconds)))
            {
                //-- 松开时判断按键是否短按，10ms ~ 1s之间都算短按
                click_keys |= key_mask;
            }

            if ((pre_keys & key_mask) && (hal_key_press_time_count[k] >= 2))
            {
                release_keys |= key_mask;
            }
        }
    }

    if (iot_app_data_system_state == IOT_APP_COMMON_DATA_SYSTEM_STATE_NORMAL || iot_app_data_system_state == IOT_APP_COMMON_DATA_SYSTEM_STATE_KEY_LOCK)
    {
        if (iot_app_barometer_disp_key_lock(cur_keys, click_keys, hold_keys, release_keys, poll_time_milliseconds))
        {
            if ((click_keys & (HAL_KEY_MODE | HAL_KEY_RIGHT_DOWN | HAL_KEY_LEFT_UP)) && (iot_app_barometer_disp_key_lock_flag == IOT_APP_BAROMETER_DISP_KEY_STATE_LOCKING))
            {
                iot_app_barometer_disp_event_display_state = IOT_APP_BAROMETER_DISP_EVENT_DISPLAY_STATE_KEY_LOCK;
                IOT_APP_BAROMETER_DISP_EVENT_DISPLAY_PERIOD_START();
            }
            return scan_flag;
        }
    }

    if (iot_app_common_data_eco_flag == IOT_APP_COMMON_DATA_ECO_FULL)
    {
        if (iot_app_barometer_disp_screen_off(cur_keys, click_keys, hold_keys, release_keys, poll_time_milliseconds))
        {
            return scan_flag;
        }
    }

    if (iot_app_data_system_state == IOT_APP_COMMON_DATA_SYSTEM_STATE_NORMAL || iot_app_data_system_state == IOT_APP_COMMON_DATA_SYSTEM_STATE_ZERO_CALIBRATION)
    {
        if (iot_app_barometer_disp_zero_calibration(cur_keys, click_keys, hold_keys, release_keys, poll_time_milliseconds))
        {
            return scan_flag;
        }
    }

    if (iot_app_data_system_state == IOT_APP_COMMON_DATA_SYSTEM_STATE_NORMAL || iot_app_data_system_state == IOT_APP_COMMON_DATA_SYSTEM_STATE_PEAK_BOTTOM_HOLD)
    {
        if (iot_app_barometer_disp_peak_bottom_hold(cur_keys, click_keys, hold_keys, release_keys, poll_time_milliseconds))
        {
            return scan_flag;
        }
    }

    //-- 通过按键状态判断当前的系统状态，主状态，菜单，恢复出厂等
    if (iot_app_data_system_state == IOT_APP_COMMON_DATA_SYSTEM_STATE_NORMAL || iot_app_data_system_state == IOT_APP_COMMON_DATA_SYSTEM_STATE_MENU)
    {
        // MODE按键长按3秒，进入基础设置
        if (iot_app_barometer_disp_menu(cur_keys, click_keys, hold_keys, release_keys, poll_time_milliseconds))
        {
            return scan_flag;
        }
    }

    #ifdef _IOT_APP_DATA_PROCESSING_AND_OUTPUT_CONTROL_H
    if(iot_app_data_system_state == IOT_APP_COMMON_DATA_SYSTEM_STATE_NORMAL && iot_app_data_system_state != IOT_APP_COMMON_DATA_SYSTEM_STATE_MENU)
    {
        //= 测试副屏设置
        if (iot_app_data_processing_and_output_control_common_key_callback(cur_keys, click_keys, hold_keys, release_keys, poll_time_milliseconds))
        {
            return scan_flag;
        }
    }
    #endif 

    return scan_flag;
}

uint16_t iot_app_barometer_disp_process_event(uint8_t task_id, uint16_t events)
{
    (void)task_id; // 显式忽略未引用参数（避免编译器警告）

    // 显示动画事件
    if (events & IOT_APP_BAROMETER_DISP_EVENT_DISPLAY_EVT)
    {
        static uint16_t count = 0;

        // 开机动画
        if (iot_app_barometer_disp_event_display_state == IOT_APP_BAROMETER_DISP_EVENT_DISPLAY_STATE_BOOT_ANIMATION)
        {
            div_disp_by_string(MAIN_SCREEN, " L03");
            div_disp_by_string(SECOND_SCREEN, " NPN");
            if (++count > 30)
            {
                count = 0;
                hal_disp_clear_all();
                iot_app_common_data_system_state_set(IOT_APP_COMMON_DATA_SYSTEM_STATE_NORMAL);
                iot_app_barometer_disp_event_display_state = IOT_APP_BAROMETER_DISP_EVENT_DISPLAY_STATE_NORMAL;
                IOT_APP_BAROMETER_DISP_EVENT_DISPLAY_STOP();
                
                iot_app_data_open_system_init_flag = IOT_APP_SYSTEM_INIT_OK;
            }
        }

        // 按键锁定动画
        if (iot_app_barometer_disp_event_display_state == IOT_APP_BAROMETER_DISP_EVENT_DISPLAY_STATE_KEY_LOCK)
        {
            switch (iot_app_barometer_disp_key_lock_flag)
            {
                case IOT_APP_BAROMETER_DISP_KEY_STATE_UNLOCK:
                {
                    //= 清除小数点
                    div_disp_clearpoint();
                    
                    div_disp_by_string(MAIN_SCREEN, "LOCK");
                    div_disp_by_string(SECOND_SCREEN, " OFF");
                }break;
                case IOT_APP_BAROMETER_DISP_KEY_STATE_LOCK:
                {
                     //= 清除小数点
                    div_disp_clearpoint();
                    
                    div_disp_by_string(MAIN_SCREEN, "LOCK");
                    div_disp_by_string(SECOND_SCREEN, "  ON");
                }break;
                case IOT_APP_BAROMETER_DISP_KEY_STATE_LOCKING:
                {
                    //= 清除残留主屏小数点
                    main_screen_disp_clear_point();
                    div_disp_by_string(MAIN_SCREEN, "LOCK");
                }break;
                default:break;
            }
            
            if (++count > 10)
            {
                count = 0;
                
                hal_disp_clear_all();
                
                iot_app_barometer_disp_event_display_state = IOT_APP_BAROMETER_DISP_EVENT_DISPLAY_STATE_NORMAL;
                
                switch (iot_app_barometer_disp_key_lock_flag)
                {
                    case IOT_APP_BAROMETER_DISP_KEY_STATE_UNLOCK:
                    {
                        
                        iot_app_common_data_system_state_set(IOT_APP_COMMON_DATA_SYSTEM_STATE_NORMAL);
                        
                    }break;
                    case IOT_APP_BAROMETER_DISP_KEY_STATE_LOCK:
                    {
                        // 开启按键锁定后更改状态，使处于锁定状态
                        iot_app_barometer_disp_key_lock_flag = IOT_APP_BAROMETER_DISP_KEY_STATE_LOCKING;
                        
                    }break;
                    case IOT_APP_BAROMETER_DISP_KEY_STATE_LOCKING:
                    {
                    }break;
                    default:break;
                }
                
                IOT_APP_BAROMETER_DISP_EVENT_DISPLAY_STOP();
            }
        }

        // 峰值谷值功能显示
        if (iot_app_barometer_disp_event_display_state == IOT_APP_BAROMETER_DISP_EVENT_DISPLAY_STATE_PEAK_BOTTOM_HOLD)
        {
            div_disp_by_string(MAIN_SCREEN, "PEAK");
            div_disp_by_string(SECOND_SCREEN, "botm");
            if (++count > 10)
            {
                count = 0;
                hal_disp_clear_all();
                // 清除小数点
                main_screen_disp_clear_point();
                second_screen_disp_clear_point();
                iot_app_common_data_system_state_set(IOT_APP_COMMON_DATA_SYSTEM_STATE_NORMAL);
                iot_app_barometer_disp_event_display_state = IOT_APP_BAROMETER_DISP_EVENT_DISPLAY_STATE_NORMAL;
                IOT_APP_BAROMETER_DISP_EVENT_DISPLAY_STOP();
            }
        }

        // 校零功能动画显示
        if (iot_app_barometer_disp_event_display_state == IOT_APP_BAROMETER_DISP_EVENT_DISPLAY_STATE_ZERO_CALIBRATION)
        {
            div_disp_by_string(MAIN_SCREEN, "0000");
            if (++count > 10)
            {
                count = 0;
                
                #ifdef _IOT_APP_DATA_PROCESSING_AND_OUTPUT_CONTROL_H
                
                zero_calibration_flag = 1;
                
                zero_calibration_value =  pressure_read_once();
                
                #endif
                
                iot_app_common_data_system_state_set(IOT_APP_COMMON_DATA_SYSTEM_STATE_NORMAL);
                iot_app_barometer_disp_event_display_state = IOT_APP_BAROMETER_DISP_EVENT_DISPLAY_STATE_NORMAL;
                IOT_APP_BAROMETER_DISP_EVENT_DISPLAY_STOP();
            }
        }

        return (events ^ IOT_APP_BAROMETER_DISP_EVENT_DISPLAY_EVT);
    }

    // 闪烁事件
    if (events & IOT_APP_BAROMETER_DISP_EVENT_FLASH_EVT)
    {
        static uint8_t flash_count = 0;

        if (iot_app_barometer_disp_event_flash_state == IOT_APP_BAROMETER_DISP_EVENT_FLASH_STATE_PEAK_BOTTOM_HOLD)
        {
            if (iot_app_common_data_pressure_max < iot_app_common_data_unit_conversion_pressure)
            {
                iot_app_common_data_pressure_max = iot_app_common_data_unit_conversion_pressure;
            }
            else if (iot_app_common_data_pressure_min > iot_app_common_data_unit_conversion_pressure)
            {
                iot_app_common_data_pressure_min = iot_app_common_data_unit_conversion_pressure;
            }
            if (!flash_count)
            {
                // 清除小数点
                main_screen_disp_clear_point();
                second_screen_disp_clear_point();
                div_disp_by_string(MAIN_SCREEN, "PEAK");
                div_disp_by_string(SECOND_SCREEN, "botm");
            }
            else
            {
                main_screen_disp_float_by_unit(iot_app_common_data_pressure_unit_flag, iot_app_common_data_pressure_max);
                second_screen_disp_float_by_unit(iot_app_common_data_pressure_unit_flag, iot_app_common_data_pressure_min);
            }
            flash_count = !flash_count;
        }

        // 次屏显示NO当左编号闪烁事件
        if (iot_app_barometer_disp_event_flash_state == IOT_APP_BAROMETER_DISP_EVENT_FLASH_STATE_NO_LEFT)
        {
            if (!flash_count)
            {
                div_disp_num(2, iot_app_common_data_data_subsidiary_display_no_left);
            }
            else
            {
                div_disp_alphabet(2, ALPHABET_NULL);
            }
            flash_count = !flash_count;
        }

        // 次屏显示NO当右编号闪烁事件
        if (iot_app_barometer_disp_event_flash_state == IOT_APP_BAROMETER_DISP_EVENT_FLASH_STATE_NO_RIGHT)
        {
            if (flash_count)
            {
                div_disp_num(3, iot_app_common_data_data_subsidiary_display_no_right);
            }
            else
            {
                div_disp_alphabet(3, ALPHABET_NULL);
            }
            flash_count = !flash_count;
        }

        return (events ^ IOT_APP_BAROMETER_DISP_EVENT_FLASH_EVT);
    }

    // 息屏事件
    if (events & IOT_APP_BAROMETER_DISP_EVENT_ECO_FULL_EVT)
    {
        if (++iot_app_common_data_eco_count > 5000)
        {
            iot_app_common_data_screen_set_brightness(0);
            IOT_APP_BAROMETER_DISP_EVENT_ECO_FULL_STOP();
        }
        return (events ^ IOT_APP_BAROMETER_DISP_EVENT_ECO_FULL_EVT);
    }

    // 丢弃未知事件
    return 0;
}

void iot_app_barometer_disp_init(uint8_t task_id)
{
    iot_app_barometer_disp_task_id = task_id;

    #if (defined HAL_READFLASH) && (HAL_READFLASH == TRUE)
    iot_app_data_flash_restore();
    #endif

    iot_app_data_system_state = IOT_APP_COMMON_DATA_SYSTEM_STATE_BOOT_ANIMATION;
    iot_app_barometer_disp_event_display_state = IOT_APP_BAROMETER_DISP_EVENT_DISPLAY_STATE_BOOT_ANIMATION;
    IOT_APP_BAROMETER_DISP_EVENT_DISPLAY_PERIOD_START();

    HalKeyCallbackRegister(iot_app_barometer_disp_key_callback);
    iot_app_menu_item_init();

}

#endif 

/*************************************************-*************************************************
*                                                END
**************************************************-*****************^******************************/
#ifdef __cplusplus
}
#endif
#endif 

