/*
	database.h

	Contains the record definitions for the database

    System  :  Train Control Command Communication System
    Author  :  Ansa Pty. Ltd. (c) 1991
    Revision:  1.0
*/

#ifndef _DATABASE_H_
#define _DATABASE_H_

/*----------------------------MASTER INDEX RECORD------------------------- */

/*	values for flag in type MASTER_INDEX */

#define	ACTIVE		"A"
#define	EMPTY		"E"
#define	DELETEABLE	"D"

/*  key description */

EXPORT char    *mi_key1[];
EXPORT char    *mi_key2[];

/*  record description */

#define	MI_FLAG		0
#define	MI_INDEX	1

EXPORT char    *mi_rec_desc[];

/*	static key values */

EXPORT char    *active[];
EXPORT char    *empty[];
EXPORT char    *deleteable[];
EXPORT char    *ref[];
EXPORT char    *new_ref[];

/*----------------------------GROUP PAGE RECORD--------------------------- */

/*	key description */

EXPORT char    *gp_key1[];
EXPORT char    *gp_key2[];

/*  record description */

#define GP_LABEL	0
#define GP_INDEX	3

EXPORT char    *gp_rec_desc[];

/*----------------------------PHONE NUMBER RECORD------------------------- */

/*	key description */

EXPORT char    *pn_key1[];
EXPORT char    *pn_key2[];
EXPORT char    *pn_key3[];
EXPORT char    *pn_key4[];

/*	record description */

#define PN_LABEL	0
#define PN_NUMBER	3
#define PN_LINE_TYPE	4
#define PN_INDEX	5

EXPORT char    *pn_rec_desc[];

/*----------------------------PASSIVE LINE ASSIGNMENTS-------------------- */

/*	key description */

EXPORT char    *la_key1[];
EXPORT char    *la_key2[];
EXPORT char    *la_key3[];

/*	record description */

#define LA_PN_INDEX	0
#define LA_GP_INDEX	1
#define LA_PAGE		2
#define LA_LINE_NO	3
#define LA_LINE_TYPE	4

EXPORT char    *la_rec_desc[];

/*----------------------------B PARTY------------------------------------- */

/*	key description */

EXPORT char    *bp_key1[];
EXPORT char    *bp_key2[];
EXPORT char    *bp_key3[];
EXPORT char    *bp_key4[];

/*	record description */

#define BP_OG_INDEX		0
#define BP_STANDARD_NUM		1
#define BP_PRIORITY_NUM		2
#define BP_EMERGENCY_NUM	3

EXPORT char    *bp_rec_desc[];

/*----------------------------ACTIVE LINE ASSIGNMENTS--------------------- */

/*	key description */

EXPORT char    *al_key1[];
EXPORT char    *al_key2[];
EXPORT char    *al_key3[];

/*	record description */

#define AL_OG_INDEX	0
#define AL_PN_INDEX	1

EXPORT char    *al_rec_desc[];

/*----------------------------PRIMARY / SECONDARY ACTIVE LINE ASSIGNMENTS-- */

/*	key description */

EXPORT char    *ps_key1[];
EXPORT char    *ps_key2[];
EXPORT char    *ps_key3[];
EXPORT char    *ps_key4[];
EXPORT char    *ps_key5[];
EXPORT char    *ps_key6[];

/*	record description */

#define PS_OG_INDEX	0
#define PS_ASSIGN_NUM	1
#define PS_OPERATOR_NUM	2
#define PS_CONFIG_NUM	3
#define PS_PN_INDEX	4

EXPORT char    *ps_rec_desc[];

/*----------------------------COMMAND TERMINAL HARDWARE CONFIGURATION----- */

/*	key description */

EXPORT char    *hc_key1[];

/*	record description */

#define  HC_OG_INDEX	0
#define	HC_START_EXT	1
#define	HC_END_EXT	10

EXPORT char    *hc_rec_desc[];

/*----------------------------GENERAL COMMUNICATIONS CONFIGURATION-------- */

/*	key description */

EXPORT char    *cg_key1[];

/*	record description */

#define	CG_MIN_QUEUE	0
#define	CG_MAX_QUEUE	1
#define	CG_PICKUP		2
#define	CG_MAX_RING 	3
#define  CG_IN_OFLOW    4
#define  CG_AANS_DELAY  5

EXPORT char    *cg_rec_desc[];

/*----------------------------SPECIFIC COMMUNICATIONS CONFIGURATION------- */

/*	key description */

EXPORT char    *cc_key1[];
EXPORT char    *cc_key2[];
EXPORT char    *cc_key3[];
EXPORT char    *cc_key4[];
EXPORT char    *cc_key5[];
EXPORT char    *cc_key6[];
EXPORT char    *cc_key7[];

/*  record description */

#define CC_LABEL	         0
#define CC_ECBU_PORT_NO	   1
#define CC_ECBU_MAGAZINE   2
#define CC_BASE_ACBU_EXT	3
#define CC_MONITOR_FAC  	4
#define CC_DDIAL_EXT      	5
#define CC_INTR_EXT     	6
#define CC_CT_TARGET_DESC1	7
#define CC_CT_TARGET_DESC2	8
#define CC_CT_TARGET_DESC3	9
#define CC_INDEX           10

EXPORT char    *cc_rec_desc[];

/*----------------------------GROUP CONFERENCE---------------------------- */

/*	key description */

EXPORT char    *gc_key1[];
EXPORT char    *gc_key2[];
EXPORT char    *gc_key3[];

/*	record description */

#define GC_PIN_INDEX	   0
#define GC_PAGE         1
#define GC_LABEL        2
#define GC_PN_INDEX     5

EXPORT char    *gc_rec_desc[];

/*----------------------------PASSWORD------------------------------------ */

/*	key description */

EXPORT char    *pw_key1[];
EXPORT char    *pw_key2[];
EXPORT char    *pw_key3[];
EXPORT char    *pw_key4[];

/*	record description */

#define PW_PIN    		0
#define PW_OG_INDEX     1
#define PW_GP_INDEX     2
#define PW_INDEX        3

EXPORT char    *pw_rec_desc[];

/*----------------------------OPERATOR GROUP------------------------------ */

/*	key description */

EXPORT char    *og_key1[];
EXPORT char    *og_key2[];

/*	record description */

#define OG_NAME		0
#define OG_INDEX	1

EXPORT char    *og_rec_desc[];

/*----------------------------OPERATOR ASSIGNMENT LABELS------------------ */

/*	key description */

EXPORT char    *ol_key1[];
EXPORT char    *ol_key2[];

/*	record description */

#define OL_OG_INDEX	0
#define OL_CONFIG	   1
#define OL_OP_TELE	2
#define OL_DEFAULT	3
#define OL_LABEL	   4

EXPORT char    *ol_rec_desc[];

/*----------------------------PARTY LINE ASSIGNMENTS---------------------- */

/*	key description */

EXPORT char    *pl_key1[];
EXPORT char    *pl_key2[];
EXPORT char    *pl_key3[];
EXPORT char    *pl_key4[];

/*	record description */

#define PL_LABEL_1   0
#define PL_LABEL_2   1
#define PL_LABEL_3   2
#define PL_LINE_TYPE 3
#define PL_ROUTE_NUM 4
#define PL_CALLER_P  5
#define PL_CC_INDEX	6
#define PL_STATUS	   7

EXPORT char    *pl_rec_desc[];

/*----------------------------D PARTY------------------------------------- */

/*	key description */

EXPORT char    *dp_key1[];
EXPORT char    *dp_key2[];

/*	record description */

#define DP_PN_INDEX	   0
#define DP_CALLED_PARTY 1
#define DP_CC_INDEX	   2
#define DP_CALL_STATUS  3

EXPORT char    *dp_rec_desc[];

/*----------------------------OUTGOING PARTY LINE ASSIGNMENTS------------- */

/*	key description */

EXPORT char    *op_key1[];
EXPORT char    *op_key2[];
EXPORT char    *op_key3[];

/*	record description */

#define OP_PN_INDEX     0
#define OP_LABEL        1
#define OP_CCIR_TONE    2

EXPORT char    *op_rec_desc[];

/*----------------------------ECBU MAGAZINE LABELS------------------------ */

/*	key description */

EXPORT char    *ec_key1[];
EXPORT char    *ec_key2[];
EXPORT char    *ec_key3[];

/*	record description */

#define EC_LABEL        0
#define EC_ROUTE_NUM    1
#define EC_INDEX        2

EXPORT char    *ec_rec_desc[];

/*----------------------------SERVICE PERSONNEL PASSWORDS----------------- */

/*	key description */

EXPORT char    *sp_key1[];
EXPORT char    *sp_key2[];

/*	record description */

#define SP_PIN        0
#define SP_NAME       1

EXPORT char    *sp_rec_desc[];

/*----------------------------REPORT CONFIGURATION------------------------ */

/*	key description */

EXPORT char    *rc_key1[];

/*	record description */

#define RC_INTEGRITY    0
#define RC_RECORD       1
#define RC_FILE         2

EXPORT char    *rc_rec_desc[];

#endif

