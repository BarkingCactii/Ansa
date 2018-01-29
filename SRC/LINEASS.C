#pragma pack (1)
/*
	lineass.c
	functions for passive line assignments.

    System  :  Train Control Command Communication System
    Author  :  Ansa Pty. Ltd. (c) 1991
    Revision:  1.0
*/

#include <stdlib.h>
#include <string.h>

#include "sysdef.h"
#include "linebox.h"
#include "filename.h"
#include "dbio.h"
#include "loaddb.h"
#include "getidx.h"
#include "field.h"
#include "title.h"
#include "boxes.h"
#include "err.h"
#include "win_hues.h"
#include "smt_idle.h"
#include "sdb_defn.h"
#include "scroller.h"

PRIVATE	int	current_line;
PRIVATE int	current_page;
PRIVATE int	box_attr;
PRIVATE char	group_index[MAX_INDEX_LEN];
PRIVATE char	phone_index[MAX_INDEX_LEN];
PRIVATE int	box_contents[NUM_LINE_KEY_BUTTONS];
PRIVATE int	last_number_choosen;
PRIVATE int	last_party_choosen;

		/* used only for page copies */
PRIVATE char	page_phone_indexes[NUM_LINE_KEY_BUTTONS][MAX_INDEX_LEN];
PRIVATE char	page_line_type_indexes[NUM_LINE_KEY_BUTTONS][MAX_INDEX_LEN];

PRIVATE	int	src_line;
PRIVATE int	src_page = -1;

PRIVATE void init_lineass(void);
PRIVATE int select_group(void);
PRIVATE int write_la_rec(int, int);
PRIVATE int get_la_recs(void);
PRIVATE int del_la_rec(void);

PRIVATE	char	*page_names[] = 
{	"Page 1 ",
	"Page 2 ",
	"Page 3 ",
	"Page 4 ",
	"Page 5 ",
	(char *)NULL
};

/* 	Function:	delete_page()
	Rescription:	deletes the current page of passive line
			assignments
	Inputs:		None
	Outputs:	None
	Return:		None
*/

PRIVATE void delete_page(void)
{
	Db_Obj		*handle;
	Index_Obj	*index;
	char		*fld_store[MAX_FIELDS];
	char		*key_store[MAX_FIELDS];
	char		page[10];
	char		line[10];
	char		rec_store[MAX_REC_LENGTH];
	int		tmp_line = 0;

	if (!confirm_delete_page())
		return;

	itoa(current_page, page, 10);
	key_store[0] = group_index;
	key_store[1] = page;
	key_store[2] = line;

	if ((handle = open_database(LINEASS_FILE - 1)) == (Db_Obj *)NULL)
		return;

	if ((index = get_index_handle(handle, key2)) == (Index_Obj *)NULL)
		return;

	hilite_box(8, BOX_HILITE);

	for (tmp_line = 0; tmp_line < NUM_LINE_KEY_BUTTONS; tmp_line++)
	{
		itoa(tmp_line, line, 10);
		if (move_file_cursor(FIND, rec_store, fld_store, handle, index, key_store) != NOTFOUND)
			update_file(RECDEL, (char **)0, handle, index);
	}

	close_database(handle);

	init_lineass();

	unhilite_box();
}

/* 	Function:	add_page()
	Rescription:	copies the selected page of line assignments
			into the current page. The current page must
			be empty.
	Inputs:		None
	Outputs:	None
	Return:		None
*/

PRIVATE void add_page(void)
{	
	int	choice = ERROR;
	int	i;
	int	no_recs;
	int	save_current_page;
	int	save_current_line;
	char	*ptr;
	char	**pptr;
	char	save_group_index[MAX_INDEX_LEN];
	char	tmp_group_index[MAX_INDEX_LEN];

 	/* before we insert a page, check if the page is blank.
 	   If it is not blank, then the operation is cancelled */


 	for (i = 0; i < NUM_LINE_KEY_BUTTONS; i++)
 		if (box_contents[i] == BOX_NORMAL)
 		{
 			show_dberr(PAGE_EXISTS);
 		 	return;
 		}

	hilite_box(0, BOX_HILITE);

	no_recs = load_file(GROUP_FILE, &ptr, &pptr, NO_ARGS);
	if (no_recs)
	{
		choice = Smt_Scroller((char *)0, 5, 20, AUTO, AUTO, POPUP_NORMAL, POPUP_HILITE, (char **)pptr, 1, 0);
		if (choice != ERROR)
			get_index(GROUP_FILE, *(pptr + choice) + MARGIN, tmp_group_index, CHOICE1);

		if (choice == ERROR)
		{
			unhilite_box();
			return;
		}
	}

	choice = Smt_Scroller((char *)0, 7, 24, AUTO, AUTO, POPUP_NORMAL, POPUP_HILITE, page_names, 1, 0);

	if (choice != ERROR)
	{
		strcpy(save_group_index, group_index);
		strcpy(group_index, tmp_group_index);

		save_current_page = current_page;		
		save_current_line = current_line;

		current_page = choice;

		init_lineass();
	
		strcpy(group_index, save_group_index);
		current_page = save_current_page;

		for (current_line = 0; current_line < NUM_LINE_KEY_BUTTONS; current_line++)
		{
			if (box_contents[current_line] == BOX_NORMAL)
			{
				strcpy(phone_index, page_phone_indexes[current_line]);
				write_la_rec(RECADD, atoi(page_line_type_indexes[current_line]));
			}
		}


		current_line = save_current_line;
	}

	unhilite_box();
}

/* 	Function:	line_assignment()
	Rescription:	high level function for handling passive line 
			assignments. Pops up a window to select a group
			page, draws all the relevant boxes and passes
			control to command_dispatcher()
	Inputs:		None
	Outputs:	None
	Return:		TRUE
*/

PUBLIC int line_assignment(void)
{
	current_line = 0;
	current_page = 0;
	last_number_choosen = 0;
	last_party_choosen = 0;

	if (!select_group())
		return(TRUE);

	box_attr = BOX_HILITE;

	draw_all_boxes();
 	draw_lineass_boxes();
 	init_lineass();

	command_dispatcher((unsigned)NO_ARGS);	

 	close_all_lineass_boxes();
	close_all_boxes();

 	return(TRUE);
}

/* 	Function:	init_lineass()
	Rescription:	reads and displays all the records for the current
			group page and hilites the box at the current
			cursor position
	Inputs:		None
	Outputs:	None
	Return:		None
*/

PRIVATE void init_lineass(void)
{
	get_la_recs();

	hilite_lineass_box(current_line, box_attr);
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
	if (current_line - BOXES_PER_ROW >= 0)
		current_line -= BOXES_PER_ROW;
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
	if (current_line + BOXES_PER_ROW <= NUM_LINE_KEY_BUTTONS - 1)
		current_line += BOXES_PER_ROW;
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
	if (current_line)
		current_line--;
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
	if (current_line < NUM_LINE_KEY_BUTTONS - 1)
		current_line++;
}

/* 	Function:	process_passive_key()
	Rescription:	high level function to handle keystrokes specific
			to passive line assignments not handled by
			command_dispatcher()
	Inputs:		ch - key pressed
	Outputs:	None
	Return:		ch - key_pressed
*/

PUBLIC unsigned process_passive_key(unsigned ch)
{ 	
	unhilite_lineass_box(box_contents[current_line]);
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
		case (SF1):
			/* copy a page */
			add_page();
		break;
		case (SF9):
			/* delete a page */
			delete_page();
		break;
	}
  	hilite_lineass_box(current_line, box_attr);

	return(ch);
}

/* 	Function:	prev_page()
	Rescription:	displays the previous line page. This function is
			mapped to the PREV (<) function box
	Inputs:		None
	Outputs:	None
	Return:		None
*/

PUBLIC void prev_page(void)
{
	if (current_page)
	{
		current_page--;
		init_lineass();
		display_page(current_page + 1);
	}
	else
		show_dberr(NO_PREV_PAGE);
}

/* 	Function:	next_page()
	Rescription:	displays the next line page. This function is
			mapped to the NEXT (>) function box
	Inputs:		None
	Outputs:	None
	Return:		None
*/

PUBLIC void next_page(void)
{
	if (current_page < NUM_OF_LINE_PAGES - 1)
	{
		current_page++;
		init_lineass();
		display_page(current_page + 1);
	}
	else
		show_dberr(NO_NEXT_PAGE);
}

/* 	Function:	first_page()
	Rescription:	displays the first line page for this group page. 
			This function is mapped to the BEGINNING (<<) 
			function box
	Inputs:		None
	Outputs:	None
	Return:		None
*/

PUBLIC void first_page(void)
{
	current_page = 0;
	init_lineass();
	display_page(current_page + 1);
}

/* 	Function:	last_page()
	Rescription:	displays the last line page (page 5) for this group 
			page. This function is mapped to the END (>>) 
			function box
	Inputs:		None
	Outputs:	None
	Return:		None
*/

PUBLIC void last_page(void)
{
	current_page = NUM_OF_LINE_PAGES - 1;
	init_lineass();
	display_page(current_page + 1);
}

/* 	Function:	get_group()
	Rescription:	pops up a window allowing the user to select
			a group page,This function is mapped to the GRP 
			function box
	Inputs:		None
	Outputs:	None
	Return:		None
*/

PUBLIC void get_group(void)
{
	int	tmp;

	if (!select_group())
		return;

	if (src_page != -1)
	{
		/* in the middle of a move */
		src_page = -1;
		box_attr = BOX_HILITE;
		tmp = last_hilite_box;
		last_hilite_box = 2;
		unhilite_box();
		last_hilite_box = tmp;
	}
	init_lineass();
}

/* 	Function:	linerec_to_db()
	Rescription:	pops a list of extension numbers which the user
			may select one which will be displayed in the
			current cursor position. The choice is written
			immediately to the database
	Inputs:		db_action - RECADD - add to a blank key
				  - RECMOD - modify an existing key
	Outputs:	Mone
	Return:		None
*/

PRIVATE void linerec_to_db(int db_action, int file)
{
	int	choice = ERROR;
	char	*ptr;
	char	**pptr;
	int	no_recs;

	if (db_action == RECMOD && box_contents[current_line] == BOX_UNUSED)
	{
		show_dberr(NOT_WRITTEN);
	 	return;
	}

	if (db_action == RECADD && box_contents[current_line] == BOX_NORMAL)
	{	
		show_dberr(RECORD_EXISTS);
	 	return;
	}

   if (file == NUMBER_FILE)
   {
   	no_recs = load_file(NUMBER_FILE, &ptr, &pptr, PASSIVE_LINE);
   	if (no_recs)
         choice = Smt_Scroller((char *)0, 5, 20, AUTO, AUTO, POPUP_NORMAL, POPUP_HILITE, pptr, 1, last_number_choosen);
   	else
   	{	
	   	show_dberr(NO_PHONE_RECORDS);
   		return;
   	}
   }
   else
   {
   	no_recs = load_file(CT_COMMS_FILE, &ptr, &pptr, LINEASS_FILE);
   	if (no_recs)
         choice = Smt_Scroller((char *)0, 5, 20, AUTO, AUTO, POPUP_NORMAL, POPUP_HILITE, pptr, 1, last_party_choosen);
   	else
   	{	
	   	show_dberr(NO_CT_RECS);
   		return;
   	}
   }


	if (choice != ERROR)
	{
      if (file == NUMBER_FILE)
      {
   		last_number_choosen = choice;
  	   	get_index(file, *(pptr + choice) + MARGIN, phone_index, CHOICE1);
      }
      else
      {
   		last_party_choosen = choice;
  	   	get_index(file, *(pptr + choice) + MARGIN, phone_index, CHOICE2);
      }

		/* write the new record */
		if (write_la_rec(db_action, file))
			/* print label in box */
			display_label(current_line, *(pptr + choice) + MARGIN);
		else
			show_dberr(NOT_WRITTEN);

		hilite_lineass_box(current_line, box_attr);

	}
}

/* 	Function:	add_line()
	Rescription:	pops up a window of extension numbers allowing the 
			user to add a new line key.This function is mapped 
			to the ADD (F1)	function box
	Inputs:		None
	Outputs:	None
	Return:		None
*/

PUBLIC void add_line(void)
{
	linerec_to_db(RECADD, NUMBER_FILE);
}

PUBLIC void add_party(void)
{
	linerec_to_db(RECADD, CT_COMMS_FILE);
}

/* 	Function:	get_la_recs()
	Rescription:	reads all the line keys from the database for the
			current page and uses this information to refresh
			the screen.
	Inputs:		None
	Outputs:	None
	Return:		TRUE - ok
			FALSE = database error
*/

PRIVATE int get_la_recs(void)
{
	Db_Obj		*handle;
	Index_Obj	*index;
	int		status;
	char		*fld_store[MAX_FIELDS];
	char		*key_store[MAX_FIELDS];
	char		page[10];
	char		line[10];
	char		rec_store[MAX_REC_LENGTH];
	int		tmp_line = 0;
	char		label_txt[50];
   int      file;
   int      label_fld;

	if ((handle = open_database(LINEASS_FILE - 1)) == (Db_Obj *)NULL)
		return(FALSE);

	if ((index = get_index_handle(handle, key2)) == (Index_Obj *)NULL)
		return(FALSE);

	itoa(current_page, page, 10);
	key_store[0] = group_index;
	key_store[1] = page;
	key_store[2] = line;

	memset(page_phone_indexes, 0, sizeof(page_phone_indexes));
   memset(page_line_type_indexes, 0, sizeof(page_line_type_indexes));
          
	for (tmp_line = 0; tmp_line < NUM_LINE_KEY_BUTTONS; tmp_line++)
	{
		memset(label_txt, ' ', sizeof(label_txt));
		itoa(tmp_line, line, 10);

		status = move_file_cursor(FIND, rec_store, fld_store, 
		     handle, index, key_store);

		box_contents[tmp_line] = BOX_UNUSED;
		if (status != NOTFOUND)
		{
         file = atoi(fld_store[LA_LINE_TYPE]);
         if (file == NUMBER_FILE)
            label_fld = 0;
         else
            label_fld = CC_CT_TARGET_DESC1;

			find_index(file, label_fld, label_txt, fld_store[LA_PN_INDEX]);
			strcpy(page_phone_indexes[tmp_line], fld_store[LA_PN_INDEX]);
         strcpy(page_line_type_indexes[tmp_line], fld_store[LA_LINE_TYPE]);
			box_contents[tmp_line] = BOX_NORMAL;
		}

		display_label(tmp_line, label_txt);
	}

	close_database(handle);
	return(TRUE);
}

/* 	Function:	write_la_rec()
	Rescription:	writes the current line key to the database
	Inputs:		mode - RECADD - add a new record
			       RECMOD - modify an existing record
	Outputs:	None
	Return:		TRUE - ok
			FALSE - database error
*/

PRIVATE int write_la_rec(int mode, int file)
{	
	Db_Obj		*handle;
	Index_Obj	*index;
	char		*fld_store[MAX_FIELDS];
	char		*key_store[MAX_FIELDS];
	char		page[10];
	char		line[10];
	char		type[10];
	char		rec_store[MAX_REC_LENGTH];

	itoa(current_page, page, 10);
	itoa(current_line, line, 10);
	itoa(file, type, 10);

	if ((handle = open_database(LINEASS_FILE - 1)) == (Db_Obj *)NULL)
		return(FALSE);

	if ((index = get_index_handle(handle, key2)) == (Index_Obj *)NULL)
		return(FALSE);

	if (mode == RECMOD)
	{	
		key_store[0] = group_index;
		key_store[1] = page;
		key_store[2] = line;

		if (move_file_cursor(FIND, rec_store, fld_store, handle, index, key_store) == NOTFOUND)
			return(FALSE);
	}

	fld_store[LA_PN_INDEX] = phone_index;
	fld_store[LA_GP_INDEX] = group_index;
	fld_store[LA_PAGE] = page;
	fld_store[LA_LINE_NO] = line;
   fld_store[LA_LINE_TYPE] = type;

	if (update_file(mode, (char **)&fld_store, handle, index) == ERROR)
		return(FALSE);

	box_contents[current_line] = BOX_NORMAL;
	
	close_database(handle);

    return(TRUE);
}

/* 	Function:	delete_line()
	Rescription:	high level function to delete the current line key. 
			This functions is mapped to the DEL (F9) key
	Inputs:		None
	Outputs:	None
	Return:		None
*/

PUBLIC void delete_line(void)
{
	if (box_contents[current_line] == BOX_UNUSED)
		show_dberr(NOT_WRITTEN);
	else
	{
		if (confirm_delete())
		{
			del_la_rec();
			init_lineass();
		}
	}
}

/* 	Function:	del_la_rec()
	Rescription:	deletes the current line key from the database. 
	Inputs:		None
	Outputs:	None
	Return:		TRUE - ok
			FALSE - database error
*/

PRIVATE int del_la_rec(void)
{	Db_Obj		*handle;
	Index_Obj	*index;
	char		*fld_store[MAX_FIELDS];
	char		*key_store[MAX_FIELDS];
	char		page[10];
	char		line[10];
	char		rec_store[MAX_REC_LENGTH];
	int		tmp_line = 0;
	int		shifting_delete;

	shifting_delete = delete_method();

	itoa(current_page, page, 10);
	tmp_line = current_line;
	itoa(tmp_line, line, 10);
	key_store[0] = group_index;
	key_store[1] = page;
	key_store[2] = line;

	if ((handle = open_database(LINEASS_FILE - 1)) == (Db_Obj *)NULL)
		return(FALSE);

	if ((index = get_index_handle(handle, key2)) == (Index_Obj *)NULL)
		return(FALSE);

	if (move_file_cursor(FIND, rec_store, fld_store, handle, index, key_store) != NOTFOUND)
		update_file(RECDEL, (char **)0, handle, index);

	tmp_line++;
	itoa(tmp_line, line, 10);

	while (shifting_delete && tmp_line <= NUM_LINE_KEY_BUTTONS)
	{	
		if (move_file_cursor(FIND, rec_store, fld_store, handle, index, key_store) == OK)
		{
			/* the following is legal ONLY because the line no
			   is the last field for the record */
			itoa(tmp_line - 1, fld_store[LA_LINE_NO], 10);
			if (update_file(RECMOD, (char **)&fld_store, handle, index) == ERROR)
				return(FALSE);
		}
		tmp_line++;
		itoa(tmp_line, line, 10);
	}

	close_database(handle);
	return(TRUE);
}

/* 	Function:	modify_line()
	Rescription:	pops up a window of extension numbers allowing the 
			user to modify an existing line key.This function 
			is mapped to the MOD (F2) function box
	Inputs:		None
	Outputs:	None
	Return:		None
*/

PUBLIC void modify_line(void)
{
	linerec_to_db(RECMOD, atoi(page_line_type_indexes[current_line]));
}

/* 	Function:	move_line()
	Rescription:	allows the user to move the line key at the current
			cursor position to a new un-occupied position
			within the same group page. The line key to be moved
			will be highlighted to signify move mode is active.
			This function is mapped to the Mov (F3) key
	Inputs:		None
	Outputs:	None
	Return:		None
*/

PUBLIC void move_line(void)
{
	Db_Obj		*handle;
	Index_Obj	*index;
	char	*fld_store[MAX_FIELDS];
	char	rec_store[MAX_REC_LENGTH];
	char	*key_store[MAX_FIELDS];
	int	status;
	char	page_str[10];
	char	line_str[10];

	/* set up the key */
	itoa(current_page, page_str, 10);
	itoa(current_line, line_str, 10);
	key_store[0] = group_index;
	key_store[1] = page_str;
	key_store[2] = line_str;

	if (src_page == -1)
	{	
		/* check if a record exists at the current cursor position */
		status = read_record(LINEASS_FILE, key_store, rec_store, fld_store, key2);

		if (status != OK)
		{
			/* no record to move */
			show_dberr(NO_RECORDS);
			return;
		}

		/* hilite box to show VIEW mode */
		hilite_box(2, BOX_VIEW_MODE);
		last_hilite_box = -1;
		box_attr = BOX_VIEW_MODE;

		/* save position of record to be moved */
		src_page = current_page;
		src_line = current_line;

		/* hilite the box */
	  	hilite_lineass_box(current_line, box_attr);

		return;
	}

	/* check if a record exists at the current cursor position */
	status = read_record(LINEASS_FILE, key_store, rec_store, fld_store, key2);
	if (status == OK)
		show_dberr(DUPLICATE);
	else
	{
		handle = open_database(LINEASS_FILE - 1);
		index = get_index_handle(handle, key2);

		itoa(src_page, page_str, 10);
		itoa(src_line, line_str, 10);

		/* position file cursor */
		status = move_file_cursor(FIND, rec_store, fld_store, handle, index, key_store);

		if (status == OK)
		{
			/* update record */
			itoa(current_page, page_str, 10);
			itoa(current_line, line_str, 10);
			fld_store[LA_LINE_NO] = line_str;
			fld_store[LA_PAGE] = page_str;
			update_file(RECMOD, (char **)&fld_store, handle, index);
			init_lineass();
		}
	}

	/* set box attribues back to normal */
	last_hilite_box = 2;
	box_attr = BOX_HILITE;
	src_page = -1;
  	hilite_lineass_box(current_line, box_attr);
}

/* 	Function:	select_group()
	Rescription:	pops up a windows allowing the user to select
			a new group page
	Inputs:		None
	Outputs:	None
	Return:		TRUE - ok
			FALSE - database error
*/

PRIVATE int select_group(void)
{	
	int	choice;
	char	*ptr;
	char	**pptr;
	int	no_recs;

	no_recs = load_file(GROUP_FILE, &ptr, &pptr, NO_ARGS);
	if (no_recs)
	{
		choice = Smt_Scroller((char *)0, 5, 20, AUTO, AUTO, POPUP_NORMAL, POPUP_HILITE, (char **)pptr, 1, 0);
		if (choice != ERROR)
		{
			display_group(*(pptr + choice) + MARGIN);
			display_page(current_page + 1);

			get_index(GROUP_FILE, *(pptr + choice) + MARGIN, group_index, CHOICE1);
		}

		if (choice == ERROR)
			return(FALSE);
	}
	else
	{
		show_dberr(NO_GROUP_PAGES);
		return(FALSE);
	}

	return(TRUE);
}

