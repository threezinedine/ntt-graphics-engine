#ifndef _LOGGING_MESSAGE_H_
#define _LOGGING_MESSAGE_H_

#include "engine/core/result.h"
#include "engine/core/types.h"
#include "engine/core/utils/time.h"
#include "logging_type.h"

/**
 * Store all the information of the message which is logged, the handlers will receive this
 *      struct and will be able to process it.
 */
struct ntt_LoggingMessage
{
	ntt_LoggingType	 type;
	ntt_LoggingLevel level;
	char			 file[512];
	u32				 line;
	ntt_Time		 timestamp;
	char			 message[512];
	char			 finalMessage[1024]; /// The output message that is going to be printed.
};

typedef struct ntt_LoggingMessage ntt_LoggingMessage;

/**
 * Format the message with the given format and store it in the ntt_LoggingMessage struct.
 *
 * @param pMessage The ntt_LoggingMessage struct where the formatted message will be stored.
 * @param pFormat The format string for the message. The format should be:
 * 		"[%(level)] [%(type)] %(file):%(line)] %(message)"
 * 		whereas:
 * 			- %(level) is the logging level (e.g., INFO, WARNING, ERROR)
 * 			- %(type) is the logging type (e.g., SYSTEM, NETWORK, GRAPHICS)
 * 			- %(file) is the source file name where the log was generated
 * 			- %(line) is the line number in the source file where the log was generated
 * 			- %(message) is the actual log message
 */
ntt_Result ntt_LoggingMessage_FormatMessage(ntt_LoggingMessage* pMessage, const char* pFormat);

#endif // _LOGGING_MESSAGE_H_