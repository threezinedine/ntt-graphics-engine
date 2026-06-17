#include "engine/systems/systems.h"

ntt_Result ntt_SystemsRegister()
{
	NTT_SUCCESS_ASSERT(ntt_ComponentRegisterType());
	NTT_SUCCESS_ASSERT(ntt_SystemRegisterType());
	NTT_SUCCESS_ASSERT(ntt_DisplaySystemRegister());
	return NTT_RESULT_SUCCESS;
}

ntt_Result ntt_SystemsUnregister()
{
	NTT_SUCCESS_ASSERT(ntt_DisplaySystemUnregister());
	NTT_SUCCESS_ASSERT(ntt_SystemUnregisterType());
	NTT_SUCCESS_ASSERT(ntt_ComponentUnregisterType());
	return NTT_RESULT_SUCCESS;
}