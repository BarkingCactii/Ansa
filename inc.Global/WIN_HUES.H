
#ifndef _WIN_HUES_H_
#define _WIN_HUES_H_

#include <sfwin.h>

#define  BKGROUND_CHAR          '±'

/*
 *  defines the attributes. Use these values when calling any window
 *  related functions
 */

#define MENU_NORMAL             17
#define MENU_HILITE         	18
#define MENU_SELECT             19
#define WINDOW_NORMAL           20
#define WINDOW_HILITE           21
#define WINDOW_HEADER       	22
#define FIELD_NORMAL            23
#define FIELD_HILITE            24
#define FIELD_ERROR             25
#define POPUP_NORMAL            26
#define POPUP_HILITE            27
#define POPUP_SHADOW        	28
#define WAIT_WINDOW         	29
#define ERROR_WINDOW        	30
#define TITLE_WINDOW        	31
#define BOX_NORMAL          	32
#define BOX_HILITE          	33
#define BOX_UNUSED          	34
#define BOX_DORMANT         	35
#define BOX_VIEW_MODE         	36
#define WINDOW_HILITE2      	37
#define WINDOW_HILITE3      	38
#define WINDOW_BLINK            39
#define HELP_WINDOW             40
#define HELP_NORMAL             41
#define FAILED_NORNAL           42

EXPORT void InitUserColours ( void );

#endif

