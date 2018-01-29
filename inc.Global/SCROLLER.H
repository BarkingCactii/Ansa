
#ifndef _SCROLLER_H_
#define _SCROLLER_H_

#include "field.h"

typedef struct
{
    char   *title;
    int     top, left, height, width;
    int     margin;
    int     attribute;
    int     selection_attribute;
    int     hilite_attribute;
    int     header_attribute;
    char  **text;
    int     columns;
    int   (*FKeyFunction)(unsigned);
    int     selection_index;            /* for masking top rows           */
    int     non_selectable_column;      /* for masking left hand columns  */
} SCROLL_PARAMS;

EXPORT int  Smt_MultiScroller ( SCROLL_PARAMS *locals );
EXPORT int  Smt_SelectScroller ( SCROLL_PARAMS *locals );

EXPORT int  Smt_Scroller ( char *title, int top, int left, int height,      \
                        int width, int attribute, int selection_attribute,  \
                        char *text[], int columns, int selection_index );

EXPORT int  Smt_PopUpScroller ( char *title, int height, int width,         \
                                int attribute, int selection_attribute,     \
                                char *text[], int columns );

EXPORT int  Smt_FormScroller ( char *title, int top, int left, int height,  \
                                int width, int attribute,                   \
                                int selection_attribute, char *text[],      \
                                int columns, FORM *fmptr, FIELD *fldptr );
#endif

