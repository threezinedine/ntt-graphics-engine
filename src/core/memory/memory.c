#include "engine/core/memory/memory.h"

ntt_MemoryGlobals g_memoryGlobals;

void ntt_MemoryGlobalsInitialize()
{
	g_memoryGlobals.mallocAllocator = ntt_CreateMallocAllocator();
}

void ntt_MemoryGlobalsDestroy()
{
	ntt_DestroyAllocator(g_memoryGlobals.mallocAllocator);
	g_memoryGlobals.mallocAllocator = NULL;
}