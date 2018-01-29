
#ifndef _QMM_FUNC_H_
#define _QMM_FUNC_H_

#define ICUX_DEV     1
#define ACBU_DEV     2
#define ECBU_DEV     3
#define NETW_DEV     4
#define CIPS_DEV     5
#define MNTS_DEV     6
#define USER_DEV     7

EXPORT int  Qmm_EnQueueManager ( int dev, void *event );

#endif

