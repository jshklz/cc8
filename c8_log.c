#include "c8_log.h"
#include <stdarg.h>
#include <stdio.h>

void c8_log_im(u8* file, u32 line, c8_severity sev, u8* msg, ...)
{
    va_list args = {0};
    va_start(args, msg);
    
    printf("[%s:%d] %s: %s: ",
           file, line, c8_eval_sev(sev), c8_log_sm.active_module);
    
    vprintf(msg, args);
    printf("\n");
    va_end(args);
}


void
c8_push_mod(u8* mod)
{
    c8_log_sm.active_module = mod;
}
void c8_pop_mod(void)
{
    c8_log_sm.active_module = "none";
}



u8* c8_eval_sev(c8_severity sev)
{
    switch (sev){
        
        case C8_ERROR: {
            return "error";
        };
        
        case C8_INFO:
        {
            return "info";
        };
        
        case C8_WARNING:
        {
            return "warning";
        };
        
        default:
        {
            return "none";
        }
    }
}