
#include <ctype.h>

#include "globals.h"
#include "times.h"
#include "win_util.h"
#include "mnts_log.h"
#include "smt_idle.h"
#include "win_hues.h"
#include "net_smts.h"
#include "mnt_smts.h"
#include "dir_base.h"
#include "fil_data.h"

#include "sdb_data.h"

#define MAX_WIN_DEPTH   15

PRIVATE char   *confirm_regress[3] =
{
    "",
    "      Press Y to confirm",
    " or N to abort the operation."
};

PRIVATE char   *confirm_download[3] =
{
    "",
    "    Press Y to change over",
    " or N to abort the operation."
};

PRIVATE char   *abort_mesg  = "Press ANY key to ABORT process";
PRIVATE char   *reboot_mesg = "System will be re-booted in >>> 2 minutes";

PRIVATE int ShowPanel ( char *mesg[], int attr )
{
    int     ch;
    int     valid = FALSE;
    HWND    win;

    _PROC("ShowPanel");

    win = vcreat ( 5, 36, ERROR_WINDOW, YES );
    vwind ( win, 5, 36, 0, 0 );
    vlocate ( win, 10, 22 );
    vframe ( win, ERROR_WINDOW, FRDOUBLE );

    vratputs ( win, 1, 3, attr, mesg[0] );
    vratputs ( win, 2, 3, ERROR_WINDOW, mesg[1] );
    vratputs ( win, 3, 3, ERROR_WINDOW, mesg[2] );
    visible ( win, YES, NO );
    vbeep ( );

    while ( !valid )
    {
        ch = Smt_KbdGet ( );
        ch = toupper ( ch );

        if ( ( ch == 'Y' ) || ( ch == 'N' ) )
            valid = TRUE;
        else
            vbeep ( );
    }
    vdelete ( win, NONE );

    return ( ( ch == 'Y' )? TRUE : FALSE );
}

PRIVATE int ShowBackOutPanel ( char *mesg[], int err_flag )
{
    int     show = ERROR_WINDOW;

    _PROC("ShowBackOutPanel");

    if ( err_flag == TRUE )
    {
        show = WAIT_WINDOW;
        mesg[0] = " NOT all CT's processed request";
    }
    else
        mesg[0] = "   All CT's processed request";

    return ( ShowPanel ( mesg, show ) );
}

PUBLIC int Sdb_TransferData ( char *path )
{
    int     i;
    int     col;
    int     max;
    int     mark;
    int     attr;
    int     line;
    int     quit = FALSE;
    int     status;
    int     last_col;
    int     user_total;
    int     mapped_done;
    int     files_before;
    int     percent_done;
    int     ct_failed_flag = FALSE;
    int     total_files_qed;
    char   *m_ptr;
    HWND    win;

    _PROC("Sdb_TransferData");

    Smt_OpenBorderBars ( "Copy Data Files To Lan Server", abort_mesg );

    win = vcreat ( 5, 64, WINDOW_NORMAL, YES );
    vwind ( win, 5, 64, 0, 0 );
    vlocate ( win, 8, 8 );
    vframe ( win, WINDOW_NORMAL, FRSINGLE );
    vtitle ( win, WINDOW_NORMAL, "File Download Status" );
    vratputs ( win, 1, 10, WINDOW_BLINK, "Clearing work directory on Lan Server" );
    visible ( win, YES, NO );

    if ( Mnt_GetAsciiFiles ( path ) != SUCCESS )
    {
        vdelete ( win, NONE );
        Smt_DisplayErrorMesg ( "Copy to Lan Server Failed !!" );
        Smt_CloseBorderBars ( );
        return ( ERROR );
    }
    percent_done = 0;
    total_files_qed = files_before = Mnt_numb_of_files_qed;

    vratputs ( win, 1, 10, WINDOW_NORMAL, "Percentage of files transferred to Server");
    vratputf ( win, 3, 10, WINDOW_NORMAL, "±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±   %3d %%", percent_done );

    _DEBUG( printf ( "FILES QED - ABOUT TO COPY TO SVR\n" );)

    last_col = 0;

    while ( !quit )
    {
        if ( Mnt_numb_of_files_qed < files_before )
        {
            files_before = Mnt_numb_of_files_qed;
            percent_done = ( 100 * ( total_files_qed - files_before ) ) /   \
                                                            total_files_qed;
            mapped_done = percent_done / 3;

            for ( col = last_col; col < mapped_done; col++ )
                vratputs (win, 3, 10 + col, WINDOW_HILITE3, "±");

            vratputs (win, 3, 10 + col, WAIT_WINDOW, "±");
            last_col = mapped_done;
            vratputf ( win, 3, 46, WINDOW_HILITE2, "%3d %%", percent_done );
        }
        Net_PollNetwork ( &status );

        if ( gfkbhit ( ) )
        {
            quit = (int) getkey ( );
            quit = TRUE;
            status = ERROR;
        }
        else
            quit = Mnt_PollMaintenance ( &status );
    }
    vdelete ( win, NONE );
    Smt_CloseBorderBars ( );

    _DEBUG( printf ( "DONE - copy status: %d  quit: %d\n", status, quit );)

    if ( status == ERROR )
    {
        return ( FAILURE );
    }
    else if ( status == FAILURE )
    {
        return ( ERROR );
    }
    Smt_OpenBorderBars ( "Advise Command Terminals", abort_mesg );

    user_total = Net_GetRegisteredUsers ( path );

    max = ( user_total > MAX_WIN_DEPTH )? MAX_WIN_DEPTH : user_total;

    win = vcreat ( max + 2, 52, WINDOW_NORMAL, YES );
    vwind ( win, max + 2, 52, 0, 0 );
    vlocate ( win, 4, 14 );
    vframe ( win, WINDOW_NORMAL, FRSINGLE );
    vtitle ( win, WINDOW_NORMAL, "Command Terminal Data File Transfer" );

    for ( i = 0 ; i < max; i++ )
        vratputf ( win, i + 1, 3, WINDOW_NORMAL, "%s", (char*)              \
                                                Net_user_entry[i].machine );
    visible ( win, YES, NO );

    sprintf ( Mnt_mnt_err_mesg, "Advising %d CTWS(s) - new data @ Server",  \
                                                                user_total );
    Mnt_LogMaintError ( _ID("Sdb_TransferData"), SDB_GENERAL_REPORT, FALSE, \
                                                        Mnt_mnt_err_mesg );

    for ( line = quit = i = 0 ; !quit && ( i < user_total ); i++ )
    {
        status = ERROR;

        if ( i >= max  )
        {
            vratputs ( win, 1, 1, WINDOW_NORMAL, "                                                   " );
            vscrolup ( win, 1 );
            vratputs ( win, line, 1, WAIT_WINDOW, "" );
            vratputf ( win, line, 3, WINDOW_NORMAL, "%s",               \
                                    (char*) Net_user_entry[i].machine );
        }
        else
            vratputf ( win, ++line, 1, WAIT_WINDOW, "" );
                                                
        vratputs (win, line, 18, WINDOW_NORMAL, " - Polling Command Terminal   " );

        Net_HelloCtws ( Net_user_entry[i].machine );
        Smt_PollWhileIdle ( 5L * SECONDS, &status );

        if ( status == SUCCESS )
        {                                            
            vratputs ( win, line, 18, WINDOW_NORMAL, " - File transfer initiated . ." );

            Net_AdviseCtwsUpload ( Net_user_entry[i].machine );
            Smt_PollWhileIdle ( 15L * MINUTES, &status );

            if ( status == SUCCESS )
            {
                attr = WINDOW_HILITE3;
                mark = 'û';
                m_ptr = " - File transfer completed    ";
            }
            else if ( status == ERROR )
            {
                attr = FAILED_NORNAL;
                mark = 'X';
                m_ptr = " - Copy aborted by user !!    ";
                quit = TRUE;
                Net_CancelCtwsUpload ( Net_user_entry[i].machine );
            }
            else
            {
                attr = FAILED_NORNAL;
                mark = 'X';
                m_ptr = " - CTWS advises copy failed !!";
                ct_failed_flag = TRUE;
            }
            vratputf ( win, line, 1, attr, "%c", mark );
            vratputs ( win, line, 18, WINDOW_NORMAL, m_ptr );
        }
        else if ( status != ERROR )
        {
            ct_failed_flag = TRUE;
            vratputs ( win, line, 1, WINDOW_HILITE2, "X" );
            vratputs ( win, line, 18, WINDOW_NORMAL, " - CTWS did not respond !!    " );
                                                     
            sprintf ( Mnt_mnt_err_mesg, "CTWS [%s] didn't reply to upload re\
quest", (char*) Net_user_entry[i].machine );

            Mnt_LogMaintError ( _ID("Sdb_TransferData"), SDB_GENERAL_ERROR, \
                                                ERROR, Mnt_mnt_err_mesg );
        }
        else
            quit = TRUE;
    }
    if ( quit )
    {
        status = FAILURE;
    }
    else if ( !ShowBackOutPanel ( confirm_download, ct_failed_flag ) )
    {
        status = FAILURE;
    }
    else if ( !Mnt_MakeActiveData ( ) )
    {
        Smt_DisplayErrorMesg ( "Change over to active data failed !!" );
        status = ERROR;
    }
    else
    {
        _DEBUG( printf ( "SET NEW PATH SUCCEEDED\n" );)

        Net_AdviseDoActive ( );
        Smt_DisplayErrorMesg ( reboot_mesg );
        status = SUCCESS;
    }
    vdelete ( win, NONE );
    Smt_CloseBorderBars ( );
    return ( status );
}

PUBLIC int Sdb_DataRegression ( char *path )
{
    int     i;
    int     max;
    int     mark;
    int     attr;
    int     line;
    int     quit = FALSE;
    int     status;
    int     user_total;
    int     ct_failed_flag = FALSE;
    char   *m_ptr;
    HWND    win;

    _PROC("Sdb_DataRegression");

    Smt_OpenBorderBars ( "Advise Command Terminals", abort_mesg );

    user_total = Net_GetRegisteredUsers ( path );

    max = ( user_total > MAX_WIN_DEPTH )? MAX_WIN_DEPTH : user_total;

    win = vcreat ( max + 2, 52, WINDOW_NORMAL, YES );
    vwind ( win, max + 2, 52, 0, 0 );
    vlocate ( win, 4, 14 );
    vframe ( win, WINDOW_NORMAL, FRSINGLE );
    vtitle ( win, WINDOW_NORMAL, "Command Terminal Data Regression" );

    for ( i = 0 ; i < max; i++ )
        vratputf ( win, i + 1, 3, WINDOW_NORMAL, "%s", (char*)              \
                                                Net_user_entry[i].machine );
    visible ( win, YES, NO );

    sprintf ( Mnt_mnt_err_mesg, "Advising %d CTWS(s) of Data Regression",   \
                                                                user_total );
    Mnt_LogMaintError ( _ID("Sdb_DataRegression"), SDB_GENERAL_REPORT,      \
                                                FALSE, Mnt_mnt_err_mesg );

    for ( line = quit = i = 0 ; !quit && ( i < user_total ); i++ )
    {
        status = ERROR;

        if ( i >= max  )
        {
            vratputs ( win, 1, 1, WINDOW_NORMAL, "                                                   " );
            vscrolup ( win, 1 );
            vratputs ( win, line, 1, WAIT_WINDOW, "" );
            vratputf ( win, line, 3, WINDOW_NORMAL, "%s",                   \
                                    (char*) Net_user_entry[i].machine );
        }
        else
            vratputf ( win, ++line, 1, WAIT_WINDOW, "" );
                                                
        vratputs (win, line, 18, WINDOW_NORMAL, " - Polling Command Terminal   " );

        Net_HelloCtws ( Net_user_entry[i].machine );
        Smt_PollWhileIdle ( 5L * SECONDS, &status );

        if ( status == SUCCESS )
        {                                            
            attr = WINDOW_HILITE3;
            mark = 'û';
            m_ptr = " - Command Terminal is ready  ";
        }
        else if ( status != ERROR )
        {
            ct_failed_flag = TRUE;
            attr = FAILED_NORNAL;
            mark = 'X';
            m_ptr = " - CTWS did not respond !!    ";
                                                     
            sprintf ( Mnt_mnt_err_mesg, "CTWS [%s] didn't reply to regress r\
equest", (char*) Net_user_entry[i].machine );

            Mnt_LogMaintError ( _ID("Sdb_DataRegression"),                  \
                                SDB_GENERAL_ERROR, ERROR, Mnt_mnt_err_mesg );
        }
        else
        {
            attr = FAILED_NORNAL;
            mark = 'X';
            m_ptr = STR_EMPTY;
            quit = TRUE;
        }
        vratputf ( win, line, 1, attr, "%c", mark );
        vratputs ( win, line, 18, WINDOW_NORMAL, m_ptr );
    }
    Smt_CloseBorderBars ( );
    Smt_OpenBorderBars ( "Data File Regression", STR_EMPTY );

    if ( quit )
    {
        status = FAILURE;
    }
    else if ( !ShowBackOutPanel ( confirm_regress, ct_failed_flag ) )
    {
        status = FAILURE;
    }
    else
    {
        status = Mnt_SetPassiveData ( );

        if ( status == SUCCESS )
        {
            Net_AdviseDoActive ( );
            Smt_DisplayErrorMesg ( reboot_mesg );
        }
        else if ( status == ERROR )
        {
            Mnt_LogMaintError ( _ID("Sdb_DataRegression"),                  \
                                    SDB_GENERAL_ERROR, ERROR,               \
                                        "No passive data to regress to !!" );
            Smt_DisplayErrorMesg ( "No Passive data at Server !!" );
        }
        else
        {
            status = ERROR;
            Smt_DisplayErrorMesg ( "Failed to change to Passive data !!" );
        }
    }
    vdelete ( win, NONE );
    Smt_CloseBorderBars ( );
    return ( status );
}

