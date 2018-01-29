#ifndef _OPER_CFG_H_
#define _OPER_CFG_H_

typedef struct
{   int     time_format;
    int     alert_flag;
    int     alert_freq;
    int     direct_flag;
} OPER_CFG_REC;

#define OP_CFG_REC_SIZE     sizeof (OPER_CFG_REC)

#define OP_DATA_PATH        "E:\\OP_DATA\\"

EXPORT  OPER_CFG_REC        op_setting;

EXPORT int GetOperatorsConfig (char *op_pin);
EXPORT int SetOperatorsConfig (void);

#endif

