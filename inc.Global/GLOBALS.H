
/*
 *  File        :   globals.h
 *
 *  Project     :   SRA VPR26   -   Enhanced Conference Facility.
 *  Author      :   Ansa Pty. Ltd. (c) 1992
 *
 *  Description :   global definitions and declarations
 *
 */

#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include <stdio.h>

#define BITE        unsigned char
#define WORD        unsigned int
#define uint        unsigned int
#define ulong       unsigned long

#define SUCCESS     1
#define FAILURE     0
#define ERROR       (-1)
#define WARNING     (-2)
#define DANGER      (-3)
#define TRUE        1
#define FALSE       0

#define EQUIP_FAULTY    ERROR
#define EQUIP_OFF_LINE  FALSE
#define EQUIP_ON_LINE   TRUE

#define EXPORT      extern
#define PUBLIC
#define PRIVATE     static

#define STR_EQUAL   0
#define STR_EMPTY   ""
#define SPACE_BAR   ' '

#define SOH         1
#define STX         2
#define CR          13
#define LF          10

#define _ID(s) s, __FILE__, __LINE__

#define _DEC( x, n )    x = ( x > 0 )? x - 1 : n - 1
#define _INC( x, n )    x = ( x < n - 1 )? x + 1 : 0

#ifdef  TRACE
#define _LOG_ERR(cmd)   cmd
#else
#define _LOG_ERR(cmd)
#endif

#ifdef  DL1
#define _PROC(str)      printf ( "### PROC: %s\n", str )
#else
#define _PROC(str)
#endif

#ifdef  DL2
#define _DEBUG(cmd)     cmd
#else
#define _DEBUG(cmd)
#endif

#ifdef  DL3
#define _DEBUG2(cmd)   cmd
#else
#define _DEBUG2(cmd)
#endif

#ifdef  DL4
#define _DEBUG3(cmd)   cmd
#else
#define _DEBUG3(cmd)
#endif

#endif
