
#ifndef _MNT_CTWS_H_
#define _MNT_CTWS_H_

typedef struct
{
    BITE        mesg_id;
    BITE        log_type;
    int         status_flag;
} FROM_MNT_MESG;

#define NO_NET_MESSAGE              0x00

#define MNT_LOG_COPY_FINISHED       0x01
#define MNT_TELL_LST_HD_IS_FULL     0x02
#define MNT_SMT_DATA_COPIED         0x03
#define MNT_ONE_MINUTE_WARNING      0x04

EXPORT void Mnt_QuitFileCopy ( void );
EXPORT int  Mnt_GetAsciiFiles ( void );
EXPORT WORD Mnt_GetDiskStatus ( void );
EXPORT int  Mnt_ActOnLogRequest ( BITE log_type );
EXPORT void Mnt_PrepareToReboot ( void );

EXPORT void _pascal Mnt_PollMaintenance ( void );
EXPORT int  Mnt_InitMaintenance ( void );

#endif

