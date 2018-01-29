
#ifndef _COMS_LOG_H_
#define _COMS_LOG_H_

#define ACBU                    100     /* acbu errors start from 100 */
#define ICU1                    200     /* icu1 errors start from 200 */
#define ICU2                    300     /* icu2 errors start from 300 */
#define ICUX                    ICU1    /* both icu errors from 200   */
#define ECBU                    400     /* ecbu errors start from 400 */
#define TSCR                    500     /* tch scr errors start @ 500 */

#define _ACBU(offset)           ( ACBU + offset )
#define _ICU1(offset)           ( ICU1 + offset )
#define _ICU2(offset)           ( ICU2 + offset )
#define _ECBU(offset)           ( ECBU + offset )
#define _TSCR(offset)           ( TSCR + offset )

#define TX_BUFFER_OVERFLOW      1
#define TX_LINE_STATUS_ERROR    2
#define RX_BUFFER_OVERFLOW      3
#define RX_LINE_STATUS_ERROR    4
#define RX_UNEXPECTED_STX       5
#define TX_PORT_UNAVAILABLE     6
#define DIGI_COMS_CHANNEL_N_A   7
#define COMS_PORT_INIT_ERROR    8
#define EQUIP_STATUS_REPORT     9
#define WRONG_EQUIP_CONNECTED   10
#define UNEXPECTED_MESG_ERROR	11
#define UN_USED_MESG_REPORT     12
#define MESSAGE_LENGTH_ERROR    13
#define DIGI_DRIVER_NO_LOADED   14
#define MESG_CHECK_SUM_ERROR    15
#define TX_QUEUE_OVERFLOW       16
#define TX_EQUIP_ACK_TIME_OUT   17
#define WRONG_SEQ_NUMB_ERROR    18
#define UNEXPECTED_ACK_ERROR    19
#define TX_ASYNC_INTR_TIME_OUT  20
#define LOG_FREQUENCY_ERROR     21
#define RX_INCOMPLETE_MESG      22
#define UNEXPECTED_NACK_ERROR   23
#define TX_EQUIP_NACK_ERROR     24


#define COMS_ERR_MESG_LEN       80

EXPORT BITE Mnt_com_err_mesg[COMS_ERR_MESG_LEN+1];

EXPORT void Mnt_LogCommsError ( char* func, char* fname, int line,          \
                                    char *who, int err_no,                  \
                                            WORD err_status, BITE mesg[] );
#endif

