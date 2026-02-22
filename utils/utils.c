#include "utils.h"


EPOXY_function void
U_ClockStart(U_Clock* clock, OS_App* app)
{
    U_ClockRestart(clock, app);
}


EPOXY_function r32
U_ClockGetElapsedTime(U_Clock* clock, OS_App* app)
{
    return (OS_GetTime(app) - clock->start_time);
}
EPOXY_function void
U_ClockRestart(U_Clock* clock, OS_App* app)
{
    
    clock->elapsed = OS_GetTime(app) - clock->start_time;
    clock->start_time = OS_GetTime(app);
}



EPOXY_function i32
U_GetTextLength(u8* text)
{
    i32 length = 0;
    while (text[length++] != '\0');
    return length;
}

EPOXY_function i32
U_Compare(const u8* compare , const u8* source)
{
    i32 success = 1;
    while (*compare != '\0')
    {
        if (*compare != *source)
            return 0;
        success &= (*compare && *source);
        *compare++;
        *source++;
    }
    return success;
    
}

EPOXY_function r32 
U_Lerp(r32 start, r32 end, r32 time)
{
    return start + time * (end - start);
}



EPOXY_function r32 
U_Min(r32 x, r32 y)
{
    return x > y ? y : x;
}

EPOXY_function r32 
U_Max(r32 x, r32 y)
{
    return x > y ? x : y;
}

EPOXY_function r32 
U_Clamp(r32 value, r32 max, r32 min)
{
    return U_Min( U_Max(value, min), max);
}
