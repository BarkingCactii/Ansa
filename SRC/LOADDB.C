#pragma pack (1)
/*
	list.c

	Reads all the records of a particular type and stores them
	in memory. This is then used to create a selectable list
	when a pop-up window is displayed
	These lists will be either group or directory lists.
	
    System  :  Train Control Command Communication System
    Author  :  Ansa Pty. Ltd. (c) 1991
    Revision:  1.0
*/

#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>

#include "sysdef.h"
#include "filename.h"
#include "dbio.h"
#include "field.h"
#include "forms.h"
#include "form_exp.h"
#include "mnts_log.h"
#include "sdb_defn.h"
#include "err.h"
#include "formdef.h"

#define MIN_CTS_PER_GROUP  2

PRIVATE int ct_assigned_selcall(char *ct_idx, int file_no)
{
	Db_Obj		*handle;
	Index_Obj	*index;
	char			rec_store[MAX_REC_LENGTH];
	char			*fld_store[MAX_FIELDS];
	char			*key_store[MAX_FIELDS];
	int			status;

	handle = dbfile_handle(file_no - 1);

	if ((index = get_index_handle(handle, key2)) == (Index_Obj *)NULL)
		return(FALSE);

	mark_current_record(file_no - 1, index);

   key_store[0] = ct_idx;
	status = move_file_cursor(FIND, rec_store, fld_store, handle, index, key_store);

	find_mark(file_no - 1, index);

   if (status == OK)
      return(FALSE);

   return(TRUE);
}

PRIVATE int selcall_assigned(char *phone_idx, int file_no)
{
	Db_Obj		*handle;
	Index_Obj	*index;
	char			rec_store[MAX_REC_LENGTH];
	char			*fld_store[MAX_FIELDS];
	char			*key_store[MAX_FIELDS];
	int			status;

	handle = dbfile_handle(file_no - 1);

	if ((index = get_index_handle(handle, key1)) == (Index_Obj *)NULL)
		return(FALSE);

	mark_current_record(file_no - 1, index);

   key_store[0] = phone_idx;
	status = move_file_cursor(FIND, rec_store, fld_store, handle, index, key_store);

	find_mark(file_no - 1, index);

   if (status == OK)
      return(FALSE);

   return(TRUE);
}

PRIVATE int ct_in_group(char *group_idx, char *ct_idx)
{
	Db_Obj	    *handle;
	Index_Obj	*index;
	char		rec_store[MAX_REC_LENGTH];
	char		**key_ptr = (char **)&rec_store[0];
	char		*fld_store[MAX_FIELDS];
	int			status;
	int			i;

	if ((handle = open_database(CT_CONFIG_FILE - 1)) == (Db_Obj *)NULL)
		return(FALSE);

	if ((index = get_index_handle(handle, key1)) == (Index_Obj *)NULL)
		return(FALSE);

	strcpy(rec_store, group_idx);
	status = move_file_cursor(FIND, rec_store, fld_store, 
	     handle, index, (char **)&key_ptr);

	close_database(handle);

	if (status != OK)
		return(FALSE);

	for (i = HC_START_EXT; i <= HC_END_EXT; i++)
      if (strcmp(fld_store[i], ct_idx) == 0)
         return(TRUE);

	return(FALSE);
}

PRIVATE int cts_assigned_to_group(char *grp_idx, int file_no)
{
	Db_Obj		*handle;
	Index_Obj	*index;
	char			rec_store[MAX_REC_LENGTH];
	char			*fld_store[MAX_FIELDS];
	char			*key_store[MAX_FIELDS];
	int			status;
   int         i;
   int         cts_in_group = 0;

	handle = open_database(file_no - 1);

	if ((index = get_index_handle(handle, key1)) == (Index_Obj *)NULL)
		return(FALSE);

   key_store[0] = grp_idx;
	status = move_file_cursor(FIND, rec_store, fld_store, handle, index, key_store);

   if (status == OK)
   {
   	for (i = HC_START_EXT; i <= HC_END_EXT; i++)
	   {
   		if (isdigit((int)*fld_store[i]))
            cts_in_group++;
   	}
   }

	close_database(handle);

   if (cts_in_group >= MIN_CTS_PER_GROUP)
      return(TRUE);

   return(FALSE);
}

PRIVATE int group_assigned(char *grp_idx, int file_no)
{
	Db_Obj		*handle;
	Index_Obj	*index;
	char			rec_store[MAX_REC_LENGTH];
	char			*fld_store[MAX_FIELDS];
	char			*key_store[MAX_FIELDS];
	int			status;

	handle = dbfile_handle(file_no - 1);

	if ((index = get_index_handle(handle, key1)) == (Index_Obj *)NULL)
		return(FALSE);

	mark_current_record(file_no - 1, index);

   key_store[0] = grp_idx;
	status = move_file_cursor(FIND, rec_store, fld_store, handle, index, key_store);

	find_mark(file_no - 1, index);

   if (status == OK)
      return(FALSE);

   return(TRUE);
}

/* 	Function:	terminal_already_grouped()
	Rescription:	determines whether a command terminal belongs
			to a controller group
	Inputs:		term_index - the command terminal index
	Outputs:	None
	Return:		FALSE - terminal exists in a group
			TRUE - terminal not assigned to any group
*/

PRIVATE int terminal_already_grouped(char *term_index)
{
	Db_Obj			*handle;
	Index_Obj		*index;
	char			rec_store[MAX_REC_LENGTH];
	char			*fld_store[MAX_FIELDS];
	int			status;
	int			i;
	int			terminal_in_group = TRUE;
    int         current_idx;
    int         file_idx;

    current_idx = atoi ( field[HC_OG_INDEX] );

	handle = dbfile_handle(CT_CONFIG_FILE - 1);

	if ((index = get_index_handle(handle, display_order[CT_CONFIG_FILE - 1])) == (Index_Obj *)NULL)
		return(FALSE);

	mark_current_record(CT_CONFIG_FILE - 1, index);

	/* get first record */
	status = move_file_cursor(BOIND, rec_store, fld_store, 
		handle, index, NO_ARGS);

	while ((status == OK) && terminal_in_group)
	{
        file_idx = atoi ( fld_store[HC_OG_INDEX] );

        if ( file_idx == current_idx )
        {
            /*
             *      we are looking at the record we are modifying
             */

    		for ( i = HC_START_EXT; i <= HC_END_EXT; i++ )
	    	{
    			if ( isdigit (( int ) *field[i] ))
	    		{
		    		if ( !strcmp ( term_index, field[i] ))
			    		terminal_in_group = FALSE;
    			}
    		}
        }
        else
        {
		/* work out how many terminals in group */
		for (i = HC_START_EXT; i <= HC_END_EXT; i++)
		{
			if (isdigit((int)*fld_store[i]))
			{
				if (!strcmp(term_index, fld_store[i]))
					terminal_in_group = FALSE;
			}
		}
        }
		status = move_file_cursor(NEXT, rec_store, 
			fld_store, handle, index, NO_ARGS);
	}

	find_mark(CT_CONFIG_FILE - 1, index);

	return(terminal_in_group);
}

/* 	Functions 	get_file_sz()
	Rescription: 	finds the file size in bytes
	Inputs:		what - index as to what file to open
	Outputs:	None
	Return:		file size (this is represented by no of records *
				max record size)
			FALSE - error
*/

PRIVATE unsigned long get_file_size(int what_file)
{
	Db_Obj		*handle;
	Index_Obj	*index;
	int		status;
	char		rec_store[MAX_REC_LENGTH];
	char		*fld_store[MAX_FIELDS];
	unsigned long	size = 0L;

	if ((handle = open_database(what_file - 1)) == (Db_Obj *)NULL)
		return(FALSE);

	if ((index = get_index_handle(handle, key1)) == (Index_Obj *)NULL)
		return(FALSE);

	status = move_file_cursor(BOIND, rec_store, fld_store, 
		handle, index, NO_ARGS);

	while (status == OK)
	{
		size += MAX_FIELD_LEN;

		status = move_file_cursor(NEXT, rec_store, 
			fld_store, handle, index, NO_ARGS);
	}
	close_database(handle); 
	return(size);
}

/* 	Functions 	load_file()
	Rescription: 	loads the specified file into memory
			memory is malloced for the size of the file
			It is up to the calling function to free the
			memory when not required any more
	Inputs:		what - index as to what file to open
			ptr - address of a pointer
	Outputs:	ptr - address of a pointer which will point to
				the first record of the file in memory
	Return:		number of records read, or
			FALSE - error
*/

PRIVATE char	*hptr[MAX_FILES - MAX_INDEX_FILES] = 
{
    (char *)NULL, (char *)NULL, (char *)NULL, (char *)NULL, (char *)NULL,
    (char *)NULL, (char *)NULL, (char *)NULL, (char *)NULL, (char *)NULL,
    (char *)NULL, (char *)NULL, (char *)NULL, (char *)NULL, (char *)NULL,
    (char *)NULL, (char *)NULL, (char *)NULL, (char *)NULL
};

PRIVATE char	**hptrptr[MAX_FILES - MAX_INDEX_FILES] =
{
    (char *)NULL, (char *)NULL, (char *)NULL, (char *)NULL, (char *)NULL,
    (char *)NULL, (char *)NULL, (char *)NULL, (char *)NULL, (char *)NULL,
    (char *)NULL, (char *)NULL, (char *)NULL, (char *)NULL, (char *)NULL,
    (char *)NULL, (char *)NULL, (char *)NULL, (char *)NULL
};

/* PRIVATE int	previous_what = -1; */
PRIVATE int	previous_mask[MAX_FILES - MAX_INDEX_FILES] = 
{
    -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1
};

PRIVATE long	previous_no_recs[MAX_FILES - MAX_INDEX_FILES] = 
{
    0L, 0L, 0L, 0L, 0L, 0L, 0L,
    0L, 0L, 0L, 0L, 0L, 0L, 0L,
    0L, 0L, 0L, 0L, 0L
};

PUBLIC void release_choice_list(int which_file)
{
	if (hptr[which_file])
	{
		hfree((void huge *)hptr[which_file]);
		hptr[which_file] = (char *)0;
	}
	if (hptrptr[which_file])
	{
		hfree((void huge *)hptrptr[which_file]);
		hptrptr[which_file] = (char **)0;
	}
/*	previous_what = -1; */
	previous_mask[which_file] = -1;
}

PUBLIC int load_file ( int what, char **ptr, char ***pptr, int mask )
{
    Db_Obj      *handle;
    Index_Obj   *index;
    int         status;
	char		rec_store[MAX_REC_LENGTH];
	char		*fld_store[MAX_FIELDS];
	char		*data;
	long		rec_sz;
	long		file_sz;
	long		no_recs;
	char		**dataptr;
	int		add_to_list;
	char		line_type;
	long		halloc_sz;
   int      label_fld = 0;

/*	if (what == previous_what && mask == previous_mask && mask != GROUP_CONF_FILE) */
/*	if (what == previous_what && mask == previous_mask) */

    /*
     *      selecting terminals in command terminal grouping is 
     *      never buffered
     */

    if ( what == CT_COMMS_FILE && mask == TRUE )
        release_choice_list ( what - 1 );

	if (mask == previous_mask[what - 1])
	{
		*ptr = hptr[what - 1];
		*pptr = hptrptr[what - 1];
		return((int)previous_no_recs[what - 1]);
	}
	else
		release_choice_list(what - 1);

    display_wait();

	*ptr = 0;
	*pptr = 0;

	if (!(file_sz = get_file_size(what)))
    {
        drop_wait();
		return(FALSE);
    }
	
	if ((handle = open_database(what - 1)) == (Db_Obj *)NULL)
    {
        drop_wait();
		return(FALSE);
    }

	if ((index = get_index_handle(handle, display_order[what - 1])) == (Index_Obj *)NULL)
    {
        drop_wait();
		return(FALSE);
    }

	rec_sz = MAX_FIELD_LEN;
	no_recs = file_sz / rec_sz;

	halloc_sz = no_recs + 2;

	if ((dataptr = (char **)halloc(halloc_sz, sizeof(char *))) == NULL)
	{
        Mnt_LogMaintError(_ID("load_file"), 1, ERROR, "malloc failure");
        drop_wait();
		return(FALSE);
	}

	halloc_sz = file_sz + ((MARGIN + 2) * no_recs);
	if ((data = (char *)halloc(halloc_sz, sizeof(char))) == (char *)NULL)
	{
		hfree((void huge *)dataptr);	
        Mnt_LogMaintError(_ID("load_file"), 1, ERROR, "malloc failure");
        drop_wait();
		return(FALSE);
	}

/*   dataptr = (char **)pp;
   data = p;               /* JH TEST */
	
	/* save halloced pointers */
	*ptr = hptr[what - 1] = data;
	*pptr = hptrptr[what - 1] = dataptr;
/*	previous_what = what; */
	previous_mask[what - 1] = mask;

	no_recs = 0L;

	status = move_file_cursor(BOIND, rec_store, fld_store, 
		handle, index, NO_ARGS);

	while (status == OK)
	{
		/* since a key label is actually 3 fields we
		   join them here so they can be displayed on 
		   1 line */
		if (what == GROUP_FILE || what == NUMBER_FILE)
		{	
			strcpy(&rec_store[KEY_LABEL_COLUMNS], &rec_store[KEY_LABEL_COLUMNS+1]);
			strcpy(&rec_store[KEY_LABEL_COLUMNS*2], &rec_store[(KEY_LABEL_COLUMNS*2)+2]);
		}

      if (what == CT_COMMS_FILE && mask == LINEASS_FILE)
      {
         label_fld = CC_CT_TARGET_DESC1;
         strcpy(fld_store[CC_CT_TARGET_DESC1] + strlen(fld_store[CC_CT_TARGET_DESC1]), fld_store[CC_CT_TARGET_DESC2]);
         strcpy(fld_store[CC_CT_TARGET_DESC1] + strlen(fld_store[CC_CT_TARGET_DESC1]), fld_store[CC_CT_TARGET_DESC3]);
      }

		add_to_list = TRUE;

      if ((what == OPERATOR_GROUP_FILE) && mask)
      {
         switch(mask)
         {
            case(B_PARTY_FILE):
               /* don't show groups in called party if record already exists */
		      	add_to_list = group_assigned(fld_store[OG_INDEX], mask);
            break;
            case(CT_CONFIG_FILE):
               /* don't show groups in called party if record already exists */
		      	add_to_list = group_assigned(fld_store[OG_INDEX], mask);
            break;
            case(-CT_CONFIG_FILE):
               /* only show groups that have 2 or more terminals assigned to it */
               add_to_list = cts_assigned_to_group(fld_store[OG_INDEX], abs(mask));
            break;
         }
      }

		if ((what == NUMBER_FILE) && mask)
		{
			line_type = *fld_store[PN_LINE_TYPE];

			/* mask out un-wanted records */
			if (mask == PASSIVE_LINE)
			{
                ;
            }

         if (mask == ACTIVE_LINE_FILE)
         {
            if (line_type != INTERNAL_CALL && line_type != SELECTIVE_CALL)
               add_to_list = FALSE;
         }

			if (mask == D_PARTY_FILE)
			{
            if (line_type != SELECTIVE_CALL)
/*				if (line_type != DUMMY_ADMIN_AUTO &&
 				    line_type != DUMMY_CONTROL_AUTO)
*/
					add_to_list = FALSE;
            else
		      	add_to_list = selcall_assigned(fld_store[PN_INDEX], mask);
			}


			if (mask == OUT_PARTY_LINE)			
			{
				if (line_type != OMNIBUS_CALL &&
				    line_type != S_RADIO_CALL)
					add_to_list = FALSE;
			}

			if (mask == IN_PARTY_LINE)			
			{
				if (line_type != OMNIBUS_CALL &&
                    line_type != S_RADIO_CALL /* &&
                    line_type != DUMMY_IN_OPEN_CHANNEL */)
					add_to_list = FALSE;
			}

			if (mask == GROUP_CONF_FILE)
			{
/* NOT NEEDED JH 16/1/92
				if (line_type == DUMMY_OMNIBUS && partyln_defined)
					add_to_list = FALSE;
				if (line_type == DUMMY_SINGLE_CHANNEL ||
				    line_type == DUMMY_OPEN_CHANNEL ||
				    line_type == DUMMY_IN_SINGLE_CHANNEL ||
				    line_type == DUMMY_IN_OMNIBUS)
					add_to_list = FALSE;
*/
            if (what == NUMBER_FILE) /* dont check for duplicates on terminal numbers */
            {
   				if (duplicate_phone(fld_store[PN_INDEX], GC_PN_INDEX, GC_PN_INDEX + MAX_AMBA_MEMBERS, NUMBER_FILE))
	   				add_to_list = FALSE;
            }
			}
		}

      if ((what == CT_COMMS_FILE) && mask == CT_CONFIG_FILE)
         add_to_list = ct_in_group(cf->cf->data, fld_store[CC_INDEX]);
		else 
      if ((what == CT_COMMS_FILE) && mask == D_PARTY_FILE)
         add_to_list = ct_assigned_selcall(fld_store[CC_INDEX], mask);
      else
      if ((what == CT_COMMS_FILE) && (mask != LINEASS_FILE && mask != GROUP_CONF_FILE) && mask)
      {
			add_to_list = terminal_already_grouped(fld_store[CC_INDEX]);
    }
      else
      if (what == CT_COMMS_FILE && mask == GROUP_CONF_FILE)
      {
   		if (duplicate_phone(fld_store[CC_INDEX], GC_PN_INDEX, GC_PN_INDEX + MAX_AMBA_MEMBERS, CT_COMMS_FILE))
            add_to_list = FALSE;
      }

		if (add_to_list)
		{
			*dataptr = data;
			dataptr++;
			memset(data, ' ', MARGIN);

			/* the text to appear in the selection
			   list is always the first field */
			strcpy(data + MARGIN, fld_store[label_fld]);
			data += (unsigned)rec_sz + 1 + MARGIN;
			no_recs++;
		}

		status = move_file_cursor(NEXT, rec_store, 
			fld_store, handle, index, NO_ARGS);
	}
	*dataptr = NULL;

	close_database(handle); 

	previous_no_recs[what - 1] = no_recs;

    drop_wait();

	return((int)no_recs);
}









