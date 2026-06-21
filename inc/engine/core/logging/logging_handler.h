#ifndef _LOGGING_HANDLER_H_
#define _LOGGING_HANDLER_H_

#include "engine/core/result.h"
#include "engine/core/types.h"
#include "logging_message.h"
#include "logging_type.h"

struct ntt_LoggingHandler
{
	ntt_LoggingLevel	   level;
	ntt_LoggingHandlerType type;

	ntt_Result (*initialize)(struct ntt_LoggingHandler* pHandler);
	ntt_Result (*handleMessage)(struct ntt_LoggingHandler* pHandler, ntt_LoggingMessage* pMessage);
	ntt_Result (*shutdown)(struct ntt_LoggingHandler* pHandler);
};

typedef struct ntt_LoggingHandler ntt_LoggingHandler;

#endif // _LOGGING_HANDLER_H_