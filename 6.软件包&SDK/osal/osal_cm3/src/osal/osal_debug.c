
#include "OSALDef.h"
#include "OSAL.h"

#ifndef WIN32
  #include "OnBoard.h"
#endif

#include "OSAL_debug.h"



//***************************[ UART ]******************************
            
//====================================================================
void Uart_Debug_SendString(const char *pt)
{
#if ENABLE_SERDEBUG_MESSAGES
    //EA=0;
    while(*pt)
        Uart_Debug_SendByte(*pt++);
    //EA=1;                 
#endif     
}

#if 0   //some debug helper function ,now dont use
//
// Functional Prototypes
//
//void pOutputByte (unsigned char c);
#define pOutputByte  Uart_Debug_SendByte
void pOutputNumHex (unsigned long n, long depth);
void pOutputNumDecimal (unsigned int n);
void OutputString (const unsigned char *s);

// If this pointer is not null, pOutputByte() will store characters where 
// it points and increment it instead of calling OEMWriteDebugByte()
char *szSprintf;

//
// Routine starts
//
/*****************************************************************************
*
*
*   @func   void    |   OutputFormatString | Simple formatted output string routine
*
*   @rdesc  none
*
*   @parm   const unsigned char * |   sz,... |
*               Format String:
*
*               @flag Format string | type
*               @flag u | unsigned
*               @flag d | int
*               @flag c | char
*               @flag s | string
*               @flag x | 4-bit hex number
*               @flag B | 8-bit hex number
*               @flag H | 16-bit hex number
*               @flag X | 32-bit hex number
*
*   @comm
*           Same as FormatString, but output to serial port instead of buffer.
*/
/*
   void
   OutputDebugStringW(
   const unsigned char *sz, ...
   )
   {
   }
 */
void
EdbgOutputDebugString (
    const char *sz,...
)
{
    unsigned char c;
	unsigned long int tmp;
    va_list vl;

    va_start (vl, sz);

    // make sure that pOutputByte() calls OEMWriteDebugByte() instead of
    // storing charactes at *szSprintf++
    szSprintf = 0;

    while (*sz)
	{
	    c = *sz++;
	    switch (c)
		{
		    case (unsigned char) '%':
			c = *sz++;
			switch (c)
			    {
				case 'x':
				    pOutputNumHex (va_arg (vl, unsigned long), 0);
				    break;
				case 'B':
				    pOutputNumHex (va_arg (vl, unsigned long), 2);
				    break;
				case 'H':
				    pOutputNumHex (va_arg (vl, unsigned long), 4);
				    break;
				case 'X':
				    pOutputNumHex (va_arg (vl, unsigned long), 8);
				    break;
				case 'd':
				    {
					long l;

					l = va_arg (vl, long);
					if (l < 0)
					    {
						pOutputByte ('-');
						l = -l;
					    }
					pOutputNumDecimal ((unsigned long) l);
				    }
				    break;
				case 'u':
				tmp=va_arg (vl, unsigned  int);
                pOutputNumDecimal(tmp);
				    //pOutputNumDecimal (va_arg (vl, unsigned long int));
				    break;
				case 's':
				    OutputString (va_arg (vl, char *));
				    break;
				case '%':
				    pOutputByte ('%');
				    break;
				case 'c':
				    c = va_arg (vl, unsigned char);
				    pOutputByte (c);
				    break;

				default:
				    pOutputByte (' ');
				    break;
			    }
			break;
		    case '\n':
			pOutputByte ('\r');
			// fall through
		    default:
			pOutputByte (c);
		}
	}

    va_end (vl);
}

/*****************************************************************************
*
*
*   @func   void    |   FormatString | Simple formatted output string routine
*
*   @rdesc  Returns length of formatted string
*
*   @parm   unsigned char * |   pBuf |
*               Pointer to string to return formatted output.  User must ensure
*               that buffer is large enough.
*
*   @parm   const unsigned char * |   sz,... |
*               Format String:
*
*               @flag Format string | type
*               @flag u | unsigned
*               @flag d | int
*               @flag c | char
*               @flag s | string
*               @flag x | 4-bit hex number
*               @flag B | 8-bit hex number
*               @flag H | 16-bit hex number
*               @flag X | 32-bit hex number
*
*   @comm
*           Same as OutputFormatString, but output to buffer instead of serial port.
*/
unsigned int
FormatString (
    unsigned char *pBuf, const unsigned char *sz,...
)
{
    unsigned char c;
    va_list vl;

    va_start (vl, sz);

    szSprintf = pBuf;
    while (*sz)
	{
	    c = *sz++;
	    switch (c)
		{
		    case (unsigned char) '%':
			c = *sz++;
			switch (c)
			    {
				case 'x':
				    pOutputNumHex (va_arg (vl, unsigned long), 0);
				    break;
				case 'B':
				    pOutputNumHex (va_arg (vl, unsigned long), 2);
				    break;
				case 'H':
				    pOutputNumHex (va_arg (vl, unsigned long), 4);
				    break;
				case 'X':
				    pOutputNumHex (va_arg (vl, unsigned long), 8);
				    break;
				case 'd':
				    {
					long l;

					l = va_arg (vl, long);
					if (l < 0)
					    {
						pOutputByte ('-');
						l = -l;
					    }
					pOutputNumDecimal ((unsigned long) l);
				    }
				    break;
				case 'u':
				    pOutputNumDecimal (va_arg (vl, unsigned long));
				    break;
				case 's':
				    OutputString (va_arg (vl, char *));
				    break;
				case '%':
				    pOutputByte ('%');
				    break;
				case 'c':
				    c = va_arg (vl, unsigned char);
				    pOutputByte (c);
				    break;

				default:
				    pOutputByte (' ');
				    break;
			    }
			break;
		    case '\n':
			pOutputByte ('\r');
			// fall through
		    default:
			pOutputByte (c);
		}
	}
    pOutputByte (0);
    c = ((char *) szSprintf - (char *) pBuf);
    szSprintf = 0;
    va_end (vl);
    return (c);
}

/*****************************************************************************
*
*
*   @func   void    |   pOutputByte | Sends a byte out of the monitor port.
*
*   @rdesc  none
*
*   @parm   unsigned int |   c |
*               Byte to send.
*
*/
//static void
/*void
pOutputByte (
    unsigned char c
)
{
    if (szSprintf)
	{
	    //FIXME: need to add code to prevent buffer overrun. DCP
	    *szSprintf++ = c;
	}
    else
	OEMWriteDebugByte (c);
}
*/

/*****************************************************************************
*
*
*   @func   void    |   pOutputNumHex | Print the hex representation of a number through the monitor port.
*
*   @rdesc  none
*
*   @parm   unsigned long |   n |
*               The number to print.
*
*   @parm   long | depth |
*               Minimum number of digits to print.
*
*/
//static void
void
pOutputNumHex (
    unsigned long n,
    long depth
)
{
    if (depth)
	{
	    depth--;
	}

    if ((n & ~0xf) || depth)
	{
	    pOutputNumHex (n >> 4, depth);
	    n &= 0xf;
	}

    if (n < 10)
	{
	    pOutputByte ((unsigned char) (n + '0'));
	}
    else
	{
	    pOutputByte ((unsigned char) (n - 10 + 'A'));
	}
}

/*****************************************************************************
*
*
*   @func   void    |   pOutputNumDecimal | Print the decimal representation of a number through the monitor port.
*
*   @rdesc  none
*
*   @parm   unsigned long |   n |
*               The number to print.
*
*/
//static void
void pOutputNumDecimal ( unsigned int n ) 
{
   Uchar tmp;

   do{
     tmp=n %10;
     pOutputByte ((unsigned char) (n + '0'));
     n=n/10;
   }while(n>=10);

  /*  if (n >= 10)
	{
	    pOutputNumDecimal (n / 10);
	    n %= 10;
	}
    pOutputByte ((unsigned char) (n + '0'));
	*/
}

/*****************************************************************************
*
*
*   @func   void    |   OutputString | Sends an unformatted string to the monitor port.
*
*   @rdesc  none
*
*   @parm   const unsigned char * |   s |
*               points to the string to be printed.
*
*   @comm
*           backslash n is converted to backslash r backslash n
*/
//static void
void
OutputString (
    const unsigned char *s
)
{
    while (*s)
	{
	    if (*s == '\n')
		{
		    pOutputByte ('\r');
		}
	    pOutputByte (*s++);
	}
}

#endif

/*
void OEMWriteDebugString (unsigned short *str)
{
    // Send message to serial port
    //while (*str)
	//  OEMWriteDebugByte ((unsigned char) *str++);
}
*/

/*
 * OEMWriteDebugLED() - Display value to specified LED port.
 *
 * The wIndex parameter can be used to specify a write to the discrete LEDS (if
 * 0xffff), otherwise is used as an offset (DWORD aligned) for the Alpha LEDS
 * for triggering.
 */
/*void OEMWriteDebugLED (WORD wIndex, DWORD dwPattern)
{
}
*/     

/*********************************************************************
*********************************************************************/
