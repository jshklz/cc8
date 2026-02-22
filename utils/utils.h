#ifndef UTILS_H
#define UTILS_H

#include <base/base.h>
#include <os/os.h>
typedef struct U_Clock U_Clock;
struct U_Clock
{
    r32 elapsed;
    r32 now;
    r32 start_time;
    
};



EPOXY_function void
U_ClockStart(U_Clock* clock, OS_App* app);
EPOXY_function void
U_ClockRestart(U_Clock* clock, OS_App* app);
EPOXY_function r32
U_ClockGetElapsedTime(U_Clock* clock, OS_App* app);

EPOXY_function i32
U_GetTextLength(u8* text);

EPOXY_function r32 U_Clamp(r32 value, r32 max, r32 min);

EPOXY_function r32 U_Min(r32 x, r32 y);

EPOXY_function r32 U_Max(r32 x, r32 y);


i32 
U_Compare(const u8* compare , const u8* source);

EPOXY_function r32 
U_Lerp(r32 start, r32 end, r32 time);
#endif


