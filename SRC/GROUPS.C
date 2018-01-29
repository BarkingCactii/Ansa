/*
	ascii_og.c

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
#include "sdb_defn.h"
#include "smt_idle.h"

/* 	Function:	ascii_og()
	Rescription:	produces the file groups.dat
	Inputs:		handle - handle to file containing b party records
			fp - output file
	Outputs:	None
	Return:		FALSE - database error
			>0 - number of records processed
*/

PUBLIC int ascii_og(Db_Obj *gp_handle, FILE *fp)
{
	Index_Obj		*gp_index;
	char			rec_store[MAX_REC_LENGTH];
	char			*fld_store[MAX_FIELDS];
	int			    status;
    CNTL_GRP_LABEL	group_rec;
	int			    no_recs = 0;

	/* write header rec */
	write_header(fp, no_recs);

	if ((gp_index = get_index_handle(gp_handle, key2)) == (Index_Obj *)NULL)
		return(FALSE);

	/* get first record */
	status = move_file_cursor(BOIND, rec_store, fld_store, 
		gp_handle, gp_index, NO_ARGS);

	while (status == OK)
	{
		/* index exists */
	 	memset((char *)group_rec, 0, sizeof(CNTL_GRP_LABEL));

      if (strlen(fld_store[OG_NAME]) < KEY_LABEL_LEN)
         strcpy((char *)group_rec, fld_store[OG_NAME]);
      else
   	 	memcpy((char *)group_rec, fld_store[OG_NAME], KEY_LABEL_LEN);

	 	fwrite((char *)&group_rec, 1, sizeof(CNTL_GRP_LABEL), fp);

		status = move_file_cursor(NEXT, rec_store, fld_store, gp_handle, gp_index, NO_ARGS);
		no_recs++;

		set_process_status((long)no_recs);

		if (process_interrupted() == ABORT_PROCESS)
			break;
	}

	write_header(fp, no_recs);
	return(no_recs);
}

