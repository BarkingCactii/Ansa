
#ifndef _NET_CTWS_H_
#define _NET_CTWS_H_

#define MAX_NET_MESG_LEN        20

typedef struct
{
    BITE        mesg_id;
    int         status_flag;
    int         index_1;
    int         index_2;
    BITE        ctws_id;
    BITE        operation;
    BITE        sender[MAX_NET_MESG_LEN+1];
} FROM_NET_MESG;

#define NO_NET_MESSAGE              0x00

#define NET_LAN_EQUIP_STATUS        0x01
#define NET_WE_NOW_OWN_TOKEN        0x02
#define NET_WE_DONT_OWN_TOKEN       0x03
#define NET_START_OPERATION         0x04
#define NET_FINISH_OPERATION        0x05
#define NET_ABORT_OPERATION         0x06
#define NET_REBUILD_OPERATION       0x07
#define NET_DO_AN_EQUIP_TEST        0x08
#define NET_QUEUE_SMT_FILES         0x09
#define NET_DATA_CHANGE_OVER        0x0A
#define NET_QUEUE_LOG_FILE          0x0B
#define NET_LOAD_CALL_HISTORY       0x0C
#define NET_SERVER_TERMINATED       0x0D
#define NET_DEQUEUE_SMT_FILES       0x0E
#define NET_CTWS_TALKING_TO_US      0x0F
#define NET_SET_TIME_AND_DATE       0x10

#define NET_OPERATION_BUSY_OK       0x20
#define NET_CT_CONTACT_NOT_OK       0x21

EXPORT WORD Net_GetEquipStatus ( void );

EXPORT int  Net_ContactServer ( BITE operation );
EXPORT int  Net_ContactTeleCTs ( BITE operation, int arg_1, int arg_2 );
EXPORT int  Net_ContactCtws ( BITE who, BITE oper, int arg_1, int arg_2 );

EXPORT void Net_HardDriveFull ( void );
EXPORT void Net_ConfirmEquipTest ( BITE test, WORD status );
EXPORT void Net_LogRequestReply ( BITE log_type, int status );
EXPORT void Net_PassiveDataCopied ( int status );

EXPORT int  Net_InitUserSession ( char *pin );
EXPORT void Net_ExitUserSession ( void );

EXPORT void _pascal Net_PollNetwork ( void );

EXPORT int  Net_InitialiseNetwork ( int flag, int *my_group, int *who_am_i, \
                                                        int *how_many_cts );
#endif

