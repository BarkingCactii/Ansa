#pragma pack (1)
/*
	linebox.c
	Handles the screen display (the line boxes) for passive line
	assignments.

    System  :  Train Control Command Communication System
    Author  :  Ansa Pty. Ltd. (c) 1991
    Revision:  1.0
*/

#include <stdlib.h>
#include <string.h>

/*
#include <ibmkeys.h>
*/

#include "sysdef.h"
#include "lineass.h"
#include "title.h"

#include "win_hues.h"
#include "sdb_defn.h"

#define START_ROW	2
#define START_COL	0 
#define BOX_GAP		1
#define BOX_WIDTH	7	/* width (in characters) of each box */
#define BOX_HEIGHT	3	/* height (in characters) of each box */


PRIVATE HWND	box_wptr[NUM_LINE_KEY_BUTTONS];

/* 	Function:	display_label()
	Rescription:	displays a label within a line key
	Inputs:		line - the box number to put the text into
			text - the label to display
	Outputs:	None
	Return:		None
*/

PUBLIC void display_label(int line, char *text)
{
	int	i;
	int	attr;
	int	all_spaces = TRUE;

	for (i = 0; i < 18; i++)
		if (*(text + i) != ' ')
			all_spaces = FALSE;

	if (all_spaces)
		attr = BOX_UNUSED;
	else
		attr = BOX_NORMAL;

	for (i = 0; i < BOX_HEIGHT; i++)
		vratputc(box_wptr[line], i, 0, attr, ' ');

	for (i = 0; i < 18; i++)
	 	vratputc(box_wptr[line], i / 6, (i % 6) + 1, attr, *(text + i)); 

}

/* 	Function:	draw_box()
	Rescription:	creates and displays a single line key box
	Inputs:		box_no - the box to draw
	Outputs:	None
	Return:		TRUE
*/

PRIVATE int draw_box(int box_no)
{	
	int	row;
	int	col;

	box_wptr[box_no] = vcreat(BOX_HEIGHT, BOX_WIDTH, BOX_UNUSED, YES);
	vwind(box_wptr[box_no], BOX_HEIGHT, BOX_WIDTH, 0, 0);

	row = START_ROW + ((box_no / BOXES_PER_ROW) * (BOX_HEIGHT + BOX_GAP));
	col = START_COL	+ ((box_no % BOXES_PER_ROW) * (BOX_WIDTH + BOX_GAP));

	vlocate(box_wptr[box_no], row, col);
	visible(box_wptr[box_no], YES, YES);

	return(TRUE);
}

/* 	Function:	draw_lineass_boxes()
	Rescription:	displays all the line key boxes on screen
	Inputs:		None
	Outputs:	None
	Return:		None
*/
	
PUBLIC int draw_lineass_boxes()
{	
	int	i;

	for (i = 0; i < NUM_LINE_KEY_BUTTONS; i++)
		draw_box(i);

	return(TRUE);	
}

/* 	Function:	close_all_lineass_boxes()
	Rescription:	removes all the line key boxes from the screen
	Inputs:		None
	Outputs:	None
	Return:		TRUE
*/

PUBLIC int close_all_lineass_boxes()
{	
	int	i;

	for (i = 0; i < NUM_LINE_KEY_BUTTONS; i++)
		vdelete(box_wptr[i], NONE);

	return(TRUE);	
}

PRIVATE int last_hilite_box = -1;

/* 	Function:	hilite_lineass_box()
	Rescription:	displays a line key box in the specified attribute
	Inputs:		box_no - the box number 
			att - the attribute
	Outputs:	None
	Return:		TRUE
*/

PUBLIC int hilite_lineass_box(int box_no, int att)
{	
	int	j, k;

 	for (j = 0; j < BOX_HEIGHT; j++)
		for (k = 0; k < BOX_WIDTH; k++)
			vatputa(box_wptr[box_no], j, k, att); 
	last_hilite_box = box_no;
	return(TRUE);
}

/* 	Function:	unhilite_lineass_box
	Rescription:	draws the last box hilited by hilite_lineass_box()
			in the specified attribute. This is used typically
			to unhighlight boxes which were previously the
			current cursor and thus highlighted.
	Inputs:		attr - the new attribute
	Outputs:	None
	Return:		TRUE
*/

PUBLIC int unhilite_lineass_box(int attr)
{	
	int	i, j;

	if (last_hilite_box != -1)
	{	/* draw the shadow */
		for (i = 0; i < BOX_HEIGHT; i++)
			for (j = 0; j < BOX_WIDTH; j++)
				vatputa(box_wptr[last_hilite_box],
					 i, j, attr); 
	}
	return(TRUE);
}


