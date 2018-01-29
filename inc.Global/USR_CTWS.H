
#ifndef _USR_CTWS_H_
#define _USR_CTWS_H_

typedef struct
{
    BITE        mesg_id;
    BITE        ctws_id;
    BITE        mode;
    int         index;
    int         button;
    int         pickup;
    char        call_type;
    char       *string;
    ulong       called;
} FROM_USR_MESG;

#define NO_USR_MESSAGE              0x00

#define USR_NORMAL_TERMINATION      0x01
#define USR_CHECK_LOG_ON_PIN        0x02
#define USR_START_TELE_ENTRY        0x03
#define USR_REVERT_TO_LOG_OFF       0x04
#define USR_ABORT_TELE_ENTRY        0x05
#define USR_LEAD_ENTRY_CHOICE       0x06
#define USR_TELE_CHANGE_CHOICE      0x07
#define USR_TELE_CHANGE_ABORT       0x08
#define USR_ACCEPTS_TELE_ENTRY      0x09
#define USR_MAKE_OUTGOING_CALL      0x0A
#define USR_TURN_ON_SPEECH_MODE     0x0B
#define USR_ASK_TO_BATCH_CALLS      0x0C
#define USR_CHECK_LOG_OVER_PIN      0x0D
#define USR_START_TELE_DIVERT       0x0E
#define USR_ABORT_TELE_DIVERT       0x0F
#define USR_LEAD_DIVERT_TO_CTWS     0x10
#define USR_LEAD_DIVERTS_A_LINE     0x11
#define USR_LEAD_DIVERT_ENDED       0x12
#define USR_DEST_DIVERT_ACCEPT      0x13
#define USR_DEST_DIVERT_CHANGED     0x14
#define USR_START_TELE_EXIT         0x15
#define USR_ABORT_TELE_EXIT         0x16
#define USR_EXIT_ADVISE_OTHERS      0x17
#define USR_EXIT_CHANGE_ASSIGN      0x18
#define USR_TELE_CHOICE_ABORT       0x19
#define USR_EXIT_ASSIGN_ACCEPTED    0x1A
#define USR_IN_POST_DIAL_A_DIGIT    0x1B
#define USR_OUT_POST_DIAL_DIGIT     0x1C
#define USR_PICK_UP_AN_IN_CALL      0x1D
#define USR_MUTE_ALL_OUT_LINES      0x1E
#define USR_MUTE_AN_OUT_LINE        0x1F
#define USR_MUTE_ALL_IN_LINES       0x20
#define USR_MUTE_AN_IN_LINE         0x21
#define USR_LISTEN_ALL_OUT_LINES    0x22
#define USR_LISTEN_TO_AN_OUT_LINE   0x23
#define USR_LISTEN_ALL_IN_LINES     0x24
#define USR_LISTEN_TO_AN_IN_LINE    0x25
#define USR_HANGUP_AN_OUT_CALL      0x26
#define USR_OUT_CALL_CONNECTED      0x27
#define USR_SEND_USER_PTT_ON        0x28
#define USR_SEND_USER_PTT_OFF       0x29
#define USR_TELE_CHANGED_OK         0x2A
#define USR_OUT_CALL_HUNG_UP        0x2B
#define USR_HANGUP_ALL_OUT_CALLS    0x2C
#define USR_ALL_OUT_CALLS_HUNGUP    0x2D
#define USR_IN_CALL_CONNECTED       0x2E
#define USR_IN_CALL_HUNG_UP         0x2F
#define USR_HANGUP_ALL_IN_CALLS     0x30
#define USR_ALL_IN_CALLS_HUNGUP     0x31
#define USR_HANGUP_AN_IN_CALL       0x32

EXPORT void Icn_ShowIconStatus ( int number, int status );

EXPORT void Lds_ActOnJackIn ( BITE state );
EXPORT void Lds_ActOnJackOut ( BITE state );
EXPORT void Lds_HandSetInOrOut ( int flag, WORD state );
EXPORT void Lds_CheckSpeechSetOk ( int flag );

EXPORT void Usr_ActOnPTT_On ( BITE state );
EXPORT void Usr_ActOnPTT_Off ( BITE state );
EXPORT void Usr_CheckPTTedOk ( int status_ok );

EXPORT void Usr_UpdateEveryMinute ( void );
EXPORT void Usr_LoadLogOnStuff ( int flag );
EXPORT void Usr_LoadLogOverStuff ( int flag );
EXPORT void Usr_BackToLoggedOff ( void );

EXPORT void Usr_ContactServerBusy ( int whos_in_tele );
EXPORT void Usr_ContactServerFailed ( int whos_in_tele );
EXPORT void Usr_ResetAfterAbortion ( int whos_in_tele );
EXPORT void Usr_SetOperatingModeBusy ( void );
EXPORT void Usr_SetOperatingModeFree ( int whos_in_tele );
EXPORT void Usr_SetOperatingModePanic ( void );

EXPORT void Usr_ActOnOutCallAnswered ( int ecb_ref );
EXPORT void Usr_ActOnOutCallCleared ( int ecb_ref );

EXPORT void Usr_UpdateInitiatedCall ( int our_ref, int ecb_ref, int status );
EXPORT void Usr_UpdateHeldOutCall ( int our_ref, int status_ok );
EXPORT void Usr_UpdateAllHeldOutCalls ( int status_ok );
EXPORT void Usr_UpdateListenOutCall ( int our_ref, int status_ok );
EXPORT void Usr_UpdateAllListenOutCalls ( int status_ok );
EXPORT void Usr_UpdateClearedOutCall ( int our_ref, int status_ok );
EXPORT void Usr_UpdateClearAllOutCalls ( int status_ok );
EXPORT void Usr_ResetAllOutCalls ( void );
EXPORT void Usr_ShowPostDialFailed ( void );
EXPORT void Usr_StartAmbaCalls ( void );
EXPORT void Usr_AbortAmbaCalls ( void );

EXPORT void Usr_ResetAllInCalls ( void );
EXPORT void Usr_UpdateCallPickup ( int our_ref, int status_ok );
EXPORT void Usr_ActOnPickUpRinging ( ulong called, int pickup );
EXPORT void Usr_ActOnInCallConfirmed ( int acb_ref );
EXPORT void Usr_ActOnInCallFailed ( int acb_ref );
EXPORT void Usr_ActOnInCallCleared ( int acb_ref );
EXPORT void Usr_UpdateHeldInCall ( int our_ref, int status_ok );
EXPORT void Usr_UpdateAllHeldInCalls ( int status_ok );
EXPORT void Usr_UpdateListenInCall ( int our_ref, int status_ok );
EXPORT void Usr_UpdateAllListenInCalls ( int status_ok );
EXPORT void Usr_UpdateClearedInCall ( int acb_ref, int status_ok );
EXPORT void Usr_UpdateClearAllInCalls ( int status_ok );

EXPORT void Usr_ProblemsWithIcu ( BITE id );
EXPORT void Usr_ProblemsWithAcbu ( BITE id );
EXPORT void Usr_ProblemsWithEcbu ( BITE id );

EXPORT int  Usr_InitTelephonyMode ( void );
EXPORT void Usr_ExitTelephonyMode ( void );

EXPORT void _pascal Usr_PollUser ( void );

#endif

