
#ifndef _TELE_PNL_H_
#define _TELE_PNL_H_

EXPORT int          Tel_redraw_hidden_flag;
EXPORT ob_desc far  telephony_panel[NUM_TELEPHONY_BUTTONS];
EXPORT BUTTON_DESC  telephony_panel_value;

EXPORT int  IsButtonHidden ( int idx );
EXPORT int  IsConfButtonHidden ( int idx );
EXPORT void SetHiddenButtons ( int new_ones[] );
EXPORT void ResetHiddenButtons ( int old_ones[] );
EXPORT void _pascal RedrawHiddenButton ( void );

EXPORT void SetAutoAnsaButton ( int flag );
EXPORT void SetLoudspkButtonLabel ( int flag );
EXPORT void SetOutLinePanelButton ( int idx, char *ptr, int attr, int flag );

EXPORT void UpdateTelephonyPanel ( int have_amba, int have_dir );
EXPORT void DisplayTelephonyPanel ( int have_amba, int have_dir );
EXPORT void RemoveTelephonyPanel ( void );

EXPORT int  DefineTelephonyPanel ( void );

#endif

