
/*
 *  File        :   sounds.h
 *
 *  Project     :   SRA VPR26   -   Enhanced Conference Facility.
 *  Author      :   Ansa Pty. Ltd. (c) 1992
 *
 *  Description :   real-time sounds routions and declarations
 */

#ifndef _SOUNDS_H_
#define _SOUNDS_H_

#define MAX_NOTES       20

#define S_ON            1
#define S_OFF           2
#define STICKY          3

#define PRIORITY_1      1
#define PRIORITY_2      2
#define PRIORITY_3      3
#define PRIORITY_4      4

typedef struct
{
    int     type;
    ulong   dur;
    ulong   freq;
} NOTES;

typedef struct
{
    int     count;
    int     priority;
    NOTES   data[MAX_NOTES];
} SOUNDS;

EXPORT int     Gen_sound_effects;

EXPORT SOUNDS  valid;
EXPORT SOUNDS  invalid;
EXPORT SOUNDS  sound_on;

EXPORT void Gen_StopSound ( void );
EXPORT void Gen_QueueSound ( SOUNDS val );

EXPORT void Gen_SoundAction ( void );
EXPORT void Gen_InitSounds ( void );

#endif

