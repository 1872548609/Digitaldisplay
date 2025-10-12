#include "iot_menu.h"

//======================START==========================
/*
如果需要添加东西自行建立一个模块，调用本身存在的接口，不要修改本身内容
*/

MenuSystem menuSystem = {0};

// 初始化菜单系统
void MenuSystem_Init(MenuItem* root) {
    menuSystem.root = root;
    menuSystem.current = root;
    menuSystem.running = false;
}

// 菜单是否启动
bool MenuSystem_IsActive(void){
	return menuSystem.running;
}

// 启动菜单系统
void MenuSystem_Start() {
    if (!menuSystem.running) {
		menuSystem.current = menuSystem.root;
        menuSystem.running = true;
    }
}
 
// 停止菜单系统
void MenuSystem_Stop() {
    if (menuSystem.running) {
		menuSystem.current = menuSystem.root;
        menuSystem.running = false;
    }
}

// 返回当前菜单项的等级
int Menu_GetCurrentLevel() {
    if (menuSystem.current != NULL) {
        return menuSystem.current->level;
    }
    return -1;  // 如果当前菜单项为空，返回-1表示错误
}
 

// 进入当前菜单的子菜单（下一级）
void Menu_Enter() {
    if (menuSystem.current->childCount > 0) {
		
		// 菜单退出回调
		Menu_Execute(2);
		
        // 如果有记录的子菜单指针，则进入该记录的子菜单
        if (menuSystem.current->childrenmenu != NULL) {
            menuSystem.current = menuSystem.current->childrenmenu;
        } 
        // 否则进入第一个子菜单
        else {
            menuSystem.current = menuSystem.current->children[0];
        }
		
		// 菜单进入回调
		Menu_Execute(1);
    }
}

// 返回父菜单（上一级）
// record 是否记录当前子菜单（true=记录，false=不记录）
void Menu_Back(bool record) {
    if (menuSystem.current->parent != NULL) {
		
		// 菜单退出回调
		Menu_Execute(2);
		
        // 如果需要记录，则更新父菜单的 childrenmenu
        if (record) {
            menuSystem.current->parent->childrenmenu = menuSystem.current;
        }
		
        // 返回父菜单
        menuSystem.current = menuSystem.current->parent;
		
		// 菜单进入回调
		Menu_Execute(1);
    }
}

// 切换到下一个同级菜单
void Menu_Next() {
    MenuItem* parent = menuSystem.current->parent;
    if (parent == NULL) return; // 根菜单没有同级菜单
	
	// 菜单退出回调
	Menu_Execute(2);
 
    for (int i = 0; i < parent->childCount; i++) {
        if (parent->children[i] == menuSystem.current) {
            int nextIdx = (i + 1) % parent->childCount; // 循环切换
            menuSystem.current = parent->children[nextIdx];
			
			// 菜单进入回调
			Menu_Execute(1);
			
            break;
        }
    }
}

// 切换到上一个同级菜单
void Menu_Prev() {
    MenuItem* parent = menuSystem.current->parent;
    if (parent == NULL) return; // 根菜单没有同级菜单
 
	// 菜单退出回调
	Menu_Execute(2);
	
    for (int i = 0; i < parent->childCount; i++) {
        if (parent->children[i] == menuSystem.current) {
            int prevIdx = (i - 1 + parent->childCount) % parent->childCount; // 循环切换
            menuSystem.current = parent->children[prevIdx];
			
			// 菜单进入回调
			Menu_Execute(1);
			
            break;
        }
    }
}

// 获取当前菜单项
MenuItem* Menu_GetCurrent() {
    return menuSystem.current;
}

// 执行当前菜单项的回调函数
void Menu_Execute(char wichcallback) {
    if (menuSystem.current->func != NULL && menuSystem.current->enabled) {
        menuSystem.current->whichcallback = wichcallback;		
		menuSystem.current->func(menuSystem.current);
    }
}

/*
* 递归查找菜单项
* current 当前菜单项
* targetText 要查找的菜单文本
* 找到的菜单项指针，未找到则返回 NULL
*/
static MenuItem* Menu_FindItemRecursive(MenuItem* current, const char* targetText) {
    if (current == NULL || targetText == NULL) return NULL;

    // 检查当前菜单是否匹配
    if (strcmp(current->text, targetText) == 0) {
        return current;
    }

    // 递归查找子菜单
    if (current->children != NULL) {
        for (int i = 0; i < current->childCount; i++) {
            MenuItem* found = Menu_FindItemRecursive(current->children[i], targetText);
            if (found != NULL) {
                return found;
            }
        }
    }

    return NULL;
}

char *my_strdup(const char *str) {
    if (str == NULL) return NULL;
    size_t len = strlen(str) + 1;
    char *copy = malloc(len);
    if (copy) memcpy(copy, str, len);
    return copy;
}

/**
* 根据菜单文本导航到指定菜单
* targetText 目标菜单文本（支持绝对路径或相对路径）
* 是否成功切换（true=成功，false=未找到）
*/
bool Menu_NavigateTo(const char* targetText) {
    // 检查输入参数是否有效（目标文本和菜单根节点不能为空）
    if (targetText == NULL || menuSystem.root == NULL) return false;
 
    // 判断路径是否包含 '/'，若包含则为绝对路径（如 "Root Menu/FinE1234/hsp1234"）
    if (strchr(targetText, '/') != NULL) {
        // 复制路径字符串，避免修改原字符串（需手动实现 my_strdup，因 strdup 非标准）
        char* path = my_strdup(targetText);
        if (path == NULL) return false; // 分配失败
 
        // 使用 strtok 分割路径（首次调用，以 '/' 为分隔符）
        char* token = strtok(path, "/"); // 如 "A/B/C" → 依次处理 "A"、"B"、"C"
        // 从根菜单开始导航
        MenuItem* current = menuSystem.root;
 
		 // 检查路径的第一级是否是根菜单本身
        if (token != NULL && strcmp(current->text, token) == 0) {
            // 路径的第一级是根菜单，继续处理下一级
            token = strtok(NULL, "/");// 输入：NULL（表示继续分割之前的字符串）
        } else {
            // 路径的第一级不是根菜单，说明可能是相对路径或错误路径
            // 但当前分支是绝对路径处理，所以直接返回失败
            free(path);
            return false;
        }
		
        // 循环处理路径中的每一级菜单
        while (token != NULL && current != NULL) {
            // 在当前菜单的子菜单中查找与 token 匹配的项
            MenuItem* found = NULL;
            for (int i = 0; i < current->childCount; i++) {
                // 比较子菜单文本是否与当前 token 一致
                if (strcmp(current->children[i]->text, token) == 0) {
                    found = current->children[i]; // 找到匹配项
                    break;
                }
            }
 
            // 若未找到匹配的子菜单，释放路径内存并返回失败
            if (found == NULL) {
                free(path);
                return false;
            }
 
            // 移动到下一级菜单，并继续分割路径
            current = found;
            token = strtok(NULL, "/");
        }
 
        // 释放临时路径字符串
        free(path);
 
        // 若最终找到有效菜单项，更新当前菜单并返回成功
        if (current != NULL) {
            menuSystem.current = current;
            return true;
        }
    } 
    else {
        // 相对路径处理（如 "hsp1234"），从根菜单递归查找目标项
        MenuItem* found = Menu_FindItemRecursive(menuSystem.root, targetText);
        if (found != NULL) {
            menuSystem.current = found; // 更新当前菜单
            return true;
        }
    }
 
    // 所有路径均未找到目标菜单，返回失败
    return false;
}


//如果需要添加东西自行建立一个模块，调用本身存在的接口，不要修改本身内容
//=========================END============================

// 创建并初始化一个菜单项
//MenuItem* CreateMenuItem(const char* text, void (*func)(MenuItem*), bool enabled , int level) {
//    MenuItem* item = osal_mem_alloc(sizeof(MenuItem));
//    if (item == NULL) return NULL;
//    
//    strncpy(item->text, text, sizeof(item->text) - 1);
//    item->text[sizeof(item->text) - 1] = '\0';
//    item->func = func;
//    item->enabled = enabled;
//    item->parent = NULL;
//    item->children = NULL;
//    item->childCount = 0;
//	item->level = level;  // 初始等级设为0 
//	
//    return item;
//}
 
// 为父菜单项添加子菜单项
//void MenuItem_AddChild(MenuItem* parent, MenuItem* child) {
//    if (parent == NULL || child == NULL) return;
//    
//    // 设置子项的父指针
//    child->parent = parent;
//	
//	// 设置子项的等级
//    child->level = parent->level + 1;
//    
//    // 如果父项还没有子项数组，先创建
//    if (parent->children == NULL) {
//        parent->children = osal_mem_alloc(sizeof(MenuItem*));
//        if (parent->children == NULL) return;
//        
//        parent->children[0] = child;
//        parent->childCount = 1;
//    } 
//    else {
//        // 否则重新分配内存以容纳新的子项
//        MenuItem** newChildren = osal_mem_alloc(sizeof(MenuItem*) * (parent->childCount + 1));
//        if (newChildren == NULL) return;
//        
//        // 复制原有子项指针
//        for (int i = 0; i < parent->childCount; i++) {
//            newChildren[i] = parent->children[i];
//        }
//        
//        // 添加新子项
//        newChildren[parent->childCount] = child;
//        parent->childCount++;
//        
//        // 释放旧数组（如果有）
//        if (parent->children != NULL) {
//            osal_mem_free(parent->children);
//        }
//        
//        parent->children = newChildren;
//    }
//}

// 在父菜单项的指定位置插入子菜单项
// position: 插入位置索引（从0开始），如果大于当前子项数量，则追加到末尾
//void MenuItem_InsertChild(MenuItem* parent, MenuItem* child, int position) {
//    if (parent == NULL || child == NULL) return;
//    
//    // 设置子项的父指针
//    child->parent = parent;
//    
//    // 如果位置超出范围，调整为末尾
//    if (position < 0) position = 0;
//    
//    // 情况1：父项还没有子项
//    if (parent->children == NULL || parent->childCount == 0) {
//        parent->children = osal_mem_alloc(sizeof(MenuItem*));
//        if (parent->children == NULL) return;
//        
//        parent->children[0] = child;
//        parent->childCount = 1;
//        return;
//    }
//    
//    // 确保位置不超过子项数量
//    if (position > parent->childCount) {
//        position = parent->childCount;
//    }
//    
//    // 分配新数组（多一个位置）
//    MenuItem** newChildren = osal_mem_alloc(sizeof(MenuItem*) * (parent->childCount + 1));
//    if (newChildren == NULL) return;
//    
//    // 复制插入位置前的子项
//    for (int i = 0; i < position; i++) {
//        newChildren[i] = parent->children[i];
//    }
//    
//    // 插入新子项
//    newChildren[position] = child;
//    
//    // 复制插入位置后的子项
//    for (int i = position; i < parent->childCount; i++) {
//        newChildren[i + 1] = parent->children[i];
//    }
//    
//    // 更新父项信息
//    parent->childCount++;
//    
//    // 释放旧数组
//    if (parent->children != NULL) {
//        osal_mem_free(parent->children);
//    }
//    
//    parent->children = newChildren;
//}
// 

// 在父菜单项中查找指定文本的子菜单项
//MenuItem* MenuItem_FindChild(MenuItem* parent, const char* text) {
//    if (parent == NULL || text == NULL || parent->children == NULL) return NULL;
//    
//    for (int i = 0; i < parent->childCount; i++) {
//        if (strcmp(parent->children[i]->text, text) == 0) {
//            return parent->children[i];
//        }
//    }
//    
//    return NULL;
//}

// 回调函数
//void onItem1Click(MenuItem* item) {


//}


MenuItem* root;

MenuItem* basic_setting;

MenuItem* power_setting;

MenuItem* FinE_mode ;

MenuItem* highspeed_mode;

MenuItem* mega_mode;

MenuItem* super_mode;

MenuItem* att_setting;

MenuItem* sta_setting;

MenuItem* end_setting;

MenuItem* end_end;

// 统一创建菜单树，返回树根 ，注册回调
MenuItem* CreateTestMenu() {
    // 创建根项
    root = osal_mem_alloc(sizeof(MenuItem));
    strcpy(root->text, "Root Menu");
    root->func = NULL;
    root->enabled = true;
    root->parent = NULL;
    root->children = NULL;
    root->childCount = 0;
	root->level = 0;

    // 1.基础设置
    basic_setting = osal_mem_alloc(sizeof(MenuItem));
    strcpy(basic_setting->text, "basicset");
    basic_setting->func = onItem1Click;
    basic_setting->enabled = true;
    basic_setting->parent = root;  // 父菜单是根菜单
    basic_setting->children = NULL;
    basic_setting->childCount = 0;
	basic_setting->level = 1;
	
		// 1.1功率设置
		power_setting = osal_mem_alloc(sizeof(MenuItem));
		strcpy(power_setting->text, "basicset");
		power_setting->func = onItem1Click;
		power_setting->enabled = true;
		power_setting->parent = basic_setting;  // 父菜单是basic_setting
		power_setting->children = NULL;
		power_setting->childCount = 0;
		power_setting->level = 2;

			// 1.1.1fine模式
			FinE_mode = osal_mem_alloc(sizeof(MenuItem));
			strcpy(FinE_mode->text, "FinE1234");
			FinE_mode->func = onItem1Click;
			FinE_mode->enabled = true;
			FinE_mode->parent = power_setting;  
			FinE_mode->children = NULL;
			FinE_mode->childCount = 0;
			FinE_mode->level = 3;
			
			// 1.1.2highspeed模式
			highspeed_mode = osal_mem_alloc(sizeof(MenuItem));
			strcpy(highspeed_mode->text, " hsp1234");
			highspeed_mode->func = onItem1Click;
			highspeed_mode->enabled = true;
			highspeed_mode->parent = power_setting; 
			highspeed_mode->children = NULL;
			highspeed_mode->childCount = 0;
			highspeed_mode->level = 3;

			mega_mode = osal_mem_alloc(sizeof(MenuItem));
			strcpy(mega_mode->text, "mega1234");
			mega_mode->func = onItem1Click;
			mega_mode->enabled = true;
			mega_mode->parent = power_setting;  
			mega_mode->children = NULL;
			mega_mode->childCount = 0;
			mega_mode->level = 3;
			
			super_mode = osal_mem_alloc(sizeof(MenuItem));
			strcpy(super_mode->text, "supr1234");
			super_mode->func = onItem1Click;
			super_mode->enabled = true;
			super_mode->parent = power_setting;  
			super_mode->children = NULL;
			super_mode->childCount = 0;
			super_mode->level = 3;

			// 分配功率设置的子菜单数组
			power_setting->children = osal_mem_alloc(sizeof(MenuItem*) * 4);
			power_setting->children[0] = FinE_mode;
			power_setting->children[1] = highspeed_mode;
			power_setting->children[2] = mega_mode;
			power_setting->children[3] = super_mode;
			power_setting->childCount = 4;
			
		// 1.2 ATT设置
		att_setting = osal_mem_alloc(sizeof(MenuItem));
		strcpy(att_setting->text, " Att set");
		att_setting->func = onItem1Click;
		att_setting->enabled = true;
		att_setting->parent = basic_setting;  // 父菜单是basic_setting
		att_setting->children = NULL;
		att_setting->childCount = 0;
		att_setting->level = 2;
		
		// 1.2.1 STA设置
		sta_setting = osal_mem_alloc(sizeof(MenuItem));
		strcpy(sta_setting->text, " STA set");
		sta_setting->func = onItem1Click;
		sta_setting->enabled = true;
		sta_setting->parent = basic_setting;  // 父菜单是basic_setting
		sta_setting->children = NULL;
		sta_setting->childCount = 0;
		sta_setting->level = 2;
		
		//1.3 有关灵敏度设置的菜单
//		MenuItem* about_setting = osal_mem_alloc(sizeof(MenuItem));
//		strcpy(about_setting->text, "aboutset");
//		about_setting->func = onItem1Click;  
//		about_setting->enabled = true;
//		about_setting->parent = basic_setting;
//		about_setting->children = NULL;
//		about_setting->childCount = 0;
//		about_setting->level = 2;
			
//			// 1.3.1 标志灵敏度设置
//			MenuItem* sensitivity_mode = osal_mem_alloc(sizeof(MenuItem));
//			strcpy(sensitivity_mode->text, " SET std");
//			sensitivity_mode->func = onItem1Click;
//			sensitivity_mode->enabled = true;
//			sensitivity_mode->parent = about_setting;  
//			sensitivity_mode->children = NULL;
//			sensitivity_mode->childCount = 0;
//			sensitivity_mode->level = 3;
//			
//			// 1.3.1 百分比校准
//			MenuItem* percentage_mode = osal_mem_alloc(sizeof(MenuItem));
//			strcpy(percentage_mode->text, " SETSETP");
//			percentage_mode->func = onItem1Click;
//			percentage_mode->enabled = true;
//			percentage_mode->parent = about_setting;  
//			percentage_mode->children = NULL;
//			percentage_mode->childCount = 0;
//			percentage_mode->level = 3;
//			
//			// 1.3.1 零点迁移
//			MenuItem* zeroshift_mode = osal_mem_alloc(sizeof(MenuItem));
//			strcpy(zeroshift_mode->text, " SETOSET");
//			zeroshift_mode->func = onItem1Click;
//			zeroshift_mode->enabled = true;
//			zeroshift_mode->parent = about_setting;  
//			zeroshift_mode->children = NULL;
//			zeroshift_mode->childCount = 0;
//			zeroshift_mode->level = 3;

//			// 分配功率设置的子菜单数组
//			about_setting->children = osal_mem_alloc(sizeof(MenuItem*) * 3);
//			about_setting->children[0] = sensitivity_mode;
//			about_setting->children[1] = percentage_mode;
//			about_setting->children[2] = zeroshift_mode;
//			about_setting->childCount = 3;
			
			
		//1.4 有关三大模式设置的菜单
		end_setting = osal_mem_alloc(sizeof(MenuItem));
		strcpy(end_setting->text, "     End");
		end_setting->func = onItem1Click;  
		end_setting->enabled = true;
		end_setting->parent = basic_setting;
		end_setting->children = NULL;
		end_setting->childCount = 0;
		end_setting->level = 2;
//			
//			// 1.4.1检测设置
//			MenuItem* check_setting = osal_mem_alloc(sizeof(MenuItem));
//			strcpy(check_setting->text, "stg- Fun");
//			check_setting->func = onItem1Click;
//			check_setting->enabled = true;
//			check_setting->parent = end_setting;
//			check_setting->children = NULL;
//			check_setting->childCount = 0;
//			check_setting->level = 3;

//			// 1.4.2显示设置
//			MenuItem* disp_setting = osal_mem_alloc(sizeof(MenuItem));
//			strcpy(disp_setting->text, "stg-disp");
//			disp_setting->func = onItem1Click;
//			disp_setting->enabled = true;
//			disp_setting->parent = end_setting;
//			disp_setting->children = NULL;
//			disp_setting->childCount = 0;
//			disp_setting->level = 3;

//			// 1.4.3系统设置
//			MenuItem* sys_setting = osal_mem_alloc(sizeof(MenuItem));
//			strcpy(sys_setting->text, "stg- SYS");
//			sys_setting->func = onItem1Click;
//			sys_setting->enabled = true;
//			sys_setting->parent = end_setting;
//			sys_setting->children = NULL;
//			sys_setting->childCount = 0;
//			sys_setting->level = 3;	
//			
//			// 分配三大模式设置的子菜单数组
//			end_setting->children = osal_mem_alloc(sizeof(MenuItem*) * 3);
//			end_setting->children[0] = check_setting;
//			end_setting->children[1] = disp_setting;
//			end_setting->children[2] = sys_setting;
//			end_setting->childCount = 3;
		//1.5 结束项
		end_end = osal_mem_alloc(sizeof(MenuItem));
		strcpy(end_end->text, "   End  ");
		end_end->func = onItem1Click;  
		end_end->enabled = true;
		end_end->parent = basic_setting;
		end_end->children = NULL;
		end_end->childCount = 0;
		end_end->level = 2;

	// 分配基础设置的子菜单数组
//	basic_setting->children = osal_mem_alloc(sizeof(MenuItem*) * 5);
//	basic_setting->children[0] = power_setting;
//	basic_setting->children[1] = att_setting;
//	basic_setting->children[2] = about_setting;
//	basic_setting->children[3] = end_setting;
//	basic_setting->children[4] = end_end;
//	basic_setting->childCount = 5;
	basic_setting->children = osal_mem_alloc(sizeof(MenuItem*) * 5);
	basic_setting->children[0] = power_setting;
	basic_setting->children[1] = att_setting;
	basic_setting->children[2] = sta_setting;
	basic_setting->children[3] = end_setting;
	basic_setting->children[4] = end_end;
	basic_setting->childCount = 5;


    // 分配根项子菜单数组
    root->children = osal_mem_alloc(sizeof(MenuItem*) * 1);
    root->children[0] = basic_setting;
    root->childCount = 1;
			
    return root;
}






