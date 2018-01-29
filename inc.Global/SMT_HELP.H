
#ifndef _SMT_HELP_H_
#define _SMT_HELP_H_

#define NO_HELP         0          /* Displays message "No help available" */
#define GLOBAL_HLP      1
#define DEPENDENCY_HLP	2
#define DATABASE_HLP	3
#define STATUS_HLP      20
#define LABEL_HLP       21
#define UNDEF_FLD_HLP	22
#define NUMBER_HLP      23
#define CHOICE_HLP      24
#define CCIR_HLP        25
#define EMERGENCY_HLP	26
#define C_PARTY_NO_HLP	27
#define STANDARD_NO_HLP 28
#define PRIORITY_NO_HLP 29
#define EMERGNCY_NO_HLP	30
#define TERM_NO_HLP     31
#define CONF_NO_HLP     32
#define MON_NO_HLP      33
#define LABEL_POS_HLP	34
#define MIN_QUEUE_EXT   35
#define MAX_QUEUE_EXT 	36
#define CALL_PICKUP_HLP	37
#define MAX_RING_TIME	38
#define IN_OVERFLOW     39
#define AUTO_ANS_DELAY	40
#define PASSIVE_HLP     41
#define ACTIVE_HLP      42
#define PRIMARY_HLP     43
#define CONTROL_GRP_HLP	44
#define NUM_TELE_HLP	45
#define DEF_CONTROL_HLP	46
#define PIN_HLP         47
#define PIN_LABEL_HLP	48
#define PERMISSION_HLP	49
#define OUT_PARTY_HLP	50
#define MDU_CHAN_HLP	51
#define CONFIG_NUM_HLP	52
#define	PTYLN_STAT_HLP  53
#define	CONF_BRIDGE_HLP 54
#define LST_STATUS_HLP  55
#define D_DIAL_EXT      56
#define INTR_EXT        57
#define BASE_ACBU_EXT   58
#define ROUTE_NUM_HLP   59
#define CALLED_PTY_HLP  60
#define CALLER_PTY_HLP  61
#define SERV_PIN_HLP    62
#define TECH_NAME_HLP   63
#define MAIN_MENU       64
#define SYS_MGMNT       65
#define TAPE_BACKUP     66
#define EXITING         67
#define LAN_MGMNT       68
#define DB_MAIN_MENU    69
#define ROUTINE_MAINT   70
#define VIEW_SYS_LOG    71
#define PRINT_SYS_LOG   72
#define HARDWARE_TESTS  73
#define NETWORK_STATUS  74
#define DB_MGMNT_MENU   75

EXPORT int  Smt_PopHelp ( void );
EXPORT void Smt_PushHelp ( int category );
EXPORT void Smt_ShowHelp ( void );
EXPORT void Smt_InitHelp ( void );

#endif

