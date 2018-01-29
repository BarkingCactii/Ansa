/*
	ascii_cg.c

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
#include "getidx.h"
#include "ascii.h"
#include "sdb_defn.h"

/* 	Function:	ascii_cg()
	Rescription:	produces the file commsys.dat
	Inputs:		handle - handle to file containing b party records
			fp - output file
	Outputs:	None
	Return:		FALSE - database error
			>0 - number of records processed
*/

PUBLIC int ascii_cg(Db_Obj *gp_handle, FILE *fp)
{
	Index_Obj		*gp_index;
	char			rec_store[MAX_REC_LENGTH];
	char			*fld_store[MAX_FIELDS];
	int			    status;
	int			    no_recs = 0;
    PABX_CFG_REC    rec;

	if ((gp_index = get_index_handle(gp_handle, key1)) == (Index_Obj *)NULL)
		return(FALSE);

	/* get first record */
	status = move_file_cursor(BOIND, rec_store, fld_store, 
		gp_handle, gp_index, NO_ARGS);

   if (status == OK)
   {
      memset((char *)&rec, 0, sizeof(rec));
      rec.min_queue_extn = atoi(fld_store[CG_MIN_QUEUE]);
      rec.max_queue_extn = atoi(fld_store[CG_MAX_QUEUE]);
      rec.pick_up_code = atoi(fld_store[CG_PICKUP]);
      rec.extn_ring_time = atoi(fld_store[CG_MAX_RING]);
      rec.call_oflow_time = atoi(fld_store[CG_IN_OFLOW]);
      rec.auto_ansa_time = atoi(fld_store[CG_AANS_DELAY]);

		fwrite((char *)&rec, 1, sizeof(rec), fp);

		no_recs++;
		set_process_status((long)no_recs);
	}

	return(no_recs);
}

