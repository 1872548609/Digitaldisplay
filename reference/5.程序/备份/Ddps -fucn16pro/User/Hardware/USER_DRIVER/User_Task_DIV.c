#include "User_Task_Div.h"

Task LogicTask ={0};


void Task_Init(LogicParaDef*p )//初始化的任务
{
    TASK_QUIT
    {
        CtrlPara.execute = 0;
        CtrlPara.quit    = 0;
        CtrlPara.step    = 0;
    }
    TASK_PROCESS
    {
        case 1:
       break;
        case 2:
            
        break;
        case 3:
           
            CtrlPara.execute = 0;
            CtrlPara.quit    = 0;
            CtrlPara.step    = 0;
        break;
    }
}

void Logic(void)
{

	}


