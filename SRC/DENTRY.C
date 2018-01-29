#pragma pack (1)

/*
	dentry.c

	Contains:
		data validation routines
		Interface from user choices to database functions

    System  :  Train Control Command Communication System
    Author  :  Ansa Pty. Ltd. (c) 1991
    Revision:  1.0
*/

#include <string.h>
#include <stdlib.h>

#include "sysdef.h"
#include "dbio.h"
#include "filename.h"
#include "field.h"
#include "forms.h"
#include "err.h"
#include "boxes.h"
#include "purge.h"
#include "sdb_defn.h"
#include "dentry.h"

/* to be moved to sdb_defn.h */
#define MAX_GROUP_PAGE_LABELS           50
#define MAX_CONTROLLER_GROUP_LABELS     30
/* to be moved to sysdef.h */

#define PIN_TOO_SMALL   94

/* 	Function:	ptrscpy
	Rescription:	copy the contents of an array of pointers
			to another likewise structure
	Inputs:		dest - destination char **
			src  - source char **
	Outputs:	None
	Return:		None
*/

PRIVATE void ptrscpy(char **dest, char **src)
{
	while (*src != (char *)NULL)
		strcpy(*dest++, *src++);

	**dest = (char)NULL;
}			 

/*
   handles the ALT-x key combination to quickly jump to records
   in the telephone number file. Note this functions is particular
   for this file
*/

PUBLIC void advance_to_record(int file_no, char *key, SCREEN_DETS *si, char *key_num)
{
	Db_Obj		*handle;
	Index_Obj	*index;
	char			rec_store[MAX_REC_LENGTH];
	char			*fld_store[MAX_FIELDS];
	char			*key_store[MAX_FIELDS];
   char        key_data[MAX_REC_LENGTH];

	handle = dbfile_handle(file_no);

	if ((index = get_index_handle(handle, key_num)) == (Index_Obj *)NULL)
      return;

    strcpy(key_data, key);
/*   memset(key_data, 0, sizeof(key_data));
   key_data[0] = key; */
   key_store[0] = key_data;
   key_store[1] = &key_data[1];
   key_store[2] = &key_data[1];

   if (find_closest_record(rec_store, fld_store, handle, index, key_store) != ERROR)
    	ptrscpy(si->data, fld_store);
}


/* 	Function:	non_generic_validation
	Rescription:	performs validation at a field specific level
	Inputs:		SCREEN_DETS *
	Outputs:	None
	Return:		TRUE  - validation passed
			FALSE - validation failed
*/

PRIVATE int non_generic_validation(SCREEN_DETS *info)
{
	Db_Obj		*handle;
	Db_Obj		*label_handle;
	Index_Obj	*index;
	Index_Obj	*label_index;
	int		status;
	char		ops_in_telephony[MAX_INDEX_LEN];
	char		config[MAX_INDEX_LEN];
/*
    char        index_store[MAX_INDEX_LEN];
    char        *index_str = index_store;
*/
	char		rec_store[MAX_REC_LENGTH];
	char		*fld_store[MAX_FIELDS];
	char		*key_store[MAX_FIELDS];
	char		*chptr;
	int		i;
	int		config_num;
	int		terminals_in_group;
    unsigned long entered_acbu_ext = (unsigned long)atol(*(info->data + CC_BASE_ACBU_EXT));
    unsigned long rec_acbu_ext;

/*    if (info->key_pressed == RECMOD)
    {
		mark_current_record(info->file_no, info->index);
    }
*/
	switch (info->file_no + 1)
	{
        case ( GROUP_FILE ):

            /*
             * check to make sure that the limit of 50 group page
             * labels has not been exceeded
             */

            if ( info->key_pressed == RECADD )
            {
    			if ( record_count ( info->handle ) >= MAX_GROUP_PAGE_LABELS )
                {
                    show_dberr ( GROUP_PAGE_LIMIT );
                    return ( FALSE );
                }
            }

        break;
        
        case ( OPERATOR_GROUP_FILE ):

            /*
             * check to make sure that the limit of 30 controller group
             * labels has not been exceeded
             */

            if ( info->key_pressed == RECADD )
            {
    			if ( record_count ( info->handle ) >= MAX_CONTROLLER_GROUP_LABELS )
                {
                    show_dberr ( CONTROLLER_LABEL_LIMIT );
                    return ( FALSE );
                }
            }

        break;

      case (SERVICE_PASSWORD_FILE):

        /*
         *      make sure the pin is 3 digits
         */

        if ( strchr ( *( info->data + SP_PIN ) , ( int ) ' ' ))        
        {
            show_dberr ( PIN_TOO_SMALL );
            return ( FALSE );
        }

        /*
         *      make sure the pin doesn't appear in the service personnel file
         */

         if (read_record(PASSWORD_FILE, info->data + SP_PIN, rec_store, fld_store, key2))
         {
		 	show_dberr(PASSWORD_IN_GROUP);
		 	return(FALSE);
		 }

      break;
      case (PASSWORD_FILE):

        /*
         *      make sure the pin is 3 digits
         */

        if ( strchr ( *( info->data + PW_PIN ), ( int ) ' ' ))        
        {
            show_dberr ( PIN_TOO_SMALL );
            return ( FALSE );
        }

        /*
         *      make sure the pin doesn't appear in the service personnel file
         */

        if (read_record(SERVICE_PASSWORD_FILE, info->data + PW_PIN, rec_store, fld_store, key1))
        {
            show_dberr(PASSWORD_IN_SERVICE_GROUP);
            return(FALSE);
        }

      break;
      case (B_PARTY_FILE):
         /* make sure standard, priority and emergency calls are unique */
         status = FALSE;
         if (strcmp(*(info->data + BP_STANDARD_NUM), *(info->data + BP_PRIORITY_NUM)) == (int)NULL)
            status = TRUE;
         if (strcmp(*(info->data + BP_STANDARD_NUM), *(info->data + BP_EMERGENCY_NUM)) == (int)NULL)
            status = TRUE;
         if (strcmp(*(info->data + BP_PRIORITY_NUM), *(info->data + BP_EMERGENCY_NUM)) == (int)NULL)
            status = TRUE;
         
         if (status == TRUE)
         {
				show_dberr(NUMBERS_NOT_UNIQUE);
				return(FALSE);
			}
      break;

		case (CT_CONFIG_FILE):

            /*
               whether adding of modifying, we need to check that there
               are no duplicate Command Terminals in a group
            */

    		for ( i = HC_START_EXT; i < HC_END_EXT + 1; i++ )
    		{
                int     j;
                int     ct_idx;

                /* if field is empty, skip this iteration */
                if ( ! strlen ( * ( info->data + i )))
                    continue;

                ct_idx = atoi ( * ( info->data + i ));

                if ( !ct_idx )
                    continue;

        		for ( j = HC_START_EXT; j < HC_END_EXT + 1; j++ )
                {
                    /* if field is empty, skip this iteration */
                    if ( ! strlen ( * ( info->data + j )))
                        continue;

                    /* don't compare identical indexes */
                    if ( j == i )
                        continue;

                    if ( ct_idx == atoi ( * ( info->data + j )))
                    {
                        show_dberr ( DUPLICATE_CTS );
                        return ( FALSE );
                    }
                }
            }

		   /* 
            *   remove records active assignments and
            *   controller labels that are not needed 
            */

			for (terminals_in_group = 0, i = HC_START_EXT; i <= HC_END_EXT; i++)
				if (*(*(info->data + i)))
					terminals_in_group++;

			/* remove records active assignments and
			   controller labels that are not needed */

			handle = open_database(PRIMARY_LINE_FILE - 1);
			index = get_index_handle(handle, key2);

			label_handle = open_database(OPERATOR_ASSIGN_FILE - 1);
			label_index = get_index_handle(label_handle, key1);

			key_store[0] = *(info->data + HC_OG_INDEX);
			key_store[1] = ops_in_telephony;
			key_store[2] = config;
			for (i = terminals_in_group + 1; i < MAX_TERMINALS_IN_GROUP; i++)
			{
				for (config_num = 1; config_num <= 5; config_num++)
				{
			      		itoa(i, ops_in_telephony, 10);
			      		itoa(config_num, config, 10);

					do
					{
						status = move_file_cursor(FIND, rec_store, fld_store, handle, index, key_store);

						if (status == OK)
							update_file(RECDEL, (char **)0, handle, index);
					}
					while (status == OK);

					ops_in_telephony[0] = (char)(i + 'A');
					ops_in_telephony[1] = 0;
					status = move_file_cursor(FIND, rec_store, fld_store, label_handle, label_index, key_store);
					if (status == OK)
						update_file(RECDEL, (char **)0, label_handle, label_index);
				}
			}

			close_database(label_handle);
			close_database(handle);

		break;
/*      case (OPERATOR_ASSIGN_FILE):
/*			/* check that the default operator is in range */
/* NO LONGER NEEDED JH 7/1/93
/*            default_op = atoi(*(info->data + OL_DEFAULT));
/*
/*            _DEBUG( fprintf (stderr, "DEFAULT OP <%d>\n", default_op); )
/*
/*            if (default_op < 0 || default_op > *(*(info->data + OL_OP_TELE)) - 'A')
/*			{
/*				show_dberr(RANGE_ERR_2);
/*				return(FALSE);
/*			}
/*
/*		break; */

		case (REPORT_CONFIG_FILE):

			mark_current_record(REPORT_CONFIG_FILE - 1, info->index);
			if (record_count(info->handle) >= 1 &&
				info->key_pressed == RECADD)
			{
				show_dberr(ONE_REC_LIMIT);
				return(FALSE);
			}
			find_mark(REPORT_CONFIG_FILE - 1, info->index);

		break;

		case (GEN_COMMS_FILE):

			mark_current_record(GEN_COMMS_FILE - 1, info->index);
			if (record_count(info->handle) >= 1 &&
				info->key_pressed == RECADD)
			{
				show_dberr(ONE_REC_LIMIT);
				return(FALSE);
			}
			find_mark(GEN_COMMS_FILE - 1, info->index);

         if (atoi(*(info->data + CG_MIN_QUEUE)) >= atoi(*(info->data + CG_MAX_QUEUE)))
			{
				show_dberr(EMERGENCY_RANGE);
				return(FALSE);
			}

         if (atoi(*(info->data + CG_AANS_DELAY)) < 1 ||
             atoi(*(info->data + CG_AANS_DELAY)) > 9)
			{
				show_dberr(NUMBER_RANGE);
				return(FALSE);
			}
		break;

		case (OUT_PARTY_LINE_FILE):
			mark_current_record(OUT_PARTY_LINE_FILE - 1, info->index);
            status = record_count_on_keyfield(info->handle, *(info->data + OP_PN_INDEX), OP_PN_INDEX);
/*			status = record_count(info->handle);*/
			find_mark(OUT_PARTY_LINE_FILE - 1, info->index);

            if (status >= MAX_PARTIES_PER_OMNI &&
				info->key_pressed == RECADD)
			{
				show_dberr(PARTY_REC_LIMIT);
				return(FALSE);
			}

			/* an omnibus type party line code consists of
			   digits 3 or 6 */
			read_record(NUMBER_FILE, info->data + OP_PN_INDEX, rec_store, fld_store, key1);
			if (*fld_store[PN_LINE_TYPE] == OMNIBUS_CALL)
			{
				chptr = *(info->data + OP_CCIR_TONE);
				while (*chptr)
				{
					if (*chptr != '3' && *chptr != '6' && *chptr != ' ')
					{
						show_dberr(WRONG_OMNI_DIG);
						return(FALSE);
					}
					chptr++;
				}
			}
		break;

        case ( CT_COMMS_FILE ):

    		mark_current_record ( info->file_no, info->index );

        	status = move_file_cursor(BOIND, rec_store, fld_store, info->handle, info->index, NO_ARGS);

        	while (status == OK)
        	{
                if ( info->key_pressed == RECMOD )
                {
                    /* if we are modifying a record skip over the current record */
                    if ( ! strcmp ( fld_store[CC_INDEX], *(info->data + CC_INDEX )))
                    {
        		        status = move_file_cursor(NEXT, rec_store, fld_store, info->handle, info->index, NO_ARGS);
                        continue;
                    }
                }

                rec_acbu_ext = (unsigned long) atol ( fld_store[CC_BASE_ACBU_EXT] );
                if ( labs ( (long) ( entered_acbu_ext - rec_acbu_ext ) ) < 8L )
                {
                	find_mark(info->file_no, info->index);
                    show_dberr(MONITOR_DEFINED);
                    return(FALSE);
                }    
                
        		status = move_file_cursor(NEXT, rec_store, fld_store, info->handle, info->index, NO_ARGS);
        	}

            find_mark(info->file_no, info->index);

        break;

        case ( GROUP_CONF_FILE ):

            /*
               whether adding of modifying, we need to check that there
               are no duplicate telephone numbers in a confernence
            */

    		for ( i = 0; i < MAX_AMBA_MEMBERS; i++ )
    		{
                int     j;
                int     phone_idx;
                int     conf_type;

                /* if field is empty, skip this iteration */
                if ( ! strlen ( * ( info->data + GC_PN_INDEX + i )))
                    continue;

                phone_idx = atoi ( * ( info->data + GC_PN_INDEX + i ));
                conf_type = atoi ( * ( info->data + START_CONF_TYPE + i ));

                if ( !phone_idx )
                    continue;

                for ( j = 0; j < MAX_AMBA_MEMBERS; j++ )
                {
                    /* if field is empty, skip this iteration */
                    if ( ! strlen ( * ( info->data + GC_PN_INDEX + j )))
                        continue;

                    /* don't compare identical indexes */
                    if ( j == i )
                        continue;

                    if ( phone_idx == atoi ( * ( info->data + GC_PN_INDEX + j )))
                    {
                        if ( conf_type == atoi ( * ( info->data + START_CONF_TYPE + j )))
                        {
                            show_dberr ( DUPLICATE_PHONES );
                            return ( FALSE );
                        }
                    }
                }
            }
        break;

	}
	return(TRUE);
}

/* 	Functions 	process_requests()
	Rescription: 	The user can only do what the buttons dictate
			(apart from data entry) and here is where we
			handle it (add, delete etc)
	Inputs:		si - pointer to type SCREEN_DETS built in
				data_entry()
	Outputs:	None
	Return:		TRUE - operation OK
			FALSE - user wants to exit or error
*/

PRIVATE int previous_errnum = OK;

PUBLIC int process_requests(SCREEN_DETS *si)
{
	char		rec_store[MAX_REC_LENGTH];
	char		*fld_store[MAX_FIELDS];
	int		status;
	char		*key_data[MAX_SIG_KEY_FLDS];
	char		key_store[MAX_SIG_KEY_FLDS][MAX_FIELD_LEN];
	char		result[MAX_INDEX_LEN];
	int		i, num_keys;
	Index_Obj	*key_index;
   UNIQUE_KEY  *uniq_key;

	switch (si->key_pressed)
	{
		case (BOIND):
		case (EOIND):
		case (NEXT):
		case (PREVIOUS):
		case (FIND):
		case (RECCURRENT):
		       	previous_errnum = status = move_file_cursor(si->key_pressed, rec_store, 
		       		fld_store, si->handle, si->index, key_data);

			if (status != OK)
			{
				fld_store[0] = (char *)NULL;
				clear_form();
			}
			else
				/* copy data back for screen re-display */
				ptrscpy(si->data, fld_store);

 			show_dberr(status);
		break;
		case (RECADD):
			/* do some specific field validation */
			if (!non_generic_validation(si))
				break;

			/* check for a duplicate key */
         for (num_keys = 0; num_keys < MAX_UNIQUE_KEYS; num_keys++)
         {
            uniq_key = &significant_key[si->file_no][num_keys];

            if (uniq_key->num_flds == 1)
            {
               /* if key consists of 1 field, and it is all spaces, then ignore */
               if (all_spaces(*(si->data + uniq_key->fld_offset[0]), strlen(*(si->data + uniq_key->fld_offset[0]))))
                  continue;
            }

   			if (uniq_key->num_flds)
	   		{
		   		for (i = 0; i < uniq_key->num_flds; i++)
			   	{	
   					strcpy(key_store[i], *(si->data + uniq_key->fld_offset[i]));				
	   				key_data[i] = key_store[i];
			   	}
         
   				key_index = get_index_handle(si->handle, uniq_key->key_num);
	   		       	status = move_file_cursor(FIND, rec_store, 
		   	       		fld_store, si->handle, key_index, key_data);


   				if (status == OK)
	   			{     	
                    show_dberr(DUPLICATE);
                    return(TRUE);
                }
   			}
         }

			if (update_mindex(ass_indexes[si->file_no], result)) 
				strcpy(*(si->data + inx_pos[si->file_no]), result);
		case (RECMOD):

/*      need to put code to check as above except for main key */

			if (!non_generic_validation(si))
      		break;
		case (RECDEL):

			if (si->key_pressed == RECDEL)
			{
                if ( previous_errnum == EOI || previous_errnum == BOI )
                    /* user is at head or end of file, so delete requests
                     * can be ignored
                     */
                     return ( TRUE );

				if (!confirm_delete())
					return(TRUE);

                if (!remove_related_records(si->file_no, si->data))
                {
                   show_dberr(NON_DELETABLE_REC);
                   return(TRUE);
                }

				delete_mindex(ass_indexes[si->file_no], 
					*(si->data + inx_pos[si->file_no]));
			}

			previous_errnum = status = update_file(si->key_pressed, (char **)si->data,
				si->handle, si->index);

			show_dberr(status); 

			si->key_pressed = RECCURRENT;
			/* re-fetch record */
			process_requests(si);
		break;
		case (NO_OPERATION):
			return(TRUE);
		case (EXIT):
			return(FALSE);
		default:
			/* if exit */
			return(TRUE);
	}

    if ( previous_errnum == BOI )
        display_oob ( " Beginning of File " );
    else 
    if ( previous_errnum == EOI )
        display_oob ( " End of File " );
    else
        drop_oob ();

	return(TRUE);
}	

/* 	Functions 	data_entry()
	Rescription: 	high level function called when user wishes to
			do some data entry. Makes the type SCREEN_DETS
			for use by lower level functions
	Inputs:		option - reference number as to which data the
				user wishes to edit. Directly corresponds
				with the item number choosen from the menu
	Outputs:	None	
	Return:		TRUE - (function not complete)
*/


PUBLIC int data_entry(int option)
{
	SCREEN_DETS	scn_info;
	int		first_iteration = TRUE;

	scn_info.handle = open_database(option);
	scn_info.index = get_index_handle(scn_info.handle, display_order[option]);
	scn_info.file_no = option;
	popup_form(&scn_info);

	do
	{
		form_active(&scn_info);
		if (!first_iteration)
			scn_info.key_pressed = map_key(command_dispatcher(0));
		else 
		{
			/* fetch the first record when screen
			   first displayed */
			scn_info.key_pressed = BOIND;
			first_iteration = FALSE;
		}
    } while (process_requests(&scn_info));

    drop_oob ();

	/* pop down screen */
	popdown_form();

	close_database(scn_info.handle);

	return(TRUE);
}

