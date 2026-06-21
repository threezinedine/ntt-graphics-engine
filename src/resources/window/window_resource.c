#include "engine/resources/window/window_resource.h"
#include "engine/core/logging/logging.h"
#include "engine/drivers/drivers.h"
#include <string.h>

OBJECT_DEFINE(ntt_WindowResource, ntt_Resource)

static ntt_Result WindowResource_Load(ntt_Resource* pResource);
static ntt_Result WindowResource_Unload(ntt_Resource* pResource);

ntt_WindowResource g_defaultWindowResource;

ntt_Result
ntt_WindowResource_Initialize(ntt_WindowResource* pWindowResource, ntt_Allocator* pAllocator, void* pUserData)
{
	OBJECT_INITIALIZE(pWindowResource, ntt_WindowResource, ntt_Resource, NULL);
	((ntt_Resource*)pWindowResource)->resourceType = NTT_RESOURCE_TYPE_WINDOW;
	((ntt_Resource*)pWindowResource)->pfnLoad	   = WindowResource_Load;
	((ntt_Resource*)pWindowResource)->pfnUnload	   = WindowResource_Unload;

	memcpy(&pWindowResource->createInfo, pUserData, sizeof(ntt_WindowResourceCreateInfo));

	return NTT_RESULT_SUCCESS;
}

ntt_Result ntt_WindowResource_Destroy(ntt_WindowResource* pWindowResource)
{
	OBJECT_DESTROY(pWindowResource, ntt_WindowResource, ntt_Resource);
	return NTT_RESULT_SUCCESS;
}

static ntt_Result WindowResource_Load(ntt_Resource* pResource)
{
	ntt_WindowResource* pWindowResource = (ntt_WindowResource*)pResource;
	NTT_ASSERT_IF(pWindowResource == NULL)
	{
		return NTT_RESULT_NULL_POINTER;
	}
	NTT_RESOURCE_DEBUG("Loading new window: %s", pWindowResource->createInfo.title);

	IDResult windowIDResult = g_DisplayDriver->CreateWindow(
		pWindowResource->createInfo.title, pWindowResource->createInfo.width, pWindowResource->createInfo.height);
	NTT_ASSERT_IF(windowIDResult.result != NTT_RESULT_SUCCESS)
	{
		return windowIDResult.result;
	}

	pWindowResource->windowID = windowIDResult.data;

	pResource->isLoading = FALSE;
	pResource->isLoaded	 = TRUE;
	return NTT_RESULT_SUCCESS;
}

static ntt_Result WindowResource_Unload(ntt_Resource* pResource)
{
	NTT_RESOURCE_DEBUG("Unloading window: %s", ((ntt_WindowResource*)pResource)->createInfo.title);

	ntt_WindowResource* pWindowResource = (ntt_WindowResource*)pResource;
	NTT_ASSERT_IF(pWindowResource == NULL)
	{
		return NTT_RESULT_NULL_POINTER;
	}

	NTT_SUCCESS_ASSERT(g_DisplayDriver->DestroyWindow(pWindowResource->windowID));

	pResource->isLoading   = FALSE;
	pResource->isLoaded	   = FALSE;
	pResource->isUnloading = FALSE;
	return NTT_RESULT_SUCCESS;
}
