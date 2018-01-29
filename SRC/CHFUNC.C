#pragma pack (1)
/*
	chfunc.c

    System  :  Train Control Command Communication System
    Author  :  Ansa Pty. Ltd. (c) 1991
    Revision:  1.0
*/

#include <stdlib.h>
#include <string.h>

#include "sysdef.h"
#include "field.h"
#include "forms.h"
#include "filename.h"
#include "formdef.h"
#include "loaddb.h"
#include "getidx.h"
#include "err.h"
#include "primass.h"
#include "lists.h"
#include "win_util.h"
#include "win_hues.h"
#include "scroller.h"

PRIVATE int mask_flag;

/* 	Function:	select_dbrec()
	Rescription:	reads all relevant record types and pops
			up a window allowing the user to select from
			a choice list. This function is used when the
			user presses Enter on a choice field and wishes
			to choose from a list (eg extension numbers). This
			function is not called directly, but from the
			functions bwlow.
	Inputs:		rectype - the file from which to extract the
			labels that make up the choice list
	Outputs:	None
	Return:		None
*/

PRIVATE void select_dbrec(int rectype)
{
	int	choice;
	char	*ptr = 0;
	char	**pptr = 0;
	int	no_recs;

	no_recs = load_file(rectype, &ptr, &pptr, mask_flag);
	if (no_recs)
	{
		choice = Smt_FormScroller((char *)0, AUTO, AUTO, AUTO, AUTO, POPUP_NORMAL, POPUP_HILITE, pptr, 1, cf, cfld);

		if (choice != ERROR)
		{
			get_index(rectype, *(pptr + choice) + MARGIN, cfld->data, CHOICE1);

			*cfld->table = *cfld->data;
			strcpy(*cfld->chtext, *(pptr + choice) + MARGIN);

		}
	}
	else
	{
      if (rectype == CT_COMMS_FILE && mask_flag == CT_CONFIG_FILE)
			show_dberr(NO_CTS_DEFINED);
      else
		if (rectype == CT_COMMS_FILE && mask_flag)
			show_dberr(CTS_DEFINED);
		else
            show_dberr(NO_RECORDS);
	}
}

/* 	Function:	get_choice()
	Rescription:	performs the same function as select_dbrec() but
			uses a statically declared list to build the
			choice list.
	Inputs:		None
	Outputs:	None
	Return:		None
*/

/*
	the following table is used for making a real time
	choice list for the number of controllers in telephony 
*/

PRIVATE char	var_list_store[10][3];
PRIVATE	char	*var_list[10] = 
{
	var_list_store[0],
	var_list_store[1],
	var_list_store[2],
	var_list_store[3],
	var_list_store[4],
	var_list_store[5],
	var_list_store[6],
	var_list_store[7],
	var_list_store[8],
	(char *)NULL
};

PUBLIC void get_choice()
{
	int	choice;
	int	controllers_in_grp;
	int	i;

    if(cfld->table == conttele_vals)
	{
		/* specifically for controller in telephony field
		   in the controller labels screen. The choice list
		   will comprise only of a selection of how many
		   controllers are in the selected controller group */
		memset(var_list_store, 0, sizeof(var_list_store));
		memset(var_list, 0, sizeof(var_list));
		if (*(cf->cf->data))
		{
			controllers_in_grp = no_of_operators(cf->cf->data);
			for (i = 2; i <= controllers_in_grp; i++)
			{
				itoa(i, var_list_store[i-2], 10);
				var_list[i-2] = var_list_store[i-2];
			}
		}
		choice = Smt_FormScroller((char *)0, AUTO, AUTO, AUTO, AUTO, POPUP_NORMAL, POPUP_HILITE, var_list, 1, cf, cfld);
	}
    else if(cfld->table == default_vals)
    {
		memset(var_list_store, 0, sizeof(var_list_store));
		memset(var_list, 0, sizeof(var_list));
		if (*(cf->cf->data))
		{
			controllers_in_grp = no_of_operators(cf->cf->data);
            for (i = 0; i < controllers_in_grp; i++)
			{
                itoa(i+1, var_list_store[i], 10);
                var_list[i] = var_list_store[i];
			}
		}
		choice = Smt_FormScroller((char *)0, AUTO, AUTO, AUTO, AUTO, POPUP_NORMAL, POPUP_HILITE, var_list, 1, cf, cfld);
    }
	else
		choice = Smt_FormScroller((char *)0, AUTO, AUTO, AUTO, AUTO, POPUP_NORMAL, POPUP_HILITE, cfld->chtext, 1, cf, cfld);

	if (choice != ERROR)
		*(cfld->data) = *(cfld->table + choice);
}

/* 	Function:	see below
	Rescription:	these functions are called from function pointers
			set up in each form where these is a choice lists.
			So, if the user presses Enter on a choice list,
			one of the below functions will be called indirectly,
			depending on which type of data is to comprise
			the choice list
	Inputs:		None
	Outputs:	None
	Return:		None
*/

PUBLIC void sel_group()
{
	select_dbrec(GROUP_FILE);
}

PUBLIC void sel_phone()
{
	mask_flag = FALSE;
	select_dbrec(NUMBER_FILE);
}

PUBLIC void sel_ecbu()
{
	select_dbrec(ECBU_MAGAZINE_FILE);
}

PUBLIC void sel_phone_internal()
{
	mask_flag = D_PARTY_FILE;
	select_dbrec(NUMBER_FILE);
}

PUBLIC void sel_phone_inpartyln()
{
	mask_flag = IN_PARTY_LINE;
	select_dbrec(NUMBER_FILE);
}

PUBLIC void sel_phone_outpartyln()
{
	mask_flag = OUT_PARTY_LINE;
	select_dbrec(NUMBER_FILE);
}

PUBLIC void sel_phone_grpconf()
{
	mask_flag = GROUP_CONF_FILE;
	select_dbrec(NUMBER_FILE);
}

PUBLIC void sel_terminal_grpconf()
{
	mask_flag = GROUP_CONF_FILE;
	select_dbrec(CT_COMMS_FILE);
}

PUBLIC void sel_opgrp()
{
   mask_flag = FALSE;
	select_dbrec(OPERATOR_GROUP_FILE);
}

PUBLIC void sel_opgrp_mask()
{
   mask_flag = B_PARTY_FILE;
	select_dbrec(OPERATOR_GROUP_FILE);
}

PUBLIC void sel_opgrp_ctgroup()
{
   mask_flag = CT_CONFIG_FILE;
	select_dbrec(OPERATOR_GROUP_FILE);
}

PUBLIC void sel_opgrp_label()
{
   mask_flag = -CT_CONFIG_FILE;
	select_dbrec(OPERATOR_GROUP_FILE);
}

PUBLIC void sel_extn()
{
	mask_flag = FALSE;
	select_dbrec(CT_COMMS_FILE);
}

PUBLIC void sel_extn_selcall()
{
	mask_flag = D_PARTY_FILE;
	select_dbrec(CT_COMMS_FILE);
}

PUBLIC void sel_extn_group()
{
	mask_flag = CT_CONFIG_FILE;
	select_dbrec(CT_COMMS_FILE);
}

PUBLIC void sel_extn_mask()
{
	mask_flag = TRUE;
	select_dbrec(CT_COMMS_FILE);
}

PUBLIC void sel_pin()
{
	select_dbrec(PASSWORD_FILE);
}

