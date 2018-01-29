#pragma pack (1)
/*
	purge.c

	Purges un-related records from the database.
	(ie. removes records which point to non existant indexes)
 	NOTE: this functinos deletes records which point to
	a base item, such as passive assignments to group pages for
	which there is a direct one to one relationship. It does not
	delete other cases such as:
		where the relationship is not exclusive. ie group
	conference to phone numbers
		indirect relationships such as secondary assignments 
	to active assignments
	There cases are handled elsewhere.

    System  :  Train Control Command Communication System
    Author  :  Ansa Pty. Ltd. (c) 1991
    Revision:  1.0
*/

#include <string.h>
#include <stdlib.h>

#include "sysdef.h"
#include "filename.h"
#include "dbio.h"
#include "mnts_log.h"

typedef struct
{
    int file_to_purge;      /* file to delete records from */
	int	file_relation;      /* the file where the delete was requested */ 
 	int	start_fld;          /* start field offset with record of dependancy */ 
	int	count;              /* number of dependancies */ 
    int mpf_determiner;     /* multi-purpose field offset to determine the
                              fields type */
    int   status;           /* is deletion allowed ? */
} FIELD_DEPENDENCY;

#define MAX_OPS_IN_CONFERENCE	19
#define MAX_OPS_IN_GROUP	10
#define	MAX_NUM_OF_DEPENDANTS	7
#define END_LIST		   -1
#define DELETABLE       1
#define NOT_DELETABLE   2

/* 	this is our table of field dependancies which contains all
	the information needed for purging fields with irrelevant
	indexes. For example, if we delete a phone extension, we look in 
	the list below and find that group conferences have fields
	dependant in phone extension. These fields start from HC_START_EXT
	for a length of MAX_OPS_IN_GROUP. Thus we will work through the
	group conference file, a record at a time, and any field which matches
	the index of the deleted phone extension will be cleared.
*/
PRIVATE FIELD_DEPENDENCY field_dependants[] = 
{
	{ CT_CONFIG_FILE,   CT_COMMS_FILE, HC_START_EXT, MAX_OPS_IN_GROUP, -1, NOT_DELETABLE },
	{ GROUP_CONF_FILE,  NUMBER_FILE, GC_PN_INDEX, MAX_OPS_IN_CONFERENCE, GC_PN_INDEX + MAX_OPS_IN_CONFERENCE, DELETABLE },
	{ GROUP_CONF_FILE,  CT_COMMS_FILE, GC_PN_INDEX, MAX_OPS_IN_CONFERENCE, GC_PN_INDEX + MAX_OPS_IN_CONFERENCE, NOT_DELETABLE }
};

#define MAX_FIELD_DEPENDANTS ( sizeof ( field_dependants ) / sizeof (FIELD_DEPENDENCY ))

/* 	Function:	remove_related_fields()
	Rescription:	clears fields within the database which
			now point to a record that doesn't exist
			(a deleted record)
	Inputs:		which_file - the file in which the record was deleted
			data - the record contents of the deleted record
			       (this is where we get the index from)
	Outputs:	None
	Return:		TRUE - ok
			FALSE - database error
*/

PRIVATE int remove_related_fields(int which_file, char *data)
{
	Db_Obj		*handle;
	Index_Obj	*index;
	char		rec_store[MAX_REC_LENGTH];
	char        *fld_store[MAX_FIELDS];
	int			status;
	int			i;
	int			fld_num;
	FIELD_DEPENDENCY	*field_info;
    int         field_count;
    
	for (i = 0; i < MAX_FIELD_DEPENDANTS; i++)
	{
		field_info = &field_dependants[i];

		if (field_info->file_relation - 1 == which_file)
		{
			if ((handle = open_database(field_info->file_to_purge - 1)) == (Db_Obj *)0)
				return(FALSE);
			if ((index = get_index_handle(handle, key1)) == (Index_Obj *)NULL)
				return(FALSE);

			status = move_file_cursor(BOIND, rec_store, fld_store, handle, index, NO_ARGS);

			while (status == OK)
			{
                field_count = field_info->count;
				for (fld_num = field_info->start_fld; fld_num < field_info->start_fld + field_info->count; fld_num++)
				{
					if (!strcmp(fld_store[fld_num], data))
					{

                        if (field_info->mpf_determiner == -1)
                        {
                            if ( field_info->status == NOT_DELETABLE )
                            {
                                /* 
                                 * can't delete the record until its field
                                 * relation is deleted first 
                                 */

                		    	close_datafile ( handle );
                                return ( FALSE );
                            }

    						*(fld_store[fld_num]) = 0;
	    					update_file(RECMOD, (char **)&fld_store, handle, index);
                            field_count--;
                        }
                        else
                        {
                            if ( atoi ( fld_store[field_info->mpf_determiner + ( fld_num - field_info->start_fld ) ] ) == field_info->file_relation )
                            {
                                if ( field_info->status == NOT_DELETABLE )
                                {
                                    /* 
                                     * can't delete the record until its field
                                     * relation is deleted first 
                                     */

                		        	close_datafile ( handle );
                                    return ( FALSE );
                                }

                                sprintf ( Mnt_mnt_err_mesg, "Purging fld [%d] from file [%s]", atoi ( fld_store[field_info->mpf_determiner] ) + fld_num, file_description[field_info->file_to_purge] );
                                Mnt_LogMaintError ( _ID("remove_related_fields"), SDB_GENERAL_ERROR, ERROR, Mnt_mnt_err_mesg );
        						*(fld_store[fld_num]) = 0;
        						*(fld_store[field_info->mpf_determiner + ( fld_num - field_info->start_fld )] ) = 0;
	        					update_file(RECMOD, (char **)&fld_store, handle, index);
                                field_count--;
                            }
                        }
					}
                    else
                    {
                        if ( *fld_store[fld_num] == 0 )
                            /* empty field */
                            field_count--;
                    }
				}
                if (field_count <= 0)
                {
                    /* all relevant fields are gone, so delete the whole record */
                    sprintf ( Mnt_mnt_err_mesg, "Purging record from file [%s]", file_description[field_info->file_to_purge] );
                    Mnt_LogMaintError ( _ID("remove_related_fields"), SDB_GENERAL_ERROR, ERROR, Mnt_mnt_err_mesg );
	        		update_file(RECDEL, (char **)NULL, handle, index);
				    status = move_file_cursor(PREVIOUS, rec_store, fld_store, handle, index, NO_ARGS);
                }

				status = move_file_cursor(NEXT, rec_store, fld_store, handle, index, NO_ARGS);
			}

			close_datafile(handle);
		}
	}
	return(TRUE);
}


typedef struct
{
 	int	file_num;
	char	*delete_key;
   int   status;
    int index_type_match;

} DEPENDENCY;

/* 	this is our table of record dependancies which contains all
	the information needed for purging records with irrelevant
	indexes. For example, if we delete a group page, we look in 
	the group of dependants for group pages and find there are 2
	entries; passive line assignments and passwords. 
	Thus we will work through both these files (in the stated key)
	and remove all records that have the same index as the
	deleted group page

   Note: NOT_DELETABLE records are placed first
*/

PRIVATE DEPENDENCY record_dependants[MAX_FILES - MAX_INDEX_FILES][MAX_NUM_OF_DEPENDANTS]  = 
{
	/* group pages */
	{{ PASSWORD_FILE,       "Key 4",    NOT_DELETABLE , -1},
	{ LINEASS_FILE, 	      "Key 3", 	DELETABLE , -1},
	{ END_LIST, 		      0,          0 , -1},
	{ END_LIST, 		      0,          0 , -1},
	{ END_LIST,      		0,          0 , -1}, 
	{ END_LIST, 	      	0,          0 , -1},
	{ END_LIST, 		      0,          0 , -1}},
     
	/* phone numbers */
	{{ LINEASS_FILE,		   "Key 1",    DELETABLE, LA_LINE_TYPE },
	{ ACTIVE_LINE_FILE,	   "Key 3",    DELETABLE , -1},
	{ PRIMARY_LINE_FILE,	"Key 4",    DELETABLE , -1},
	{ OUT_PARTY_LINE_FILE,	"Key 1",    DELETABLE , -1},
	{ D_PARTY_FILE,		   "Key 1",    DELETABLE , -1},
    { END_LIST,            0,          0 , -1},
	{ END_LIST, 		      0,          0, -1 }},

	/* passive assignments */
	{{ END_LIST,      		0,          0 , -1},
	{ END_LIST,      		0,          0 , -1},
	{ END_LIST,      		0,          0 , -1},
	{ END_LIST,      		0,          0 , -1},
	{ END_LIST,      		0,          0 , -1},
	{ END_LIST,      		0,          0 , -1},
	{ END_LIST,      		0,          0 , -1}},

	/* Called party (controller) */
	{{ END_LIST,      		0,          0 , -1},
	{ END_LIST,      		0,          0 , -1},
	{ END_LIST,      		0,          0 , -1},
	{ END_LIST,      		0,          0 , -1},
	{ END_LIST,      		0,          0 , -1},
	{ END_LIST,      		0,          0 , -1},
	{ END_LIST,      		0,          0 , -1}},

	/* Active Line Assignments */
	{{ END_LIST,      		0,          0 , -1},
	{ END_LIST,      		0,          0 , -1},
	{ END_LIST,      		0,          0 , -1},
	{ END_LIST,      		0,          0 , -1},
	{ END_LIST,      		0,          0 , -1},
	{ END_LIST,      		0,          0 , -1},
	{ END_LIST,      		0,          0 , -1}},

	/* Primary / Secondary Assignments */
	{{ END_LIST,      		0,          0 , -1},
	{ END_LIST,      		0,          0 , -1},
	{ END_LIST,      		0,          0 , -1},
	{ END_LIST,      		0,          0 , -1},
	{ END_LIST,      		0,          0 , -1},
	{ END_LIST,      		0,          0 , -1},
	{ END_LIST,      		0,          0 , -1}},

	/* Command Terminal Grouping */
	{{ ACTIVE_LINE_FILE,   "Key 2",    DELETABLE , -1},
	{ PRIMARY_LINE_FILE,   "Key 5",    DELETABLE , -1},
    { OPERATOR_ASSIGN_FILE,"Key 2",    DELETABLE , -1},
	{ END_LIST,      		0,          0 , -1},
	{ END_LIST,      		0,          0 , -1},
	{ END_LIST,      		0,          0 , -1},
	{ END_LIST,      		0,          0 , -1}},

	/* Comms System Configuration */
	{{ END_LIST,      		0,          0 , -1},
	{ END_LIST,      		0,          0 , -1},
	{ END_LIST,      		0,          0 , -1},
	{ END_LIST,      		0,          0 , -1},
	{ END_LIST,      		0,          0 , -1},
	{ END_LIST,      		0,          0 , -1},
	{ END_LIST,      		0,          0 , -1}},

	/* Command Terminal Configuration */
	{{ PARTY_LINE_FILE,	   "Key 4",    DELETABLE , -1},
	{ D_PARTY_FILE, 	   "Key 2",    DELETABLE , -1},
	{ LINEASS_FILE,		   "Key 1",    DELETABLE,  LA_LINE_TYPE },
	{ END_LIST,      		0,          0 , -1},
	{ END_LIST,      		0,          0 , -1},
	{ END_LIST,      		0,          0 , -1},
	{ END_LIST,      		0,          0 , -1}},

	/* Group Conferences */
	{{ END_LIST,      		0,          0 , -1},
	{ END_LIST,      		0,          0 , -1},
	{ END_LIST,      		0,          0 , -1},
	{ END_LIST,      		0,          0 , -1},
	{ END_LIST,      		0,          0 , -1},
	{ END_LIST,      		0,          0 , -1},
	{ END_LIST,      		0,          0 , -1}},

	/* Passwords */
	{{ GROUP_CONF_FILE,	   "Key 2",    DELETABLE , -1},
	{ END_LIST,      		0,          0 , -1},
	{ END_LIST,      		0,          0 , -1},
	{ END_LIST,      		0,          0 , -1},
	{ END_LIST,      		0,          0 , -1},
	{ END_LIST,      		0,          0 , -1},
	{ END_LIST,      		0,          0 , -1}},

	/* Operator Group Names */
	{{ PASSWORD_FILE,		   "Key 3",    NOT_DELETABLE , -1},
	{ B_PARTY_FILE, 		   "Key 1",    DELETABLE , -1},
	{ CT_CONFIG_FILE,		"Key 1",    DELETABLE , -1},
	{ OPERATOR_ASSIGN_FILE,"Key 2",    DELETABLE , -1},
    { ACTIVE_LINE_FILE,	   "Key 2",    DELETABLE , -1},
	{ PRIMARY_LINE_FILE,	"Key 5",    DELETABLE , -1},
	{ END_LIST,      		0,          0, -1 }},

	/* Operator Assignment Labels */
	{{ END_LIST,      		0,          0 , -1},
	{ END_LIST,      		0,          0 , -1},
	{ END_LIST,      		0,          0 , -1},
	{ END_LIST,      		0,          0 , -1},
	{ END_LIST,      		0,          0 , -1},
	{ END_LIST,      		0,          0 , -1},
	{ END_LIST,      		0,          0 , -1}},
 
	/* Party line */
	{{ END_LIST,      		0,          0 , -1},
	{ END_LIST,      		0,          0 , -1},
	{ END_LIST,      		0,          0 , -1},
	{ END_LIST,      		0,          0 , -1},
	{ END_LIST,      		0,          0 , -1},
	{ END_LIST,      		0,          0 , -1},
	{ END_LIST,      		0,          0 , -1}},

	/* D Party */
	{{ END_LIST,      		0,          0 , -1},
	{ END_LIST,      		0,          0 , -1},
	{ END_LIST,      		0,          0 , -1},
	{ END_LIST,      		0,          0 , -1},
	{ END_LIST,      		0,          0 , -1},
	{ END_LIST,      		0,          0 , -1},
	{ END_LIST,      		0,          0 , -1}},

	/* Outgoing Party Line */
	{{ END_LIST,      		0,          0 , -1},
	{ END_LIST,      		0,          0 , -1},
	{ END_LIST,      		0,          0 , -1},
	{ END_LIST,      		0,          0 , -1},
	{ END_LIST,      		0,          0 , -1},
	{ END_LIST,      		0,          0 , -1},
	{ END_LIST,      		0,          0 , -1}},

	/* ECBU Magazine labels */
    {{ CT_COMMS_FILE,       "Key 6",    NOT_DELETABLE , -1},
	{ END_LIST,      		0,          0 , -1},
	{ END_LIST,      		0,          0 , -1},
	{ END_LIST,      		0,          0 , -1},
	{ END_LIST,      		0,          0 , -1},
	{ END_LIST,      		0,          0 , -1},
	{ END_LIST,      		0,          0 , -1}},

	/* Service personnel passwords */
	{{ END_LIST,      		0,          0 , -1},
	{ END_LIST,      		0,          0 , -1},
	{ END_LIST,      		0,          0 , -1},
	{ END_LIST,      		0,          0 , -1},
	{ END_LIST,      		0,          0 , -1},
	{ END_LIST,      		0,          0 , -1},
	{ END_LIST,      		0,          0 , -1}},

	/* report configuration */
	{{ END_LIST,      		0,          0 , -1},
	{ END_LIST,      		0,          0 , -1},
	{ END_LIST,      		0,          0 , -1},
	{ END_LIST,      		0,          0 , -1},
	{ END_LIST,      		0,          0 , -1},
	{ END_LIST,      		0,          0 , -1},
	{ END_LIST,      		0,          0 , -1}}
};

/* 	this is a table showing the field position in each file
	of the index for which other related records will be
	compared against
*/

PRIVATE int	del_key_position[MAX_FILES - MAX_INDEX_FILES] = 
{
	{ GP_INDEX },
	{ PN_INDEX },
	{ 0 },
	{ 0 },
	{ 0 },
	{ 0 },
	{ HC_OG_INDEX },
	{ 0 },
	{ CC_INDEX },
	{ 0 },
	{ PW_INDEX },
	{ OG_INDEX },
	{ 0 },
	{ 0 },
	{ 0 },
	{ 0 },
    { EC_INDEX },
    { 0 },
    { 0 }
};

/* 	Function:	remove_related_records()
	Rescription:	deletes records within the database which
			now point to a record that doesn't exist
			(a deleted record)
	Inputs:		which_file - the file in which the record was deleted
			data - the record contents of the deleted record
			       (this is where we get the index from)
	Outputs:	None
	Return:		None
*/

PUBLIC int remove_related_records(int which_file, char **data)
{
	Db_Obj		*handle;
	Index_Obj	*index;
	DEPENDENCY	*file_info;
	char		*fld_store[MAX_FIELDS];
	char		*key_store[MAX_FIELDS];
	char		rec_store[MAX_REC_LENGTH];
	int		status;

    if ( !remove_related_fields ( which_file, *( data + del_key_position[which_file] )))
        return ( FALSE );

	file_info = &record_dependants[which_file][0];
	key_store[0] = *(data + del_key_position[which_file]);

	while (file_info->file_num != END_LIST)
	{
		handle = open_database(file_info->file_num - 1);
	 	index = get_index_handle(handle, file_info->delete_key);
     
	 	/* keep deleting records until no more matching keys */
	 	do
	 	{
			status = move_file_cursor(FIND, rec_store, fld_store, handle, index, key_store);
			if (status == OK)
             {
                if (file_info->status == NOT_DELETABLE)
                {
           		   close_datafile(handle);
                   return(FALSE);
                }
				update_file(RECDEL, (char **)0, handle, index);
             }
		}
		while (status == OK);
		
		close_datafile(handle);
		file_info++;
	}

   return(TRUE);
}


