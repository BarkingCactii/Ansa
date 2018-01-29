#pragma pack (1)
/*
	forms.c

    System  :  Train Control Command Communication System
    Author  :  Ansa Pty. Ltd. (c) 1991
    Revision:  1.0
*/

#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <ibmkeys.h>
#include <asciidef.h>

#include "sysdef.h"
#include "boxes.h"
#include "filename.h"
#include "field.h"
#include "loaddb.h"
#include "formdef.h"
#include "form_exp.h"
#include "getidx.h"
#include "err.h"
#include "dentry.h"

#include "smt_idle.h"
#include "smt_help.h"
#include "win_hues.h"

PUBLIC int form_active(SCREEN_DETS *);

PUBLIC FORM 	*cf;
PUBLIC FIELD	*cfld;

PUBLIC  int     partyln_defined;

PRIVATE HWND    wn;
PRIVATE HWND    twn;

PRIVATE int     pos_partyln_defined;

#define	HASH	'#'
#define STAR	'*'
#define COMMA	','

PRIVATE SCREEN_DETS *local_dets;

typedef struct 
{
   unsigned   alt_key;
   char       search_key;
} search_item;

PRIVATE search_item     search_table[] =
{
    { ALTA,    'a' },
    { ALTB,    'b' },
    { ALTC,    'c' },
    { ALTD,    'd' },
    { ALTE,    'e' },
    { ALTF,    'f' },
    { ALTG,    'g' },
    { ALTH,    'h' },
    { ALTI,    'i' },
    { ALTJ,    'j' },
    { ALTK,    'k' },
    { ALTL,    'l' },
    { ALTM,    'm' },
    { ALTN,    'n' },
    { ALTO,    'o' },
    { ALTP,    'p' },
    { ALTQ,    'q' },
    { ALTR,    'r' },
    { ALTS,    's' },
    { ALTT,    't' },
    { ALTU,    'u' },
    { ALTV,    'v' },
    { ALTW,    'w' },
    { ALTX,    'x' },
    { ALTY,    'y' },
    { ALTZ,    'z' }

};

#define SEARCH_TABLE_SZ (sizeof(search_table) / sizeof(search_item))

PRIVATE void process_search(unsigned key)
{
   int   i;
   char    key_fld[MAX_FIELD_LEN];

   if (local_dets->file_no != NUMBER_FILE - 1)
      return;

    memset((void *)key_fld, 0, sizeof(key_fld));
    for (i = 0; i < SEARCH_TABLE_SZ; i++)
    {
        if (search_table[i].alt_key == key)
        {
            key_fld[0] = search_table[i].search_key;
            advance_to_record(NUMBER_FILE - 1, key_fld, local_dets, key2);
            form_active(local_dets);
            break;
        }
    }
}

/* 	Function:	all_spaces()
	Rescription:	determines whether a string contains all spaces
	Inputs:		str - the string to check
			len - length of the string
	Outputs:	None
	Return:		TRUE - string is all spaces
			FALSE - string contains some characters other
				than spaces
*/

PUBLIC int all_spaces(char *str, int len)
{
	int	i;
	int	retval = TRUE;

	for (i = 0; i < len; i++)
		if (*(str+i) != ' ')
			retval = FALSE;

	return(retval);
}

/* 	Function:	clear_form()
	Rescription:	clears a form on the screen and sets up any
			applicable default choices
	Inputs:		None
	Outputs:	None
	Return:		None
*/

PUBLIC void clear_form( void )
{
	FIELD	*fldptr;
	int	i;

	memset(field, 0, sizeof(field));

	fldptr = cf->cf;

	for (i = 0; i < cf->nf; i++, fldptr++)
	{	
   	memset(field[i], ' ', MAX_FIELD_LEN);
      if (fldptr->ft == CHOICE || fldptr->ft == NON_MANDATORY_CHOICE)
		{	memset(field[i], 0, MAX_FIELD_LEN);
			/* shove in default */
	 		field[i][0] = fldptr->def;
		}

		/* terminate field */
		field[i][fldptr->fl] = 0;
	}		

	/* terminate next field */
	field[i][0] = 0;
}

/*  Function:   display_defaults()
	Rescription:	now that the defaults have been setup by
			clear form, we will display these defaults on
			the screen
	Inputs:		None
	Outputs:	None
	Return:		None
*/

PRIVATE void display_defaults( void )
{
	int	i = 0;
	int	j;

	/* clear field first */
	for (j = cfld->fc; j < cfld->fc + cfld->fl; j++)
		vratputc(wn, cfld->fr, j, FIELD_NORMAL, ' ');

	if (!(*cfld->data))
		/* no default */
		return;

	/* find a match between what is stored in the current form
	   and a list of possible defaults */
	while (*cfld->data != *(cfld->table + i))
		i++;

	/* display text */
 	vratputs(wn, cfld->fr, cfld->fc, FIELD_NORMAL, *(cfld->chtext + i));
}

/* 	Function:	popup_form()
	Rescription:	makes the screen visible on screen
	Inputs:		dets - pointer to type SCREEN_DETS
	Outputs:	None
	Return:		TRUE - ok
*/

PUBLIC int popup_form(SCREEN_DETS *dets)
{	int	i;
	FIELD	*fldptr;


   local_dets = dets;      /* copy for local reference */

	draw_all_boxes();

	cf = &forms[dets->file_no];

	wn = vcreat(cf->nr, cf->nc, WINDOW_NORMAL, YES);
	vwind(wn, cf->nr, cf->nc, 0, 0);
	vlocate(wn, cf->top, cf->left);
	vframe(wn, WINDOW_NORMAL, FRDOUBLE);
	visible(wn, YES, YES);

   /* display title in box */
   vtitle(wn, WINDOW_NORMAL, file_description[dets->file_no]);

	for (fldptr = cf->cf, i = 0; i < cf->nf; i++, fldptr++)
		vratputs(wn, fldptr->pr, fldptr->pc, WINDOW_NORMAL, fldptr->prompt); 
    clear_form();

    Smt_PushHelp(cf->cf->help); 

	return(TRUE);
}

/* 	Function:	popdown_form()
	Rescription:	removes the current form from the screen as well
			as the function boxes
	Inputs:		None
	Outputs:	None
	Return:		TRUE - ok
*/

PUBLIC int popdown_form()
{
	vdelete(wn, NONE);
	vdelete(twn, NONE);
	close_all_boxes();

    Smt_PopHelp(); 

	return(TRUE);
}

/* 	Function:	clear_fields()
	Rescription:	clear the fields on the current form to spaces.
			This is only a visual clear. The contents of the
			current record are not altered
	Inputs:		None
	Outputs:	None
	Return:		None
*/

PRIVATE void clear_fields( void )
{	
	int	i, k;
	FIELD	*tmpfld;

	for (tmpfld = cf->cf, i = 0; i < cf->nf; i++, tmpfld++)
	{
	 	/* clear fields visually */
		for (k = tmpfld->fc; k < tmpfld->fc + tmpfld->fl; k++)
			vratputc(wn, tmpfld->fr, k, FIELD_NORMAL, ' ');
	}
}

/* 	Function:	hilite_curr_field()
	Rescription:	higlights the current field with the 
			specified attribute
	Inputs:		attr - the attribute the current field is
			to be highlighted in
	Outputs:	None
	Return:		None
*/

PRIVATE void hilite_curr_field(int attr)
{
	int	j;

	for (j = cfld->fc; j < cfld->fc + cfld->fl; j++)
		vatputa(wn, cfld->fr, j, attr); 
}

PRIVATE char phone_indexes_in_conference[MAX_FIELDS + 1][MAX_INDEX_LEN];
PRIVATE int  conference_idx_type[MAX_FIELDS];

PUBLIC int duplicate_phone(char *phone_index, int from, int to, int type)
{
	int	i;

	for (i = from; i < to; i++)
   {
      if (conference_idx_type[i] == type)
      {
   		if (!strcmp(phone_index, phone_indexes_in_conference[i]))
	   		return(TRUE);
      }
   }

	return(FALSE);
}

/* 	Function:	form_active()
	Rescription:	puts the current record on screen, displays 
			labels from indexes, displays the cursor and
			highlights the current field
	Inputs:		dets - pointer to type SCREEN_DETS
	Outputs:	None
	Return:		TRUE - ok
*/

PUBLIC int form_active(SCREEN_DETS *dets)
{
	int	i;

	memset(phone_indexes_in_conference, 0, sizeof(phone_indexes_in_conference));
	memset(conference_idx_type, 0, sizeof(conference_idx_type));

	/* copy field contents to screen and terminate */
    clear_fields();

	partyln_defined = FALSE;
	pos_partyln_defined = -1;

	for (cfld = cf->cf, i = 0; i < cf->nf || cf->nf == 0; i++, cfld++)
	{
      if (cfld->indexval && (*cfld->data))
		{
			if (dets->file_no == GROUP_CONF_FILE - 1 && i >= START_CONF_NUM)
   			find_index(atoi(*(dets->data + START_CONF_TYPE + (i - START_CONF_NUM))), 0, *cfld->chtext, cfld->data);
                            /* was cfld */                                          /* was 0 */
         else
   			find_index(cfld->indexval, 0, *cfld->chtext, cfld->data);

			*cfld->table = *cfld->data;

			/* special processing for group conferences */
			if (dets->file_no == GROUP_CONF_FILE - 1)
			{
				strcpy(phone_indexes_in_conference[i], cfld->data);
    			conference_idx_type[i] = atoi(*(dets->data + START_CONF_TYPE + (i - START_CONF_NUM)));
                                               /* was cfld */
				if (partyln_defined && (pos_partyln_defined == -1))
					pos_partyln_defined = i;
			}

		}
		if (cfld->ft == CHOICE || cfld->ft == NON_MANDATORY_CHOICE)
	 		display_defaults();
		else	
		{	
			vratputs(wn, cfld->fr, cfld->fc, FIELD_NORMAL, cfld->data);
			memset(cfld->data + strlen(cfld->data), ' ', MAX_FIELD_LEN - strlen(cfld->data));
			*(cfld->data + cfld->fl) = 0;
		}
		cfld->cc = 0;
	}

	/* attach hardware cursor to logical cursor */
	visible(wn, YES, YES);
	curdrag(wn, ON);

	/* highlight current input field and set cursor */

	cfld = cf->cf;
	hilite_curr_field(FIELD_HILITE);
 	lcurset(wn, cfld->fr, cfld->fc + cfld->cc);

	dets->data = record;

	return(TRUE);
}

/* 	Function:	valid_key()
	Rescription:	checks if a key pressed is a valid key in view
			of its mask.
	Inputs:		ch - character pressed
			mask - the character type mask to check
	Outputs:	None
	Return:		TRUE - valid character for the specified mask
			FALSE - invalid character
*/

PRIVATE	int valid_key(int ch, int mask)
{
	if (ch > 0x007f)
		return(FALSE);

	if (ch < 0)
		return(FALSE);

	switch (mask)
	{
		case ('X'):
			return(isprint(ch));
		case ('3'):
			/* allow the digits 0 - 3 */
			if (ch < '0' || ch > '3')
				return(FALSE);
			else
				return(TRUE);
		case ('6'):
			if (ch != '3' && ch != '6')
				return(FALSE);
			/* do the '9' test as well */
		case ('N'):
 			if (cfld->cc && ch == HASH)
				return(TRUE);
 			if (cfld->cc && ch == STAR)
				return(TRUE);
 			if (cfld->cc && ch == COMMA)
				return(TRUE);
			return(isdigit(ch));
		case ('D'):
			return(isdigit(ch));
	}
	return(FALSE);
}

/* 	Function:	process_key()
	Rescription:	handles keys pressed by the user apart from
			those assigned to function boxes. This mainly
			consists of field movement and field editing
			actions
	Inputs:		k - character pressed
	Outputs:	None
	Return:		k - character pressed (may be re-mapped on
			    certain situations
*/


PUBLIC unsigned process_key(unsigned k)
{	
	int	i;
	FIELD	*tmpfld;

   Smt_PopHelp();

   process_search(k);

 	switch(k)
 	{
      case (F1):
		case (F2):
			/* before wa pass add and modify keys on
			   we want to see now if the record is valid
			   or not */
			for (i = 0; i < cf->nf; i++)
			{	tmpfld = cf->cf + i;
				if (!(*(tmpfld->data)) && tmpfld->ft == CHOICE)
					break;
				if (all_spaces(tmpfld->data, tmpfld->fl) && tmpfld->ft == ENTRY)
					break;
			}
			if (i < cf->nf)
			{
				show_dberr(FORM_INCOMPLETE);
	 			hilite_curr_field(FIELD_NORMAL);
	 			cfld = cf->cf + i;
	 			hilite_curr_field(FIELD_ERROR);
	 			cfld->cc = 0;
				k = NO_OPERATION;
			}
		break;
      case (LF):
         if (cfld->func2)
         {
				/* the following is possible only if we are not
				   on the first field on the form (which we aren't
				   for this specific group conference test) */
				if (pos_partyln_defined == cfld->pf + 1)
				{
					partyln_defined = FALSE;
					pos_partyln_defined = -1;
				}

				(*cfld->func2)();

				/* group conference check */
				if (local_dets->file_no == GROUP_CONF_FILE - 1 && cfld->indexval2 && ( cfld->nf > START_CONF_NUM || cfld->nf == 0 ))
				{
/*					itoa(CT_COMMS_FILE, *(local_dets->data + START_CONF_TYPE + (cfld->nf - START_CONF_NUM - 1)), 10);  */
					itoa(CT_COMMS_FILE, *(local_dets->data + START_CONF_TYPE + (cfld->pf + 1 - START_CONF_NUM)), 10); 

					strcpy(phone_indexes_in_conference[cfld->pf + 1], cfld->data);
                    conference_idx_type[cfld->pf + 1] = CT_COMMS_FILE; /* was cfld */
					find_index(cfld->indexval2, 0, *cfld->chtext, cfld->data);
					if (partyln_defined && pos_partyln_defined == -1)
						pos_partyln_defined = cfld->pf + 1;
				}
				
				display_defaults();
				hilite_curr_field(FIELD_HILITE);
         }
      break;
		case (CR):
			if (cfld->func)
			{
				/* the following is possible only if we are not
				   on the first field on the form (which we aren't
				   for this specific group conference test) */
				if (pos_partyln_defined == cfld->pf + 1)
				{
					partyln_defined = FALSE;
					pos_partyln_defined = -1;
				}

				(*cfld->func)();

				/* group conference check */
				if (local_dets->file_no == GROUP_CONF_FILE - 1 && cfld->indexval && ( cfld->nf > START_CONF_NUM || cfld->nf == 0 ))
				{
/*					itoa(NUMBER_FILE, *(local_dets->data + START_CONF_TYPE + (cfld->nf - START_CONF_NUM - 1)), 10); */
					itoa(NUMBER_FILE, *(local_dets->data + START_CONF_TYPE + (cfld->pf + 1 - START_CONF_NUM)), 10); 

					strcpy(phone_indexes_in_conference[cfld->pf + 1], cfld->data);
                    conference_idx_type[cfld->pf + 1] = NUMBER_FILE;
					find_index(cfld->indexval, 0, *cfld->chtext, cfld->data);
					if (partyln_defined && pos_partyln_defined == -1)
						pos_partyln_defined = cfld->pf + 1;
				}
				
				display_defaults();
				hilite_curr_field(FIELD_HILITE);
			}
			else
				process_key(CURDN);
		break;
 		case (CURUP):	    
		case (REVTAB):
 			hilite_curr_field(FIELD_NORMAL);
 			cfld = cf->cf + cfld->pf;
 			hilite_curr_field(FIELD_HILITE);
 			cfld->cc = 0;
 		break;
 		case (CURDN):
		case (HT):
 			hilite_curr_field(FIELD_NORMAL);
 			cfld = cf->cf + cfld->nf;
 			hilite_curr_field(FIELD_HILITE);
 			cfld->cc = 0;
 		break;
 		case (CURRT):
 			if (cfld->cc < cfld->fl - 1)
 				cfld->cc++;
 		break;
 		case (CURLF):
 			if (cfld->cc)
 				cfld->cc--;
 		break;
		case (BS):
  			if (cfld->cc)
    			cfld->cc--;
		case (DELETE):
			/* the following is possible only if we are not
			   on the first field on the form (which we aren't
			   for this specific group conference test */
			if (pos_partyln_defined == cfld->pf + 1)
			{
				partyln_defined = FALSE;
				pos_partyln_defined = -1;
			}

			/* if we delete on a choice field, we want to 
			   delete the field, not the record */

			if ((cfld->ft == CHOICE || cfld->ft == NON_MANDATORY_CHOICE) && cfld->indexval)
			{
				memset(phone_indexes_in_conference[cfld->pf + 1], 0, MAX_INDEX_LEN);
				conference_idx_type[cfld->pf + 1] = 0;
            
				*(cfld->data) = 0;
				display_defaults();
				break;
			}

    			/* remove the offending character */
    			*(cfld->data + cfld->cc) = ' ';	
    			vratputc(wn, cfld->fr, cfld->fc + cfld->cc, FIELD_HILITE, ' ');

    			/* move remainder of string down to fill the gap */
    			for (i = cfld->cc; i < cfld->fl - 1; i++)
    			{	*(cfld->data + i) = *(cfld->data + (i + 1));	
     				vratputc(wn, cfld->fr, cfld->fc + i, FIELD_HILITE, *(cfld->data + (i + 1)));
    			}
    			*(cfld->data + i) = ' ';	
    			vratputc(wn, cfld->fr, cfld->fc + i, FIELD_HILITE, ' ');
		break;
 		default:
			if (cfld->ft == CHOICE || cfld->ft == NON_MANDATORY_CHOICE)
				break;
 			if (valid_key(k, cfld->fm))
 			{
				if (cfld->cc)
					/* only add the key if the key before is valid;
				   	   this prevents users from embedding spaces in
				   	   numeric fields */
					if (!valid_key((int)*(cfld->data + cfld->cc - 1), cfld->fm))
						break;

 				vratputc(wn, cfld->fr, cfld->fc + cfld->cc, FIELD_HILITE, (char)k);
 				*(cfld->data + cfld->cc) = (char)k;
 				if (cfld->cc < cfld->fl - 1)
 					cfld->cc++;
 				else
 					process_key(CURDN);
 			}
 	       	break;
 	}
	visible(wn, YES, YES);
	curdrag(wn, ON);
 	lcurset(wn, cfld->fr, cfld->fc + cfld->cc);

    Smt_PushHelp(cfld->help);

	return(k);
}
