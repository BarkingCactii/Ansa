#pragma pack (1)
/*
	mapopgrp.c

    System  :  Train Control Command Communication System
    Author  :  Ansa Pty. Ltd. (c) 1991
    Revision:  1.0
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "sysdef.h"
#include "filename.h"
#include "dbio.h"
#include "mnts_log.h"
#include "sdb_defn.h"

#define MAX_OP_GROUPS   31

typedef struct {
	int	stored_index;
} OPERATOR_GROUP_OFFSET;

PRIVATE OPERATOR_GROUP_OFFSET opgrp_offset[MAX_OP_GROUPS];

/* Function:	map_operator_groups()
	Rescription:	maps actual controller groups into contiguous
	      		numbers starting from 0. controller group numbers
			      are accessed by the type's OPERATOR_GROUP_OFFSET
			      offset rather than its contents. This achieves
			      the desired result
	Inputs:		None
	Outputs:	None
	Return:		TRUE - ok
			FALSE - database error
*/

PRIVATE int map_operator_groups(void)
{
	Db_Obj			*handle;
	Index_Obj       *index;
	char            rec_store[MAX_REC_LENGTH];
	char			*fld_store[MAX_FIELDS];
	int			    status;
	int			    index_no = 0;

	handle = open_database(OPERATOR_GROUP_FILE - 1);

	if ((index = get_index_handle(handle, key2)) == (Index_Obj *)NULL)
		return(FALSE);

	/* get first record */
	status = move_file_cursor(BOIND, rec_store, fld_store, handle, index, NO_ARGS);

	while (status == OK)
	{
		opgrp_offset[index_no].stored_index = atoi(fld_store[OG_INDEX]);
		status = move_file_cursor(NEXT, rec_store, fld_store, handle, index, NO_ARGS);
		index_no++;
	}

	opgrp_offset[index_no].stored_index = ERROR;
	
	close_database(handle); 

	return(TRUE);
}

/* 	Function:	get_opgrp_index()
	Rescription:	obtains a controllers group number. This number
			is typically used for producing the file extension
			for various files produced for the CT's
	Inputs:		stored_index - pointer to the actual controller
				       group index. 
	Outputs:	None
	Return:		the controller group number
*/

PUBLIC int get_opgrp_index(char *stored_index)
{	
	int	index;
	int	i = 0;

	index = atoi(stored_index);
	
	while (opgrp_offset[i].stored_index != ERROR)
	{
		if (opgrp_offset[i].stored_index == index)
			break;
		i++;
	}
	return(i);	
}

/* 	Function:	get_opgrp_item()
	Rescription:	returns a controllers group index (as stored
			in the database)
	Inputs:		which_item - the controller group number
	Outputs:	None
	Return:		the controllers group index (as stored in the
			database)
*/

PUBLIC int get_opgrp_item(int which_item)
{
	return(opgrp_offset[which_item].stored_index);
}

/* 	Function:	get_total_opgrps()
	Rescription:	returns the total number of controller groups
			This is used when creating the g_active.xxx files
			as groups with no assignments will not be created
	Inputs:		None
	Outputs:	None
	Return:		the total number of controller groups
*/

PUBLIC int get_total_opgrps(void)
{
	int	i = 0;

	while (opgrp_offset[i].stored_index != ERROR)
		i++;

	return(i);
}

/* 	Function:	map_group_page()
	Rescription:	maps actual group pages into contiguous
			numbers starting from 0. This is used
			in the default group page in password.dat
	Inputs:		None
	Outputs:	None
	Return:		TRUE - ok
			FALSE - database error
*/

PRIVATE int grppage_offset[(NUM_OF_LINE_PAGES * NUM_LINE_KEY_BUTTONS) + 1];

PRIVATE int map_group_page(void)
{	
	Db_Obj			*handle;
	Index_Obj		*index;
	char			rec_store[MAX_REC_LENGTH];
	char			*fld_store[MAX_FIELDS];
	int			    status;
	int			    index_no = 0;

	handle = open_database(GROUP_FILE - 1);

	if ((index = get_index_handle(handle, key2)) == (Index_Obj *)NULL)
		return(FALSE);

	/* get first record */
	status = move_file_cursor(BOIND, rec_store, fld_store, handle, index, NO_ARGS);

	while (status == OK)
	{
		grppage_offset[index_no] = atoi(fld_store[GP_INDEX]);
		status = move_file_cursor(NEXT, rec_store, fld_store, handle, index, NO_ARGS);
		index_no++;
	}

	grppage_offset[index_no] = ERROR;
	
	close_database(handle); 

	return(TRUE);
}

/* 	Function:	get_group_page_index()
	Rescription:	obtains a group pages number. This number
			is typically used in setting the default
			group page in password.dat
	Inputs:		stored_index - pointer to the actual group page index
	Outputs:	None
	Return:		the group page number
*/

PUBLIC int get_group_page_index(char *stored_index)
{	
	int	index;
	int	i = 0;

	index = atoi(stored_index);

	while (grppage_offset[i] != ERROR)
	{
		if (grppage_offset[i] == index)
			break;
		i++;
	}
	return(i);	
}

/* command terminal indexing routines for flat files */

#define MAX_CTS   31

typedef struct {
	int	stored_index;
} CT_OFFSET;

PRIVATE CT_OFFSET ct_offset[MAX_CTS];

PRIVATE int map_cts(void)
{
	Db_Obj			*handle;
	Index_Obj		*index;
	char            rec_store[MAX_REC_LENGTH];
	char			*fld_store[MAX_FIELDS];
	int			    status;
	int			    index_no = 0;

	handle = open_database(CT_COMMS_FILE - 1);

	if ((index = get_index_handle(handle, key2)) == (Index_Obj *)NULL)
		return(FALSE);

	/* get first record */
	status = move_file_cursor(BOIND, rec_store, fld_store, handle, index, NO_ARGS);

	while (status == OK)
	{
		ct_offset[index_no].stored_index = atoi(fld_store[CC_INDEX]);
		status = move_file_cursor(NEXT, rec_store, fld_store, handle, index, NO_ARGS);
		index_no++;
	}

	ct_offset[index_no].stored_index = ERROR;
	
	close_database(handle); 

	return(TRUE);
}

PUBLIC int get_ct_index(char *stored_index)
{	
	int	index;
	int	i = 0;

	index = atoi(stored_index);
	
	while (ct_offset[i].stored_index != ERROR)
	{
		if (ct_offset[i].stored_index == index)
			break;
		i++;
	}
	return(i);	
}

PUBLIC void make_file_indexes(void)
{
   if (!map_operator_groups())
        Mnt_LogMaintError(_ID("make_file_indexes"), 1, ERROR, "Operator Group indexing failure");
   if (!map_group_page())
        Mnt_LogMaintError(_ID("make_file_indexes"), 1, ERROR, "Group Page indexing failure");
   if (!map_cts())
        Mnt_LogMaintError(_ID("make_file_indexes"), 1, ERROR, "Command Terminal indexing failure");
}

