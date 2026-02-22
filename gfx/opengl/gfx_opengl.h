#ifndef GFX_OPENGL_H
#define GFX_OPENGL_H

#include <os/os.h>
#include <base/base.h>


//~ gfx_opengl only has generic loader functions
// to load opengl.
EPOXY_function i32 GFXGL_LoadOpenGL(OS_App* app);
EPOXY_function void GFXGL_Present(void);

#endif
