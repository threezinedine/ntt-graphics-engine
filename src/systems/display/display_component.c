#include "engine/systems/display/display_component.h"

OBJECT_DEFINE(ntt_DisplayComponent, ntt_Component)

ntt_Result
ntt_DisplayComponent_Initialize(ntt_DisplayComponent* pDisplayComponent, ntt_Allocator* pAllocator, void* pUserData)
{
	NTT_UNUSED(pUserData);
	OBJECT_INITIALIZE(pDisplayComponent, ntt_DisplayComponent, ntt_Component, NULL);
	return NTT_RESULT_SUCCESS;
}

ntt_Result ntt_DisplayComponent_Destroy(ntt_DisplayComponent* pDisplayComponent)
{
	OBJECT_DESTROY(pDisplayComponent, ntt_DisplayComponent, ntt_Component);
	return NTT_RESULT_SUCCESS;
}