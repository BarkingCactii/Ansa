#pragma pack (1)
/*
	formdef.c

	Form and field definitions for database screens

    System  :  Train Control Command Communication System
    Author  :  Ansa Pty. Ltd. (c) 1991
    Revision:  1.0
*/

#include "sysdef.h"
#include "field.h"
#include "chfunc.h"
#include "filename.h"
#include "lists.h"
#include "primass.h"
#include "smt_help.h"
#include "sdb_defn.h"
#include "attr_def.h"

#define LINE_TYPE_LEN      23

/*------LINE TYPES---------------------------------------------------------*/

PUBLIC	char	*line_names[] = 
{
	"Telecom                ",
	"SRA Auto               ",
    "Omnibus (Magneto) Line ",
    "Selective Calling Line ", 
	"Single Channel Radio   ",
	"Open Channel Radio    ",
	(char *)NULL   
};

PUBLIC	char	line_vals[] = 
{
	EXTERNAL_CALL, 
	INTERNAL_CALL,
	OMNIBUS_CALL,
   SELECTIVE_CALL,
	S_RADIO_CALL,
	O_RADIO_CALL,
	(char)NULL
};

PUBLIC	char	*pl_line_names[] = 
{
	"Single Channel Radio   ",
   "Omnibus (Magneto) Line ",
	(char *)NULL   
};

PUBLIC	char	pl_line_vals[] = 
{
	S_RADIO_CALL,
	OMNIBUS_CALL,
	(char)NULL
};

/*------CALL STATUS--------------------------------------------------------*/

PUBLIC 	char	*callstat_names[] = 
{
	"Normal   ",    
/*	"Priority ",    FUTURE */
	"Emergency",    
	(char *)NULL   
};

PUBLIC	char	callstat_vals[] = 
{
	RING_NORMAL_ATTR, 
/*	RING_PRIORITY_ATTR,  FUTURE */
	RING_EMERGENCY_ATTR, 
	(char)NULL
};

/*------DEFAULT CONTROLLER-------------------------------------------------*/

PUBLIC  char    *default_names[] =
{
    "1 ",
    "2 ",
	"3 ",
	"4 ",
	"5 ",
	"6 ",
	"7 ",
	"8 ",
	"9 ",
    "10",
	(char *)NULL
};

PUBLIC  char    default_vals[] =
{
    '0',
    '1',
    '2',
    '3',
    '4',
    '5',
    '6',
    '7',
    '8',
    '9',
    (char)NULL
};

/*------CONTROLLERS IN TELEPHONY-------------------------------------------*/

PUBLIC	char	*conttele_names[] = 
{
	"2 ",
	"3 ",
	"4 ",
	"5 ",
	"6 ",
	"7 ",
	"8 ",
	"9 ",
	"10",
	(char *)NULL
};

PUBLIC  char    conttele_vals[] =
{
	'C',
	'D',
	'E',
	'F',
	'G',
	'H',
	'I',
	'J',
	'K',
	(char)NULL
};


/*------CONFIG NUMBER------------------------------------------------------*/

PUBLIC	char	*config_names[] = 
{
	"1",
	"2",
	"3",
	"4",
	"5",
	(char *)NULL
};

PUBLIC	char	config_vals[] = 
{
	'1',
	'2',
	'3',
	'4',
	'5',
	(char)NULL
};

/*------CONFERENCE LABEL POSITION------------------------------------------*/

PUBLIC	char	*confpos_names[] = 
{
	"Left most  ",
	"Position 2 ",
	"Position 3 ",
	"Centre     ",
	"Position 5 ",
	"Position 6 ",
	"Right most ",
	(char *)NULL
};

PUBLIC	char	confpos_vals[] = 
{
	'0',
	'1',
	'2',
	'3',
	'4',
	'5',
	'6',
	(char)NULL
};

/*------ECBU PORT NUMBER---------------------------------------------------*/

PUBLIC	char	*ecbuport_names[] = 
{
	"Port 1 ",
	"Port 2 ",
	"Port 3 ",
	"Port 4 ",
	(char *)NULL
};

PUBLIC	char	ecbuport_vals[] = 
{
	'0',
	'1',
	'2',
	'3',
	(char)NULL
};

/*------MONITOR FACILITY---------------------------------------------------*/

PUBLIC	char	*monfac_names[] = 
{
	"Disabled ",
	"Enabled  ",
	(char *)NULL
};

PUBLIC	char	monfac_vals[] = 
{
	'0',
	'1',
	(char)NULL
};

/*------YES/NO------------------------------------------------------------*/

PUBLIC	char	*yesno_names[] = 
{
	"Yes",
	"No ",
	(char *)NULL
};

PUBLIC	char	yesno_vals[] = 
{
	'1',
	'0',
	(char)NULL
};

/*
	field storage for the current form
*/

PUBLIC	char	field[MAX_FIELDS][MAX_FIELD_LEN];
PUBLIC	char	*record[MAX_FIELDS]; 

/*
	storage for choice list information
*/

PUBLIC	char	db_vals[MAX_FIELDS][MAX_FIELD_LEN];
PUBLIC	char	dbdat[MAX_FIELDS][MAX_FIELD_LEN];
PUBLIC	char	*db_names[MAX_FIELDS]; /* = dbdat;*/

/* int AllocateDBStorage()
{
    char    *tmp;

    if (Gen_Alloc(MAX_FIELDS * MAX_FIELD_LEN, &tmp) == FAILURE)
        return(FAILURE);   

}
*/ 
FIELD gpflds[3] = 
{
	{ NULL, NULL , 2, 5,  "Group Page Label:",
    2, 25, field[0], KEY_LABEL_COLUMNS, 'X', 1, 2, 
	FALSE, 0, ENTRY, 
	0, 0, 0, 0, 0,
	LABEL_HLP} ,

	{ NULL, NULL , 3, 5,"", 
	3, 25, field[1], KEY_LABEL_COLUMNS, 'X', 2, 0, 
	FALSE, 0, NON_MANDATORY_ENTRY, 
	0, 0, 0, 0, 0,
	LABEL_HLP} ,

	{ NULL, NULL , 4, 5, "",
	4, 25, field[2], KEY_LABEL_COLUMNS, 'X', 0, 1, 
	FALSE, 0, NON_MANDATORY_ENTRY, 
	0, 0, 0, 0, 0,
	LABEL_HLP }
};

FIELD pnflds[5] = 
{
	{ NULL, NULL , 2, 5, "Target Description:", 
	2, 27, field[0], KEY_LABEL_COLUMNS, 'X', 1, 4,
	FALSE, 0, ENTRY , 
	0, 0, 0, 0, 0,
	LABEL_HLP} ,

	{ NULL, NULL , 3, 5, "", 
	3, 27, field[1], KEY_LABEL_COLUMNS, 'X', 2, 0, 
	FALSE, 0, NON_MANDATORY_ENTRY , 
	0, 0, 0, 0, 0,
	LABEL_HLP} ,

	{ NULL, NULL , 4, 5, "",
	4, 27, field[2], KEY_LABEL_COLUMNS, 'X', 3, 1, 
	FALSE, 0, NON_MANDATORY_ENTRY , 
	0, 0, 0, 0, 0,
	LABEL_HLP} ,

	{ NULL, NULL , 6, 5, "Telephone Number:",
	6, 27, field[3], PHONE_NUM_LEN, 'N', 4, 2, 
	FALSE, 0, ENTRY, 
	0, 0, 0, 0, 0,
	NUMBER_HLP} ,

	{ get_choice, NULL, 8, 3, "* Line Type:",
	8, 27, field[4], LINE_TYPE_LEN, 'X', 0, 3, 
	FALSE, 0, CHOICE , 
	EXTERNAL_CALL, 0, 0, line_vals, line_names,
	CHOICE_HLP }
};

FIELD bpflds[4] = 
{
	{ sel_opgrp_mask, NULL, 2, 3, " * Controller Group Name:",
	2, 30, field[0], KEY_LABEL_LEN, 'X', 1, 3, 
	FALSE, 0, CHOICE, 
	0, OPERATOR_GROUP_FILE, 0, db_vals[0], &db_names[0], 
	CHOICE_HLP} ,

	{ NULL, NULL , 5, 5, "Normal Call:",
	5, 30, field[1], TONE_SET_LEN, 'N', 2, 0, 
	FALSE, 0, ENTRY, 
	0, 0, 0, 0, 0,
	STANDARD_NO_HLP} ,

	{ NULL, NULL ,7, 5, "Priority Call:", 
	7, 30, field[2], TONE_SET_LEN, 'N', 3, 1, 
	FALSE, 0, ENTRY, 
	0, 0, 0, 0, 0,
	PRIORITY_NO_HLP} ,

	{ NULL, NULL , 9, 5, "Emergency Call:",
	9, 30, field[3], TONE_SET_LEN, 'N', 0, 2, 
	FALSE, 0, ENTRY, 
	0, 0, 0, 0, 0,
	EMERGNCY_NO_HLP }
};

FIELD hcflds[11] = 
{	
	{ sel_opgrp_ctgroup, NULL, 2, 3, "* Controller Group:", 
	2, 30, field[0], KEY_LABEL_LEN, 'X', 1, 10, 
	FALSE, 0, CHOICE,  
	0, OPERATOR_GROUP_FILE, 0, db_vals[0], &db_names[0], 
	CHOICE_HLP} ,

	{ sel_extn_mask, NULL, 4, 3, "* Terminal Designation:",
	4, 30, field[1], KEY_LABEL_LEN, 'D', 2, 0, 
	FALSE, 0, CHOICE,  
	0, CT_COMMS_FILE, 0, db_vals[1], &db_names[1], 
	CHOICE_HLP} ,

	{ sel_extn_mask, NULL, 4, 3, "", 
	5, 30, field[2], KEY_LABEL_LEN, 'D', 3, 1, 
	FALSE, 0, NON_MANDATORY_CHOICE,  
	0, CT_COMMS_FILE, 0, db_vals[2], &db_names[2],  
	CHOICE_HLP} ,

	{ sel_extn_mask, NULL, 5, 3, "", 
	6, 30, field[3], KEY_LABEL_LEN, 'D', 4, 2, 
	FALSE, 0, NON_MANDATORY_CHOICE,  
	0, CT_COMMS_FILE, 0, db_vals[3], &db_names[3], 
	CHOICE_HLP} ,

	{ sel_extn_mask, NULL, 6, 3, "", 
	7, 30, field[4],  KEY_LABEL_LEN, 'D', 5, 3,
	FALSE, 0, NON_MANDATORY_CHOICE,  
	0, CT_COMMS_FILE, 0, db_vals[4], &db_names[4], 
	CHOICE_HLP} ,

	{ sel_extn_mask, NULL, 7, 3, "", 
	8, 30, field[5],  KEY_LABEL_LEN, 'D', 6, 4, 
	FALSE, 0, NON_MANDATORY_CHOICE,  
	0, CT_COMMS_FILE, 0, db_vals[5], &db_names[5], 
	CHOICE_HLP} ,

	{ sel_extn_mask, NULL, 8, 3,  "",
	9, 30, field[6],  KEY_LABEL_LEN, 'D', 7, 5, 
	FALSE, 0, NON_MANDATORY_CHOICE,  
	0, CT_COMMS_FILE, 0, db_vals[6], &db_names[6], 
	CHOICE_HLP} ,

	{ sel_extn_mask, NULL, 9, 3,  "",
	10, 30, field[7],  KEY_LABEL_LEN, 'D', 8, 6, 
	FALSE, 0, NON_MANDATORY_CHOICE,  
	0, CT_COMMS_FILE, 0, db_vals[7], &db_names[7], 
	CHOICE_HLP} ,

	{ sel_extn_mask, NULL, 10, 3, "",
	11, 30, field[8],  KEY_LABEL_LEN, 'D', 9, 7, 
	FALSE, 0, NON_MANDATORY_CHOICE,  
	0, CT_COMMS_FILE, 0, db_vals[8], &db_names[8], 
	CHOICE_HLP} ,

	{ sel_extn_mask, NULL, 11, 3, "",
	12, 30, field[9],  KEY_LABEL_LEN, 'D', 10, 8, 
	FALSE, 0, NON_MANDATORY_CHOICE,  
	0, CT_COMMS_FILE, 0, db_vals[9], &db_names[9], 
	CHOICE_HLP} ,

	{ sel_extn_mask, NULL, 12, 3,  "",
	13, 30, field[10], KEY_LABEL_LEN, 'D', 0, 9, 
	FALSE, 0, NON_MANDATORY_CHOICE,  
	0, CT_COMMS_FILE, 0, db_vals[10], &db_names[10], 
	CHOICE_HLP }

};

#define MAX_QUEUE    4
#define MAX_MINUTES  2
#define MAX_SECONDS  1
#define MAX_PICKUP   2

FIELD cgflds[6] =
{	
	{ NULL, NULL , 1, 5, "Min. Queue Group Extension:", 
	1, 37, field[0], MAX_QUEUE, 'D', 1, 5, 
	FALSE, 0, ENTRY, 
	0, 0, 0, 0, 0,
	MIN_QUEUE_EXT} ,
    
	{ NULL, NULL , 2, 5, "Max. Queue Group Extension:", 
	2, 37, field[1], MAX_QUEUE, 'D', 2, 0, 
	FALSE, 0, ENTRY, 
	0, 0, 0, 0, 0,
	MAX_QUEUE_EXT} ,

	{ NULL, NULL , 3, 5, "Call Pick-up Code:",
	3, 37, field[2], MAX_PICKUP, 'D', 3, 1, 
	FALSE, 0, ENTRY, 
	0, 0, 0, 0, 0,
	CALL_PICKUP_HLP} ,

	{ NULL, NULL , 5, 5, "Max. Ring Time For Extensions:        minutes",
	5, 37, field[3], MAX_MINUTES, 'D', 4, 2, 
	FALSE, 0, ENTRY, 
	0, 0, 0, 0, 0,
	MAX_RING_TIME} ,

	{ NULL, NULL , 6, 5, "Incoming Call Overflow TIme:          minutes",
	6, 37, field[4], MAX_MINUTES, 'D', 5, 3, 
	FALSE, 0, ENTRY, 
	0, 0, 0, 0, 0,
	IN_OVERFLOW} ,

	{ NULL, NULL , 7, 5, "Automatic Answer Delay Time:          seconds", 
	7, 37, field[5], MAX_SECONDS, 'D',  0, 4, 
	FALSE, 0, ENTRY, 
	0, 0, 0, 0, 0,
	AUTO_ANS_DELAY} 
};

FIELD ccflds[10] = 
{	
	{ NULL, NULL , 2, 5, "Terminal Designation:", 
	2, 29, field[0], KEY_LABEL_LEN, 'X',1, 9, 
	FALSE, 0, ENTRY, 
	0, 0, 0, 0, 0,
	LABEL_HLP} ,

	{ get_choice, NULL, 4, 5, "* ECBU Port Number:", 
	4, 29, field[1], 7, 'X',2, 0, 
	FALSE, 0, CHOICE, 
	'0', 0, 0, ecbuport_vals, ecbuport_names, 
	CHOICE_HLP} ,

	{ sel_ecbu, NULL, 6, 5, "* ECBU Magazine:",
	6, 29, field[2], KEY_LABEL_LEN, 'X',3, 1, 
	FALSE, 0, CHOICE, 
	0, ECBU_MAGAZINE_FILE, 0, db_vals[2], &db_names[2], 
	CHOICE_HLP} ,

	{ NULL, NULL , 8, 5, "Base ACBU Extension:",
	8, 29, field[3], TONE_SET_LEN, 'N',4, 2, 
	FALSE, 0, ENTRY, 
	0, 0, 0, 0, 0,
	BASE_ACBU_EXT} ,

	{ get_choice, NULL, 8, 37, "* Monitor Facility:",          
	8, 62, field[4], 9, 'X', 5, 3, 
	FALSE, 0, CHOICE, 
	'0', 0, 0, monfac_vals, monfac_names, 
	CHOICE_HLP} ,

	{ NULL, NULL , 10, 5, "Direct Dial Extension:",          
	10, 29, field[5], TONE_SET_LEN, 'N', 6, 4, 
	FALSE, 0, ENTRY, 
	0, 0, 0, 0, 0,
	D_DIAL_EXT} ,

	{ NULL, NULL , 10, 39, "Intrusion Extension:",          
	10, 62, field[6], TONE_SET_LEN, 'N', 7, 5, 
	FALSE, 0, ENTRY, 
	0, 0, 0, 0, 0,
	INTR_EXT} ,

	{ NULL, NULL , 12, 5, "",          
	12, 43, field[7], KEY_LABEL_COLUMNS, 'X', 8, 6, 
	FALSE, 0, ENTRY, 
	0, 0, 0, 0, 0,
	LABEL_HLP} ,

	{ NULL, NULL , 13, 5, "Command Terminal Target Description:",          
	13, 43, field[8], KEY_LABEL_COLUMNS, 'X', 9, 7, 
	FALSE, 0, NON_MANDATORY_ENTRY, 
	0, 0, 0, 0, 0,
	LABEL_HLP} ,

	{ NULL, NULL , 14, 5, "",          
	14, 43, field[9], KEY_LABEL_COLUMNS, 'X', 0, 8, 
	FALSE, 0, NON_MANDATORY_ENTRY, 
	0, 0, 0, 0, 0,
	LABEL_HLP} ,
};

FIELD gcflds[24] = 
{	{ sel_pin, NULL,  1, 3, "* P.I.N:",  
	1, 12, field[0], MAX_PIN_LEN, 'D', 1, 23, 
	FALSE, 0, CHOICE,  
	0, PASSWORD_FILE, 0, db_vals[0], &db_names[0], 
	CHOICE_HLP} ,

	{ get_choice, NULL, 3, 3, "* Target Position:", 
	3, 22, field[1], 11, 'D', 2, 0, 
	FALSE, 0, CHOICE, 
	'1', 0, 0, confpos_vals, confpos_names, 
	LABEL_POS_HLP} ,

	{ NULL, NULL ,       1, 30, "Target Description:",             
	1, 51, field[2], KEY_LABEL_COLUMNS, 'X', 3, 1, 
	FALSE, 0, ENTRY , 
	0, 0, 0, 0, 0,
	LABEL_HLP} ,

	{ NULL, NULL ,       2, 43, "",       
	2, 51, field[3], KEY_LABEL_COLUMNS, 'X', 4, 2, 
	FALSE, 0, NON_MANDATORY_ENTRY, 
	0, 0, 0, 0, 0,
	LABEL_HLP} ,

	{ NULL, NULL ,       3, 35, "",             
	3, 51, field[4], KEY_LABEL_COLUMNS, 'X', 5, 3, 
	FALSE, 0, NON_MANDATORY_ENTRY , 
	0, 0, 0, 0, 0,
	LABEL_HLP} ,

	{ sel_phone_grpconf, sel_terminal_grpconf, 5, 3, "* AMBA Members:",
	5, 35, field[5], PHONE_NUM_LEN + 2,  'D', 6, 4, 
	FALSE, 0, CHOICE,  
	0, NUMBER_FILE, CT_COMMS_FILE, db_vals[5], &db_names[5], 
	CHOICE_HLP },    /* add this to all other records */ 

	{ sel_phone_grpconf, sel_terminal_grpconf,  15, 5, "Enter       Telephone Numbers", 
	6, 7, field[6], PHONE_NUM_LEN + 2,  'D', 7, 5, 
	FALSE, 0, NON_MANDATORY_CHOICE,  
	0, NUMBER_FILE, CT_COMMS_FILE, db_vals[6], &db_names[6], 
	CHOICE_HLP} ,

	{ sel_phone_grpconf, sel_terminal_grpconf,  16, 5, "Cntl-Enter  Command Terminal Numbers", 
	6, 35, field[7], PHONE_NUM_LEN + 2,   'D', 8, 6, 
	FALSE, 0, NON_MANDATORY_CHOICE,  
	0, NUMBER_FILE, CT_COMMS_FILE, db_vals[7], &db_names[7],
	CHOICE_HLP} ,

	{ sel_phone_grpconf, sel_terminal_grpconf,  7, 5, "", 
	7, 7, field[8], PHONE_NUM_LEN + 2,  'D', 9, 7, 
	FALSE, 0, NON_MANDATORY_CHOICE,  
	0, NUMBER_FILE, CT_COMMS_FILE, db_vals[8], &db_names[8],
	CHOICE_HLP} ,

	{ sel_phone_grpconf, sel_terminal_grpconf,  7, 5, "", 
	7, 35, field[9], PHONE_NUM_LEN + 2,  'D', 10, 8, 
	FALSE, 0, NON_MANDATORY_CHOICE,  
	0, NUMBER_FILE, CT_COMMS_FILE, db_vals[9], &db_names[9],
	CHOICE_HLP} ,

	{ sel_phone_grpconf, sel_terminal_grpconf, 7, 5, "", 
	8, 7, field[10], PHONE_NUM_LEN + 2, 'D',11, 9, 
	FALSE, 0, NON_MANDATORY_CHOICE,  
	0, NUMBER_FILE, CT_COMMS_FILE, db_vals[10], &db_names[10],
	CHOICE_HLP} ,

	{ sel_phone_grpconf, sel_terminal_grpconf, 7, 5, "", 
	8, 35, field[11], PHONE_NUM_LEN + 2,  'D',12,10, 
	FALSE, 0, NON_MANDATORY_CHOICE,  
	0, NUMBER_FILE, CT_COMMS_FILE, db_vals[11], &db_names[11],
	CHOICE_HLP} ,

	{ sel_phone_grpconf, sel_terminal_grpconf,  7, 5, "", 
	9, 7, field[12], PHONE_NUM_LEN + 2, 'D',13,11, 
	FALSE, 0, NON_MANDATORY_CHOICE,  
	0, NUMBER_FILE, CT_COMMS_FILE, db_vals[12], &db_names[12],
	CHOICE_HLP} ,

	{ sel_phone_grpconf, sel_terminal_grpconf,  7, 5, "", 
	9, 35, field[13], PHONE_NUM_LEN + 2, 'D',14,12, 
	FALSE, 0, NON_MANDATORY_CHOICE,  
	0, NUMBER_FILE, CT_COMMS_FILE, 	db_vals[13], &db_names[13],
	CHOICE_HLP} ,

	{ sel_phone_grpconf, sel_terminal_grpconf,  7, 5, "", 
	10, 7, field[14], PHONE_NUM_LEN + 2,'D',15,13, 
	FALSE, 0, NON_MANDATORY_CHOICE,  
	0, NUMBER_FILE, CT_COMMS_FILE, db_vals[14], &db_names[14],
	CHOICE_HLP} ,

	{ sel_phone_grpconf, sel_terminal_grpconf,  7, 5, "",
	10, 35, field[15], PHONE_NUM_LEN + 2, 'D',16,14, 
	FALSE, 0, NON_MANDATORY_CHOICE,  
	0, NUMBER_FILE, CT_COMMS_FILE, db_vals[15], &db_names[15],
	CHOICE_HLP} ,

	{ sel_phone_grpconf, sel_terminal_grpconf,  7, 5, "",
	11, 7, field[16], PHONE_NUM_LEN + 2,'D',17,15, 
	FALSE, 0, NON_MANDATORY_CHOICE,  
	0, NUMBER_FILE, CT_COMMS_FILE, db_vals[16], &db_names[16],
	CHOICE_HLP} ,

	{ sel_phone_grpconf, sel_terminal_grpconf,  7, 5, "",
	11, 35, field[17], PHONE_NUM_LEN + 2, 'D',18,16, 
	FALSE, 0, NON_MANDATORY_CHOICE,  
	0, NUMBER_FILE, CT_COMMS_FILE, db_vals[17], &db_names[17],
	CHOICE_HLP} ,

	{ sel_phone_grpconf, sel_terminal_grpconf,  7, 5, "",
	12, 7, field[18], PHONE_NUM_LEN + 2,'D',19,17, 
	FALSE, 0, NON_MANDATORY_CHOICE,  
	0, NUMBER_FILE, CT_COMMS_FILE, db_vals[18], &db_names[18],
	CHOICE_HLP} ,

	{ sel_phone_grpconf, sel_terminal_grpconf, 7, 5, "",
	12, 35, field[19], PHONE_NUM_LEN + 2, 'D',20,18, 
	FALSE, 0, NON_MANDATORY_CHOICE,  
	0, NUMBER_FILE, CT_COMMS_FILE, db_vals[19], &db_names[19],
	CHOICE_HLP} ,

	{ sel_phone_grpconf, sel_terminal_grpconf,  7, 5, "",
	13, 7, field[20], PHONE_NUM_LEN + 2,'D',21,19, 
	FALSE, 0, NON_MANDATORY_CHOICE,  
	0, NUMBER_FILE, CT_COMMS_FILE, db_vals[20], &db_names[20],
	CHOICE_HLP} ,

	{ sel_phone_grpconf, sel_terminal_grpconf,  7, 5, "",
	13, 35, field[21], PHONE_NUM_LEN + 2, 'D',22,20, 
	FALSE, 0, NON_MANDATORY_CHOICE,  
	0, NUMBER_FILE, CT_COMMS_FILE, db_vals[21], &db_names[21],
	CHOICE_HLP} ,

	{ sel_phone_grpconf, sel_terminal_grpconf,  7, 5, "",
	14, 7, field[22], PHONE_NUM_LEN + 2,'D',23,21, 
	FALSE, 0, NON_MANDATORY_CHOICE,  
	0, NUMBER_FILE, CT_COMMS_FILE, db_vals[22], &db_names[22],
	CHOICE_HLP} ,

	{ sel_phone_grpconf, sel_terminal_grpconf,  7, 5, "",
	14, 35, field[23], PHONE_NUM_LEN + 2, 'D', 0,22, 
	FALSE, 0, NON_MANDATORY_CHOICE,  
	0, NUMBER_FILE, CT_COMMS_FILE, db_vals[23], &db_names[23],
	CHOICE_HLP }

};

FIELD pwflds[3] = 
{	
	{ NULL, NULL ,       2, 5, "P.I.N:",             
	2, 25, field[0], MAX_PIN_LEN,     'D', 1, 2, 
	FALSE, 0, ENTRY,  
	0, 0, 0, 0,0,
	PIN_HLP} ,

	{ sel_opgrp, NULL,  4, 3, "* Controller Group:",    
   4, 25, field[1], KEY_LABEL_LEN, 'X', 2, 0, 
	FALSE, 0, CHOICE, 
	0, OPERATOR_GROUP_FILE, 0, db_vals[1], &db_names[1],
	CHOICE_HLP} ,

	{ sel_group, NULL, 6, 3, "* Default Group Page:",
	6,25, field[2], KEY_LABEL_LEN,   'X', 0, 1, 
	FALSE, 0, CHOICE, 
	0, GROUP_FILE, 0, db_vals[2], &db_names[2],
	CHOICE_HLP }
};

FIELD ogflds[1] = 
{	
	{ NULL, NULL , 2, 5, "Controller Group Name:", 
	2, 28, field[0], KEY_LABEL_LEN, 'X', 0, 0, 
	FALSE, 0, ENTRY, 
	0, 0, 0, 0,0,
	CONTROL_GRP_HLP }
};

FIELD olflds[34] = 
{
	{ sel_opgrp_label, NULL,  1, 3, "* Controller Group:",    
	1, 31, field[0], KEY_LABEL_LEN, 'X', 1,33, 
	FALSE, 0, CHOICE, 
	0, OPERATOR_GROUP_FILE, 0, db_vals[0], &db_names[0],
	CHOICE_HLP} ,

   { get_choice, NULL,       3, 3, "* Configuration Number:",
   3, 31, field[1], 1, 'D', 2, 0,
   FALSE, 0, CHOICE,
   '1', 0, 0, config_vals, config_names,
	CONFIG_NUM_HLP} ,

	{ get_choice, NULL,       3, 38, "* Controllers in Telephony:",    
	3, 67, field[2], 2, 'D', 3, 1, 
	FALSE, 0, CHOICE, 
   'C', 0, 0, conttele_vals, conttele_names,
	NUM_TELE_HLP} ,

	{ sel_extn_group, NULL, 5, 3, "* Preferred Controller:",
	5, 31, field[3], KEY_LABEL_LEN, 'D', 4, 2, 
	FALSE, 0, CHOICE,  
	0, CT_COMMS_FILE, 0, db_vals[3], &db_names[3], 
	CHOICE_HLP} ,

	{ NULL, NULL ,       7, 5,  "Assignment Labels"     ,
	7, 31, field[4], KEY_LABEL_COLUMNS, 'X', 5, 3, 
	FALSE, 0, ENTRY , 
	0, 0, 0, 0, 0,
	LABEL_HLP} ,

	{ NULL, NULL ,      10, 34, "1",         
	8, 31, field[5], KEY_LABEL_COLUMNS, 'X', 6, 4, 
	FALSE, 0, NON_MANDATORY_ENTRY , 
	0, 0, 0, 0, 0,
	LABEL_HLP} ,

	{ NULL, NULL ,       9, 18, "",          
	9, 31, field[6], KEY_LABEL_COLUMNS, 'X', 7, 5, 
	FALSE, 0, NON_MANDATORY_ENTRY , 
	0, 0, 0, 0, 0,
	LABEL_HLP} ,

	{ NULL, NULL ,       8, 5,  "Target Description:",       
	7, 39, field[7], KEY_LABEL_COLUMNS, 'X', 8, 6, 
	FALSE, 0, NON_MANDATORY_ENTRY , 
	0, 0, 0, 0, 0,
	LABEL_HLP} ,

	{ NULL, NULL ,      10, 42, "2",         
	8, 39, field[8], KEY_LABEL_COLUMNS, 'X', 9, 7, 
	FALSE, 0, NON_MANDATORY_ENTRY , 
	0, 0, 0, 0, 0,
	LABEL_HLP} ,

	{ NULL, NULL ,      9, 26, "",          
	9, 39, field[9], KEY_LABEL_COLUMNS, 'X', 10, 8, 
	FALSE, 0, NON_MANDATORY_ENTRY , 
	0, 0, 0, 0, 0,
	LABEL_HLP} ,

	{ NULL, NULL ,      9, 5, "", 
	7, 47, field[10], KEY_LABEL_COLUMNS, 'X',11, 9, 
	FALSE, 0, NON_MANDATORY_ENTRY , 
	0, 0, 0, 0, 0,
	LABEL_HLP} ,

	{ NULL, NULL ,      10, 50, "3",         
	8, 47, field[11], KEY_LABEL_COLUMNS, 'X',12, 10, 
	FALSE, 0, NON_MANDATORY_ENTRY , 
	0, 0, 0, 0, 0,
	LABEL_HLP} ,

	{ NULL, NULL ,       9, 34, "",          
	9, 47, field[12], KEY_LABEL_COLUMNS, 'X',13,11, 
	FALSE, 0, NON_MANDATORY_ENTRY , 
	0, 0, 0, 0, 0,
	LABEL_HLP} ,

	{ NULL, NULL ,       7, 42, "",          
	7, 55, field[13], KEY_LABEL_COLUMNS, 'X',14,12, 
	FALSE, 0, NON_MANDATORY_ENTRY , 
	0, 0, 0, 0, 0,
	LABEL_HLP} ,

	{ NULL, NULL ,      10, 58, "4",         
	8, 55, field[14], KEY_LABEL_COLUMNS, 'X',15,13, 
	FALSE, 0, NON_MANDATORY_ENTRY , 
	0, 0, 0, 0, 0,
	LABEL_HLP} ,

	{ NULL, NULL ,      9, 42, "",          
	9, 55, field[15], KEY_LABEL_COLUMNS, 'X',16,14, 
	FALSE, 0, NON_MANDATORY_ENTRY , 
	0, 0, 0, 0, 0,
	LABEL_HLP} ,

	{ NULL, NULL ,       7, 50, "",          
	7, 63, field[16], KEY_LABEL_COLUMNS, 'X',17,15, 
	FALSE, 0, NON_MANDATORY_ENTRY , 
	0, 0, 0, 0, 0,
	LABEL_HLP} ,

	{ NULL, NULL ,      10, 66, "5",         
	8, 63, field[17], KEY_LABEL_COLUMNS, 'X',18,16, 
	FALSE, 0, NON_MANDATORY_ENTRY , 
	0, 0, 0, 0, 0,
	LABEL_HLP} ,

	{ NULL, NULL ,       10, 50, "",          
	9, 63, field[18], KEY_LABEL_COLUMNS, 'X',19,17, 
	FALSE, 0, NON_MANDATORY_ENTRY , 
	0, 0, 0, 0, 0,
	LABEL_HLP} ,

	{ NULL, NULL ,      13, 18, "",          
	12, 31, field[19], KEY_LABEL_COLUMNS, 'X',20,18, 
	FALSE, 0, NON_MANDATORY_ENTRY , 
	0, 0, 0, 0, 0,
	LABEL_HLP} ,

	{ NULL, NULL ,      15, 34, "6",         
	13, 31, field[20], KEY_LABEL_COLUMNS, 'X',21,19, 
	FALSE, 0, NON_MANDATORY_ENTRY , 
	0, 0, 0, 0, 0,
	LABEL_HLP} ,

	{ NULL, NULL ,      13, 18, "",          
	14, 31, field[21], KEY_LABEL_COLUMNS, 'X',22,20, 
	FALSE, 0, NON_MANDATORY_ENTRY , 
	0, 0, 0, 0, 0,
	LABEL_HLP} ,

	{ NULL, NULL ,      13, 26, "",          
	12, 39, field[22], KEY_LABEL_COLUMNS, 'X',23,21, 
	FALSE, 0, NON_MANDATORY_ENTRY , 
	0, 0, 0, 0, 0,
	LABEL_HLP} ,

	{ NULL, NULL ,      15, 42, "7",         
	13, 39, field[23], KEY_LABEL_COLUMNS, 'X',24,22, 
	FALSE, 0, NON_MANDATORY_ENTRY , 
	0, 0, 0, 0, 0,
	LABEL_HLP} ,

	{ NULL, NULL ,      13, 26, "",          
	14, 39, field[24], KEY_LABEL_COLUMNS, 'X',25,23, 
	FALSE, 0, NON_MANDATORY_ENTRY , 
	0, 0, 0, 0, 0,
	LABEL_HLP} ,

	{ NULL, NULL ,      13, 34, "",          
	12, 47, field[25], KEY_LABEL_COLUMNS, 'X',26,24, 
	FALSE, 0, NON_MANDATORY_ENTRY , 
	0, 0, 0, 0, 0,
	LABEL_HLP} ,

	{ NULL, NULL ,      15, 50, "8",         
	13, 47, field[26], KEY_LABEL_COLUMNS, 'X',27,25, 
	FALSE, 0, NON_MANDATORY_ENTRY , 
	0, 0, 0, 0, 0,
	LABEL_HLP} ,

	{ NULL, NULL ,      13, 34, "",          
	14, 47, field[27], KEY_LABEL_COLUMNS, 'X',28,26, 	
	FALSE, 0, NON_MANDATORY_ENTRY , 
	0, 0, 0, 0, 0,
	LABEL_HLP} ,

	{ NULL, NULL ,      13, 42, "",          
	12, 55, field[28], KEY_LABEL_COLUMNS, 'X',29,27, 
	FALSE, 0, NON_MANDATORY_ENTRY , 
	0, 0, 0, 0, 0,
	LABEL_HLP} ,

	{ NULL, NULL ,      15, 58, "9",         
	13, 55, field[29], KEY_LABEL_COLUMNS, 'X',30,28, 
	FALSE, 0, NON_MANDATORY_ENTRY , 
	0, 0, 0, 0, 0,
	LABEL_HLP} ,

	{ NULL, NULL ,      13, 42, "",          
	14, 55, field[30], KEY_LABEL_COLUMNS, 'X',31,29, 
	FALSE, 0, NON_MANDATORY_ENTRY , 
	0, 0, 0, 0, 0,
	LABEL_HLP} ,
										    
	{ NULL, NULL ,      13, 50, "",          
	12, 63, field[31], KEY_LABEL_COLUMNS, 'X',32,30, 
	FALSE, 0, NON_MANDATORY_ENTRY , 
	0, 0, 0, 0, 0,
	LABEL_HLP} ,

	{ NULL, NULL ,      15, 65, "10",        
	13, 63, field[32], KEY_LABEL_COLUMNS, 'X',33,31, 
	FALSE, 0, NON_MANDATORY_ENTRY , 
	0, 0, 0, 0, 0,
	LABEL_HLP} ,

	{ NULL, NULL ,      13, 50, "",          
	14, 63, field[33], KEY_LABEL_COLUMNS, 'X', 0,32, 
	FALSE, 0, NON_MANDATORY_ENTRY , 
	0, 0, 0, 0, 0,
	LABEL_HLP }
};

FIELD plflds[8] =
{	
	{ NULL, NULL ,      2, 5, "Party Line",          
	2, 26, field[0], KEY_LABEL_COLUMNS, 'X', 1,7, 
	FALSE, 0, ENTRY , 
	0, 0, 0, 0, 0,
	LABEL_HLP} ,

	{ NULL, NULL ,      3, 5, "Description:",          
	3, 26, field[1], KEY_LABEL_COLUMNS, 'X', 2,0, 
	FALSE, 0, NON_MANDATORY_ENTRY , 
	0, 0, 0, 0, 0,
	LABEL_HLP} ,

	{ NULL, NULL ,      4, 3, "",          
	4, 26, field[2], KEY_LABEL_COLUMNS, 'X', 3,1, 
	FALSE, 0, NON_MANDATORY_ENTRY , 
	0, 0, 0, 0, 0,
	LABEL_HLP} ,

	{ get_choice, NULL, 6, 3, "* Line Type:",
	6, 26, field[3], LINE_TYPE_LEN, 'X', 4, 2, 
	FALSE, 0, CHOICE , 
	S_RADIO_CALL, 0, 0, pl_line_vals, pl_line_names,
	CHOICE_HLP} ,

	{ NULL, NULL ,      8, 5, "Route Number:",          
	8, 26, field[4], 2, 'D', 5,3, 
	FALSE, 0, ENTRY , 
	0, 0, 0, 0, 0,
	ROUTE_NUM_HLP} ,

	{ NULL, NULL ,      10, 5, "Caller Party (Controller):",          
	10, 32, field[5], 5, 'N', 6,4, 
	FALSE, 0, ENTRY ,            
	0, 0, 0, 0, 0,
	CALLED_PTY_HLP} ,

	{ sel_extn, NULL,  12, 3, "* Assigned Terminal:",  
	12, 26, field[6], KEY_LABEL_LEN, 'X', 7, 5, 
	FALSE, 0, CHOICE, 
	0, CT_COMMS_FILE, 0, db_vals[1], &db_names[1],
	CHOICE_HLP} ,

	{ get_choice, NULL,  14, 3, "* Call Status:",    
	14, 26, field[7], 10, 'X', 0, 6, 
	FALSE, 0, CHOICE, 
	RING_NORMAL_ATTR, 0, 0, callstat_vals, callstat_names,
	CHOICE_HLP }

};

FIELD dpflds[4] =
{	
	{ sel_phone_internal, NULL,  2, 3, "* Selcall Line:",
	2, 33, field[0], PHONE_NUM_LEN,  'D', 1, 3, 
	FALSE, 0, CHOICE,  
	0, NUMBER_FILE, 0, db_vals[0], &db_names[0], 
	CHOICE_HLP} ,

	{ NULL, NULL , 4, 5, "Caller Party (Controller):",
	4, 33, field[1], TONE_SET_LEN, 'N', 2, 0, 
	FALSE, 0, ENTRY, 
	0, 0, 0, 0, 0,
	CALLER_PTY_HLP} ,

	{ sel_extn_selcall, NULL, 6, 3, "* Assigned Terminal:",  
	6, 33, field[2], KEY_LABEL_LEN, 'X', 3, 1, 
	FALSE, 0, CHOICE, 
	0, CT_COMMS_FILE, 0, db_vals[2], &db_names[2],
	CHOICE_HLP} ,

	{ get_choice, NULL,  8, 3, "* Status:",    
	8, 33, field[3], 10, 'X', 0, 2, 
	FALSE, 0, CHOICE, 
	RING_NORMAL_ATTR, 0, 0, callstat_vals, callstat_names,
	CHOICE_HLP} ,
};              

FIELD opflds[3] = 
{	
	{ sel_phone_outpartyln, NULL,  2, 3, "* Party Line:",
	2, 25, field[0], PHONE_NUM_LEN,  'D', 1, 2, 
	FALSE, 0, CHOICE,  
	0, NUMBER_FILE, 0, db_vals[0], &db_names[0], 
	CHOICE_HLP} ,

	{ NULL, NULL ,       4, 5, "Party Description:",             
	4, 25, field[1], KEY_LABEL_LEN - 2, 'X', 2, 0, 
	FALSE, 0, ENTRY, 
	0, 0, 0, 0,0,
	OUT_PARTY_HLP} ,

	{ NULL, NULL ,       6, 5, "Code:",         
	6, 25, field[2], TONE_SET_LEN, 'D', 0, 1, 
	FALSE, 0, ENTRY ,
	0, 0, 0, 0,0,
	CCIR_HLP }
};

FIELD ecflds[2] = 
{
	{ NULL, NULL , 2, 5, "Magazine Label:",
	2, 25, field[0], KEY_LABEL_LEN, 'X', 1, 1,
	FALSE, 0, ENTRY , 
	0, 0, 0, 0, 0,
	LABEL_HLP} ,

	{ NULL, NULL , 4, 5, "ECBU Route Number:",
	4, 25, field[1], TONE_SET_LEN, 'N', 0, 0, 
	FALSE, 0, ENTRY, 
	0, 0, 0, 0, 0,
	NUMBER_HLP} ,
};

FIELD spflds[2] = 
{
	{ NULL, NULL , 2, 5, "P.I.N.:",
	2, 25, field[0], 3, 'D', 1, 1,
	FALSE, 0, ENTRY , 
	0, 0, 0, 0, 0,
	SERV_PIN_HLP} ,

	{ NULL, NULL , 4, 5, "Technician's Name:",
	4, 25, field[1], 20, 'X', 0, 0, 
	FALSE, 0, NON_MANDATORY_ENTRY, 
	0, 0, 0, 0, 0,
	TECH_NAME_HLP} ,
};

FIELD rcflds[3] = 
{
	{ get_choice, NULL,  2, 3, "* Integrity Summary:",    
	2, 27, field[0], 3, 'X', 1, 2, 
	FALSE, 0, CHOICE, 
	'0', 0, 0, yesno_vals, yesno_names,
	CHOICE_HLP} ,

	{ get_choice, NULL,  4, 3, "* Record Summary:",    
	4, 27, field[1], 3, 'X', 2, 0, 
	FALSE, 0, CHOICE, 
	'0', 0, 0, yesno_vals, yesno_names,
	CHOICE_HLP} ,

	{ get_choice, NULL,  6, 3, "* File Summary:",    
	6, 27, field[2], 3, 'X', 0, 1, 
	FALSE, 0, CHOICE, 
	'0', 0, 0, yesno_vals, yesno_names,
	CHOICE_HLP} ,
};

FORM forms[MAX_FILES - MAX_INDEX_FILES] = 
{	 
    {  7, 36, 22,  9,  3, &gpflds[0], 0 },
  	{ 11, 65,  7,  7,  5, &pnflds[0], 0 },
  	{ 11, 50, 15,  7,  3, 0, 		0 },
  	{ 12, 54, 13,  6,  4, &bpflds[0], 0 },
  	{ 13, 60, 10,  5,  4, 0, 		0 },
  	{ 12, 60, 10,  8, 54, 0, 		0 },
  	{ 16, 54, 13,  3, 11, &hcflds[0], 0 },
  	{  9, 58, 11,  7,  6, &cgflds[0], 0 },
  	{ 17, 76,  2,  3, 10, &ccflds[0], 0 },
  	{ 17, 64,  8,  3, 24, &gcflds[0], 0 },
  	{  9, 50, 15,  7,  3, &pwflds[0], 0 },
  	{  5, 55, 12,  9,  1, &ogflds[0], 0 },
  	{ 17, 76,  2,  3, 34, &olflds[0], 0 }, 
  	{ 16, 64,  8,  3,  8, &plflds[0], 0 },
  	{ 11, 56, 12,  6,  4, &dpflds[0], 0 },
  	{  9, 50, 15,  7,  3, &opflds[0], 0 },
  	{  7, 65,  7,  9,  2, &ecflds[0], 0 },
  	{  7, 60, 10,  9,  2, &spflds[0], 0 },
  	{  9, 36, 22,  7,  3, &rcflds[0], 0 },
};
