#include "engine/systems/component.h"

OBJECT_DEFINE(ntt_Component, ntt_Object)

ntt_Result ntt_Component_Initialize(ntt_Component* pComponent, ntt_Allocator* pAllocator, void* pUserData)
{
	NTT_UNUSED(pUserData);
	OBJECT_INITIALIZE(pComponent, ntt_Component, ntt_Object, NULL);
	return NTT_RESULT_SUCCESS;
}

ntt_Result ntt_Component_Destroy(ntt_Component* pComponent)
{
	OBJECT_DESTROY(pComponent, ntt_Component, ntt_Object);
	return NTT_RESULT_SUCCESS;
}