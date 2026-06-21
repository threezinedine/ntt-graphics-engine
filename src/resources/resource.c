#include "engine/resources/resource.h"

OBJECT_DEFINE(ntt_Resource, ntt_Object)

ntt_Result ntt_Resource_Initialize(ntt_Resource* pResource, ntt_Allocator* pAllocator)
{
	OBJECT_INITIALIZE(pResource, ntt_Resource, ntt_Object);
	return NTT_RESULT_SUCCESS;
}

ntt_Result ntt_Resource_Destroy(ntt_Resource* pResource)
{
	OBJECT_DESTROY(pResource, ntt_Resource, ntt_Object);
	return NTT_RESULT_SUCCESS;
}