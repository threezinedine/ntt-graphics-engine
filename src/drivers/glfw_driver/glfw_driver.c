#include "engine/drivers/glfw_driver/glfw_driver.h"
#include "engine/core/types.h"
#include <stddef.h>

ntt_DisplayDriver* g_GLFW_DisplayDriver = NULL;

#if NTT_GRAPHICS_DRIVER_GLFW

ntt_Result ntt_GLFW_Register()
{
	return NTT_RESULT_SUCCESS;
}

ntt_Result ntt_GLFW_Unregister()
{
	return NTT_RESULT_SUCCESS;
}

#endif /* NTT_GRAPHICS_DRIVER_GLFW */