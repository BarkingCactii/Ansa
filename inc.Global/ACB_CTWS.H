
#ifndef _ACB_CTWS_H_
#define _ACB_CTWS_H_

typedef struct
{
    BITE        mesg_id;
    WORD        csw;
    int         ref_numb;
    int         line_idx;
    int         status_flag;
} FROM_ACB_MESG;

#define NO_ACB_MESSAGE              0x00

#define COM_ACBU_EQUIP_STATUS       0x01
#define COM_ACBU_EQUIP_RESET        0x02
#define COM_ACBU_EQUIP_TEST         0x03
#define COM_PTT_ON                  0x04
#define COM_PTT_OFF                 0x05
#define COM_HEADPHONE_IN            0x06
#define COM_HEADPHONE_OUT           0x07
#define COM_IN_CALL_PICKED_UP       0x08
#define COM_IN_CALL_CLEARS          0x09
#define COM_CALL_PICKUP_FAILS       0x10
#define COM_ACBU_ERROR_CODE         0x11

#define COM_AUDIO_MODE_SET_OK       0x12
#define COM_ACBU_PARAM_SET_OK       0x13
#define COM_IN_CALL_PICKUP_OK       0x14
#define COM_CALL_XFR_ECBU_OK        0x15
#define COM_RECLAIM_ECBU_XFR_OK     0x16
#define COM_MUTED_IN_CALL_OK        0x17
#define COM_MUTED_ALL_IN_CALL_OK    0x18
#define COM_LISTEN_TO_IN_CALL_OK    0x19
#define COM_LISTEN_ALL_IN_CALLS_OK  0x20
#define COM_RELEASED_IN_CALL_OK     0x22
#define COM_CLEAR_ALL_IN_CCTS_OK    0x23
#define COM_MON_XFR_ECBU_OK         0x24
#define COM_MONITOR_CLEARED_OK      0x25
#define COM_USER_PTT_ON_OK          0x26
#define COM_USER_PTT_OFF_OK         0x27
#define COM_SET_UP_STATUS_OK        0x28

EXPORT WORD Ecb_GetEquipStatus ( void );

EXPORT int  Acb_PerformEquipTest ( void );
EXPORT int  Acb_SetAudioStatus ( BITE status );
EXPORT int  Acb_ClearPickedUpCall ( int index );
EXPORT int  Acb_PickupRingingCall ( int idx, int c_party, char type, int code );
EXPORT int  Acb_InitCallTransfer ( int idx, ulong route_numb );
EXPORT int  Acb_ReclaimXfrCall ( int idx );
EXPORT int  Acb_MuteInCall ( int idx );
EXPORT int  Acb_MuteAllInCalls ( void );
EXPORT int  Acb_ListenToInCall ( int idx );
EXPORT int  Acb_ListenToAllInCalls ( void );
EXPORT int  Acb_PostDialToInCall ( int idx, char tone );
EXPORT int  Acb_InitMonitorXfr ( ulong route_numb );
EXPORT int  Acb_ClearMonitorCall ( void );
EXPORT int  Acb_SendPTT_On ( void );
EXPORT int  Acb_SendPTT_Off ( void );
EXPORT WORD Acb_GetEquipStatus ( void );

EXPORT void _pascal Acb_PollPort ( void );

EXPORT int  Acb_InitialiseAcbu ( int flag );

#endif

