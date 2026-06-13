#include "engine/systems/display/display_system.h"

OBJECT_DEFINE(ntt_DisplaySystem, ntt_System)

void ntt_DisplaySystemInitialize(ntt_DisplaySystem* pDisplaySystem, ntt_Allocator* pAllocator)
{
	OBJECT_INITIALIZE(pDisplaySystem, ntt_DisplaySystem, ntt_System);
}

void ntt_DisplaySystemDestroy(ntt_DisplaySystem* pDisplaySystem)
{
	OBJECT_DESTROY(pDisplaySystem, ntt_DisplaySystem, ntt_System);
}