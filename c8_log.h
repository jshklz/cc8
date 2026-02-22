
#ifndef C8_LOG_H
#define C8_LOG_H

#include "c8_types.h"

typedef enum c8_severity c8_severity;
enum c8_severity
{
    C8_ERROR = 1,
    C8_WARNING = 2,
    C8_INFO = 3,
};




typedef struct c8_log_state_machine c8_log_state_machine;
struct c8_log_state_machine
{
    u8* active_module;
};

global c8_log_state_machine c8_log_sm = {.active_module = "none"};


void c8_log_im(u8* file, u32 line, c8_severity sev, u8* msg, ...);

void c8_push_mod(u8* mod);
void c8_pop_mod(void);


u8* c8_eval_sev(c8_severity sev);

#define c8_log(sev, format, ...) c8_log_im(__FILE__, __LINE__, sev, format, __VA_ARGS__)



#endif 
