
#ifndef _BUT_FUNC_H_
#define _BUT_FUNC_H_

#include "attr_def.h"

/*
**  Color Definitions
*/

#define BLACK                   0
#define BLUE                    1
#define GREEN                   2
#define CYAN                    3
#define RED                     4
#define MAGENTA                 5
#define BROWN                   6
#define LITE_GREY               7
#define GREY                    8
#define LITE_BLUE               9
#define LITE_GREEN              10
#define LITE_CYAN               11
#define LITE_RED                12
#define LITE_MAGENTA            13
#define YELLOW                  14
#define WHITE                   15

#define PROG_BUTTON             50
#define MON_BUTTON              51
#define AMBA_BUTTON             52
#define OUT_BUTTON_1            53
#define OUT_BUTTON_2            54
#define OUT_BUTTON_3            55
#define OUT_BUTTON_4            56
#define OUT_BUTTON_5            57
#define OUT_BUTTON_6            58
#define OUT_BUTTON_7            59
#define HAND_SET_BUTTON         60
#define AUTO_ANSWER_BUTTON      61
#define PATCH_BUTTON            62
#define IN_BUTTON_1             63
#define IN_BUTTON_2             64
#define IN_BUTTON_3             65
#define IN_BUTTON_4             66
#define IN_BUTTON_5             67
#define IN_BUTTON_6             68
#define IN_BUTTON_7             69
#define DIR_BUTTON              70
#define CALL_BUTTON             71
#define CLEAR_BUTTON            72
#define SELECT_ALL_BUTTON       73
#define HOLD_BUTTON             74
#define LISTEN_BUTTON           75
#define CALL_BACK_BUTTON        76
#define INTRUDE_BUTTON          77
#define TRANSFER_BUTTON         78
#define POST_DIAL_BUTTON        79
#define GROUP_PAGE_BUTTON       80
#define NEXT_PAGE_BUTTON        81
#define PREV_PAGE_BUTTON        82
#define RESET_GROUP_BUTTON      83
#define MASTER_Q_BUTTON         84
#define QUEUE_BUTTON_1          85
#define QUEUE_BUTTON_2          86
#define QUEUE_BUTTON_3          87
#define QUEUE_BUTTON_4          88
#define QUEUE_BUTTON_5          89

typedef struct
{
    int         butt_num;
    int         butt_max;
    int         sv_handle;
    ob_desc    *butt_struct;
    fm_desc     butt_form;
} BUTTON_DESC;


EXPORT void FlashButton ( BUTTON_DESC butval );
EXPORT void ToggleButton ( BUTTON_DESC butval );

EXPORT void SetButtonAttr ( void );
EXPORT void SetWindowAttr ( void );

#endif


