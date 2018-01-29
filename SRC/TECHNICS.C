/*
	technics.c

    System  :  Train Control Command Communication System
    Author  :  Ansa Pty. Ltd. (c) 1991
    Revision:  1.0
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


/* 	Function:	make_technics()
	Rescription:	produces the file technices.dat
	Inputs:		handle - handle to file
			fp - output file
	Outputs:	None
	Return:		FALSE - database error
			>0 - number of records processed
*/

PUBLIC int make_technics(Db_Obj *handle, FILE *fp)
{
	Index_Obj	*index;
	char			rec_store[MAX_REC_LENGTH];
	char			*fld_store[MAX_FIELDS];
	int			status;
	int			no_recs = 0;
	TECHNIC_REC	rec;

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
		memset((char *)&rec, 0, sizeof(rec));

      strcpy(rec.pin, fld_store[SP_PIN]);
      strcpy(rec.name, fld_store[SP_NAME]);

		fwrite((char *)&rec, 1, sizeof(rec), fp);

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

