
#ifndef _EDIT_PNL_H_
#define _EDIT_PNL_H_

EXPORT BUTTON_DESC  editor_panel_value;

EXPORT int  ToggleEditorHelp ( void );
EXPORT int  GotoTopLineEditor ( void );
EXPORT int  GotoTopLastEditor ( void );
EXPORT void MoveCursorEditorRowCol ( int x, int y );
EXPORT int  EditorKeyBrdInput ( int key );

EXPORT int  DisplayEditorPanel ( char *pin );
EXPORT int  RemoveEditorPanel ( int save_flag );

EXPORT int DefineEditorPanel ( void );

#endif

