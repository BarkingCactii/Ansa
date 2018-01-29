/*
	ascii_dp.c

    System  :  Train Control Command Communication System
    Author  :  Ansa Pty. Ltd. (c) 1991
    Revision:  1.0

    NOTE: this file produces selcall.dat and is no longer used.
    and is not part of the download anymore.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "sysdef.h"
#include "filename.h"
#include "dbio.h"
#include "ascii.h"
#include "mapopgrp.h"
#include "sdb_defn.h"

PUBLIC void extract_opgrp_from_ctnum(char *, int *, int *, char *);

/* 	Function:	ascii_dp()
	Rescription:	produces the file d_party.dat
	Inputs:		handle - handle to file containing d party records
			fp - output file
	Outputs:	None
	Return:		FALSE - database error
			>0 - number of records processed
*/

PUBLIC int ascii_dp(Db_Obj *handle, FILE *fp)
{
	Index_Obj		*index;
	char			rec_store[MAX_REC_LENGTH];
	char			*fld_store[MAX_FIELDS];
	char			tmp_rec_store[MAX_REC_LENGTH];
	char			*tmp_fld_store[MAX_FIELDS];
	int			    status;
	int			    no_recs = 0;
	ACTIVE_SC_REC	party;

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
		memset((char *)&party, 0, sizeof(party));

		read_record(NUMBER_FILE, &fld_store[DP_PN_INDEX], tmp_rec_store, tmp_fld_store, key1);
		format_label(tmp_fld_store, party.label, PN_LABEL);
      party.a_party_extn = (unsigned long)atol(tmp_fld_store[PN_NUMBER]);

      party.b_party_extn = (unsigned long)atol(fld_store[DP_CALLED_PARTY]);
      party.ct_index = get_ct_index(fld_store[DP_CC_INDEX]);
      party.status_flag = atoi(fld_store[DP_CALL_STATUS]);

		fwrite((char *)&party, 1, sizeof(party), fp);

		status = move_file_cursor(NEXT, rec_store, 
			fld_store, handle, index, NO_ARGS);
		no_recs++;
		set_process_status((long)no_recs);

		if (process_interrupted() == ABORT_PROCESS)
			break;
	}

	write_header(fp, no_recs); 
	return(no_recs);
}

