/*
	ascii_pl.c

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
#include "ascii.h"
#include "mapopgrp.h"
#include "sdb_defn.h"

/* 	Function:	    ascii_pl()
	Rescription:	produces the file party_ln.dat
	Inputs:		    handle - handle to file containing incoming partry
				             line records
			        fp - output file
	Outputs:	    None
	Return:		    FALSE - database error
			        >0 - number of records processed
*/

PUBLIC int ascii_pl(Db_Obj *handle, FILE *fp)
{
	Index_Obj		*index;
	char			rec_store[MAX_REC_LENGTH];
	char			*fld_store[MAX_FIELDS];
	int			    status;
	int			    no_recs = 0;
    ACTIVE_PL_REC   party;

	/* write header rec */
	no_recs = 0;
	write_header(fp, no_recs); 

	if ((index = get_index_handle(handle, key1)) == (Index_Obj *)NULL)
		return(FALSE);

	/* get first record */
	status = move_file_cursor(BOIND, rec_store, fld_store, 
		handle, index, NO_ARGS);

	while (status == OK)
	{
		format_label(fld_store, (char *)party.label, PL_LABEL_1);

		party.line_type =  *(fld_store[PL_LINE_TYPE]);
        party.route_num = atoi(fld_store[PL_ROUTE_NUM]);
        party.b_party_extn = (unsigned long)atol(fld_store[PL_CALLER_P]);
        party.ct_index = get_ct_index(fld_store[PL_CC_INDEX]);
        party.status_flag = atoi(fld_store[PL_STATUS]);

		fwrite((char *)&party, 1, sizeof(party), fp);

		status = move_file_cursor(NEXT, rec_store, fld_store, handle, index, NO_ARGS);
		no_recs++;
		set_process_status((long)no_recs);

		if (process_interrupted() == ABORT_PROCESS)
			break;
	}

	write_header(fp, no_recs); 
	return(no_recs);
}

