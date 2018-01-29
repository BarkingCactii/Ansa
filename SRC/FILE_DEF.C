/*
	file_def.c
	Contains specific details related to the database.

    System  :  Train Control Command Communication System
    Author  :  Ansa Pty. Ltd. (c) 1991
    Revision:  1.0
*/

#include "sysdef.h"
#include "filename.h"

/*
	Key labels
*/

PUBLIC char    *key1 = "Key 1";        /* key 1 name */
PUBLIC char    *key2 = "Key 2";        /* key 2 name */
PUBLIC char    *key3 = "Key 3";        /* key 3 name */
PUBLIC char    *key4 = "Key 4";        /* key 4 name */
PUBLIC char    *key5 = "Key 5";        /* key 5 name */
PUBLIC char    *key6 = "Key 6";        /* key 6 name */
PUBLIC char    *key7 = "Key 7";        /* key 7 name */

/*
	File types - text description
*/

PUBLIC char    *file_description[MAX_FILES + 1] =
{
	" Group Page Labels ",
	" Telephone Numbers ",
	" Passive Line Assignments ",
	" Called Party (Controller) ",
	" Primary Active Assignments ",
	" Secondary Active Assignments ",
	" Command Terminal Grouping ",
	" PABX Configuration ",
	" Command Terminal Configuration ",
	" AMBA (Batch Conferences) ",
	" Controller Passwords ",
	" Controller Group Labels ",
	" Active Assignment Labels ",
	" Party Line Active Assignments ",
	" Selcall Line Active Assignments ",
	" Outgoing Party Line Parties ",
	" ECBU Magazine Labels ",
	" Service Personnel Passwords ",
	" Report Configuration ",
	" Index File 1 ",
	" Index File 2 ",
	" Index File 3 ",
	" Index File 4 ",
	" Index File 5 ",
	" Index File 6 ",
	"Quit Program"
};

/*
	Physical description of the files within the database
*/

PUBLIC FILE_DETAILS    file_attr[MAX_FILES] =
{
	{ "GRPPAGE", "GRP_PAGE", gp_rec_desc, { gp_key1, gp_key2, 0, 0, 0, 0, 0 }},
	{ "PHONENO", "",         pn_rec_desc, { pn_key1, pn_key2, pn_key3, pn_key4, 0, 0, 0 }},
	{ "PASSLIN", "LINE_KEY", la_rec_desc, { la_key1, la_key2, la_key3, 0, 0, 0, 0 }},
	{ "B_PARTY", "CALLED_B", bp_rec_desc, { bp_key1, bp_key2, bp_key3, bp_key4, 0, 0, 0 }},
	{ "GACTIVE", "PRIMARY.", al_rec_desc, { al_key1, al_key2, al_key3, 0, 0, 0, 0 }},
	{ "GASSIGN", "SECONDRY.",ps_rec_desc, { ps_key1, ps_key2, ps_key3, ps_key4, ps_key5, ps_key6, 0 }},
	{ "CTGROUP", "CT_GROUP", hc_rec_desc, { hc_key1, 0,       0, 0, 0, 0, 0 }},
	{ "PABXCFG", "PABX_CFG", cg_rec_desc, { cg_key1, 0,       0, 0, 0, 0, 0 }},
	{ "CTRMCFG", "CTWS_CFG", cc_rec_desc, { cc_key1, cc_key2, cc_key3, cc_key4, cc_key5, cc_key6, cc_key7 }},
	{ "GRPCONF", "GRP_AMBA.",gc_rec_desc, { gc_key1, gc_key2, gc_key3, 0, 0, 0, 0 }},
	{ "CTRPASS", "CT_PWORD", pw_rec_desc, { pw_key1, pw_key2, pw_key3, pw_key4, 0, 0, 0 }},
	{ "OPGROUP", "CNTL_GRP", og_rec_desc, { og_key1, og_key2, 0, 0, 0, 0, 0 }},
	{ "AASSLAB", "",         ol_rec_desc, { ol_key1, ol_key2, 0, 0, 0, 0, 0 }},
	{ "PLACTAS", "PTY_LINE", pl_rec_desc, { pl_key1, pl_key2, pl_key3, pl_key4, 0, 0, 0 }},
	{ "SELCALL", "",         dp_rec_desc, { dp_key1, dp_key2, 0, 0, 0, 0, 0 }},
	{ "OPLPART", "OMNIBUS.", op_rec_desc, { op_key1, op_key2, op_key3, 0, 0, 0, 0 }},
	{ "ECBUMAG", "",         ec_rec_desc, { ec_key1, ec_key2, ec_key3, 0, 0, 0, 0 }},
    { "SERVPAS", "TECHNICS", sp_rec_desc, { sp_key1, sp_key2, 0, 0, 0, 0, 0 }},
    { "RPRTCFG", "",         rc_rec_desc, { rc_key1, 0,       0, 0, 0, 0, 0 }},
	{ "_GRPPAGE","",         mi_rec_desc, { mi_key1, mi_key2, 0, 0, 0, 0, 0 }},
	{ "_PHONENO","",         mi_rec_desc, { mi_key1, mi_key2, 0, 0, 0, 0, 0 }},
	{ "_OPGROUP","",         mi_rec_desc, { mi_key1, mi_key2, 0, 0, 0, 0, 0 }},
	{ "_CTRPASS","",         mi_rec_desc, { mi_key1, mi_key2, 0, 0, 0, 0, 0 }},
	{ "_CTRMCFG","",         mi_rec_desc, { mi_key1, mi_key2, 0, 0, 0, 0, 0 }},
	{ "_ECBUMAG","",         mi_rec_desc, { mi_key1, mi_key2, 0, 0, 0, 0, 0 }}
};

/*
	the field position of an index within a file
	a non zero value indicates the field position within 
	the record which will store an index to a master index file
*/

PUBLIC int inx_pos[MAX_FILES] =
{	GP_INDEX,
	PN_INDEX,
	0,
	0,
	0,
	0,
	0,
	0,
	CC_INDEX,
	0,
	PW_INDEX,
	OG_INDEX,
	0,
	0,
	0,
	0,
	EC_INDEX,
	0,
	0,
	0,
	0,
	0,
	0,
   0,
   0,
};

/*
	we know where the index is stored within a file
	(see above), so we need to describe as to what master index file
	this index field relates to
*/

PUBLIC int ass_indexes[MAX_FILES] =
{	GROUP_INDEX,
	PHONE_INDEX,
	0,
	0,
	0,
	0,
	0,
	0,
	EXTENSION_INDEX,
	0,
	PASSWORD_INDEX,
	OPERATOR_GROUP_INDEX,
	0,
	0,
	0,
	0,
	ECBU_INDEX,
	0,
	0,
	0,
	0,
	0,
	0,
   0,
   0
};

PUBLIC UNIQUE_KEY significant_key[MAX_FILES - MAX_INDEX_FILES][MAX_UNIQUE_KEYS] =
{
   /* group page labels */
	{{ 3, { GP_LABEL,   GP_LABEL+1,    GP_LABEL+2,  	0 },          "Key 2" },
	{ 0,	{ 0,0,0,0 },""  } ,
	{ 0,	{ 0,0,0,0 },""  } ,
	{ 0,	{ 0,0,0,0 },""  },} ,

   /* telephone numbers */
	{{ 3,	{ PN_LABEL,   PN_LABEL+1,    PN_LABEL+2,    0 },          "Key 2" },
	{ 1,	{ PN_NUMBER,  0,             0,             0 },          "Key 3" }, 
	{ 0,	{ 0,0,0,0 },""  } ,
	{ 0,	{ 0,0,0,0 },""  }, } ,

	{{ 0,	{ 0,          0,             0,             0 },          "" } ,
	{ 0,	{ 0,0,0,0 },""  } ,
	{ 0,	{ 0,0,0,0 },""  } ,
	{ 0,	{ 0,0,0,0 },""  },} ,

   /* called party (controller) */
	{{ 1,	{ BP_OG_INDEX,0,             0,             0 }, 			"Key 1" },
	{ 1,	{ BP_STANDARD_NUM,0,         0,             0 },          "Key 2" },
	{ 1,	{ BP_PRIORITY_NUM,0,         0,             0 },          "Key 3" },
	{ 1,	{ BP_EMERGENCY_NUM,0,        0,             0 },          "Key 4" },},

	{{ 0,	{ 0,          0,             0,             0 },				""  } ,
	{ 0,	{ 0,0,0,0 },""  } ,
	{ 0,	{ 0,0,0,0 },""  } ,
	{ 0,	{ 0,0,0,0 },""  },} ,

	{{ 0,	{ 0,          0,             0,	            0 },          "" } ,
	{ 0,	{ 0,0,0,0 },""  } ,
	{ 0,	{ 0,0,0,0 },""  } ,
	{ 0,	{ 0,0,0,0 },""  },} ,

	{{ 1, { HC_OG_INDEX,0,             0,             0 },          "Key 1" },
	{ 0,	{ 0,0,0,0 },""  } ,
	{ 0,	{ 0,0,0,0 },""  } ,
	{ 0,	{ 0,0,0,0 },""  },} ,

	{{ 0,	{ 0,0,0,0 },""  } ,
	{ 0,	{ 0,0,0,0 },""  } ,
	{ 0,	{ 0,0,0,0 },""  } ,
	{ 0,	{ 0,0,0,0 },""  },} ,

   /* command terminal configuration */
	{{ 1,	{ CC_LABEL,   0,             0,             0 },          "Key 2" },
    { 2, { CC_ECBU_MAGAZINE,CC_ECBU_PORT_NO,0,       0 },          "Key 3" },
    { 1, { CC_DDIAL_EXT,0,            0,             0 },          "Key 4" },
    { 1, { CC_INTR_EXT,0,             0,             0 },          "Key 5" },},

   /* AMBA Batch conference */
	{{ 2,	{ GC_PIN_INDEX,GC_PAGE,      0,             0 },          "Key 1" },
/*	{ 3,	{ GC_LABEL,   GC_LABEL+1,    GC_LABEL+2,    0 },          "Key 3" }, */
	{ 0,	{ 0,          0,             0,             0 },          ""  } ,
	{ 0,	{ 0,          0,             0,             0 },          ""  } ,
	{ 0,	{ 0,          0,             0,             0 },          ""  },} ,

   /* controller passwords */
	{{ 1,	{ PW_PIN,     0,             0,             0 },          "Key 2" },
	{ 0,	{ 0,          0,             0,             0 },          ""  } ,
	{ 0,	{ 0,          0,             0,             0 },          ""  } ,
	{ 0,	{ 0,          0,             0,             0 },          ""  },} ,

	{{ 1,	{ OG_NAME,0,0,0 },"Key 2" },
	{ 0,	{ 0,          0,              0,              0 },        ""  } ,
	{ 0,	{ 0,          0,              0,              0 },        ""  } ,
	{ 0,	{ 0,          0,              0,              0 },        ""  },} ,

   /* active assignment labels */
	{{ 3,	{ OL_OG_INDEX,OL_OP_TELE,   OL_CONFIG,      0 },	      "Key 1" },
	{ 0,	{ 0,          0,             0,             0 },       ""  } ,
	{ 0,	{ 0,          0,             0,             0 },       ""  } ,
	{ 0,	{ 0,          0,             0,             0 },       ""  },} ,

   /* party line active assignments */
	{{ 0,	{ 0,          0,             0,             0 },       ""  } ,
	{ 0,	{ 0,          0,             0,             0 },       ""  } ,
/*	{{ 3,	{ PL_LABEL_1, PL_LABEL_2,    PL_LABEL_3,    0 },       "Key 1" }, */
    { 1, { PL_ROUTE_NUM,0,            0,             0 },       "Key 2" },
/*    { 1, { PL_CALLER_P,0,             0,             0 },       "Key 3" }, See Ian */
	{ 0,	{ 0,          0,             0,             0 },       ""  },} ,

   /* selcall line active assignments */
	{{ 1,	{ DP_PN_INDEX,0,	            0,		         0 },	      "Key 1" },
	{ 1,	{ DP_CC_INDEX,0,	            0,		         0 },    	"Key 2" },
	{ 0,	{ 0,          0,             0,             0 },       ""  } ,
	{ 0,	{ 0,          0,             0,             0 },       ""  },} ,

   /* outgoing party line parties */
	{{ 2,	{ OP_PN_INDEX,OP_CCIR_TONE,  0,             0 },          "Key 3" },
	{ 0,	{ 0,          0,             0,             0 },          ""  } ,
	{ 0,	{ 0,          0,             0,             0 },          ""  } ,
	{ 0,	{ 0,          0,             0,             0 },          ""  },} ,

   /* ECBU magazine labels */
	{{ 1,	{ EC_LABEL,   0,             0,             0 },          "Key 2" } ,
    { 1, { EC_ROUTE_NUM,0,            0,             0 },       	"Key 3" },
	{ 0,	{ 0,          0,             0,             0 },          ""  } ,
	{ 0,	{ 0,          0,             0,             0 },          ""  },} ,

   /* service personell passwords */
    {{ 1, { SP_PIN,     0,             0,             0 },          "Key 1" },
    { 1, { SP_NAME,    0,             0,             0 },          "Key 2" },
	{ 0,	{ 0,          0,             0,             0 },          ""  }, 
	{ 0,	{ 0,          0,             0,             0 },          ""  },}, 

   /* report configuration */
   {{ 0, { 0,          0,             0,             0 },          "" },
   { 0, { 0,          0,             0,             0 },          "" },
   { 0, { 0,          0,             0,             0 },          "" },
   { 0, { 0,          0,             0,             0 },          "" },}
};

PUBLIC char    *display_order[MAX_FILES - MAX_INDEX_FILES] =
{	"Key 2",
	"Key 2",
	"Key 1",
	"Key 1",
	"Key 1",
	"Key 1",
	"Key 1",
	"Key 1",
	"Key 2",
	"Key 1",
	"Key 2",
	"Key 2",
	"Key 1",
	"Key 1",
	"Key 1",
	"Key 2", /* outgoing party lines */
    "Key 2",
	"Key 1",
	"Key 1",
};

