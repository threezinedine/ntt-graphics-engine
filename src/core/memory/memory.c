#include "engine/core/memory/memory.h"
#include <string.h>

ntt_MemoryGlobals g_memoryGlobals;

ntt_Result ntt_MemoryGlobalsInitialize()
{
	g_memoryGlobals.mallocAllocator = ntt_CreateMallocAllocator();
	return NTT_RESULT_SUCCESS;
}

ntt_Result ntt_MemoryGlobalsDestroy()
{
	ntt_DestroyAllocator(g_memoryGlobals.mallocAllocator);
	g_memoryGlobals.mallocAllocator = NULL;
	return NTT_RESULT_SUCCESS;
}

ntt_Result ntt_Memset(void* dest, u8 value, usize size)
{
	memset(dest, value, size);
	return NTT_RESULT_SUCCESS;
}