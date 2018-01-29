
#ifndef _ECB_CTWS_H_
#define _ECB_CTWS_H_

#define OUT_A_PARTY_LEN 	20

typedef struct
{
    BITE        mesg_id;
    WORD        csw;
    int         ref_numb;
    int         index_1;
    int         index_2;
    int         status_flag;
} FROM_ECB_MESG;

#define NO_ECB_MESSAGE              0x00

#define COM_ECBU_EQUIP_STATUS       0x01
#define COM_PARTY_ANSAWERED         0x02
#define COM_CALL_FROM_ACBU          0x03
#define COM_TRANSFER_ENDED          0x04
#define COM_OUT_CALL_CLEARED        0x05
#define COM_ECBU_ERROR_CODE         0x06

#define COM_OUT_CALL_MADE_OK        0x07
#define COM_EXTENDED_CONF_OK        0x08
#define COM_START_CALL_XTF_OK       0x09
#define COM_TRANSFER_MADE_OK        0x0A
#define COM_END_TRANSFER_OK         0x0B
#define COM_CLEARED_XFR_OK          0x0C
#define COM_RECONNECT_XFR_OK        0x0D
#define COM_MUTED_OUT_CALL_OK       0x0E
#define COM_MUTED_ALL_OUT_CALLS_OK  0x0F
#define COM_LISTEN_TO_OUT_CALL_OK   0x10
#define COM_LISTEN_ALL_OUT_CALLS_OK 0x11
#define COM_OUT_POST_DIALED_OK      0x12
#define COM_RELEASED_OUT_CALL_OK    0x13
#define COM_CLEAR_ALL_OUT_CCTS_OK   0x14
#define COM_STATUS_SET_OK           0x15
#define COM_ECBU_EQUIP_RESET        0x16
#define COM_ECBU_EQUIP_TEST         0x17

EXPORT WORD Ecb_GetEquipStatus ( void );

EXPORT int  Ecb_PerformEquipTest ( void );
EXPORT int  Ecb_InitiateOutCall ( int ref, char *a_party, char line_type );
EXPORT int  Ecb_ClearOutgoingCall ( int ref, int line_index );
EXPORT int  Ecb_ClearAllOutCalls ( void );
PUBLIC int  Ecb_RequestMoreCircuits ( int how_many );
PUBLIC int  Ecb_PostDialToOutCall ( int idx, char *tones );
PUBLIC int  Ecb_MuteOutCall ( int ref, int idx );
PUBLIC int  Ecb_MuteAllOutCalls ( void );
PUBLIC int  Ecb_ListenToOutCall ( int ref, int idx );
PUBLIC int  Ecb_ListenToAllOutCalls ( void );

EXPORT void _pascal Ecb_PollPort ( void );

EXPORT int  Ecb_InitialiseEcbu ( int flag );

#endif

