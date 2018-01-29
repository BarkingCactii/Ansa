
/*
 *  File        :   times.h
 *
 *  Project     :   SRA VPR26   -   Enhanced Conference Facility.
 *  Author      :   Ansa Pty. Ltd. (c) 1992
 *
 *  Description :   real-time timer functions and declarations
 *
 */

#ifndef _TIMES_H_
#define _TIMES_H_

#define MIDNITE_TICK_COUNT  1573040L            /* ticks til midnite    */
#define SECONDS             18L                 /* ticks/sec is 18.2    */
#define MINUTES             1092L               /* ticks per minute     */
#define HOURS               (60L * MINUTES)     /* ticks per hour       */

EXPORT  void  Gen_GetStartTime ( ulong *clock_count );
EXPORT  ulong Gen_TimeElapsed ( ulong start_time );
EXPORT  void  Gen_Delay ( ulong number_of_ticks );

/*
 *  Function    :   GetStartTime
 *
 *  Description :   mark this point in time and store the result
 *                  in clock_count
 *
 *  Inputs      :   clock_count -   address of var to hold start
 *                                  time
 *
 *  Outputs     :   clock_count -   current number of ticks passed
 *                                  midnight
 */

/*
 *  Function    :   TimeElapsed
 *
 *  Description :   calculate the number of ticks that have passed
 *                  since start_time was set
 *
 *  Precondition:   the parameter start_time should have been set
 *                  by a prior call to the GetStartTime function
 *
 *  Inputs      :   start_time  -   var set by previous call to
 *                                  GetStartTime
 *
 *  Return Value:   (ulong) number of ticks that have elapsed since
 *                  from start_time to invocation
 */

/*
 *  Function    :   Delay
 *
 *  Description :   wait for a period of number_of_ticks and then
 *                  return from the function call
 *
 *  Inputs      :   number_of_ticks -   to delay by
 */

/*
 *  Each clock_count is 54.9ms and approximately 18 ticks to the
 *  second, ie.
 *                  54.9 * 18 = 988.2   (nearly 1000 ms)
 *
 *  Example:    To delay for at least 3 seconds use the format
 *
 *                  ulong   time_now, time_out;
 *
 *                  GetStartTime ( &time_now );
 *                      ..
 *                  time_out = 3L * SECONDS;
 *                      ..
 *                  while ( .. )
 *                  {
 *                      if ( TimeElapsed ( time_now ) > time_out )
 *                          time out has occurred
 *                      ..
 *                  }
 */

#endif

