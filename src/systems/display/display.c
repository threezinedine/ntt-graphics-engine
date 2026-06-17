#include "engine/systems/display/display.h"

ntt_Result ntt_DisplaySystem_Register()
{
	NTT_SUCCESS_ASSERT(ntt_DisplayComponent_RegisterType());
	NTT_SUCCESS_ASSERT(ntt_DisplaySystem_RegisterType());
	return NTT_RESULT_SUCCESS;
}

ntt_Result ntt_DisplaySystem_Unregister()
{
	NTT_SUCCESS_ASSERT(ntt_DisplaySystem_UnregisterType());
	NTT_SUCCESS_ASSERT(ntt_DisplayComponent_UnregisterType());
	return NTT_RESULT_SUCCESS;
}