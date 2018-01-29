
#ifndef _ICU_CTWS_H_
#define _ICU_CTWS_H_

#define OUT_A_PARTY_LEN 	20

typedef struct
{
    BITE        mesg_id;
    WORD        mask;
    int         seq_number;
    int         status_flag;
    int         orig_ctws_idx;
    int         dest_ctws_idx;
    ulong       a_party;
    ulong       b_party;
    int         c_party;
} FROM_ICU_MESG;

#define NO_ICU_MESSAGE              0x00

#define COM_ICU_1_EQUIP_STATUS		0x01
#define COM_ICU_2_EQUIP_STATUS		0x02
#define COM_INCOMING_CALL_RING		0x03
#define COM_CHECK_FAILED_PICKUP 	0x04
#define COM_INCOMING_CALL_CLR       0x05
#define COM_OTHER_CT_MADE_CALL      0x06
#define COM_CT_IS_CALLING_ME        0x07
#define COM_OTHER_CT_O_CALL_CLEAR   0x08
#define COM_OTHER_CT_CALL_PICKUP    0x09
#define COM_CT_ABLE_TO_INTRUDE      0x0A
#define COM_CT_CLEARS_ALL_O_CALLS   0x0B
#define COM_CT_ICU_1_EQUIP_TEST     0x0C
#define COM_CT_ICU_2_EQUIP_TEST     0x0D
#define COM_CT_AM_I_ON_CALL         0x0E
#define COM_OTHER_CT_I_CALL_CLEAR   0x10
#define COM_CT_CLEARS_ALL_I_CALLS   0x11
#define COM_INCOMING_PSTN_RING      0x12

EXPORT int  Icu_ImOnInternalCall ( int ref, ulong who );
EXPORT int  Icu_CallingAnotherCTW ( int to, int extn, int flag );
EXPORT int  Icu_ClearMyCall ( int ref );
EXPORT int  Icu_AdviseCallPickedUp ( int idx, ulong a, int c );
EXPORT int  Icu_ClearMyPickUp ( int ref );
EXPORT int  Icu_ReplyToIntrudingCTW ( int to, int flag );
EXPORT int  Icu_HaveClearedAllCalls ( void );
EXPORT int  Icu_HaveClearedAllPickUps ( void );
EXPORT int  Icu_EquipmentTest_1 ( void );
EXPORT int  Icu_EquipmentTest_2 ( void );
EXPORT int  Icu_VerifyCtwsOnCall ( WORD i_who, ulong o_who );
EXPORT WORD Icu_GetEquipStatus ( int which );
EXPORT WORD Icu_GetActiveIcuDevice ( void );

EXPORT  void _pascal Icu_PollPort_1 ( void );
EXPORT  void _pascal Icu_PollPort_2 ( void );

EXPORT  int  Icu_InitialiseIcu1 ( int flag, int my_ct_id );
EXPORT  int  Icu_InitialiseIcu2 ( void );

#endif

