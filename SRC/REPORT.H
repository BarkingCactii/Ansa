#ifndef _REPORT_H
#define _REPORT_H

#define INTEGRITY_SUMMARY          0
#define RECORD_SUMMARY             1
#define FILE_SUMMARY               2

#define ALL_RECORDS_DEFINED        0
#define GROUP_PAGES_ASSIGNED       1
#define SELCALL_EXISTS             2
#define OMNIBUS_PARTIED            3
#define CALLED_PARTY_ASSIGNED      4
#define GROUP_LABELS_GROUPED       5
#define PINS_PER_GROUP             6
#define ACTIVE_ASSIGNMENTS_DEFINED 7
#define CONTROLLER_LABELS_DEFINED  8


EXPORT void ReportIntegrityLine(int);
EXPORT void ReportData(int, char *);
EXPORT void ReportHeader(int);
EXPORT int ReportInitialize(void);
EXPORT int ReportClose(void);

#endif
