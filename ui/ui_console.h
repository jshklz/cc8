

#ifndef UI_CONSOLE_H
#define UI_CONSOLE_H
#include <EPOXY.h>
typedef struct UI_Console UI_Console;
struct UI_Console
{
    GFX_Rect background;
    GFX_Rect foreground;
    
    b32 visiblity;
    
    MEM_Arena arena;
    struct 
    {
        GFX_Text* texts;
        i32 text_used;
        i32 col;
    } line;
    
    struct
    {
        GFX_Rect base;
        GFX_Rect button;
        i32 offset;
        i32 clicked;
        
    } scrollbar;
    
    GFX_Font  font;
    
};

EPOXY_function void
UI_ShowConsole(UI_Console* console);
EPOXY_function void
UI_HideConsole(UI_Console* console);

EPOXY_function UI_Console UI_CreateConsole(OS_App* app, GFX_Font font);
EPOXY_function void UI_DrawConsole(UI_Console* console, GFX_Renderer* renderer);
EPOXY_function void UI_UpdateConsole(UI_Console* console, GFX_Renderer* renderer, OS_App* app);
EPOXY_function void UI_ConsolePrintLn(UI_Console* console, u8* text);
#endif 

