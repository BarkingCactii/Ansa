
#ifndef _PROG_PNL_H_
#define _PROG_PNL_H_

EXPORT BUTTON_DESC  prog_panel_value;
EXPORT BUTTON_DESC  config_panel_value;

EXPORT void ToggleCallAlertLabel ( int on_or_off );
EXPORT void ToggleTimeDisplayLabel ( int on_or_off );
EXPORT void HiliteCallFreqButton ( int button );

EXPORT void ShadeProgExitButton ( void );
EXPORT void ActivateProgExitButton ( int divert_flag );
EXPORT void ActivateProgChgeButton ( void );

EXPORT void DisplayConfigPanel ( int time, int alert, int calib, int freq );
EXPORT void RemoveConfigPanel ( void );
EXPORT void DisplayProgPanel ( int alert );
EXPORT void RemoveProgPanel ( void );

EXPORT void DefineConfigPanel ( void );
EXPORT void DefineProgPanel ( void );

#endif

