
#ifndef _OUT_CALL_H_
#define _OUT_CALL_H_

EXPORT int  IsExistingOutCall ( ulong a_party );
EXPORT int  IsExistingExtdCall ( ulong a_party );
EXPORT int  InitOutgoingCall ( ulong a, int ref, char type, char *label );
EXPORT int  InitExtendedCall ( ulong a, int ref, char type, char *label );
EXPORT int  CheckDialedOutLine ( int index, char *type, ulong *a );
EXPORT void UpdateDialedOutLine ( int index, int tlu_numb );
EXPORT void ResetReservedOutLine ( int index );
EXPORT int  GetAssociatedOutLine ( int tlu_ref, int *index );
EXPORT int  GetAssociatedExtLine ( int tlu_ref, int *index );
EXPORT void SetAnsweredOutLine ( int index );
EXPORT void SetAnsweredExtLine ( int index );
EXPORT int  GetNextClearingOutLine ( int at, int *our_ref, int *ecb_ref );
EXPORT int  GetNextClearingExtLine ( int at, int *our_ref, int *ecb_ref );
EXPORT void SetClearedOutLine ( int index, char *type, ulong *a );
EXPORT void SetClearedExtLine ( int index, char *type, ulong *a );
EXPORT void SetAllOutLinesCleared ( void );
EXPORT void SetAllExtLinesCleared ( void );
EXPORT void SetOutLineNowCleared ( int index, char *type, ulong *a );
EXPORT int  ValidateOutLinePosted ( void );
EXPORT void CheckOutLineSelect ( int index );
EXPORT void SelectWholeOutPanel ( void );
EXPORT void SelectWholeExtPanel ( void );
EXPORT void DeSelectWholeOutPanel ( void );
EXPORT void DeSelectWholeExtPanel ( void );
EXPORT int  FreeOutCallTotal ( void );
EXPORT int  DoWeHaveAnOutCall ( void );
EXPORT int  IsExtCallPanelEmpty ( void );
EXPORT int  IsOutCallSelected ( void );
EXPORT int  AreAllOutCallsSelected ( void );
EXPORT int  GetNextSelectedOutLine ( int at, int *our_ref, int *ecb_ref, int from_attr, int to_attr );
EXPORT int  GetNextSelectedExtLine ( int at, int *our_ref, int *ecb_ref, int from_attr, int to_attr );
EXPORT void SetOutLineNowHeld ( int index );
EXPORT void SetAllOutLinesHeld ( void );
EXPORT void SetAllExtLinesHeld ( void );
EXPORT void SetOutLineNowListening ( int index );
EXPORT void SetAllOutLinesListened ( void );
EXPORT void SetAllExtLinesListened ( void );
EXPORT void ResetFailedOutLineOp ( int index, char *mesg );
EXPORT void ResetAllOutLinesFailedOp ( char *mesg );
EXPORT void ResetAllExtLinesFailedOp ( char *mesg );

EXPORT void ResetAllOutCalls ( void );
EXPORT void ResetAllExtCalls ( void );

EXPORT void InitOutCallModule ( void );

#endif

