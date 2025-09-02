#ifndef __KEY_H__
#define __KEY_H__

#define KEY_PORT PORT2
#define KEY_PIN    PIND
#define KEY_DDR DDRD

#define KB_INT   //define keyboard use interrupt!!!  


//liu0920 add key drivers task event id  
//#define LED_BLINK_EVT          0x0001
#define KEYPRESS_POLL_EVT      0x0002    //now use polling key !!!
#define KEYPRESS_SETUP_INT_EVT 0x0004
#define KEYPRESS_DEBOUNCE_EVT  0x0008   


#define EVAL_SW_MASK 0x00F       /*0-11 12bit */
#define EVAL_SW4 0x008               /* PIO2_3 */
#define EVAL_SW3 0x004               /* PIO2_2 */
#define EVAL_SW2 0x002               /* PIO2_1 */
#define EVAL_SW1 0x001               /* PIO2_0 */

extern void KBInit( void );
extern byte RegisterForKeys( byte task_id );         
extern void Key_TaskInit( byte task_id );
extern unsigned char Key_ProcessEvent( byte task_id, UINT16 events );

#endif