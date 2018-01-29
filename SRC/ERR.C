#pragma pack (1)
/*
	err.c

    System  :  Train Control Command Communication System
    Author  :  Ansa Pty. Ltd. (c) 1991
    Revision:  1.0
*/

#include <ctype.h>
#include <io.h>

#include "sysdef.h"

#include "win_hues.h"
#include "smt_idle.h"

/*
 * Out of bounds display window
 * This window is displayed when beginning or end of file is reached
 */

PRIVATE HWND oob_win;
PRIVATE int  oob_up = FALSE;

PUBLIC void drop_oob()
{
    if (!oob_up)
        return;

	vdelete(oob_win, NONE);

    oob_up = FALSE;
}

PUBLIC void display_oob ( char *message )
{
    if ( oob_up )
        return;

	oob_win = vcreat ( 1, -1, REVERR, YES );
	vwind ( oob_win, 1, -1, 0, 0 );
	vlocate ( oob_win, 24, 0 );
	visible ( oob_win, YES, YES );

    vratputs ( oob_win, 0, ( 80 - strlen ( message )) / 2, REVERR, message );

    oob_up = TRUE;
}

/*
 * please wait
 * accessing database window routines
 */

PRIVATE HWND wait_win;
PRIVATE int  wait_up = FALSE;

PUBLIC void drop_wait()
{
    
    if (!wait_up)
        return;

	vdelete(wait_win, NONE);

    wait_up = FALSE;
}

PUBLIC void display_wait()
{
    if (wait_up)
        return;

	wait_win = vcreat(8, 40, ERROR_WINDOW, YES);
	vwind(wait_win, 2, 30, 0, 0);
	vlocate(wait_win, 10, 25);
	vframe(wait_win, ERROR_WINDOW, FRDOUBLE);
	visible(wait_win, YES, YES);

	vratputs(wait_win, 0, 3, WAIT_WINDOW,  "      Please wait...    ");
	vratputs(wait_win, 1, 3, ERROR_WINDOW, "  Accessing the database ");

    wait_up = TRUE;
}

/* 	Function:	delete_method()
	Rescription:	pops up a window when the user pressed the 
			Delete function (F9) requesting the type a
			deletion; (shifting up line keys or blanking
			the current line key) this function is used
			only for passive line assignments
	Inputs:		None
	Outputs:	None
	Return:		'S' - shift up remaining line keys
			'B' - blank current line key
*/

PUBLIC int delete_method()
{
	HWND	win;
	int	ch;

	win = vcreat(6, 40, ERROR_WINDOW, YES);
	vwind(win, 6, 40, 0, 0);
	vlocate(win, 8, 20);
	vframe(win, ERROR_WINDOW, FRDOUBLE);
	visible(win, YES, YES);

	vratputs(win, 1, 3, ERROR_WINDOW, "     Do you wish the delete to:");
	vratputs(win, 2, 3, ERROR_WINDOW, "(S)hift up remaining key labels, or");
	vratputs(win, 3, 3, ERROR_WINDOW, "(B)lank the current key label only");
	vratputs(win, 4, 3, ERROR_WINDOW, "          Press (S) or (B)");

	ch = Smt_KbdGet();
	ch = toupper(ch);
	vdelete(win, NONE);

	if (ch == 'S')
		return(TRUE);
	else
		return(FALSE);
}

/* 	Function:	confirm_delete_page()
	Rescription:	pops up a confirmation on whether the user
			really wants to clear the current page. This
			functions is used only for passive line assignments
	Inputs:		None
	Outputs:	None
	Return:		TRUE = really delete
			FALSE - cancel delete
*/

PUBLIC int confirm_delete_page()
{
	HWND	win;
	int	ch;

	win = vcreat(8, 40, ERROR_WINDOW, YES);
	vwind(win, 8, 40, 0, 0);
	vlocate(win, 7, 20);
	vframe(win, ERROR_WINDOW, FRDOUBLE);
	visible(win, YES, YES);

	vratputs(win, 1, 3, WAIT_WINDOW, "            WARNING!!");
	vratputs(win, 3, 3, ERROR_WINDOW, "This PAGE is about to be deleted");
	vratputs(win, 6, 3, ERROR_WINDOW, "       ARE YOU SURE ? (Y/N)");

	vbeep();
	ch = Smt_KbdGet();
	ch = toupper(ch);
	vdelete(win, NONE);

	if (ch == 'Y')
		return(TRUE);
	else
		return(FALSE);
}

/* 	Function:	confirm_delete()
	Rescription:	pops up a confirmation on whether the user
			really wants to delete the current record
	Inputs:		None
	Outputs:	None
	Return:		TRUE = really delete
			FALSE - cancel delete
*/

PUBLIC int confirm_delete()
{
	HWND	win;
	int	ch;

	win = vcreat(8, 40, ERROR_WINDOW, YES);
	vwind(win, 8, 40, 0, 0);
	vlocate(win, 7, 20);
	vframe(win, ERROR_WINDOW, FRDOUBLE);
	visible(win, YES, YES);

	vratputs(win, 1, 3, WAIT_WINDOW, "            WARNING!!");
	vratputs(win, 2, 3, ERROR_WINDOW, "This record is about to be deleted");
	vratputs(win, 3, 3, ERROR_WINDOW, "  as well as EVERY record which");
	vratputs(win, 4, 3, ERROR_WINDOW, "       has references to it.");
	vratputs(win, 6, 3, ERROR_WINDOW, "       ARE YOU SURE ? (Y/N)");

	vbeep();
	ch = Smt_KbdGet();
	ch = toupper(ch);
	vdelete(win, NONE);

	if (ch == 'Y')
		return(TRUE);
	else
		return(FALSE);
}

/* 	Function:	show_dberr()
	Rescription:	pops up a window displaying an error message
	Inputs:		status - the error message to display
	Outputs:	None
	Return:		status
*/

PUBLIC int show_dberr(int status)
{
	HWND	win;

	if (status == OK)
		return(status);

	win = vcreat(1, -1, ERROR_WINDOW, YES);
	vwind(win, 1, -1, 0, 0);
	vlocate(win, 24, 0);
	visible(win, YES, YES);

	switch (status)
	{	case (BOI):
			vratputs(win, 0, 1, ERROR_WINDOW, "<Beginning of file>");
		break;
		case (EOI):
			vratputs(win, 0, 1, ERROR_WINDOW, "<End of file>");
		break;
		case (FOUND):
			vratputs(win, 0, 1, ERROR_WINDOW, "<Record Found>");
		break;
		case (NOTFOUND):
			vratputs(win, 0, 1, ERROR_WINDOW, "<Record Not Found>");
		break;
		case (DUPLICATE):
			vratputs(win, 0, 1, ERROR_WINDOW, "<Duplicate Record>");
		break;
		case (NO_ACTIVE_ASSIGNMENTS):
			vratputs(win, 0, 1, ERROR_WINDOW, "<No Active Assignments>");
		break;
		case (NO_TERMINALS):
			vratputs(win, 0, 1, ERROR_WINDOW, "<No Command Terminals for this Controller Group>");
		break;
		case (ONE_TERMINAL):
			vratputs(win, 0, 1, ERROR_WINDOW, "<This Controller Group needs no secondary assignments>");
		break;
		case (NO_NEXT_PAGE):
		case (NO_PREV_PAGE):
			vratputs(win, 0, 1, ERROR_WINDOW, "<Valid pages are in the range ( 1 to 5 )>");
		break;
		case (NOT_WRITTEN):
			vratputs(win, 0, 1, ERROR_WINDOW, "<Can't modify a record that doesnt exist!>");
		break;
		case (NO_RECORDS):
			vratputs(win, 0, 1, ERROR_WINDOW, "<NO records have been defined yet>");
		break;
		case (RECORD_EXISTS):
			vratputs(win, 0, 1, ERROR_WINDOW, "<Record already exists. Use Modify>");
		break;
 		case (FORM_INCOMPLETE):
			vratputs(win, 0, 1, ERROR_WINDOW, "<Incomplete form - record not written>");
		break;
 		case (NO_PHONE_RECORDS):
			vratputs(win, 0, 1, ERROR_WINDOW, "<NO Extension Numbers have been defined>");
		break;
 		case (NO_GROUP_RECORDS):
			vratputs(win, 0, 1, ERROR_WINDOW, "<NO Controller Groups have been defined>");
		break;
 		case (NO_GROUP_PAGES):
			vratputs(win, 0, 1, ERROR_WINDOW, "<NO Group Pages have been defined>");
		break;
 		case (RANGE_ERR_1):
			vratputs(win, 0, 1, ERROR_WINDOW, "<Label Position must be in the range 1 - 7>");
		break;
 		case (RANGE_ERR_2):
			vratputs(win, 0, 1, ERROR_WINDOW, "<Default Controller is invalid>");
		break;
 		case (NO_PHONE_SELECTED):
			vratputs(win, 0, 1, ERROR_WINDOW, "<NO Extension Numbers have been selected>");
		break;
 		case (ONE_REC_LIMIT):
			vratputs(win, 0, 1, ERROR_WINDOW, "<A record of this type already exists>");
		break;
 		case (MAX_ASSIGNS_EXCEEDED):
			vratputs(win, 0, 1, ERROR_WINDOW, "<Maximum number of assignments (250) exceeded>");
		break;
 		case (PAGE_EXISTS):
			vratputs(win, 0, 1, ERROR_WINDOW, "<Pages can be added to blank pages only>");
		break;
 		case (PARTY_REC_LIMIT):
			vratputs(win, 0, 1, ERROR_WINDOW, "<Limit of 16 assignments per single party line exceeded>");
		break;
 		case (MONITOR_DEFINED):
			vratputs(win, 0, 1, ERROR_WINDOW, "<Base ACBU Extensions must be 8 digits apart>");
		break;
 		case (WRONG_OMNI_DIG):
			vratputs(win, 0, 1, ERROR_WINDOW, "<Code can only be 3 or 6 digits for omnibus type>");
		break;
		case (INTEGRITY_CHK_FAILED):
			vratputs(win, 0, 1, ERROR_WINDOW, "<Integrity check FAILED>");
		break;
		case (PIN_CHK_FAILED):
			vratputs(win, 0, 1, ERROR_WINDOW, "<P.I.N. check FAILED. More P.I.N. 's required (see list above)>");
		break;
		case (ASSIGN_CHK_FAILED):
			vratputs(win, 0, 1, ERROR_WINDOW, "<Assignment check FAILED. See list above for un-assigned extensions>");
		break;
		case (LABELS_CHK_FAILED):
			vratputs(win, 0, 1, ERROR_WINDOW, "<Controller Labels check FAILED. See list above>");
		break;
		case (EMERGENCY_RANGE):
			vratputs(win, 0, 1, ERROR_WINDOW, "<Max. Queue extension must be greater than Min. Queue extension>");
		break;
		case (NUMBER_RANGE):
			vratputs(win, 0, 1, ERROR_WINDOW, "<Automatic Answer Delay time must be a number from 1 to 9>");
		break;
		case (CTS_DEFINED):
			vratputs(win, 0, 1, ERROR_WINDOW, "<All the terminals defined have been assigned to a Controller Group>");
		break;
		case (MIN_IN_TELEPHONY):
			vratputs(win, 0, 1, ERROR_WINDOW, "<The mininum of 2 controllers in telephony been reached>");
		break;
		case (MAX_IN_TELEPHONY):
			vratputs(win, 0, 1, ERROR_WINDOW, "<The maximum number of controllers in telephony has been reached>");
		break;
		case (DOWNLOAD_ABORTED):
			vratputs(win, 0, 1, ERROR_WINDOW, "<Download Configuration ABORTED by user request>");
		break;
		case (NO_LABEL_DEFINED):
			vratputs(win, 0, 1, ERROR_WINDOW, "<No Controller Label defined for this configuration>");
		break;
		case (MDU_CHAN_ERR):
			vratputs(win, 0, 1, ERROR_WINDOW, "<MDU Channel must be unique and in the range 2 thru 31>");
		break;
		case (NUMBERS_NOT_UNIQUE):
			vratputs(win, 0, 1, ERROR_WINDOW, "<Call numbers must be unique>");
		break;
		case (NO_CTS_DEFINED):
			vratputs(win, 0, 1, ERROR_WINDOW, "<No terminals have been assigned to this group>");
		break;
		case (PASSWORD_IN_GROUP):
			vratputs(win, 0, 1, ERROR_WINDOW, "<This P.I.N. already appears in Controller Passwords>");
		break;
		case (NON_DELETABLE_REC):
			vratputs(win, 0, 1, ERROR_WINDOW, "<This record is used critically elsewhere. Deletion aborted>");
		break;
		case (NO_CT_RECS):
			vratputs(win, 0, 1, ERROR_WINDOW, "<No Command Terminals have been defined>");
		break;
		case (PASSWORD_IN_SERVICE_GROUP):
			vratputs(win, 0, 1, ERROR_WINDOW, "<This P.I.N. already appears in Service Personnel Passwords>");
		break;
		case (DUPLICATE_PHONES):
			vratputs(win, 0, 1, ERROR_WINDOW, "<Duplicate telephone numbers not allowed>");
		break;
		case (DUPLICATE_CTS):
			vratputs(win, 0, 1, ERROR_WINDOW, "<Duplicate Command Terminals not allowed>");
		break;
		case ( GROUP_PAGE_LIMIT ):
			vratputs(win, 0, 1, ERROR_WINDOW, "<Limit of 50 Group Page Labels exceeded>");
		break;
		case ( CONTROLLER_LABEL_LIMIT ):
			vratputs(win, 0, 1, ERROR_WINDOW, "<Limit of 30 Controller Group Labels exceeded>");
		break;
		case ( 94 ):
			vratputs(win, 0, 1, ERROR_WINDOW, "<P.I.N must contain 3 digits>");
		break;
		default:
			vratputs(win, 0, 1, ERROR_WINDOW, "<UNKNOWN DATABASE ERROR>");
		break;
	}		

	/* sound bell */
	vbeep();

	Smt_KbdGet();
	vdelete(win, NONE);

	return(status);
}

