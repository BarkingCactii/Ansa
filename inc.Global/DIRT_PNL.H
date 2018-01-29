#ifndef _DIRT_PNL_H_
#define _DIRT_PNL_H_

EXPORT  BUTTON_DESC  dir_panel_value;

EXPORT  int  InitPopUpDirectory ( char *pin );

EXPORT void ShowSelectedDirectory ( int entry );
EXPORT void GetSelectedDirectory ( char *a_party, char **label, char *type );
EXPORT int  GotoDirectoryExtreme ( int direction );
EXPORT int  GotoNextDirectoryPage ( int direction );

EXPORT int  DefineDirectoryPanel ( void );
EXPORT int  DisplayDirectoryPanel ( void );
EXPORT void RemoveDirectoryPanel ( void );

#endif

