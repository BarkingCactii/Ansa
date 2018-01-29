#pragma pack (1)
/*
	ascii_hc.c

    System  :  Train Control Command Communication System
    Author  :  Ansa Pty. Ltd. (c) 1991
    Revision:  1.0
*/

#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "sysdef.h"
#include "filename.h"
#include "dbio.h"
#include "getidx.h"
#include "ascii.h"
#include "mapopgrp.h"
#include "sdb_defn.h"

/* 	Function:	ascii_hc()
	Rescription:	produces the file t_matrix.dat
	Inputs:		handle - handle to file containing command terminal
				 grouping records
			fp - output file
	Outputs:	None
	Return:		FALSE - database error
			>0 - number of records processed
*/

PUBLIC int ascii_hc(Db_Obj *handle, FILE *fp)
{
	Index_Obj		*bp_index;
	char			rec_store[MAX_REC_LENGTH];
	char			*fld_store[MAX_FIELDS];
	char			*key_store[MAX_FIELDS];
	int			    status;
	int			    no_recs = 0;
	CNTL_GRP_REC    rec;
	int			    index = 0;
	int			    which_item = 0;
	char			index_str[MAX_INDEX_LEN];
   int              i, j;

	/* write header rec */
	write_header(fp, no_recs);

	if ((bp_index = get_index_handle(handle, key1)) == (Index_Obj *)NULL)
		return(FALSE);

	key_store[0] = index_str;
	index = get_opgrp_item(which_item++);

	while (index != ERROR)
	{
      rec.ct_count = 0;
      memset((char *)rec.ct_index, UNUSED_CT, sizeof(rec.ct_index));

		itoa(index, index_str, 10);
		status = move_file_cursor(FIND, rec_store, fld_store, handle, bp_index, key_store);

		if (status == OK)
		{
   		for (i = HC_START_EXT, j = 0; i <= HC_END_EXT; i++)
         {
	   		if (isdigit((int)*fld_store[i]))
		   	{
               if (j < MAX_CTS_PER_GROUP)
               {
                  rec.ct_count++;
                  rec.ct_index[j++] = get_ct_index(fld_store[i]);
               }
            }
         }
		}

		fwrite((char *)&rec, 1, sizeof(rec), fp);

		no_recs++;
		set_process_status((long)no_recs);

		index = get_opgrp_item(which_item++);

		if (process_interrupted() == ABORT_PROCESS)
			break;
	}

	write_header(fp, no_recs);
	return(no_recs);
}
