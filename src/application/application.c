#include "engine/application/application.h"
#include "engine/core/core.h"
#include "engine/drivers/drivers.h"
#include "engine/resources/resources.h"
#include "engine/systems/systems.h"

ntt_Result ntt_Application_Initialize(ntt_Application* pApplication)
{
	NTT_ASSERT_IF(pApplication == NULL)
	{
		return NTT_RESULT_NULL_POINTER;
	}

	NTT_SUCCESS_ASSERT(ntt_MemoryGlobals_Initialize());
	NTT_SUCCESS_ASSERT(ntt_Logging_Initialize(LOGGING_HANDLER_TYPE_CONSOLE, NULL));
	NTT_SUCCESS_ASSERT(ntt_Logging_SetLevel(LOGGING_HANDLER_TYPE_CONSOLE, LOGGING_LEVEL_DEBUG));
	NTT_SUCCESS_ASSERT(ntt_InitializeIDSystem());
	NTT_SUCCESS_ASSERT(ntt_Drivers_Register());

	NTT_ENGINE_INFO("Engine initialized successfully!");

	NTT_SUCCESS_ASSERT(ntt_Object_RegisterType());
	NTT_SUCCESS_ASSERT(ntt_Resources_Register());
	NTT_SUCCESS_ASSERT(ntt_Systems_Register());

	if (pApplication->pfn_Initialize != NULL)
	{
		pApplication->pfn_Initialize(pApplication);
	}

	ntt_WindowResourceCreateInfo windowCreateInfo = {"Main Window", 800, 600};
	NTT_SUCCESS_ASSERT(ntt_WindowResource_Initialize(
		&pApplication->defaultWindowResource, g_memoryGlobals.mallocAllocator, &windowCreateInfo));

	ntt_Resource_Load((ntt_Resource*)&pApplication->defaultWindowResource);

	while (ntt_Resource_IsLoading((ntt_Resource*)&pApplication->defaultWindowResource));

	return NTT_RESULT_SUCCESS;
}

ntt_Result ntt_Application_Run(ntt_Application* pApplication)
{
	NTT_ASSERT_IF(pApplication == NULL)
	{
		return NTT_RESULT_NULL_POINTER;
	}

	return NTT_RESULT_SUCCESS;
}

ntt_Result ntt_Application_Shutdown(ntt_Application* pApplication)
{
	NTT_ASSERT_IF(pApplication == NULL)
	{
		return NTT_RESULT_NULL_POINTER;
	}

	ntt_Resource_Unload((ntt_Resource*)&pApplication->defaultWindowResource);

	while (ntt_Resource_IsUnloading((ntt_Resource*)&pApplication->defaultWindowResource));

	ntt_WindowResource_Destroy(&pApplication->defaultWindowResource);

	if (pApplication->pfn_Shutdown != NULL)
	{
		pApplication->pfn_Shutdown(pApplication);
	}

	NTT_SUCCESS_ASSERT(ntt_Systems_Unregister());
	NTT_SUCCESS_ASSERT(ntt_Resources_Unregister());
	NTT_SUCCESS_ASSERT(ntt_Object_UnregisterType());

	NTT_SUCCESS_ASSERT(ntt_Drivers_Unregister());
	NTT_SUCCESS_ASSERT(ntt_DestroyIDSystem());

	NTT_ENGINE_INFO("Engine shutdown successfully!");

	NTT_SUCCESS_ASSERT(ntt_Logging_Shutdown());
	NTT_SUCCESS_ASSERT(ntt_MemoryGlobals_Destroy());

	return NTT_RESULT_SUCCESS;
}