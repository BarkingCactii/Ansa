
#ifndef _SMT_IDLE_H_
#define _SMT_IDLE_H_

#include <ibmkeys.h>

EXPORT uint Smt_KbdGet ( void );
EXPORT void Smt_PollWhileIdle ( ulong wait_for, int *status );

#endif

