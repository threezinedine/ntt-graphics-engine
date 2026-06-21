#include "engine/engine.h"
#include <stdio.h>

int	   g_argc;
char** g_argv;

int main(i32 argc, char** argv)
{
	g_argc = argc;
	g_argv = argv;

	NTT_SUCCESS_ASSERT(ntt_MemoryGlobals_Initialize());
	NTT_SUCCESS_ASSERT(ntt_Logging_Initialize(LOGGING_HANDLER_TYPE_CONSOLE, NULL));
	NTT_SUCCESS_ASSERT(ntt_Logging_SetLevel(LOGGING_HANDLER_TYPE_CONSOLE, LOGGING_LEVEL_DEBUG));
	NTT_SUCCESS_ASSERT(ntt_InitializeIDSystem());
	NTT_SUCCESS_ASSERT(ntt_Drivers_Register());

	NTT_SUCCESS_ASSERT(ntt_Object_RegisterType());
	NTT_SUCCESS_ASSERT(ntt_Systems_Register());

	ntt_Object object;
	NTT_SUCCESS_ASSERT(ntt_Object_Initialize(&object, g_memoryGlobals.mallocAllocator));

	ntt_Object_Destroy(&object);

	NTT_SUCCESS_ASSERT(ntt_Systems_Unregister());
	NTT_SUCCESS_ASSERT(ntt_Object_UnregisterType());

	NTT_SUCCESS_ASSERT(ntt_Drivers_Unregister());
	NTT_SUCCESS_ASSERT(ntt_DestroyIDSystem());
	NTT_SUCCESS_ASSERT(ntt_Logging_Shutdown());
	NTT_SUCCESS_ASSERT(ntt_MemoryGlobals_Destroy());

	return 0;
}