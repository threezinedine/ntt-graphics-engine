#include "engine/core/color.h"

const char* ntt_ColorToString(enum ntt_Color color)
{
	switch (color)
	{
#define ENUM_OPTION(option)                                                                                            \
	case option:                                                                                                       \
		return #option;
#define ENUM_OPTION_END(name)                                                                                          \
	default:                                                                                                           \
		return "Unknown";
#include "engine/core/color.inl"
#undef ENUM_OPTION
#undef ENUM_OPTION_END
	}
}