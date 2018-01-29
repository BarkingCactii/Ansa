
#ifndef _SDB_DEFN_H_
#define _SDB_DEFN_H_

#define MAX_PIN_LEN             3
#define PHONE_NUM_LEN           20
#define KEY_LABEL_LEN           20
#define TONE_SET_LEN            5

#define NUM_OF_LINE_PAGES       5
#define NUM_LINE_KEY_BUTTONS    50
#define NUM_QUEUE_BUTTONS       5
#define NUM_GROUP_PAGES         1

#define MAX_PARTIES_PER_OMNI    16
#define MAX_TERMINALS_IN_GROUP  10

#define LABEL_1_OFFSET          0
#define LABEL_2_OFFSET          7
#define LABEL_3_OFFSET          14
#define KEY_LINE_LABEL_SZ       6
#define MAX_CTS_PER_GROUP       10
#define UNUSED_CT               -1
#define EMPTY_LABEL             "      \n      \n      "
#define MAX_AMBA_MEMBERS        19
#define UNFORMATTED_LABEL_SZ    18
#define PARTY_LN_LABEL_SZ       20

#define RADIO_CALL              'R'
#define S_RADIO_CALL            'S'
#define O_RADIO_CALL            'O'
#define OMNIBUS_CALL            'M'
#define INTERNAL_CALL           'I'
#define EXTERNAL_CALL           'E'
#define SELECTIVE_CALL          'C'
#define CONTROL_CALL            'D'

#define NO_CALL                 0
#define NORMAL_CALL             1
#define PRIORITY_CALL           2
#define EMERGENCY_CALL          3

#define PARTY_EXTN_SEPARATOR    ","

/*
 *  header record
 */

typedef ulong   STD_HDR_REC;

#define STD_HDR_REC_SIZE    sizeof ( STD_HDR_REC )

/*
 *  group page record
 */

typedef char GRP_PAGE_LABEL[KEY_LABEL_LEN + 1];

/*
 *  controller group label record
 */

typedef char CNTL_GRP_LABEL[KEY_LABEL_LEN + 1];

#define CNTL_GRP_LABEL_SZ   sizeof ( CNTL_GRP_LABEL )

/*
 *  called party (controller) record
 */

typedef struct
{
   ulong    normal_call;
   ulong    priority_call;
   ulong    emergency_call;
} GRP_B_PARTY_REC;

#define GRP_B_PARTY_SIZE        sizeof ( GRP_B_PARTY_REC )

/*
 *  command terminal configuration record
 */

typedef struct
{
    char    designation[KEY_LABEL_LEN + 1];
    int     ecbu_port;
    int     ecbu_magazine;    /* not used */
    ulong   base_acbu_extn;
    int     monitor_flag;
    ulong   direct_extn;
    ulong   intrude_extn;
    char    ct_label[KEY_LABEL_LEN + 1];
} CTWS_CFG_REC;

#define CTWS_CFG_SIZE       sizeof ( CTWS_CFG_REC )

/*
 *  command terminal grouping record
 */

typedef struct
{
   int      ct_count;
   int      ct_index[MAX_CTS_PER_GROUP];
} CNTL_GRP_REC;

/*
 *  controller passwords record
 */

typedef struct
{
   char     pin[MAX_PIN_LEN + 1];
   int      cntl_grp_idx;
   int      default_line_page;
} PASSWD_REC;

/*
 *  passive line assignment record
 */

typedef struct
{
   char     number[KEY_LABEL_LEN + 1];
   char     label[KEY_LABEL_LEN + 1];
   char     type;
   int      active_flag;
   int      attr;
} LINE_REC;

typedef struct
{
   LINE_REC line[NUM_LINE_KEY_BUTTONS];
} LINE_PAGE_REC;

typedef LINE_PAGE_REC   GRP_PAGE_REC[NUM_OF_LINE_PAGES];

#define LINE_PAGE_SIZE      sizeof ( LINE_PAGE_REC )
#define GROUP_PAGES_SIZE    LINE_PAGE_SIZE * NUM_OF_LINE_PAGES

/*
 *  AMBA batch conference record
 */

typedef struct
{
   int          count;
   char         label[KEY_LABEL_LEN + 1];
   LINE_REC     member[MAX_AMBA_MEMBERS];
} AMBA_REC;

#define AMBA_REC_SIZE       sizeof ( AMBA_REC )

/*
 *  Outgoing party line record
 */

typedef struct
{
   char     label[UNFORMATTED_LABEL_SZ + 1];
   char     code[TONE_SET_LEN + 1];
} OMNIBUS_REC;

/*
 *  Party line active assignments record
 */

typedef struct
{
   char     label[KEY_LABEL_LEN + 1];
   char     line_type;
   int      route_num;
   ulong    b_party_extn;
   int      ct_index;
   int      status_flag;
} ACTIVE_PL_REC;

#define ACTIVE_PL_SIZE      sizeof ( ACTIVE_PL_REC )

/*
 *  Service Personnel Passwords
 */

typedef struct
{
   char     pin[MAX_PIN_LEN + 1];
   char     name[KEY_LABEL_LEN + 1];
} TECHNIC_REC;

/*
 *  PABX configuration
 */

typedef struct
{
   int      min_queue_extn;
   int      max_queue_extn;
   int      pick_up_code;
   int      extn_ring_time;
   int      call_oflow_time;
   int      auto_ansa_time;
} PABX_CFG_REC;

#define PABX_CFG_SIZE       sizeof ( PABX_CFG_REC )
/*
 *  Primary Assignment Record
 */

typedef struct
{
    ulong   number;
    char    label[KEY_LABEL_LEN+1];
    char    call_type;
} A_PARTY_REC;

#define A_PARTY_REC_SIZE    sizeof ( A_PARTY_REC )

/*
 *  Secondary Assignment Label Records
 */

typedef struct
{
    int     count;
    char    label[KEY_LABEL_LEN+1];
} CT_OP_REC;

#define CT_OP_REC_SIZE      sizeof ( CT_OP_REC )

/*
 *  Secondary Assignment Records
 */

typedef struct
{   int     count;
    int     unknowns;
} CFG_HDR;

#define CFG_HDR_SIZE        sizeof (CFG_HDR)

#endif

