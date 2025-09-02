#ifndef FLOW_H_
#define FLOW_H_

#include "main.h"
extern FlowCtrlTypedef FlowCtrl;
extern u16 volatile CurFlow	;
#define FlowPara FlowCtrl.CtrlState.FlowCtrlBite

void Flow_Init(void);
void Flow_Ctrl(void);

#endif // !FLOW_H_