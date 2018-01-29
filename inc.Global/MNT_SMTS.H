
#ifndef _MNT_SMTS_H_
#define _MNT_SMTS_H_

EXPORT WORD Mnt_GetDiskStatus ( void );
EXPORT int  Mnt_RequestSmtLog ( BITE log_id );

EXPORT int  _pascal Mnt_PollMaintenance ( int *status );
EXPORT int  Mnt_InitMaintenance ( void );

#endif

