#pragma pack (1)
/*
	actass.c
	Handles all functions pertaining to primary line assignments.

    System  :  Train Control Command Communication System
    Author  :  Ansa Pty. Ltd. (c) 1991
    Revision:  1.0
*/

#include  <stdlib.h>
#include  <malloc.h>
#include  <string.h>
#include  <ibmkeys.h>

#include "sysdef.h"
#include "field.h"
#include "forms.h"
#include "filename.h"
#include "formdef.h"
#include "loaddb.h"
#include "getidx.h"
#include "actbox.h"
#include "dbio.h"
#include "boxes.h"
#include "err.h"

#include "win_hues.h"
#include "mnts_log.h"
#include "scroller.h"

#define AS_OPEN		1
#define AS_DISPLAY	2
#define AS_CLOSE	3
#define AS_END		4

PRIVATE	char 	**pptr;
PRIVATE	char 	operator_index[MAX_INDEX_LEN];
PRIVATE	char 	number_index[MAX_INDEX_LEN];
PRIVATE HWND	stat_win;
PRIVATE int	first_group_selected;

/* 	Function:	active_ass_status_win()
	Rescription:	displays the window header information
			for primary active assignments
	Inputs:		mode - AS_OPEN (create window)
			     - AS_DISPLAY (re-display contents of window)
			     - AS_CLOSE (remove from screen)
			ass - number of current assignments for this group
			grp - controller group
	Outputs:	None
	Return:		None
*/

PRIVATE void active_ass_status_win(int mode, char *ass, char *grp)
{
	switch (mode)
	{
		case (AS_OPEN):
			stat_win = vcreat(1, 70, WINDOW_NORMAL, YES);
			vwind(stat_win, 1, 70, 0, 0);
			vlocate(stat_win, 3, 5);
			vframe(stat_win, WINDOW_NORMAL, FRDOUBLE);
			visible(stat_win, YES, YES);
			vratputs(stat_win, 0, 50, WINDOW_NORMAL, "Assignments:");
			vratputs(stat_win, 0, 3, WINDOW_NORMAL, "Controller Group:");
			break;
		case (AS_DISPLAY):
			if (ass)
			{
				vratputs(stat_win, 0, 63, WINDOW_NORMAL, "      ");
				vratputs(stat_win, 0, 63, WINDOW_NORMAL, ass);
			}
			if (grp)
			{	
				vratputs(stat_win, 0, 21, WINDOW_NORMAL, "                     ");
				vratputs(stat_win, 0, 21, WINDOW_NORMAL, grp);
			}
			break;
		case (AS_CLOSE):
			vdelete(stat_win, NONE);
	}
}

/* 	Function:	number_active_assignments()
	Rescription:	determines how many extensions are deemed
			as active for the current controller group
	Inputs:		None
	Outputs:	None
	Return:		the number of active assignments
*/

PRIVATE int number_active_assignments(void)
{
	char	**tmpptr;
	int	num_assignments;

	tmpptr = pptr;
	num_assignments = 0;

	while (*tmpptr)
	{
		if (*(*tmpptr) == SELECTED_CHAR)
			num_assignments++;
		tmpptr++;
	} 	
	return(num_assignments);
}

/* 	Function:	display_assignments()
	Rescription:	high level function to re-display the primary
			assignments screen
	Inputs:		None
	Outputs:	None
	Return:		None
*/

PUBLIC void display_assignments(void)
{
	int	no_ass;
	char	ass_ch[5];

	no_ass = number_active_assignments();
	active_ass_status_win(AS_DISPLAY, itoa(no_ass, ass_ch, 10), 0);
}

/* 	Function:	display_def_assignments()
	Rescription:	prepares the primary active screen for displaying
			by displaying_assignments(). This function is called
			when a new controller group is selected
	Inputs:		None
	Outputs:	None
	Return:	     	TRUE - ok
			FALSE - unsuccessful
*/

PRIVATE int display_def_assignments(void)
{
	char		**tmpptr;
	Db_Obj		*handle;
	Index_Obj	*index;
	int		status;
	char		*fld_store[MAX_FIELDS];
	char		*key_store[MAX_FIELDS];
	char		rec_store[MAX_REC_LENGTH];
	char		label_txt[50];

	key_store[0] = operator_index;

	if ((handle = open_database(ACTIVE_LINE_FILE - 1)) == (Db_Obj *)NULL)
		return(FALSE);

	if ((index = get_index_handle(handle, key2)) == (Index_Obj *)NULL)
		return(FALSE);

	/* reset assignments */
	tmpptr = pptr;
	while (*tmpptr)
		*(*tmpptr++) = ' ';

	status = move_file_cursor(FIND, rec_store, fld_store, handle, index, 
	    key_store);

	while (status == OK && !strcmp(fld_store[AL_OG_INDEX], operator_index))
	{
		find_index(NUMBER_FILE, PN_LABEL, label_txt, fld_store[AL_PN_INDEX]);

		tmpptr = pptr;
		while (*tmpptr)
		{
			if (!strcmp(label_txt, *tmpptr + MARGIN))
			{
				*(*tmpptr) = SELECTED_CHAR;
				break;
			}
			tmpptr++;
		} 	

		status = move_file_cursor(NEXT, rec_store, fld_store, handle, index, 
			key_store);

	}
	close_database(handle);
	display_assignments();

	return(TRUE);
}

/* 	Function:	save_active_ass()
	Rescription:	saves the current screen of active assignments for
			the current controller group to disk
	Inputs:		None
	Outputs:	None
	Return:		None
*/


PUBLIC void save_active_ass(void)
{
	char		**tmpptr;
	Db_Obj		*handle;
	Index_Obj	*index;
	int		    status;
	char		*fld_store[MAX_FIELDS];
	char		*key_store[MAX_FIELDS];
	char		rec_store[MAX_REC_LENGTH];
	int		    num_assignments;
	int		    i;
	char		*index_ptr;
	char		*ind_tab_ptr;

	num_assignments = number_active_assignments();

	if (num_assignments > MAX_ASSIGNS_PER_GROUP)
	{
		show_dberr(MAX_ASSIGNS_EXCEEDED);
		return;
	}

	if ((index_ptr = (char *)halloc((long)num_assignments + 1, MAX_INDEX_LEN + 1)) == (char *)NULL)
	{
        Mnt_LogMaintError(_ID("save_active_ass"), 1 , ERROR, "malloc failure _save_active_ass");
		return;
	}
	ind_tab_ptr = index_ptr;

	key_store[0] = operator_index;
	if ((handle = open_database(ACTIVE_LINE_FILE - 1)) == (Db_Obj *)NULL)
		return;

	if ((index = get_index_handle(handle, key2)) == (Index_Obj *)NULL)
		return;

	/* remove existing records from database */
	while (move_file_cursor(FIND, rec_store, fld_store, handle, index, 
	    key_store) == OK)
		if (update_file(RECDEL, (char **)0, handle, index) == ERROR)
			return;

	tmpptr = pptr;
	fld_store[AL_OG_INDEX] = operator_index;
	fld_store[AL_PN_INDEX] = number_index;

	while (*tmpptr)
	{
		if (*(*tmpptr) == SELECTED_CHAR)
		{
			get_index(NUMBER_FILE, *tmpptr + MARGIN, number_index, CHOICE1);
			if ((status = update_file(RECADD, (char **)&fld_store, handle, index)) == ERROR)
			{
				hfree((void huge *)index_ptr);
				return;
			}

			strcpy(ind_tab_ptr, number_index);
			ind_tab_ptr += MAX_INDEX_LEN + 1;
		}
		tmpptr++;
	} 	

	close_database(handle);

	/* remove old assignments from the g_assign file */

	if ((handle = open_database(PRIMARY_LINE_FILE - 1)) == (Db_Obj *)NULL)
	{
		hfree((void huge *)index_ptr);
		return;
	}

	if ((index = get_index_handle(handle, key5)) == (Index_Obj *)NULL)
	{
		hfree((void huge *)index_ptr);
		return;
	}

	key_store[0] = operator_index;
	status = move_file_cursor(FIND, rec_store, fld_store, handle, index, key_store);
	while (status == OK) 
	{
		if (strcmp(operator_index, fld_store[PS_OG_INDEX]))
			break;

		for (i = 0, ind_tab_ptr = index_ptr; i < num_assignments; i++, ind_tab_ptr += MAX_INDEX_LEN + 1)
		{
			if (!strcmp(fld_store[PS_PN_INDEX], ind_tab_ptr))
				break;
		}

		if (i == num_assignments)
		{
			update_file(RECDEL, (char **)0, handle, index);
			status = move_file_cursor(FIND, rec_store, fld_store, handle, index, key_store);
			continue;
		}

		status = move_file_cursor(NEXT, rec_store, fld_store, handle, index, fld_store);
	} 

	close_database(handle);
	hfree((void huge *)index_ptr);
}

/* 	Function:	view_active()
	Rescription:	displays the active assignments for the current
			controller group when View is selected.
	Inputs:		None
	Outputs:	None
	Return:		None
*/

PUBLIC void view_active(void)
{
	char	**tmpptr;
	char	**line_ptr_ptr;
	int	i;
	int	no_lines;
	HWND	win;

	tmpptr = pptr;
	no_lines = 0;

	/* work out how many lines are in the scroll window */
	while (*tmpptr)
	{	
		if (*(*tmpptr) == SELECTED_CHAR)
			no_lines++;
		tmpptr++;
	} 	

	if (no_lines == 0)
	{
		show_dberr(NO_PHONE_SELECTED);
		return;
	}

	if ((line_ptr_ptr = (char **)malloc((no_lines + 1) * sizeof(char *))) == NULL)
	{
        Mnt_LogMaintError(_ID("view_active"), 1, ERROR, "Malloc failed _view_active");
		return;
	}

	/* re-parse selected entries */

	tmpptr = pptr;
	i = 0;
	while (*tmpptr)
	{	
		if (*(*tmpptr) == SELECTED_CHAR)
		{
			*(line_ptr_ptr + i) = *tmpptr + MARGIN - 1;
			i++;
		}
		tmpptr++;
	} 	
	*(line_ptr_ptr + i) = (char *)0;

	/* create window */
	win = vcreat(1, 25, WINDOW_NORMAL, YES);
	vwind(win, 1, 25, 0, 0);
	vlocate(win, 22, 27);
	vframe(win, WINDOW_NORMAL, FRSINGLE);
	visible(win, YES, YES);

	vratputs(win, 0, 4, WINDOW_NORMAL, "Press ESC to exit");

	Smt_Scroller((char *)0, 9, 5, 11, 70, WINDOW_NORMAL, WINDOW_HILITE, line_ptr_ptr, 3, 0);

	vdelete(win, NONE);

	free(line_ptr_ptr);
	
}

/* 	Function:	process_active_key()
	Rescription:	handles miscellaneous keystrokes for primary
			active assignments not handled by the
			function box handler command_dispatcher()
	Inputs:		ch - the key pressed
	Outputs:	None
	Return:		ch - the key pressed
*/

PUBLIC unsigned process_active_key(unsigned ch)
{
	if (ch == SELECT_CHAR)
		display_assignments();

/*	if (ch == F12) */
	if (ch == ESC)
		return(FALSE);

	return(ch);
}

PRIVATE int opgroup_status = FALSE;

/* 	Function:	select_opgroup()
	Rescription:	pops up a window allowing the user to select a new
			controller group. The primary active assignments
			screen is redrawn
	Inputs:		None
	Outputs:	None
	Return:		TRUE - ok
			FALSE -	no groups to select
			ERROR - user abort
*/

PUBLIC void select_opgroup(void)
{
	int	choice = ERROR;
	char	*ptr;
	char	**ptrs;
	int	no_recs;

	no_recs = load_file(OPERATOR_GROUP_FILE, &ptr, &ptrs, -CT_CONFIG_FILE); /* was NO_ARGS */
	if (!no_recs)
    {
        show_dberr(NO_GROUP_RECORDS);
        opgroup_status = FALSE;
        return;
    }

	choice = Smt_Scroller((char *)0, 5, 20, AUTO, AUTO, POPUP_NORMAL, POPUP_HILITE, ptrs, 1, 0);

	if (choice != ERROR)
	{
		active_ass_status_win(AS_DISPLAY, 0, *(ptrs + choice) + MARGIN);

		get_index(OPERATOR_GROUP_FILE, *(ptrs + choice) + MARGIN, operator_index, CHOICE1);

		/* display existing assignments */
		no_recs = load_file(NUMBER_FILE, &ptr, &pptr, ACTIVE_LINE_FILE);

        if (!no_recs)
        {
            show_dberr(NO_GROUP_RECORDS);
            opgroup_status = FALSE;
        }
        else
        {
            opgroup_status = TRUE;
            display_wait();
    		display_def_assignments();
            drop_wait();
	    	first_group_selected = FALSE;
        }
	}
	else
	{
		if (!first_group_selected)
		{
			no_recs = load_file(NUMBER_FILE, &ptr, &pptr, ACTIVE_LINE_FILE);
            display_wait();
			display_def_assignments();
            drop_wait();
            opgroup_status = TRUE;
		}
        else
            opgroup_status = FALSE;
	}
}

/* 	Function:	active_assignments()
	Rescription:	high level function for handling display
			of primary active assignments. Called directly
			from the menu
	Inputs:		None
	Outputs:	None
	Return:		TRUE - ok
*/

PUBLIC int active_assignments(void)
{
	SCROLL_PARAMS   	scroll_params;

	draw_all_boxes();

	active_ass_status_win(AS_OPEN, NO_ARGS, NO_ARGS);

/*	no_recs =  load_file(NUMBER_FILE, &ptr, &pptr, ACTIVE_LINE_FILE); */

/*	if (no_recs)
	{
*/
		first_group_selected = TRUE;

		select_opgroup();

/*		if (opgroup_status == FALSE)

		{
			show_dberr(NO_GROUP_RECORDS);
			no_recs = 0;
		}

		if (opgroup_status == ERROR)
			no_recs = 0;
	}
	else
		show_dberr(NO_PHONE_RECORDS);

*/
	if (opgroup_status == TRUE)
	{
		scroll_params.title = (char *)0;
		scroll_params.top = 7;
		scroll_params.left = 5;
		scroll_params.height = 13;
		scroll_params.width = 70;
		scroll_params.margin = 0;
		scroll_params.attribute = POPUP_NORMAL;
		scroll_params.selection_attribute = POPUP_HILITE;
		scroll_params.hilite_attribute = MENU_SELECT;
		scroll_params.header_attribute = WINDOW_NORMAL;
		scroll_params.text = pptr;
        scroll_params.columns = 1;
		scroll_params.FKeyFunction = command_dispatcher;
		scroll_params.selection_index = 0;
		scroll_params.non_selectable_column = 0;

		Smt_SelectScroller(&scroll_params);
	}

	active_ass_status_win(AS_CLOSE, NO_ARGS, NO_ARGS);

	close_all_boxes();

	return(TRUE);
}
