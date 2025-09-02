#ifndef __APP_H__
#define __APP_H__


#define LED_PORT  PORT0
#define LED_BIT    7

#define LED_ON  1
#define LED_OFF 0
extern void App_TaskInit( byte task_id );

extern unsigned char App_ProcessEvent( byte task_id, UINT16 events );

#endif