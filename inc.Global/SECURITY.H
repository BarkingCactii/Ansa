
#ifndef _SECURITY_H_
#define _SECURITY_H_

#define PROG_NAME       "  SYSTEM MANAGEMENT TERMINAL"
#define COMPANY         "         ANSA Pty Ltd"

EXPORT int  Smt_protected_flag;

EXPORT int  Smt_DbasePassword ( void );
EXPORT void Smt_ChangePassword ( void );
EXPORT int  Smt_BlankForPassword ( void );

#endif

