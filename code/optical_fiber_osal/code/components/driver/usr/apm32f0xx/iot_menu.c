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
		//Menu_Execute(2);
		
        // 如果有记录的子菜单指针，则进入该记录的子菜单
        if (menuSystem.current->childrenmenu != NULL) {
            menuSystem.current = menuSystem.current->childrenmenu;
        } 
        // 否则进入第一个子菜单
        else {
            menuSystem.current = menuSystem.current->children[0];
        }
		
		// 菜单进入回调
		//Menu_Execute(1);
    }
}

// 返回父菜单（上一级）
// record 是否记录当前子菜单（true=记录，false=不记录）
void Menu_Back(bool record) {
    if (menuSystem.current->parent != NULL) {
		
		// 菜单退出回调
		//Menu_Execute(2);
		
        // 如果需要记录，则更新父菜单的 childrenmenu
        if (record) {
            menuSystem.current->parent->childrenmenu = menuSystem.current;
        }
		
        // 返回父菜单
        menuSystem.current = menuSystem.current->parent;
		
		// 菜单进入回调
		//Menu_Execute(1);
    }
}

// 切换到下一个同级菜单
void Menu_Next() {
    MenuItem* parent = menuSystem.current->parent;
    if (parent == NULL) return; // 根菜单没有同级菜单
	
	// 菜单退出回调
	//Menu_Execute(2);
 
    for (int i = 0; i < parent->childCount; i++) {
        if (parent->children[i] == menuSystem.current) {
            int nextIdx = (i + 1) % parent->childCount; // 循环切换
            menuSystem.current = parent->children[nextIdx];
			
			// 菜单进入回调
			//Menu_Execute(1);
			
            break;
        }
    }
}

// 切换到上一个同级菜单
void Menu_Prev() {
    MenuItem* parent = menuSystem.current->parent;
    if (parent == NULL) return; // 根菜单没有同级菜单
 
	// 菜单退出回调
	//Menu_Execute(2);
	
    for (int i = 0; i < parent->childCount; i++) {
        if (parent->children[i] == menuSystem.current) {
            int prevIdx = (i - 1 + parent->childCount) % parent->childCount; // 循环切换
            menuSystem.current = parent->children[prevIdx];
			
			// 菜单进入回调
			//Menu_Execute(1);
			
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

// callback 0
__attribute__((weak)) void root_Click(MenuItem* item) {}
	
// callback 1
__attribute__((weak)) void normset_click(MenuItem* item) {}
	
__attribute__((weak)) void proset_click(MenuItem* item) {}
	
// callback 2	
__attribute__((weak)) void out1mode_click(MenuItem* item) {}	
__attribute__((weak)) void out2mode_click(MenuItem* item) {}		
__attribute__((weak)) void noncmode_click(MenuItem* item) {}		
__attribute__((weak)) void maincolormode_click(MenuItem* item) {}	
__attribute__((weak)) void unitconvermode_click(MenuItem* item) {}

__attribute__((weak)) void secscreenmode_click(MenuItem* item) {}
__attribute__((weak)) void dispeedmode_click(MenuItem* item) {}	
__attribute__((weak)) void diflevelmode_click(MenuItem* item) {}	
__attribute__((weak)) void coloractmode_click(MenuItem* item) {}
__attribute__((weak)) void eocmode_click(MenuItem* item) {}
__attribute__((weak)) void copymode_click(MenuItem* item) {}
__attribute__((weak)) void facrecovermode_click(MenuItem* item) {}	
	
	
// root 
MenuItem* root;

// menu 1
MenuItem* normset_menu;

MenuItem* proset_menu;
	
// menu 2
MenuItem* out1mode_menu;	
MenuItem* out2mode_menu;
MenuItem* NONCmode_menu;
MenuItem* MAINColormode_menu;
MenuItem* unitconvermode_menu;
	
MenuItem* secscreenmode_menu;
MenuItem* dispeedmode_menu;
MenuItem* diflevelmode_menu;
MenuItem* coloractmode_menu;
MenuItem* eocmode_menu;
MenuItem* copymode_menu;
MenuItem* facrecovermode_menu;
	
	
	
// 统一创建菜单树，返回树根 ，注册回调
MenuItem* CreateTestMenu() {
    // 创建根项
    root = osal_mem_alloc(sizeof(MenuItem));
    strcpy(root->text, "Root");
    root->func = root_Click;
    root->enabled = true;
    root->parent = NULL;
    root->children = NULL;
    root->childCount = 0;
	root->level = 0;
	{
		 // 1.NROMSET
		normset_menu = osal_mem_alloc(sizeof(MenuItem));
		strcpy(normset_menu->text, "NORMSET");
		normset_menu->func = normset_click;
		normset_menu->enabled = true;
		normset_menu->parent = root;  
		normset_menu->children = NULL;
		normset_menu->childCount = 0;
		normset_menu->level = 1;
		{
			// 通道1模式
			out1mode_menu = osal_mem_alloc(sizeof(MenuItem));
			strcpy(out1mode_menu->text, "OUT1MODE");
			out1mode_menu->func = out1mode_click;
			out1mode_menu->enabled = true;
			out1mode_menu->parent = normset_menu;  
			out1mode_menu->children = NULL;
			out1mode_menu->childCount = 0;
			out1mode_menu->level = 2;
			
			// 通道2模式
			out2mode_menu = osal_mem_alloc(sizeof(MenuItem));
			strcpy(out2mode_menu->text, "OUT2MODE");
			out2mode_menu->func = out2mode_click;
			out2mode_menu->enabled = true;
			out2mode_menu->parent = normset_menu;  
			out2mode_menu->children = NULL;
			out2mode_menu->childCount = 0;
			out2mode_menu->level = 2;
		
			// nonc模式
			NONCmode_menu = osal_mem_alloc(sizeof(MenuItem));
			strcpy(NONCmode_menu->text, "NONCMODE");
			NONCmode_menu->func = noncmode_click;
			NONCmode_menu->enabled = true;
			NONCmode_menu->parent = normset_menu;  
			NONCmode_menu->children = NULL;
			NONCmode_menu->childCount = 0;
			NONCmode_menu->level = 2;
			
			// maincolor模式
			MAINColormode_menu = osal_mem_alloc(sizeof(MenuItem));
			strcpy(MAINColormode_menu->text, "MAINCOLORMODE");
			MAINColormode_menu->func = maincolormode_click;
			MAINColormode_menu->enabled = true;
			MAINColormode_menu->parent = normset_menu;  
			MAINColormode_menu->children = NULL;
			MAINColormode_menu->childCount = 0;
			MAINColormode_menu->level = 2;
			
			//unitconver模式//
			unitconvermode_menu = osal_mem_alloc(sizeof(MenuItem));
			strcpy(unitconvermode_menu->text, "UNITCONVER");
			unitconvermode_menu->func = unitconvermode_click;
			unitconvermode_menu->enabled = true;
			unitconvermode_menu->parent = normset_menu;  
			unitconvermode_menu->children = NULL;
			unitconvermode_menu->childCount = 0;
			unitconvermode_menu->level = 2;
			
			
			// 分配根项子菜单数组
			normset_menu->children = osal_mem_alloc(sizeof(MenuItem*) * 5);		
			normset_menu->childCount = 5;
			normset_menu->children[0] = out1mode_menu;
			normset_menu->children[1] = out2mode_menu;
			normset_menu->children[2] = NONCmode_menu;
			normset_menu->children[3] = MAINColormode_menu;
			normset_menu->children[4] = unitconvermode_menu;
		}	

		// 2.PROSET
		proset_menu = osal_mem_alloc(sizeof(MenuItem));
		strcpy(proset_menu->text, "PROSET");
		proset_menu->func = proset_click;
		proset_menu->enabled = true;
		proset_menu->parent = root;  
		proset_menu->children = NULL;
		proset_menu->childCount = 0;
		proset_menu->level = 1;
		{
		
			secscreenmode_menu = osal_mem_alloc(sizeof(MenuItem));
			strcpy(secscreenmode_menu->text, "SECSCREEN");
			secscreenmode_menu->func = secscreenmode_click;
			secscreenmode_menu->enabled = true;
			secscreenmode_menu->parent = proset_menu;  
			secscreenmode_menu->children = NULL;
			secscreenmode_menu->childCount = 0;
			secscreenmode_menu->level = 1;
			
			dispeedmode_menu = osal_mem_alloc(sizeof(MenuItem));
			strcpy(dispeedmode_menu->text, "DISPEED");
			dispeedmode_menu->func = dispeedmode_click;
			dispeedmode_menu->enabled = true;
			dispeedmode_menu->parent = proset_menu;  
			dispeedmode_menu->children = NULL;
			dispeedmode_menu->childCount = 0;
			dispeedmode_menu->level = 1;
			
			diflevelmode_menu = osal_mem_alloc(sizeof(MenuItem));
			strcpy(diflevelmode_menu->text, "DIFLEVEL");
			diflevelmode_menu->func = diflevelmode_click;
			diflevelmode_menu->enabled = true;
			diflevelmode_menu->parent = proset_menu;  
			diflevelmode_menu->children = NULL;
			diflevelmode_menu->childCount = 0;
			diflevelmode_menu->level = 1;
			
			coloractmode_menu = osal_mem_alloc(sizeof(MenuItem));
			strcpy(coloractmode_menu->text, "COLORACT");
			coloractmode_menu->func = coloractmode_click;
			coloractmode_menu->enabled = true;
			coloractmode_menu->parent = proset_menu;  
			coloractmode_menu->children = NULL;
			coloractmode_menu->childCount = 0;
			coloractmode_menu->level = 1;
			
			eocmode_menu = osal_mem_alloc(sizeof(MenuItem));
			strcpy(eocmode_menu->text, "EOC");
			eocmode_menu->func = eocmode_click;
			eocmode_menu->enabled = true;
			eocmode_menu->parent = proset_menu;  
			eocmode_menu->children = NULL;
			eocmode_menu->childCount = 0;
			eocmode_menu->level = 1;
			
			copymode_menu = osal_mem_alloc(sizeof(MenuItem));
			strcpy(copymode_menu->text, "COPY");
			copymode_menu->func = copymode_click;
			copymode_menu->enabled = true;
			copymode_menu->parent = proset_menu;  
			copymode_menu->children = NULL;
			copymode_menu->childCount = 0;
			copymode_menu->level = 1;
			
			facrecovermode_menu = osal_mem_alloc(sizeof(MenuItem));
			strcpy(facrecovermode_menu->text, "FACRECOVER");
			facrecovermode_menu->func = facrecovermode_click;
			facrecovermode_menu->enabled = true;
			facrecovermode_menu->parent = proset_menu;  
			facrecovermode_menu->children = NULL;
			facrecovermode_menu->childCount = 0;
			facrecovermode_menu->level = 1;
			
			// 分配根项子菜单数组
			proset_menu->children = osal_mem_alloc(sizeof(MenuItem*) * 7);		
			proset_menu->childCount = 7;
			proset_menu->children[0] = secscreenmode_menu;
			proset_menu->children[1] = dispeedmode_menu;
			proset_menu->children[2] = diflevelmode_menu;
			proset_menu->children[3] = coloractmode_menu;
			proset_menu->children[4] = eocmode_menu;
			proset_menu->children[5] = copymode_menu;
			proset_menu->children[6] = facrecovermode_menu;
		}

		// 分配根项子菜单数组
		root->children = osal_mem_alloc(sizeof(MenuItem*) * 2);
		root->childCount = 2;
		root->children[0] = normset_menu;
		root->children[1] = proset_menu;
	}
	
   
    
    return root;
}






