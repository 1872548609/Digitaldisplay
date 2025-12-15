#include "main.h"

#ifdef _IOT_APP_MENU_ITEM_H

/*************************************************-*************************************************
*                                             FUNCTIONS
**************************************************-*****************^******************************/
iot_app_menu_t iot_app_menu = {0};

// 进入当前菜单的子菜单（下一级）
void iot_app_menu_enter_child_menu(void) 
{
    if (iot_app_menu.current->children_count == 0)
    {
        return;
    }
    
    // 菜单退出回调
    //Menu_Execute(2);

    //-- 先执行当前菜单退出，然后再在下面执行切换菜单
    if (iot_app_menu.current->enabled && iot_app_menu.current->cb_exit)
    {
        iot_app_menu.current->cb_exit(iot_app_menu.current);
    }
    
    // 如果有记录的子菜单指针，则进入该记录的子菜单
    if (iot_app_menu.current->child_menu != NULL) 
    {
        iot_app_menu.current = iot_app_menu.current->child_menu;
    } 
    // 否则进入第一个子菜单
    else 
    {
        iot_app_menu.current = iot_app_menu.current->children[0];
    }

    //-- 切换菜单后进行一次进入回调函数
    if (iot_app_menu.current->enabled && iot_app_menu.current->cb_enter)
    {
        iot_app_menu.current->cb_enter(iot_app_menu.current);
    }
    
    // 菜单进入回调
    //Menu_Execute(1);
}

// 返回父菜单（上一级）
// record 是否记录当前子菜单（true=记录，false=不记录）
void iot_app_menu_return_parent_menu(bool record) 
{
    if (iot_app_menu.current->parent == NULL)
    {
        return;
    }
    
    // 菜单退出回调
    //Menu_Execute(2);

    //-- 先执行当前菜单退出，然后再在下面执行切换菜单
    if (iot_app_menu.current->enabled && iot_app_menu.current->cb_exit)
    {
        iot_app_menu.current->cb_exit(iot_app_menu.current);
    }
    
    // 如果需要记录，则更新父菜单的 child_menu
    if (record) 
    {
        iot_app_menu.current->parent->child_menu = iot_app_menu.current;
    }
    
    // 返回父菜单
    iot_app_menu.current = iot_app_menu.current->parent;

    //-- 切换菜单后进行一次进入回调函数
    if (iot_app_menu.current->enabled && iot_app_menu.current->cb_enter)
    {
        iot_app_menu.current->cb_enter(iot_app_menu.current);
    }
    
    // 菜单进入回调
    //Menu_Execute(1);
}

// 切换到下一个同级菜单
void iot_app_menu_switch_next(void) 
{
    iot_app_menu_item_t *parent = iot_app_menu.current->parent;
    if (parent == NULL) 
    {
        return; // 根菜单没有同级菜单
    }
    
    // 菜单退出回调
    //Menu_Execute(2);

    //-- 先执行当前菜单退出，然后再在下面执行切换菜单
    if (iot_app_menu.current->enabled && iot_app_menu.current->cb_exit)
    {
        iot_app_menu.current->cb_exit(iot_app_menu.current);
    }
 
    for (int i = 0; i < parent->children_count; i++) 
    {
        if (parent->children[i] != iot_app_menu.current) 
        {
            continue;
        }
        
        int next_index = (i + 1) % parent->children_count; // 循环切换
        iot_app_menu.current = parent->children[next_index];
        
        // 菜单进入回调
        //Menu_Execute(1);
        
        //-- 切换菜单后进行一次进入回调函数
        if (iot_app_menu.current->enabled && iot_app_menu.current->cb_enter)
        {
            iot_app_menu.current->cb_enter(iot_app_menu.current);
        }
        
        break;
    }
}

// 切换到上一个同级菜单
void iot_app_menu_switch_prev(void) 
{
    iot_app_menu_item_t *parent = iot_app_menu.current->parent;
    if (parent == NULL) 
    {
        return; // 根菜单没有同级菜单
    }
 
    // 菜单退出回调
    //Menu_Execute(2);

    //-- 先执行当前菜单退出，然后再在下面执行切换菜单
    if (iot_app_menu.current->enabled && iot_app_menu.current->cb_exit)
    {
        iot_app_menu.current->cb_exit(iot_app_menu.current);
    }
    
    for (int i = 0; i < parent->children_count; i++) 
    {
        if (parent->children[i] != iot_app_menu.current) 
        {
            continue;
        }
        
        int next_index = (i - 1 + parent->children_count) % parent->children_count; // 循环切换
        iot_app_menu.current = parent->children[next_index];
        
        // 菜单进入回调
        //Menu_Execute(1);

        //-- 切换菜单后进行一次进入回调函数
        if (iot_app_menu.current->enabled && iot_app_menu.current->cb_enter)
        {
            iot_app_menu.current->cb_enter(iot_app_menu.current);
        }
        
        break;
    }
}
// 切换到指定菜单
void iot_app_menu_designative_menu(iot_app_menu_item_t* item      )
{
    //退出当前菜单
    if (iot_app_menu.current->enabled && iot_app_menu.current->cb_exit)
    {
        iot_app_menu.current->cb_exit(iot_app_menu.current);
    }

    //设定当前菜单为指定菜单
    iot_app_menu.current = item;

    //执行菜单回调
    if (iot_app_menu.current->enabled && iot_app_menu.current->cb_enter)
    {
        iot_app_menu.current->cb_enter(iot_app_menu.current);
    }
}

// 菜单系统是否启动
bool iot_app_menu_is_active(void)
{
    return iot_app_menu.running;
}
 
// 停止菜单系统
void iot_app_menu_stop(void) 
{
    if (iot_app_menu.running) 
    {
        iot_app_menu.current = iot_app_menu.root;
        iot_app_menu.running = false;
    }
}

// 启动菜单系统
void iot_app_menu_start(void) 
{
    if (!iot_app_menu.running) 
    {
        iot_app_menu.current = iot_app_menu.root;
        iot_app_menu.running = true;
    }
}



//iot_app_menu_t iot_app_menu = {0};
//iot_app_menu_item_t* iot_app_menu_item_root;
//iot_app_menu_item_t* iot_app_menu_item_setting_power;
//iot_app_menu_item_t* iot_app_menu_item_mode_fine;
//iot_app_menu_item_t* iot_app_menu_item_mode_high_speed;
//iot_app_menu_item_t* iot_app_menu_item_mode_mega;
//iot_app_menu_item_t* iot_app_menu_item_mode_super;
//iot_app_menu_item_t* iot_app_menu_item_setting_att;
//iot_app_menu_item_t* iot_app_menu_item_setting_sta;
//iot_app_menu_item_t* iot_app_menu_item_setting_end;
//iot_app_menu_item_t* iot_app_menu_item_stg_fun;
//iot_app_menu_item_t* iot_app_menu_item_stg_fun_dtec;
//iot_app_menu_item_t* iot_app_menu_item_stg_disp;
//iot_app_menu_item_t* iot_app_menu_item_stg_disp_rev;
//iot_app_menu_item_t* iot_app_menu_item_stg_disp_acct;
//iot_app_menu_item_t* iot_app_menu_item_stg_sys;
//iot_app_menu_item_t* iot_app_menu_item_setting_end_end;

// 菜单项变量位置
iot_app_menu_item_t* iot_app_menu_item_root;
    iot_app_menu_item_t* iot_app_menu_item_setting_normal;
        iot_app_menu_item_t* iot_app_menu_item_setting_out1mode;
        iot_app_menu_item_t* iot_app_menu_item_setting_out2mode;
        iot_app_menu_item_t* iot_app_menu_item_setting_nonc_output;
        iot_app_menu_item_t* iot_app_menu_item_setting_response_time;
        iot_app_menu_item_t* iot_app_menu_item_setting_screen_text_color;
        iot_app_menu_item_t* iot_app_menu_item_setting_pressure_unit;
    iot_app_menu_item_t* iot_app_menu_item_setting_pro;
        iot_app_menu_item_t* iot_app_menu_item_setting_subsidiary_display;
        iot_app_menu_item_t* iot_app_menu_item_setting_display_speed;
        iot_app_menu_item_t* iot_app_menu_item_setting_hysteresis_level;
        iot_app_menu_item_t* iot_app_menu_item_setting_display_color_linkage;
        iot_app_menu_item_t* iot_app_menu_item_setting_eco;
        iot_app_menu_item_t* iot_app_menu_item_setting_copy;
        iot_app_menu_item_t* iot_app_menu_item_setting_reset;

// 统一创建菜单树，返回树根 ，注册回调
void iot_app_menu_create_all_item(void) 
{
    // 1 创建根项
    iot_app_menu_item_root = osal_mem_alloc(sizeof(iot_app_menu_item_t));
    memset((uint8*)iot_app_menu_item_root, 0, sizeof(iot_app_menu_item_t));
    iot_app_menu_item_root->text = IOT_APP_MENU_ITEM_TEXT_STRING_ROOT;
    iot_app_menu_item_root->enabled = true;
    iot_app_menu_item_root->level = 0;
    iot_app_menu_item_root->cb_enter = NULL;
    iot_app_menu_item_root->cb_exit  = NULL;
    iot_app_menu_item_root->cb_click_keys_1 = NULL;
    iot_app_menu_item_root->cb_click_keys_2 = NULL;
    iot_app_menu_item_root->cb_hold_keys    = NULL;
    iot_app_menu_item_root->cb_release_keys = NULL;
    iot_app_menu_item_root->parent = NULL;
    iot_app_menu_item_root->children = NULL;
    iot_app_menu_item_root->children_count = 0;

        // 1.1 长按2s进入菜单模式
        iot_app_menu_item_setting_normal = osal_mem_alloc(sizeof(iot_app_menu_item_t));
        memset((uint8*)iot_app_menu_item_setting_normal, 0, sizeof(iot_app_menu_item_t));
        iot_app_menu_item_setting_normal->text = IOT_APP_MENU_ITEM_TEXT_STRING_NORMAL;
        iot_app_menu_item_setting_normal->enabled = true;
        iot_app_menu_item_setting_normal->level = 1;
        iot_app_menu_item_setting_normal->parent = iot_app_menu_item_root;
        iot_app_menu_item_setting_normal->cb_enter = iot_app_menu_item_setting_normal_cb_enter;
        iot_app_menu_item_setting_normal->cb_exit = iot_app_menu_item_setting_normal_cb_exit;
        iot_app_menu_item_setting_normal->cb_click_keys_1 = iot_app_menu_item_setting_normal_cb_click_key;

            // 1.1.1 out1输出模式设置
            iot_app_menu_item_setting_out1mode = osal_mem_alloc(sizeof(iot_app_menu_item_t));
            memset((uint8*)iot_app_menu_item_setting_out1mode, 0, sizeof(iot_app_menu_item_t));
            iot_app_menu_item_setting_out1mode->text = IOT_APP_MENU_ITEM_TEXT_STRING_SETTING_OUT1MODE;
            iot_app_menu_item_setting_out1mode->enabled = true;
            iot_app_menu_item_setting_out1mode->level = 2;
            iot_app_menu_item_setting_out1mode->parent = iot_app_menu_item_setting_normal;
            iot_app_menu_item_setting_out1mode->cb_enter = iot_app_menu_item_setting_out1mode_cb_enter;
            iot_app_menu_item_setting_out1mode->cb_exit = iot_app_menu_item_setting_out1mode_cb_exit;
            iot_app_menu_item_setting_out1mode->cb_click_keys_1 = iot_app_menu_item_level_2_cb_click_key;
            iot_app_menu_item_setting_out1mode->cb_click_keys_2 = iot_app_menu_item_setting_out1mode_cb_click_key;

            // 1.1.2 out2输出模式设置
            iot_app_menu_item_setting_out2mode = osal_mem_alloc(sizeof(iot_app_menu_item_t));
            memset((uint8*)iot_app_menu_item_setting_out2mode, 0, sizeof(iot_app_menu_item_t));
            iot_app_menu_item_setting_out2mode->text = IOT_APP_MENU_ITEM_TEXT_STRING_SETTING_OUT2MODE;
            iot_app_menu_item_setting_out2mode->enabled = true;
            iot_app_menu_item_setting_out2mode->level = 2;
            iot_app_menu_item_setting_out2mode->parent = iot_app_menu_item_setting_normal;
            iot_app_menu_item_setting_out2mode->cb_enter = iot_app_menu_item_setting_out2mode_cb_enter;
            iot_app_menu_item_setting_out2mode->cb_exit = iot_app_menu_item_setting_out2mode_cb_exit;
            iot_app_menu_item_setting_out2mode->cb_click_keys_1 = iot_app_menu_item_level_2_cb_click_key;
            iot_app_menu_item_setting_out2mode->cb_click_keys_2 = iot_app_menu_item_setting_out2mode_cb_click_key;

            // 1.1.3 nonc输出模式设置
            iot_app_menu_item_setting_nonc_output = osal_mem_alloc(sizeof(iot_app_menu_item_t));
            memset((uint8*)iot_app_menu_item_setting_nonc_output, 0, sizeof(iot_app_menu_item_t));
            iot_app_menu_item_setting_nonc_output->text = IOT_APP_MENU_ITEM_TEXT_STRING_SETTING_NONC_OUTPUT;
            iot_app_menu_item_setting_nonc_output->enabled = true;
            iot_app_menu_item_setting_nonc_output->level = 2;
            iot_app_menu_item_setting_nonc_output->parent = iot_app_menu_item_setting_normal;
            iot_app_menu_item_setting_nonc_output->cb_enter = iot_app_menu_item_setting_nonc_output_cb_enter;
            iot_app_menu_item_setting_nonc_output->cb_exit = iot_app_menu_item_setting_nonc_output_cb_exit;
            iot_app_menu_item_setting_nonc_output->cb_click_keys_1 = iot_app_menu_item_level_2_cb_click_key;
            iot_app_menu_item_setting_nonc_output->cb_click_keys_2 = iot_app_menu_item_setting_nonc_output_cb_click_key;

            // 1.1.4 nonc输出模式设置
            iot_app_menu_item_setting_response_time = osal_mem_alloc(sizeof(iot_app_menu_item_t));
            memset((uint8*)iot_app_menu_item_setting_response_time, 0, sizeof(iot_app_menu_item_t));
            iot_app_menu_item_setting_response_time->text = IOT_APP_MENU_ITEM_TEXT_STRING_SETTING_RESPONSE_TIME;
            iot_app_menu_item_setting_response_time->enabled = true;
            iot_app_menu_item_setting_response_time->level = 2;
            iot_app_menu_item_setting_response_time->parent = iot_app_menu_item_setting_normal;
            iot_app_menu_item_setting_response_time->cb_enter = iot_app_menu_item_setting_response_time_cb_enter;
            iot_app_menu_item_setting_response_time->cb_exit = iot_app_menu_item_setting_response_time_cb_exit;
            iot_app_menu_item_setting_response_time->cb_click_keys_1 = iot_app_menu_item_level_2_cb_click_key;
            iot_app_menu_item_setting_response_time->cb_click_keys_2 = iot_app_menu_item_setting_response_time_cb_click_key;

            // 1.1.5 屏幕文字颜色设置
            iot_app_menu_item_setting_screen_text_color = osal_mem_alloc(sizeof(iot_app_menu_item_t));
            memset((uint8*)iot_app_menu_item_setting_screen_text_color, 0, sizeof(iot_app_menu_item_t));
            iot_app_menu_item_setting_screen_text_color->text = IOT_APP_MENU_ITEM_TEXT_STRING_SETTING_SCREEN_TEXT_COLOR;
            iot_app_menu_item_setting_screen_text_color->enabled = true;
            iot_app_menu_item_setting_screen_text_color->level = 2;
            iot_app_menu_item_setting_screen_text_color->parent = iot_app_menu_item_setting_normal;
            iot_app_menu_item_setting_screen_text_color->cb_enter = iot_app_menu_item_setting_screen_text_color_cb_enter;
            iot_app_menu_item_setting_screen_text_color->cb_exit = iot_app_menu_item_setting_screen_text_color_cb_exit;
            iot_app_menu_item_setting_screen_text_color->cb_click_keys_1 = iot_app_menu_item_level_2_cb_click_key;
            iot_app_menu_item_setting_screen_text_color->cb_click_keys_2 = iot_app_menu_item_setting_screen_text_color_cb_click_key;
            
            // 1.1.6 单位转换设置
            iot_app_menu_item_setting_pressure_unit = osal_mem_alloc(sizeof(iot_app_menu_item_t));
            memset((uint8*)iot_app_menu_item_setting_pressure_unit, 0, sizeof(iot_app_menu_item_t));
            iot_app_menu_item_setting_pressure_unit->text = IOT_APP_MENU_ITEM_TEXT_STRING_SETTING_PRESSURE_UNIT;
            iot_app_menu_item_setting_pressure_unit->enabled = true;
            iot_app_menu_item_setting_pressure_unit->level = 2;
            iot_app_menu_item_setting_pressure_unit->parent = iot_app_menu_item_setting_normal;
            iot_app_menu_item_setting_pressure_unit->cb_enter = iot_app_menu_item_setting_pressure_unit_cb_enter;
            iot_app_menu_item_setting_pressure_unit->cb_exit = iot_app_menu_item_setting_pressure_unit_cb_exit;
            iot_app_menu_item_setting_pressure_unit->cb_click_keys_1 = iot_app_menu_item_level_2_cb_click_key;
            iot_app_menu_item_setting_pressure_unit->cb_click_keys_2 = iot_app_menu_item_setting_pressure_unit_cb_click_key;

        // 分配菜单模式子菜单数组
        iot_app_menu_item_setting_normal->children = osal_mem_alloc(sizeof(iot_app_menu_item_t*) * 6);
        iot_app_menu_item_setting_normal->children[0] = iot_app_menu_item_setting_out1mode;
        iot_app_menu_item_setting_normal->children[1] = iot_app_menu_item_setting_out2mode;
        iot_app_menu_item_setting_normal->children[2] = iot_app_menu_item_setting_nonc_output;
        iot_app_menu_item_setting_normal->children[3] = iot_app_menu_item_setting_response_time;
        iot_app_menu_item_setting_normal->children[4] = iot_app_menu_item_setting_screen_text_color;
        iot_app_menu_item_setting_normal->children[5] = iot_app_menu_item_setting_pressure_unit;
        iot_app_menu_item_setting_normal->children_count = 6;

        // 1.2 长按4s进入PRO模式
        iot_app_menu_item_setting_pro = osal_mem_alloc(sizeof(iot_app_menu_item_t));
        memset((uint8*)iot_app_menu_item_setting_pro, 0, sizeof(iot_app_menu_item_t));
        iot_app_menu_item_setting_pro->text = IOT_APP_MENU_ITEM_TEXT_STRING_PRO;
        iot_app_menu_item_setting_pro->enabled = true;
        iot_app_menu_item_setting_pro->level = 1;
        iot_app_menu_item_setting_pro->parent = iot_app_menu_item_root;
        iot_app_menu_item_setting_pro->cb_enter = iot_app_menu_item_setting_pro_cb_enter;
        iot_app_menu_item_setting_pro->cb_exit = iot_app_menu_item_setting_pro_cb_exit;
        iot_app_menu_item_setting_pro->cb_click_keys_1 = iot_app_menu_item_setting_pro_cb_click_key;

            // 1.2.1 副屏内容显示设置
            iot_app_menu_item_setting_subsidiary_display = osal_mem_alloc(sizeof(iot_app_menu_item_t));
            memset((uint8*)iot_app_menu_item_setting_subsidiary_display, 0, sizeof(iot_app_menu_item_t));
            iot_app_menu_item_setting_subsidiary_display->text = IOT_APP_MENU_ITEM_TEXT_STRING_SETTING_SUBSIDIARY_DISPLAY;
            iot_app_menu_item_setting_subsidiary_display->enabled = true;
            iot_app_menu_item_setting_subsidiary_display->level = 2;
            iot_app_menu_item_setting_subsidiary_display->parent = iot_app_menu_item_setting_pro;
            iot_app_menu_item_setting_subsidiary_display->cb_enter = iot_app_menu_item_setting_subsidiary_display_cb_enter;
            iot_app_menu_item_setting_subsidiary_display->cb_exit = iot_app_menu_item_setting_subsidiary_display_cb_exit;
            iot_app_menu_item_setting_subsidiary_display->cb_click_keys_1 = iot_app_menu_item_level_2_cb_click_key;
            iot_app_menu_item_setting_subsidiary_display->cb_click_keys_2 = iot_app_menu_item_setting_subsidiary_display_cb_click_key;

            // 1.2.2 显示速度设置
            iot_app_menu_item_setting_display_speed = osal_mem_alloc(sizeof(iot_app_menu_item_t));
            memset((uint8*)iot_app_menu_item_setting_display_speed, 0, sizeof(iot_app_menu_item_t));
            iot_app_menu_item_setting_display_speed->text = IOT_APP_MENU_ITEM_TEXT_STRING_SETTING_DISPLAY_SPEED;
            iot_app_menu_item_setting_display_speed->enabled = true;
            iot_app_menu_item_setting_display_speed->level = 2;
            iot_app_menu_item_setting_display_speed->parent = iot_app_menu_item_setting_pro;
            iot_app_menu_item_setting_display_speed->cb_enter = iot_app_menu_item_setting_display_speed_cb_enter;
            iot_app_menu_item_setting_display_speed->cb_exit = iot_app_menu_item_setting_display_speed_cb_exit;
            iot_app_menu_item_setting_display_speed->cb_click_keys_1 = iot_app_menu_item_level_2_cb_click_key;
            iot_app_menu_item_setting_display_speed->cb_click_keys_2 = iot_app_menu_item_setting_display_speed_cb_click_key;

            // 1.2.3 应差级别设置
            iot_app_menu_item_setting_hysteresis_level = osal_mem_alloc(sizeof(iot_app_menu_item_t));
            memset((uint8*)iot_app_menu_item_setting_hysteresis_level, 0, sizeof(iot_app_menu_item_t));
            iot_app_menu_item_setting_hysteresis_level->text = IOT_APP_MENU_ITEM_TEXT_STRING_SETTING_HYSTERESIS_LEVEL;
            iot_app_menu_item_setting_hysteresis_level->enabled = true;
            iot_app_menu_item_setting_hysteresis_level->level = 2;
            iot_app_menu_item_setting_hysteresis_level->parent = iot_app_menu_item_setting_pro;
            iot_app_menu_item_setting_hysteresis_level->cb_enter = iot_app_menu_item_setting_hysteresis_level_cb_enter;
            iot_app_menu_item_setting_hysteresis_level->cb_exit = iot_app_menu_item_setting_hysteresis_level_cb_exit;
            iot_app_menu_item_setting_hysteresis_level->cb_click_keys_1 = iot_app_menu_item_level_2_cb_click_key;
            iot_app_menu_item_setting_hysteresis_level->cb_click_keys_2 = iot_app_menu_item_setting_hysteresis_level_cb_click_key;

            // 1.2.4 颜色联动转换设置
            iot_app_menu_item_setting_display_color_linkage = osal_mem_alloc(sizeof(iot_app_menu_item_t));
            memset((uint8*)iot_app_menu_item_setting_display_color_linkage, 0, sizeof(iot_app_menu_item_t));
            iot_app_menu_item_setting_display_color_linkage->text = IOT_APP_MENU_ITEM_TEXT_STRING_SETTING_DISPLAY_COLOR_LINKAGE;
            iot_app_menu_item_setting_display_color_linkage->enabled = true;
            iot_app_menu_item_setting_display_color_linkage->level = 2;
            iot_app_menu_item_setting_display_color_linkage->parent = iot_app_menu_item_setting_pro;
            iot_app_menu_item_setting_display_color_linkage->cb_enter = iot_app_menu_item_setting_display_color_linkage_cb_enter;
            iot_app_menu_item_setting_display_color_linkage->cb_exit = iot_app_menu_item_setting_display_color_linkage_cb_exit;
            iot_app_menu_item_setting_display_color_linkage->cb_click_keys_1 = iot_app_menu_item_level_2_cb_click_key;
            iot_app_menu_item_setting_display_color_linkage->cb_click_keys_2 = iot_app_menu_item_setting_display_color_linkage_cb_click_key;

            // 1.2.5 ECO设置
            iot_app_menu_item_setting_eco = osal_mem_alloc(sizeof(iot_app_menu_item_t));
            memset((uint8*)iot_app_menu_item_setting_eco, 0, sizeof(iot_app_menu_item_t));
            iot_app_menu_item_setting_eco->text = IOT_APP_MENU_ITEM_TEXT_STRING_SETTING_ECO;
            iot_app_menu_item_setting_eco->enabled = true;
            iot_app_menu_item_setting_eco->level = 2;
            iot_app_menu_item_setting_eco->parent = iot_app_menu_item_setting_pro;
            iot_app_menu_item_setting_eco->cb_enter = iot_app_menu_item_setting_eco_cb_enter;
            iot_app_menu_item_setting_eco->cb_exit = iot_app_menu_item_setting_eco_cb_exit;
            iot_app_menu_item_setting_eco->cb_click_keys_1 = iot_app_menu_item_level_2_cb_click_key;
            iot_app_menu_item_setting_eco->cb_click_keys_2 = iot_app_menu_item_setting_eco_cb_click_key;

            // 1.2.6 复制功能设置
            iot_app_menu_item_setting_copy = osal_mem_alloc(sizeof(iot_app_menu_item_t));
            memset((uint8*)iot_app_menu_item_setting_copy, 0, sizeof(iot_app_menu_item_t));
            iot_app_menu_item_setting_copy->text = IOT_APP_MENU_ITEM_TEXT_STRING_SETTING_COPY;
            iot_app_menu_item_setting_copy->enabled = true;
            iot_app_menu_item_setting_copy->level = 2;
            iot_app_menu_item_setting_copy->parent = iot_app_menu_item_setting_pro;
            iot_app_menu_item_setting_copy->cb_enter = iot_app_menu_item_setting_copy_cb_enter;
            iot_app_menu_item_setting_copy->cb_exit = iot_app_menu_item_setting_copy_cb_exit;
            iot_app_menu_item_setting_copy->cb_click_keys_1 = iot_app_menu_item_level_2_cb_click_key;
            iot_app_menu_item_setting_copy->cb_click_keys_2 = iot_app_menu_item_setting_copy_cb_click_key;

            // 1.2.7 恢复出厂设置
            iot_app_menu_item_setting_reset = osal_mem_alloc(sizeof(iot_app_menu_item_t));
            memset((uint8*)iot_app_menu_item_setting_reset, 0, sizeof(iot_app_menu_item_t));
            iot_app_menu_item_setting_reset->text = IOT_APP_MENU_ITEM_TEXT_STRING_SETTING_RESET;
            iot_app_menu_item_setting_reset->enabled = true;
            iot_app_menu_item_setting_reset->level = 2;
            iot_app_menu_item_setting_reset->parent = iot_app_menu_item_setting_pro;
            iot_app_menu_item_setting_reset->cb_enter = iot_app_menu_item_setting_reset_cb_enter;
            iot_app_menu_item_setting_reset->cb_exit = iot_app_menu_item_setting_reset_cb_exit;
            iot_app_menu_item_setting_reset->cb_click_keys_1 = iot_app_menu_item_level_2_cb_click_key;
            iot_app_menu_item_setting_reset->cb_click_keys_2 = iot_app_menu_item_setting_reset_cb_click_key;

        // 分配PRO模式子菜单数组
        iot_app_menu_item_setting_pro->children = osal_mem_alloc(sizeof(iot_app_menu_item_t*) * 7);
        iot_app_menu_item_setting_pro->children[0] = iot_app_menu_item_setting_subsidiary_display;
        iot_app_menu_item_setting_pro->children[1] = iot_app_menu_item_setting_display_speed;
        iot_app_menu_item_setting_pro->children[2] = iot_app_menu_item_setting_hysteresis_level;
        iot_app_menu_item_setting_pro->children[3] = iot_app_menu_item_setting_display_color_linkage;
        iot_app_menu_item_setting_pro->children[4] = iot_app_menu_item_setting_eco;
        iot_app_menu_item_setting_pro->children[5] = iot_app_menu_item_setting_copy;
        iot_app_menu_item_setting_pro->children[6] = iot_app_menu_item_setting_reset;
        iot_app_menu_item_setting_pro->children_count = 7;

     // 分配根项子菜单数组
    iot_app_menu_item_root->children = osal_mem_alloc(sizeof(iot_app_menu_item_t*) * 2);
    iot_app_menu_item_root->children[0] = iot_app_menu_item_setting_normal;
    iot_app_menu_item_root->children[1] = iot_app_menu_item_setting_pro;
    iot_app_menu_item_root->children_count = 2;
}

// 菜单项初始化
void iot_app_menu_item_init(void) 
{
    iot_app_menu_create_all_item();// 动态创建菜单，所有菜单都在这个函数里编辑好
    
    iot_app_menu.root    = iot_app_menu_item_root;
    iot_app_menu.current = iot_app_menu_item_root;
    
    iot_app_menu.running = false;
}


#endif


