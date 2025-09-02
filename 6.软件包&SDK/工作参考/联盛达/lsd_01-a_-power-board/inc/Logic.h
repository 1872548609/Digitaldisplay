#ifndef LOGIC_H_
#define LOGIC_H_


#include "main.h"
typedef struct 
{
	union 
	{
		u8 LogictrlByte;
		struct 
		{
			u8 step 	:6;
			u8 execute 	:1; 
			u8 quit     :1;
		}LogictrlBit;
	}Logictrl;
    u32 tm;
} LogicParaDef;


typedef struct 
{
    LogicParaDef InitTask;//
} Task;

extern Task LogicTask;

#define my 		 (*p)
#define Tim 	 (&my.tm)
#define CtrlPara my.Logictrl.LogictrlBit   
#define LT 		 LogicTask

#define	TASK_PROCESS \
	if(CtrlPara.execute== 1 && CtrlPara.step== 0)\
	{\
		CtrlPara.step = 1;\
	}\
	switch(CtrlPara.step)

#define TASK_QUIT  \
	if(CtrlPara.execute == 0&& CtrlPara.quit == 1)\
	{\
		CtrlPara.quit = 0;\
	}\
	else if(CtrlPara.execute == 1 && CtrlPara.quit == 1)

//void Logic();


#endif // !LOGIC_H_
