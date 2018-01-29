#pragma pack (1)
/*
	menu.c

    System  :  Train Control Command Communication System
    Author  :  Ansa Pty. Ltd. (c) 1991
    Revision:  1.0
*/

#include "sysdef.h"
#include "filename.h"
#include "title.h"

#include "win_hues.h"
#include "win_util.h"
#include "scroller.h"

#include "pop_menu.h"
#include "smt_help.h"

#define MAX_MENU_ITEMS  6
#define MAX_MENUS       8


PRIVATE char	*db_main[] = 
{
	" Elementary Data                ",
	" Controller Group Configuration ",
	" Line Assignment                ",
	" System Configuration           ",
	" Download Configuration         ",
	" Regress to Passive Data        ",
    NULL
};

PRIVATE char	*db_line_1[] = 
{
	" Telephone Numbers              ",
	" Group Page Labels              ",
	" Controller Group Labels        ",
	" ECBU Magazine Labels           ",
    NULL
};

PRIVATE char	*db_line_2[] = 
{
	" Called Party (Controller)      ",
	" Command Terminal Configuration ",
	" Command Terminal Grouping      ",
	" Controller Passwords           ",
	" AMBA (Batch Conferences)       ",
    NULL
};

PRIVATE char	*db_line_3[] = 
{
	" Passive Assignments            ",
	" Active Assignments             ",
    NULL
};

PRIVATE char	*db_line_3_1[] = 
{
	" Passive Line Assignments       ",
	" Outgoing Party Line Parties    ",
    NULL
};
PRIVATE char	*db_line_3_2[] = 
{
	" Active Assignment Labels       ",
	" Primary Active Assignments     ",
	" Secondary Active Assignments   ",
	" Party Line Active Assignments  ",
/*	" Selcall Line Active Assignments", */
    NULL
};

PRIVATE char	*db_line_4[] = 
{
	" PABX Configuration             ",
	" Service Personnel Passwords     ",
	" Report Configuration           ",
    NULL
};

typedef struct
{
	char	**menu_to_display;
    int     choice;
} MENU_ITEM_INFO;

typedef struct
{
    char          **current_menu;
    char          **previous_menu;
    char           *title;
    int             last_item_selected;
	MENU_ITEM_INFO	menu_item[MAX_MENU_ITEMS];
} MENU_INFO;

PRIVATE MENU_INFO   smt_menu[MAX_MENUS] =
{
	/* top database menu */

    {
        db_main,    NULL,    " Database Management Menu",    0,
        {
            { db_line_1,  0 },
            { db_line_2,  0 },
            { db_line_3,  0 },
            { db_line_4,  0 },
            { 0, ASCII_FILES },
            { 0, REGRESS_DATA }
        }
    },

    {
        db_line_1,  db_main, " Elementary Data",     0,
        {
            { 0, NUMBER_FILE },
            { 0, GROUP_FILE },
            { 0, OPERATOR_GROUP_FILE },
            { 0, ECBU_MAGAZINE_FILE },
            { 0, 0 },
            { 0, 0 }
        }
    },

    {
        db_line_2,  db_main, " Controller Group Configuration",  0,
        {
            { 0, B_PARTY_FILE },
            { 0, CT_COMMS_FILE },
            { 0, CT_CONFIG_FILE },
            { 0, PASSWORD_FILE },
            { 0, GROUP_CONF_FILE },
            { 0, 0 }
        }
    },

    {
        db_line_3,  db_main, " Line Assignments",    0,
        {
            { db_line_3_1, 0 },
            { db_line_3_2, 0 },
            { 0, 0 },
            { 0, 0 },
            { 0, 0 },
            { 0, 0 }
        }
    },

    {
        db_line_3_1,    db_line_3, " Passive Assignments",   0,
        {
            { 0, LINEASS_FILE },
            { 0, OUT_PARTY_LINE_FILE },
            { 0, 0 },
            { 0, 0 },
            { 0, 0 },
            { 0, 0 }
        }
    },

    {
        db_line_3_2,    db_line_3, " Active Assignments",    0,
        {
            { 0, OPERATOR_ASSIGN_FILE },
            { 0, ACTIVE_LINE_FILE },
            { 0, PRIMARY_LINE_FILE },
            { 0, PARTY_LINE_FILE },
/*            { 0, D_PARTY_FILE }, */
            { 0, 0 },
            { 0, 0 }
        }
    },

    {
        db_line_4,  db_main, " System Configuration",    0,
        {
            { 0, GEN_COMMS_FILE },
            { 0, SERVICE_PASSWORD_FILE },
            { 0, REPORT_CONFIG_FILE },
            { 0, 0 },
            { 0, 0 },
            { 0, 0 }
        }
    }
};

PRIVATE char    **the_menu = db_main;

/*
 *  Function:       popup_menu()
    Description:    handles the menu system.
    Return:         the corresponding value for choice for
                    the selected menu item
*/

PUBLIC int popup_menu ( void )
{
	int		i;
	int		choice;
    int     status;
	int		forever = TRUE;

    Smt_PushHelp ( DATABASE_HLP );

	while (forever)
	{
        display_title ();

		/* work out which menu we are in */

        if (the_menu == db_main)
            Smt_PushHelp ( DB_MGMNT_MENU );
        else
            Smt_PushHelp ( DEPENDENCY_HLP );
            
		for (i = 0; i < MAX_MENUS; i++)
			if (smt_menu[i].current_menu == the_menu)
				break;

        choice = Smt_Scroller ( smt_menu[i].title, 7, 26, AUTO, AUTO,       \
                                POPUP_NORMAL, POPUP_HILITE, the_menu, 1,    \
                                            smt_menu[i].last_item_selected);
        drop_title ( );

		/* go back 1 menu */

		if (choice == ERROR)
		{
            if (smt_menu[i].previous_menu)
				the_menu = smt_menu[i].previous_menu;
            else                                        /* exit program */
            {
                status = EXIT;
                forever = FALSE;
            }
		}
        else if ( choice == DANGER )
        {
                /* just ignore it - error !!! */
        }
		else
		{
			smt_menu[i].last_item_selected = choice;

			if (smt_menu[i].menu_item[choice].menu_to_display)
				the_menu = smt_menu[i].menu_item[choice].menu_to_display;
			else
            {
                status = smt_menu[i].menu_item[choice].choice;
                forever = FALSE;
            }
		}

        Smt_PopHelp ( );

	}

    Smt_PopHelp ( );
    return ( status );
}

                         
