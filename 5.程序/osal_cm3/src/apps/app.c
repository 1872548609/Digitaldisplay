#include "osaldef.h"
#include "OnBoard.h"
#include "OSAL.h"

#include "app.h"  

#include "key.h"

#include "gpio.h"


byte App_TaskID;  

void App_TaskInit( byte task_id )
{  

 GPIOSetDir( LED_PORT, LED_BIT, 1 );
 App_TaskID = task_id;
 RegisterForKeys( task_id );

 // init to off
 GPIOSetValue(LED_PORT,LED_BIT,1);
  GPIOSetValue(LED_PORT,LED_BIT,0);
   GPIOSetValue(LED_PORT,LED_BIT,1);
    GPIOSetValue(LED_PORT,LED_BIT,0);
}

unsigned char App_ProcessEvent( byte task_id, UINT16 events )
{
  osal_msg_received_t *msg;
  byte		*msgPtr;
  static int state;
  // Could be multiple events, so switch won't work
  if ( events & SYS_EVENT_MSG ) {
    while ( (msg = osal_msg_receive( App_TaskID )) ) {
       msgPtr = msg->msg_ptr;
    	switch(*msgPtr)	{
		case KEY_CHANGE:
 		        if ( msgPtr[1] & EVAL_SW4 ){  // Switch 4
		           //PORTE = ~(0x10);
                        }
                        else 
                           if ( msgPtr[1] & EVAL_SW3 ){ // Switch 3
        		      //PORTE = ~(0x20);
                           }
                           else 
                             if ( msgPtr[1] & EVAL_SW2 ){ // Switch 2
			        //PORTE = ~(0x40);
			        if(state){
						state = 0;
			           GPIOSetValue( LED_PORT, LED_BIT, LED_OFF );
			        	}
				else{
											state = 1;
			           GPIOSetValue( LED_PORT, LED_BIT, LED_ON );

					}
					
			     }
                             else 
                                if ( msgPtr[1] & EVAL_SW1 ){ // Switch 1
				   //PORTE = ~(0x80);
                                }
		      break;
                default:
		      break;
		}
		osal_msg_deallocate( msgPtr );   //liu0925 note must add ,!!!!!! send message alloc,reveive message must dealloc!!!
    }
  }

#if 0
  if ( events & xxx ) {
    func_xxx();    
  }

  if ( events & xx ) {
    func_xx();    
  }

  if ( events & x ) {
    func_x();    
  }
#endif
  
  return 0;
 }
