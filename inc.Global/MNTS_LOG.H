
#ifndef _MNTS_LOG_H_
#define _MNTS_LOG_H_

#include "mnt_path.h"

#define MNTS_ERR_MESG_LEN       80

#define GEN_FILE_OPEN_ERROR     501
#define GEN_FILE_SEEK_ERROR     502
#define GEN_FILE_CLOSE_ERROR    503
#define GEN_FILE_READ_ERROR     504
#define GEN_FILE_WRITE_ERROR    505
#define GEN_FILE_UNLINK_ERROR   506
#define GEN_FILE_ACCESS_ERROR   507
#define GEN_DIR_ACCESS_ERROR    508
#define GEN_MEM_ALLOC_ERROR     509
#define GEN_SET_ATEXIT_ERROR    510
#define GEN_FILE_CREATE_ERROR   511 /* added by JH */
#define GEN_EMS_HANDLE_ERROR    513 /* added by JH */

#define QMM_UNKNOWN_ID_ERROR    601
#define QMM_UNKNOWN_DEV_ERROR   602

#define LAN_INTR_CALL_ERROR     701
#define LAN_IO_DRIVER_ERROR     702
#define LAN_SW_VERSION_ERROR    703
#define LAN_PC_NAME_ERROR       704
#define LAN_RX_BUF_UNDERFLOW    705
#define LAN_RX_BUF_OVERFLOW     706
#define LAN_GENERAL_REPORT      707
#define LAN_CONFIG_ERROR        708
#define LAN_STATUS_REPORT       709
#define LAN_TX_BUF_OVERFLOW     710
#define LAN_TX_QUEUE_OVERFLOW   711
#define LAN_UNEXPECTED_TX_ACK   712
#define LAN_WRONG_SEQ_NO_ERROR  713
#define LAN_TX_RETRY_REPORT     714
#define LAN_TX_NACKED_ERROR     715
#define LAN_REPEAT_RX_REPORT    716
#define LAN_RX_UNKNOWN_ERROR    717
#define LAN_GENERAL_ERROR       718
#define LAN_TOKEN_LOST_ERROR    719

#define CIP_HISTORY_REPORT      801
#define CIP_HISTORY_ERROR       802
#define CIP_GENERAL_REPORT      803
#define CIP_GENERAL_ERROR       804
#define CIP_Q_GRP_RANGE_ERROR   805

#define MNT_LOG_FILE_REPORT     901
#define MNT_LOG_FREQ_ERROR      902
#define MNT_HARD_DRIVE_ERROR    903
#define MNT_GENERAL_REPORT      904
#define MNT_GENERAL_ERROR       905
#define MNT_BUF_OFLOW_ERROR     906
#define MNT_NO_DATA_REPORT      907
#define MNT_VER_MISMATCH_ERROR  908
#define MNT_VER_MATCH_REPORT    909
#define MNT_NEW_DATA_REPORT     910
#define MNT_GET_ENVIRON_NAME    911

#define SMT_GENERAL_ERROR       1001
#define SMT_GENERAL_REPORT      1002

#define SDB_GENERAL_ERROR       1101
#define SDB_GENERAL_REPORT      1102

#define USR_GENERAL_ERROR       1201
#define USR_GENERAL_REPORT      1202
#define USR_DATA_ENTRY_RANGE    1203 /* added by JH */
#define USR_IDX_RANGE_ERROR     1204 /* added by JH */
#define USR_DIR_C_TYPE_ERROR    1205 /* added by JH */
#define USR_ACTION_RANGE_ERROR  1206 /* added by JH */
#define USR_DATA_ENTRY_ERROR    1207 /* added by JH */

EXPORT char Mnt_mnt_err_mesg[MNTS_ERR_MESG_LEN+1];

EXPORT void Mnt_LogMaintError ( char* func, char* fname, int line,          \
                                    int err_no, int status, char mesg[] );
#endif

