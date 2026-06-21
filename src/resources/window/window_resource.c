#include "engine/resources/window/window_resource.h"

OBJECT_DEFINE(ntt_WindowResource, ntt_Resource)

static ntt_Result WindowResource_Load(ntt_Resource* pResource);
static ntt_Result WindowResource_Unload(ntt_Resource* pResource);

ntt_Result
ntt_WindowResource_Initialize(ntt_WindowResource* pWindowResource, ntt_Allocator* pAllocator, void* pUserData)
{
	NTT_UNUSED(pUserData);
	OBJECT_INITIALIZE(pWindowResource, ntt_WindowResource, ntt_Resource, NULL);
	((ntt_Resource*)pWindowResource)->resourceType = NTT_RESOURCE_TYPE_WINDOW;
	((ntt_Resource*)pWindowResource)->pfnLoad	   = WindowResource_Load;
	((ntt_Resource*)pWindowResource)->pfnUnload	   = WindowResource_Unload;

	return NTT_RESULT_SUCCESS;
}

ntt_Result ntt_WindowResource_Destroy(ntt_WindowResource* pWindowResource)
{
	OBJECT_DESTROY(pWindowResource, ntt_WindowResource, ntt_Resource);
	return NTT_RESULT_SUCCESS;
}

static ntt_Result WindowResource_Load(ntt_Resource* pResource)
{
	NTT_UNUSED(pResource);
	return NTT_RESULT_SUCCESS;
}

static ntt_Result WindowResource_Unload(ntt_Resource* pResource)
{
	NTT_UNUSED(pResource);
	return NTT_RESULT_SUCCESS;
}
