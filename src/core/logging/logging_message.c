#include "engine/core/logging/logging_message.h"
#include "engine/core/containers/string.h"
#include "engine/core/utils/console.h"
#include <string.h>

typedef ntt_Result (*ReplacementFunction)(ntt_LoggingMessage* pMessage, char* pBuffer, u32* pIndex, u32 bufferSize);

struct KeyWordReplacement
{
	const char*			keyword;
	ReplacementFunction replacementFunction;
};

typedef struct KeyWordReplacement KeyWordReplacement;

#define REPLACE_FUNCTION(name, getStr)                                                                                 \
	ntt_Result name(ntt_LoggingMessage* pMessage, char* pBuffer, u32* pIndex, u32 bufferSize)                          \
	{                                                                                                                  \
		const char* str = getStr;                                                                                      \
		usize		len = strlen(str);                                                                                 \
                                                                                                                       \
		if (*pIndex + len >= bufferSize)                                                                               \
		{                                                                                                              \
			return NTT_RESULT_BUFFER_OVERFLOW;                                                                         \
		}                                                                                                              \
                                                                                                                       \
		strncpy(&pBuffer[*pIndex], str, len);                                                                          \
		*pIndex += (u32)len;                                                                                           \
                                                                                                                       \
		return NTT_RESULT_SUCCESS;                                                                                     \
	}

REPLACE_FUNCTION(ReplaceLevel, ntt_LoggingLevel_ToString(pMessage->level))
REPLACE_FUNCTION(ReplaceType, ntt_LoggingType_ToString(pMessage->type))
REPLACE_FUNCTION(ReplaceFile, pMessage->file)
REPLACE_FUNCTION(ReplaceMessage, pMessage->message)

ntt_Result ReplaceLine(ntt_LoggingMessage* pMessage, char* pBuffer, u32* pIndex, u32 bufferSize)
{
	char lineBuffer[7];

	// Format the line number as a string
	ntt_FormatMessage(lineBuffer, sizeof(lineBuffer), "%u", pMessage->line);

	if (*pIndex + (i32)ntt_StrLen(lineBuffer) >= bufferSize)
	{
		return NTT_RESULT_BUFFER_OVERFLOW;
	}

	strncpy(&pBuffer[*pIndex], lineBuffer, ntt_StrLen(lineBuffer));
	*pIndex += (u32)ntt_StrLen(lineBuffer);
	return NTT_RESULT_SUCCESS;
}

static KeyWordReplacement s_keyWordReplacements[] = {
	// clang-format off
	{"%(level)",   ReplaceLevel},
	{"%(type)",	   ReplaceType},
	{"%(file)",	   ReplaceFile},
	{"%(line)",	   ReplaceLine},
	{"%(message)", ReplaceMessage},
	// clang-format on
};

ntt_Result ntt_LoggingMessage_FormatMessage(ntt_LoggingMessage* pMessage, const char* pFormat)
{
	u32 keyWordCount	= (u32)(sizeof(s_keyWordReplacements) / sizeof(KeyWordReplacement));
	i32 formatCharIndex = 0;

	while (formatCharIndex < (i32)strlen(pFormat))
	{
		if (pFormat[formatCharIndex] == '%')
		{
			for (u32 i = 0; i < keyWordCount; ++i)
			{
				if (strncmp(&pFormat[formatCharIndex],
							s_keyWordReplacements[i].keyword,
							strlen(s_keyWordReplacements[i].keyword)) == 0)
				{
					// Call the replacement function for the matched keyword
					if (s_keyWordReplacements[i].replacementFunction != NULL)
					{
						u32 bufferSize = sizeof(pMessage->finalMessage) - strlen(pMessage->finalMessage) - 1;
						u32 index	   = (u32)strlen(pMessage->finalMessage);
						NTT_SUCCESS_ASSERT(s_keyWordReplacements[i].replacementFunction(
							pMessage, pMessage->finalMessage, &index, bufferSize));
					}

					// Move the formatCharIndex past the matched keyword
					formatCharIndex += (i32)strlen(s_keyWordReplacements[i].keyword);
					break;
				}
			}
		}
		else
		{
			// Copy the character to the final message
			strncat(pMessage->finalMessage, &pFormat[formatCharIndex], 1);
			formatCharIndex++;
		}
	}

	return NTT_RESULT_SUCCESS;
}