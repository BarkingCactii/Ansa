/*
 *  File        :   comports.h
 *
 *  Project     :   SRA VPR26   -   Enhanced Conference Facility.
 *  Author      :   Ansa Pty. Ltd. (c) 1992
 *
 *  Description :   IBM PC asynchronous RS232 i/o registers and various
 *                  utility procedures:
 *
 *                      COM1 uses 8250 communications controller connected
 *                      via channel 4 (IRQ 4) of 8259 interrupt controller
 *
 *                      COM2 uses 8250 communications controller connected
 *                      via channel 3 (IRQ 3) of 8259 interrupt controller
 */

#include <bios.h>

#define _NO_COM     (-1)
#define _COM_1      0
#define _COM_2      1

EXPORT int	OpenComPort ( int port, WORD parameters );
EXPORT int	CloseComPort ( int port );
EXPORT void ClearComPort ( int port );

EXPORT int  IsComBufferReady ( int port );
EXPORT BITE ComGetChar ( int port, WORD *error );
EXPORT int  ComPutChar ( int port, BITE ch );
EXPORT int  ComPutStr ( int port, BITE *str );
EXPORT int  CheckComPortStatus ( int port, WORD *status );

EXPORT void DTR_On ( int port );
EXPORT void DTR_Off ( int port );

