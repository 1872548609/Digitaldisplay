/*********************************************************************
    Filename:       OnBoard.c
    Revised:        $Date: 2008/09/20 $
    Revision:       $Revision: 0.1 $
    
    Description:    This file contains the UI and control for the 
                    Peripherals on the board.
    
    Notes:          
*********************************************************************/


/*********************************************************************
 * INCLUDES
 */
#include "OSALDef.h"
#include "OSAL.h" 

#include "OnBoard.h"
                
//derivers include
#include "key.h"


#if (ENABLE_SERDEBUG_MESSAGES==TRUE)
void Uart_Debug_Init(void)
{
  uint32_t Fdiv;
  uint32_t regVal;

  //UARTTxEmpty = 1;
  //UARTCount = 0;
  
  NVIC_DisableIRQ(UART_IRQn);

   /*  UART I/O config */
  LPC_IOCON->PIO1_6 &= ~0x07;
  LPC_IOCON->PIO1_6 |= 0x01;     /* UART RXD */
  LPC_IOCON->PIO1_7 &= ~0x07;	
  LPC_IOCON->PIO1_7 |= 0x01;     /* UART TXD */
  
  /* Enable UART clock */
  LPC_SYSCON->SYSAHBCLKCTRL |= (1<<12);
  LPC_SYSCON->UARTCLKDIV = 0x1;     /* divided by 1 */

  LPC_UART->LCR = 0x83;             /* 8 bits, no Parity, 1 Stop bit */
  regVal = LPC_SYSCON->UARTCLKDIV;

  Fdiv = (((SystemCoreClock*LPC_SYSCON->SYSAHBCLKDIV)/regVal)/16)/UART_DEBUG_BITRATE ;	/*baud rate */

  LPC_UART->DLM = Fdiv / 256;
  LPC_UART->DLL = Fdiv % 256;
  LPC_UART->LCR = 0x03;		/* DLAB = 0 */
  LPC_UART->FCR = 0x07;		/* Enable and reset TX and RX FIFO. */

  /* Read to clear the line status. */
  regVal = LPC_UART->LSR;

  /* Ensure a clean start, no data in either TX or RX FIFO. */
// CodeRed - added parentheses around comparison in operand of &
  while (( LPC_UART->LSR & (LSR_THRE|LSR_TEMT)) != (LSR_THRE|LSR_TEMT) );
  while ( LPC_UART->LSR & LSR_RDR )
  {
	regVal = LPC_UART->RBR;	/* Dump data from RX FIFO */
  }

#if 0            /* debug only send msg and use polling mode */
  /* Enable the UART Interrupt */
  NVIC_EnableIRQ(UART_IRQn);

#if TX_INTERRUPT
  LPC_UART->IER = IER_RBR | IER_THRE | IER_RLS;	/* Enable UART interrupt */
#else
  LPC_UART->IER = IER_RBR | IER_RLS;	/* Enable UART interrupt */
#endif
#endif

  return; 

}

//=====================================================================
void Uart_Debug_SendByte(byte ch)  //alternate at app or int!!!
{
	  /* Below flag is set inside the interrupt handler when THRE occurs. */
	  
	if(ch=='\n'){
              //Delay(10);               //because the slow response of hyper_terminal 


		/* Check status in the LSR to see if */
		while ( !(LPC_UART->LSR & LSR_THRE) );
			LPC_UART->THR = '\r';
	}

	while ( !(LPC_UART->LSR & LSR_THRE) );
			LPC_UART->THR = ch;

}   
#endif   /*end ENABLE_SERDEBUG_MESSAGES==TRUE*/

/*********************************************************************
 * LOCAL FUNCTIONS
 */
 
/*********************************************************************
 * @fn      Low Voltage Detect Interrupt
 * @brief   This ISR is called when power is low.
 * @param   
 * @return  
 */
/*interrupt [EXT_INT0]*/ void LowVoltageDetectISR( void )
{
  
  // PUT YOUR CODE HERE to handle low power indicator
  
  // This code will toggle LEDs forever until power is gone
  //osal_int_disable( INTS_ALL );  // Might not be needed since this is ISR.
  //for ( ;; )
  //  runThroughLEDs();
}

/*********************************************************************
 * @fn      Initialize the Board Peripherals
 * @brief   
 * @param   
 * @return  
 */
void InitBoard( void )
{  
    //1.Port Init   
    //PortInit();
    
    //2. External Interrupt(s) initialization

    //3. Timer(s)/Counter(s) Interrupt(s) initialization
    
#if (ENABLE_SERDEBUG_MESSAGES==TRUE)
    Uart_Debug_Init();
#endif     
    
    //4.hardware drivers init!
     KBInit();
} 

/* about system clk  P13 P16 P41(FCLKOUT Frequency of sys_pllclkout)
system oscillator (12M cystal)\
                                            => (SYSPLLCLKSEL=1)=>sys_pllclkin(12M) =>
IRC oscillator      (12M clock) /

=>(MAINCLKSEL=3,SYSPLLCTRL=0x23[M=3,P=1])=> main clock=sys_pllclkout(FCLKOUT)=12x(M+1)=48M
=>(SYSAHBCLKDIV=0x1)=>system clock= main clock/SYSAHBCLKDIV =48M

main clock = 48M
system clock = 48M

SystemCoreClock = system clock = 48M


*/

/*********************************************************************
 *                    TIMER CONTROL FUNCTIONS
 *
 * This development board uses Timer/PWM 1 to provide
 * system clock ticks for the OSAL scheduler. These functions perform
 * the hardware specific actions required by the OSAL_Timers module.
 */
 
/*********************************************************************
 * @fn      TimerSetControls
 *
 * @brief   Sets various time control registers
 * @param   turnOn - 0=disable, !0=enable
 * @param   count - Output Compare A pre-load
 * @return  none
 */
void TimerSetControls( byte turnOn, uint32 count )  /* 0-23 24bit*/
{

  if ( turnOn ){
   if (SysTick_Config(count/* SystemCoreClock / 1000*/)) { /* Setup SysTick Timer for 1 msec interrupts  */
    while (1);                                  /* Capture error */
   }
  }
  else {
  	      SysTick->CTRL  &=  ~( SysTick_CTRL_TICKINT_Msk   | 
                   SysTick_CTRL_ENABLE_Msk );                    /* Disable SysTick IRQ and SysTick Timer */
  }
}

/*********************************************************************
 * @fn      TimerSetInterrupt
 *
 * @brief   Enables or disables timer interrupts
 * @param   0=disable, !0=enable
 * @return  none
 */
void TimerSetInterrupt( byte turnOn ) 
{
  if ( turnOn ){
    // Timer(s)/Counter(s) Interrupt(s) initialization
   SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk | 
                   SysTick_CTRL_TICKINT_Msk   | 
                   SysTick_CTRL_ENABLE_Msk;                    /* Enable SysTick IRQ and SysTick Timer */
  }
  else {
  	      SysTick->CTRL  &=  ~( SysTick_CTRL_TICKINT_Msk   | 
                   SysTick_CTRL_ENABLE_Msk );                    /* Disable SysTick IRQ and SysTick Timer */
  	
  	}
}


/*********************************************************************
 * @fn      TimerTickCheck
 *
 * @brief   Checks for, and clears the timer's Compare Interrupt Flag
 * @param   none
 * @return  0x00=no interrupt, 0x40=Compare A Match interrupt
 */
 #if 0
byte TimerTickCheck( void )    //only for timer tick at polling mode !!!
{
  byte stat;
  
  stat = ?;  // Timer status
  
  if ( stat & ? )
    ? = stat ;  // Reset TOV0 flag ,use write 1!!!
  else
    stat = 0;  // No tick this time

  return ( stat );
}
#endif


volatile uint32_t msTicks;                            /* counts 1ms timeTicks */
/*----------------------------------------------------------------------------
  SysTick_Handler
 *----------------------------------------------------------------------------*/
void SysTick_Handler(void) {
#if 0
  osalTimerRec_t *srchTimer;
  osalTimerRec_t *prevTimer;
  osalTimerRec_t *saveTimer;     
#endif  
  msTicks++;                        /* increment counter necessary in Delay() */
#if 1
    osal_update_timers( );
#else  


  // Update the system time
  osal_systemClock += tmr_decr_time;

  // Look for open timer slot
  if ( timerHead != NULL ) {
    // Add it to the end of the timer list
    srchTimer = timerHead;
    prevTimer = (void *)NULL;
  
    // Look for open timer slot
    while ( srchTimer ) {
      /* Decrease the correct amount of time */
      if (srchTimer->timeout >= tmr_decr_time)
        srchTimer->timeout = srchTimer->timeout - tmr_decr_time;
        
      /* When timeout, execute the task */
      if ( srchTimer->timeout == 0 ) {
        osal_set_event( srchTimer->task_id, srchTimer->event_flag );
        
        // Take out of list
        if ( prevTimer == NULL )
          timerHead = srchTimer->next;
        else
          prevTimer->next = srchTimer->next;

        // next 
        saveTimer = srchTimer->next;

        // Free memory
        osal_mem_free( srchTimer );

        srchTimer = saveTimer;
      }
      else {
        // Get next
        prevTimer = srchTimer;
        srchTimer = srchTimer->next;
      }
    }
  
    if ( osal_timer_num_active() == 0 ) {
      osal_timer_hw_setup( false );  // Turn timer hardware off
      timerActive = false;
    }
    else {
#ifdef POWER_SAVING
      osal_retune_timers();
#endif
    }
  }
#endif     
}


