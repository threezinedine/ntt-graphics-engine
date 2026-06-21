#include "engine/systems/display/display_driver.h"

ntt_DisplayDriverPtrResult ntt_DisplayDriver_Register(ntt_DisplayDriverType driverType)
{
	switch (driverType)
	{
#define ENUM_OPTION(option)                                                                                            \
	case NTT_DISPLAY_DRIVER_##option: {                                                                                \
		extern ntt_DisplayDriver* g_##option##_DisplayDriver;                                                          \
		if (g_##option##_DisplayDriver == NULL)                                                                        \
		{                                                                                                              \
			return (ntt_DisplayDriverPtrResult){NTT_RESULT_GRAPHICS_DRIVER_NOT_SUPPORTED, NULL};                       \
		}                                                                                                              \
		return (ntt_DisplayDriverPtrResult){NTT_RESULT_SUCCESS, g_##option##_DisplayDriver};                           \
	}
#define ENUM_OPTION_END(option)                                                                                        \
	default: {                                                                                                         \
		return (ntt_DisplayDriverPtrResult){NTT_RESULT_GRAPHICS_DRIVER_NOT_FOUND, NULL};                               \
	}
#include "engine/systems/display/display_driver_type.inl"
#undef ENUM_OPTION
#undef ENUM_OPTION_END
	}
}