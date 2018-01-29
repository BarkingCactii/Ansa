
#ifndef _PASSWORD_H_
#define _PASSWORD_H_

#define CTRL_PIN            FALSE
#define TECH_PIN            TRUE

typedef struct
{
    int     class;
    int     group_idx;
    int     default_page;
    char    pin[MAX_PIN_LEN + 1];
    char    name[KEY_LABEL_LEN + 1];
} PIN_REC;

EXPORT PIN_REC   users;   /* current user description */

EXPORT int  ReadPinFile ( int ct_group, char *ops_pin );

EXPORT void SetNewUserAccount ( void );
EXPORT void SetNoUserAccount ( void );

#endif

