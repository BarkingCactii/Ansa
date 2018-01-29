
#ifndef _LOUD_PNL_H_
#define _LOUD_PNL_H_

EXPORT int  IsSpeechOn ( void );

EXPORT int  Check_PTT_On ( void );
EXPORT int  Check_PTT_Off ( void );

EXPORT int  SetHardwarePTT_On ( BITE state );
EXPORT int  SetHardwarePTT_Off ( BITE state );

EXPORT int  ToggleSpeechMedium ( BITE *to );
EXPORT int  TurnOnSpeechMedium ( BITE *to );
EXPORT void MuteSpeechMedium ( BITE *to );

#endif

