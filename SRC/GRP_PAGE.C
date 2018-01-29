/*
	ascii_la.c

	Produces the ascii file containing group page information

    System  :  Train Control Command Communication System
    Author  :  Ansa Pty. Ltd. (c) 1991
    Revision:  1.0
*/

#include <string.h>
#include <stdlib.h>

#include "sysdef.h"
#include "filename.h"
#include "dbio.h"
#include "getidx.h"
#include "ascii.h"
/*
#include "smt_usr.h"
#include "line_def.h"
*/
#include "sdb_defn.h"

/* Function:	   ascii_gp()
	Rescription:	produces the file grp_page.dat
	Inputs:		   handle - handle to file containing group page
				      records
			         fp - output file
	Outputs:	      None
	Return:		   FALSE - database error
			         >0 - number of records processed
*/

PUBLIC int ascii_gp(Db_Obj *handle, FILE *fp)
{	
	Index_Obj            *index;
	char                 rec_store[MAX_REC_LENGTH];
	char                 *fld_store[MAX_FIELDS];
	int                  status;
	int                  no_recs = 0;
   GRP_PAGE_LABEL       page_rec;

	/* write header rec */
	write_header(fp, no_recs);

	/* records to be read in alphabetical order */
	if ((index = get_index_handle(handle, key2)) == (Index_Obj *)NULL)
		return(FALSE);

	/* get first record */
	status = move_file_cursor(BOIND, rec_store, fld_store, 
		handle, index, NO_ARGS);

   while (status == OK)
   {
   	if (process_interrupted() == ABORT_PROCESS)
         break;

		no_recs++;
   	set_process_status((long)no_recs);

		format_label(fld_store, (char *)page_rec, GP_LABEL);
		fwrite((char *)page_rec, 1, sizeof(GRP_PAGE_LABEL), fp);

		status = move_file_cursor(NEXT, rec_store, fld_store, handle, index, NO_ARGS);
	}

	write_header(fp, no_recs);
	return(no_recs);
}

