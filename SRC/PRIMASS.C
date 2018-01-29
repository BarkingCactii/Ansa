#pragma pack (1)

/*
	primass.c

	handles the primary/secondary active assignments

    System  :  Train Control Command Communication System
    Author  :  Ansa Pty. Ltd. (c) 1991
    Revision:  1.0
*/

#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <ctype.h>
#include <ibmkeys.h>

#include "sysdef.h"
#include "field.h"
#include "forms.h"
#include "filename.h"
#include "formdef.h"
#include "loaddb.h"
#include "getidx.h"
#include "primbox.h"
#include "dbio.h"
#include "err.h"
#include "boxes.h"
#include "title.h"

#include "mnts_log.h"
#include "win_util.h"
#include "win_hues.h"
#include "scroller.h"
#include "sdb_defn.h"

PUBLIC int no_of_operators(char *);
PRIVATE int select_a_group(void);
PRIVATE int get_phone_idx(int *);
PRIVATE int get_active_phone_idx(char *);

PRIVATE char    phone_labels[13][KEY_LABEL_LEN + 1];
PRIVATE char	phone_indexes[13][MAX_INDEX_LEN + 1];
PRIVATE char	user_select_table[MAX_ASSIGNS_PER_GROUP + 1][50];

/*
	File Layout

	HEADER	crc
		count	n

	long[n-1]
	|
	-----> (n-n)		int	no configurations	x

				(configuration 1)
				DETAILS[x-x][operators on line]

				(configuration 2)
				DETAILS[x-x][operators on line]

			       
	-----> (n-n+1)		etc


	where DETAILS is 	key_label
				count	y
				indexes[y]
				
			
*/

PRIVATE int changed_flag = FALSE;
PRIVATE char	*vert_lines =     "                    ³          ³          ³          ³          ³          ";
PRIVATE char	*vert_cfg_line1 = "                    ³Active Terminal Configuration Number:                 ";
PRIVATE char	*vert_cfg_line2 = "                    ³     1    ³     2    ³     3    ³     4    ³     5    ";

PRIVATE int	assign_num;
PRIVATE int	config_num;
PRIVATE int	current_row;
PRIVATE int	current_col;
PRIVATE int 	previous_col;
PRIVATE int	num_rows;
PRIVATE int	num_cols;
PRIVATE	int	terminals_in_group;
PRIVATE	int	phone_label_ptr;
PRIVATE int	current_page;
PRIVATE int	max_pages;

PRIVATE	HWND	main_win;
PRIVATE	HWND	win;

PRIVATE char	group_name[KEY_LABEL_LEN + 1];
PRIVATE char 	group_index[MAX_INDEX_LEN + 1];
PRIVATE int	refresh_assignments(void);
PRIVATE int LookForMatch(unsigned text);

/* 	Function:	first_empty_config()
	Rescription:	tests whether there are any active assignments
			in the database for the current configuration,
			current controllers in telephony and current
			controller group.
	Inputs:		None
	Outputs:	None
	Return:		TRUE - there are active assignments
			FALSE - no active assignments for the current config
*/

PRIVATE	int first_empty_config(void)
{
	Db_Obj		*handle;
	Index_Obj	*index;
	int		status;
	char		*fld_store[MAX_FIELDS];
	char		*key_store[MAX_FIELDS];
	char		rec_store[MAX_REC_LENGTH];
	char		assign_num_str[5];
	char		config_num_str[5];

	key_store[0] = group_index;
	itoa(assign_num, assign_num_str, 10);
	key_store[1] = assign_num_str;
	itoa(config_num, config_num_str, 10);
	key_store[2] = config_num_str;

	if ((handle = open_database(PRIMARY_LINE_FILE - 1)) == (Db_Obj *)NULL)
		return(FALSE);

	if ((index = get_index_handle(handle, key2)) == (Index_Obj *)NULL)
		return(FALSE);

	status = move_file_cursor(FIND, rec_store, fld_store, handle, index, key_store);

	close_database(handle);

	if (status == OK)
		return(TRUE);

	return(FALSE);
}

/* 	Function:	show_label()
	Rescription:	displays the controller label to match
			the controller at the current cursor position
	Inputs:		None
	Outputs:	None
	Return:		None
*/

PRIVATE int show_label(void)
{
	Db_Obj		*handle;
	Index_Obj	*index;
	int		status;
	char		*fld_store[MAX_FIELDS];
	char		*key_store[MAX_FIELDS];
	char		rec_store[MAX_REC_LENGTH];
	char		assign_num_str[5];
	char		config_num_str[5];

	key_store[0] = group_index;
	sprintf(assign_num_str, "%c", (char)assign_num + 'A');
	key_store[1] = assign_num_str;
	sprintf(config_num_str, "%d", config_num);
	key_store[2] = config_num_str;

	if ((handle = open_database(OPERATOR_ASSIGN_FILE - 1)) == (Db_Obj *)NULL)
		return(FALSE);

	if ((index = get_index_handle(handle, key1)) == (Index_Obj *)NULL)
		return(FALSE);

	status = move_file_cursor(FIND, rec_store, fld_store, handle, index, key_store);

	if (status == OK)
	{
		vratputs(win, 0, 13, WINDOW_HILITE3, fld_store[OL_LABEL + (current_col * 3)]);
		vratputs(win, 1, 13, WINDOW_HILITE3, fld_store[OL_LABEL + (current_col * 3) + 1]);
		vratputs(win, 2, 13, WINDOW_HILITE3, fld_store[OL_LABEL + (current_col * 3) + 2]);
		status = TRUE;
	}
	else
	{
		vratputs(win, 0, 13, WINDOW_HILITE3, "NO    ");
		vratputs(win, 1, 13, WINDOW_HILITE3, "LABEL ");
		vratputs(win, 2, 13, WINDOW_HILITE3, "EXISTS");
		status = FALSE;
	}
	close_database(handle);

	return(status);
}

/* 	Function:	read_assignments()
	Rescription:	reads the first page of active assignments 
			for the current:
				controller group
				controllers in telephony
				config
	Inputs:		None
	Outputs:	None
	Return:		None
*/

PRIVATE	void read_assignments(void)
{
	Db_Obj		*handle;
	Index_Obj	*index;
	int	    	status;
	char		*fld_store[MAX_FIELDS];
	char		*key_store[MAX_FIELDS];
	char		rec_store[MAX_REC_LENGTH];
	char		assign_num_str[5];
	char		config_num_str[5];
	int		    i;
	int		    tmp;

	key_store[0] = group_index;
	itoa(assign_num, assign_num_str, 10);
	key_store[1] = assign_num_str;
	itoa(config_num, config_num_str, 10);
	key_store[2] = config_num_str;

	if ((handle = open_database(PRIMARY_LINE_FILE - 1)) == (Db_Obj *)NULL)
		return;

	if ((index = get_index_handle(handle, key2)) == (Index_Obj *)NULL)
		return;

	status = move_file_cursor(FIND, rec_store, fld_store, handle, index, key_store);
	while (status == OK)
	{
		/* lets check for a match */
		for (i = 0; i < num_rows; i++)
		{
			if (!strcmp(fld_store[PS_PN_INDEX], phone_indexes[i]))
			{
				tmp = atoi(fld_store[PS_OPERATOR_NUM]);
				user_select_table[i][tmp-1] = 'X';
				break;
			}
		}
		status = move_file_cursor(NEXT, rec_store, fld_store, handle, index, NO_ARGS);

		if (strcmp(fld_store[PS_OG_INDEX], key_store[0]))
			break;
		if (strcmp(fld_store[PS_ASSIGN_NUM], key_store[1]))
			break;
		if (strcmp(fld_store[PS_CONFIG_NUM], key_store[2]))
			break;
	}

	close_database(handle);
}

/* 	Function:	save_assign()
	Rescription:	saves the current page active assignments 
			for the current:
				controller group
				controllers in telephony
				config
	Inputs:		None
	Outputs:	None
	Return:		None
*/

PUBLIC void save_assign(void)
{
	Db_Obj		*handle;
	Index_Obj	*index;
	int		    status;
	char		*fld_store[MAX_FIELDS];
	char		*key_store[MAX_FIELDS];
	char		rec_store[MAX_REC_LENGTH];
	char		assign_num_str[5];
	char		operator_num_str[5];
	char		config_num_str[5];
	char		operator_num;
	int		    i, j;

	operator_num = 1;

	key_store[0] = group_index;
	itoa(assign_num, assign_num_str, 10);
	key_store[1] = assign_num_str;
	itoa(operator_num, operator_num_str, 10);
	key_store[2] = operator_num_str;
	itoa(config_num, config_num_str, 10);
	key_store[3] = config_num_str;

	if ((handle = open_database(PRIMARY_LINE_FILE - 1)) == (Db_Obj *)NULL)
		return;

	if ((index = get_index_handle(handle, key1)) == (Index_Obj *)NULL)
		return;

    display_wait();

	/* remove existing records from database */
	for (i = operator_num; i <= assign_num; i++)
	{	
		for (j = 0; j < phone_label_ptr; j++)
		{
			/* set up key */
			itoa(i, operator_num_str, 10);
			key_store[2] = operator_num_str;
			key_store[4] = phone_indexes[j];
	
			status = move_file_cursor(FIND, rec_store, fld_store, handle, index, key_store);
			if (status == OK)
				update_file(RECDEL, (char **)0, handle, index);
		}
	}

	/* now add the entire page to the database */
	fld_store[PS_OG_INDEX] = group_index;
	fld_store[PS_ASSIGN_NUM] = assign_num_str;
	fld_store[PS_OPERATOR_NUM] = operator_num_str;
	fld_store[PS_CONFIG_NUM] = config_num_str;

	for (i = operator_num; i <= assign_num; i++)
	{
		itoa(i, operator_num_str, 10);
		for (j = 0; j < phone_label_ptr; j++)
		{
			fld_store[PS_PN_INDEX] = phone_indexes[j];
			if (user_select_table[j][i - 1] == 'X')
				status = update_file(RECADD, (char **)&fld_store, handle, index);
		}
	}

	close_database(handle);

    drop_wait();
}


/* CHANGES FROM HERE */

typedef struct {
	char	label[KEY_LABEL_LEN + 1];
	char	index[MAX_INDEX_LEN + 1];
	int	active_for_current_group;
} PHONE_LABEL;

PRIVATE PHONE_LABEL *phone_info = 0;
PRIVATE	int	num_phone_recs;

PRIVATE int load_sorted_phone_info(void)
{
	Db_Obj		*handle;
	Index_Obj	*index;
	char	     	rec_store[MAX_REC_LENGTH];
	char	     	*fld_store[MAX_FIELDS];
	int		status;
	PHONE_LABEL 	*current_phone_info;

    display_wait();

	/* open database and related index file */
	if ((handle = open_database(NUMBER_FILE - 1)) == (Db_Obj *)NULL)
    {
        drop_wait();
		return(FALSE);
    }
	if ((index = get_index_handle(handle, key2)) == (Index_Obj *)NULL)
    {
        drop_wait();
		return(FALSE);
    }

 	num_phone_recs = record_count(handle);

	if ((phone_info = (PHONE_LABEL *)halloc((long)(num_phone_recs + 1), sizeof(PHONE_LABEL))) == NULL)
	{
        Mnt_LogMaintError(_ID("load_sorted_phone_info"), 1, ERROR, "malloc failure _load_sorted_phone_info");
        drop_wait();
		return(FALSE);
	}
	current_phone_info = phone_info;

	status = move_file_cursor(BOIND, rec_store, fld_store, handle, index, NO_ARGS);

	while (status == OK)
	{
		strcpy(current_phone_info->label, fld_store[PN_LABEL]);
		strcat(current_phone_info->label, fld_store[PN_LABEL + 1]);
		strcat(current_phone_info->label, fld_store[PN_LABEL + 2]);
		strcpy(current_phone_info->index, fld_store[PN_INDEX]);
		current_phone_info->active_for_current_group = FALSE;
		current_phone_info++;

		status = move_file_cursor(NEXT, rec_store, fld_store, handle, index, NO_ARGS);
	}

	close_database(handle);

    drop_wait();

	return(TRUE);
}


/* 	Function:	get_a_group()
	Rescription:	high level function to allow user to select a new
			controller group, do some validation and refresh
			the screen. This function is mapped to the GRP (F10)
			function box.
	Inputs:		None
	Outputs:	None
	Return:		TRUE - ok
			FALSE -	no groups to select or user abort
*/

PRIVATE	int	choice = ERROR;
PRIVATE	char	*ptr;
PRIVATE	char	**ptrs;
PRIVATE	char	tmp_name[KEY_LABEL_LEN + 1];
PRIVATE	char 	tmp_index[MAX_INDEX_LEN + 1];
PRIVATE int     get_a_group_status = FALSE;

PUBLIC void get_a_group(void)
{
	int	status;

	strcpy(tmp_name, group_name);
	strcpy(tmp_index, group_index);

	do
	{
		status = select_a_group();

		if (status == ERROR)
		{
			strcpy(group_name, tmp_name);
			strcpy(group_index, tmp_index);
			show_label();
            get_a_group_status = FALSE;
			return;
		}

		if (status == FALSE)
		{
			show_dberr(NO_GROUP_RECORDS);
			continue;
		}

		/* save group info */
		get_index(OPERATOR_GROUP_FILE, *(ptrs + choice) + MARGIN, group_index, CHOICE1);

		/* get no of terminals for group */
		terminals_in_group = no_of_operators(group_index);

		if (!terminals_in_group)
		{	/* no terminals for selected group */
			show_dberr(NO_TERMINALS);
			continue;
		}

		/* no need to make assignments with 1 terminal in group */
		if (terminals_in_group == 1)
		{
			show_dberr(ONE_TERMINAL);
			continue;
		}

		if (!show_label())
		{
		 	show_dberr(NO_LABEL_DEFINED);
			continue;
		}

		assign_num = 2;
		config_num = 1;
		current_page = 0;

		/* re-display */
		display_controller_group(*(ptrs + choice) + MARGIN);
		get_index(OPERATOR_GROUP_FILE, *(ptrs + choice) + MARGIN, group_index, CHOICE1);
		strcpy(group_name, *(ptrs + choice) + MARGIN);
		show_label();

		if (!get_active_phone_idx(group_index))
			continue;

		if (refresh_assignments())
        {
            get_a_group_status = TRUE;
			return;
	    }

	} while (status != ERROR);

    get_a_group_status = TRUE;
}

/* 	Function:	primtsec_title_win()
	Rescription:	creates and displays the title window for
			secondary active assignments
	Inputs:		None
	Outputs:	None
	Return:		None
*/

PRIVATE void primsec_title_win(void)
{
	int	i, j;	

	if (!win)
	{
		win = vcreat(3, 75, WINDOW_NORMAL, YES);
		vwind(win, 3, 75, 0, 0);
		vlocate(win, 3, 2);
		vframe(win, WINDOW_NORMAL, FRDOUBLE);
		visible(win, YES, YES);
	}

	vratputs(win, 0, 0, WINDOW_NORMAL, vert_cfg_line1);
	vratputs(win, 1, 0, WINDOW_NORMAL, vert_cfg_line2);
	vratputs(win, 2, 0, WINDOW_NORMAL, vert_lines);
	
	vratputs(win, 1, 1, WINDOW_NORMAL, "Target");
	vratputs(win, 2, 1, WINDOW_NORMAL, "Desc.");

	for (i = 0; i < assign_num; i++)
		for (j = 21; j < (5 * 11) + 21; j+=11)
			vratputc(win, 2, j + i, WINDOW_NORMAL, BKGROUND_CHAR); 
}

/* 	Function:	primsec_main_win()
	Rescription:	creates and displays the main window for 
			secondary active assignments. ie draws the
			vertical lines etc.
	Inputs:		max_phones - the number of phone extensions to
				     be displayed for this page
	Outputs:	None
	Return:		None
*/

PRIVATE void primsec_main_win(int max_phones)
{
	int		i;	

	if (!main_win)
	{
		main_win = vcreat(13, 75, WINDOW_NORMAL, YES);
		vwind(main_win, 13, 75, 0, 0);
		vlocate(main_win, 7, 2);
		vframe(main_win, WINDOW_NORMAL, FRDOUBLE);
		visible(main_win, YES, YES);
	}
	for (i = 0; i < 13; i++)
		vratputs(main_win, i, 0, WINDOW_NORMAL, vert_lines);

	for (i = 0; i < 13 && i < max_phones; i++)
        vratputs(main_win, i, 1, WINDOW_NORMAL, phone_labels[i]);
}

/* 	Function:	primsec_assignments_main()
	Rescription:	high level function. Gets a controller group
			passes control to command_dispatcher() and
			closes the database on exit. Called once only
			entry.
	Inputs:		None
	Outputs:	None
	Return:		TRUE - ok
			FALSE - error
*/

PUBLIC int primsec_assignments_main(void)
{
/*	SCREEN_DETS	scn_info; */
    unsigned    ch = (unsigned)NULL;

	win = main_win = (HWND)0;

	get_a_group();
    if (!get_a_group_status)
		return(FALSE);

	command_dispatcher(ch);

/*	close_database(scn_info.handle); */

	vdelete(main_win, NONE);
	vdelete(win, NONE);

	win = main_win = (HWND)0;
	return(TRUE);
}

/* 	Function:	primsec_assignments()
	Rescription:	highest level function for secondary active 
			assignments. Called from the menu system
	Inputs:		None
	Outputs:	None
	Return:		TRUE
*/

PUBLIC int primsec_assignments(void)
{
	draw_all_boxes();

	load_sorted_phone_info();

	assign_num = 2;
	config_num = 1;
	current_page = 0;
	
	group_index[0] = group_name[0] = 0;

	primsec_assignments_main();

	if (phone_info)
		hfree(phone_info);

	close_all_boxes();

	return(TRUE);
}

/* 	Function:	select_a_group()
	Rescription:	allows the user to select a group on which
			primary/secondary assignments will be done
	Inputs:		None
	Outputs:	group_idx (index to group selected)
			group_name (name of the group selected)
	Return:		TRUE or ERROR
*/

PRIVATE int select_a_group(void)
{
	int	no_recs;

	no_recs = load_file(OPERATOR_GROUP_FILE, &ptr, &ptrs, NO_ARGS);

	if (no_recs)
		choice = Smt_Scroller((char *)0, 5, 20, AUTO, AUTO, POPUP_NORMAL, POPUP_HILITE, ptrs, 1, 0);
	
	if (!no_recs)
		return(FALSE);

	if (choice == ERROR)
		return(ERROR);

	return(TRUE);
}

/* 	Function:	no_of_operators()
	Rescription:	determines how many terminals are in the
			current controller group
	Inputs:		group_idx - index of the current controller group
	Outputs:	None
	Return:		FALSE - error
			>0 - no of terminals in group
*/

PUBLIC int no_of_operators(char *group_idx)
{
	Db_Obj			*handle;
	Index_Obj		*index;
	char			rec_store[MAX_REC_LENGTH];
	char			**key_ptr = (char **)&rec_store[0];
	char			*fld_store[MAX_FIELDS];
	int			    status;
	int			    no_terminals = 0;
	int			    i;

	no_terminals = 0;

	if ((handle = open_database(CT_CONFIG_FILE - 1)) == (Db_Obj *)NULL)
		return(FALSE);

	if ((index = get_index_handle(handle, key1)) == (Index_Obj *)NULL)
		return(FALSE);

	strcpy(rec_store, group_idx);
	status = move_file_cursor(FIND, rec_store, fld_store, 
	     handle, index, (char **)&key_ptr);

	if (status != OK)
		return(FALSE);

	for (i = HC_START_EXT; i <= HC_END_EXT; i++)
		if (*fld_store[i])
			no_terminals++;
	close_database(handle);

	return(no_terminals);
}

/* 	Function:	get_phone_idx(void)
    Description:    copies the phone labels and indexes
			from the table of phone numbers to the
			table for the current page
	Inputs:		group_idx (index to a operator group)
	Outputs:	None
	Return:		None
*/

PRIVATE int get_phone_idx(int *idx_ptr)
{
 	int	   	recs_read = 0;
	PHONE_LABEL	*current_phone_info;
	int	   	i;

	*idx_ptr = 0;
	for (current_phone_info = phone_info, i = 0; i < num_phone_recs; current_phone_info++, i++)
	{
		if (current_phone_info->active_for_current_group)
		{
			recs_read++;
			if (recs_read > current_page * 13 && recs_read < (current_page * 13) + 14)
			{
				strcpy(phone_labels[*idx_ptr], current_phone_info->label);
				strcpy(phone_indexes[*idx_ptr], current_phone_info->index);
				(*idx_ptr)++;
		       	}
		}
	}

	return(TRUE);
}

/* 	Function:	get_active_phone_idx()
	Rescription:	Read the active assignment file for all
			phone numbers related to a certain group
			and marks the matching entry in the table
			as being active for this group
	Inputs:		group_idx (index to a operator group)
	Outputs:	None
	Return:		None
*/

PRIVATE int get_active_phone_idx(char *group_idx)
{
	char			rec_store[MAX_REC_LENGTH];
	char			*fld_store[MAX_FIELDS];
	char			**index_ptr;
	int			status;
	Db_Obj			*handle;
	Index_Obj		*index;
 	int			recs_read = 0;
  	int			i;
	PHONE_LABEL 		*current_phone_info;

	index_ptr = (char **)group_idx;

	/* open database and related index file */
	if ((handle = open_database(ACTIVE_LINE_FILE - 1)) == (Db_Obj *)NULL)
		return(FALSE);
	if ((index = get_index_handle(handle, key2)) == (Index_Obj *)NULL)
		return(FALSE);

	/* get first record for group */
	status = move_file_cursor(FIND, rec_store, fld_store, 
		handle, index, (char **)&index_ptr);

	if (status != OK)
	{
		/* no active assignments for this group */
		show_dberr(NO_ACTIVE_ASSIGNMENTS);
		return(FALSE);
	}

    display_wait();

	/* reset table */
	for (current_phone_info = phone_info, i = 0; i < num_phone_recs; current_phone_info++, i++)
		current_phone_info->active_for_current_group = FALSE;

	/* keep reading records until end of group */
	while (status == OK)
	{
		/* get the detailed info using the phone index */
		recs_read++;

		for (current_phone_info = phone_info, i = 0; 
			i < num_phone_recs; 
			current_phone_info++, i++)
		{
			if (!strcmp(fld_store[AL_PN_INDEX], current_phone_info->index))
			{
				current_phone_info->active_for_current_group = TRUE;
				break;
			}
		}

		/* get next record */
		status = move_file_cursor(NEXT, rec_store, 
			fld_store, handle, index, NO_ARGS);

		if (strcmp(fld_store[AL_OG_INDEX], group_idx))
			break;
	}

	max_pages = (recs_read - 1) / 13;

	close_database(handle);

    drop_wait();

	return(TRUE);
}

/* 	Function:	display_assignments()
	Rescription:	places an 'X' on screen if there is an active
			assignment for the current controller for the
			current phone extension
	Inputs:		None
	Outputs:	None
	Return:		None
*/

PRIVATE void display_assignments(void)
{
	int	i, j;

	for (i = 0; i < num_rows; i++)
	{
		for (j = 0; j < num_cols; j++)
			if (user_select_table[i][j] == 'X')
				vratputc(main_win, i, j + 21 + ((config_num - 1) * 11), WINDOW_NORMAL, 'X'); 
	}
}

/* 	Function:	refresh_assignments()
	Rescription:	high level function to handle refreshing the
			display. This occurs when the user Pages up
			or down, select a new controller group, selects
			a new number of controllers in telephony or a
			new configuration
	Inputs:		None
	Outputs:	None
	Return:		TRUE - ok
			FALSE - error
*/

PRIVATE int refresh_assignments(void)
{
	int	i;
	int	save_config;
	char	controllers_in_telephony[5];

    display_wait();

	num_cols = assign_num;
	previous_col = current_col = current_row = 0;

	if (!get_phone_idx(&phone_label_ptr))
    {
        drop_wait();
		return(FALSE);
    }

	primsec_title_win();

    num_rows = phone_label_ptr;

	primsec_main_win(phone_label_ptr);

	save_config = config_num;
	for (config_num = 1; config_num <= 5; config_num++)
	{
		memset(user_select_table, ' ', sizeof(user_select_table));
		read_assignments();
		display_assignments();
	}
	config_num = save_config;

	/* now re-setup the current config # */

 	memset(user_select_table, ' ', sizeof(user_select_table));
 	read_assignments();
 	display_assignments();

	vatputa(main_win, current_row, current_col + 21 + ((config_num - 1) * 11), BOX_VIEW_MODE);
	for (i = 1; i <= 5; i++)
		vatputa(win, 2, current_col + 21 + ((i - 1) * 11), BOX_VIEW_MODE);

	show_label();

	/* show controllers in telephony */
	itoa(assign_num, controllers_in_telephony, 10);
	vratputs(win, 0, 1, WINDOW_HILITE2, controllers_in_telephony);

    drop_wait();

	return(TRUE);
}

/* 	Function:	next_assign()
	Rescription:	function to handle case where END (>>) is
			selected. This increases the number of
			controllers in telephony by 1
	Inputs:		None
	Outputs:	None
	Return:		None
*/

PUBLIC void next_assign(void)
{
	int	previous_config;

	if (assign_num < terminals_in_group)
	{
		assign_num++;
		previous_config = config_num;
		config_num = 1;
		if (!show_label())
		{
		 	show_dberr(NO_LABEL_DEFINED);
			assign_num--;
			config_num = previous_config;
			show_label();
		}
		else
			refresh_assignments();
	}
	else
		show_dberr(MAX_IN_TELEPHONY);
}

/* 	Function:	previous_assign()
	Rescription:	function to handle case where BEGINNING (<<) is
			selected. This decreases the number of
			controllers in telephony by 1
	Inputs:		None
	Outputs:	None
	Return:		None
*/

PUBLIC void previous_assign(void)
{
	if (assign_num > 2)
	{
		assign_num--;
		config_num = 1;
		refresh_assignments();
	}
	else
		show_dberr(MIN_IN_TELEPHONY);
}

/* 	Function:	next_config()
	Rescription:	function to handle case where NEXT (>) is
			selected. This moves to the next
			configuration
	Inputs:		None
	Outputs:	None
	Return:		None
*/

PUBLIC void next_config(void)
{
	if (first_empty_config())
		if (config_num < 5)
		{
			config_num++;
			if (!show_label())
			{
				show_dberr(NO_LABEL_DEFINED);
				config_num--;
				show_label();
			}
			else
				refresh_assignments();
		}
}

/* 	Function:	previous_config()
	Rescription:	function to handle case where PREV (<) is
			selected. This moves to the previous configuration
	Inputs:		None
	Outputs:	None
	Return:		None
*/

PUBLIC void previous_config(void)
{
	if (config_num > 1)
	{
		config_num--;
		refresh_assignments();
	}
}

/* 	Function:	cursup()
	Rescription:	functions to handle case where cursor up key
			is pressed
	Inputs:		None
	Outputs:	None
	Return:		None
*/

PRIVATE void cursup(void)
{
	if (current_row)
		current_row--;
}

/* 	Function:	cursdn()
	Rescription:	functions to handle case where cursor down key
			is pressed
	Inputs:		None
	Outputs:	None
	Return:		None
*/

PRIVATE void cursdn(void)
{
	if (current_row < num_rows - 1)
		current_row++;
}

/* 	Function:	curslf()
	Rescription:	functions to handle case where cursor left key
			is pressed
	Inputs:		None
	Outputs:	None
	Return:		None
*/

PRIVATE void curslf(void)
{
	if (current_col)
		current_col--;
}

/* 	Function:	cursrt()
	Rescription:	functions to handle case where cursor right key
			is pressed
	Inputs:		None
	Outputs:	None
	Return:		None
*/

PRIVATE void cursrt(void)
{
	if (current_col < num_cols - 1)
		current_col++;
}

/* 	Function:	page_up()
	Rescription:	functions to handle case where Page Up
			is pressed
	Inputs:		None
	Outputs:	None
	Return:		None
*/

PRIVATE void page_up(void)
{
	if (current_page)
	{
        current_page--;
        refresh_assignments();
	}
}

/* 	Function:	page_down()
	Rescription:	function to handle case where Page Down
			is pressed
	Inputs:		None
	Outputs:	None
	Return:		None
*/

PRIVATE void page_down(void)
{
	if (current_page < max_pages)
	{
        current_page++;
		refresh_assignments();
	}
}

/* 	Function:	user_select()
	Rescription:	function to handle case where space bar
			is pressed. This will toggle the assignment
			at the current cursor position
	Inputs:		None
	Outputs:	None
	Return:		None
*/

PRIVATE void user_select(void)
{	char	*ptr;

    ptr = &user_select_table[current_row][current_col];

	if (*ptr == 'X')
		*ptr = ' ';
	else
		*ptr = 'X';
    vratputc(main_win, current_row, current_col + 21 + ((config_num - 1) * 11), WINDOW_NORMAL, *ptr);
    changed_flag = TRUE;
}

/* 	Function:	process_assign_key()
	Rescription:	high level function to handle keystrokes specific
			to active secondary line assignments not handled by
			command_dispatcher()
	Inputs:		ch - key pressed
	Outputs:	None
	Return:		ch - key_pressed
*/

PUBLIC unsigned process_assign_key(unsigned ch)
{
	int	i;
	vatputa(main_win, current_row, current_col + 21 + ((config_num - 1) * 11), WINDOW_NORMAL);
	for (i = 1; i <= 5; i++)
		vatputa(win, 2, current_col + 21 + ((i - 1) * 11), WINDOW_NORMAL);
	switch (ch)
	{	case (CURUP):
			cursup();
		break;
		case (CURDN):
			cursdn();
		break;
		case (CURLF):
			curslf();
		break;
		case (CURRT):
			cursrt();
		break;
		case (PGUP):
            if(changed_flag == TRUE)
                save_assign();
            page_up();
            changed_flag = FALSE;
		break;
		case (PGDN):
            if(changed_flag == TRUE)
                save_assign();
            page_down();
            changed_flag = FALSE;
		break;
        case (SELECT_CHAR):
			user_select();
		break;
        case (F4):
            changed_flag = FALSE;
        case (F1): case (F2): case (F3): case (F5):
        case (F6): case (F7): case (F8): case (F9): case (F10):
        case (F11): case (F12):
        case (ESC):
        break;
        default:
            if(changed_flag == TRUE)
                save_assign();
            changed_flag = FALSE;
            if(LookForMatch(ch) == SUCCESS)
                refresh_assignments();
        break;
	}
	vatputa(main_win, current_row, current_col + 21 + ((config_num - 1) * 11), BOX_VIEW_MODE);
	for (i = 1; i <= 5; i++)
		vatputa(win, 2, current_col + 21 + ((i - 1) * 11), BOX_VIEW_MODE);

	if (previous_col != current_col)
	{
		show_label();
		previous_col = current_col;
	}

	return(ch);
}

PRIVATE int LookForMatch(unsigned text)
{
    int save_curr_page = current_page;
    int label_ptr;
    int counter;

    current_page = 0;

    while(current_page < max_pages)
    {
        if (!get_phone_idx(&label_ptr))
        {
            current_page = save_curr_page;
            return ERROR;
        }

        for(counter = 0; counter < 13; counter++)
        {
            if((toupper((signed)text) == toupper(phone_labels[counter][0])) && (save_curr_page != current_page))
                return SUCCESS;
        }
        ++current_page;
    }
    current_page = save_curr_page;
    return FAILURE;
}
