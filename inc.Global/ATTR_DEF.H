
#ifndef _ATTR_DEF_H_
#define _ATTR_DEF_H_

/*
 *  Button Attributes
 */

#define RECT_ATTR               40

#define UNUSED_BUTT_ATTR        41          /* unused button */
#define LINE_UNUSED_ATTR        UNUSED_BUTT_ATTR
#define LINE_BUSY_ATTR          42          /* busy line indication */
#define LINE_DIAL_ATTR          43
#define LINE_ANSWERED_ATTR      44          /* answered incoming call */
#define LINE_FREE_ATTR          45
#define ACTIVE_FREE_ATTR        46
#define DIVERT_FREE_ATTR        47
#define RING_NORMAL_ATTR        48          /* assigned incoming call indic */
#define RING_PRIORITY_ATTR      49          /* assigned incoming call indic */
#define RING_EMERGENCY_ATTR     50          /* assigned incoming call indic */
#define ACTIVE_NORMAL_ATTR      51          /* unassigned incoming call indic */
#define ACTIVE_PRIORITY_ATTR    52          /* unassigned incoming call indic */
#define ACTIVE_EMERGENCY_ATTR   53          /* unassigned incoming call indic */

#define LINE_SELECT_ATTR        54
#define LINE_CBACK_ATTR         55
#define LINE_PICK_UP_ATTR       56

#define FUNC_BUTT_ATTR          57          /* call, dir, clear etc */
#define FEAT_BUTT_ATTR          58          /* hold, listen etc */
#define PANEL_BUTT_ATTR         59

#define GRP_PAGE_ATTR           LINE_BUSY_ATTR

#define MQ_OURS_ATTR            ACTIVE_NORMAL_ATTR
#define MQ_GROUP_ATTR           ACTIVE_PRIORITY_ATTR
#define MQ_OTHERS_ATTR          ACTIVE_EMERGENCY_ATTR

#define NORMAL_ASS_ATTR         RING_PRIORITY_ATTR
#define HILITE_ASS_ATTR         ACTIVE_EMERGENCY_ATTR

typedef struct
{
    char   *label;
    int     attr;
} BUTTON_ATTR;

#endif

