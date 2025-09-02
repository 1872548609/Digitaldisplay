#ifndef ONBOARD_H
#define ONBOARD_H

/*********************************************************************
    Filename:       OnBoard.h
    Revised:        $Date: 2008/09/20 $
    Revision:       $Revision: 0.1 $

    Description:    Defines stuff for  boards.
    Notes:          None.
*********************************************************************/

#ifdef __cplusplus
extern "C"
{
#endif

#ifdef __GNUC__
#define GCC
#define __packed __attribute__ ((packed))
#else
#define __packed
#endif



#define LPC11XX
#define LPC1114
#define ENABLE_SERDEBUG_MESSAGES  TRUE  //FALSE //

#if defined(LPC11XX) // || defined()
   #include <LPC11xx.h> 
extern unsigned long _etext;
extern unsigned long _data;
extern unsigned long _edata;
extern unsigned long _bss;
extern unsigned long _ebss;
/*#elif defined()
  #include  <LPC11xx.h> */
#elif 
  #error not define mcu type!
#endif   

#define TIMER_INT    /* kernel tick use timer interrupt!!!*/

//1.about kernel or task!
// Internal MCU RAM addresses
#if defined( LPC1114 ) //|| defined( )
#define MCU_RAM_BEG 0x10000000
#define MCU_RAM_END 0x10001f00       /* 1ff0 stack top ,(0xf0 stacksize) ??? */
/*#elif defined()
#define MCU_RAM_BEG 0x0100
#define MCU_RAM_END 0x0100 */
#endif
 

/*********************************************************************
 * INCLUDES
 */
 
/*********************************************************************
 * MACROS
 */
#if defined ( WIN32 )
  #define IntsStorage  byte bIntsStorage
  #define StoreDisableInts
  #define RestoreInts
  #define DisableInts
  #define EnableInts
#else
  #define IntsStorage
  #define StoreDisableInts   __disable_irq    /* liunote  for store,restore  have intr pending regs */
  #define RestoreInts           __enable_irq
  #define DisableInts           __disable_irq
  #define EnableInts            __enable_irq
#endif

// Restart system from absolute beginning
#ifdef WIN32
  #define SystemReset() 
#else 
  #define SystemReset()
#endif

// Jump to Serial Port Bootloader
#define BootLoader() 

// Power down the CPU and wait for interrupt
#ifdef WIN32
  #define OSAL_SET_CPU_INTO_SLEEP
#else
  #define OSAL_SET_CPU_INTO_SLEEP              //liu0920  can use sleep lib ,at sleep.h
#endif

//some kernel use hw register flag
/* debug uart register flag */
#define RS485_ENABLED		0
#define TX_INTERRUPT		0	/* 0 if TX uses polling, 1 interrupt driven. */
#define MODEM_TEST			0

#define IER_RBR		0x01
#define IER_THRE	0x02
#define IER_RLS		0x04

#define IIR_PEND	0x01
#define IIR_RLS		0x03
#define IIR_RDA		0x02
#define IIR_CTI		0x06
#define IIR_THRE	0x01

#define LSR_RDR		0x01
#define LSR_OE		0x02
#define LSR_PE		0x04
#define LSR_FE		0x08
#define LSR_BI		0x10
#define LSR_THRE	0x20
#define LSR_TEMT	0x40
#define LSR_RXFE	0x80

#define BUFSIZE		0x40


/* system tick */


/* for onboard uart debug   */
extern void Uart_Debug_Init(void);

extern void Uart_Debug_SendByte(byte ch);  //alternate at app or int!!!

#define UART_DEBUG_BITRATE   115200



// Timer interrupts @ 1.0 msecs countdown
#define TC_OCC   (/*__SYSTEM_CLOCK*/SystemCoreClock/1000 * 0x1)

/* Timer Control Functions */
  /* 
   * Check for a timer tick.
   */
//  extern byte TimerTickCheck( void );   //liu0920 remove
   
/* 
   * Set up timer control registers.
   *   on:    TRUE or FALSE
   *   count: timer interrupt count
*/
extern void TimerSetControls( byte on, uint32 count );  /* cm3 count 24bit */
   
/* 
   * Enable/disable timer interrupts.
   *   on:    TRUE or FALSE
 */
extern void TimerSetInterrupt( byte on );



//liu0920 message id must define in onboard.h!!!
// Task Message ID - Outgoing to apps
#define KEY_CHANGE                0xC0


//liu0920 add for osal.c osal_init_system call osalAddTasks
extern void osalAddTasks( void );

//extern void PortInit(void);
extern void InitBoard( void );

//2. about driver 

/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* ONBOARD_H */
