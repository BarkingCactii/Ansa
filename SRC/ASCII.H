
#ifndef _ASCII_H_
#define _ASCII_H_

#define DB_EMPTY_LABEL      "      \xa      \xa      \0"

EXPORT  int interrupt_mode;

EXPORT  int  make_ct_file(void);
EXPORT  int  format_label(char **, char *, int);
EXPORT  int  write_header(FILE *, int);
EXPORT  void remove_trail_spaces(char *, int);
EXPORT  void set_process_status(long);
EXPORT  void add_message_to_process_status(char *);
EXPORT  void extract_opgrp_from_ctnum(char *, int *, int *, char *);
EXPORT  int  process_interrupted(void);

#endif
