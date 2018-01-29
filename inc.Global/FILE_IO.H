/*
 *  File        :   file_io.h
 *
 *  Project     :   SRA VPR26   -   Enhanced Conference Facility.
 *  Author      :   Ansa Pty. Ltd. (c) 1992
 *
 *  Description :   accountable file i/o functions
 */

#ifndef _FILE_IO_H_
#define _FILE_IO_H_

EXPORT int   Gen_open_files;

EXPORT FILE *Gen_Open ( char *fname, char *access );
EXPORT int   Gen_Close ( FILE *fd );

#endif

