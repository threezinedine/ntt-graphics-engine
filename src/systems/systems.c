#include "engine/systems/systems.h"

ntt_Result ntt_Systems_Register()
{
	NTT_SUCCESS_ASSERT(ntt_Component_RegisterType());
	NTT_SUCCESS_ASSERT(ntt_System_RegisterType());
	NTT_SUCCESS_ASSERT(ntt_DisplaySystem_Register());
	NTT_SUCCESS_ASSERT(ntt_Entity_RegisterType());
	return NTT_RESULT_SUCCESS;
}

ntt_Result ntt_Systems_Unregister()
{
	NTT_SUCCESS_ASSERT(ntt_Entity_UnregisterType());
	NTT_SUCCESS_ASSERT(ntt_DisplaySystem_Unregister());
	NTT_SUCCESS_ASSERT(ntt_System_UnregisterType());
	NTT_SUCCESS_ASSERT(ntt_Component_UnregisterType());
	return NTT_RESULT_SUCCESS;
}