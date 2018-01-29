/*
	field.h

    System  :  Train Control Command Communication System
    Author  :  Ansa Pty. Ltd. (c) 1991
    Revision:  1.0
*/

#ifndef _FIELD_H_
#define _FIELD_H_

#define ENTRY                   1       /* field types */
#define CHOICE                  2
#define NON_MANDATORY_ENTRY     3
#define NON_MANDATORY_CHOICE	4

typedef struct
{
    void (*func)(void); /* function to perform              */
    void (*func2)(void); /* function2 to perform             */
    int pr;             /* prompt row                       */
    int pc;             /* prompt column                    */
    char    *prompt;    /* prompt text                      */
    int fr;             /* field row                        */
    int fc;             /* field column                     */
    char    *data;      /* data in field                    */
    int fl;             /* field length                     */
    char    fm;         /* field mask                       */
    int nf;             /* next field                       */
    int pf;             /* previous field                   */
    int valid;          /* field validation passed          */
    int cc;             /* current cursor position          */
    int ft;             /* field type                       */
    char    def;        /* default choice value             */
    int indexval;       /* file to which this index belongs */
    int indexval2;      /* file to which this index belongs */
    char    *table;     /* pointer to choice table          */
    char    **chtext;   /* choice text to be displayed      */
    int help;           /* keyword for help for this field  */
} FIELD;

typedef struct
{
    int nr;             /* number rows     */
    int nc;             /* number cols     */
    int left;           /* leftmost co-ord */
    int top;            /* topmost co-ord  */
    int nf;             /* number fields   */
    FIELD   *cf;        /* current field   */
    int sf;             /* start field     */
} FORM;

#endif
