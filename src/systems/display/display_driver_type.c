#include "engine/systems/display/display_driver_type.h"

const char* ntt_DisplayDriverType_ToString(ntt_DisplayDriverType type)
{
	switch (type)
	{
#define ENUM_OPTION(option)                                                                                            \
	case NTT_DISPLAY_DRIVER_##option:                                                                                  \
		return #option;
#define ENUM_OPTION_END(option)                                                                                        \
	default:                                                                                                           \
		return "Unknown Display Driver Type";
#include "engine/systems/display/display_driver_type.inl"
#undef ENUM_OPTION
#undef ENUM_OPTION_END
	}
}