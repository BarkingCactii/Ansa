#ifndef _AMBA_PNL_H_
#define _AMBA_PNL_H_

EXPORT BUTTON_DESC  amba_panel_value;

EXPORT int  GetAmbaLablesForPin ( char *pin, int *flag );
EXPORT int  AmbaGroupMemberTotal ( int group );
EXPORT int  GetNextAmbaMember ( int *index, char *numb, char *label, char *type );

EXPORT int  DisplayAmbaPanel ( void );
EXPORT void RemoveAmbaPanel ( void );

EXPORT void DefineAmbaPanel ( void );

#endif

