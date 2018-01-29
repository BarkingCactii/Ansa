/*
	ascii_la.c

    System  :  Train Control Command Communication System
    Author  :  Ansa Pty. Ltd. (c) 1991
    Revision:  1.0
*/

#include <string.h>
#include <stdlib.h>

/*
#include <ibmkeys.h>
*/

#include "sysdef.h"
#include "filename.h"
#include "dbio.h"
#include "getidx.h"
#include "ascii.h"

#include "sdb_defn.h"
#include "mnts_log.h"
#include "attr_def.h"

PRIVATE int	process_status;
PRIVATE long records_processed = 0L;

PRIVATE int write_empty_line_rec(LINE_REC *empty_line_rec)
{	
    memcpy(empty_line_rec->label, DB_EMPTY_LABEL, sizeof(empty_line_rec->label));
	empty_line_rec->active_flag = FALSE;
 	empty_line_rec->attr = LINE_UNUSED_ATTR;
	return(TRUE);
}

PRIVATE int write_a_group(Db_Obj *handle, Index_Obj *index, FILE *fp, char *group_index)
{
	int		    status;
	char		*fld_store[MAX_FIELDS];
	char		*key_store[MAX_FIELDS];
	char		tmp_rec_store[MAX_REC_LENGTH];
	char		*tmp_fld_store[MAX_FIELDS];
	char		page[10];
	char		line[10];
	char		rec_store[MAX_REC_LENGTH];
	int	    	bline = 0;
	int		    bpage = 0;
	char		extension[5];
    LINE_PAGE_REC  *line_page;
    LINE_REC    *line_rec;

	key_store[0] = group_index;
	key_store[1] = page;
	key_store[2] = line;

	for (bpage = 0; bpage < NUM_OF_LINE_PAGES; bpage++)
	{
      if ((line_page = (LINE_PAGE_REC *)malloc(sizeof(LINE_PAGE_REC))) == NULL)
      {
         Mnt_LogMaintError(_ID("write_a_group"), 1, ERROR, "Malloc failure");
         return(TRUE);
      }

      memset((char *)line_page, 0, sizeof(LINE_PAGE_REC));

		for (bline = 0; bline < NUM_LINE_KEY_BUTTONS; bline++)
		{
            line_rec = &(line_page->line[bline]);

			itoa(bpage, page, 10);
			itoa(bline, line, 10);

			status = move_file_cursor(FIND, rec_store, fld_store, handle, index, key_store);

			if (status != NOTFOUND)
			{
                records_processed++;
               	set_process_status((long)records_processed); 

                if (atoi(fld_store[LA_LINE_TYPE]) == NUMBER_FILE)
                {
   		    		read_record(NUMBER_FILE, &fld_store[LA_PN_INDEX], rec_store, fld_store, key1);
   			    	strcpy(line_rec->number, fld_store[PN_NUMBER]);
    				format_label(fld_store, line_rec->label, PN_LABEL);
    				line_rec->type =  *(fld_store[PN_LINE_TYPE]);
	    			remove_trail_spaces(line_rec->number, sizeof(line_rec->number));

                    if (line_rec->type ==  OMNIBUS_CALL || line_rec->type == S_RADIO_CALL)
                    {
                        if (read_record(OUT_PARTY_LINE_FILE, &fld_store[PN_INDEX], tmp_rec_store, tmp_fld_store, key1) == OK)
	    	    		{
		    	    		strcat(line_rec->number, PARTY_EXTN_SEPARATOR);
			    	    	sprintf(extension, "%03d", atoi(tmp_fld_store[OP_PN_INDEX]));
    				    	strcat(line_rec->number, extension);
        				}       
                    }
                }
                else
                {
       				read_record(CT_COMMS_FILE, &fld_store[LA_PN_INDEX], rec_store, fld_store, key1);
   	    			strcpy(line_rec->number, fld_store[CC_DDIAL_EXT]);
    				format_label(fld_store, line_rec->label, CC_CT_TARGET_DESC1);
    				line_rec->type =  CONTROL_CALL;
	    			remove_trail_spaces(line_rec->number, sizeof(line_rec->number));
                }   

				line_rec->active_flag = FALSE;
				line_rec->attr = LINE_FREE_ATTR;

			}
			else
				write_empty_line_rec(line_rec);

			if ((process_status = process_interrupted()) == ABORT_PROCESS)
            {
                free((char *)line_page);
				return(TRUE);
            }
		}

   	fwrite((char *)line_page, 1, sizeof(LINE_PAGE_REC), fp);
    free((char *)line_page);

	}

	return(TRUE);
}

/* 	Function:	ascii_la()
	Rescription:	produces the file line_key.dat
	Inputs:		handle - handle to file containing passive line
				 assignment records
			fp - output file
	Outputs:	None
	Return:		FALSE - database error
			>0 - number of records processed
*/

PUBLIC int ascii_la(Db_Obj *la_handle, FILE *fp)
{	Db_Obj		*gp_handle;
	Index_Obj	*gp_index;
	Index_Obj	*la_index;
	char		rec_store[MAX_REC_LENGTH];
	char		*fld_store[MAX_FIELDS];
	char		**fld_ptr = fld_store;
	int		status;
	int		no_recs = 0;

#ifdef	JEFFS_HOME
	return(0);
#endif

	/* write header rec */
    write_header(fp, 0);

	/* open group file */
	if ((gp_handle = open_database(GROUP_FILE - 1)) == (Db_Obj *)NULL)
		return(FALSE);
	if ((gp_index = get_index_handle(gp_handle, key2)) == (Index_Obj *)NULL)
		return(FALSE);

	/* open line assignment index */
	if ((la_index = get_index_handle(la_handle, key2)) == (Index_Obj *)NULL)
		return(FALSE);

	/* get first record */
	status = move_file_cursor(BOIND, rec_store, fld_store, 
		gp_handle, gp_index, NO_ARGS);

   while (status == OK)
	{
   	if (process_interrupted() == ABORT_PROCESS)
         break;

		no_recs++;

/*       	set_process_status((long)records_processed); */

		write_a_group(la_handle, la_index, fp, *(fld_ptr + GP_INDEX));
		status = move_file_cursor(NEXT, rec_store, fld_store, gp_handle, gp_index, NO_ARGS);
	}

   write_header(fp, no_recs);
   close_database(gp_handle);
   return((int)records_processed);
}

