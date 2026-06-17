#include "engine/systems/display/display_system.h"

OBJECT_DEFINE(ntt_DisplaySystem, ntt_System)

ntt_Result ntt_DisplaySystem_Initialize(ntt_DisplaySystem* pDisplaySystem, ntt_Allocator* pAllocator)
{
	OBJECT_INITIALIZE(pDisplaySystem, ntt_DisplaySystem, ntt_System);
	return NTT_RESULT_SUCCESS;
}

ntt_Result ntt_DisplaySystem_Destroy(ntt_DisplaySystem* pDisplaySystem)
{
	OBJECT_DESTROY(pDisplaySystem, ntt_DisplaySystem, ntt_System);
	return NTT_RESULT_SUCCESS;
}