#ifdef _FM

#include "sysdef.h"
#include "filename.h"

#include "win_hues.h"

#include "file_mon.h"

PRIVATE HWND    mon_wn;


PUBLIC void start_file_monitor ( void )
/*
    Function:   start_file_monitor ( )
    Rescription:    initializes the file monitor function
            (the little boxes that appear on top of the
            screen)
    Inputs:     None
    Outputs:    None
    Return:     None
*/
{
    int i;

    mon_wn = vcreat ( 1, MAX_FILES, REVNORML, YES );
    vwind ( mon_wn, 1, MAX_FILES, 0, 0 );
    vlocate ( mon_wn, 1, 0 );
    visible ( mon_wn, YES, NO );
    for ( i = 0; i < MAX_FILES; i++ )
        vratputc ( mon_wn, 0, i, REVNORML, BKGROUND_CHAR );
}

PUBLIC  void file_monitor ( int file_no, int state )
/*
    Function:   file_monitor()
    Rescription:    hilites or dims a box depending on whether a
            file is open or closed. Each box represents
            a file.
    Inputs:     file_no - the file to monitor
            state - TRUE - file open
                  - FALSE - file closed
    Outputs:    None
    Return:     None
*/
{
/*
    visible ( mon_wn, YES, YES );
*/
    switch ( state )
    {
        case ( OPEN_STATE ):
            vratputc ( mon_wn, 0, file_no, FIELD_HILITE, (char) SOLID_CHAR );
        break;
        case ( READ_STATE ):
            vratputc ( mon_wn, 0, file_no, FIELD_HILITE, (char) 'R' );
        break;
        case ( WRITE_STATE ):
            vratputc ( mon_wn, 0, file_no, REVEMPHNORML, (char) 'W' );
        break;
        case ( CLOSE_STATE ):
            vratputc ( mon_wn, 0, file_no, REVNORML, BKGROUND_CHAR );
        break;
    }
    /*
    if ( state )
        vratputc ( mon_wn, 0, file_no, FIELD_HILITE, (char) SOLID_CHAR );
    else
        vratputc ( mon_wn, 0, file_no, REVNORML, BKGROUND_CHAR );
    */
}

#endif

