#include "chip8.h"

#include "c8_log.h"

// Attributions:
// http://devernay.free.fr/hacks/chip8/C8TECH10.HTM#2.5
c8_memory c8_memory_initialize(c8_rom rom)
{
    c8_memory memory = {0};
    srand(time(0)); // for rng 
    printf("chip8: info: memory init\n");
    // The first 512 bytes are reserved. Here we will pad the area with zero.
    
    
    for (i32 i = 0x000; i < 0x200; i++)
    {
        memory.map[i] = 0;
    }
    
    // Within the memory address reserved for the interpreter - we include a font. (0x000 to 0x1FF)
    
    u8 font_data[80] = 
    {
        
        // 0
        0xF0,
        0x90,
        0x90,
        0x90,
        0xF0,
        
        // 1
        
        0x20,
        0x60,
        0x20,
        0x20,
        0x70,
        
        // 2
        0xF0,
        0x10,
        0xF0,
        0x80,
        0xF0,
        
        // 3
        0xf0,
        0x10,
        0xf0,
        0x10,
        0xf0,
        
        // 4
        0x90,
        0x90,
        0xf0,
        0x10,
        0x10,
        
        // 5
        0xf0,
        0x80,
        0xf0,
        0x10,
        0xf0,
        
        // 6 
        0xf0,
        0x80,
        0xf0,
        0x90,
        0xf0,
        
        // 7
        0xf0,
        0x10,
        0x20,
        0x40,
        0x40,
        
        // 8
        0xf0,
        0x90,
        0xf0,
        0x90,
        0xf0,
        
        // 9
        0xf0,
        0x90,
        0xf0,
        0x10,
        0xf0,
        
        // A
        0xf0,
        0x90,
        0xf0,
        0x90,
        0x90,
        
        // B
        0xe0,
        0x90,
        0xe0,
        0x90,
        0xe0,
        
        // C
        0xf0,
        0x80,
        0x80,
        0x80,
        0xf0,
        
        // D
        0xe0,
        0x90,
        0x90,
        0x90,
        0xe0,
        
        // E
        0xf0,
        0x80,
        0xf0,
        0x80,
        0xf0,
        
        // F
        0xf0,
        0x80,
        0xf0,
        0x80,
        0x80
    };
    
    
    for (i32 i = 0x050; i < 0x9F; i++)
    {
        memory.map[i] = font_data[i - 0x050];
    }
    
    // The following (0x200, 0xFFF] is reserved for programs.
    
    for (i32 i = 0x200; i <= 0xFFF; i++)
    {
        memory.map[i] = rom.data[i-0x200];
    }
    
    printf("chip8: info: memory init finished\n");
    
    return memory;
    
}

void c8_init(c8_machine* machine)
{
    machine->cpu->pc = 0x200;
}

void c8_run(c8_machine* machine)
{
    // implements the fetch-decode-execute cycle.
    
    
    // FETCH
    
    u8 first_byte = machine->memory.map[machine->cpu->pc];
    u8 second_byte = machine->memory.map[machine->cpu->pc + 1];
    u16 opcode = ((u16)first_byte << 8) | second_byte;
    
    //  printf("active_opcode: %#06hx\n", opcode);
    
    
    
    u16 nnn = opcode & 0x0FFF; // 0x0fff -> 0b00001111111111111
    u16 n   = opcode & 0x000f;
    u8  x   = (opcode >> 8) & 0xF;
    u8  y   = (opcode >> 4) & 0xF;
    u8  kk   = (opcode) & 0xFF;
    
    c8_cpu* cpu = machine->cpu;
    
    switch (opcode & 0xf000 )
    {
        case (0x0000):
        {
            
            
            switch (opcode & 0x00ff){
                case (0x00e0):
                {
                    c8_log(C8_INFO, "opcode: 00E0: clear screen");
                    
                    for (i32 y = 0; y < 32; y++) 
                    {
                        for (i32 x = 0; x < 64; x++)
                            machine->display[y][x] = 0;
                    }
                    cpu->pc += 2;
                } break;
                
                case (0x00ee):
                {
                    c8_log(C8_INFO, "opcode: 00EE: Return from Subroutine");
                    
                    cpu->sp--;
                    cpu->pc = cpu->stack[cpu->sp];
                    cpu->pc += 2;
                } break;
                
                
            }
            
        } break;
        
        case (0x1000):
        {
            
            c8_log(C8_INFO,"opcode: 1NNN: jump");
            cpu->pc = nnn; 
            
        } break;
        
        case(0x2000):
        {
            c8_log(C8_INFO,"opcode: 2NNN: call");
            cpu->stack[cpu->sp] = cpu->pc;
            cpu->sp+=1;
            cpu->pc = nnn;
        } break;
        
        
        
        case(0x3000):
        {
            c8_log(C8_INFO,"opcode: 3XKK: Skip Vx == kk");
            
            if (cpu->v[x] == kk)
                cpu->pc += 4;
            else
                cpu->pc += 2;
            
        } break;
        
        
        case(0x4000):
        {
            c8_log(C8_INFO,"opcode: 4XKK: Skip Vx != kk");
            
            if (cpu->v[x] != kk)
                cpu->pc += 4;
            else
                cpu->pc += 2;
            
        } break;
        
        
        case(0x5000):
        {
            c8_log(C8_INFO,"opcode: 5XY0: Skip Vx == Vy");
            
            if (cpu->v[x] == cpu->v[y])
                cpu->pc += 4;
            else
                cpu->pc += 2;
            
        } break;
        
        
        case (0x6000):
        {
            
            c8_log(C8_INFO, "opcode: 6XNN: set register \'Vx\'");
            cpu->v[x] = kk; 
            cpu->pc += 2;
            
            
        } break;
        case (0x7000):
        {
            c8_log(C8_INFO,"opcode: 7XNN: add nn to register \'Vx\'");
            cpu->v[x] += kk; 
            cpu->pc += 2;
            
        } break;
        
        case (0x8000):
        {
            
            
            switch (opcode & 0x000f){
                
                case (0x0):
                {
                    c8_log(C8_INFO,"opcode: 8XY0: Vx = Vy");
                    
                    cpu->v[x] = cpu->v[y];
                    cpu->pc += 2;;
                    
                }; break;
                
                
                
                case (0x1):
                {
                    
                    c8_log(C8_INFO,"opcode: 8XY1: Vx OR Vy");
                    
                    cpu->v[x] |= cpu->v[y];
                    cpu->pc += 2;
                    
                    
                }; break;
                
                
                case (0x2):
                {
                    c8_log(C8_INFO,"opcode: 8XY2: Vx AND Vy");
                    
                    cpu->v[x] &= cpu->v[y];
                    cpu->pc += 2;
                }; break;
                
                
                
                case (0x3):
                {
                    c8_log(C8_INFO,"opcode: 8XY3: Vx XOR Vy");
                    
                    cpu->v[x] ^= cpu->v[y];
                    cpu->pc += 2;
                }; break;
                
                
                
                case (0x4):
                {
                    
                    c8_log(C8_INFO,"opcode: 8XY4: Vx += Vy -- VF = carry");
                    
                    u32 res = cpu->v[x] + cpu->v[y];
                    cpu->v[0xf] = res > 255;
                    cpu->v[x] = (u8)(res & 0xFF);
                    
                    cpu->pc += 2;
                }; break;
                
                
                case (0x5):
                {
                    
                    c8_log(C8_INFO,"opcode: 8XY5: Vx -= Vy -- VF = NOT borrow");
                    
                    
                    u8 flag = (cpu->v[x] >= cpu->v[y]) ? 1 : 0;
                    cpu->v[x] -= cpu->v[y];
                    
                    cpu->v[0xf] = flag;
                    
                    
                    cpu->pc += 2;
                    
                }; break;
                
                
                case (0x6):
                {
                    
                    c8_log(C8_INFO,"opcode: 8XY6: Shift Right");
                    
                    u8 old_bit_lsb = cpu->v[x] & 0x1;
                    cpu->v[x] >>= 1;
                    cpu->v[0xf] = old_bit_lsb;
                    
                    cpu->pc += 2;
                    
                    
                }; break;
                
                case (0xE):
                {
                    c8_log(C8_INFO,"opcode: 8XYE: Shift Left");
                    
                    
                    u8 old_bit_msb = (cpu->v[x] & 0x80) >> 7;
                    cpu->v[x] <<= 1;
                    cpu->v[0xf] = old_bit_msb;
                    
                    cpu->pc += 2;
                }; break;
                
                
                case (0x7):
                {
                    c8_log(C8_INFO,"opcode: 8XY7: Vx = Vy - Vx -- VF = NOT borrow");
                    
                    
                    u8 flag = (cpu->v[x] <= cpu->v[y]) ? 1 : 0;
                    cpu->v[x] = cpu->v[y] - cpu->v[x];
                    
                    cpu->v[0xf] = flag;
                    
                    
                    cpu->pc += 2;
                    
                }; break;
                
                
                
                
                
                
            }
            
            
        } break;
        
        
        case(0x9000):
        {
            c8_log(C8_INFO,"opcode: 9XY0: Skip Vx != Vy");
            
            if (cpu->v[x] != cpu->v[y])
                cpu->pc += 4;
            else
                cpu->pc += 2;
            
        } break;
        
        
        
        case (0xA000):
        {   
            c8_log(C8_INFO,"opcode: ANNN: set the index register to NNN");
            cpu->i = nnn; 
            cpu->pc += 2;
            
            
        } break;
        
        case(0xB000):
        {
            c8_log(C8_INFO,"opcode: Bnnn: Jump to Location nnn + V0");
            cpu->pc = nnn + cpu->v[0];
            
        } break;
        
        
        case(0xC000):
        {
            c8_log(C8_INFO,"opcode: Cxkk: Sets V[x] = {0 <= x <= 255} & kk");
            
            
            cpu->v[x] = (rand() % 255) & kk;
            
            cpu->pc += 2;
            
        } break;
        
        
        
        case (0xD000):
        {
            c8_log(C8_INFO,"opcode: DXYN: display");
            
            u16 vx = cpu->v[x];
            u16 vy = cpu->v[y];
            cpu->v[0xf] = 0; // collision flag autoset to 0
            
            
            for (i32 row = 0; row < n; row++) 
            {
                u8 sb = machine->memory.map[cpu->i + row]; // get the location of the sprite + the row we are on.
                
                for (i32 column = 0; column < 8; column++) 
                {
                    if ((sb & (0x80 >> column)) != 0) {
                        
                        i32 sx = (vx + column) % 64; 
                        i32 sy = (vy + row) % 32;
                        u8* pixel = &machine->display[sy][sx];
                        
                        
                        if (*pixel == 1) cpu->v[0xF] = 1;
                        *pixel ^= 1;
                    }
                }
                
            }
            
            cpu->pc += 2;
            
        } break;
        
        
        
        case (0xe000):
        {
            
            
            switch (opcode & 0x00ff)
            {
                
                
                case (0x9E):
                {
                    
                    c8_log(C8_INFO,"opcode: EX9E: Skip next instruction when key(Vx) == PRESSED");
                    
                    if (machine->keyboard[cpu->v[x]])
                        cpu->pc += 4;
                    else
                        cpu->pc += 2;
                    
                } break;
                
                
                
                case (0x0A):
                {
                    
                    c8_log(C8_INFO,"opcode: FX0A: Await for key input (paused)");
                    
                    b8 pressed = 0;
                    for (i32 i = 0; 
                         i < 16;
                         i++)
                    {
                        if (machine->keyboard[i])
                        {
                            cpu->v[x] = (u8)i;
                            pressed = 1;
                            break;
                        }
                    }
                    
                    
                    if (pressed) cpu->pc += 2;
                    
                } break;
                
                
                
                
                
                case (0xA1):
                {
                    
                    c8_log(C8_INFO,"opcode: EXA1: Skip next instruction when key(Vx) NOT PRESSED");
                    
                    if (!machine->keyboard[cpu->v[x]])
                        cpu->pc += 4;
                    else
                        cpu->pc += 2;
                    
                } break;
                
            }
            
            
        } break;
        
        
        case (0xf000):
        {
            
            
            switch (opcode & 0x00ff){
                
                
                
                
                
                case (0x07):
                {
                    
                    c8_log(C8_INFO,"opcode: FX07: Vx = Dt");
                    
                    
                    cpu->v[x] = cpu->dt;
                    cpu->pc += 2;
                    
                } break;
                
                
                case (0x15):
                {
                    
                    c8_log(C8_INFO,"opcode: FX15: Dt = Vx");
                    
                    
                    cpu->dt = cpu->v[x];
                    cpu->pc += 2;
                    
                } break;
                
                
                case (0x18):
                {
                    
                    c8_log(C8_INFO,"opcode: FX18: St = Vx");
                    
                    
                    cpu->st = cpu->v[x];
                    cpu->pc += 2;
                    
                } break;
                
                
                case (0x1E):
                {
                    
                    c8_log(C8_INFO,"opcode: FX1E: I += Vx");
                    
                    
                    cpu->i += cpu->v[x];
                    cpu->pc += 2;
                    
                } break;
                
                
                case (0x29):
                {
                    
                    c8_log(C8_INFO,"opcode: FX29: I =  Sprite(Vx)");
                    
                    
                    u8 nibble = cpu->v[x] & 0x0f;
                    
                    // each character is 5 bytes....
                    cpu->i = 0x050 + (nibble * 5);
                    
                    cpu->pc += 2;
                    
                } break;
                
                
                case (0x33):
                {
                    
                    c8_log(C8_INFO,"opcode: FX33: BCD representation of Vx ");
                    
                    
                    // hundreds
                    machine->memory.map[cpu->i] = cpu->v[x] / 100;
                    
                    
                    // tens
                    machine->memory.map[cpu->i + 1] = (cpu->v[x]/ 10) % 10;
                    
                    // ones
                    machine->memory.map[cpu->i + 2] = cpu->v[x]% 10;
                    
                    cpu->pc += 2;
                    
                } break;
                
                case (0x55):
                {
                    
                    c8_log(C8_INFO,"opcode: FX55:Store registers from V0 -> Vx starting at I onto Memory Map ");
                    
                    for (i32 i = 0 ; i <= x; i++)
                    {
                        machine->memory.map[cpu->i + i] = cpu->v[i];
                    }
                    cpu->pc += 2;
                } break;
                
                case (0x65):
                {
                    
                    c8_log(C8_INFO,"opcode: FX65: Read V0 -> Vx from the stored position on the Memory Map");
                    
                    for (i32 i = 0 ; i <= x; i++)
                    {
                        
                        cpu->v[i] = machine->memory.map[cpu->i + i];
                        
                    }
                    cpu->pc += 2;
                    
                } break;
                
                
            };
            
            
            
        } break;
        
        
        
        default: {
            c8_log(C8_WARNING, "unsupported opcode (%#X) - skipped", opcode);
            cpu->pc += 2;
        } break;
        
        
    }
    // printf("\n");
    
    
}

void c8_dump_mem(c8_memory* mem)
{
    // stub
}

