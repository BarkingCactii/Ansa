
#ifndef _ACTIVELY_H_
#define _ACTIVELY_H_

#define MY_QUEUE        1
#define GROUP_QUEUE     2
#define OTHER_QUEUE     3
#define DIRECT_QUEUE    4
#define INTRUDE_QUEUE   5

typedef struct
{
    int     attr;                           /* set by active assignment */
    int     found;
    int     ctws_id;                        /* set by active assignment */
    int     which_q;                        /* set by active assignment */
    int     search_flag;
    ulong   a_party;
    ulong   b_party;
    char    call_type;                      /* set by active assignment */
    char    label[KEY_LABEL_LEN+1];         /* set by active assignment */
} IN_CALL_REC;

#define IN_CALL_REC_SIZE     sizeof ( IN_CALL_REC )

EXPORT  BUTTON_DESC     assign_panel_value;
EXPORT  BUTTON_DESC     control_panel_value;

EXPORT int  IsPreferredCaller ( void );
EXPORT int  IsControlGroupCall ( ulong a_party, ulong b_party, IN_CALL_REC *in_stuff );
EXPORT int  IsControlDirectCall ( ulong b_party, IN_CALL_REC *in_stuff );
EXPORT int  IsPartyLineCall ( ulong a_party, ulong b_party, IN_CALL_REC *in_stuff );

EXPORT void GetThisCtsDirectNumber ( char *our_direct );
EXPORT void GetThisCtsIntrudeNumber ( char *our_intrude );

EXPORT void ActivateNewAssignment ( void );
EXPORT void SetAssignmentForOne ( void );

EXPORT int  DisplayEntryAssPanel ( int numb_cols );
EXPORT void RemoveEntryAssPanel ( void );
EXPORT void DefineAssignCntlPanel ( void );

EXPORT int  InitPrimaryAssignment ( int our_group, int our_ctws );

#endif

