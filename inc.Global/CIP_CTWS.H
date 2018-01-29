
#ifndef _CIP_CTWS_H_
#define _CIP_CTWS_H_

typedef struct
{
    BITE        mesg_id;
    int         code;
    int         seq_number;
    int         dest_ctws_idx;
    int         c_party;
    char        call_type;
    WORD        call_mask;
    ulong       a_party;
    ulong       b_party;
} FROM_CIP_MESG;

#define NO_CIP_MESSAGE              0x00

#define CIP_VERIFY_CTS_IN_CALLS     0x01
#define CIP_AUTO_PICK_UP_IN_CALL    0x02

EXPORT void Cip_AssignTelecomCall ( int c );
EXPORT void Cip_AssignIncomingCall ( ulong a, ulong b, int c );
EXPORT void Cip_QueueGroupClears ( int c_party );
EXPORT void Cip_CheckIfMyPickUp ( ulong a_party, int c_party );

EXPORT void Cip_ShowBusyInCall ( int from, int on, ulong who, int c_party );
EXPORT void Cip_ClearBusyInCall ( int from, int on );
EXPORT void Cip_ClearAllBusyInCalls ( int from );

EXPORT void Cip_VerifyReplyIsBack ( int from, WORD ct_mask );

EXPORT void Cip_ShowBusyOutCall ( int from, int on, ulong who );
EXPORT void Cip_ClearBusyOutCall ( int from, int on );
EXPORT void Cip_ClearAllBusyOutCalls ( int from );

EXPORT void _pascal Cip_PollQueue ( void );

EXPORT int  Cip_InitInCallQueues ( int our_ctws, int all_the_ctws );

#endif

