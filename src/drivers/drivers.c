#include "engine/drivers/drivers.h"
#include "engine/core/logging/logging.h"

ntt_Result ntt_Drivers_Register()
{
	NTT_DRIVER_INFO("Registering drivers...");

	// Initialize drivers here
#if NTT_GRAPHICS_DRIVER_GLFW
	NTT_SUCCESS_ASSERT(ntt_GLFW_Register());
#endif /* NTT_GRAPHICS_DRIVER_GLFW */

	return NTT_RESULT_SUCCESS;
}

ntt_Result ntt_Drivers_Unregister()
{
	// Destroy drivers here
#if NTT_GRAPHICS_DRIVER_GLFW
	NTT_SUCCESS_ASSERT(ntt_GLFW_Unregister());
#endif /* NTT_GRAPHICS_DRIVER_GLFW */

	NTT_DRIVER_INFO("Drivers unregistered.");

	return NTT_RESULT_SUCCESS;
}