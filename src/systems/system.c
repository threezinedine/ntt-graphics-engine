#include "engine/systems/system.h"

OBJECT_DEFINE(ntt_System, ntt_Object)

void ntt_SystemInitialize(ntt_System* pSystem, ntt_Allocator* pAllocator)
{
	OBJECT_INITIALIZE(pSystem, ntt_System, ntt_Object);
}

void ntt_SystemDestroy(ntt_System* pSystem)
{
	OBJECT_DESTROY(pSystem, ntt_System, ntt_Object);
}