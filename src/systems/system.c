#include "engine/systems/system.h"

OBJECT_DEFINE(ntt_System, ntt_Object)

ntt_Result ntt_System_Initialize(ntt_System* pSystem, ntt_Allocator* pAllocator)
{
	OBJECT_INITIALIZE(pSystem, ntt_System, ntt_Object);
	return NTT_RESULT_SUCCESS;
}

ntt_Result ntt_System_Destroy(ntt_System* pSystem)
{
	OBJECT_DESTROY(pSystem, ntt_System, ntt_Object);
	return NTT_RESULT_SUCCESS;
}