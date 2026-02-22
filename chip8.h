#ifndef CHIP_8_H
#define CHIP_8_H


#include "c8_types.h"
#include <time.h>
typedef struct c8_rom c8_rom;
struct c8_rom
{
    u8* path_to_rom;
    u8* data;
};


typedef struct c8_memory c8_memory;
struct c8_memory
{
    u8 map[4096]; // ~ 4KB
};

typedef struct c8_cpu c8_cpu;
struct c8_cpu
{   
    u8 v[0xF];
    u16 i;
    
    
    u16 pc;
    u8 sp;
    
    
    u16 stack[16];
    
    
    u8 dt;
    u8 st;
    
    
};


typedef struct c8_machine c8_machine;
struct c8_machine
{
    c8_memory memory;
    c8_cpu* cpu;
    
    
    u8 display[32][64];
    u8 keyboard[16];
};

c8_memory c8_memory_initialize(c8_rom rom);
void c8_dump_mem(c8_memory* mem);
void c8_init(c8_machine* machine);

void c8_run(c8_machine* machine);

#endif /* CHIP_8_H */