/*
	b_party.c

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
#include "mapopgrp.h"

/* 	Function:	ascii_bp()
	Rescription:	produces the file b_party.dat
	Inputs:		handle - handle to file containing b party records
			fp - output file
	Outputs:	None
	Return:		FALSE - database error
			>0 - no of b party records processed
*/

PUBLIC int ascii_bp(Db_Obj *handle, FILE *fp)
{
	Index_Obj		*bp_index;
	char			rec_store[MAX_REC_LENGTH];
	char			*fld_store[MAX_FIELDS];
	char			*key_store[MAX_FIELDS];
	int			    status;
	int			    no_recs = 0;
	GRP_B_PARTY_REC	bparty_rec;
	int			    index = 0;
	int			    which_item = 0;
	char			index_str[MAX_INDEX_LEN];

	/* write header rec */
	write_header(fp, no_recs);

	if ((bp_index = get_index_handle(handle, key1)) == (Index_Obj *)NULL)
		return(FALSE);
	key_store[0] = index_str;
	index = get_opgrp_item(which_item++);

	while (index != ERROR)
	{
		itoa(index, index_str, 10);
		status = move_file_cursor(FIND, rec_store, fld_store, handle, bp_index, key_store);
		if (status == OK)
		{
         bparty_rec.normal_call = (unsigned long)atol(fld_store[BP_STANDARD_NUM]);
         bparty_rec.priority_call = (unsigned long)atol(fld_store[BP_PRIORITY_NUM]);
			bparty_rec.emergency_call = (unsigned long)atol(fld_store[BP_EMERGENCY_NUM]);
		}
		else
			memset(&bparty_rec, 0, sizeof(GRP_B_PARTY_REC));

		fwrite((char *)&bparty_rec, 1, sizeof(GRP_B_PARTY_REC), fp);

		no_recs++;
		set_process_status((long)no_recs);

		index = get_opgrp_item(which_item++);

		if (process_interrupted() == ABORT_PROCESS)
			break;
	}

	write_header(fp, no_recs);
	return(no_recs);
}

