#include "engine/systems/display/display_component.h"

OBJECT_DEFINE(ntt_DisplayComponent, ntt_Component)

ntt_Result ntt_DisplayComponentInitialize(ntt_DisplayComponent* pDisplayComponent, ntt_Allocator* pAllocator)
{
	OBJECT_INITIALIZE(pDisplayComponent, ntt_DisplayComponent, ntt_Component);
	return NTT_RESULT_SUCCESS;
}

ntt_Result ntt_DisplayComponentDestroy(ntt_DisplayComponent* pDisplayComponent)
{
	OBJECT_DESTROY(pDisplayComponent, ntt_DisplayComponent, ntt_Component);
	return NTT_RESULT_SUCCESS;
}