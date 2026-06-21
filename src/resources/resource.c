#include "engine/resources/resource.h"
#include "engine/core/logging/logging.h"

OBJECT_DEFINE(ntt_Resource, ntt_Object)

ntt_Result ntt_Resource_Initialize(ntt_Resource* pResource, ntt_Allocator* pAllocator, void* pUserData)
{
	NTT_UNUSED(pUserData);
	OBJECT_INITIALIZE(pResource, ntt_Resource, ntt_Object, NULL);

	pResource->isLoaded	 = FALSE;
	pResource->isLoading = FALSE;

	return NTT_RESULT_SUCCESS;
}

ntt_Result ntt_Resource_Destroy(ntt_Resource* pResource)
{
	OBJECT_DESTROY(pResource, ntt_Resource, ntt_Object);
	return NTT_RESULT_SUCCESS;
}

ntt_Result ntt_Resource_Load(ntt_Resource* pResource)
{
	NTT_ASSERT_IF(pResource == NULL)
	{
		return NTT_RESULT_NULL_POINTER;
	}

	NTT_ASSERT_IF(ntt_Resource_HasInstance(pResource) == FALSE)
	{
		return NTT_RESULT_INVALID_OBJECT_TYPE;
	}

	if (pResource->isLoading == TRUE)
	{
		return NTT_RESULT_RESOURCE_ALREADY_LOADING;
	}

	if (pResource->isLoaded == TRUE)
	{
		return NTT_RESULT_RESOURCE_ALREADY_LOADED;
	}

	pResource->isLoading = TRUE;

	if (pResource->pfnLoad != NULL)
	{
		pResource->pfnLoad(pResource);
	}

	return NTT_RESULT_SUCCESS;
}

b8 ntt_Resource_IsLoading(ntt_Resource* pResource)
{
	NTT_ASSERT_IF(pResource == NULL)
	{
		return FALSE;
	}

	return pResource->isLoading;
}

b8 ntt_Resource_IsLoaded(ntt_Resource* pResource)
{
	NTT_ASSERT_IF(pResource == NULL)
	{
		return FALSE;
	}

	return pResource->isLoaded;
}

b8 ntt_Resource_IsUnloading(ntt_Resource* pResource)
{
	NTT_ASSERT_IF(pResource == NULL)
	{
		return FALSE;
	}

	return pResource->isUnloading;
}

ntt_Result ntt_Resource_Unload(ntt_Resource* pResource)
{
	NTT_ASSERT_IF(pResource == NULL)
	{
		return NTT_RESULT_NULL_POINTER;
	}

	NTT_ASSERT_IF(ntt_Resource_HasInstance(pResource) == FALSE)
	{
		return NTT_RESULT_INVALID_OBJECT_TYPE;
	}

	if (pResource->isLoaded == FALSE)
	{
		return NTT_RESULT_RESOURCE_NOT_LOADED;
	}

	if (pResource->isLoading == TRUE)
	{
		return NTT_RESULT_RESOURCE_ALREADY_LOADING;
	}

	if (pResource->pfnUnload != NULL)
	{
		pResource->isUnloading = TRUE;
		pResource->pfnUnload(pResource);
	}

	return NTT_RESULT_SUCCESS;
}