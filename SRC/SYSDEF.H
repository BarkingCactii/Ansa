/* 
	sysdef.h

    System  :  Train Control Command Communication System
    Author  :  Ansa Pty. Ltd. (c) 1991
    Revision:  1.0
*/

#ifndef _SYSDEF_H_
#define _SYSDEF_H_

#include "isam.h"
#include "isamerr.h"

#include "globals.h"

/* 
	the following line types are defined as dummy values 
	until implemented. They are, however, mapped back to
	legitimate values in the ascii files
*/

#define PASSIVE_LINE    'W'
#define ACTIVE_LINE     'X'
#define IN_PARTY_LINE   'Y'
#define OUT_PARTY_LINE  'Z'

/*
	miscellaneous defines
*/
 
#define KEY_LABEL_COLUMNS       6       /* no of characters per row */
#define SELECT_CHAR             ' '     /* selection key for choice lists */
#define SOLID_CHAR              0xdb    /* inverse character */
#define SELECTED_CHAR           '*'     /* selected item in choice list */
#define MAX_PATH                80      /* max length file path */
#define BOXES_PER_ROW           10      /* key labels per row on screen */
#define NO_ARGS                 0       /* function argument not used */
#define AUTO                    0       /* auto positioning of choice lists */
#define MARGIN                  2       /* left margin in choice lists */
#define MAX_ASSIGNS_PER_GROUP   250     /* maximum num active assignments */
#define INTEGRITY_CHK_COL       35      /* column for integrity check messages */
#define ABORT_PROCESS           -1      /* flag when download interrupted */

/*
	internal database function defines
*/

#define EXIT            0
#define BOIND           1
#define EOIND           2
#define NEXT            3
#define PREVIOUS        4
#define FIND            5
#define RECADD          6
#define RECMOD          7
#define RECDEL          8
#define PAGE_UP         9
#define PAGE_DOWN       10
#define RECCURRENT      11
#define NO_OPERATION	20


/* 
	database error and warnings codes 
*/

#define NO_ACTIVE_ASSIGNMENTS	50
#define NO_TERMINALS            51
#define ONE_TERMINAL            52
#define NO_PREV_PAGE            53
#define NO_NEXT_PAGE            54
#define DUPLICATE               55
#define NOT_WRITTEN             56
#define NO_RECORDS              57
#define RECORD_EXISTS           58
#define FORM_INCOMPLETE         59
#define NO_PHONE_RECORDS        60
#define NO_GROUP_RECORDS        61
#define RANGE_ERR_1             62
#define RANGE_ERR_2             63
#define NO_GROUP_PAGES          64
#define NO_PHONE_SELECTED       65
#define ONE_REC_LIMIT           66
#define MAX_ASSIGNS_EXCEEDED	67
#define PAGE_EXISTS             68
#define PARTY_REC_LIMIT         69
#define MONITOR_DEFINED         70
#define WRONG_OMNI_DIG          71
#define INTEGRITY_CHK_FAILED	72
#define PIN_CHK_FAILED          73
#define ASSIGN_CHK_FAILED       74
#define LABELS_CHK_FAILED       75
#define EMERGENCY_RANGE         76
#define NUMBER_RANGE            77
#define CTS_DEFINED             78
#define MIN_IN_TELEPHONY        79
#define MAX_IN_TELEPHONY        80
#define DOWNLOAD_ABORTED        81
#define NO_LABEL_DEFINED        82
#define MDU_CHAN_ERR            83
#define NUMBERS_NOT_UNIQUE      84
#define NO_CTS_DEFINED          85
#define PASSWORD_IN_GROUP       86
#define NON_DELETABLE_REC       87
#define NO_CT_RECS              88
#define PASSWORD_IN_SERVICE_GROUP  89
#define DUPLICATE_PHONES        90
#define DUPLICATE_CTS           91
#define GROUP_PAGE_LIMIT        92
#define CONTROLLER_LABEL_LIMIT  93

typedef struct
{	char		**data;
    int         key_pressed;
	Db_Obj		*handle;
	Index_Obj 	*index;
    int         file_no;
} SCREEN_DETS;

#endif

