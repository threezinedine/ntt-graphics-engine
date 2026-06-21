#include "engine/drivers/drivers.h"
#include "engine/core/logging/logging.h"

ntt_DisplayDriver* g_DisplayDriver = NULL;

ntt_Result ntt_Drivers_Register()
{
	NTT_DRIVER_INFO("Registering drivers...");

	// Initialize drivers here
#if NTT_GRAPHICS_DRIVER_GLFW
	NTT_SUCCESS_ASSERT(ntt_GLFW_Register());
	g_DisplayDriver = g_GLFW_DisplayDriver;
	NTT_ASSERT_IF(g_DisplayDriver == NULL)
	{
		return NTT_RESULT_NULL_POINTER;
	}
#endif /* NTT_GRAPHICS_DRIVER_GLFW */

	return NTT_RESULT_SUCCESS;
}

ntt_Result ntt_Drivers_Unregister()
{
	// Destroy drivers here
#if NTT_GRAPHICS_DRIVER_GLFW
	NTT_SUCCESS_ASSERT(ntt_GLFW_Unregister());
	g_DisplayDriver = NULL;
#endif /* NTT_GRAPHICS_DRIVER_GLFW */

	NTT_DRIVER_INFO("Drivers unregistered.");

	return NTT_RESULT_SUCCESS;
}

ntt_Result ntt_Drivers_Initialize()
{
	NTT_DRIVER_INFO("Initializing drivers...");

	// Initialize drivers here
#if NTT_GRAPHICS_DRIVER_GLFW
	if (g_GLFW_DisplayDriver == NULL)
	{
		return NTT_RESULT_NULL_POINTER;
	}

	if (g_GLFW_DisplayDriver->Initialize == NULL)
	{
		return NTT_RESULT_NULL_POINTER;
	}

	NTT_SUCCESS_ASSERT(g_GLFW_DisplayDriver->Initialize());
#endif /* NTT_GRAPHICS_DRIVER_GLFW */

	NTT_DRIVER_INFO("Drivers initialized.");

	return NTT_RESULT_SUCCESS;
}

ntt_Result ntt_Drivers_Destroy()
{
	NTT_DRIVER_INFO("Destroying drivers...");

	// Destroy drivers here
#if NTT_GRAPHICS_DRIVER_GLFW
	if (g_GLFW_DisplayDriver == NULL)
	{
		return NTT_RESULT_NULL_POINTER;
	}

	if (g_GLFW_DisplayDriver->Shutdown == NULL)
	{
		return NTT_RESULT_NULL_POINTER;
	}

	NTT_SUCCESS_ASSERT(g_GLFW_DisplayDriver->Shutdown());
#endif /* NTT_GRAPHICS_DRIVER_GLFW */
	NTT_DRIVER_INFO("Drivers destroyed.");

	return NTT_RESULT_SUCCESS;
}