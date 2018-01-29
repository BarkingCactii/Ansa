#pragma pack (1)
/*
	dbio.c

	Contains high level functions of access to
	the ISAM library. 
	Helps to keep all direct references to the ISAM library
	isolated

    System  :  Train Control Command Communication System
    Author  :  Ansa Pty. Ltd. (c) 1991

    Revision;   11 Dec 90   Written     Jeff Hill
	        	12 Feb 91	Modified	Jeff Hill

	12 Feb 91
		calls to open_database() now store handles seperately for
		each file instead of previously being overwritten by
		subsequent calls. This means that if a file is accidentally
		left open an error will not occur when it is re-opened;
		rather it will be closed anbd re-opened.
*/

#include <string.h>
#include <stdlib.h>
#include <ibmkeys.h>
/*
#include <sfwin.h>
*/

#include "sysdef.h"
#include "filename.h"
#include "file_mon.h"
#include "err.h"
#include "ascii.h" 
#include "dbconfig.h"
#include "mnts_log.h"

#include "dbio.h"


#define	BOI_MESS	"Start of file"
#define	EOI_MESS	"End of file"

/*
 *  list of handles associated with each file
 */

PRIVATE int      database_initialized = FALSE;
PRIVATE Db_Obj 	*open_file_handle[MAX_FILES];

/* 	Function:	create_ref()
    Description:    creates the reference record in an index file
			normally this will only be called once when the
			first imdex is added to the file
	Inputs:		handle - index file handle
			index - index file key
	Outputs:	None
	Return:		ERROR - severe condition
			1 - when the reference record is first added
*/
	
PRIVATE int create_ref ( Db_Obj *handle, Index_Obj *index )
{	
	int	status;

    Mnt_LogMaintError ( _ID("create_ref"), SDB_GENERAL_REPORT, FALSE,       \
                                                "Reference Record created" );

	if ((status = update_file(RECADD, new_ref, handle, index)) == ERROR)
	{
        sprintf ( Mnt_mnt_err_mesg, "Creating Ref record - status [%d]",    \
                                                                    status );
        Mnt_LogMaintError ( _ID("create_ref"), SDB_GENERAL_ERROR, ERROR,    \
                                                            Mnt_mnt_err_mesg );
		return(ERROR);
	}

	return(atoi(new_ref[MI_INDEX]));
}	

/* 	Function:	next_slot()
    Description:    add a new index to an index file
	Inputs:		handle - index file handle
			index - key to read on
	Outputs:	tmp - a value which will be used as a pointer
			to this record from other records
	Return:		ERROR - fatal terminal condition
			RECMOD - the new index replaces an existing empty index
			RECADD - the new index is to be created
*/

PRIVATE int next_slot(Db_Obj *handle, Index_Obj *index, int *tmp)
{	
	int		status;
	char		rec_store[MAX_REC_LENGTH];
	char		*fld_store[MAX_FIELDS];

	/* index on flag field to check if there are any records marked
	   as empty. If so, then that record will be used */

	if ((status = move_file_cursor(FIND, rec_store, fld_store, handle, index, empty)) == ERROR)
		return(ERROR);

	/* empty record found */
	if (status == OK)
	{
		*tmp = atoi(fld_store[MI_INDEX]);
		return(RECMOD);
	}

    /*
     *  look at the reference record to get the last record added
     */

    if ((status = move_file_cursor(FIND, rec_store, fld_store, handle, index, ref)) == ERROR)
	{
        sprintf ( Mnt_mnt_err_mesg, "Locating Ref record - status [%d]",    \
                                                                    status );
        Mnt_LogMaintError ( _ID("next_slot"), SDB_GENERAL_ERROR, ERROR,     \
                                                        Mnt_mnt_err_mesg );
		return(ERROR);
	}

	/* reference record found */
	if (status == OK)
		*tmp = atoi(fld_store[MI_INDEX]) + 1;
	else
	{
		/* create the reference record */
        if ((status = create_ref(handle, index)) == ERROR)
			return(ERROR);
		else
			*tmp = status;
	}

	return(RECADD);
}

/* 	Function:	update_ref()
    Description:    updates the reference record in an index file
			to reflect the last index number occupied
	Inputs:		handle - index file handle
			index - index file key
			new_index - value the reference record will now
				contain
	Outputs:	None
	Return:		ERROR - reference record not found (FATAL ERROR)
			OK - update succuessful
*/

PRIVATE int update_ref(Db_Obj *handle, Index_Obj *index, char *new_index)
{	
	int		status;
    char    rec_store[MAX_REC_LENGTH];
    char   *fld_store[MAX_FIELDS];

    /* get the reference record */

	if ((status = move_file_cursor(FIND, rec_store, fld_store, handle, index, ref)) == ERROR)
	{
        sprintf ( Mnt_mnt_err_mesg, "Locating Ref record - status [%d]",    \
                                                                    status );
        Mnt_LogMaintError ( _ID("update_ref"), SDB_GENERAL_ERROR, ERROR,    \
                                                            Mnt_mnt_err_mesg );
		return(ERROR);
	}

	/* this line is valid ONLY because this is the last field in the record 
	   (which it always is in this implementation) */
	strcpy(fld_store[MI_INDEX], new_index);

	return(update_file(RECMOD, fld_store, handle, index));
}	

/*
 *  Functions       file_path()
    Description:    makes the full path of the database
    Inputs:         index - a value which is used to index
                            a table of file names
                    fullpath - pointer to a string to contain the
                                full file path
    Outputs:        fullpath - will contain the path and filename
                    where files are to be stored
*/
PUBLIC void file_path ( int index, char *fullpath )
{	
    /*
     *  copy path and append filename
     */

    strcat ( strcpy ( fullpath, Sdb_data_path ), file_attr[index].filename );
}

/*
 *  Functions       create_database()
    Description:    creates a database
	Pre-condition:	database must not exist
	Post-condition:	database is created
    Inputs:         which_database - a value which is used to index
                                    a table of file names
                    fld_ptr - a pointer to an array of pointers
                                which point to the fields which define
                                the record
    Return:         NULL - database not created
                    or Db_obj * to the database
*/

PUBLIC Db_Obj *create_database ( int which_database, char *fld_ptr[] )
{
    char    path[_MAX_PATH];

#ifdef  _FM
    file_monitor ( which_database, OPEN_STATE );
#endif

    file_path ( which_database, path );

    sprintf ( Mnt_mnt_err_mesg, "Creating database file [%s]", path );
    Mnt_LogMaintError ( _ID("create_database"), SDB_GENERAL_REPORT, FALSE,  \
                                                        Mnt_mnt_err_mesg );

    open_file_handle[which_database] = icreate_db ( path, DEFAULT_BLKSIZE,  \
                                                                fld_ptr );
    return ( open_file_handle[which_database] );
}

/*
 *  Function:       dbfile_handle()
    Description:    returns the handle of an open database file
    Inputs:         which_database - the file name index
    Return:         Db_Obj *database handle
                    0 - file not open
*/

PUBLIC Db_Obj *dbfile_handle ( int which_database )
{
	return(open_file_handle[which_database]);
} 

/* 	Function:	mark_current_record()
    Description:    marks the current record in the specified database
	Inputs:		which_database - the file name index
	Outputs:	None
	Return:		OK - ok
			ERROR - unsuccessful
*/

PUBLIC int mark_current_record(int which_database, Index_Obj *index)
{
	return(imarkrec(open_file_handle[which_database], index));
}

/* 	Function:	find_mark()
    Description:    makes the record marked by mark_current_record
			the current record
	Inputs:		which_database - the file name index
	Outputs:	None
	Return:		OK - ok
			ERROR - unsuccessful
*/

PUBLIC int find_mark(int which_database, Index_Obj *index)
{
	return(ifindmark(open_file_handle[which_database], index));
}

/* 	Functions 	close_database()
    Description:    closes a database
	Pre-condition:	database must be open
	Post-condition:	database is closed
	Inputs:		db_ptr - pointer to an open database
	Outputs:	None
	Return:		None
*/

#pragma argsused

PUBLIC void close_database(Db_Obj *db_ptr)
{
    return;

/*
 *  find out which file this handle belongs to

	for (i = 0; i < MAX_FILES; i++)
	{
		if (open_file_handle[i] == db_ptr)
		{
			open_file_handle[i] = (Db_Obj *)NULL;
			file_closed = TRUE;
#ifdef  _FM
			file_monitor(i, CLOSE_STATE);
#endif
		}
	}

	if (!file_closed)
		return;
	else
		iclose_db(db_ptr);
 */
}

PUBLIC void close_datafile(Db_Obj *db_ptr)
{
    int     i;
/*
 *  find out which file this handle belongs to
 */
	for (i = 0; i < MAX_FILES; i++)
	{
		if (open_file_handle[i] == db_ptr)
		{
    		iclose_db(db_ptr);
			open_file_handle[i] = (Db_Obj *)NULL;
#ifdef  _FM
			file_monitor(i, CLOSE_STATE);
#endif
		}
	}
}

/* 	Functions 	open_database()
    Description:    Opens a database
	Pre-condition:	None
	Post-condition:	database is opened
	Inputs:		which_database - a value which is used to index
				a table of file names
	Outputs:	None
	Return:		NULL - database not opened
			or Db_obj * to the database
*/

PUBLIC Db_Obj *open_database ( int which_database )
{
    char    path[_MAX_PATH];

	/* initialize array of handles if first time called */

    if ( database_initialized == FALSE )
	{
        memset ( (void *) &open_file_handle[0], 0x00,                       \
                                            sizeof ( open_file_handle ) );
		database_initialized = TRUE;
	}

    /*
     *  close the database first if has been left open
     *
     *  if (open_file_handle[which_database] != (Db_Obj *)NULL)
     *      close_database(open_file_handle[which_database]);
     */


   /*
    *   this line new edition
    */

    if ( open_file_handle[which_database] == (Db_Obj*) NULL )
    {
        file_path ( which_database, path );
        open_file_handle[which_database] = iopen_db ( path );

#ifdef  _FM
        if (open_file_handle[which_database])
            file_monitor ( which_database, OPEN_STATE );
#endif
    }

    if (open_file_handle[which_database] == (Db_Obj *)NULL)
    {
        sprintf(Mnt_mnt_err_mesg, "Open [%s] failed. Errno [%d]", path, errno);
        Mnt_LogMaintError ( _ID("open_database"), SDB_GENERAL_ERROR, ERROR, Mnt_mnt_err_mesg );
    }
 
    return ( open_file_handle[which_database] );
}

/* 	Functions 	flush_database()
    Description:    flushes a database (ie all files are closed)
	Pre-condition:	None
	Post-condition:	database is closed
	Inputs:		None
	Outputs:	None
	Return:		None
*/

PUBLIC void flush_database()
{	int	i;

	for (i = 0; i < MAX_FILES; i++)
    {
        if ( open_file_handle[i] )
		{
/*
 *          LogMaintenanceError(i, "Database flushed to disk _flush_database");
 */

#ifdef  _FM
			file_monitor(i, CLOSE_STATE);
#endif
            iclose_db(open_file_handle[i]);      /* addition for quick file access */
            open_file_handle[i] = (Db_Obj *)NULL;
/*
 *          close_database(open_file_handle[i]);
 */
		}
    }
}
	
/* 	Functions 	move_file_cursor()
    Description:    performs actions which can affect the file cursor
	   		(except key reads).
	Pre-condition:	the database must be opened
	Post-condition:	None
	Inputs:		which_one - the type of action to perform on the
				database; ie next, previous, beginning,
				end of file or kryed find
			rec_ptr - a pointer to a char array large
				enough to receive the record to be read
			field_ptr - a pointer to an array of pointers
				large enough to receive pointer for each
				field of the record
			db_ptr - the database handle of the file
			int_ptr - the index handle of the file
			key_ptr - pointer to an array of pointers
				containing the key values (FIND only)
	Outputs:	rec_ptr - will contain the record
			field_ptr - will contains the field pointers
	Return:		OK - operation OK
			BOI - Beginning of Index
			EOI - End of index
			ERROR - error has occurred
			FOUND - record found (FIND only)
			NOTFOUND - record not found (FIND only)
*/

PUBLIC int move_file_cursor(	int		which_one,
				char 		*rec_ptr,
				char		*field_ptr[],
				Db_Obj 		*db_ptr,
				Index_Obj	*ind_ptr,
				char		*key_ptr[])

{
	int	status;		/* result of ISAM function calls */
	Rec_Len	rec_len;	/* record length for the record to be read */

#ifdef _FM
    int     i;

	for (i = 0; i < MAX_FILES; i++)
	{
		if (open_file_handle[i] == db_ptr)
        {
            file_monitor ( i, READ_STATE );
            break;
        }
    }
#endif
	if (interrupt_mode && gfkbhit())
	{
		interrupt_mode = ABORT_PROCESS;
	 	getkey();
	}

	switch (which_one)
	{
		case (BOIND):
			status = ifindhead(db_ptr, ind_ptr);
		break;
		case (EOIND):
			status = ifindtail(db_ptr, ind_ptr);
		break;
		case (NEXT):
			status = ifindnext(db_ptr, ind_ptr);
		break;
		case (PREVIOUS):
			status = ifindprev(db_ptr, ind_ptr);
		break;
		case (FIND):
			status = ifindkey(db_ptr, ind_ptr, key_ptr);
		break;
		case (RECCURRENT):
			status = OK;
		break;
		default:
			status = ERROR;
		break;

	}
	if (status == OK || status == FOUND)
	{	status = ERROR;
		if (igetreclen(db_ptr, ind_ptr, &rec_len) == OK)
			if (igetrec(db_ptr, ind_ptr, field_ptr, rec_ptr, rec_len + 1) == OK)
				status = OK;
		if (which_one == RECCURRENT && status != OK)
			/* end of file reached after delete, so ignore */
			status = EOI;
	}
#ifdef _FM
    file_monitor ( i, OPEN_STATE );
#endif
	return(status);
}

PUBLIC int find_closest_record(char 		*rec_ptr,
				char		*field_ptr[],
				Db_Obj 		*db_ptr,
				Index_Obj	*ind_ptr,
				char		*key_ptr[])

{
	int	status;		/* result of ISAM function calls */
	Rec_Len	rec_len;	/* record length for the record to be read */

   status = ERROR;
    if (ifindkey(db_ptr, ind_ptr, key_ptr) != ERROR)
	    if (igetreclen(db_ptr, ind_ptr, &rec_len) == OK)
		    if (igetrec(db_ptr, ind_ptr, field_ptr, rec_ptr, rec_len + 1) == OK)
			    status = OK;

	return(status);
}

/* 	Functions 	update_file()
    Description:    performs actions which can affect the file contents
	   		(add modify delete).
	Pre-condition:	the database must be opened
	Post-condition:	the file will be modified as requested
	Inputs:		which_one - the type of action to perform on the
				database; ie next, previous, beginning,
				end of file or kryed find
			field_ptr - a pointer to an array of pointers
				which point to the fields in the record
			db_ptr - the database handle of the file
			int_ptr - the index handle of the file
	Outputs:	None
	Return:		OK - operation OK
			ERROR - error has occurred
*/

PUBLIC int update_file( int which_one, char *field_ptr[], Db_Obj *db_ptr,   \
                                                        Index_Obj   *ind_ptr)
{	
	int	status;

#ifdef _FM
    int     i;

	for (i = 0; i < MAX_FILES; i++)
	{
		if (open_file_handle[i] == db_ptr)
        {
            file_monitor ( i, WRITE_STATE );
            break;
        }
    }
#endif

	switch (which_one)
	{
		case (RECADD):
			status = iaddrec(db_ptr, ind_ptr, field_ptr);
			break;
		case (RECMOD):
			/* 
				move file cursor back a record
			   	as modify advances it
			*/ 
			if ((status = imodrec(db_ptr, ind_ptr, field_ptr)) == OK)
				ifindprev(db_ptr, ind_ptr);
			break;
		case (RECDEL):
			status = idelrec(db_ptr, ind_ptr);
			break;
		default:
			status = ERROR;
	}

	if (status != OK)
    {
        sprintf ( Mnt_mnt_err_mesg, "Record update failed - status [%d]",   \
                                                                    status );
        Mnt_LogMaintError ( _ID("update_file"), SDB_GENERAL_ERROR, ERROR,   \
                                                        Mnt_mnt_err_mesg );
    }
#ifdef _FM
    file_monitor ( i, OPEN_STATE );
#endif
	return(status);
}

/* 	Functions 	make_index()
    Description:    makes the physical and user indexes for a
			given file
	Pre-condition:	database must exist and opened
	Post-condition:	index is created
	Inputs:		file_no - a value which is used to *index
				a table of file names
			handle - database handle to the file to be indexed
	Outputs:	None
	Return:		OK
			ERROR
*/

PUBLIC int make_index(Db_Obj *handle, int file_no)
{
	inew_index_file(handle, DEFAULT_BLKSIZE, YES);
	imkindex(handle, key1, file_attr[file_no].key_fields[0]);		

	if (file_attr[file_no].key_fields[1])
		imkindex(handle, key2, file_attr[file_no].key_fields[1]);		
	if (file_attr[file_no].key_fields[2])
		imkindex(handle, key3, file_attr[file_no].key_fields[2]);		
	if (file_attr[file_no].key_fields[3])
		imkindex(handle, key4, file_attr[file_no].key_fields[3]);		
	if (file_attr[file_no].key_fields[4])
		imkindex(handle, key5, file_attr[file_no].key_fields[4]);		
	if (file_attr[file_no].key_fields[5])
		imkindex(handle, key6, file_attr[file_no].key_fields[5]);		
	if (file_attr[file_no].key_fields[6])
		imkindex(handle, key7, file_attr[file_no].key_fields[6]);		

	return(OK);
}

/* 	Functions 	get_index_handle()
    Description:    gets the handle for an index
	Pre-condition:	database opened
	Post-condition:	None
	Inputs:		handle - database handle to the file to be indexed
			index_name - the name to which the index is referred
	Outputs:	None
	Return:		handle of index, or
			NULL
*/

PUBLIC Index_Obj *get_index_handle(Db_Obj *handle, char *index_name)
{
	Index_Obj 	*index;

	index = ihandle(handle, index_name);

	if (index == (Index_Obj *)NULL)
    {
        sprintf ( Mnt_mnt_err_mesg, "Bad db index handle [%s]", index_name );
        Mnt_LogMaintError ( _ID("get_index_handle"), SDB_GENERAL_ERROR,     \
                                                ERROR, Mnt_mnt_err_mesg );
    }
	return(index);
}

/* 	Function:	update_mindex()
    Description:    top level function to update an index file
			with a new entry
	Inputs:		type - file number which is to contain the
			       new index
	Outputs:	index_val - the string value the new index is
 				stored as in the index file. This value
				will be stored in the modified record
				as a pointer to the index file
	Return:		TRUE - done
			FALSE - not done (not neccessarily an error) 
*/


PUBLIC int update_mindex(int type, char *index_val)
{
	Db_Obj		*handle;
	Index_Obj	*index;
    int          status;
    char         new_rec_store[MAX_REC_LENGTH];
	char		*fld_store[MAX_FIELDS];
    int          new_rec;
    int          action;
	char		*tmpptr;

	if (!type)
		return(FALSE);

	if ((handle = open_database(type - 1)) == (Db_Obj *)NULL)
		return(FALSE);

	if ((index = get_index_handle(handle, key2)) == (Index_Obj *)NULL)
		return(ERROR);

	/* find where the data is to be slotted into */

	if ((action = next_slot(handle, index, &new_rec)) == ERROR)
	{
        sprintf ( Mnt_mnt_err_mesg, "Slot not found - index type [%d]",     \
                                                                    type );
        Mnt_LogMaintError ( _ID("update_mindex"), SDB_GENERAL_ERROR, ERROR, \
                                                        Mnt_mnt_err_mesg );
		return(ERROR);
	}

	/* update the index */
	fld_store[MI_FLAG] = new_rec_store;
	strcpy(fld_store[MI_FLAG], (char *)*active);
	tmpptr = new_rec_store + strlen(new_rec_store) + 1;
	fld_store[MI_INDEX] = tmpptr;
	strcpy(fld_store[MI_INDEX], itoa(new_rec, fld_store[MI_INDEX], 10));

	/* update the reference record */
	if ((status = update_file(action, fld_store, handle, index)) != ERROR)
		if (action == RECADD)
			status = update_ref(handle, index, fld_store[MI_INDEX]);
	
	close_database(handle); 

	if (status == ERROR)
	{
        Mnt_LogMaintError ( _ID("update_mindex"), SDB_GENERAL_ERROR, ERROR, \
                                            "Master index update failed" );
		return(FALSE); 
	}

	strcpy(index_val, fld_store[MI_INDEX]);

	return(TRUE);
}

/* 	Function:	delete_mindex()
    Description:    deletes an entry in the index file by
			marking it as empty
	Inputs:		type - the index file type
			index_val - the matching entry to be deleted
	Outputs:	None
	Return:		TRUE - ok
			FALSE - not done because irrelevant
			ERROR - fatal error
*/

PUBLIC int delete_mindex(int type, char *index_val)
{
	Db_Obj		*handle;
	Index_Obj	*index;
	char		rec_store[MAX_REC_LENGTH];
	char		*fld_store[MAX_FIELDS];

	if (!type)
		return(FALSE);

	if ((handle = open_database(type - 1)) == (Db_Obj *)NULL)
		return(FALSE);

	if ((index = get_index_handle(handle, key1)) == (Index_Obj *)NULL)
		return(ERROR);

	fld_store[0] = index_val;

    if (move_file_cursor(FIND, rec_store, fld_store, handle,                \
                                                index, fld_store) == ERROR)
	{
        sprintf ( Mnt_mnt_err_mesg, "Could not find matching idx type [%d]",\
                                                                    type );
        Mnt_LogMaintError ( _ID("delete_mindex"), SDB_GENERAL_ERROR, ERROR, \
                                                        Mnt_mnt_err_mesg );
                                        
		return(ERROR);
	}

	if (!(strcmp(fld_store[MI_FLAG], *ref)))
    {
		/* got reference record */
        if ( move_file_cursor(NEXT, rec_store, fld_store, handle, index,    \
                                                        fld_store ) == ERROR)
		{
            sprintf ( Mnt_mnt_err_mesg, "Could not find matching index type \
[%d]", type );

            Mnt_LogMaintError ( _ID("delete_mindex"), SDB_GENERAL_ERROR,    \
                                                ERROR, Mnt_mnt_err_mesg );
            return(ERROR);
		}
    }

	/* update the index */
	strcpy(fld_store[MI_FLAG], (char *)*empty);

	/* update the reference record */

	if (update_file(RECMOD, fld_store, handle, index) == ERROR)
	{
            sprintf ( Mnt_mnt_err_mesg, "Deleted index type [%d] slot could \
not be emptied", type );

        Mnt_LogMaintError ( _ID("delete_mindex"), SDB_GENERAL_ERROR, ERROR, \
                                                        Mnt_mnt_err_mesg );
		return(ERROR);
	}

	close_database(handle); 

	return(TRUE);
}

/* 	Function:	read_record()
    Description:    general purpose function to read a record from a file
	Inputs:		what - the file to read from
			indexptr - the key contents to find a match on
			rec_store - storage for the record
			fld_store - storage for field pointers
			key - the key no to read on
	Outputs:	rec_store - the record read
			fld_store - field pointers
	Return:		TRUE - record found
			FALSE - record not found or error
*/

PUBLIC int read_record(int what, char **indexptr, char *rec_store, char **fld_store, char *key)
{
	Db_Obj		*handle;
	Index_Obj	*index;
	int		status;

	what--;
	if ((handle = open_database(what)) == (Db_Obj *)NULL)
		return(FALSE);
	if ((index = get_index_handle(handle, key)) == (Index_Obj *)NULL)
		return(FALSE);

	status = move_file_cursor(FIND, rec_store, fld_store, handle, index, indexptr);

	close_database(handle); 
	if (status != OK)
		return(FALSE);
	return(TRUE);
}

/*
 	Function:	record_count()
    Description:    determines how many records are in a particular file
	Inputs:		handle - database handle
	Return:		the total record count for handle
*/

PUBLIC int record_count(Db_Obj *handle)
{
	Index_Obj	*index;
	char		rec_store[MAX_REC_LENGTH];
	char		*fld_store[MAX_FIELDS];
	int		status;
	int		rec_count = 0;

	if ((index = get_index_handle(handle, key1)) == (Index_Obj *)NULL)
		return(FALSE);

	status = move_file_cursor(BOIND, rec_store, fld_store, handle, index, NO_ARGS);

	while (status == OK)
	{
		rec_count++;
		status = move_file_cursor(NEXT, rec_store, fld_store, handle, index, NO_ARGS);
	}

	return(rec_count);
}

PUBLIC int record_in_file(Db_Obj *handle)
{
	Index_Obj	*index;
	char		rec_store[MAX_REC_LENGTH];
	char		*fld_store[MAX_FIELDS];
	int		status;

	if ((index = get_index_handle(handle, key1)) == (Index_Obj *)NULL)
		return(FALSE);

	status = move_file_cursor(BOIND, rec_store, fld_store, handle, index, NO_ARGS);

	if (status == OK)
        return(TRUE);
    
    return(FALSE);
}

/*
 	Function:	record_count_on_key()
    Description:    Determines how many records contain a
			common field
	Inputs:		handle - database handle
			keyfield - string to compare against a field
			pos - which field number to compare
	Return:		the number of records matching keyfield at
			field position pos
*/

PUBLIC int record_count_on_keyfield(Db_Obj *handle, char *keyfield, int pos)
{
	Index_Obj	*index;
	char		rec_store[MAX_REC_LENGTH];
	char		*fld_store[MAX_FIELDS];
	int		status;
	int		rec_count = 0;

	if ((index = get_index_handle(handle, key1)) == (Index_Obj *)NULL)
		return(FALSE);

	status = move_file_cursor(BOIND, rec_store, fld_store, handle, index, NO_ARGS);

	while (status == OK)
	{
		if (!(strcmp(fld_store[pos], keyfield)))
			rec_count++;
		status = move_file_cursor(NEXT, rec_store, fld_store, handle, index, NO_ARGS);
	}

	return(rec_count);
}
