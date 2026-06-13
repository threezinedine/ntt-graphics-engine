#ifndef _MEMORY_H_
#define _MEMORY_H_

#include "alloc.h"
#include "malloc.h"

/**
 * Holds the singleton data for all the allocators which other parts of the engine can use, such as the default
 *      allocator, the temporary allocator, etc. This is used to provide a global access to the allocators, so that
 *      they can be easily accessed by other parts of the engine without having to pass them around.
 */
typedef struct ntt_MemoryGlobals
{
	ntt_Allocator* mallocAllocator;
} ntt_MemoryGlobals;

extern ntt_MemoryGlobals g_memoryGlobals;

/**
 * Starting all needed global data, resources for the memory management.
 */
void ntt_MemoryGlobalsInitialize();

/**
 * Cleaning up all the global data, resources for the memory management.
 */
void ntt_MemoryGlobalsDestroy();

#endif /* _MEMORY_H_ */