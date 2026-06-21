#ifndef _LOGGING_H_
#define _LOGGING_H_

#include "engine/core/utils/console.h"
#include "logging_handler.h"
#include "logging_message.h"
#include "logging_type.h"

ntt_Result ntt_Logging_Initialize(u32 loggingTypes, const char* format);

ntt_Result
ntt_Logging_LogMessage(ntt_LoggingType type, ntt_LoggingLevel level, const char* message, const char* file, u32 line);

ntt_Result ntt_Logging_SetLevel(ntt_LoggingHandlerType handlerType, ntt_LoggingLevel level);

ntt_Result ntt_Logging_Shutdown();

#define NTT_LOG(type, level, message, ...)                                                                             \
	do                                                                                                                 \
	{                                                                                                                  \
		char _loggingbuffer[512];                                                                                      \
		ntt_FormatMessage(_loggingbuffer, sizeof(_loggingbuffer), message, ##__VA_ARGS__);                             \
		ntt_Logging_LogMessage(type, level, _loggingbuffer, __FILE__, __LINE__);                                       \
	} while (0)

#define NTT_LOG_TRACE(type, message, ...) NTT_LOG(type, LOGGING_LEVEL_TRACE, message, ##__VA_ARGS__)
#define NTT_LOG_DEBUG(type, message, ...) NTT_LOG(type, LOGGING_LEVEL_DEBUG, message, ##__VA_ARGS__)
#define NTT_LOG_INFO(type, message, ...)  NTT_LOG(type, LOGGING_LEVEL_INFO, message, ##__VA_ARGS__)
#define NTT_LOG_WARN(type, message, ...)  NTT_LOG(type, LOGGING_LEVEL_WARN, message, ##__VA_ARGS__)
#define NTT_LOG_ERROR(type, message, ...) NTT_LOG(type, LOGGING_LEVEL_ERROR, message, ##__VA_ARGS__)
#define NTT_LOG_FATAL(type, message, ...) NTT_LOG(type, LOGGING_LEVEL_FATAL, message, ##__VA_ARGS__)

#define NTT_CORE_TRACE(message, ...) NTT_LOG_TRACE(LOGGING_TYPE_CORE, message, ##__VA_ARGS__)
#define NTT_CORE_DEBUG(message, ...) NTT_LOG_DEBUG(LOGGING_TYPE_CORE, message, ##__VA_ARGS__)
#define NTT_CORE_INFO(message, ...)	 NTT_LOG_INFO(LOGGING_TYPE_CORE, message, ##__VA_ARGS__)
#define NTT_CORE_WARN(message, ...)	 NTT_LOG_WARN(LOGGING_TYPE_CORE, message, ##__VA_ARGS__)
#define NTT_CORE_ERROR(message, ...) NTT_LOG_ERROR(LOGGING_TYPE_CORE, message, ##__VA_ARGS__)
#define NTT_CORE_FATAL(message, ...) NTT_LOG_FATAL(LOGGING_TYPE_CORE, message, ##__VA_ARGS__)

#endif // _LOGGING_H_