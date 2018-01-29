
#include <ibmkeys.h>

#include "globals.h"
#include "win_hues.h"
#include "pop_menu.h"
#include "mnt_smts.h"
#include "security.h"
#include "smt_help.h"
#include "net_smts.h"
#include "sdb_adms.h"

EXPORT  uint _stklen;

#define LOOP_FOREVER    1

PUBLIC void main ( int argc, char *argv[] )
{
    int     status;

    _PROC(argv[0]);

    _DEBUG( printf ( "STACK SIZE [%u]\n", _stklen );)

    status = ( argv[( argc )? 1 : 1] != NULL )? TRUE : FALSE;

/*    if ( !Mnt_InitMaintenance ( ) || !Net_InitialiseNetwork ( status ) )
        vexit ( 1 );

    Smt_InitHelp ( );
*/
    InitUserColours ( );

    breakchk ( OFF );

    pclrchar ( BKGROUND_CHAR );
    pclrattr ( REVNORML );

/*
    status = LOOP_FOREVER;

    while ( !status )
    {
        status = PopUpMenu ( );

        if ( status == EXIT_TO_DBASE )
*/
            status = Sdb_AccessDatabase ( );
/*    } */
    breakchk ( ON );

    vexit ( status - 1 );
}

