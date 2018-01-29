#pragma pack (1)

/*
	getidx.c

    System  :  Train Control Command Communication System
    Author  :  Ansa Pty. Ltd. (c) 1991
    Revision:  1.0
*/

#include <string.h>
#include <stdlib.h>

#include "sysdef.h"
#include "filename.h"
#include "dbio.h"
#include "form_exp.h"
#include "sdb_defn.h"

#include "getidx.h"

#define NUM_CHOICES  2

char *what_idx[MAX_FILES][NUM_CHOICES] = 	/* this is a list of keys (1 for each file)
				   on which a read is based on. This list is
				   used exclusively by get_index() */
{	{ "Key 2", "" },
	{ "Key 2", "" },
	{ "Key 2", "" },
	{ "Key 2", "" },
	{ "Key 2", "" },
	{ "Key 2", "" },
	{ "Key 2", "" },
	{ "Key 2", "" },
	{ "Key 2", "Key 7" },
	{ "Key 2", "" },
	{ "Key 2", "" },
	{ "Key 2", "" },
	{ "Key 2", "" },
	{ "Key 2", "" },
	{ "Key 2", "" },
	{ "Key 2", "" },
	{ "Key 2", "" },
	{ "Key 1", "" },
	{ "Key 1", "" },
	{ "Key 2", "" },
	{ "Key 2", "" },
	{ "Key 2", "" },
	{ "Key 2", "" },
	{ "Key 2", "" },
	{ "Key 2", "" } 
};

/* 	Function:	get_index()
	Rescription:	given the file and the key, read this record
			and get its index
	Inputs:		what - the index to the file list
			keyfld - pointer to array of pointers which make
			up the records key we wish to read
			indexptr - pointer to storage
	Outputs:	indexptr - the address the index will be copied back
				   into 
	Return:		TRUE - record found and index copied
			FALSE - error
*/

PUBLIC int get_index(int what, char *keyfld, char *indexptr, int choice)
{	
	Db_Obj		*handle;
	Index_Obj	*index;
	int		status;
	char		rec_store[MAX_REC_LENGTH];
	char		*fld_store[MAX_FIELDS];
	char		*key_store[MAX_FIELDS];
	char		tmp_store[KEY_LABEL_LEN+5];

	what--;
	if ((handle = open_database(what)) == (Db_Obj *)NULL)
		return(FALSE);

	if ((index = get_index_handle(handle, what_idx[what][choice])) == (Index_Obj *)NULL)
		return(FALSE);

	/* since a key label is made into 1 field and is actually 
	   3 fields we want put in back into its original format as
	   stored on disk so the key read can be successful */
	key_store[0] = &keyfld[0];
	if (what == GROUP_FILE - 1 || what == NUMBER_FILE - 1 ||
   	(what == CT_COMMS_FILE - 1 && choice == CHOICE2))
	{
		memset(tmp_store, 0, sizeof(tmp_store));
		memcpy(&tmp_store[0], &keyfld[0], KEY_LABEL_COLUMNS);
		memcpy(&tmp_store[KEY_LABEL_COLUMNS + 1], &keyfld[KEY_LABEL_COLUMNS], KEY_LABEL_COLUMNS);
		memcpy(&tmp_store[(KEY_LABEL_COLUMNS * 2) + 2], &keyfld[KEY_LABEL_COLUMNS * 2], KEY_LABEL_COLUMNS);

		key_store[0] = &tmp_store[0];
		key_store[1] = &tmp_store[KEY_LABEL_COLUMNS + 1];
		key_store[2] = &tmp_store[(KEY_LABEL_COLUMNS * 2) + 2];
	}

	status = move_file_cursor(FIND, rec_store, fld_store, handle, index, key_store);

	close_database(handle); 

	if (status != OK)
		return(FALSE);

	strcpy(indexptr, fld_store[inx_pos[what]]);
	return(TRUE);
}

char *index_idx[MAX_FILES] = 	/* this is a list of keys (1 for each file)
				   of which the key contents will be a index
				   This list used exclusively by find_index() */
{	"Key 1",	/* group page name */
	"Key 1",	/* phone label */
	"Key 2",
	"Key 2",
	"Key 2",
	"Key 2",
	"Key 2",
	"Key 2",
	"Key 1",	/* extension */
	"Key 2",
	"Key 1",	/* PIN number */
	"Key 1",	/* operator group name */
	"Key 2",	
	"Key 2",
	"Key 2",
	"Key 2",
	"Key 1", /* ecbu magazine */
	"Key 1",
	"Key 1",
	"Key 2",
	"Key 2",
	"Key 2",
	"Key 2",
	"Key 2",
	"Key 2"
};

/* 	Function:	find_index()
	Rescription:	given the file and its index, read this record
			to obtain other info (typically a label)
	Inputs:		what - the index to the file list
			fldno - the field number in the record
			result - pointer to storage from which the read
				 records fldno will be copied back into
			indexptr - pointer to array of pointers which make
	Outputs:	result - pointer to storage from which the read
				 records fldno will be copied back into
	Return:		TRUE - record found and info copied into result
			FALSE - error
*/

PUBLIC int find_index(int what, int fldno, char *result, char *indexptr)
{
	Db_Obj		*handle;
	Index_Obj	*index;
	int		status;
	char		rec_store[MAX_REC_LENGTH];
	char		*fld_store[MAX_FIELDS];
	char		*keyptr;

	what--;
	if ((handle = open_database(what)) == (Db_Obj *)NULL)
		return(FALSE);

	if ((index = get_index_handle(handle, index_idx[what])) == (Index_Obj *)NULL)
		return(FALSE);

	keyptr = indexptr;
	status = move_file_cursor(FIND, rec_store, fld_store, handle, index, &keyptr);

	close_database(handle); 

	if (status != OK)
		return(FALSE);

	strcpy(result, fld_store[fldno]);
	if (what == GROUP_FILE - 1 || what == NUMBER_FILE - 1 || 
       (what == CT_COMMS_FILE - 1 && fldno))
	{
		strcpy(&result[strlen(result)], fld_store[fldno+1]); 
		strcpy(&result[strlen(result)], fld_store[fldno+2]);
 	}

	/* flag if this number is a specific party line type */
	if (what == NUMBER_FILE - 1)
	{
		if (*fld_store[PN_LINE_TYPE] == OMNIBUS_CALL)
			partyln_defined = TRUE;
	}

	return(TRUE);
}

