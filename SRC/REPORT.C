/*
	report.c

	Handles production of the integrity report

    System  :  Train Control Command Communication System
    Author  :  Ansa Pty. Ltd. (c) 1991
    Revision:  1.0
*/

#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "sysdef.h"
#include "file_io.h"
#include "filename.h"
#include "dbio.h"
#include "report.h"
#include "net_smts.h"
#include "mnts_log.h"

#define FORM_FEED           0x0d
#define NEW_LINE            "\r\n"

#define NUM_REPORT_TYPES    3
#define REPORT_BANNER_1       "S Y S T E M   M A N A G E M E N T   T E R M I N A L"
#define REPORT_BANNER_2       "Download Report"

#define MAX_TIME_LEN        30

typedef struct
{
   int      ref;
   char     *text;
   char     *desc;
   char     *desc2;
} I_TEST_REC;

PRIVATE int      new_header = TRUE;
PRIVATE int      integrity_summary = FALSE;
PRIVATE int      record_summary = FALSE;
PRIVATE int      file_summary = FALSE;
PRIVATE int      previous_test = ERROR;

PRIVATE char    *rep_name = "\\TMP\\SMT.RPT";

PRIVATE char    *banner_names[] =
{
   "I N T E G R I T Y    S U M M A R Y",
   "R E C O R D    S U M M A R Y",
   "F I L E    S U M M A R Y",
};
   
PRIVATE FILE    *rep_fp = (FILE *)NULL;

PRIVATE I_TEST_REC far  integrity_line[] =
{
   { ALL_RECORDS_DEFINED,       "Existence Check",
   "The following files do not exist", "" },
   { GROUP_PAGES_ASSIGNED,      "Group Page Assignments",
   "These Group Pages are not defined in Passive Line Assignments", "" },
   { SELCALL_EXISTS,            "Selcall Active Assignments",
   "These extensions are not defined in Primary Active Assignments", "" },
/*   "These extensions are not defined in Selcall Line Active Assignments", "" }, */
   { OMNIBUS_PARTIED,           "Party Line Assignments",
   "These extensions are not defined in Outgoing Party Line Assignments", "" },
   { CALLED_PARTY_ASSIGNED,     "Called Party Assignments",
   "These Controller Group Labels are not defined in Called Party Assignments", "" },
   { GROUP_LABELS_GROUPED,      "Command Terminal Grouping",
   "These Controller Group Labels are not defined in Command Terminal Grouping", "" },
   { PINS_PER_GROUP,            "P.I.N. \'s",
	"CONTROLLER GROUP               # TERMINALS    # P.I.N. \'s",
   "P.I.N. \'s must equal or exceed number of terminals in group"} ,
   { ACTIVE_ASSIGNMENTS_DEFINED,"Active Assignments", 
	"CONTROLLER GROUP      IN TELEPHONY  EXTENSION          CONFIG" ,
	"These extension are not assigned" },
   { CONTROLLER_LABELS_DEFINED, "Active Assignment Labels", 
	"CONTROLLER GROUP      IN TELEPHONY  CONFIG",
	"Group Controller Labels are not defined" }
};


/*
 *  Functions       ReportIntegrityLine()
 *  Description:    Prints a sub-heading of a single integrity check when
 *                  performing a download. The sub-heading is identical to
 *                  those seen on screen when the integrity test is performed.
 *  Args:           which_test - which integrity test is being performed
 *  Return:         None
 */

PUBLIC void ReportIntegrityLine ( int which_test )
{
   if (integrity_summary)
   {
      if (previous_test != which_test)
      {
         fprintf(rep_fp, "ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ%s", NEW_LINE );
         fprintf(rep_fp, "%s%s%s", NEW_LINE, integrity_line[which_test].desc, NEW_LINE);
         if (strlen(integrity_line[which_test].desc2))
            fprintf(rep_fp, "%s%s%s", NEW_LINE, integrity_line[which_test].desc2, NEW_LINE);
         previous_test = which_test;
      }
      fprintf(rep_fp, "%s%s%s", NEW_LINE, integrity_line[which_test].text, NEW_LINE);
   }
}

/*
 *  Functions       ReportData()
 *  Description:    Prints a line of data. The line will only be printed if
 *                  the actual option was choosen to be reported on as 
 *                  defined in Report Configuration
 *  Args:           which_option - which option is being printed. This can be
 *                      an integrity test , a record summary of file summary.
 *                  text - the line to print
 *  Return:         None
 */

PUBLIC void ReportData(int which_option, char *text)
{
   int   print_line = FALSE;

   if (integrity_summary && which_option == INTEGRITY_SUMMARY)
      print_line = TRUE;

   if (record_summary && which_option == RECORD_SUMMARY)
      print_line = TRUE;

   if (file_summary && which_option == FILE_SUMMARY)
      print_line = TRUE;

   if (print_line)
      fprintf(rep_fp, "%s%s", text, NEW_LINE);
}

/*
 *  Functions       ReportHeader()
 *  Description:    Prints the heading on the option to be reported.
 *                  This can be an integrity test , a record summary 
                    of file summary. The heading is only printed if the
                    user has choosen to report on this option as defined
                    in Report Configuration.
 *  Args:           which_option - which option to print a heading for
 *  Return:         None
 */

PUBLIC void ReportHeader(int which_option)
{
   time_t   time_in_secs;
   int      print_line = FALSE;
   char     *time_buf;
   char     *time_buf_nl;

   fprintf(rep_fp, "%c%s", FORM_FEED, NEW_LINE);

   if (new_header)
   {
      new_header = FALSE;
      time_in_secs = time(NULL);
      time_buf = ctime ( &time_in_secs );
      if (( time_buf_nl = strchr ( time_buf, ( int ) '\n' )) != ( char * ) NULL )
        *time_buf_nl = ( char ) NULL;

      fprintf(rep_fp, "ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿%s", NEW_LINE );
      fprintf(rep_fp, "³ %51s                      ³%s", REPORT_BANNER_1, NEW_LINE);
      fprintf(rep_fp, "³ %15s as at %25s                          ³%s", REPORT_BANNER_2, time_buf, NEW_LINE);
      fprintf(rep_fp, "ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ%s%s", NEW_LINE, NEW_LINE );
   }

   if (which_option < 0 || which_option > NUM_REPORT_TYPES - 1)
      return;

   if (integrity_summary && which_option == INTEGRITY_SUMMARY)
      print_line = TRUE;

   if (record_summary && which_option == RECORD_SUMMARY)
      print_line = TRUE;

   if (file_summary && which_option == FILE_SUMMARY)
      print_line = TRUE;

   if (print_line)
   {
      fprintf(rep_fp, "ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿%s", NEW_LINE );
      fprintf(rep_fp, "³ %50s                       ³%s", banner_names[which_option], NEW_LINE);
      fprintf(rep_fp, "ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ%s%s", NEW_LINE, NEW_LINE );
    }
}

/*
 *  Functions       ReportInitialize()
 *  Description:    Determines which options are to be reported on as defined
 *                  in Report Configuration and opens the report file. This 
                    option is called before any other calls to this module
                    and prior to a report being generated.
 *  Args:           None
 *  Return:         TRUE - the module successfully initialized
 *                  FALSE - unsuccessful
 */

PUBLIC int ReportInitialize()
{
	Db_Obj		*handle;
	Index_Obj	*index;
	int	   	    status;
	char  		rec_store[MAX_REC_LENGTH];
	char  		*fld_store[MAX_FIELDS];

   previous_test = -1;

	if ((handle = open_database(REPORT_CONFIG_FILE - 1)) == (Db_Obj *)NULL)
		return(FALSE);

	if ((index = get_index_handle(handle, key1)) == (Index_Obj *)NULL)
		return(FALSE);

	/* get first record */
	status = move_file_cursor(BOIND, rec_store, fld_store, handle, index, NO_ARGS);
	if (status == OK)
   {
      if (atoi(fld_store[RC_INTEGRITY]))
         integrity_summary = TRUE;
      else
         integrity_summary = FALSE;
      if (atoi(fld_store[RC_RECORD]))
         record_summary = TRUE;
      else
         record_summary = FALSE;
      if (atoi(fld_store[RC_FILE]))
         file_summary = TRUE;
      else
         file_summary = FALSE;
   }
   else
   {
         integrity_summary = FALSE;
         record_summary = FALSE;
         file_summary = FALSE;
   }
   if ((rep_fp = Gen_Open(rep_name, "wb")) == (FILE *)NULL)
   {
      sprintf ( Mnt_mnt_err_mesg, "Failed to open report [%s]", rep_name );
      Mnt_LogMaintError ( _ID("ReportInitialize"), SDB_GENERAL_ERROR, \
         ERROR, Mnt_mnt_err_mesg );
      return(FALSE);
    }

    return(TRUE);
}

/*
 *  Functions       ReportClose()
 *  Description:    Closes the report. This close the file and spools it
 *                  to the printer only if at least 1 option had been
 *                  choosen to be reported on.
 *  Args:           None
 *  Return:         None
 */

PUBLIC int ReportClose()
{
   if (rep_fp)
   {
      fprintf(rep_fp, "%s", NEW_LINE);  
      fprintf(rep_fp, "ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿%s", NEW_LINE );
      fprintf(rep_fp, "³                   E N D    R E P O R T                                   ³%s", NEW_LINE );
      fprintf(rep_fp, "ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ%s", NEW_LINE );

      Gen_Close(rep_fp);
      
      if ( integrity_summary || record_summary || file_summary )
      {
        if ( ! Net_PrintFile ( rep_name ))
        {
                sprintf ( Mnt_mnt_err_mesg, "Print of [%s] failed", rep_name );
                Mnt_LogMaintError ( _ID("ReportClose"), SDB_GENERAL_ERROR, \
                                                ERROR, Mnt_mnt_err_mesg );
        }
      }

      new_header = TRUE;
      rep_fp = (FILE *)NULL;
      return(TRUE);
   }

   return(FALSE);
}
