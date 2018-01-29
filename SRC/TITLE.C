#pragma pack (1)
/*
	title.c

    System  :  Train Control Command Communication System
    Author  :  Ansa Pty. Ltd. (c) 1991
    Revision:  1.0
*/

#include <stdlib.h>
#include <string.h>
#include <ibmkeys.h>

#include "sysdef.h"
#include "filename.h"
#include "win_hues.h"

#include "title.h"

PRIVATE HWND	twn;

/* 	Function:	display_title()
	Rescription:	creates and displays a title on the top
			row of the screen
	Inputs:		option - the file we are editing
	Outputs:	None
	Return:		None
*/

PUBLIC void display_title ()
{
    int     i, len;
    char   *s_ptr = "System Management Terminal";

    twn = vcreat ( 1, -1, TITLE_WINDOW, YES );
    vwind ( twn, 1, -1, 0, 0 );

    len = ( 80 - strlen ( s_ptr ) ) / 2;

    for ( i = 0; i < len; i++ )
        vputc ( twn, ' ' );

    vputf ( twn, "%s", s_ptr );
    visible ( twn, YES, YES);
}

/* 	Function:	title_visible()
	Rescription:	makes the title window created by display_title()
			visible on screen
	Inputs:		None
	Outputs:	None
	Return:		None
*/

PUBLIC void drop_title ( )
{
    vdelete ( twn, NONE );
}

/* 	Function:	display_group()
	Rescription:	displays the current group page in the title window
	Inputs:		group - the group page label
	Outputs:	None
	Return:		None
*/
	
PUBLIC void display_group ( char *group )
{	
    vratputf ( twn, 0, 0, TITLE_WINDOW, " Passive Line Assignment   -   Group Page: %s", group );
}

/* 	Function:	display_controller_group()
	Rescription:	displays the current controller group in the 
			title window
	Inputs:		group - the controller group label
	Outputs:	None
	Return:		None
*/
	
PUBLIC void display_controller_group ( char *group )
{	
    vratputf ( twn, 0, 0, TITLE_WINDOW, "   Secondary Active Line Assignment  -  Controller Group: %s", group );
}

/* 	Function:	display_page()
	Rescription:	displays the current page number for passive
			line assignments
	Inputs:		page - the page number to be displayed
	Outputs:	None
	Return:		None
*/
	
PUBLIC void display_page ( int page )
{	
    vratputf ( twn, 0, 72, TITLE_WINDOW, "Page: %d", page );
}
