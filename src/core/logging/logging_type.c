#include "engine/core/logging/logging_type.h"

const char* ntt_LoggingType_ToString(LoggingType type)
{
	switch (type)
	{
#define ENUM_OPTION(name)                                                                                              \
	case LOGGING_TYPE_##name:                                                                                          \
		return #name;
#define ENUM_OPTION_END()                                                                                              \
	default:                                                                                                           \
		return "UNKNOWN";
#include "engine/core/logging/logging_type.inl"
#undef ENUM_OPTION
#undef ENUM_OPTION_END
	}
}

const char* ntt_LoggingLevel_ToString(LoggingLevel level)
{
	switch (level)
	{
#define ENUM_OPTION(name)                                                                                              \
	case LOGGING_LEVEL_##name:                                                                                         \
		return #name;
#define ENUM_OPTION_END()                                                                                              \
	default:                                                                                                           \
		return "UNKNOWN";
#include "engine/core/logging/logging_level.inl"
#undef ENUM_OPTION
#undef ENUM_OPTION_END
	}
}