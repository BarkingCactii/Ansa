/*
	ascii.c

	High level function for	producing the flat ascii files 
	from the database for use by the command terminals

    System  :  Train Control Command Communication System
    Author  :  Ansa Pty. Ltd. (c) 1991
    Revision:  1.0
*/

#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <dos.h>

#include "sysdef.h"
#include "file_io.h"
#include "smt_idle.h"
#include "win_hues.h"
#include "mnts_log.h"
#include "sdb_defn.h"

#include "filename.h"
#include "dbio.h"
#include "mapopgrp.h"
#include "cleandir.h"
#include "integrit.h"
#include "dbconfig.h"
#include "line_key.h"
#include "grp_page.h"
#include "b_party.h"
#include "groups.h"
#include "password.h"
#include "t_matrix.h"
#include "commsys.h"
#include "termcfg.h"
#include "g_active.h"
#include "grp_conf.h"
#include "g_assign.h"
#include "party_ln.h"
#include "d_party.h"
#include "opartyln.h"
#include "err.h"
#include "technics.h"
#include "report.h"

#include "ascii.h"
 
#define NUMBER_OF_FSPECS    7

PUBLIC  int     interrupt_mode = FALSE;

PRIVATE long    percent_complete;
PRIVATE long    display_column;
PRIVATE int     file_being_processed;
PRIVATE int     current_row;

PRIVATE HWND    win;

typedef struct
{	
	int 	(*func)(Db_Obj *, FILE *);
    int     multiple_files;
	long	records_to_process;
    int     records_processed;
} MAKE_ASCII_FUNC_ARRAY;

MAKE_ASCII_FUNC_ARRAY   ascii_func[MAX_FILES - MAX_INDEX_FILES] =
{
    { ascii_gp, FALSE, 0, 0 },
    { 0,        FALSE, 0, 0 },
/*   ascii_la, FALSE, (long)(NUM_GROUP_PAGES * NUM_LINE_KEY_BUTTONS), 0,*/
    { ascii_la, FALSE, 0, 0 },
    { ascii_bp, FALSE, 0, 0 },
    { ascii_al, TRUE,  0, 0 },
    { ascii_ps, TRUE,  0, 0 },
    { ascii_hc, FALSE, 0, 0 },
    { ascii_cg, FALSE, 0, 0 },
    { ascii_cc, FALSE, 0, 0 },
    { ascii_gc, TRUE,  0, 0 },
    { ascii_pw, FALSE, 0, 0 },
    { ascii_og, FALSE, 0, 0 },
    { 0,        FALSE, 0, 0 },
    { ascii_pl, FALSE, 0, 0 },
    { 0,        FALSE, 0, 0 },
/*    { ascii_dp, FALSE, 0, 0 }, */
    { ascii_op, TRUE,  0, 0 },
    { 0,        FALSE, 0, 0 },
    { make_technics, FALSE, 0, 0 },
    { 0,        FALSE, 0, 0 }
};				  

PUBLIC void make_file_summary ( void )
{
	struct		find_t	find;
	char		path[MAX_PATH];
	char		file[MAX_PATH];
    char        report_line[255];
    int         i;
    char        *tmp;

    ReportHeader(FILE_SUMMARY);

	/* get the directory path */
	strcpy ( path, Sdb_ascii_path );
	strcat ( path, "*.*" );

	/* Find first matching file, then find additional matches. */
	if ( _dos_findfirst ( path, _A_ARCH | _A_HIDDEN, &find ))
	{
        Mnt_LogMaintError ( _ID ( "make_file_summary" ), 1, ERROR, "CT ascii file directory empty!" );
		return;
	}

    do 
    {
        strcpy ( file, Sdb_ascii_path );
        strcat ( file, find.name );
        strupr ( file );

    	for ( i = 0; i < MAX_FILES - MAX_INDEX_FILES; i++ )
        {
            if ( !strlen ( file_attr[i].ascii_filename ))
                continue;

            if ( ( tmp = strstr ( file, file_attr[i].ascii_filename )) != ( char * ) NULL )
            {
                /*
                 *  file exists, so add it to the file summary 
                 */

                 sprintf ( report_line, "%40s - %s", file_description[i], file );
                 ReportData ( FILE_SUMMARY, report_line );
                 break;
            }
        }
    }
    while ( !_dos_findnext ( &find ));
}

PUBLIC int process_interrupted ( void )
{
	HWND	win;
    int     ch;
    int     ansa = FALSE;

	if (interrupt_mode == ABORT_PROCESS)
	{
		win = vcreat(4, 44, POPUP_NORMAL, YES);
		vwind(win, 4, 44, 0, 0);
		vlocate(win, 10, 18);
		vframe(win, POPUP_NORMAL, FRDOUBLE);
        visible(win, YES, YES);
		vratputs(win, 1, 3, POPUP_NORMAL, "User abort request has been detected.");
		vratputs(win, 2, 3, POPUP_NORMAL, "  Press Y to abort or N to continue.");

        ch = Smt_KbdGet();
		ch = toupper(ch);
		vdelete(win, NONE);

		if (ch == 'Y')
            ansa = ABORT_PROCESS;
		else
			interrupt_mode = TRUE;
	}		
    return(ansa);
}

/* 	Functions 	make_flat_fn()
	Rescription: 	makes the fullpath of the designated ascii file
            ready to be opened by Gen_Open()
	Inputs:		filename - pointer to a char array large enough
				to contain the full path
	:		index - index into FILE_DETAILS from where the
				file name is obtained
	Outputs:	filename - string being the full path of the file
				to be opened
	Return:		None
*/

PRIVATE	void make_flat_fn(char *filename, int index)
{

    strcpy ( filename, Sdb_ascii_path );
    strcat ( filename, file_attr[index].ascii_filename );
	strcat(filename, ".dat");
}

/* 	Functions 	setup_files()
	Rescription: 	opens the ascii file
	Inputs:		index - index into FILE_DETAILS from where the
				file name is obtained
			fp - pointer to a file pointer (type FILE *)
	Outputs:	fp - will contain the pointer to the open file
	Return:		TRUE - operation OK
			FALSE - error
*/

PRIVATE int setup_files(int index, FILE **fp)
{
    char    filename[MAX_PATH];
    char    report_line[80];

	if (ascii_func[index].multiple_files)
		return(TRUE);

	if (strcmp(file_attr[index].ascii_filename, ""))
    {
		make_flat_fn(filename, index);
/*        sprintf(report_line, "%40s - %s", file_description[index], filename);
        ReportData(FILE_SUMMARY, report_line);
*/
    }
	else
		/* no file to produce for this data file */
		return(FALSE);

    if ((*fp = Gen_Open(filename, "wb")) == (FILE *)NULL)
	{
        sprintf ( Mnt_mnt_err_mesg, "Open failed - ascii file [%s]",        \
                                                                filename );
        Mnt_LogMaintError ( _ID("setup_files"), GEN_FILE_OPEN_ERROR, errno, \
                                                        Mnt_mnt_err_mesg );
		return(FALSE);
	}

	return(TRUE);
}

/* 	Function:	show_processing_summary()
	Rescription:	puts up a window showing a summary of
                    the record types and how many records for each
                type were processed
	Inputs:		None
	Outputs:	Mone
	Return:		None

PRIVATE void	show_processing_summary(void)
{
	HWND	summary_win;
	int	i;
	int	row;
	char	count[10];

	summary_win = vcreat(19, 70, BOX_NORMAL, YES);
	vwind(summary_win, 19, 70, 0, 0);
	vlocate(summary_win, 3, 5);
	vframe(summary_win, BOX_NORMAL, FRSINGLE);
    visible(summary_win, YES, YES);
	vratputs(summary_win, 0, 5, BOX_NORMAL, "FILE PRODUCED                           RECORDS PROCESSED");

	for (row = 2, i = 0; i < MAX_FILES - MAX_INDEX_FILES; i++)
	{
		if (ascii_func[i].func)
		{
			vratputs(summary_win, row, 5, BOX_NORMAL, file_description[i]);
            if (ascii_func[i].records_processed)
				sprintf(count, "%6d", ascii_func[i].records_processed);		
			else
				strcpy(count, "     -");
			vratputs(summary_win, row++, 56, BOX_NORMAL, count);
		}
	}
	vratputs(summary_win, row + 1, 24, MENU_SELECT, "Press a key to continue");

    Smt_KbdGet();

	vdelete(summary_win, NONE);
}
*/

/* 	Function:	set_process_limit()
	Rescription:	notes the total number of records to be
			processed for each file type. This is used
			as a base for calculating the percentage
			of records processed.
	Inputs:		which_file - the database file
			limit - total records to be processed
	Outputs:	None
	Return:		None
*/

PRIVATE void set_process_limit(int which_file, long limit)
{
	if (ascii_func[which_file - 1].records_to_process == (long)0)
		ascii_func[which_file - 1].records_to_process = limit;
    percent_complete = 0L;
    display_column = 0L;
    file_being_processed = which_file;
}

/* 	Function:	special()
	Rescription:	performs any special post processing on the
                    ascii files not already done
	Inputs:		None
	Outputs:	None
	Return:		TRUE - ok
			FALSE - error

PRIVATE int special( void )
{
	FILE	*in;
	FILE	*out;
	char	filename[MAX_PATH];
	int	ch;

	make_flat_fn(filename, CT_COMMS_FILE - 1);
    if ((in = Gen_Open(filename, "rb")) == (FILE *)NULL)
	{
        sprintf ( Mnt_mnt_err_mesg, "Open failed - ascii file [%s]",        \
                                                                filename );
        Mnt_LogMaintError ( _ID("special"), GEN_FILE_OPEN_ERROR, errno,     \
                                                        Mnt_mnt_err_mesg );
		return(FALSE);
	}

	make_flat_fn(filename, GEN_COMMS_FILE - 1);
    if ((out = Gen_Open(filename, "a+b")) == (FILE *)NULL)
	{
        sprintf ( Mnt_mnt_err_mesg, "Open failed - ascii file [%s]",        \
                                                                filename );
        Mnt_LogMaintError ( _ID("special"), GEN_FILE_OPEN_ERROR, errno,     \
                                                        Mnt_mnt_err_mesg );
        Gen_Close(in);
		return(FALSE);
	}

	fseek(out, 0L, SEEK_END);

	while ((ch = fgetc(in)) != EOF)
		fputc(ch, out);

    Gen_Close(out);
    Gen_Close(in);

	return(TRUE);
}
*/

/* 	Function:	open_status_win()
	Rescription:	places the window on screen which will
			give progress on production of ascii files
	Inputs:		rows - number of rows for the window
	Outputs:	None
	Return:		None
*/

PRIVATE void open_status_win(int rows)
{
	win = vcreat(rows, 76, WINDOW_NORMAL, YES);
	vwind(win, rows, 76, 0, 0);
	vlocate(win, ((25 - rows) / 2) + 1, 2);
	vframe(win, WINDOW_NORMAL, FRSINGLE);
    visible(win, YES, YES);
}

/* 	Functions 	make_ct_file()
	Rescription: 	High level function for producing flat ascii files
	Return:		TRUE - operation OK
			FALSE - error
*/

PUBLIC int make_ct_file()
{
    int         i;
	Db_Obj		*db_handle;
	FILE		*fp;
	char		tmp[40];
    HWND        top_win;
    HWND        bot_win;
   char         report_line[80];

	interrupt_mode = TRUE;

    top_win = vcreat ( 1, 80, MENU_NORMAL, YES );
    vwind ( top_win, 1, 80, 0, 0 );
    vlocate ( top_win, 0, 0 );
    vratputs ( top_win, 0, 1, MENU_NORMAL, "                        Create System Data Files" );

    bot_win = vcreat ( 1, -1, MENU_NORMAL, YES );
    vwind ( bot_win, 1, -1, 0, 0 );
    vlocate ( bot_win, 24, 0 );
    vratputs ( bot_win, 0, 1, MENU_NORMAL, "                     Press any key to abort download" );

    /* both both to start with */

    visible ( top_win, YES, NO );
    visible ( bot_win, YES, NO );

    open_status_win (20);

    make_file_indexes();

	vclear(win);

	for (i = 0; i < MAX_FILES - MAX_INDEX_FILES; i++)
		vratputs(win, i + 1, 2, WINDOW_NORMAL, file_description[i]);

   ReportInitialize();
   ReportHeader(INTEGRITY_SUMMARY);

	if (!integrity_check(&win))
	{
		show_dberr(INTEGRITY_CHK_FAILED);
        vdelete ( top_win, NONE );
        vdelete ( bot_win, NONE );
        vdelete ( win, NONE );
		interrupt_mode = FALSE;
        ReportClose();
        return( FALSE );
	}

	if (interrupt_mode == ABORT_PROCESS)
	{
	 	show_dberr(DOWNLOAD_ABORTED);
        vdelete ( top_win, NONE );
        vdelete ( bot_win, NONE );
		vdelete(win, NONE);
		interrupt_mode = FALSE;
        ReportClose();
        return(FALSE);
	}

	for (i = 0; i < MAX_FILES - MAX_INDEX_FILES; i++)
		vratputs(win, i + 1, INTEGRITY_CHK_COL, WINDOW_HILITE2, "                            ");

	vratputs(win, 0, INTEGRITY_CHK_COL, WINDOW_HILITE2, "                           ");
	vratputs(win, 0, INTEGRITY_CHK_COL, FIELD_ERROR, "Clearing directory");
	clean_directory ( DELETE_ALL_FILES );

	vratputs(win, 0, INTEGRITY_CHK_COL, WINDOW_HILITE2, "                           ");
	vratputs(win, 0, INTEGRITY_CHK_COL, FIELD_ERROR, "Creating Data...");

/*    ReportHeader(FILE_SUMMARY); */

	for (i = 0; i < MAX_FILES - MAX_INDEX_FILES; i++)
	{
		current_row = i + 1;

		if (!setup_files(i, &fp))
		{	/* no file specified, or file cannot be opened */
			db_handle = open_database(i);
			sprintf(tmp, "%6d records processed", record_count(db_handle));
			vratputs(win, i + 1, INTEGRITY_CHK_COL, WINDOW_HILITE2, tmp);
			close_database(db_handle); 
			continue;
		}

		flush_database();
/*
        if (i == 2)             /? IAN'S SHORT CUT !!! ?/
            continue;
*/

		db_handle = open_database(i);

		if (db_handle != (Db_Obj *)NULL)
		{	
			if (ascii_func[i].func)
			{
				vratputs(win, i + 1, INTEGRITY_CHK_COL, WINDOW_NORMAL, "北北北北北北北北北北北北北北北北");
				
				set_process_limit(i + 1, (long)record_count(db_handle));
				ascii_func[i].records_processed = (*ascii_func[i].func)(db_handle, fp);

				if (interrupt_mode == ABORT_PROCESS)
				{
					i = MAX_FILES - MAX_INDEX_FILES;
					continue;
				}
			}

			close_database(db_handle); 
			if (!ascii_func[i].multiple_files)
                Gen_Close(fp);

		}

		vratputs(win, i + 1, INTEGRITY_CHK_COL, WINDOW_HILITE2, "                                             ");
		if (ascii_func[i].records_processed)
			sprintf(tmp, "%6d records processed", ascii_func[i].records_processed);
		else
			sprintf(tmp, "     - records processed", ascii_func[i].records_processed);

/*      sprintf(report_line , "%35s - %6d records processed", file_description[i], ascii_func[i].records_processed);
      ReportData(RECORD_SUMMARY, report_line);
*/
		vratputs(win, i + 1, INTEGRITY_CHK_COL, WINDOW_HILITE2, tmp);

	}

    if (interrupt_mode == ABORT_PROCESS)
	{
	 	show_dberr(DOWNLOAD_ABORTED);
        vdelete ( top_win, NONE );
        vdelete ( bot_win, NONE );
		vdelete(win, NONE);
		interrupt_mode = FALSE;
        ReportClose();
        return( FALSE );
	}


	/* do any special post processing here */
/*	special(); */

    vdelete(win, NONE);
    vdelete ( top_win, NONE );
    vdelete(bot_win, NONE);
	interrupt_mode = FALSE;

    clean_directory ( DELETE_EMPTY_FILES );

    make_file_summary();

    ReportHeader(RECORD_SUMMARY);
	for (i = 0; i < MAX_FILES - MAX_INDEX_FILES; i++)
    {
        if ( ascii_func[i].records_processed )
        {
            sprintf(report_line , "%35s - %6d records processed", file_description[i], ascii_func[i].records_processed);
            ReportData(RECORD_SUMMARY, report_line);
        }
    }

    ReportClose();

    return ( TRUE );
}


/* 	Function:	format_label()
	Rescription:	routine to format a string to a key label
	Inputs:		label - source string
	Outputs:	form_label - the formatted label
	Return:		TRUE - ok
*/

PUBLIC int format_label(char **label, char *form_label, int start_pos)
{	
   memcpy(form_label, EMPTY_LABEL, sizeof(EMPTY_LABEL));
	memcpy(form_label + LABEL_1_OFFSET, label[start_pos++], KEY_LINE_LABEL_SZ);
	memcpy(form_label + LABEL_2_OFFSET, label[start_pos++], KEY_LINE_LABEL_SZ);
	memcpy(form_label + LABEL_3_OFFSET, label[start_pos], KEY_LINE_LABEL_SZ);

	return(TRUE);
}

/* Function:   	write_header()
   Rescription:	writes a standard header at the start of
	               each ascii file
   Inputs:	   	outfile - file pointer to file to write header to
      		   	count - record count for header
   Outputs:	      None
   Return:	      TRUE - ok
*/

PUBLIC int write_header(FILE *outfile, int count)
{	
   STD_HDR_REC	header;

	header = (STD_HDR_REC)count;

	fseek(outfile, 0L, SEEK_SET); 
	fwrite((char *)&header, 1, sizeof(STD_HDR_REC), outfile);

	return(TRUE);
}

/* 	Function:	remove_trail_spaces()
	Rescription:	removes trailing spaces from a string and
			replace with nulls
	Inputs:		str - source string
			len - string length
	Outputs:	str - string with no trailing spaces
	Return:		None
*/

PUBLIC void remove_trail_spaces(char *str, int len)
{
	int	i;
	
	i = len - 1;

	while (*(str + i) == ' ' || *(str + i) == 0)
	{
		*(str + i) = 0;
		i--;
	}
}

/* 	Function:	add_message_to_process_status()
	Rescription:	adds a string as a comment to the window
			showing the status of the ascii file production
	Inputs:		message - string to display
	Outputs:	None
	Return:		None
*/

#pragma argsused

PUBLIC void add_message_to_process_status(char *message)
{
	return;
/*
	vratputs(win, 4, 2, WINDOW_HILITE2, message);
*/
}

/* 	Function:	set_process_status()
	Rescription:	handles the percentage bar display
	Inputs:		num_processed - the number of records
				processed so far
	Outputs:	None
	Return:		None
*/
		   
PUBLIC void set_process_status(long num_processed)
{
	long	result = 0L;
	long	tmp_percent_complete = 0L;
	long	tmp_display_column = 0L;
	long	i;
	char	message[40];

	if (num_processed)
		result = (100L * ascii_func[file_being_processed - 1].records_to_process) / 
			num_processed;

	if (result)
	{
		tmp_percent_complete = 10000L / result;
		tmp_display_column = 3200L / result;
	}

	if (tmp_percent_complete != percent_complete)
	{
		percent_complete = tmp_percent_complete;

		sprintf(message, "%ld %c", percent_complete, '%');
		vratputs(win, current_row, 69, WINDOW_HILITE2, message);
	}
	if (tmp_display_column != display_column)
	{
		display_column = tmp_display_column;
		for (i = (long)INTEGRITY_CHK_COL; i < display_column + (long)INTEGRITY_CHK_COL; i++)
			vatputa(win, current_row, (int)i, WINDOW_HILITE3);
	}
}

/* 	Function:	extract_opgrp_from_ctnum()
	Rescription:	works out from any given command terminal number,
			which controller group they belong to.
	Inputs:		cc_index - index to command terminal number
	Outputs:	og_index - index to controller group this terminal
			belongs to
			terminal - the controller # within that particular
			controller group
			store - string index of the command terminal number
	Return:		None
*/

PUBLIC void extract_opgrp_from_ctnum(char *cc_index, int *og_index, int *terminal, char *store)
{
	int	index, status, i;
	int	which_item = 0;
	char	index_store[MAX_INDEX_LEN];
	char	*index_str = index_store;
	char	rec_store[MAX_REC_LENGTH];
	char	*fld_store[MAX_FIELDS];
	int	term_index;

	index = get_opgrp_item(which_item++);

	while (index != ERROR)
	{
		itoa(index, index_str, 10);
		status = read_record(CT_CONFIG_FILE, &index_str, rec_store, fld_store, key1);
		if (status == OK)
		{
			for (term_index = 0, i = HC_START_EXT; i <= HC_END_EXT; i++)
			{
				if (isdigit((int)*fld_store[i]))
				{
					if (!strcmp(fld_store[i], cc_index))
					{
						*terminal = term_index;
						*og_index = which_item - 1;
						strcpy(store, fld_store[i]);
						return;
					}
					term_index++;
				}
			}
		}

		index = get_opgrp_item(which_item++);

	}

	/* an error has occurred if we get here */
	*og_index = 0;
	*terminal = 0;
}

