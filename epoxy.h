#ifndef EPOXY_H
#define EPOXY_H
#ifdef __cplusplus
extern "C" {
#endif
#include <base/base.h>
#include <os/os_memory.h>
#include <memory/arena.h>
#include <os/os.h>
#ifdef EPOXY_OPENGL
#include <third-party/glad/glad.h>
#endif
#include <gfx/gfx.h>
#include <gfx/gfx_2d.h>
#include <utils/utils.h>
#include <sound/sound.h>
#include <ui/widgets.h>
    
    EPOXY_function inline void
        EPOXY_Init(void)
    {
#ifdef EPOXY_VERBOSE
        char* symbol = "epoxy: multimedia framework for developing applications\nepoxy: report errors: https://github.com/jshklz/epoxy";
        
        EPOXY_Log("%s\n", symbol);
#endif
        EPOXY_Log("EPOXY: You've enabled verbose logging!\n");
        EPOXY_Log("EPOXY: To disable it, please undefine EPOXY_VERBOSE!\n");
        EPOXY_Log("EPOXY: Initialized EPOXY %s\n", EPOXY_VERSION);
        // if exit != 1, then everything succeeded.
        // if exit == 1, then it failed.
        
        
        MEM_Init();
        
    }
    
    
    EPOXY_function inline void
        EPOXY_Destroy(void)
    {
        EPOXY_Log("EPOXY: Destroying all modules for EPOXY\n");
        MEM_Destroy();
        EPOXY_Log("EPOXY: Destroyed all modules\n");
    }
#ifdef __cplusplus
}
#endif.

#endif



