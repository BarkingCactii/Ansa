
#ifndef _FILE_MON_H_
#define _FILE_MON_H_

#define OPEN_STATE      1
#define READ_STATE      2
#define WRITE_STATE     3 
#define CLOSE_STATE     4

EXPORT  void file_monitor ( int file_no, int state );
EXPORT  void start_file_monitor ( void );

#endif

