/*
	ascii_gc.c

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
#include "report.h"
#include "sdb_defn.h"
#include "mnts_log.h"
#include "dbconfig.h"
#include "attr_def.h"

PRIVATE void make_file(char *filename, char *pin)
{
	strcpy(filename, Sdb_ascii_path);
	strcat(filename, file_attr[GROUP_CONF_FILE - 1].ascii_filename);
	strcat(filename, pin);
}

/* 	Function:	parse_phone_number()
	Rescription:	re-map some symbols that can appear in a
			phone number so as to provide a consistent
			interface to the conference equipment
	Inputs:		str - pointer to string (phone number) to be
			      parsed
	Outputs:	None
	Return:		None
*/

/*
PRIVATE void parse_phone_number(char *str)
{
	do 
	{
		switch (*str)
		{
			case (HASH_SYMB):
				*str = HASH_SUBST;
				break;
			case (STAR_SYMB):
				*str = STAR_SUBST;
				break;
			case (WAIT_SYMB):
				*str = WAIT_SUBST;
		}
	} while (*str++);
}
*/

/* 	Function:	ascii_gc()
	Rescription:	produces the file grp_conf.xxx where xxx
			is the PIN
	Inputs:		handle - handle to file containing group conference
				 records
			fp - output file
	Outputs:	None
	Return:		FALSE - database error
			>0 - number of records processed
*/

PUBLIC int ascii_gc(Db_Obj *handle, FILE *dummy_arg)
{	
	Index_Obj	*index;
	char		rec_store[MAX_REC_LENGTH];
	char		tmp_rec_store[MAX_REC_LENGTH];
	char		*fld_store[MAX_FIELDS];
	char		*tmp_fld_store[MAX_FIELDS];
	char		phone_store[MAX_REC_LENGTH];
	char		*phone_fld_store[MAX_FIELDS];
	int		status;
	char		previous_pin[MAX_INDEX_LEN];
	FILE		*fp = dummy_arg;	/* fix up compiler warn */
	char		filename[MAX_PATH];
	AMBA_REC	rec;
	int		no_recs = 0;
	int		total_recs = 0;
	int 		result, j;
    char     report_line[80];
    int     next_slot;
	char		extension[5];
   
	if ((index = get_index_handle(handle, key1)) == (Index_Obj *)NULL)
		return(FALSE);

	/* get first record */
    status = move_file_cursor(BOIND, rec_store, fld_store, handle, index, NO_ARGS);

	strcpy(previous_pin, "*");

	fp = (FILE *)0;

	while (status == OK)
	{
		memset(&rec, 0, sizeof(AMBA_REC));
      rec.count = 0;
   	format_label(fld_store, rec.label, GC_LABEL);

		result = strcmp(fld_store[GC_PIN_INDEX], previous_pin);
		if (result)
		{
         if (fp)
         {
   		   write_header(fp, no_recs);
            Gen_Close(fp);
         }

			strcpy(previous_pin, fld_store[GC_PIN_INDEX]);
			read_record(PASSWORD_FILE, &fld_store[GC_PIN_INDEX], phone_store, phone_fld_store, key1);
			make_file(filename, phone_fld_store[PW_PIN]);

         sprintf(report_line, "%40s - %s", file_description[GROUP_CONF_FILE - 1], filename);
/*         ReportData(FILE_SUMMARY, report_line); */

            if ((fp = Gen_Open(filename, "wb")) == (FILE *)NULL)
			{
                Mnt_LogMaintError(_ID("ascii_gc"), 1, ERROR, "Gen_Open failed");
				return(FALSE);
			}
			no_recs = 0;
			write_header(fp, no_recs);
		}


		for (next_slot = j = 0; j < MAX_AMBA_MEMBERS; j++)
		{
	        /* initialize the structure */
    	   	memset ( rec.member[j].number, 0, sizeof ( rec.member[j].number ));
            strcpy ( rec.member[j].label, EMPTY_LABEL );
    		rec.member[j].type = NO_CALL;
			rec.member[j].active_flag = FALSE;
			rec.member[j].attr = LINE_FREE_ATTR;

			/* get the detailed info */
			if (*fld_store[GC_PN_INDEX + j])
			{
                if (atoi(fld_store[START_CONF_TYPE + j]) == NUMBER_FILE)
                {
		   		    read_record(NUMBER_FILE, &fld_store[GC_PN_INDEX + j], phone_store, phone_fld_store, key1);
    			   	strcpy(rec.member[next_slot].number, phone_fld_store[PN_NUMBER]);
   	    			format_label(phone_fld_store, rec.member[next_slot].label, PN_LABEL);
   		    		rec.member[next_slot].type =  *(phone_fld_store[PN_LINE_TYPE]);
    				remove_trail_spaces(rec.member[next_slot].number, sizeof(rec.member[next_slot].number));

                    if (rec.member[next_slot].type ==  OMNIBUS_CALL || rec.member[next_slot].type == S_RADIO_CALL)
                    {
                        if (read_record(OUT_PARTY_LINE_FILE, &fld_store[GC_PN_INDEX + j], tmp_rec_store, tmp_fld_store, key1) == OK)
	        			{
		        			strcat(rec.member[next_slot].number, PARTY_EXTN_SEPARATOR);
			        		sprintf(extension, "%03d", atoi(tmp_fld_store[OP_PN_INDEX]));
				        	strcat(rec.member[next_slot].number, extension);
        				}       
                    }
                }
                else
                {
   		    	 	read_record(CT_COMMS_FILE, &fld_store[GC_PN_INDEX + j], phone_store, phone_fld_store, key1);
    	   	    	strcpy(rec.member[next_slot].number, phone_fld_store[CC_DDIAL_EXT]);
   	    		    format_label(phone_fld_store, rec.member[next_slot].label, CC_CT_TARGET_DESC1);
       		    	rec.member[next_slot].type = CONTROL_CALL;
        			remove_trail_spaces(rec.member[next_slot].number, sizeof(rec.member[next_slot].number));
                }

/*				parse_phone_number(rec.member[j].number); */
				rec.member[next_slot].active_flag = FALSE;
				rec.member[next_slot].attr = LINE_DIAL_ATTR;

				/* add party line extension to number */


/*				if (read_record(OUT_PARTY_LINE_FILE, &fld_store[PN_INDEX + j], tmp_rec_store, tmp_fld_store, key2) == OK)
				{
check					strcat(rec.member.number, PARTY_EXTN_SEPARATOR);
it					sprintf(extension, "%03d", atoi(fld_store[PN_INDEX]));
out					strcat(rec.member.number, extension);
				}
*/
                next_slot++;
				rec.count++;
			}
/*            else
          		rec.member[j].attr = UNUSED_BUTT_ATTR; */
		}

		fwrite((char *)&rec, 1, sizeof(AMBA_REC), fp);

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

