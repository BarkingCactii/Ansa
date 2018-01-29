
#ifndef _NET_SMTS_H_
#define _NET_SMTS_H_

#define MAX_DEVS        10
#define MAX_USERS       30

typedef struct
{
    BITE *name;
    BITE *machine;
} NET_USER_REC;

EXPORT  NET_USER_REC    Net_user_entry[MAX_USERS];

typedef struct
{
    BITE *name;
    BITE *path;
} NET_DEV_REC;

EXPORT  NET_DEV_REC     Net_devices[MAX_DEVS];

EXPORT void Net_RequestLstTest ( BITE test_id );
EXPORT void Net_RequestCtwsTest ( BITE *from, BITE test_id );

EXPORT void Net_RequestLstLog ( BITE log_id );
EXPORT void Net_RequestCtwsLog ( char *from, BITE log_id );
EXPORT void Net_SetSystemTime ( BITE th, BITE tm, BITE ts,                  \
                                                BITE dd, BITE dm, WORD dy );
EXPORT int  Net_GetActiveUsers ( void );
EXPORT int  Net_GetQueueEntries ( void );
EXPORT int  Net_GetRedirectedDevices ( void );
EXPORT int  Net_GetRegisteredUsers ( char *path );

EXPORT void Net_HelloCtws ( BITE *to );
EXPORT void Net_AdviseCtwsUpload ( BITE *to );
EXPORT void Net_CancelCtwsUpload ( BITE *to );
EXPORT void Net_AdviseDoActive ( void );

EXPORT int  Net_PrintFile ( char *what );

EXPORT WORD Net_GetEquipStatus ( void );

EXPORT int  Net_InitialiseNetwork ( int flag );
EXPORT int  _pascal Net_PollNetwork ( int *status );

#endif

