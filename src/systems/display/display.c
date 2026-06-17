#include "engine/systems/display/display.h"

ntt_Result ntt_DisplaySystemRegister()
{
	NTT_SUCCESS_ASSERT(ntt_DisplayComponentRegisterType());
	NTT_SUCCESS_ASSERT(ntt_DisplaySystemRegisterType());
	return NTT_RESULT_SUCCESS;
}

ntt_Result ntt_DisplaySystemUnregister()
{
	NTT_SUCCESS_ASSERT(ntt_DisplaySystemUnregisterType());
	NTT_SUCCESS_ASSERT(ntt_DisplayComponentUnregisterType());
	return NTT_RESULT_SUCCESS;
}