
#ifndef _DIAL_PNL_H_
#define _DIAL_PNL_H_

#define MAX_LCD_CHARS       (PHONE_NUM_LEN - 5)

EXPORT BUTTON_DESC  dial_keypad_value;

EXPORT int  DisplayDialKeypad ( int flag );
EXPORT void RemoveDialKeypad ( void );
EXPORT void DrawLCDText ( char *msg, int len );

EXPORT void DefineDialKeypad ( void );

#endif

