
/*
    dumpdata.c

    System  :  Train Control Command Communication System
    Author  :  Ansa Pty. Ltd. (c) 1991
    Revision:  1.0
*/

#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "sysdef.h"
#include "field.h"
#include "filenam.h"
#include "dbio.h"
#include "lists.h"
#include "formdef.h"
#include "smt_cfg.h"
#include "smnt_log.h"

#define SMT_DATA_PATH           "C:\\SMT\\"
#define DB_CONFIG_FILE          "C:\\SMT\\SMT.CFG"

PRIVATE SMT_CONFIG  db_config_info;
PRIVATE SMT_CONFIG *config_ptr = &db_config_info;

PRIVATE char *key_order[MAX_FILES - MAX_INDEX_FILES] =
{			/* this table dictates the ordering of
			   the records for each file */
	"Key 2",	/* group page name */
	"Key 2",	/* phone label */
	"Key 2",
	"Key 1",
	"Key 1",
	"Key 1",
	"Key 1",
	"Key 1",
	"Key 2",	/* extension */
	"Key 1",
	"Key 2",	/* PIN number */
	"Key 2",	/* operator group name */
	"Key 1",
	"Key 1",
	"Key 1",
	"Key 2"
};

PRIVATE int remove_index[MAX_FILES - MAX_INDEX_FILES] =
{		/* we will not display fields that are indexes,
		   so any files that have an index will have the
		   number stated below deducted from the total
		   number of fields for this record. Since the index
		   is always the last field, they will never be
		   displayed */
	1,	/* group page name */
	1,	/* phone label */
	0,
	0,
	0,
	0,
	0,
	0,
	1,	/* extension */
	0,
	1,	/* PIN number */
	1,	/* operator group name */
	0,
	0,
	0,
	0
};

typedef struct
{
	char	*name;
	int	file_no;
	int	start;
	int	len;
} MASTER_INDEX_INFO;

PRIVATE MASTER_INDEX_INFO	master_index[MAX_INDEX_FILES] = 
{
	"Group Page *",		GROUP_FILE,		GP_LABEL, 	3,
	"Extension Number *",	NUMBER_FILE,		PN_LABEL, 	4,
	"CT Configuration *",	CT_COMMS_FILE,		CC_LABEL, 	5,
	"P.I.N. *",		PASSWORD_FILE,		PW_PIN, 	2,
	"Controller Group *",	OPERATOR_GROUP_FILE,	OG_NAME,	1
};

#define NUM_STATIC_CHOICE_LISTS	5

typedef struct
{
	char	*name;
	char	*choice_list_value;
	char	**choice_list_desc;
} STATIC_CHOICE_LIST;

PRIVATE	STATIC_CHOICE_LIST	static_list[NUM_STATIC_CHOICE_LISTS] = 
{
    "Line Type",    line_vals,  line_names,
	"Status",	callstat_vals,	callstat_names,
	"Controllers In Telephony.",	conttele_vals,	conttele_names,
	"Configuration Number",		config_vals,	config_names,
	"Conference Label Position",	confpos_vals,	confpos_names
};

PRIVATE void display_static_list(int offset, char type)
{
	int	i = 0;

	while (static_list[offset].choice_list_value[i] != type)
		i++;
	printf("%s", *(static_list[offset].choice_list_desc + i));
}

PRIVATE int	selection_field(char *name, char *data)
{
	int	i;

	for (i = 0; i < NUM_STATIC_CHOICE_LISTS; i++)
		if (!stricmp(name, static_list[i].name))
		{
			display_static_list(i, *data);
			return(TRUE);
		}

	return(FALSE);
}

PUBLIC int display_datafile(int file_no)
{
	Db_Obj		*handle;
	Index_Obj	*index;
	char		rec_store[MAX_REC_LENGTH];
	char		*fld_store[MAX_FIELDS];
	char		tmp_store[MAX_REC_LENGTH];
	char		*tmp_fld_store[MAX_FIELDS];
	int		status;
	char		**fld_names;
	int		i;
	int		j;
	int		k;
	int		num_fields;

	/* open file */
    handle = open_database(file_no);

	/* get number of fields for each record */
	num_fields = igetfldcount(handle) - remove_index[file_no];

	/* get the field description */
	fld_names = igetfldnames(handle);

	/* order the records will be read */
	if ((index = get_index_handle(handle, key_order[file_no])) == (Index_Obj *)NULL)
		return(FALSE);

	/* get first record */
	status = move_file_cursor(BOIND, rec_store, fld_store, handle, index, NO_ARGS);

	/* process until no more records */
	while (status == OK)
	{
		/* do each record a field at a time */
		for (i = 0; i < num_fields; i++)
		{
			/* print the field name */
			printf("  %-30s ", *(fld_names+i));
			for (j = 0; j < MAX_INDEX_FILES; j++)
			{
				if (!strnicmp(*(fld_names + i), master_index[j].name, strlen(master_index[j].name)))
					/* this field points to another record type */
					break;
			}
			if (j == MAX_INDEX_FILES)
			{
				/* 'normal field ' */
				if (!selection_field(*(fld_names + i), fld_store[i]))
					printf("%s", fld_store[i]);
			}
		 	else
			{
				/* read record field points to */
				if (read_record(master_index[j].file_no, &fld_store[i], tmp_store, tmp_fld_store, key1) == OK)
			 	{
					/* print some of the contents for comparison */
					for (k = master_index[j].start; 
					  k < master_index[j].start + master_index[j].len; k++)
						printf("%s|", tmp_fld_store[k]);
				}
			}				
			printf("\n");
		}
		printf("\n");

		/* get next record */
		status = move_file_cursor(NEXT, rec_store, fld_store, handle, index, NO_ARGS);
	}

	/* close file */
	close_database(handle);

	return(TRUE);
}

PRIVATE void usage(char *program_name)
{
	int	i;

	printf("\nSYSTEM MANAGEMENT TERMINAL DUMP  v 1.0\n");
	printf("Usage: %s option\n", program_name);
	printf("where option to DUMP:\n");
	for (i = 0; i < MAX_FILES - MAX_INDEX_FILES; i++)
		printf("\t%2d. %s\n", i + 1, file_description[i]);
	printf("\t%2d. ALL files\n", i + 1);

	exit(0);
}

PRIVATE int ReadSMTConfig (void)
{
    int      result = SUCCESS;
    FILE    *cfg_fd;

    _PROC("ReadSMTConfig");                        /* DEBUG purposes */

    if ((cfg_fd = fopen (DB_CONFIG_FILE, RD_ACCESS)) == NULL)
    {
        sprintf (error_message, "SMT configuration file: %s - not found", DB_CONFIG_FILE);
        LogMaintenanceError (11, error_message);
        return (FAILURE);
    }
    rewind (cfg_fd);

    if (fscanf(cfg_fd, "%s", db_config_info.db_datapath) == ERROR)
    {
        sprintf (error_message, "SMT config file: %s - on database path name", DB_CONFIG_FILE);
        LogMaintenanceError (11, error_message);
        result = FAILURE;
    }
    else if (fscanf (cfg_fd, "%s", db_config_info.ascii_datapath) == ERROR)
    {
        sprintf (error_message, "SMT config file: %s - on ASCII path name", DB_CONFIG_FILE);
        LogMaintenanceError (11, error_message);
        result = FAILURE;
    }
    if (fclose (cfg_fd) == ERROR)
    {
        sprintf (error_message, "SMT configuration file: %s", DB_CONFIG_FILE);
        LogMaintenanceError (11, error_message);
        result = FAILURE;
    }
    return (result);

}   /* end ReadSMTConfig */

PUBLIC void main(int argc, char *argv[])
{
	int	i;
	int	j;
	time_t	the_time;
	int	option;
	int	start;
	int	end;

	if (argc < 2)
		usage(argv[0]);

	option = atoi(argv[1]);

	if (option < 1 || option > MAX_FILES - MAX_INDEX_FILES + 1)
		usage(argv[0]);

	printf("SYSTEM MANAGEMENT TERMINAL\n");
	time(&the_time);
	printf("Database Dump as at %s\n", ctime(&the_time));

	if (option == MAX_FILES - MAX_INDEX_FILES + 1)
	{
		start = 0;
		end = MAX_FILES - MAX_INDEX_FILES;
	}
	else
	{
		start = option - 1;
		end = start + 1;
	}

    if ( ReadSMTConfig ( ) != SUCCESS )
        return;

	for (i = start; i < end; i++)
	{
		printf("%s\n", file_description[i]);
				for (j = 0; j < (int)strlen(file_description[i]); j++)
			printf("%c", (char)205);
				printf("\n\n");
		display_datafile(i);
		printf("\n");
	}
	printf("END OF DUMP\n");
}



