
#ifndef _MESG_PNL_H_
#define _MESG_PNL_H_

#define GFX_NO_MESSAGE      0
#define GFX_PIN_OFF         1
#define GFX_PIN_ON          2
#define GFX_PIN_OVER        3
#define GFX_BAD_PIN         4
#define GFX_SAVE_CFG        5
#define GFX_LOAD_CFG        6
#define GFX_PIN_BUSY        7
#define GFX_LST_DEAD        8
#define GFX_LST_BUSY        9
#define GFX_CALIB_TO        10
#define GFX_BAD_CALIB       11
#define GFX_CALIB_OK        12
#define GFX_BAD_ACBU        13
#define GFX_BAD_ECBU        14
#define GFX_MODE_TOG        15
#define GFX_MODE_ON         16
#define GFX_MODE_OFF        17
#define GFX_BAD_NETW        18
#define GFX_BAD_ICUX        19
#define GFX_NO_LINES        20
#define GFX_CALL_DIAL       21
#define GFX_OMNI_PROB       22
#define GFX_CALL_FAIL       23
#define GFX_MODE_NA         24
#define GFX_BAD_AMBA        25
#define GFX_ECBU_CCTS       26
#define GFX_AMBA_FAIL		27
#define GFX_BIG_AMBA        28
#define GFX_NO_DIGIT        29
#define GFX_CUR_AMBA        30
#define GFX_SEL_NONE        31
#define GFX_SEL_ONLY        32
#define GFX_NO_PARTY        33
#define GFX_PANEL_ERR       34
#define GFX_NO_CALL         35
#define GFX_ANSA_FAIL       36
#define GFX_PTT_FAIL        37
#define GFX_BAB_ARSE        38

EXPORT int  Msg_mode_flag;
EXPORT int  Msg_numb_of_boxes;

EXPORT void IsBoxRemovable ( void );
EXPORT void IsDialogueBoxTimeUp ( void );

EXPORT void DialogueBoxOn ( int index );
EXPORT void DialogueBoxOff ( void );
EXPORT void PermanentBoxOn ( int index );

#endif

