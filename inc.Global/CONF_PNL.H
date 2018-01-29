
#ifndef _CONF_PNL_H_
#define _CONF_PNL_H_

EXPORT BUTTON_DESC  extd_conf_panel_value;

EXPORT void SetExtdConfPanelButton ( int idx, char *ptr, int attr, int flag );

EXPORT void DisplayExtdConfPanel ( void );
EXPORT void RemoveExtdConfPanel ( void );

EXPORT void DefineExtdConfPanel ( void );

#endif

