
#include <stdlib.h>
#include <string.h>
#include <dir.h>

#include "globals.h"
#include "file_io.h"
#include "mnts_log.h"
#include "gen_func.h"

#include "dbconfig.h"

#define DB_CONFIG_FILE      ".\\SMT.CFG"
#define BACKSLASH           '\\'

PUBLIC char     Sdb_data_path[_MAX_PATH];
PUBLIC char     Sdb_ascii_path[_MAX_PATH];

PUBLIC int ReadDatabaseConfig ( void )
{
    int      result = SUCCESS;
    FILE    *cfg_fd;
    char    dir_name[_MAX_PATH];
    int     i;

    _PROC("ReadDatabaseConfig");

    if ( ( cfg_fd = Gen_Open ( DB_CONFIG_FILE, "rt" ) ) == NULL )
    {
        sprintf ( Mnt_mnt_err_mesg, "Open failed - database cfg file [%s]", \
                                                            DB_CONFIG_FILE );
        Mnt_LogMaintError ( _ID("ReadDatabaseConfig"), GEN_FILE_OPEN_ERROR, \
                                                errno, Mnt_mnt_err_mesg );
        return ( FAILURE );
    }
    rewind ( cfg_fd );

    if ( !Gen_GetString ( cfg_fd, Sdb_data_path, _MAX_PATH ) )
    {
        sprintf ( Mnt_mnt_err_mesg, "Read failed on data path - file [%s]", \
                                                            DB_CONFIG_FILE );
        Mnt_LogMaintError ( _ID("ReadDatabaseConfig"), GEN_FILE_READ_ERROR, \
                                                errno, Mnt_mnt_err_mesg );
    }
    else if ( !Gen_GetString ( cfg_fd, Sdb_ascii_path, _MAX_PATH ) )
    {
        sprintf ( Mnt_mnt_err_mesg, "Read failed on ascii path - file [%s]",\
                                                            DB_CONFIG_FILE );
        Mnt_LogMaintError ( _ID("ReadDatabaseConfig"), GEN_FILE_READ_ERROR, \
                                                errno, Mnt_mnt_err_mesg );
    }
    else
        result = SUCCESS;

    if ( Gen_Close ( cfg_fd ) == ERROR )
    {
        sprintf ( Mnt_mnt_err_mesg, "Close failed - database cfg file [%s]",\
                                                            DB_CONFIG_FILE );
        Mnt_LogMaintError ( _ID("ReadDatabaseConfig"), GEN_FILE_CLOSE_ERROR,\
                                                errno, Mnt_mnt_err_mesg );
        result = FAILURE;
    }

    /* create directories if they don't exist */
    strcpy ( dir_name, Sdb_data_path );
    for ( i = strlen ( dir_name ) - 1; i; i--)
    {
        if ( i <= 0)
            break;

        if ( dir_name[i] != ' ' && dir_name[i] == BACKSLASH )
        {
            dir_name[i] = 0;
            mkdir(dir_name);
            break;
        }
    }

    strcpy ( dir_name, Sdb_ascii_path );
    for ( i = strlen ( dir_name ) - 1; i; i--)
    {
        if ( i <= 0)
            break;

        if ( dir_name[i] != ' ' && dir_name[i] == BACKSLASH )
        {
            dir_name[i] = 0;
            mkdir(dir_name);
            break;
        }
    }

    return ( result );
}

