#include "engine/systems/entity.h"

OBJECT_DEFINE(ntt_Entity, ntt_Object)

ntt_Result ntt_Entity_Initialize(ntt_Entity* pEntity, ntt_Allocator* pAllocator, void* pUserData)
{
	NTT_UNUSED(pUserData);
	OBJECT_INITIALIZE(pEntity, ntt_Entity, ntt_Object, pAllocator);
	return NTT_RESULT_SUCCESS;
}

ntt_Result ntt_Entity_Destroy(ntt_Entity* pEntity)
{
	OBJECT_DESTROY(pEntity, ntt_Entity, ntt_Object);
	return NTT_RESULT_SUCCESS;
}