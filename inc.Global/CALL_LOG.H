
#ifndef _CALL_LOG_H_
#define _CALL_LOG_H_

#define CPI_GENERAL_ERROR       101
#define CPI_GENERAL_REPORT      102
#define CPI_LOG_FREQ_ERROR      103

EXPORT void Mnt_LogCallRecord ( char* func, char* fname, int line, int code,\
                                                ulong a, ulong b, int c,    \
                                                    int ct, char mesg[] );
#endif

