#include "engine/systems/rendering/rendering_system.h"

OBJECT_DEFINE(ntt_RenderingSystem, ntt_System)

ntt_Result
ntt_RenderingSystem_Initialize(ntt_RenderingSystem* pRenderingSystem, ntt_Allocator* pAllocator, void* pUserData)
{
	NTT_UNUSED(pUserData);
	OBJECT_INITIALIZE(pRenderingSystem, ntt_RenderingSystem, ntt_System, NULL);
	return NTT_RESULT_SUCCESS;
}

ntt_Result ntt_RenderingSystem_Destroy(ntt_RenderingSystem* pRenderingSystem)
{
	OBJECT_DESTROY(pRenderingSystem, ntt_RenderingSystem, ntt_System);
	return NTT_RESULT_SUCCESS;
}