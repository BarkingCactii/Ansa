
#ifndef _GEN_FUNC_H_
#define _GEN_FUNC_H_

EXPORT  BITE Binary ( BITE hi_byte, BITE lo_byte );
EXPORT  WORD BinWord ( BITE *bytes );
EXPORT  void MakeInt ( int *dest, BITE *source, int len );
EXPORT  void MakeUlong ( ulong *dest, BITE *source, int len );
EXPORT  int  Gen_GetString ( FILE *fd, char *buf, int len );
EXPORT  char *FormatUlong ( ulong number, char *buffer );

#endif

