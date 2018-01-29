#ifndef _GETOPT_5_H_
#define _GETOPT_5_H_

#define MAX_VALID_ARGS  3

typedef struct
{
    int     option;
    int    *arg_value;
    int     valid_args[MAX_VALID_ARGS];
} ARG_ITEM;

EXPORT int  Gen_opterr;                     /* Global error flag.       */

EXPORT int  Gen_SetTracingFlags ( int argc, char *argv[], ARG_ITEM arg_table[] );

#endif

