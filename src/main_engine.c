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

	NTT_ENGINE_INFO("Engine initialized successfully!");

	NTT_SUCCESS_ASSERT(ntt_Object_RegisterType());
	NTT_SUCCESS_ASSERT(ntt_Resources_Register());
	NTT_SUCCESS_ASSERT(ntt_Systems_Register());

	ntt_WindowResourceCreateInfo windowCreateInfo = {"Main Window", 800, 600};
	ntt_WindowResource			 windowResource;
	NTT_SUCCESS_ASSERT(
		ntt_WindowResource_Initialize(&windowResource, g_memoryGlobals.mallocAllocator, &windowCreateInfo));

	ntt_Resource_Load((ntt_Resource*)&windowResource);

	while (ntt_Resource_IsLoading((ntt_Resource*)&windowResource));

	ntt_Resource_Unload((ntt_Resource*)&windowResource);

	while (ntt_Resource_IsUnloading((ntt_Resource*)&windowResource));

	ntt_WindowResource_Destroy(&windowResource);

	NTT_SUCCESS_ASSERT(ntt_Systems_Unregister());
	NTT_SUCCESS_ASSERT(ntt_Resources_Unregister());
	NTT_SUCCESS_ASSERT(ntt_Object_UnregisterType());

	NTT_SUCCESS_ASSERT(ntt_Drivers_Unregister());
	NTT_SUCCESS_ASSERT(ntt_DestroyIDSystem());

	NTT_ENGINE_INFO("Engine shutdown successfully!");

	NTT_SUCCESS_ASSERT(ntt_Logging_Shutdown());
	NTT_SUCCESS_ASSERT(ntt_MemoryGlobals_Destroy());

	return 0;
}