
#ifndef _MAST_PNL_H_
#define _MAST_PNL_H_

EXPORT  BUTTON_DESC  master_panel_value;

EXPORT int  InitCallPickup ( int c_party );
EXPORT int  CheckPickedUpInLine ( int index, ulong *a, int *c );
EXPORT void ResetReservedInLine ( int index );
EXPORT void UpdatePickedUpInLine ( int index );
EXPORT int  SetHavePickedUpInLine ( int index, char *type, ulong *a, int *c );
EXPORT int  SetPickUpInLineFailed ( int index, ulong *a, int *c );
EXPORT int  CheckIfMyPickUp ( int c_party, int *index );
EXPORT int  GetNextClearingInLine ( int at, int *our_ref );
EXPORT int  SetClearedInLine ( int index, char *type, ulong *a );
EXPORT void SetInLineNowCleared ( int index, char *type, ulong *a );
EXPORT void SetAllInLinesCleared ( void );
EXPORT void SelectWholeInPanel ( void );
EXPORT void DeSelectWholeInPanel ( void );
EXPORT int  DoWeHaveAnInCall ( void );
EXPORT int  IsInCallSelected ( void );
EXPORT int  AreAllInCallsSelected ( void );
EXPORT int  GetSelectedHoldInLine ( int at, int *our_ref );
EXPORT int  GetSelectedListenInLine ( int at, int *our_ref );
EXPORT void SetInLineNowHeld ( int index );
EXPORT void SetAllInLinesHeld ( void );
EXPORT void SetInLineNowListening ( int index );
EXPORT void SetAllInLinesListened ( void );
EXPORT void ResetFailedInLineOp ( int index, char *mesg );
EXPORT void ResetAllInLinesFailedOp ( char *mesg );
EXPORT void ResetAllInCalls ( void );

EXPORT int  HaveInCallsQueued ( void );
EXPORT void SetCallAlertTones ( void );
EXPORT void DemoCallAlertTone ( int which );

EXPORT void SetAutoAnsaMode ( void );
EXPORT void ToggleAutoAnsaMode ( void );

EXPORT void GetSelectedCallQ ( int idx, int *c_party, char *type, int *code );
EXPORT int  GetRingQueueEntry ( ulong a_party, int *c_party, char *type, int *code );

EXPORT int  GetSelectedMasterQ ( int *c_party, char *type, int *code );
EXPORT void ShowSelectedMasterQ ( int entry );
EXPORT int  GotoMasterQExtreme ( int direction );
EXPORT int  GotoNextMasterQPage ( int direction );

EXPORT int  DisplayMasterQPanel ( void );
EXPORT void RemoveMasterQPanel ( void );

EXPORT int  DefineMasterQPanel ( void );

#endif

