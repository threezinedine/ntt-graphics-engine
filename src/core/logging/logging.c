#include "engine/core/logging/logging.h"
#include "engine/core/containers/list.h"
#include "engine/core/utils/time.h"
#include <string.h>

static ntt_List g_loggingHandlers;
static char		g_loggingFormat[512];

static ntt_Result ntt_LoggingHandler_NONE_Initialize(struct ntt_LoggingHandler* pHandler);
static ntt_Result ntt_LoggingHandler_NONE_HandleMessage(struct ntt_LoggingHandler* pHandler,
														ntt_LoggingMessage*		   pMessage);
static ntt_Result ntt_LoggingHandler_NONE_Shutdown(struct ntt_LoggingHandler* pHandler);

static ntt_Result ntt_LoggingHandler_CONSOLE_Initialize(struct ntt_LoggingHandler* pHandler);
static ntt_Result ntt_LoggingHandler_CONSOLE_HandleMessage(struct ntt_LoggingHandler* pHandler,
														   ntt_LoggingMessage*		  pMessage);
static ntt_Result ntt_LoggingHandler_CONSOLE_Shutdown(struct ntt_LoggingHandler* pHandler);

static ntt_Result ntt_LoggingHandler_FILE_Initialize(struct ntt_LoggingHandler* pHandler);
static ntt_Result ntt_LoggingHandler_FILE_HandleMessage(struct ntt_LoggingHandler* pHandler,
														ntt_LoggingMessage*		   pMessage);
static ntt_Result ntt_LoggingHandler_FILE_Shutdown(struct ntt_LoggingHandler* pHandler);

ntt_Result ntt_Logging_Initialize(u32 loggingTypes, const char* format)
{
	if (format)
	{
		memcpy(g_loggingFormat, format, sizeof(g_loggingFormat));
	}
	else
	{
		ntt_FormatMessage(g_loggingFormat, sizeof(g_loggingFormat), "[%(type)] %(file):%(line) %(message)");
	}

	ntt_ListResult result = ntt_ListCreate(NULL, NULL);
	NTT_SUCCESS_ASSERT_VAR(result);
	g_loggingHandlers = result.data;

	// checking all types
#define ENUM_OPTION(name, value)                                                                                       \
	if (loggingTypes & LOGGING_HANDLER_TYPE_##name)                                                                    \
	{                                                                                                                  \
		ntt_LoggingHandler handler = {LOGGING_LEVEL_DEBUG,                                                             \
									  LOGGING_HANDLER_TYPE_##name,                                                     \
									  ntt_LoggingHandler_##name##_Initialize,                                          \
									  ntt_LoggingHandler_##name##_HandleMessage,                                       \
									  ntt_LoggingHandler_##name##_Shutdown};                                           \
		ntt_Result		   res	   = ntt_ListAppend(&g_loggingHandlers, &handler, sizeof(handler));                    \
		NTT_SUCCESS_ASSERT(res);                                                                                       \
	}
#define ENUM_OPTION_END()
#include "engine/core/logging/logging_handler_type.inl"
#undef ENUM_OPTION
#undef ENUM_OPTION_END

	ntt_ListNode* pCurrentHandlerNode = g_loggingHandlers.pHead;

	while (pCurrentHandlerNode)
	{
		ntt_LoggingHandler* pHandler = (ntt_LoggingHandler*)pCurrentHandlerNode->pData;
		if (pHandler->initialize)
		{
			NTT_SUCCESS_ASSERT(pHandler->initialize(pHandler));
		}
		pCurrentHandlerNode = pCurrentHandlerNode->pNext;
	}

	return NTT_RESULT_SUCCESS;
}

ntt_Result ntt_Logging_SetLevel(ntt_LoggingHandlerType handlerType, ntt_LoggingLevel level)
{
	ntt_ListNode* pCurrentHandlerNode = g_loggingHandlers.pHead;

	while (pCurrentHandlerNode)
	{
		ntt_LoggingHandler* pHandler = (ntt_LoggingHandler*)pCurrentHandlerNode->pData;

		if (pHandler->type == handlerType)
		{
			pHandler->level = level;
			return NTT_RESULT_SUCCESS;
		}

		pCurrentHandlerNode = pCurrentHandlerNode->pNext;
	}

	return NTT_RESULT_LOG_HANDLER_NOT_FOUND;
}

ntt_Result
ntt_Logging_LogMessage(ntt_LoggingType type, ntt_LoggingLevel level, const char* message, const char* file, u32 line)
{
	ntt_LoggingMessage logMessage;
	logMessage.type	 = type;
	logMessage.level = level;
	memcpy(logMessage.file, file, strlen(file) + 1);
	logMessage.line = line;
	memcpy(logMessage.message, message, strlen(message) + 1);
	memset(logMessage.finalMessage, 0, sizeof(logMessage.finalMessage));
	NTT_SUCCESS_ASSERT(ntt_GetCurrentTime(&logMessage.timestamp));

	if (g_loggingFormat[0] != '\0')
	{
		NTT_SUCCESS_ASSERT(ntt_LoggingMessage_FormatMessage(&logMessage, g_loggingFormat));
	}
	else
	{
		// Default format: "[%(level)] [%(type)] %(file):%(line)] %(message)"
		NTT_SUCCESS_ASSERT(ntt_LoggingMessage_FormatMessage(&logMessage, "[%(type)] %(file):%(line) %(message)"));
	}

	ntt_ListNode* pCurrentHandlerNode = g_loggingHandlers.pHead;

	while (pCurrentHandlerNode)
	{
		ntt_LoggingHandler* pHandler = (ntt_LoggingHandler*)pCurrentHandlerNode->pData;
		if (pHandler->handleMessage)
		{
			NTT_SUCCESS_ASSERT(pHandler->handleMessage(pHandler, &logMessage));
		}
		pCurrentHandlerNode = pCurrentHandlerNode->pNext;
	}

	return NTT_RESULT_SUCCESS;
}

ntt_Result ntt_Logging_Shutdown()
{
	ntt_ListNode* pCurrentHandlerNode = g_loggingHandlers.pHead;

	while (pCurrentHandlerNode)
	{
		ntt_LoggingHandler* pHandler = (ntt_LoggingHandler*)pCurrentHandlerNode->pData;
		if (pHandler->shutdown)
		{
			NTT_SUCCESS_ASSERT(pHandler->shutdown(pHandler));
		}
		pCurrentHandlerNode = pCurrentHandlerNode->pNext;
	}

	NTT_SUCCESS_ASSERT(ntt_ListClear(&g_loggingHandlers));

	return NTT_RESULT_SUCCESS;
}

static ntt_Result ntt_LoggingHandler_NONE_Initialize(ntt_LoggingHandler* pHandler)
{
	NTT_UNUSED(pHandler);
	return NTT_RESULT_SUCCESS;
}

static ntt_Result ntt_LoggingHandler_NONE_HandleMessage(ntt_LoggingHandler* pHandler, ntt_LoggingMessage* pMessage)
{
	NTT_UNUSED(pHandler);
	NTT_UNUSED(pMessage);
	return NTT_RESULT_SUCCESS;
}

static ntt_Result ntt_LoggingHandler_NONE_Shutdown(ntt_LoggingHandler* pHandler)
{
	NTT_UNUSED(pHandler);
	return NTT_RESULT_SUCCESS;
}

static ntt_Result ntt_LoggingHandler_CONSOLE_Initialize(ntt_LoggingHandler* pHandler)
{
	NTT_UNUSED(pHandler);
	return NTT_RESULT_SUCCESS;
}

static ntt_Result ntt_LoggingHandler_CONSOLE_HandleMessage(ntt_LoggingHandler* pHandler, ntt_LoggingMessage* pMessage)
{
	if (pMessage->level < pHandler->level)
	{
		return NTT_RESULT_SUCCESS;
	}

	switch (pMessage->level)
	{
	case LOGGING_LEVEL_TRACE:
		ntt_ConsoleSetColor(NTT_COLOR_GRAY);
		break;
	case LOGGING_LEVEL_DEBUG:
		ntt_ConsoleSetColor(NTT_COLOR_CYAN);
		break;
	case LOGGING_LEVEL_INFO:
		ntt_ConsoleSetColor(NTT_COLOR_GREEN);
		break;
	case LOGGING_LEVEL_WARN:
		ntt_ConsoleSetColor(NTT_COLOR_YELLOW);
		break;
	case LOGGING_LEVEL_ERROR:
		ntt_ConsoleSetColor(NTT_COLOR_RED);
		break;
	case LOGGING_LEVEL_FATAL:
		ntt_ConsoleSetColor(NTT_COLOR_MAGENTA);
		break;
	default:
		NTT_UNREACHABLE();
		return NTT_RESULT_REACHING_UNREACHABLE_CODE;
	}

	ntt_ConsolePrint("%s\n", pMessage->finalMessage);
	ntt_ConsoleResetColor();

	return NTT_RESULT_SUCCESS;
}

static ntt_Result ntt_LoggingHandler_CONSOLE_Shutdown(ntt_LoggingHandler* pHandler)
{
	NTT_UNUSED(pHandler);
	return NTT_RESULT_SUCCESS;
}

static ntt_Result ntt_LoggingHandler_FILE_Initialize(ntt_LoggingHandler* pHandler)
{
	NTT_UNUSED(pHandler);
	return NTT_RESULT_SUCCESS;
}

static ntt_Result ntt_LoggingHandler_FILE_HandleMessage(ntt_LoggingHandler* pHandler, ntt_LoggingMessage* pMessage)
{
	NTT_UNUSED(pHandler);
	NTT_UNUSED(pMessage);
	return NTT_RESULT_SUCCESS;
}

static ntt_Result ntt_LoggingHandler_FILE_Shutdown(ntt_LoggingHandler* pHandler)
{
	NTT_UNUSED(pHandler);
	return NTT_RESULT_SUCCESS;
}