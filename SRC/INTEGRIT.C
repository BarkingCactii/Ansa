#pragma pack (1)
/*
	integrit.c

    System  :  Train Control Command Communication System
    Author  :  Ansa Pty. Ltd. (c) 1991
    Revision:  1.0
*/

#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "sysdef.h"
#include "filename.h"
#include "dbio.h"
#include "err.h"
#include "mapopgrp.h"
#include "primass.h"
#include "getidx.h"
#include "ascii.h" 
#include "report.h" 
#include "win_hues.h"
#include "sdb_defn.h"

PRIVATE void clear_messages(HWND *win, char *message)
{
	int	i;

	vratputs(*win, 0, INTEGRITY_CHK_COL, WINDOW_HILITE2, "                                    ");
	vratputs(*win, 0, INTEGRITY_CHK_COL, FIELD_ERROR, message);
	for (i = 0; i < MAX_FILES - MAX_INDEX_FILES; i++)
		vratputs(*win, i + 1, INTEGRITY_CHK_COL, WINDOW_HILITE2, "                            ");

}

/* 	Function:	all_records_defined()
	Rescription:	checks to see if there is at least one
			record in each file
	Inputs:		win - window pointer to display messages to
	Outputs:	None
	Return:		0 - test passed
			> 0 - the file index of the file not containing
			      any records
*/

PRIVATE int all_records_defined(HWND *win)
{
	Db_Obj	*db_handle;
	int	i;
	int	count;
	int	incomplete = FALSE;
	char	status;
	int	optional_file;

	/* check all files for existence except outgoing party lines */
	clear_messages(win, "Existence Check");
	for (i = 0; i < MAX_FILES - MAX_INDEX_FILES; i++)
	{
		vratputs(*win, i + 1, INTEGRITY_CHK_COL, WAIT_WINDOW, "Existence Check");

		count = 0;
		if ((db_handle = open_database(i)) != (Db_Obj *)NULL)
		{
			count = record_in_file(db_handle);
			close_database(db_handle); 
      	flush_database();    /* addition */
		}

		/* skip test for primary and secondary assignments
		   as they are now checked in detail in 
		   active_assignments_defined()
		*/
		if (i == PRIMARY_LINE_FILE - 1 ||
		    i == ACTIVE_LINE_FILE - 1)
		    	count = 1;

		if (i == GROUP_CONF_FILE - 1 ||
		    i == PARTY_LINE_FILE - 1 ||
		    i == OUT_PARTY_LINE_FILE - 1 ||
            i == D_PARTY_FILE - 1 ||
            i == REPORT_CONFIG_FILE - 1)
			optional_file = TRUE;
        else
			optional_file = FALSE;

		if (count == 0)
		{
            if (i == ACTIVE_LINE_FILE || i == OPERATOR_GROUP_FILE)
                status = '-';
            else
            {   if (!optional_file)
                    incomplete = TRUE;
                status = 'X';
            }
		}
		else
			status = 0xfb;

		vratputs(*win, i + 1, INTEGRITY_CHK_COL, WINDOW_HILITE2, "               ");
		vratputc(*win, i + 1, INTEGRITY_CHK_COL, WINDOW_HILITE2, status);

		if (optional_file)
			vratputs(*win, i + 1, INTEGRITY_CHK_COL + 2, WINDOW_HILITE2, "(Optional)");

		if (process_interrupted() == ABORT_PROCESS)
			break;
	}

	return(incomplete);
}

PRIVATE  int group_pages_assigned(HWND *main_win)
{
	Db_Obj		*handle;
	Index_Obj	*index;
	HWND		win;
	int		complete = TRUE;
	int		status;
	int		party_ln_status;
	char		tmp[80];
	int		current_row = 3;
	char		*tmp_key_store[MAX_FIELDS];
	char		tmp_rec_store[MAX_REC_LENGTH];
	char  		*tmp_fld_store[MAX_FIELDS];
	char  		rec_store[MAX_REC_LENGTH];
	char  		*fld_store[MAX_FIELDS];
	char	  	group_label[30];

	clear_messages(main_win, "Group Page Assignments");

	win = vcreat(14, 60, BOX_NORMAL, YES);
	vwind(win, 14, 60, 0, 0);
	vlocate(win, 5, 10);
	vframe(win, BOX_NORMAL, FRSINGLE);
	vratputs(win, 0, 2, BOX_NORMAL, "GROUP PAGE");
	vratputs(win, 1, 2, BOX_NORMAL, "Error Report (These Group Pages are not defined");
	vratputs(win, 2, 2, BOX_NORMAL, "              in Passive Line Assignments)");

	if ((handle = open_database(GROUP_FILE - 1)) == (Db_Obj *)NULL)
		return(FALSE);

	if ((index = get_index_handle(handle, key1)) == (Index_Obj *)NULL)
		return(FALSE);

	/* get first record */
	status = move_file_cursor(BOIND, rec_store, fld_store, handle, index, NO_ARGS);

	tmp_key_store[0] = fld_store[GP_INDEX];
	while (status == OK)
	{
    	party_ln_status = read_record(LINEASS_FILE, tmp_key_store, tmp_rec_store, tmp_fld_store, key3);
		if (!party_ln_status)
		{
			visible(win, YES, YES);
			complete = FALSE;

			strcpy(group_label, fld_store[GP_LABEL]);
			strcpy(&group_label[strlen(group_label)], fld_store[GP_LABEL+1]); 
			strcpy(&group_label[strlen(group_label)], fld_store[GP_LABEL+2]);

			sprintf(tmp, "%-28s", group_label);
   		vratputs(win, current_row++, 2, BOX_NORMAL, tmp);
         ReportData(INTEGRITY_SUMMARY, tmp);
		}

		status = move_file_cursor(NEXT, rec_store, 
			fld_store, handle, index, NO_ARGS);

		if (process_interrupted() == ABORT_PROCESS)
			break;
	}

	close_database(handle); 

/*
	if (!complete)
		show_dberr(ASSIGN_CHK_FAILED);
*/
	vdelete(win, NONE);

	return(complete);
}

PRIVATE  int selcall_exists(HWND *main_win)
{
	Db_Obj		*handle;
	Index_Obj	*index;
	HWND		win;
	int		complete = TRUE;
	int		status;
	int		party_ln_status;
	char		tmp[80];
	int		current_row = 3;
	char		*tmp_key_store[MAX_FIELDS];
	char		tmp_rec_store[MAX_REC_LENGTH];
	char  		*tmp_fld_store[MAX_FIELDS];
	char  		rec_store[MAX_REC_LENGTH];
	char  		*fld_store[MAX_FIELDS];
	char	  	extension_label[30];

	clear_messages(main_win, "Analysing Selcall Active Assignments");

	win = vcreat(14, 60, BOX_NORMAL, YES);
	vwind(win, 14, 60, 0, 0);
	vlocate(win, 5, 10);
	vframe(win, BOX_NORMAL, FRSINGLE);
	vratputs(win, 0, 2, BOX_NORMAL, "EXTENSION LABEL              NUMBER");
	vratputs(win, 1, 2, BOX_NORMAL, "Error Report (These extension are not defined in");
/*	vratputs(win, 2, 2, BOX_NORMAL, "              Selcall Line Active Assignments)"); */
	vratputs(win, 2, 2, BOX_NORMAL, "              Primary Active Assignments)");

	if ((handle = open_database(NUMBER_FILE - 1)) == (Db_Obj *)NULL)
		return(FALSE);

	if ((index = get_index_handle(handle, key1)) == (Index_Obj *)NULL)
		return(FALSE);

	/* get first record */
	status = move_file_cursor(BOIND, rec_store, fld_store, handle, index, NO_ARGS);

	tmp_key_store[0] = fld_store[PN_INDEX];
	while (status == OK)
	{
		if (*(fld_store[PN_LINE_TYPE]) == SELECTIVE_CALL)
		{
/*		 	party_ln_status = read_record(D_PARTY_FILE, tmp_key_store, tmp_rec_store, tmp_fld_store, key1);  */ /* was key 2 */
		 	party_ln_status = read_record(ACTIVE_LINE_FILE, tmp_key_store, tmp_rec_store, tmp_fld_store, key3); /* was key 2 */
			if (!party_ln_status)
			{
				visible(win, YES, YES);
				complete = FALSE;

				strcpy(extension_label, fld_store[PN_LABEL]);
				strcpy(&extension_label[strlen(extension_label)], fld_store[PN_LABEL+1]); 
				strcpy(&extension_label[strlen(extension_label)], fld_store[PN_LABEL+2]);

				sprintf(tmp, "%-28s %-30s", extension_label, fld_store[PN_NUMBER]);
				vratputs(win, current_row++, 2, BOX_NORMAL, tmp);
            ReportData(INTEGRITY_SUMMARY, tmp);
			}
		}

		status = move_file_cursor(NEXT, rec_store, 
			fld_store, handle, index, NO_ARGS);

		if (process_interrupted() == ABORT_PROCESS)
			break;
	}

	close_database(handle); 

/*	if (!complete)
		show_dberr(ASSIGN_CHK_FAILED);
*/
	vdelete(win, NONE);

	return(complete);
}

/* 	Function:	omnibus_partied()
	Rescription:	checks to see if all single channel and
			omnibus lines have been defined in 
			incoming party lines
	Inputs:		win - window pointer to display messages to
	Outputs:	None
	Return:		TRUE - passed
			FALSE = failed
*/

PRIVATE  int omnibus_partied(HWND *main_win)
{
	Db_Obj		*handle;
	Index_Obj	*index;
	HWND		win;
	int		complete = TRUE;
	int		status;
	int		party_ln_status;
	char		tmp[80];
	int		current_row = 3;
	char		*tmp_key_store[MAX_FIELDS];
	char		tmp_rec_store[MAX_REC_LENGTH];
	char  		*tmp_fld_store[MAX_FIELDS];
	char  		rec_store[MAX_REC_LENGTH];
	char  		*fld_store[MAX_FIELDS];
	char	  	extension_label[30];

	clear_messages(main_win, "Analysing Party Line Assignments");

	win = vcreat(14, 60, BOX_NORMAL, YES);
	vwind(win, 14, 60, 0, 0);
	vlocate(win, 5, 10);
	vframe(win, BOX_NORMAL, FRSINGLE);
	vratputs(win, 0, 2, BOX_NORMAL, "EXTENSION LABEL              NUMBER");
	vratputs(win, 1, 2, BOX_NORMAL, "Error Report (These extension MUST be defined in");
	vratputs(win, 2, 2, BOX_NORMAL, "              Outgoing Party Line Assignments)");

	if ((handle = open_database(NUMBER_FILE - 1)) == (Db_Obj *)NULL)
		return(FALSE);

	if ((index = get_index_handle(handle, key1)) == (Index_Obj *)NULL)
		return(FALSE);

	/* get first record */
	status = move_file_cursor(BOIND, rec_store, fld_store, handle, index, NO_ARGS);

	tmp_key_store[0] = fld_store[PN_INDEX];
	while (status == OK)
	{
		if (*(fld_store[PN_LINE_TYPE]) == OMNIBUS_CALL ||
		    *(fld_store[PN_LINE_TYPE]) == S_RADIO_CALL)
		{
		 	party_ln_status = read_record(OUT_PARTY_LINE_FILE, tmp_key_store, tmp_rec_store, tmp_fld_store, key1); /* was key 2 */
			if (!party_ln_status)
			{
				visible(win, YES, YES);
				complete = FALSE;

				strcpy(extension_label, fld_store[PN_LABEL]);
				strcpy(&extension_label[strlen(extension_label)], fld_store[PN_LABEL+1]); 
				strcpy(&extension_label[strlen(extension_label)], fld_store[PN_LABEL+2]);

				sprintf(tmp, "%-28s %-30s", extension_label, fld_store[PN_NUMBER]);
				vratputs(win, current_row++, 2, BOX_NORMAL, tmp);
            ReportData(INTEGRITY_SUMMARY, tmp);
			}
		}

		status = move_file_cursor(NEXT, rec_store, 
			fld_store, handle, index, NO_ARGS);

		if (process_interrupted() == ABORT_PROCESS)
			break;
	}

	close_database(handle); 

	if (!complete)
		show_dberr(ASSIGN_CHK_FAILED);

	vdelete(win, NONE);

	return(complete);
}

PRIVATE  int called_party_assigned(HWND *main_win)
{
	Db_Obj		*handle;
	Index_Obj	*index;
	HWND		win;
	int		complete = TRUE;
	int		status;
	int		called_party_status;
	char		tmp[80];
	int		current_row = 3;
	char		*tmp_key_store[MAX_FIELDS];
	char		tmp_rec_store[MAX_REC_LENGTH];
	char  		*tmp_fld_store[MAX_FIELDS];
	char  		rec_store[MAX_REC_LENGTH];
	char  		*fld_store[MAX_FIELDS];

	clear_messages(main_win, "Analysing Called Party Assignments");

	win = vcreat(14, 60, BOX_NORMAL, YES);
	vwind(win, 14, 60, 0, 0);
	vlocate(win, 5, 10);
	vframe(win, BOX_NORMAL, FRSINGLE);
	vratputs(win, 0, 2, BOX_NORMAL, "CONTROLLER GROUP                   ");
	vratputs(win, 1, 2, BOX_NORMAL, "Error Report (These Controller Group Labels MUST");
	vratputs(win, 2, 2, BOX_NORMAL, "              be defined in Called Party Ass.)");

	if ((handle = open_database(OPERATOR_GROUP_FILE - 1)) == (Db_Obj *)NULL)
		return(FALSE);

	if ((index = get_index_handle(handle, key1)) == (Index_Obj *)NULL)
		return(FALSE);

	/* get first record */
	status = move_file_cursor(BOIND, rec_store, fld_store, handle, index, NO_ARGS);

	tmp_key_store[0] = fld_store[OG_INDEX];
	while (status == OK)
	{
	 	called_party_status = read_record(B_PARTY_FILE, tmp_key_store, tmp_rec_store, tmp_fld_store, key1); /* was key 2 */
		if (!called_party_status)
		{
			visible(win, YES, YES);
			complete = FALSE;

			sprintf(tmp, "%-28s", fld_store[OG_NAME]);
			vratputs(win, current_row++, 2, BOX_NORMAL, tmp);
         ReportData(INTEGRITY_SUMMARY, tmp);
		}

		status = move_file_cursor(NEXT, rec_store, 
			fld_store, handle, index, NO_ARGS);

		if (process_interrupted() == ABORT_PROCESS)
			break;
	}

	close_database(handle); 

	if (!complete)
		show_dberr(ASSIGN_CHK_FAILED);

	vdelete(win, NONE);

	return(complete);
}

PRIVATE  int group_labels_grouped(HWND *main_win)
{
	Db_Obj		*handle;
	Index_Obj	*index;
	HWND		win;
	int		complete = TRUE;
	int		status;
	int		label_grouped_status;
	char		tmp[80];
	int		current_row = 3;
	char		*tmp_key_store[MAX_FIELDS];
	char		tmp_rec_store[MAX_REC_LENGTH];
	char  		*tmp_fld_store[MAX_FIELDS];
	char  		rec_store[MAX_REC_LENGTH];
	char  		*fld_store[MAX_FIELDS];

	clear_messages(main_win, "Analysing Command Terminal Grouping");

	win = vcreat(14, 60, BOX_NORMAL, YES);
	vwind(win, 14, 60, 0, 0);
	vlocate(win, 5, 10);
	vframe(win, BOX_NORMAL, FRSINGLE);
	vratputs(win, 0, 2, BOX_NORMAL, "CONTROLLER GROUP                   ");
	vratputs(win, 1, 2, BOX_NORMAL, "Error Report (These Controller Group Labels MUST");
	vratputs(win, 2, 2, BOX_NORMAL, "              be defined in Command Terminal Grouping)");

	if ((handle = open_database(OPERATOR_GROUP_FILE - 1)) == (Db_Obj *)NULL)
		return(FALSE);

	if ((index = get_index_handle(handle, key1)) == (Index_Obj *)NULL)
		return(FALSE);

	/* get first record */
	status = move_file_cursor(BOIND, rec_store, fld_store, handle, index, NO_ARGS);

	tmp_key_store[0] = fld_store[OG_INDEX];
	while (status == OK)
	{
	 	label_grouped_status = read_record(CT_CONFIG_FILE, tmp_key_store, tmp_rec_store, tmp_fld_store, key1); /* was key 2 */
		if (!label_grouped_status)
		{
			visible(win, YES, YES);
			complete = FALSE;

			sprintf(tmp, "%-28s", fld_store[OG_NAME]);
			vratputs(win, current_row++, 2, BOX_NORMAL, tmp);
         ReportData(INTEGRITY_SUMMARY, tmp);
		}

		status = move_file_cursor(NEXT, rec_store, 
			fld_store, handle, index, NO_ARGS);

		if (process_interrupted() == ABORT_PROCESS)
			break;
	}

	close_database(handle); 

	if (!complete)
		show_dberr(ASSIGN_CHK_FAILED);

	vdelete(win, NONE);

	return(complete);
}

/* 	Function:	pins_per_group()
	Rescription:	tests that there is a vaild number of PINS
			for each controller group by checking that
			there is at least as many pins as terminals
			defined for that group
	Inputs:		None
	Outputs:	None
	Return:		TRUE - test passed
			FALSE - not enough pins defined for a 
				particular controller group
*/

PRIVATE int pins_per_group(HWND *main_win)
{
	Db_Obj			*handle;
	Db_Obj			*pw_handle;
	Index_Obj		*index;
	char			*tmp_key_store[MAX_FIELDS];
	char			tmp_rec_store[MAX_REC_LENGTH];
	char			*tmp_fld_store[MAX_FIELDS];
	char			rec_store[MAX_REC_LENGTH];
	char			*fld_store[MAX_FIELDS];
	int			status;
	int			i;
	int			number_of_terminals;
	int			complete = TRUE;
	HWND			win;
	int			current_row = 2;
	int			num_pins;
	char			tmp[40];

	clear_messages(main_win, "Analysing P.I.N. \'s");

	win = vcreat(14, 60, BOX_NORMAL, YES);
	vwind(win, 14, 60, 0, 0);
	vlocate(win, 5, 10);
	vframe(win, BOX_NORMAL, FRSINGLE);
	vratputs(win, 0, 2, BOX_NORMAL, "CONTROLLER GROUP               # TERMINALS    # P.I.N. \'s");
	vratputs(win, 1, 2, BOX_NORMAL, "Error Report (P.I.N. \'s must equal or exceed # terminals)");

	if ((handle = open_database(CT_CONFIG_FILE - 1)) == (Db_Obj *)0)
		return(FALSE);

	if ((pw_handle = open_database(PASSWORD_FILE - 1)) == (Db_Obj *)0)
		return(FALSE);

	if ((index = get_index_handle(handle, key1)) == (Index_Obj *)NULL)
		return(FALSE);

	/* get first record */
	status = move_file_cursor(BOIND, rec_store, fld_store, 
		handle, index, NO_ARGS);

	while (status == OK)
	{
		/* get the controller group name and print */
		tmp_key_store[0] = fld_store[HC_OG_INDEX];
	 	read_record(OPERATOR_GROUP_FILE, tmp_key_store, tmp_rec_store, tmp_fld_store, key1);
		vratputs(win, current_row, 2, BOX_NORMAL, tmp_fld_store[OG_NAME]);

		number_of_terminals = 0;

		/* work out how many terminals in group */
		for (i = HC_START_EXT; i <= HC_END_EXT; i++)
			if (isdigit((int)*fld_store[i]))
				number_of_terminals++;

		num_pins = record_count_on_keyfield(pw_handle, fld_store[HC_OG_INDEX], PW_OG_INDEX);
        if (num_pins < number_of_terminals)
		{
			visible(win, YES, YES);
			complete = FALSE;
			sprintf(tmp, "%3d            %3d", number_of_terminals, num_pins);		
			vratputs(win, current_row++, 36, BOX_NORMAL, tmp);
         ReportData(INTEGRITY_SUMMARY, tmp);
		}

		status = move_file_cursor(NEXT, rec_store, 
			fld_store, handle, index, NO_ARGS);

		if (process_interrupted() == ABORT_PROCESS)
			break;
	}

	close_database(handle); 
	close_database(pw_handle); 

	if (!complete)
		show_dberr(PIN_CHK_FAILED);

	vdelete(win, NONE);
	return(complete);
}

/*
	Check that all active extensions have been made active for
	at least 1 controller
*/

PRIVATE int active_assignments_defined(HWND *main_win)
{
	Db_Obj	  	*handle;
	Index_Obj 	*index;
	char	  	*tmp_key_store[MAX_FIELDS];
	char	  	tmp_rec_store[MAX_REC_LENGTH];
	char	  	*tmp_fld_store[MAX_FIELDS];
	char	  	rec_store[MAX_REC_LENGTH];
	char	  	*fld_store[MAX_FIELDS];
	int	  	status;
	int	  	complete = TRUE;
	HWND	  	win;
	int	  	current_row = 2;
	char	  	tmp[80];
  	char	    	group_label[30];
	char	  	extension_label[30];
	char		current_op_group[MAX_INDEX_LEN];
	char		current_extension[MAX_INDEX_LEN];
	char		current_in_telephony[MAX_INDEX_LEN];
	char		current_config[MAX_INDEX_LEN];
	int		group_index;
	int		group_num;
	int		terminals_in_group;
	int		num_in_telephony;
	int		config_num;
	int		process_status = FALSE;

	clear_messages(main_win, "Analysing Active Assignments");

	win = vcreat(14, 70, BOX_NORMAL, YES);
	vwind(win, 14, 70, 0, 0);
	vlocate(win, 5, 5);
	vframe(win, BOX_NORMAL, FRSINGLE);
	vratputs(win, 0, 2, BOX_NORMAL, "CONTROLLER GROUP      IN TELEPHONY  EXTENSION          CONFIG");
	vratputs(win, 1, 2, BOX_NORMAL, "Error Report (These extension are un-assigned)");

	if ((handle = open_database(ACTIVE_LINE_FILE - 1)) == (Db_Obj *)0)
		return(FALSE);

	if ((index = get_index_handle(handle, key1)) == (Index_Obj *)NULL)
		return(FALSE);

	group_index = 0;
	while ((group_num = get_opgrp_item(group_index)) != ERROR)
	{
		itoa(group_num, current_op_group, 10);
		terminals_in_group = no_of_operators(current_op_group);
		
		if (terminals_in_group == 1)
		{
			/* don't need to check assignments if there
			   is only 1 terminal in a group */
			group_index++;
			continue;
		}

		strcpy(current_extension, "1");

		status = move_file_cursor(BOIND, rec_store, fld_store, handle, index, NO_ARGS);
		while (strcmp(fld_store[AL_OG_INDEX], current_op_group) && status == OK)
			status = move_file_cursor(NEXT, rec_store, fld_store, handle, index, NO_ARGS);

		while (!strcmp(fld_store[AL_OG_INDEX], current_op_group) && status == OK)
		{
			for (num_in_telephony = 2; num_in_telephony <= terminals_in_group; num_in_telephony++)
			{
				for (config_num = 1; config_num <= 5; config_num++)
				{
					if (config_num != 1)
					{
						tmp_key_store[0] = current_op_group;
						itoa(num_in_telephony, current_in_telephony, 10);
						tmp_key_store[1] = current_in_telephony;
						itoa(config_num, current_config, 10);
						tmp_key_store[2] = current_config;
			 			status = read_record(PRIMARY_LINE_FILE, tmp_key_store, tmp_rec_store, tmp_fld_store, key2);
						if (status != OK)
						{
							config_num = 6;
							continue;
						}
					}
					tmp_key_store[0] = current_op_group;
					itoa(num_in_telephony, current_in_telephony, 10);
					tmp_key_store[1] = current_in_telephony;
					itoa(config_num, current_config, 10);
					tmp_key_store[2] = current_config;
					tmp_key_store[3] = fld_store[AL_PN_INDEX];
			 		status = read_record(PRIMARY_LINE_FILE, tmp_key_store, tmp_rec_store, tmp_fld_store, key6);
					if (status != OK)
					{
						visible(win, YES, YES);
						complete = FALSE;
						find_index(NUMBER_FILE, PN_LABEL, extension_label, fld_store[AL_PN_INDEX]);
						find_index(OPERATOR_GROUP_FILE, OG_NAME, group_label, fld_store[AL_OG_INDEX]);
						sprintf(tmp, "%-28s %2d     %-20s %1d", 
							group_label, num_in_telephony, extension_label, config_num);
						vratputs(win, current_row++, 2, BOX_NORMAL, tmp);
                  ReportData(INTEGRITY_SUMMARY, tmp);
					}

					if ((process_status = process_interrupted()) == ABORT_PROCESS)
					{
						config_num = 6;
						num_in_telephony = terminals_in_group + 1;
					}
				}
			}
			status = move_file_cursor(NEXT, rec_store, fld_store, handle, index, NO_ARGS);
			if (status != OK)
				break;
			if (process_status == ABORT_PROCESS)
				break;
		}
		group_index++;

		if (process_status == ABORT_PROCESS)
			break;
	}

	close_database(handle); 

	if (!complete)
		show_dberr(ASSIGN_CHK_FAILED);

	vdelete(win, NONE);
	return(complete);
}

PRIVATE int controller_labels_defined(HWND *main_win)
{
	char	  	*tmp_key_store[MAX_FIELDS];
	char	  	tmp_rec_store[MAX_REC_LENGTH];
	char	  	*tmp_fld_store[MAX_FIELDS];
	int	  	status;
	int	  	complete = TRUE;
	HWND	  	win;
	int	  	current_row = 2;
	char	  	tmp[80];
  	char	    	group_label[30];
	char		current_op_group[MAX_INDEX_LEN];
	char		current_in_telephony[MAX_INDEX_LEN];
	char		current_config[MAX_INDEX_LEN];
	int		group_index = 0;
	int		group_num;
	int		terminals_in_group;
	int		num_in_telephony;
	int		config_num;
	int		process_status = FALSE;

	clear_messages(main_win, "Analysing Controller Labels");

	win = vcreat(14, 50, BOX_NORMAL, YES);
	vwind(win, 14, 50, 0, 0);
	vlocate(win, 5, 15);
	vframe(win, BOX_NORMAL, FRSINGLE);
	vratputs(win, 0, 2, BOX_NORMAL, "CONTROLLER GROUP      IN TELEPHONY  CONFIG");
	vratputs(win, 1, 2, BOX_NORMAL, "Error Report (No Controller Labels defined)");

	while ((group_num = get_opgrp_item(group_index)) != ERROR)
	{
		itoa(group_num, current_op_group, 10);
		terminals_in_group = no_of_operators(current_op_group);
		
		if (terminals_in_group == 1)
		{
			/* don't need to check assignments if there
			   is only 1 terminal in a group */
			group_index++;
			continue;
		}

		tmp_key_store[0] = current_op_group;
      /* only need to continue if there are any primary active assignments */
		status = read_record(ACTIVE_LINE_FILE, tmp_key_store, tmp_rec_store, tmp_fld_store, key2);
      if (status != OK)
      {
			group_index++;
			continue;
		}


		for (num_in_telephony = 2; num_in_telephony <= terminals_in_group; num_in_telephony++)
		{
			for (config_num = 1; config_num <= 5; config_num++)
			{
				if (config_num != 1)
				{
					tmp_key_store[0] = current_op_group;
					itoa(num_in_telephony, current_in_telephony, 10);
					tmp_key_store[1] = current_in_telephony;
					itoa(config_num, current_config, 10);
					tmp_key_store[2] = current_config;

		 			status = read_record(PRIMARY_LINE_FILE, tmp_key_store, tmp_rec_store, tmp_fld_store, key2);
					if (status != OK)
					{
						config_num = 6;
						continue;
					}
				}
				tmp_key_store[0] = current_op_group;
				current_in_telephony[0] = (char)(num_in_telephony + 'A');
				current_in_telephony[1] = 0;
				tmp_key_store[1] = current_in_telephony;
				itoa(config_num, current_config, 10);
				tmp_key_store[2] = current_config;
		 		status = read_record(OPERATOR_ASSIGN_FILE, tmp_key_store, tmp_rec_store, tmp_fld_store, key1);
				if (status != OK)
				{
					visible(win, YES, YES);
					complete = FALSE;
					find_index(OPERATOR_GROUP_FILE, OG_NAME, group_label, current_op_group);
					sprintf(tmp, "%-28s %2d        %1d", 
						group_label, num_in_telephony, config_num);
					vratputs(win, current_row++, 2, BOX_NORMAL, tmp);
               ReportData(INTEGRITY_SUMMARY, tmp);
				}

				if ((process_status = process_interrupted()) == ABORT_PROCESS)
				{
					config_num = 6;
					num_in_telephony = terminals_in_group + 1;
				}
			}
		}
		group_index++;

		if (process_status == ABORT_PROCESS)
			break;
	}

	if (!complete)
		show_dberr(LABELS_CHK_FAILED);

	vdelete(win, NONE);
	return(complete);
}

/* 	Function:	integrity_check()
	Rescription:	high level function to perform file integrity
			checks. This is performed just prior to producing
			a new set of CT files
	Inputs:		win - pointer to window to which messages are
			      displayed
	Outputs:	None
	Return:		TRUE = integrity test passed
			FALSE = integrity test failed
*/

PUBLIC int integrity_check(HWND *win)
{
   int status = TRUE;

   ReportIntegrityLine(ALL_RECORDS_DEFINED);
	if (all_records_defined(win) != 0)
		status = FALSE;
	flush_database();
	if (interrupt_mode == ABORT_PROCESS)
		return(TRUE);

   ReportIntegrityLine(GROUP_PAGES_ASSIGNED);
   /* group pages is only information, so don't abort */
   if (!group_pages_assigned(win))
        ;

	flush_database();
	if (interrupt_mode == ABORT_PROCESS)
		return(TRUE);

   ReportIntegrityLine(SELCALL_EXISTS);

   /* selcall is only information, so don't abort */
   if (!selcall_exists(win))
        ;

	flush_database();
	if (interrupt_mode == ABORT_PROCESS)
		return(TRUE);

   ReportIntegrityLine(OMNIBUS_PARTIED);
	if (!omnibus_partied(win))
		status = FALSE;
	flush_database();
	if (interrupt_mode == ABORT_PROCESS)
		return(TRUE);

   ReportIntegrityLine(CALLED_PARTY_ASSIGNED);
   if (!called_party_assigned(win))
		status = FALSE;
	flush_database();
	if (interrupt_mode == ABORT_PROCESS)
		return(TRUE);

   ReportIntegrityLine(GROUP_LABELS_GROUPED);
   if (!group_labels_grouped(win))
		status = FALSE;
	flush_database();
	if (interrupt_mode == ABORT_PROCESS)
		return(TRUE);

   ReportIntegrityLine(PINS_PER_GROUP);
	if (!pins_per_group(win))
		status = FALSE;
	flush_database();
	if (interrupt_mode == ABORT_PROCESS)
		return(TRUE);

   ReportIntegrityLine(ACTIVE_ASSIGNMENTS_DEFINED);
   if (!active_assignments_defined(win))
		status = FALSE;
	flush_database();
	if (interrupt_mode == ABORT_PROCESS)
		return(TRUE);

   ReportIntegrityLine(CONTROLLER_LABELS_DEFINED);
	if (!controller_labels_defined(win))
		status = FALSE;
	flush_database();

	return(status);
}


