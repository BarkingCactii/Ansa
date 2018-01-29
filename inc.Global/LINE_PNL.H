
#ifndef _LINE_PNL_H_
#define _LINE_PNL_H_

EXPORT int  Tel_draw_lines_flag;

EXPORT void SetInLinePanelButton ( int idx, char *ptr, int attr, int flag );
EXPORT void SetQueuePanelButton ( int idx, char *ptr, int attr, int flag );
EXPORT void SetInCallButtonList ( ulong number, int attr, char *real_label );
EXPORT void ClearLineTargetList ( ulong number_id );
EXPORT void ResetInCallTargetList ( int call_index );
EXPORT void SetAnsaInCallTargetList ( ulong number_id, int call_index );
EXPORT void SetBusyLineTargetList ( ulong number_id );
EXPORT void CheckRingLineTargetList ( ulong number_id, int new_attr );
EXPORT void CheckBusyLineTargetList ( ulong number_id );

EXPORT int  MoveToLinePage ( int direction );
EXPORT int  ResetToDefaultLinePage ( void );
EXPORT int  ShowNewGroupPages ( int page_no );
EXPORT int  LoadDefaultGroupPages ( void );

EXPORT void GetSelectedLineKey ( int butt, ulong *a_party );
EXPORT void GetLineKeyDetails ( int butt, char *numb, char *label, char *type );

EXPORT void _pascal DrawLineKeyButton ( void );

#endif

