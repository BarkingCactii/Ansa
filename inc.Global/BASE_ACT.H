
#ifndef _BASE_ACT_H_
#define _BASE_ACT_H_

/*
 *  high level (interpretted) user event messages
 */

#define SELECT_BUTT         0x01
#define PRESS_BUTT          0x02
#define ENTER_BUTT          0x03
#define MOVE_BUTT           0x04
#define INPUT_BUTT          0x05
#define PTT_ON_BUTT         0x06
#define PTT_OFF_BUTT        0x07
#define CLEAR_BUTT          0x08
#define INVALID_BUTT        0x09
#define START_BUTT          0x0A
#define FUNC_BUTT           0x0B
#define ESCAPE_BUTT         0x0C
#define BLEFT_BUTT          0x0D
#define TRITE_BUTT          0x0E
#define CALIB_BUTT          0x0F
#define RESET_BUTT          0x10
#define CLR_SEL_BUTT        0x11

/*
 *  validation panel position definition for the base panel structure
 */

#define PIN_ON_BASE         0
#define LOG_ON_BASE         1
#define D_EDIT_BASE         2
#define CONFIG_BASE         3
#define PIN_OF_BASE         4
#define ENT_AS_BASE         5
#define ENT_OK_BASE         6
#define GROUP_BASE          7
#define PROGS_BASE          8
#define POVER_BASE          9
#define A_BASE              10
#define B_BASE              11
#define AMBAS_BASE          12
#define XCONF_BASE          13
#define PATCH_BASE          14
#define DSHOW_BASE          15
#define DKPAD_BASE          16
#define MASTQ_BASE          17
#define OMNIB_BASE          18
#define POSTD_BASE          19
#define PHONE_BASE          20

typedef struct
{
    BITE        type;
    short       x;
    short       y;
    short       status;
    app_msg    *aM;
} ACTION_REC;

EXPORT int  Bas_clear_screen_flag;

EXPORT BUTTON_DESC  *base_panel[NUMBER_OF_PANELS];

EXPORT void IsTimeToActivateTouch ( void );

EXPORT void ActOnUserEvent ( ACTION_REC what );

#endif

