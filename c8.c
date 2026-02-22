


#include <stdio.h>
#include "c8_log.h"
#include <EPOXY.h>
#include <assert.h>
#include "chip8.h"


#include <windows.h>

void c8_fetch_display(c8_machine* machine, u8* recp)
{
    for (i32 y = 0; y < 32; y++)
    {
        
        for (i32 x = 0; x < 64; x ++)
        {
            int index = (y * 64 + x) * 3;
            
            
            if (machine->display[y][x]){
                recp[index + 0] = 255;
                recp[index + 1] = 165; 
                recp[index + 2] = 0; 
            } else 
            {
                recp[index + 0] = 0;
                recp[index + 1] = 0; 
                recp[index + 2] = 0;
            }
        }
    }
}



u8* c8_helper_load_from_file(void)
{
    
    
    
    OPENFILENAME ofn = {0};
    TCHAR file[256] = {0};
    ofn.lStructSize = sizeof(ofn);
    
    ofn.lpstrFile = file;
    ofn.nMaxFile = sizeof(file);
    ofn.lpstrFilter = "Chip 8 ROM(\'.ch8\')\0*.CH8\0All Files (\'*.*\')\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
    
    BOOL result = GetOpenFileNameA(&ofn); 
    if (result == FALSE)
    {
        c8_log(C8_ERROR, "I am unable to invoke GetOpenFileNameA!");
        return 0;
    }
    
    u8* contents = MEM_ArenaAlloc(MEM_GetDefaultArena(), 256);
    CopyMemory(contents, file, 256);
    return contents;
    
}



i32 main(void)
{
    EPOXY_Init();
    
    
    /////////////////////////////////////////////////////////////////
    OS_App* app = OS_ConstructApp("chip8", 
                                  OS_AppDefault,
                                  OS_AppDefault,
                                  OS_AppStyleDefault);
    assert(app && "Failed to create window");
    /////////////////////////////////////////////////////////////////
    
    GFX_Renderer* renderer = GFX_CreateRenderer(app);
    assert(renderer && "Failed to create renderer");
    
    
    
    
    
    b32 success = 0;
    
    
    b32 rom_loaded = 0;
    
    c8_rom rom= {0};
    c8_memory memory = {0};
    c8_cpu cpu = {0};
    c8_machine machine = {0};
    
    
    
    
    u8* data =  MEM_ArenaAlloc(MEM_GetDefaultArena(),  64*32*3);
    ZeroMemory(data, 64 * 32 * 3);
    GFX_Texture2D texture = GFX_LoadDataTexture2D(
                                                  data, 
                                                  v2i(64,32),
                                                  GFX_Format_RGB
                                                  );
    
    
    
    
    
    GFX_Rect display = GFX_CreateRect(v2(app->size.w,app->size.h), v2(0,0));
    display.texture = texture;
    r32 clock_freq = 15;
    U_Clock clock; U_ClockStart(&clock, app);
    
    
    
    
    
    
    
    GFX_Font font = GFX_CreateFont("assets//JetBrainsMono-Regular.ttf",
                                   30);
    
    u8* text_contents = "Left click to open a chip-8 ROM.\nRight-click to switch ROM file upon loading.";
    
    
    
    
    
    GFX_Text text = GFX_CreateText(text_contents,
                                   v2(0,0),
                                   font);
    
    
    
    
    
    // Hard-coded value of 13 for the horizontal spacing (since offical spec states that JetBrainsMono-Regular.ttf has the horizontal spacing of 13 -- and my framework has no ability to provide these values of information yet.)
    
    u32 text_width = U_GetTextLength(text_contents) * 13;
    
    V2F center = 
    {
        .x = ((app->size.w - (text_width-15) ) / 2),
        .y = ( (app->size.h - font.size) / 2)
            
    };
    
    b8 beeping = 0;
    text.pos = center;
    u8* rom_to_load_in = 0;
    
    
    r32 time_initial = OS_GetTime(app);
    
    while (app->running)
    {
        OS_PollEvents(app);
        
        
        
        
        
        GFX_Clear(renderer);
        
        if (rom_to_load_in == 0 && OS_GetLeftMouseState().clicked)
        {
            rom_to_load_in = c8_helper_load_from_file();
            if (rom_to_load_in != 0)
            {
                ZeroMemory(data, 32 * 64 * 3);
                ZeroMemory(machine.display, 32 * 64);
                rom.path_to_rom = rom_to_load_in;
                rom.data = OS_ReadFile(rom_to_load_in);
                memory = c8_memory_initialize(rom);
                machine.memory = memory;
                machine.cpu = &cpu;
                c8_init(&machine);
            }
        }
        
        
        
        if (rom_to_load_in == 0)
        {
            GFX_DrawText(&text, renderer);
        } else 
        {
            
            for (int i = 0; i < 10; i++)
                c8_run(&machine);
            
            
            r32 time_final = OS_GetTime(app);
            r32 dt = time_final - time_initial;
            if (dt >= (1.0f/clock_freq))
            {
                if (machine.cpu->dt > 0) machine.cpu->dt--;
                if (machine.cpu->st > 0) machine.cpu->st--;
                
                time_initial = time_final;
            }
            
            
            if (machine.cpu->st > 0)
            {
                beeping = 1;
                machine.cpu->st--;
            } else
            {
                beeping = 0;
            }
            if (beeping)
            {
                Beep( 750, 16 );
            }
            
            
            
            
            c8_fetch_display(&machine, data);
            GFX_UpdateTexture2D(&display.texture, data, GFX_Format_RGB);
            GFX_DrawRect(&display, renderer);
            
            
            
            
        }
        
        
        
        ZeroMemory(machine.keyboard, 16);
        if (OS_GetKeyState('1').pressed) machine.keyboard[0x1] = 1;
        if (OS_GetKeyState('2').pressed) machine.keyboard[0x2] = 1;
        if (OS_GetKeyState('3').pressed) machine.keyboard[0x3] = 1;
        if (OS_GetKeyState('4').pressed) machine.keyboard[0xC] = 1;
        if (OS_GetKeyState('Q').pressed) machine.keyboard[0x4] = 1;
        if (OS_GetKeyState('W').pressed) machine.keyboard[0x5] = 1;
        if (OS_GetKeyState('E').pressed) machine.keyboard[0x6] = 1;
        if (OS_GetKeyState('R').pressed) machine.keyboard[0xD] = 1;
        if (OS_GetKeyState('A').pressed) machine.keyboard[0x7] = 1;
        if (OS_GetKeyState('S').pressed) machine.keyboard[0x8] = 1;
        if (OS_GetKeyState('D').pressed) machine.keyboard[0x9] = 1;
        if (OS_GetKeyState('F').pressed) machine.keyboard[0xE] = 1;
        if (OS_GetKeyState('Z').pressed) machine.keyboard[0xA] = 1;
        if (OS_GetKeyState('X').pressed) machine.keyboard[0x0] = 1;
        if (OS_GetKeyState('C').pressed) machine.keyboard[0xB] = 1;
        if (OS_GetKeyState('V').pressed) machine.keyboard[0xF] = 1;
        
        if (OS_GetRightMouseState().clicked)
        {
            
            rom_to_load_in = 0;
        }
        
        
        
        
        
        
        
        
        
        GFX_Present(renderer);
    }
    
    EPOXY_Destroy();
    return 0;
}


