
#ifndef _PINS_PNL_H_
#define _PINS_PNL_H_

#define MAX_LCD_CHARS       (PHONE_NUM_LEN - 5)

EXPORT BUTTON_DESC  pin_keypad_value;
EXPORT BUTTON_DESC  postdial_pad_value;

EXPORT int  DisplayPinOnKeypad ( void );
EXPORT void RemovePinOnKeypad ( void );

EXPORT int  DisplayPinOffKeypad ( void );
EXPORT void RemovePinOffKeypad ( void );

EXPORT int  DisplayPinOverKeypad ( void );
EXPORT void RemovePinOverKeypad ( void );

EXPORT int  GetSelectedPostParty ( void );
EXPORT int  DisplayPostDialKeypad ( int index );
EXPORT void RemovePostDialKeypad ( void );

EXPORT void DefinePinKeypad ( void );
EXPORT void DefinePostDialKeypad ( void );

#endif

