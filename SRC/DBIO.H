
#ifndef _DB_IO_H_
#define _DB_IO_H_

EXPORT void file_path(int, char *);
EXPORT Db_Obj *create_database(int, char *[]);
EXPORT Db_Obj *open_database(int);
EXPORT void close_database(Db_Obj *);
EXPORT void close_datafile(Db_Obj *);
EXPORT void exit_database ( void );
EXPORT Db_Obj *dbfile_handle(int);
EXPORT int mark_current_record(int, Index_Obj *);
EXPORT int find_mark(int, Index_Obj *);
EXPORT void flush_database(void);
EXPORT int move_file_cursor(int, char *, char *[], Db_Obj *, Index_Obj *, char *[]);
EXPORT int find_closest_record(char *, char *[], Db_Obj *, Index_Obj *, char *[]);
EXPORT int update_file(int, char *[], Db_Obj *, Index_Obj *);
EXPORT int make_index(Db_Obj *,int);
EXPORT Index_Obj *get_index_handle(Db_Obj *, char *);
EXPORT int update_mindex(int, char *);
EXPORT int delete_mindex(int, char *);
EXPORT int read_record(int, char **, char *, char **, char *);
EXPORT int record_count(Db_Obj *);
EXPORT int record_in_file(Db_Obj *);
EXPORT int record_count_on_keyfield(Db_Obj *, char *, int);

#endif
