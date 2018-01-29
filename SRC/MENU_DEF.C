/*
    menu_def.h

	Contains the record definitions for the database

    System  :  Train Control Command Communication System
    Author  :  Ansa Pty. Ltd. (c) 1991
    Revision:  1.0
*/

#include "sysdef.h"
#include "database.h"

/*----------------------------MASTER INDEX RECORD------------------------- */

/*	key description */

PUBLIC char    *mi_key1[] =
{	"*",
	NULL   
};

PUBLIC char    *mi_key2[] =
{	"flag",
	NULL
};

/*	record description */

PUBLIC char    *mi_rec_desc[] =
{	"flag",
	"*",
	NULL
};

/*	static key values */

PUBLIC char    *active[] =
{	"A",
	NULL
};

PUBLIC char    *empty[] =
{	"E",
	NULL
};

PUBLIC char    *deleteable[] =
{	"D",
	NULL
};

PUBLIC char    *ref[] =
{	"Reference",
	NULL
};

PUBLIC char    *new_ref[] =
{	"Reference",
	"1",
	NULL
};

/*----------------------------GROUP PAGE RECORD--------------------------- */

/*	key description */

PUBLIC char    *gp_key1[] =
{	"*",
	NULL
};

PUBLIC char    *gp_key2[] =
{	"Label (line 1)",
	"Label (line 2)",
	"Label (line 3)",
	NULL
};

/*	record description */

PUBLIC char    *gp_rec_desc[] =
{	"Label (line 1)",
	"Label (line 2)",
	"Label (line 3)",
	"*",
	NULL
};

/*----------------------------PHONE NUMBER RECORD------------------------- */

/*	key description */

PUBLIC char    *pn_key1[] =
{	"*",
	NULL
};

PUBLIC char    *pn_key2[] =
{	"Label (line 1)",
	"Label (line 2)",
	"Label (line 3)",
	NULL
};

PUBLIC char    *pn_key3[] =
{	"Number",
	NULL
};

PUBLIC char    *pn_key4[] =
{	"Label (line 1)",
	NULL
};

/*	record description */

PUBLIC char    *pn_rec_desc[] =
{	"Label (line 1)",
	"Label (line 2)",
	"Label (line 3)",
	"Number",
	"Line Type",
	"*",
	NULL
};

/*----------------------------PASSIVE LINE ASSIGNMENTS-------------------- */

/*	key description */

PUBLIC char    *la_key1[] =
{	"Extension Number *",
	NULL
};

PUBLIC char    *la_key2[] =
{	"Group Page *",
	"Page",
	"Label Position",
	NULL
};

PUBLIC char    *la_key3[] =
{	"Group Page *",
	NULL
};

/*	record description */

PUBLIC char    *la_rec_desc[] =
{	"Extension Number *",
	"Group Page *",
	"Page",
	"Label Position",
   "Extension Type",
	NULL
};

/*----------------------------B PARTY------------------------------------- */

/*	key description */

PUBLIC char    *bp_key1[] =
{	"Controller Group *",
	NULL
};

PUBLIC char    *bp_key2[] = 
{
	"Standard Number",
   NULL
};

PUBLIC char    *bp_key3[] = 
{
	"Priority Number",
   NULL
};

PUBLIC char    *bp_key4[] = 
{
	"Emergency Number",
   NULL
};

/*	record description */

PUBLIC char    *bp_rec_desc[] =
{	"Controller Group *",
	"Standard Number",
	"Priority Number",
	"Emergency Number",
	NULL
};

/*----------------------------ACTIVE LINE ASSIGNMENTS--------------------- */

/*	key description */

PUBLIC char    *al_key1[] =
{	"Controller Group *",
	"Extension Number *",
	NULL
};

PUBLIC char    *al_key2[] =
{	"Controller Group *",
	NULL
};

PUBLIC char    *al_key3[] =
{	"Extension Number *",
	NULL
};

/*	record description */

PUBLIC char    *al_rec_desc[] =
{	"Controller Group *",
	"Extension Number *",
	NULL
};

/*----------------------------PRIMARY / SECONDARY ACTIVE LINE ASSIGNMENTS-- */

/*	key description */

PUBLIC char    *ps_key1[] =
{	"Controller Group *",
	"Controllers in Telephony",
	"Controller Number",
	"Configuration Number",
	"Extension Number *",
	NULL
};

PUBLIC char    *ps_key2[] =
{	"Controller Group *",
	"Controllers in Telephony",
	"Configuration Number",
	NULL
};

PUBLIC char    *ps_key3[] =
{	"Controller Group *",
	"Controllers in Telephony",
	"Configuration Number",
	"Controller Number",
	NULL
};

PUBLIC char    *ps_key4[] =
{	"Extension Number *",
	NULL
};

PUBLIC char    *ps_key5[] =
{	"Controller Group *",
	NULL
};

PUBLIC char    *ps_key6[] =
{	"Controller Group *",
	"Controllers in Telephony",
	"Configuration Number",
	"Extension Number *",
	NULL
};

/*	record description */

PUBLIC char    *ps_rec_desc[] =
{	"Controller Group *",
	"Controllers in Telephony",
	"Controller Number",
	"Configuration Number",
	"Extension Number *",
	NULL
};

/*----------------------------COMMAND TERMINAL HARDWARE CONFIGURATION----- */

/*	key description */

PUBLIC char    *hc_key1[] =
{	"Controller Group *",
	NULL
};

/*	record description */

PUBLIC char    *hc_rec_desc[] =
{	"Controller Group *",
	"CT Configuration *_1",
	"CT Configuration *_2",
	"CT Configuration *_3",
	"CT Configuration *_4",
	"CT Configuration *_5",
	"CT Configuration *_6",
	"CT Configuration *_7",
	"CT Configuration *_8",
	"CT Configuration *_9",
	"CT Configuration *_10",
	NULL
};

/*----------------------------GENERAL COMMUNICATIONS CONFIGURATION-------- */

/*	key description */

PUBLIC char    *cg_key1[] =
{
   "Min Queue Group Ext",
	NULL
};

/*	record description */

PUBLIC char    *cg_rec_desc[] =
{	
   "Min Queue Group Ext",
   "Max Queue Group Ext",
	"Call Pickup Code",
   "Max Ring Time",
   "Incomming Overflow Time",
   "Auto Answer Delay Time",
	NULL
};

/*----------------------------SPECIFIC COMMUNICATIONS CONFIGURATION------- */

/*	key description */

PUBLIC char    *cc_key1[] =
{	"*",
	NULL
};

PUBLIC char    *cc_key2[] =
{	"Terminal Designation",
	NULL
};

PUBLIC char    *cc_key3[] =
{   
   "ECBU Magazine",
   "ECBU Port Number",
	NULL
};

PUBLIC char    *cc_key4[] =
{   
	"Direct Dial Extension",
	NULL
};

PUBLIC char    *cc_key5[] =
{   
	"Intrusion Extension",
	NULL
};

PUBLIC char    *cc_key6[] =
{   
   "ECBU Magazine",
	NULL
};

PUBLIC char    *cc_key7[] =
{   
	"CT Target Description (line 1)",
	"CT Target Description (line 2)",
	"CT Target Description (line 3)",
	NULL
};

/*	record description */

PUBLIC char    *cc_rec_desc[] =
{	"Terminal Designation",
   "ECBU Port Number",
   "ECBU Magazine",
	"Base ACBU Extension",
	"Monitor Facility",
	"Direct Dial Extension",
	"Intrusion Extension",
	"CT Target Description (line 1)",
	"CT Target Description (line 2)",
	"CT Target Description (line 3)",
	"*",
	NULL
};

/*----------------------------GROUP CONFERENCE---------------------------- */

/*	key description */

PUBLIC char    *gc_key1[] =
{	"P.I.N. *",
	"Conference Label Position",
	NULL
};

PUBLIC char    *gc_key2[] =
{	"P.I.N. *",
	NULL
};

PUBLIC char    *gc_key3[] =
{
	"Label (line 1)",
	"Label (line 2)",
	"Label (line 3)",
	NULL
};

/*	record description */

PUBLIC char    * far gc_rec_desc[] =
{	"P.I.N. *",
	"Conference Label Position",
	"Label (line 1)",
	"Label (line 2)",
	"Label (line 3)",
	"Extension Number *_1",
	"Extension Number *_2",
	"Extension Number *_3",
	"Extension Number *_4",
	"Extension Number *_5",
	"Extension Number *_6",
	"Extension Number *_7",
	"Extension Number *_8",
	"Extension Number *_9",
	"Extension Number *_10",
	"Extension Number *_11",
	"Extension Number *_12",
	"Extension Number *_13",
	"Extension Number *_14",
	"Extension Number *_15",
	"Extension Number *_16",
	"Extension Number *_17",
	"Extension Number *_18",
	"Extension Number *_19",
   "Number Type 1",
   "Number Type 2",
   "Number Type 3",
   "Number Type 4",
   "Number Type 5",
   "Number Type 6",
   "Number Type 7",
   "Number Type 8",
   "Number Type 9",
   "Number Type 10",
   "Number Type 11",
   "Number Type 12",
   "Number Type 13",
   "Number Type 14",
   "Number Type 15",
   "Number Type 16",
   "Number Type 17",
   "Number Type 18",
   "Number Type 19",
	NULL
};

/*----------------------------PASSWORD------------------------------------ */

/*	key description */

PUBLIC char    *pw_key1[] =
{	"*",
	NULL
};

PUBLIC char    *pw_key2[] =
{	"P.I.N.",
	NULL
};

PUBLIC char    *pw_key3[] =
{	"Controller Group *",
	NULL
};

PUBLIC char    *pw_key4[] =
{	"Group Page *",
	NULL
};

/*	record description */

PUBLIC char    *pw_rec_desc[] =
{	"P.I.N.",
	"Controller Group *",
	"Group Page *",
	"*",
	NULL
};

/*----------------------------OPERATOR GROUP------------------------------ */

/*	key description */

PUBLIC char    *og_key1[] =
{	"*",
	NULL
};

PUBLIC char    *og_key2[] =
{	"Label",
	NULL
};

/*	record description */

PUBLIC char    *og_rec_desc[] =
{	"Label",
	"*",
	NULL
};

/*----------------------------OPERATOR ASSIGNMENT LABELS------------------ */

/*	key description */

PUBLIC char    *ol_key1[] =
{	"Controller Group *",
	"Controllers In Telephony.",
	"Configuration Number",
	NULL
};

PUBLIC char    *ol_key2[] =
{	"Controller Group *",
	NULL
};

/*	record description */

PUBLIC char    * far ol_rec_desc[] =
{	"Controller Group *",
	"Configuration Number",
	"Controllers In Telephony.",
	"Default Controller",
	"label  1 (line 1)",
	"label  1 (line 2)",
	"label  1 (line 3)",
	"label  2 (line 1)",
	"label  2 (line 2)",
	"label  2 (line 3)",
	"label  3 (line 1)",
	"label  3 (line 2)",
	"label  3 (line 3)",
	"label  4 (line 1)",
	"label  4 (line 2)",
	"label  4 (line 3)",
	"label  5 (line 1)",
	"label  5 (line 2)",
	"label  5 (line 3)",
	"label  6 (line 1)",
	"label  6 (line 2)",
	"label  6 (line 3)",
	"label  7 (line 1)",
	"label  7 (line 2)",
	"label  7 (line 3)",
	"label  8 (line 1)",
	"label  8 (line 2)",
	"label  8 (line 3)",
	"label  9 (line 1)",
	"label  9 (line 2)",
	"label  9 (line 3)",
	"label 10 (line 1)",
	"label 10 (line 2)",
	"label 10 (line 3)",
	NULL
};

/*----------------------------PARTY LINE ACTIVE ASSIGNMENTS--------------- */

/*	key description */

PUBLIC char    *pl_key1[] =
{
   "Label (line 1)",
   "Label (line 2)",
   "Label (line 3)",
	NULL
};

PUBLIC char    *pl_key2[] =
{
   "Route Number",
	NULL
};

PUBLIC char    *pl_key3[] =
{
   "Caller Party",
	NULL
};

PUBLIC char    *pl_key4[] =
{
	"CT Configuration *",
	NULL
};

/*	record description */

PUBLIC char    *pl_rec_desc[] =
{
   "Label (line 1)",
   "Label (line 2)",
   "Label (line 3)",
   "Line Type",
   "Route Number",
   "Caller Party",
	"CT Configuration *",
	"Status",
	NULL
};

/*----------------------------SELCALL LINE ASSIGNMENTS-------------------- */

/*	key description */

PUBLIC char    *dp_key1[] =
{	"Selcall line *",
	NULL
};

PUBLIC char    *dp_key2[] =
{	"Assigned Terminal *",
	NULL
};

/*	record description */

PUBLIC char    *dp_rec_desc[] =
{
   "Selcall Line *",
   "Caller Party",
   "Assigned Terminal *",
	"Call Status",
	NULL
};

/*----------------------------OUTGOING PARTY LINE ASSIGNMENTS------------- */

/*	key description */

PUBLIC char    *op_key1[] =
{	"Extension Number *",
	NULL
};

PUBLIC char    *op_key2[] =
{	
	"Extension Number *",
    "Label",
	NULL
};

PUBLIC char    *op_key3[] =
{	
	"Extension Number *",
    "Code",
	NULL
};

/* PUBLIC char    *op_key2[] =
{	"Label",
	"Code",
	"Extension Number *",
	NULL
};
*/
/*	record description */

PUBLIC char    *op_rec_desc[] =
{	"Extension Number *",
	"Label",
	"Code",
	NULL
};

/*----------------------------ECBU MAGAZINE LABELS------------------------ */

/*	key description */

PUBLIC char    *ec_key1[] =
{	"*",
	NULL
};

PUBLIC char    *ec_key2[] =
{	"Magazine Label",
	NULL
};

PUBLIC char    *ec_key3[] =
{   "ECBU Route Number",
   NULL
};

/*	record description */

PUBLIC char    *ec_rec_desc[] =
{	"Magazine Label",
   "ECBU Route Number",
	"*",
	NULL
};

/*----------------------------SERVICE PERSONNEL PASSWORDS----------------- */

/*	key description */

PUBLIC char    *sp_key1[] =
{
   "PIN",
	NULL
};

PUBLIC char    *sp_key2[] =
{
   "Technicians Name", 
   NULL
};

/*	record description */

PUBLIC char    *sp_rec_desc[] =
{
   "PIN",
   "Technicians Name", 
	NULL
};

/*----------------------------REPORT CONFIGURATION------------------------ */

/*	key description */

PUBLIC char    *rc_key1[] =
{
   "Integrity Summary",
	NULL
};

/*	record description */

PUBLIC char    *rc_rec_desc[] =
{
   "Integrity Summary",
   "Record Summary",
   "File Summary",
	NULL
};
