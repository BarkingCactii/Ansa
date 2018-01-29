
#ifndef _OMNI_PNL_H
#define _OMNI_PNL_H

EXPORT BUTTON_DESC  omnibus_value;

EXPORT int  GetOmnibusFile ( char call_type, int ref_numb );

EXPORT void ShowSelectedOmniParty ( int entry );
EXPORT int  GetSelectedOmniParty ( char **code );
EXPORT int  GotoOmniPartyExtreme ( int direction );
EXPORT int  GotoNextOmniPartyPage ( int direction );

EXPORT int  DisplayOmnibusPanel ( int index, int call_type, int ref_numb  );
EXPORT void RemoveOmnibusPanel ( void );

EXPORT int DefineOmnibusPanel ( void );

#endif

