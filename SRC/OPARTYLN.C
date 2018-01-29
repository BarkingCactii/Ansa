/*
	ascii_op.c

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
#include "file_io.h"
#include "sdb_defn.h"
#include "report.h"
#include "mnts_log.h"
#include "dbconfig.h"

PRIVATE void make_file(char *filename, char *pin)
{
	strcpy(filename, Sdb_ascii_path);
	strcat(filename, file_attr[OUT_PARTY_LINE_FILE - 1].ascii_filename);
	strcat(filename, pin);
}

/* 	Function:	ascii_op()
	Rescription:	produces the file party_ln.xxx where xxx
			is the phone extension index
	Inputs:		handle - handle to file containing outgoing party
				 line records
			fp - output file
	Outputs:	None
	Return:		FALSE - database error
			>0 - number of records processed
*/

PUBLIC int ascii_op(Db_Obj *handle, FILE *dummy_arg)
{
	Index_Obj	*index;
	char		rec_store[MAX_REC_LENGTH];
	char		*fld_store[MAX_FIELDS];
	int		status;
	char		previous_phone[MAX_INDEX_LEN];
	FILE		*fp = dummy_arg;	/* fix up compiler warn */
	char		filename[MAX_PATH];
	int		no_recs = 0;
	int		total_recs = 0;
	int 		result;
	char		extension[5];
	OMNIBUS_REC	party;
   char     report_line[80];		

	if ((index = get_index_handle(handle, key2)) == (Index_Obj *)NULL)
		return(FALSE);

	/* get first record */
	status = move_file_cursor(BOIND, rec_store, fld_store, 
		handle, index, NO_ARGS);

	strcpy(previous_phone, "*");

	fp = (FILE *)0;

	while (status == OK)
	{
		result = strcmp(fld_store[OP_PN_INDEX], previous_phone);
		if (result)
		{
			if (fp)
			{
    				write_header(fp, no_recs);
                Gen_Close(fp);
			}

			strcpy(previous_phone, fld_store[OP_PN_INDEX]);
			sprintf(extension, "%03d", atoi(fld_store[OP_PN_INDEX]));
			make_file(filename, extension);

         sprintf(report_line, "%40s - %s", file_description[OUT_PARTY_LINE_FILE - 1], filename);
/*         ReportData(FILE_SUMMARY, report_line); */

         if ((fp = Gen_Open(filename, "wb")) == (FILE *)NULL)
          {
            Mnt_LogMaintError(_ID("ascii_op"), 1, ERROR, "Gen_Open failed");
				return(FALSE);
         }

			no_recs = 0;
			write_header(fp, no_recs);
		}
	
		memset((char *)&party, 0, sizeof(OMNIBUS_REC));
		strcpy(party.label, fld_store[OP_LABEL]);

		strcpy(party.code, fld_store[OP_CCIR_TONE]);
		remove_trail_spaces(party.code, sizeof(party.code));

		fwrite((char *)&party, 1, sizeof(OMNIBUS_REC), fp);
		
		status = move_file_cursor(NEXT, rec_store, 
			fld_store, handle, index, NO_ARGS);

		no_recs++;
		total_recs++;
		set_process_status((long)total_recs);

		if (process_interrupted() == ABORT_PROCESS)
			break;
	}

	if (fp)
	{
		write_header(fp, no_recs);
        Gen_Close(fp);
	}

	return(total_recs);
}


