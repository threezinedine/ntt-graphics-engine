#ifndef _LOGGING_TYPE_H_
#define _LOGGING_TYPE_H_

enum LoggingType
{
#define ENUM_OPTION(name) LOGGING_TYPE_##name,
#define ENUM_OPTION_END() LOGGING_TYPE_COUNT
#include "logging_type.inl"
#undef ENUM_OPTION
#undef ENUM_OPTION_END
};

typedef enum LoggingType LoggingType;

const char* ntt_LoggingType_ToString(LoggingType type);

enum LoggingLevel
{
#define ENUM_OPTION(name) LOGGING_LEVEL_##name,
#define ENUM_OPTION_END() LOGGING_LEVEL_COUNT
#include "logging_level.inl"
#undef ENUM_OPTION
#undef ENUM_OPTION_END
};

typedef enum LoggingLevel LoggingLevel;

const char* ntt_LoggingLevel_ToString(LoggingLevel level);

#endif // _LOGGING_TYPE_H_