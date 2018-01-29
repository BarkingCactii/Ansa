
#ifndef _BASE_PNL_H_
#define _BASE_PNL_H_

#define MIN_X_COORDIN           5
#define MAX_X_COORDIN           634

#define MIN_Y_COORDIN           1
#define MAX_Y_COORDIN           478

#define TOP_LINE_DEPTH          (MIN_Y_COORDIN+21)
#define BOT_LINE_DEPTH          (MAX_Y_COORDIN-41)
#define BOT_LINE_WIDTH          (MIN_X_COORDIN+344)

#define NUMBER_OF_PANELS        21

#define NULL_PANEL              0x00000000L

#define PIN_ON_PANEL            0x00000001L     /*  0   */
#define LOGGED_ON_PANEL         0x00000002L     /*  1   */
#define DIR_EDIT_PANEL          0x00000004L     /*  2   */
#define CONFIG_PANEL            0x00000008L     /*  3   */
#define PIN_OFF_PANEL           0x00000010L     /*  4   */
#define ENTRY_PIC_PANEL         0x00000020L     /*  5   */
#define ENTRY_CTL_PANEL         0x00000040L     /*  6   */
#define GROUP_PAGE_PANEL        0x00000080L     /*  7   */
#define PROG_PANEL              0x00000100L     /*  8   */
#define PIN_OVER_PANEL          0x00000200L     /*  9   */
#define ASSIGN_PIC_PANEL        0x00000400L     /* 10   */
#define ASSIGN_CTL_PANEL        0x00000800L     /* 11   */
#define AMBA_PANEL              0x00001000L     /* 12   */
#define EXTN_CONF_PANEL         0x00002000L     /* 13   */
#define TRANSFERS_PANEL         0x00004000L     /* 14   */
#define DIRECTORY_PANEL         0x00008000L     /* 15   */
#define DIAL_KEYPAD_PANEL       0x00010000L     /* 16   */
#define MASTER_Q_PANEL          0x00020000L     /* 17   */
#define OMNIBUS_PANEL           0x00040000L     /* 18   */
#define POSTDIAL_PANEL          0x00080000L     /* 19   */
#define TELEPHONY_PANEL         0x00100000L     /* 20   */
                                  
#define NUM_PIN_KEYPAD_BUTTONS  12
#define NUM_LOGGED_ON_BUTTONS   4
#define NUM_CONFIG_BUTTONS      10

#define NUM_TELEPHONY_BUTTONS   90
#define NUM_PROG_PANEL_BUTTONS  6
#define NUM_ASS_EXIT_BUTTONS    2
#define NUM_AMBA_BUTTONS        7
#define NUM_EXTD_CONF_BUTTONS   12
#define NUM_GROUP_CONF_BUTTONS  19
#define NUM_XFR_PANEL_BUTTONS   5
#define NUM_DIR_LIST_BUTTONS    8
#define NUM_DIAL_KEYPAD_BUTTONS 17
#define NUM_OMNI_LIST_BUTTONS   6

EXPORT ulong        panel_mask;
EXPORT BUTTON_DESC  null_panel_value;

#endif

