/*
	ascii_ps.c

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
#include "primass.h"
#include "mnts_log.h"
#include "file_io.h"
#include "report.h"
#include "dbconfig.h"
#include "sdb_defn.h"

PRIVATE long    start_offset_table;
PRIVATE int     max_numb_configs_for_n_ops;
PRIVATE int     default_operator;

PRIVATE void make_file(char *filename, char *ext)
{
	strcpy(filename, Sdb_ascii_path);
	strcat(filename, file_attr[PRIMARY_LINE_FILE - 1].ascii_filename);
	strcat(filename, ext);
}

PRIVATE int get_offset(char **key_name)
{
	Db_Obj		*handle;
	Index_Obj	*index;
	char		rec_store[MAX_REC_LENGTH];
	char		*fld_store[MAX_FIELDS];
	int		status;
	int		rec_offset = 0;

	if ((handle = open_database(ACTIVE_LINE_FILE - 1)) == (Db_Obj *)NULL)
		return(FALSE);

	if ((index = get_index_handle(handle, key1)) == (Index_Obj *)NULL)
		return(FALSE);

	status = move_file_cursor(FIND, rec_store, fld_store, handle, index, key_name);

	if (status != OK)
	{
		close_database(handle);
		return(FALSE);
	}

	while (status == OK && !strcmp(fld_store[AL_OG_INDEX], *key_name))
	{
		status = move_file_cursor(PREVIOUS, rec_store, fld_store, handle, index, NO_ARGS);
		rec_offset++;
	}

	close_database(handle);

	rec_offset--;
	return(rec_offset);
}

PRIVATE int write_offsets(FILE  *outfile, int which_one, long value)
{
	fseek(outfile, start_offset_table + (long)((which_one - 2) * sizeof(long)), SEEK_SET); 

	fwrite((char *)&value, 1, sizeof(long), outfile);
	fseek(outfile, 0L, SEEK_END); 

	return(TRUE);
}

PRIVATE int write_op_rec(FILE *fp, char *current_op_group, int current_assign_num, char *current_operator_num, 
    CT_OP_REC *op, int config_num)
{
    int         non_blank;
	char		*tmp_fld_store[MAX_FIELDS];
	char		*tmp_key_store[MAX_FIELDS];
	char		tmp_rec_store[MAX_REC_LENGTH];
    int         tmp_status;
	char		assign_num_str[MAX_INDEX_LEN];
	char		config_num_str[MAX_INDEX_LEN];
	
 	tmp_key_store[0] = current_op_group;
	sprintf(assign_num_str, "%c", current_assign_num + 'A');
 	tmp_key_store[1] = assign_num_str;
	sprintf(config_num_str, "%d", config_num);	
 	tmp_key_store[2] = config_num_str;

 	memset(tmp_fld_store, 0, sizeof(tmp_fld_store));
 	tmp_status = read_record(OPERATOR_ASSIGN_FILE, tmp_key_store, tmp_rec_store, tmp_fld_store, key1);

 	/* format, save and write count and label */
 	if (tmp_status)
    {
        format_label(&tmp_fld_store[OL_LABEL + ((atoi(current_operator_num) - 1) * 3)], op->label, 0);
        default_operator = atoi(tmp_fld_store[OL_DEFAULT]); /* - 1 */
        non_blank = TRUE;
        op->count = 0;
        fwrite ((void*) op, CT_OP_REC_SIZE, 1, fp);

        _DEBUG2( fprintf (stderr, "LABEL WAS NOT BLANK\n"); )
    }
 	else
    {   memcpy(op->label, DB_EMPTY_LABEL, KEY_LABEL_LEN);
        non_blank = FALSE;

        _DEBUG2( fprintf (stderr, "BLANK LABEL - did not write !!\n"); )
    }
    _DEBUG2( fprintf (stderr, "OP LABLE REC - OS %ld\n", ftell (fp)); )

    return (non_blank);
}

PRIVATE int start_new_file(FILE **fp, int op_group, int ops_in_group)
{
	char	extension[MAX_INDEX_LEN];
	char	filename[MAX_PATH];
    long    tmp = 0L;
	int	i;
   char  report_line[80];

	sprintf(extension, "%03d", op_group);
	make_file(filename, extension);

    _DEBUG2( fprintf (stderr, "CREATING .......... %s\n", filename); )

	if (*fp)
        Gen_Close(*fp);

   sprintf(report_line, "%40s - %s", file_description[PRIMARY_LINE_FILE - 1], filename);
/*   ReportData(FILE_SUMMARY, report_line); */

    if ((*fp = Gen_Open(filename, "wb")) == (FILE *)0)
	{
        Mnt_LogMaintError(_ID("start_new_file"), 1, ERROR, "Gen_Open failed" );
		return(FALSE);
	}
    rewind (*fp);
	write_header(*fp, ops_in_group);
    start_offset_table = ftell(*fp);

	/* write the offsets */
    for (i = 0; i < ops_in_group - 1; i++)
		fwrite((char *)&tmp, 1, sizeof(long), *fp);

    _DEBUG2( fprintf (stderr, "WROTE <%d> ASSIGN OFFSETS - OS %ld\n", ops_in_group-1, ftell (*fp)); )

	return(TRUE);
}

/* 	Function:	ascii_ps()
	Rescription:	produces the file g_assign.xxx where xxx
			is the controller group number
	Inputs:		handle - handle to file containing secondary active
				 assignment records
			fp - output file
	Outputs:	None
	Return:		FALSE - database error
			>0 - number of records processed
*/

PUBLIC int ascii_ps(Db_Obj *handle, FILE *dummy_arg)
{	
	Index_Obj	*index;
	char		rec_store[MAX_REC_LENGTH];
	char		*fld_store[MAX_FIELDS];
	char		*key_store[MAX_FIELDS];
	char		*tmp_key_store[MAX_FIELDS];
    int         status;
	char		current_op_group[MAX_INDEX_LEN];
	char		current_assign_num[MAX_INDEX_LEN];
	char		current_operator_num[MAX_INDEX_LEN];
	char		current_config_num[MAX_INDEX_LEN];
	FILE		*fp = dummy_arg;	/* fix up compiler warn */
    int         total_recs = 0;
    int         assign_num;
    int         index_to_active;
    int         operator_num;
    int         config_num;
    long        assign_offset;
    long        config_offset;
    CFG_HDR     cfg;
    CT_OP_REC   op;
    int         group_index;
    int         group_num;
    int         terminals_in_group;
    int         process_status;
    int         break_status;

	/* Key 3 will read in the following order:
		controller group
		assignment number
		configuration number
		operator number
	*/

	if ((index = get_index_handle(handle, key3)) == (Index_Obj *)NULL)
		return(FALSE);

	/* set up key */
	key_store[0] = current_op_group;
	key_store[1] = current_assign_num;
	key_store[2] = current_config_num;
	key_store[3] = current_operator_num;

	/* work on a group at a time */
	fp = (FILE *)0;
	group_index = 0;

    while ((group_num = get_opgrp_item(group_index)) != ERROR)
	{
		itoa(group_num, current_op_group, 10);

		/* get no of terminals for group */
		terminals_in_group = no_of_operators(current_op_group);

		/* open file and write header */
        start_new_file(&fp, group_index, terminals_in_group);

        for (assign_num = 2; assign_num <= terminals_in_group; assign_num++)
		{
            break_status = FALSE;

            itoa(assign_num, current_assign_num, 10);
            cfg.unknowns = cfg.count = max_numb_configs_for_n_ops = 0;

            /* save file pointer position to re-write config # */
            assign_offset = ftell(fp);

            /* write offset for assign num */
            write_offsets(fp, assign_num, assign_offset);

            _DEBUG2( fprintf (stderr, "START (HDR) OFFSET (%d operators) : %ld      ", assign_num, assign_offset); )

            /* write config # and unknown */
            fwrite ((void*) &cfg, CFG_HDR_SIZE, 1, fp);

            _DEBUG2( fprintf (stderr, "CFG HDR <%d %d> OS %ld\n", cfg.count, cfg.unknowns, ftell (fp)); )

            for (config_num = 1; config_num <= 5 && break_status == FALSE; config_num++)
			{
                _DEBUG2( fprintf (stderr, "STARTING CONFIG # %d\n", config_num); )

                itoa(config_num, current_config_num, 10);

                for (operator_num = 1; operator_num <= assign_num; operator_num++)
				{
					itoa(operator_num, current_operator_num, 10);
                    config_offset = ftell(fp);

                    _DEBUG2( fprintf (stderr, "OPER: <%d> ASSIGNMENT STARTED - return OF %ld\n", operator_num, config_offset); )

                    if (write_op_rec(fp, current_op_group, assign_num, current_operator_num, &op, config_num) == FALSE && operator_num == 1)
                    {
                        _DEBUG2( fprintf (stderr, "HAVE COME TO END OF CONFIGS <%d>\n", config_num-1); )
                        --config_num;
                        break_status = TRUE;
                        break;
                    }
					status = move_file_cursor(FIND, rec_store, fld_store, handle, index, key_store);

                    while (status == OK)
					{
						if (atoi(fld_store[PS_OG_INDEX]) != group_num)
						{
                            break_status = TRUE;
							break;
						}
						if (atoi(fld_store[PS_ASSIGN_NUM]) != assign_num)
						{
							break_status = TRUE;
							break;
						}
						if (atoi(fld_store[PS_CONFIG_NUM]) != config_num)
							break;
						if (atoi(fld_store[PS_OPERATOR_NUM]) != operator_num)
							break;

						/* we have a match so now we read the active file */
 						tmp_key_store[0] = fld_store[PS_OG_INDEX];
					 	tmp_key_store[1] = fld_store[PS_PN_INDEX];
						index_to_active = get_offset(tmp_key_store);
						fwrite((char *)&index_to_active, 1, sizeof(int), fp);

                        op.count++;

						status = move_file_cursor(NEXT, rec_store, fld_store, handle, index, NO_ARGS);

						if (status != OK)
							break_status = TRUE;

						total_recs++;
						set_process_status((long)total_recs);

						if ((process_status = process_interrupted()) == ABORT_PROCESS)
							break;
					}
					/* write the count int */

                    _DEBUG2( fprintf (stderr, "OPER: <%d> ASSIGNMENT ENDED  <call count %d> - current OF %ld\n", operator_num, op.count, ftell (fp)); )

                    fseek(fp, config_offset, SEEK_SET);

                    _DEBUG2( fprintf (stderr, "JUMP BACK TO <%ld> TO UPDATE CT OP REC (count)\n", ftell (fp)); )

                    fwrite((void *) &op, 1, CT_OP_REC_SIZE, fp);
                    fseek(fp, 0L, SEEK_END);

                    _DEBUG2( fprintf (stderr, "JUMP BACK TO <%ld> EOF (should agree with ended above)\n", ftell (fp)); )
                }
				if (break_status)
				{
                    cfg.count = config_num;

                    _DEBUG2( fprintf (stderr, "BREAK STATUS WAS TRUE -- TRIALING TO END OF CONFIGS <%d>\n", config_num); )
                }
			}
            _DEBUG2( fprintf (stderr, "CONFIG: <%d> for <%d> OP'S ASSIGNED in GRP <%d> ENDED - current OF %ld\n", config_num, assign_num, group_num, ftell (fp)); )

            fseek(fp, assign_offset, SEEK_SET);

            _DEBUG2( fprintf (stderr, "CONFIG DETAILS <total: %d   default: %d> JUMP BACK TO OS %ld\n", cfg.count, cfg.unknowns, ftell (fp)); )

        cfg.unknowns = default_operator;
            fwrite ((void*) &cfg, CFG_HDR_SIZE, 1, fp);

            fseek(fp, 0L, SEEK_END);

            _DEBUG2( fprintf (stderr, "---------------- END OF ASSIGNMENT -------------- \n"); )
        }
		group_index++;

        if (process_status == ABORT_PROCESS)
			break;

        _DEBUG2( fprintf (stderr, "------------------- END OF GROUP ----------------- \n"); )
    }
    _DEBUG2( fprintf (stderr, "---------------- EXIT STATUS <%d> -------------- \n", process_status); )

	if (fp)
        Gen_Close(fp);

	return(total_recs);
}

