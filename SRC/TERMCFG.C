/*
	ascii_cc.c

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

/* 	Function:	ascii_cc()
	Rescription:	produces the file termcfg.dat 
	Inputs:		handle - handle to file containing command terminal
				 records
			fp - output file
	Outputs:	None
	Return:		FALSE - database error
			>0 - number of records processed
*/

PUBLIC int ascii_cc(Db_Obj *gp_handle, FILE *fp)
{
	Index_Obj      *gp_index;
	char           rec_store[MAX_REC_LENGTH];
	char           *fld_store[MAX_FIELDS];
	int            status;
	int            no_recs = 0;
    CTWS_CFG_REC   rec;

	write_header(fp, no_recs);

	if ((gp_index = get_index_handle(gp_handle, key2)) == (Index_Obj *)NULL)
		return(FALSE);

	/* get first record */
	status = move_file_cursor(BOIND, rec_store, fld_store, gp_handle, gp_index, NO_ARGS);

	while (status == OK)
	{
		no_recs++;
		set_process_status((long)no_recs);

		if (process_interrupted() == ABORT_PROCESS)
			break;

      memset((char *)&rec, 0, sizeof(CTWS_CFG_REC));
      memcpy(rec.designation, fld_store[CC_LABEL], sizeof(rec.designation) - 1);
      rec.ecbu_port = atoi(fld_store[CC_ECBU_PORT_NO]);

      rec.ecbu_magazine = 0;
      rec.base_acbu_extn = (unsigned long)atol(fld_store[CC_BASE_ACBU_EXT]);
      rec.monitor_flag = atoi(fld_store[CC_MONITOR_FAC]);
      rec.direct_extn = (unsigned long)atol(fld_store[CC_DDIAL_EXT]);
      rec.intrude_extn = (unsigned long)atol(fld_store[CC_INTR_EXT]);
		format_label(fld_store, (char *)rec.ct_label, CC_CT_TARGET_DESC1);

		fwrite((char *)&rec, 1, sizeof(CTWS_CFG_REC), fp);

		status = move_file_cursor(NEXT, rec_store, fld_store, gp_handle, gp_index, NO_ARGS);
	}

	write_header(fp, no_recs);
	return(no_recs);
}

