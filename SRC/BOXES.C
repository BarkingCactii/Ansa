#pragma pack (1)
/*
	boxes.c

	draws the boxes on the screen which the user may select when
	entering data

    System  :  Train Control Command Communication System
    Author  :  Ansa Pty. Ltd. (c) 1991
    Revision:  1.0
*/

#include <string.h>

/*
#include <sfdos.h>
*/

#include "sysdef.h"
#include "filename.h"
#include "field.h"
#include "forms.h"
#include "lineass.h"
#include "actass.h"
#include "primass.h"

#include "win_hues.h"
#include "smt_idle.h"

#define ACTIVE_BOX 	1	/* box is active and selectable */
#define	DORMANT_BOX	2	/* box is inactive and non-selectable */

#define MAX_BOXES	12	/* number of boxes on screen */
#define START_ROW	22	/* screen row where boxes are to start */
#define END_ROW		23	/* screen row where boxes are to end */

#define TITLE_ROW	0	/* row where title text is to appear */
#define KEY_ROW		1	/* row where key text is to appear */
#define TITLE_COL	1	/* col where title text is to appear */
#define KEY_COL		2	/* col where key text is to appear */
#define BOX_WIDTH	5	/* width (in characters) of each box */
#define BOX_HEIGHT	2	/* height (in characters) of each box */

typedef struct			/* describes characteristics of the
				   functions boxes that appear at the
				   bottom of the screen */
{	int		start_column;
	char		*title;
	char		*key;
	unsigned	mapped_key;
	HWND		winptr;
} BOX_DESCRIPTION;

typedef struct
{	int		mapped_func;
	void	(*func)(void);
	int		mode;
} BOX_FUNCTIONS;

typedef struct     /* describes the characteristics and
				   actions to perform depending in which
				   function box is selected and which 
				   screen we are in. There is one of these
				   for every screen (see below) */
{	unsigned (*misc_func)(unsigned);
	int		break_on_func_key;
	int		second_level_handler;
	BOX_FUNCTIONS	states[MAX_BOXES];
} BOX_STATE;
 
PRIVATE BOX_DESCRIPTION boxes[MAX_BOXES] = 
{
	{ 2, "Add ", "F1",  F1,  0},
	{ 8, "Mod ", "F2",  F2,  0},
	{14, "Move", "F3",  F3,  0},
	{20, "Save", "F4",  F4,  0},
	{28, "<<  ", "F5",  F5,  0},
	{34, "<   ", "F6",  F6,  0},
	{40, ">   ", "F7",  F7,  0},
	{46, ">>  ", "F8",  F8,  0},
	{54, "Del ", "F9",  F9,  0},
	{60, "Grp ", "F10", F10, 0},
	{66, "View", "F11", F11, 0},
	{72, "Add ", "F12", F12, 0},
};

PRIVATE BOX_STATE common_entry =
{
	process_key,
	TRUE,
	FALSE,
	{{ RECADD,       0, ACTIVE_BOX},
	{ RECMOD,       0, ACTIVE_BOX},
	{ NO_OPERATION, 0, DORMANT_BOX},
	{ NO_OPERATION, 0, DORMANT_BOX},
	{ BOIND,        0, ACTIVE_BOX},
	{ PREVIOUS,     0, ACTIVE_BOX},
	{ NEXT,         0, ACTIVE_BOX},
	{ EOIND,        0, ACTIVE_BOX},
	{ RECDEL,       0, ACTIVE_BOX},
	{ NO_OPERATION, 0, DORMANT_BOX},
	{ NO_OPERATION, 0, DORMANT_BOX},
	{ NO_OPERATION, 0, DORMANT_BOX}}
/*	EXIT,         0, ACTIVE_BOX */
};

PRIVATE BOX_STATE passive_assign_entry =
{	
	process_passive_key,
	FALSE,
	FALSE,
	{{ NO_OPERATION, add_line, 	ACTIVE_BOX},
	{ NO_OPERATION, modify_line, 	ACTIVE_BOX},
	{ NO_OPERATION, move_line,	ACTIVE_BOX},
	{ NO_OPERATION, 0, 		DORMANT_BOX},
	{ NO_OPERATION, first_page, 	ACTIVE_BOX},
	{ NO_OPERATION, prev_page, 	ACTIVE_BOX},
	{ NO_OPERATION, next_page, 	ACTIVE_BOX},
	{ NO_OPERATION, last_page, 	ACTIVE_BOX},
	{ NO_OPERATION, delete_line, 	ACTIVE_BOX},
	{ NO_OPERATION, get_group, 	ACTIVE_BOX},
	{ NO_OPERATION, 0, 		DORMANT_BOX},
	{ NO_OPERATION, add_party, 		ACTIVE_BOX }}
};

PRIVATE BOX_STATE active_assign_entry =
{	
	process_active_key,
	FALSE,
	TRUE,
	{{ NO_OPERATION, 0, 		DORMANT_BOX},
	{ NO_OPERATION, 0, 		DORMANT_BOX},
	{ NO_OPERATION, 0,		DORMANT_BOX},
	{ NO_OPERATION, save_active_ass, 	ACTIVE_BOX},
	{ NO_OPERATION, 0, 		DORMANT_BOX},
	{ NO_OPERATION, 0, 		DORMANT_BOX},
	{ NO_OPERATION, 0, 		DORMANT_BOX},
	{ NO_OPERATION, 0, 		DORMANT_BOX},
	{ NO_OPERATION, 0, 		DORMANT_BOX},
	{ NO_OPERATION, select_opgroup, 	ACTIVE_BOX},
	{ NO_OPERATION, view_active,	ACTIVE_BOX},
	{ NO_OPERATION, 0, 		DORMANT_BOX }}
};

PRIVATE BOX_STATE primary_assign_entry =
{	
	process_assign_key,
	FALSE,
	FALSE,
	{{ NO_OPERATION, 0, 		DORMANT_BOX},
	{ NO_OPERATION, 0, 		DORMANT_BOX},
	{ NO_OPERATION, 0,		DORMANT_BOX},
	{ NO_OPERATION, save_assign,	ACTIVE_BOX},
	{ NO_OPERATION, previous_assign,	ACTIVE_BOX},
	{ NO_OPERATION, previous_config, 	ACTIVE_BOX},
	{ NO_OPERATION, next_config,	ACTIVE_BOX},
	{ NO_OPERATION, next_assign,	ACTIVE_BOX},
	{ NO_OPERATION, 0, 		DORMANT_BOX},
	{ NO_OPERATION, get_a_group,	ACTIVE_BOX},
	{ NO_OPERATION, 0,		DORMANT_BOX},
	{ NO_OPERATION, 0, 		DORMANT_BOX}}
};

PRIVATE	BOX_STATE *current_box_state;
			 
PRIVATE BOX_STATE *box_state_store[MAX_FILES - MAX_INDEX_FILES] = 
{	
	&common_entry,		/* GROUP_FILE */
	&common_entry,		/* NUMBER_FILE */
	&passive_assign_entry,	/* LINEASS_FILE */
	&common_entry,		/* B_PARTY_FILE */
	&active_assign_entry,	/* ACTIVE_LINE_FILE */
	&primary_assign_entry,	/* PRIMARY_LINE_FILE */
	&common_entry,		/* CT_CONFIG_FILE */
	&common_entry,		/* GEN_COMMS_FILE */
	&common_entry,		/* CT_COMMS_FILE */
	&common_entry,		/* GROUP_CONF_FILE */
	&common_entry,		/* PASSWORD_FILE */
	&common_entry,		/* OPERATOR_GROUP_FILE */
	&common_entry,		/* OPERATOR_ASSIGN_FILE */
	&common_entry,		/* PARTY_LINE_FILE */
	&common_entry,		/* D_PARTY_FILE */
	&common_entry,		/* OUT_PARTY_LINE_FILE */
	&common_entry,		/* ECBU_MAGAZINE_FILE */
	&common_entry,		/* SERVICE_PASSWORD_FILE */
	&common_entry,		/* REPORT_CONFIG_FILE */
};

/* 	Function:	set_box_state()
	Rescription:	points to the BOX_STATE which is relevent for
			this screen
	Inputs:		state - the database screen index
	Outputs:	None
	Return:		None
*/

PUBLIC void set_box_state(int state)
{
	current_box_state = box_state_store[state];
}

/* 	Function:	close_all_boxes()
	Rescription:	clears the row of function boxes from the screen
	Inputs:		None
	Outputs:	None
	Return:		TRUE - ok
*/

PUBLIC int close_all_boxes()
{
	int	i;

	for (i = 0; i < MAX_BOXES; i++)
		vdelete(boxes[i].winptr, NONE);

	return(TRUE);	
}

PUBLIC int last_hilite_box = -1;

 /* 	Function:	hilite_box()
	Rescription:	hilites a function box. This signify's that it has
			been choosen by the user
	Inputs:		box_no - index to boxes[] to which box to hilite
			attr - the attribute to hilite the box
	Outputs:	None
	Return:		TRUE - ok
*/

PUBLIC int hilite_box(int box_no, int attr)
{	
	int	j, k;

 	for (j = 0; j < BOX_HEIGHT; j++)
		for (k = 0; k < BOX_WIDTH; k++)
			vatputa(boxes[box_no].winptr, j, k, attr); 
	if (current_box_state->states[box_no].mode != DORMANT_BOX)
		last_hilite_box = box_no;
	return(TRUE);
}

/* 	Function:	unhilite_box()
	Rescription:	unhilites a function box back to its previous state
	Inputs:		None
	Outputs:	None
	Return:		TRUE - ok
*/

PUBLIC int unhilite_box(void)
{	
	int	i, j;
	int	attr;


	if (last_hilite_box != -1)
	{	if (current_box_state->states[last_hilite_box].mode == ACTIVE_BOX)
			attr = BOX_NORMAL;
		else
			attr = BOX_DORMANT;

		for (i = 0; i < BOX_HEIGHT; i++)
			for (j = 0; j < BOX_WIDTH; j++)
				vatputa(boxes[last_hilite_box].winptr,
					 i, j, attr); 
	}
	return(TRUE);
}

/* 	Function:	draw_box()
	Rescription:	places a function box on screen
	Inputs:		box_no - the index to BOX_DESCRIPTION as to
				which box to place on screen
	Outputs:	None
	Return:		TRUE - ok
*/

PRIVATE int draw_box(int box_no)
{	
	int	attr;

	if (current_box_state->states[box_no].mode == ACTIVE_BOX)
		attr = BOX_NORMAL;
	else
		attr = BOX_DORMANT;

	boxes[box_no].winptr = vcreat(BOX_HEIGHT, BOX_WIDTH, attr, YES);
	vwind(boxes[box_no].winptr, BOX_HEIGHT, BOX_WIDTH, 0, 0);
	vlocate(boxes[box_no].winptr, START_ROW, boxes[box_no].start_column);
	visible(boxes[box_no].winptr, YES, YES);
	vratputs(boxes[box_no].winptr, TITLE_ROW, TITLE_COL, attr, boxes[box_no].title); 
	vratputs(boxes[box_no].winptr, KEY_ROW, KEY_COL, attr, boxes[box_no].key); 

	return(TRUE);
}

/* 	Function:	draw_all_boxes()
	Rescription:	calls draw_box() to place all function boxes
			on screen
	Inputs:		None
	Outputs:	None
	Return:		TRUE - ok
*/

PUBLIC int draw_all_boxes()
{
	int	i;

	for (i = 0; i < MAX_BOXES; i++)
		draw_box(i);

	return(TRUE);	
}

/* 	Function:	map_key()
	Rescription:	maps the function key pressed to a database function
	Inputs:		key - the function key pressed
	Outputs:	None
	Return:		if a match is found - returns the database function
			if no match found - returns the function key pressed
*/

PUBLIC int map_key(int key)
{	
	int	i;

	for (i = 0; i < MAX_BOXES; i++)
	{
		if (boxes[i].mapped_key == (unsigned)key)
			return(current_box_state->states[i].mapped_func);
	}

	if (key == ESC)
		/* escape key acts as EXIT key */
		key = EXIT;

	return(key);
}

PRIVATE char *format_ulong ( ulong number, char *buffer )
{
    char tmp[32], *ptr;
    int l, i;

    _PROC("format_ulong");

    sprintf ( buffer, "%ld", number );

    ptr = &(tmp[31]);

    if ( ( l = strlen ( buffer ) ) <= 3 )
        return ( buffer );

    *(ptr--) = buffer[l--];
    for ( i = 0; l >= 0; i++ )
    {
        if ( ( i % 3 ) || ( i == 0 ) )
            *(ptr--) = buffer[l--];
        else
        {   *(ptr--)=',';
            i =- 1;
        }
    }
    strcpy ( buffer, ++ptr );
    return ( buffer );
}

/* 	Function:	memory_available()
	Rescription:	pops up a window showing how much system
			heap is available. activated by pressing Alt-M
	Inputs:		None
	Outputs:	None
	Return:		None
*/

PUBLIC void memory_available()
{
	HWND	win;
    char    buffer[32];

    win = vcreat ( 1, 35, WINDOW_HEADER, YES );
    vwind ( win, 1, 35, 0, 0 );
    vlocate ( win, 11, 22 );
    vframe ( win, WINDOW_HEADER, FRDOUBLE );
    vratputf ( win, 0, 3, WINDOW_HEADER, "Memory Available: %s Bytes", format_ulong ( MemorySize ( ), buffer ) );
    visible ( win, YES, YES);

    Smt_KbdGet ( );

    vdelete ( win, NONE );
}

/* 	Function:	command_dispatcher()
	Rescription:	the big daddy key handler function. Handles getting
			keystrokes, calls functions assigned to function
			keys and highlighting boxes.
	Inputs:		ch - key pressed (this is used if there is a
				higher level key handler)
	Outputs:	None
	Return:		F12 (exit key) or ch is there is a higher level
			key handler
*/

PUBLIC int command_dispatcher(unsigned ch)
{	
	int	i;
	int	break_status = FALSE;

	do 
	{
		if (current_box_state->second_level_handler)
			break_status = TRUE;
		else
            ch = Smt_KbdGet();

		/* keystroke handler for keys except function keys */
		ch = (*current_box_state->misc_func)(ch);

		for (i = 0; i < MAX_BOXES; i++)
		{
			if (ch == boxes[i].mapped_key)
			{
				if (current_box_state->states[i].mode == ACTIVE_BOX)
				{
					hilite_box(i, BOX_HILITE);
					if (current_box_state->states[i].func)
						(*current_box_state->states[i].func)();
					if (current_box_state->break_on_func_key)
/*					if (current_box_state->break_on_func_key || ch == F12)*/
						break_status = TRUE;
					break;
				}
			}

			if (ch == ESC)
				break_status = TRUE;
		}

		unhilite_box();

	} while (!break_status);

	return(ch);
}

	
