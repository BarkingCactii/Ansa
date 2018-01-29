#pragma pack (1)

/*
    Program :  cv_db.c

    System  :  Train Control Command Communication System
    Author  :  PCM Software for Ansa Pty. Ltd. (c) 1991
    Revision:  1.0
*/

#define _GLOBAL

#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "sysdef.h"
#include "filename.h"
#include "dbio.h"
#include "lists.h"
#include "dbconfig.h"
#include "sdb_defn.h"

/*
 *  Dummied functions and vars to satisfy the linker
 */

PUBLIC int getkey(void)
{
    return(TRUE);
}

PUBLIC int interrupt_mode = FALSE;

#define DUMMY_TELECOM_CALL      '1'
#define DUMMY_SELECTIVE_CALL	'2'
#define DUMMY_ADMIN_AUTO        '3'
#define DUMMY_CONTROL_AUTO      '4'
#define DUMMY_OMNIBUS           '5'
#define DUMMY_SINGLE_CHANNEL	'6'
#define DUMMY_OPEN_CHANNEL      '7'
#define DUMMY_IN_OMNIBUS        '8'
#define DUMMY_IN_SINGLE_CHANNEL	'9'
#define DUMMY_IN_SELECTIVE_CALL '0' 

typedef struct
{
    int     old;
    int     new;
} LINE_TYPE_REC;

/*
 *  Old line type and their corresponding new line types
 */

PRIVATE LINE_TYPE_REC   far new_ltype[] = 
{
    { DUMMY_TELECOM_CALL,       EXTERNAL_CALL }, 
    { DUMMY_SELECTIVE_CALL,     SELECTIVE_CALL },
    { DUMMY_IN_SELECTIVE_CALL,  SELECTIVE_CALL },
    { DUMMY_ADMIN_AUTO,         INTERNAL_CALL },
    { DUMMY_CONTROL_AUTO,       INTERNAL_CALL },
    { DUMMY_OMNIBUS,            OMNIBUS_CALL },
    { DUMMY_IN_OMNIBUS,         OMNIBUS_CALL },
    { DUMMY_SINGLE_CHANNEL,     S_RADIO_CALL },
    { DUMMY_IN_SINGLE_CHANNEL,  S_RADIO_CALL },
    { DUMMY_OPEN_CHANNEL,       O_RADIO_CALL },     
    { (int)NULL,                (int)NULL } 
};

typedef struct
{
    int     do_upload;
    char    *old_file_name;
} UPLOAD_REC;

/*
 *  files to be converted are flagged as TRUE in do_upload
 *  old_file_name is the name of this file in the old version of SMT
 */

PRIVATE UPLOAD_REC far allowable_upload[] = 
{
    { TRUE , "grppage" },       /* Group Page Labels */
    { TRUE , "phoneno" },       /* Telephone Numbers */
    { TRUE , "lineass" },       /* Passive Line Assignments */
    { FALSE, "" },              /* Called Party (Controller) */
    { TRUE , "gactive" },       /* Primary Active Assignments */
    { FALSE, "" },              /* Secondary Active Assignments */
    { FALSE, "" },              /* Command Terminal Grouping */
    { FALSE, "" },              /* PABX Configuration */
    { FALSE, "" },              /* Command Terminal Configuration */
    { FALSE, "" },              /* AMBA (Batch Conferences) */
    { FALSE, "" },              /* Controller Passwords */
    { TRUE , "opgroup" },       /* Controller Group Labels */
    { FALSE, "" },              /* Active Assignment Labels */
    { FALSE, "" },              /* Party Line Active Assignments */
    { FALSE, "" },              /* Selcall Line Active Assignments */
    { FALSE, "" },              /* Outgoing Party Line Parties */
    { FALSE, "" },              /* ECBU Magazine Labels */
    { FALSE, "" },              /* Service Personnel Passwords */
    { FALSE, "" },              /* Report Configuration */
    { TRUE , "_grppage" },      /* Index File 1 */
    { TRUE , "_phoneno" },      /* Index File 2 */
    { TRUE , "_opgroup" },      /* Index File 3 */
    { FALSE, "" },              /* Index File 4 */
    { FALSE, "" },              /* Index File 5 */
    { FALSE, "" },              /* Index File 6 */
};

PRIVATE int MapLineType ( char ltype )
{
    int     i = 0;

    while (new_ltype[i].old)
    {
        if (ltype == new_ltype[i].old)
            return ( new_ltype[i].new );
        i++;
    }
    return ( EXTERNAL_CALL );
}

PRIVATE int CreateDataFile(int file_num, char *path, Db_Obj **handle, Index_Obj **index)
{
	char		filespec[_MAX_PATH];

    /* open source data file */
	strcpy(Sdb_data_path, path);
	file_path(file_num, filespec);

	if ((*handle = create_database(file_num, file_attr[file_num].rec_desc)) == (Db_Obj *)NULL)
    {
        printf("Create file [%s] unsuccessful\n", filespec);
        return(FALSE);
    }

    make_index(*handle, file_num);
	iclose_db ( *handle );

	if ((*handle = iopen_db(filespec)) == (Db_Obj *)NULL)
    {
        printf("Open file [%s] unsuccessful\n", filespec);
        return(FALSE);
    }

    /* get info on source data file */
	if ((*index = get_index_handle(*handle, key1)) == (Index_Obj *)NULL)
    {
		printf("Failed to retreive old index for [%s]\n", filespec);
        return(FALSE);
    }

    return(TRUE);
}

PRIVATE int OpenDataFile(int file_num, char *path, Db_Obj **handle, Index_Obj **index)
{
	char		filespec[_MAX_PATH];

    /* open source data file */
	strcpy(Sdb_data_path, path);
    strcat ( strcpy ( filespec, path ), allowable_upload[file_num].old_file_name );

	if ((*handle = iopen_db(filespec)) == (Db_Obj *)NULL)
    {
        printf("Open file [%s] unsuccessful\n", filespec);
        return(FALSE);
    }

    /* get info on source data file */
	if ((*index = get_index_handle(*handle, key1)) == (Index_Obj *)NULL)
    {
		printf("Failed to retreive old index for [%s]\n", filespec);
        return(FALSE);
    }

    return(TRUE);
}

PRIVATE int ConvertWithNochange ( int file_num, char *old_path, char *new_path )
{
	Db_Obj		*ohandle;
	Db_Obj		*nhandle;
	Index_Obj	*oindex;
	Index_Obj	*nindex;
	char		rec_store[MAX_REC_LENGTH];
	char		*fld_store[MAX_FIELDS];
	int		    status;

    /* open source data file */
    if ( OpenDataFile ( file_num, old_path, &ohandle, &oindex ) == FALSE )
        return ( FALSE );

    /* create destination data file */
    if ( CreateDataFile ( file_num, new_path, &nhandle, &nindex ) == FALSE )
        return ( FALSE );

	status = move_file_cursor ( BOIND, rec_store, fld_store, ohandle, oindex, NO_ARGS );
	while ( status == OK )
	{
		update_file ( RECADD, (char **)&fld_store, nhandle, nindex );
		status = move_file_cursor ( NEXT, rec_store, fld_store, ohandle, oindex, NO_ARGS );
	}

	iclose_db ( ohandle );
	iclose_db ( nhandle );

	return ( TRUE );
}

PRIVATE int ConvertNumbers(char *old_path, char *new_path)
{
	Db_Obj		*ohandle;
	Db_Obj		*nhandle;
	Index_Obj	*oindex;
	Index_Obj	*nindex;
	char		rec_store[MAX_REC_LENGTH];
	char		*fld_store[MAX_FIELDS];
	int		    status;

    /* open source data file */
    if (OpenDataFile(NUMBER_FILE - 1, old_path, &ohandle, &oindex) == FALSE)
        return(FALSE);

    /* create destination data file */
    if (CreateDataFile(NUMBER_FILE - 1, new_path, &nhandle, &nindex) == FALSE)
        return(FALSE);

	status = move_file_cursor(BOIND, rec_store, fld_store, ohandle, oindex, NO_ARGS);
	while (status == OK)
	{
        *fld_store[PN_LINE_TYPE] = MapLineType(*fld_store[PN_LINE_TYPE]);

		update_file(RECADD, (char **)&fld_store, nhandle, nindex);
		status = move_file_cursor(NEXT, rec_store, fld_store, ohandle, oindex, NO_ARGS);
	}

	iclose_db(ohandle);
	iclose_db(nhandle);

	return(TRUE);
}

PRIVATE int ConvertPassiveAss(char *old_path, char *new_path)
{
	Db_Obj		*ohandle;
	Db_Obj		*nhandle;
	Index_Obj	*oindex;
	Index_Obj	*nindex;
	char		rec_store[MAX_REC_LENGTH];
	char		*fld_store[MAX_FIELDS];
	int		    status;

    /* open source data file */
    if (OpenDataFile(LINEASS_FILE - 1, old_path, &ohandle, &oindex) == FALSE)
        return(FALSE);

    /* create destination data file */
    if (CreateDataFile(LINEASS_FILE - 1, new_path, &nhandle, &nindex) == FALSE)
        return(FALSE);

	status = move_file_cursor(BOIND, rec_store, fld_store, ohandle, oindex, NO_ARGS);
	while (status == OK)
	{
    	itoa(NUMBER_FILE, fld_store[LA_LINE_TYPE], 10);

		update_file(RECADD, (char **)&fld_store, nhandle, nindex);
		status = move_file_cursor(NEXT, rec_store, fld_store, ohandle, oindex, NO_ARGS);
	}

	iclose_db(ohandle);
	iclose_db(nhandle);

	return(TRUE);
}

PRIVATE int ConvertDatabase(int file_no, char *old_path, char *new_path)
{
    int status = FALSE;

    file_no++;
    switch ( file_no )
    {
        case ( GROUP_FILE ):
        case ( ACTIVE_LINE_FILE ):
        case ( OPERATOR_GROUP_FILE ):
        case ( GROUP_INDEX ):
        case ( PHONE_INDEX ):
        case ( OPERATOR_GROUP_INDEX ):
            status = ConvertWithNochange ( --file_no, old_path, new_path );
        break;
        case ( NUMBER_FILE ):
            status = ConvertNumbers ( old_path, new_path );
        break;
        case ( LINEASS_FILE ):
            status = ConvertPassiveAss ( old_path, new_path );
        break;
        default:
            printf ( "ERROR: Trying to convert %s\n", file_description[file_no] );
        break;
    }

    return ( status );
}

PUBLIC void main(int argc, char *argv[])
{
	int	i;
	time_t	the_time;

	if (argc < 3)
	{
		printf("Usage: %s  src  dst\n", argv[0]);
		printf("Where src is the directory of the old data\n");
		printf("Where dst is the directory of the new data\n");
		printf("eg. %s c:\\olddata\\ c:\\newdata\\\n\n", argv[0]);
		printf("NOTE: The destination directory MUST be empty\n");
		exit(0);
	}

	time(&the_time);
	printf("SMT Database Upload as at %s\n", ctime(&the_time));

	for (i = 0; i < MAX_FILES; i++)
	{
        if (allowable_upload[i].do_upload)
        {
    		printf("Converting [%s] ...\n", file_description[i]);
	    	if (ConvertDatabase(i, argv[1], argv[2]) == FALSE)
                printf("ERROR: can't convert [%s]\n", file_description[i]);
        }
	}

	printf("Conversion complete\n", file_description[i]);
}



