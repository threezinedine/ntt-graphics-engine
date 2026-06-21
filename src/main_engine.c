#include "engine/engine.h"
#include <stdio.h>

int	   g_argc;
char** g_argv;

int main(i32 argc, char** argv)
{
	g_argc = argc;
	g_argv = argv;

	NTT_SUCCESS_ASSERT(ntt_InitializeIDSystem());
	NTT_SUCCESS_ASSERT(ntt_MemoryGlobals_Initialize());
	NTT_SUCCESS_ASSERT(ntt_Drivers_Register());

	NTT_SUCCESS_ASSERT(ntt_Object_RegisterType());
	NTT_SUCCESS_ASSERT(ntt_Systems_Register());

	ntt_Object object;
	NTT_SUCCESS_ASSERT(ntt_Object_Initialize(&object, g_memoryGlobals.mallocAllocator));

	ntt_Object_Destroy(&object);

	NTT_SUCCESS_ASSERT(ntt_Systems_Unregister());
	NTT_SUCCESS_ASSERT(ntt_Object_UnregisterType());

	NTT_SUCCESS_ASSERT(ntt_Drivers_Unregister());
	NTT_SUCCESS_ASSERT(ntt_MemoryGlobals_Destroy());
	NTT_SUCCESS_ASSERT(ntt_DestroyIDSystem());

	return 0;
}