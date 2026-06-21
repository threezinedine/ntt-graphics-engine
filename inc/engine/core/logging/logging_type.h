#ifndef _LOGGING_TYPE_H_
#define _LOGGING_TYPE_H_

#include "engine/core/types.h"

enum ntt_LoggingType
{
#define ENUM_OPTION(name) LOGGING_TYPE_##name,
#define ENUM_OPTION_END() LOGGING_TYPE_COUNT
#include "logging_type.inl"
#undef ENUM_OPTION
#undef ENUM_OPTION_END
};

typedef enum ntt_LoggingType ntt_LoggingType;

const char* ntt_LoggingType_ToString(ntt_LoggingType type);

enum ntt_LoggingLevel
{
#define ENUM_OPTION(name) LOGGING_LEVEL_##name,
#define ENUM_OPTION_END() LOGGING_LEVEL_COUNT
#include "logging_level.inl"
#undef ENUM_OPTION
#undef ENUM_OPTION_END
};

typedef enum ntt_LoggingLevel ntt_LoggingLevel;

const char* ntt_LoggingLevel_ToString(ntt_LoggingLevel level);

enum ntt_LoggingHandlerType
{
#define ENUM_OPTION(name, value) LOGGING_HANDLER_TYPE_##name = value,
#define ENUM_OPTION_END()
#include "logging_handler_type.inl"
#undef ENUM_OPTION
#undef ENUM_OPTION_END
};

extern u32 LOGGING_HANDLER_TYPE_ALL;

typedef enum ntt_LoggingHandlerType ntt_LoggingHandlerType;

const char* ntt_LoggingHandlerType_ToString(ntt_LoggingHandlerType handlerType);

#endif // _LOGGING_TYPE_H_