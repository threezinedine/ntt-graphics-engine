#include "engine/core/result.h"

const char* ntt_ResultToString(ntt_Result result)
{
	switch (result)
	{
#define ENUM_OPTION(name)                                                                                              \
	case name:                                                                                                         \
		return #name;
#define ENUM_OPTION_END(name)                                                                                          \
	case name:                                                                                                         \
		return #name;
#include "engine/core/result.inl"
#undef ENUM_OPTION
#undef ENUM_OPTION_END
	default:
		return "Unknown Result";
	}
}