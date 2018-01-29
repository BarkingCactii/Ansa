
#ifndef _STAT_PNL_H_
#define _STAT_PNL_H_

EXPORT void ShowHeadsetIcon ( void );
EXPORT void FlashDivertIcon ( void );
EXPORT void ResetDivertIcon ( void );
EXPORT void ShowNoSpeakerIcon ( void );
EXPORT void FlashAssignmentIcon ( void );
EXPORT void ResetAssignmentIcon ( void );
EXPORT void ShowLoudSpeakerIcon ( void );

EXPORT void ShowClock ( int flag );
EXPORT void ShowPageLabel ( char *label, int number );
EXPORT void ResetPageLabel ( void );
EXPORT void ShowDesignation ( void );
EXPORT void ShowCallTotals ( int mine, int total );

EXPORT int  InitStatusLines ( int for_group, int for_ctws );
EXPORT void CloseStatusLines ( void );

#endif

