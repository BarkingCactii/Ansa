/*
	filename.h
	Contains specific details related to the database.

    System  :  Train Control Command Communication System
    Author  :  Ansa Pty. Ltd. (c) 1991
    Revision:  1.0
*/

#ifndef _FILENAME_H_
#define _FILENAME_H_

#include "database.h"

#define MAX_FILES           25      /* no files in database */
#define MAX_INDEX_FILES		6		/* no index files in database */
/* #define MAX_FIELDS          56      /* max fields per record */
#define MAX_FIELDS          45      /* max fields per record */
#define MAX_KEYS            7       /* max keys per record */
#define MAX_SIG_KEY_FLDS	4		/* max fields per key */
#define MAX_UNIQUE_KEYS	   4		/* max keys that must be unique */
#define MAX_REC_LENGTH		240 		/* max length for any record */
#define MAX_INDEX_LEN		10		/* max digits in index no */
#define	MAX_FIELD_LEN		25		/* max length for any field */
#define START_CONF_TYPE    24
#define START_CONF_NUM     5

/* 
	Files within the database
	These provide an index to the type FILE_DETAILS
	where details on each file are stored
*/

#define GROUP_FILE              1
#define NUMBER_FILE             2
#define LINEASS_FILE            3
#define B_PARTY_FILE            4
#define ACTIVE_LINE_FILE        5
#define PRIMARY_LINE_FILE       6
#define CT_CONFIG_FILE          7
#define GEN_COMMS_FILE          8
#define CT_COMMS_FILE           9
#define GROUP_CONF_FILE         10
#define PASSWORD_FILE           11
#define OPERATOR_GROUP_FILE 	12
#define OPERATOR_ASSIGN_FILE 	13
#define PARTY_LINE_FILE         14
#define D_PARTY_FILE            15
#define OUT_PARTY_LINE_FILE     16
#define ECBU_MAGAZINE_FILE     17
#define SERVICE_PASSWORD_FILE  18
#define REPORT_CONFIG_FILE     19
#define GROUP_INDEX             20
#define PHONE_INDEX             21
#define OPERATOR_GROUP_INDEX 	  22
#define PASSWORD_INDEX          23
#define EXTENSION_INDEX         24
#define ECBU_INDEX              25 

/*
	Menu items off the main menu
*/

#define ASCII_FILES             102
#define DUMP_DATABASE           103
#define REGRESS_DATA            104

/*
	Key labels
*/

EXPORT char    *key1;
EXPORT char    *key2;
EXPORT char    *key3;
EXPORT char    *key4;
EXPORT char    *key5;
EXPORT char    *key6;
EXPORT char    *key7;

/*
	File types - text description
*/

EXPORT char    *file_description[MAX_FILES + 1];

/*
	Definition of a file within the database
*/

typedef struct
{
	char	*filename;			/* the database file name */
	char	*ascii_filename;		/* the ascii file name */
	char	**rec_desc;			/* no of fields in record */
	char	**key_fields[MAX_KEYS]; 	/* the keys (NOTE: max 3) */
} FILE_DETAILS;

/*
	Physical description of the files within the database
*/

EXPORT FILE_DETAILS    file_attr[MAX_FILES];

/*
	the field position of an index within a file
	a non zero value indicates the field position within 
	the record which will store an index to a master index file
*/

EXPORT int inx_pos[MAX_FILES];

/*
	we know where the index is stored within a file
	(see above), so we need to describe as to what master index file
	this index field relates to
*/

EXPORT int ass_indexes[MAX_FILES];

typedef struct
{   int     num_flds;
    int     fld_offset[MAX_SIG_KEY_FLDS];
    char   *key_num;
} UNIQUE_KEY;

EXPORT UNIQUE_KEY   significant_key[MAX_FILES - MAX_INDEX_FILES][MAX_UNIQUE_KEYS];

EXPORT char        *display_order[MAX_FILES - MAX_INDEX_FILES];

#endif
