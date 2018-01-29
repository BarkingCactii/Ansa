
#ifndef _WIN_UTIL_H_
#define _WIN_UTIL_H_

EXPORT int  Smt_OpenWaitWindow ( char *str );
EXPORT void Smt_CloseWaitWindow ( void );
EXPORT int  Smt_OpenBorderBars ( char *top_title, char *bot_title );
EXPORT void Smt_CloseBorderBars ( void );
EXPORT void Smt_DisplayErrorMesg ( char *error_message );
EXPORT int  Smt_SelectFile ( char *fspec, char *selected_fname );

#endif

