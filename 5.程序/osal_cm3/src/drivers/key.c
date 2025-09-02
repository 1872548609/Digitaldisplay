#include "osaldef.h"
#include "OnBoard.h"
#include "OSAL.h"

#include "gpio.h"
#include "key.h"  


byte Key_TaskID;  
byte registeredKeysTaskID = 0xFF;
byte _SavedKeys;


void KBInit( void )
{  
  uint32 keys;
 /* now pull up port2_1  ,it default is pullup*/
  LPC_IOCON->PIO2_1 &= ~0x0C;  
  LPC_IOCON->PIO2_1 |= 0x08;
 

#if defined ( KB_INT )
  /* Initialize GPIO (sets up clock) */
  // GPIOInit();
 // NVIC_EnableIRQ(EINT0_IRQn);
  //NVIC_EnableIRQ(EINT1_IRQn);
  NVIC_EnableIRQ(EINT2_IRQn);
  //NVIC_EnableIRQ(EINT3_IRQn);

  /* use port2_1 as input event, interrupt test. */
  GPIOSetDir( PORT2, 1, 0 );

  //keys = LPC_GPIO2->DATA;
  //keys = LPC_GPIO2->DATA;
 
  /* port2_1, single edge trigger, active high. */
  GPIOSetInterrupt( PORT2, 1, 0, 0, 0 );
  GPIOIntEnable( PORT2, 1 );
#else
  LPC_SYSCON->SYSAHBCLKCTRL |= (1<<6);
  GPIOSetDir( PORT2, 1, 0 );
#endif // KB_INT
 
 _SavedKeys = 0;
}

    
#if defined ( KB_INT )  
void KBIntSetup( byte state_on )
{
   //state_on=state_on;   //now don't use, if you use key interrput mode ,can use it to do interrupt init!!!
   if(state_on)
   	GPIOIntEnable( PORT2, 1 );
   else
   	GPIOIntDisable( PORT2, 1 );
   	
}
#endif

void SendKeys( byte keys )
{
  byte *msgPtr;
  byte *pBuf;

  // Send the address to the task
  if ( osal_msg_allocate( &msgPtr, 2 ) == ZSUCCESS ) {
    pBuf = msgPtr;
    *pBuf++ = KEY_CHANGE; // Command ID
    //*pBuf++ = DetermineKeyShift();
    *pBuf = keys;
    osal_msg_send( registeredKeysTaskID, msgPtr, 2 );
  }
}

/*
 * Read the keys from the evaluation board
 */
byte OnBoard_GetKeys( void )
{
  byte keys;

  keys = LPC_GPIO2->DATA;  // Read the keys

  // Flip the keys bits
  keys = ~keys & EVAL_SW_MASK;  //key down is low !!!

#if !defined ( KB_INT )
  if ( keys == _SavedKeys )
    return ( 0 );
  
  _SavedKeys = keys;
#endif

  return ( keys );
}


/*
 * Routine to poll for key presses
 */
void OnBoard_read_keys( void )
{
  byte keys;
  
  keys = OnBoard_GetKeys();

  if ( (keys & EVAL_SW_MASK) == 0 )
    return;
    
  //BuzzerControl( BUZZER_BLIP );  //key buzz!
 
  // Is there a registed task?    
  if ( registeredKeysTaskID != 0xFF ) {
    SendKeys( keys );
  }
  else {
    if ( keys & EVAL_SW4 ){  // Switch 4
    }
    // Pass on the key to the switch application
    else if ( keys & EVAL_SW3 ){ // Switch 3
    }
    // Pass on the key to the switch application
    else if ( keys & EVAL_SW2 ){ // Switch 2
    }
    // Pass on the key to the switch application
    else if ( keys & EVAL_SW1 ){ // Switch 1
    }
  }
}


#if defined ( KB_INT )  
/*
 * Keyboard Interrupt Module Interrupt Service Routine
 */
void PIOINT2_IRQHandler(void)      /*  Keyboard_ISR */
{
  uint32 regVal;

  //KBIntSetup( false );  // if disable intr port2_1 ,then int statu is error

#if 1
  regVal = GPIOIntStatus( PORT2, 1 );
  if ( regVal )  {
#else
   regVal =LPC_GPIO2->DATA;
   regVal &= EVAL_SW2; //(EVAL_SW1|EVAL_SW2|EVAL_SW3|EVAL_SW4)
   if ( regVal ==0 ) {
#endif   
	GPIOIntClear( PORT2, 1 );
	 osal_set_event( Key_TaskID, KEYPRESS_DEBOUNCE_EVT );
  }

  //KBIntSetup( TRUE );  // open intr at debouce evt handle
  KBIntSetup( false );
    
  return;
}

#endif // KB_INT

/*********************************************************************
 * Keyboard Register function
 * If a task registers, it will get all the keys.  You can change this
 * to register for individual keys.
 */
byte RegisterForKeys( byte task_id )
{
  // Allow only the first task
  if ( registeredKeysTaskID == 0xFF ) {
    registeredKeysTaskID = task_id;
    return ( true );
  }
  else
    return ( false );
}


void Key_TaskInit( byte task_id )
{  
 Key_TaskID = task_id;
#if !defined ( KB_INT ) 
 osal_set_event( task_id, KEYPRESS_POLL_EVT );
#endif
}

unsigned char Key_ProcessEvent( byte task_id, UINT16 events )
{      
#if defined ( KB_INT )
  if ( events & KEYPRESS_SETUP_INT_EVT )
    KBIntSetup( true );
    //KBInit();
    
  if ( events & KEYPRESS_DEBOUNCE_EVT )
    osal_start_timerEx( Key_TaskID, KEYPRESS_POLL_EVT, 50 );
#endif  


  if ( events & KEYPRESS_POLL_EVT ) {
    OnBoard_read_keys();
    
#if defined ( KB_INT )  
    osal_set_event( task_id, KEYPRESS_SETUP_INT_EVT );
#else    
    osal_start_timerEx( Key_TaskID, KEYPRESS_POLL_EVT, 100 );
#endif    
  }  
  return 0;
 }
