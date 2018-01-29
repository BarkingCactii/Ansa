/*
	ascii_pw.c
	
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
#include "mapopgrp.h"
#include "file_io.h"
#include "sdb_defn.h"

/* 	Function:	ascii_pw()
	Rescription:	produces the file password.dat
	Inputs:		handle - handle to file containing password records
			fp - output file
	Outputs:	None
	Return:		FALSE - database error
			>0 - number of records processed
*/

PUBLIC int ascii_pw(Db_Obj *gp_handle, FILE *fp)
{
	Index_Obj		*gp_index;
	char			rec_store[MAX_REC_LENGTH];
	char			*fld_store[MAX_FIELDS];
	int			status;
	int			no_recs = 0;
	PASSWD_REC	pword_rec;

	/* write header rec */
	write_header(fp, no_recs);

	if ((gp_index = get_index_handle(gp_handle, key2)) == (Index_Obj *)NULL)
		return(FALSE);

	/* get first record */
	status = move_file_cursor(BOIND, rec_store, fld_store, 
		gp_handle, gp_index, NO_ARGS);

	while (status == OK)
	{
		no_recs++;
		set_process_status((long)no_recs);
		if (process_interrupted() == ABORT_PROCESS)
			break;

   	memset((char *)&pword_rec, 0, sizeof(pword_rec));

		memcpy(pword_rec.pin, fld_store[PW_PIN], MAX_PIN_LEN);
		pword_rec.cntl_grp_idx = get_opgrp_index(fld_store[PW_OG_INDEX]);
		pword_rec.default_line_page = get_group_page_index(fld_store[PW_GP_INDEX]);

		fwrite((char *)&pword_rec, 1, sizeof(pword_rec), fp);

		status = move_file_cursor(NEXT, rec_store, fld_store, gp_handle, gp_index, NO_ARGS);
	}

	write_header(fp, no_recs);
	return(no_recs);
}
