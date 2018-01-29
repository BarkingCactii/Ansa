#pragma pack (1)
/*
	cleandir,c

    System  :  Train Control Command Communication System
    Author  :  Ansa Pty. Ltd. (c) 1991
    Revision:  1.0
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dos.h>

#include "sysdef.h"
#include "mnts_log.h"
#include "dbconfig.h"
#include "cleandir.h"
#include "sdb_defn.h"

/* 	Function:	    clean_directory()
	Rescription:    erases all ascii files used by the CT's in
                    the directory specified in smt.cfg in
                    preparation of creating a new set of ascii files
	Inputs:         None
	Outputs:        None
	Return:         None
*/

PUBLIC void clean_directory ( int method )
{
	struct		find_t	find;
	char		path[MAX_PATH];
	char		file[MAX_PATH];

	/* get the directory path */
	strcpy(path, Sdb_ascii_path);
	strcat(path, "*.*");

	/* Find first matching file, then find additional matches. */

	if (_dos_findfirst(path, _A_ARCH | _A_HIDDEN, &find))
	{
        Mnt_LogMaintError(_ID("clean_directory"), 1, ERROR, "CT ascii file directory empty!");
		return;
	}

	strcpy(file, Sdb_ascii_path);
	strcat(file, find.name);

    if ( method == DELETE_ALL_FILES )
	    remove(file);

    if ( method == DELETE_EMPTY_FILES )
    {
        if ( find.size == 0L )
    	    remove(file);
    }

	while (!_dos_findnext(&find))
	{
    	strcpy(file, Sdb_ascii_path);
		strcat(file, find.name);

        if ( method == DELETE_ALL_FILES )
	        remove(file);

        if ( method == DELETE_EMPTY_FILES )
        {
            if ( find.size <= STD_HDR_REC_SIZE )
    	        remove(file);
        }
	}	
}

