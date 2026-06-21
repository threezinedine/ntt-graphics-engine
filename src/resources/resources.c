#include "engine/resources/resources.h"

ntt_Result ntt_Resources_Register()
{
	NTT_SUCCESS_ASSERT(ntt_Resource_RegisterType());
	NTT_SUCCESS_ASSERT(ntt_WindowResource_RegisterType());
	return NTT_RESULT_SUCCESS;
}

ntt_Result ntt_Resources_Unregister()
{
	NTT_SUCCESS_ASSERT(ntt_WindowResource_UnregisterType());
	NTT_SUCCESS_ASSERT(ntt_Resource_UnregisterType());
	return NTT_RESULT_SUCCESS;
}