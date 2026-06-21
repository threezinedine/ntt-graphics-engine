#include "engine/core/logging/logging_type.h"

u32 LOGGING_HANDLER_TYPE_ALL =
#define ENUM_OPTION(name, value) value |
#define ENUM_OPTION_END()		 0
#include "engine/core/logging/logging_handler_type.inl"
#undef ENUM_OPTION
#undef ENUM_OPTION_END
	;

const char* ntt_LoggingType_ToString(ntt_LoggingType type)
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

const char* ntt_LoggingLevel_ToString(ntt_LoggingLevel level)
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

const char* ntt_LoggingHandlerType_ToString(ntt_LoggingHandlerType handlerType)
{
	switch (handlerType)
	{
#define ENUM_OPTION(name, value)                                                                                       \
	case LOGGING_HANDLER_TYPE_##name:                                                                                  \
		return #name;
#define ENUM_OPTION_END()                                                                                              \
	default:                                                                                                           \
		return "UNKNOWN";
#include "engine/core/logging/logging_handler_type.inl"
#undef ENUM_OPTION
#undef ENUM_OPTION_END
	}
}