/*
===============================================================================
 Name        : main.c
 Author      : 
 Version     :
 Copyright   : (C) Copyright
 Description : main definition
===============================================================================
*/

#ifdef __USE_CMSIS
#include "LPC11xx.h"
#endif

// TODO: insert other include files here

// TODO: insert other definitions and declarations here

#if 0
int main(void) {
	
	// TODO: insert code here

	// Enter an infinite loop, just incrementing a counter
	volatile static int i = 0 ;
	while(1) {
		i++ ;
	}
	return 0 ;
}
#endif


#include "OSALDef.h"
#include "OSAL.h"   
#include "OSAL_Tasks.h"

#include "onboard.h" 

#include "OSAL_debug.h"   

#include "key.h"

#include "app.h"

#include "gpio.h"

void osalAddTasks( void )     //1.must define this function ,2.and must place in main.c (externed at onboard.h)!
{
  //osalTaskAdd( OnBoard_TaskInit, OnBoard_ProcessEvent );
  osalTaskAdd( Key_TaskInit, Key_ProcessEvent );
  osalTaskAdd( App_TaskInit, App_ProcessEvent );
}

//const char msg1[]="osal start system";
void main( void )
{
  //uint16 bootCounter;    

  
  osal_int_disable( INTS_ALL );
  
  GPIOInit();

  InitBoard(); 
  
  // Initialize the operating system
  osal_init_system();
  
  osal_int_enable( INTS_ALL );

                              
  //Uart_Debug_SendString("osal start system");
  osal_start_system(); // No Return from here

} /* main() */
