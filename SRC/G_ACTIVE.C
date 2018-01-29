/*
	ascii_al.c

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
#include "mapopgrp.h"
#include "mnts_log.h"
#include "file_io.h"
#include "sdb_defn.h"
#include "report.h"
#include "dbconfig.h"

PRIVATE void make_file(char *filename, char *ext)
{
	strcpy(filename, Sdb_ascii_path);
	strcat(filename, file_attr[ACTIVE_LINE_FILE - 1].ascii_filename);
	strcat(filename, ext);
}

/* 	Function:	ascii_al()
	Rescription:	produces the file g_active.xxx where xxx
			is the controller group number
	Inputs:		handle - handle to file containing secondary assignment
				 records
			fp - output file
	Outputs:	None
	Return:		FALSE - database error
			>0 - number of records processed
*/

PUBLIC int ascii_al(Db_Obj *handle, FILE *dummy_arg)
{	
	Index_Obj	*index;
	char		rec_store[MAX_REC_LENGTH];
	char		*fld_store[MAX_FIELDS];
	int		status;
	char		previous_operator[MAX_INDEX_LEN];
	FILE		*fp = dummy_arg;	/* fix up compiler warn */
	char		filename[MAX_PATH];
    A_PARTY_REC rec;
	int		no_recs = 0;
	int		total_recs = 0;
	int 		result;
	char		extension[MAX_INDEX_LEN];
	int		total_groups;
	int		i;
   char     report_line[80];

	if ((index = get_index_handle(handle, key1)) == (Index_Obj *)NULL)
		return(FALSE);

	/* get first record */
	status = move_file_cursor(BOIND, rec_store, fld_store, 
		handle, index, NO_ARGS);

	/* create all empty g_active.xxx files first, as groups with no
	   assignments will otherwise be not created */
	total_groups = get_total_opgrps();
 	for (i = 0; i < total_groups; i++)
	{
		sprintf(extension, "%03d", i);
		make_file(filename, extension);

      sprintf(report_line, "%40s - %s", file_description[ACTIVE_LINE_FILE - 1], filename);
/*      ReportData(FILE_SUMMARY, report_line); */

      if ((fp = Gen_Open(filename, "wb")) == (FILE *)0)
		{
         Mnt_LogMaintError(_ID("ascii_al"), 1, ERROR, "Gen_Open failed _ascii_al");
			return(FALSE);
		}

		write_header(fp, 0);

      Gen_Close(fp);
	}

	fp = (FILE *)0;
	strcpy(previous_operator, "*");

	while (status == OK)
	{
		result = strcmp(fld_store[AL_OG_INDEX], previous_operator);
		if (result)
		{	
         if (fp)
         {
               write_header(fp, no_recs);
               Gen_Close(fp);
         }

         strcpy(previous_operator, fld_store[AL_OG_INDEX]);
			sprintf(extension, "%03d", get_opgrp_index(fld_store[AL_OG_INDEX]));
			make_file(filename, extension);

         sprintf(report_line, "%40s - %s", file_description[ACTIVE_LINE_FILE - 1], filename);
/*         ReportData(FILE_SUMMARY, report_line); */

         if ((fp = Gen_Open(filename, "wb")) == (FILE *)0)
			{
            Mnt_LogMaintError(_ID("ascii_al"), 1, ERROR, "FileOpen failed");
            return(FALSE);
         }

			write_header(fp, 0);
			no_recs = 0;
		}

		/* get the detailed info */
		read_record(NUMBER_FILE, &fld_store[AL_PN_INDEX], rec_store, fld_store, key1);
        rec.number = atol ( fld_store[PN_NUMBER] );
		format_label(fld_store, rec.label, PN_LABEL);
        rec.call_type = *(fld_store[PN_LINE_TYPE]);

		fwrite((char *)&rec, 1, sizeof(A_PARTY_REC), fp);
		status = move_file_cursor(NEXT, rec_store, fld_store, handle, index, NO_ARGS);
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

