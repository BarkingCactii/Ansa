
#ifndef _TSR_FUNC_H_
#define _TSR_FUNC_H_

#define tsMSG       0x0010

/*
 *  touch screen message types
 */

#define tsPRESS     0x0001
#define tsCALIB     0x0002
#define tsRESET     0x0004
#define tsSTART     0x0008

/*
 *  touch screen message values
 */

#define noTOUCH     0x0000
#define blTOUCH     0x0001
#define trTOUCH     0x0004
#define tmTOUCH     0x0008

/*
 *  touch screen message flags
 */

#define cFAILURE    0x0000
#define cSUCCESS    0x0001

EXPORT void CalibrateTouchScreen ( void );
PUBLIC int  ActivateTouchScreen ( void );
PUBLIC int  DeactivateTouchScreen ( void );

EXPORT int	_pascal Tsr_PollPort ( int *msg_type, int *value, int *flag );

PUBLIC int  Tsr_InitTouchScreen ( int close_flag );

#endif

