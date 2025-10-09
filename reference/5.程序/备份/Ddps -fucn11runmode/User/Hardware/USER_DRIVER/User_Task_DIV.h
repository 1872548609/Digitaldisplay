#ifndef _USER_TASK_DIV_H
#define _USER_TASK_DIV_H

#include "superhead.h"

typedef struct 
{
	union 
	{
		uint8_t LogictrlByte;
		struct 
		{
			uint8_t step 	:6;
			uint8_t execute 	:1; 
			uint8_t quit     :1;
		}LogictrlBit;
	}Logictrl;
    uint32_t tm;
} LogicParaDef;


typedef struct 
{
    LogicParaDef Task_Init;				
} Task;

extern Task LogicTask;

#define my 		 (*p)
#define Tim 	 (my.tm)
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

void Logic(void);


#endif



