#ifndef OS_MEMORY_H
#define OS_MEMORY_H

#include <base/base.h>

EPOXY_function void* OS_Allocate(i32 size);
EPOXY_function void  OS_Free(void* address, u32 size);


#endif

