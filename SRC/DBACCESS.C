#pragma pack (1)

/*
	main.c

	The great decider of all events

    System  :  Train Control Command Communication System
    Author  :  Ansa Pty. Ltd. (c) 1991
    Revision:  1.0
*/

#define _DATA_ENTRY( )      ( action < 100 )

#define EXIT_TO_MAIN        0
#define REMAIN_IN_LOOP      1

#include <dos.h>
#include <string.h>
#include <stdlib.h>
#include <sfwin.h>

#include "sysdef.h"
#include "file_io.h"
#include "sdb_data.h"
#include "smt_idle.h"
#include "smt_help.h"
#include "mnts_log.h"
#include "gen_func.h"

#include "filename.h"
#include "dbio.h"
#include "pop_menu.h"
#include "title.h"
#include "dbconfig.h"
#include "ascii.h"
#include "dentry.h"
#include "lineass.h"
#include "actass.h"
#include "primass.h"
#include "boxes.h"
#include "err.h"
#include "loaddb.h"
#include "field.h"
#include "formdef.h"

#include "sdb_adms.h"
#include "win_hues.h"
#include "file_mon.h"

/*
 *  Function:       InitFormSystem ( )
 *
 *  Description:    initializes variables used by the form system.
 *                  This sets up pointers to field contents and pointers
 *                  to choice lists. This function is called once only
 *                  prior to doing any screen operations
 */
PRIVATE void InitFormSystem ( void )
{
	int	i;

    /* force colour attributes to be loaded */

/*    vcreat ( 0,0,0,0 ); */

    /* place background on screen */

/*    pclrchar ( BKGROUND_CHAR );
    pclrattr ( REVNORML );
*/
	/* load color attributes */

/*    for ( i = FIRST_USER_ATTR_POS; i < FIRST_USER_ATTR_POS + NUM_USER_COLORS; i++ )
        tblattr ( i, color_table[i - FIRST_USER_ATTR_POS] );
*/

    for ( i = 0; i < MAX_FIELDS; i++ )
	{
	 	record[i] = field[i];
        db_names[i] = dbdat[i];
	}
}

/*
 *  Functions       InitDatabase()
 *  Description:    checks to see if all the files are present.
 *                  if they are not, thay are created and indexed now
 *  Return:         TRUE - operation OK
 *                  FALSE - error
 */
PRIVATE int InitDatabase ( void )
{
    int      i    = 0;
    int      flag = TRUE;
	Db_Obj	*db_handle;

#ifdef _FM
    start_file_monitor ( );
    display_title ();
#endif

	/*
     *  create all files comprising the database
     *  if the files don't exist. if an error
     *  is returned, then we can assume the index is
     *  corrupt; so we remake them here
     *  if we can't create a datafile or its index,
     *  then there is nothing much to do except terminate
     *  the program
     */

    display_wait();

    isaminit( 30, DEFAULT_BUFSIZE);

    do
    {   if ( ( db_handle = open_database ( i ) ) == (Db_Obj *) NULL )
        {
            /* database doesn't exist */

            if ( ( db_handle = create_database ( i,                         \
                            file_attr[i].rec_desc ) ) == (Db_Obj *) NULL )
			{ 	
                sprintf ( Mnt_mnt_err_mesg, "Creating database file index [%\
d] - failed", i );
                Mnt_LogMaintError ( _ID("InitDatabase"), SDB_GENERAL_ERROR, \
                                                ERROR, Mnt_mnt_err_mesg );
                flag = FALSE;
			}
		 	if (make_index(db_handle, i) == ERROR)
			{ 	
                sprintf ( Mnt_mnt_err_mesg, "Creating index file index [%d] \
- failed", i );
                Mnt_LogMaintError ( _ID("InitDatabase"), SDB_GENERAL_ERROR, \
                                                ERROR, Mnt_mnt_err_mesg );
                flag = FALSE;
			}
		}
		else
        {
            /* database does exists */

            if (isam_errno != I_NOERR)
            {
			 	if (make_index(db_handle, i) == ERROR)
				{
                    sprintf ( Mnt_mnt_err_mesg, "Creating index file index [\
%d] - failed", i );
                    Mnt_LogMaintError ( _ID("InitDatabase"),                \
                                SDB_GENERAL_ERROR, ERROR, Mnt_mnt_err_mesg );
                    flag = FALSE;
				}
            }
			i++;
		}
        flush_database ( ); 
        close_database ( db_handle );

	}
    while ( ( flag == TRUE ) && ( i < MAX_FILES ) );

    flush_database ( );

    drop_wait();

#ifdef _FM
    drop_title ( );
#endif

    return ( flag );
}

PUBLIC int init_system ( void )
/*
    Functions   init_system()
    Description:    high level function to perform program initialization
	Inputs:		None
	Outputs:	None
	Return:		TRUE - operation OK
			FALSE - error
*/
{
	/* open the database */


    return ( TRUE );
}


PRIVATE int MainChoice ( int action )
{
    int status = REMAIN_IN_LOOP;
    int i;

    if ( _DATA_ENTRY( ) )
	{
		/* only show title for database functions */

        display_title ();

		/* set the state of function boxes */

        set_box_state ( action - 1 );
	}
    for (i = 0; i < MAX_FILES - MAX_INDEX_FILES; i++)
        release_choice_list ( i );

	switch (action)
	{
        case(GROUP_FILE):
        case(NUMBER_FILE):
        case(B_PARTY_FILE):
        case(CT_CONFIG_FILE):
        case(GEN_COMMS_FILE):
        case(CT_COMMS_FILE):
        case(GROUP_CONF_FILE):
        case(PASSWORD_FILE):
        case(OPERATOR_GROUP_FILE):
        case(OPERATOR_ASSIGN_FILE):
        case(PARTY_LINE_FILE):
        case(D_PARTY_FILE):
        case(OUT_PARTY_LINE_FILE):
        case(ECBU_MAGAZINE_FILE):
        case(SERVICE_PASSWORD_FILE):
        case(REPORT_CONFIG_FILE):
            status = data_entry ( action - 1 );
		break;

        case(LINEASS_FILE):
            Smt_PushHelp ( PASSIVE_HLP );
			status = line_assignment();
            Smt_PopHelp ( );
		break;

        case(ACTIVE_LINE_FILE):
            Smt_PushHelp ( ACTIVE_HLP );
			status = active_assignments();
            Smt_PopHelp ();
		break;

        case(PRIMARY_LINE_FILE):
            Smt_PushHelp(PRIMARY_HLP);
			status = primsec_assignments(); 
            Smt_PopHelp ();
		break;

        case(ASCII_FILES):
            status = make_ct_file ( );

            if ( status )
            {
                switch ( Sdb_TransferData ( Sdb_ascii_path ) )
                {
                    case SUCCESS:
                        Mnt_LogMaintError ( _ID("MainChoice"),              \
                                SDB_GENERAL_REPORT, FALSE,                  \
                            "New passaive data successfully set active" );
                    break;
                    case FAILURE:
                        Mnt_LogMaintError ( _ID("MainChoice"),              \
                                SDB_GENERAL_REPORT, FALSE,                  \
                                "Passive data download aborted by user" );
                    break;
                    case ERROR:
                        Mnt_LogMaintError ( _ID("MainChoice"),              \
                                SDB_GENERAL_ERROR, ERROR,                   \
                                "Passive data download has failed !!!!" );
                    break;
                }
            }
            status = REMAIN_IN_LOOP;
		break;

        case(REGRESS_DATA):
            switch ( ( status = Sdb_DataRegression ( Sdb_ascii_path ) ) )
            {
                case SUCCESS:
                    Mnt_LogMaintError ( _ID("MainChoice"),                  \
                                SDB_GENERAL_REPORT, FALSE,                  \
                                    "Passive data regression successful" );
                break;
                case FAILURE:
                    Mnt_LogMaintError ( _ID("MainChoice"),                  \
                                    SDB_GENERAL_REPORT, FALSE,              \
                                        "Data Regression aborted by user" );
                break;
                case ERROR:
                    Mnt_LogMaintError ( _ID("MainChoice"),                  \
                            SDB_GENERAL_ERROR, ERROR,                       \
                                "Passive data regression has failed !!" );
                break;
            }
            status = REMAIN_IN_LOOP;
		break;

        case(EXIT):
            status = EXIT_TO_MAIN;
		break;

        case (DANGER):
            status = EXIT_TO_MAIN;
            Mnt_LogMaintError ( _ID("MainChoice"), SDB_GENERAL_ERROR, ERROR,\
                                        "Database initialisation error !" );
		break;

        default:
            sprintf ( Mnt_mnt_err_mesg, "Menu failure - unknown selection [%\
d]", action );
            Mnt_LogMaintError ( _ID("MainChoice"), SDB_GENERAL_ERROR, ERROR,\
                                                        Mnt_mnt_err_mesg );
            status = EXIT_TO_MAIN;
		break;
	}
    flush_database ( );

    if ( _DATA_ENTRY( ) )
        drop_title ( );

    return ( status );
}

/*
 *  Functions       MainLoop ()
    Description:    the highest function in the system. All it does
                    loop displaying the main menu until exit is choosen
    Return:         0 (user exit)
                    < 0 (error has occurred)
*/
PRIVATE int MainLoop ( void )
{
	int	status;

	do
    {   /*
         *  keep displaying the main menu
         *  until user selects exit or error occurs
         */

        flush_database ( );

        status = MainChoice ( popup_menu ( ) );

    } while ( status );

    return ( status );
}

PUBLIC int Sdb_AccessDatabase ( void )
{
    int     ok;

    InitFormSystem ( );

    if ( !ReadDatabaseConfig ( ) )
        return ( FAILURE );

    if ( !InitDatabase ( ) )
        return ( FAILURE );

    ok = MainLoop ( );

   /* close all database files */

   flush_database ( );

   return ( ok );
}

 
