#include "engine/core/logging/logging_message.h"
#include "engine/core/containers/string.h"
#include "engine/core/utils/console.h"
#include <string.h>

typedef ntt_Result (*ReplacementFunction)(ntt_LoggingMessage* pMessage, u32* pIndex, void* pUserData);

struct KeyWordReplacement
{
	const char*			keyword;
	ReplacementFunction replacementFunction;
};

typedef struct KeyWordReplacement KeyWordReplacement;

struct ntt_LoggingMessageUserData
{
	char finalFile[128];
	char strLine[7];
};

typedef struct ntt_LoggingMessageUserData ntt_LoggingMessageUserData;

#define REPLACE_FUNCTION(name, getStr)                                                                                 \
	ntt_Result name(ntt_LoggingMessage* pMessage, u32* pIndex, void* pUserData)                                        \
	{                                                                                                                  \
		NTT_UNUSED(pUserData);                                                                                         \
		const char* str		   = getStr;                                                                               \
		usize		len		   = strlen(str);                                                                          \
		usize		bufferSize = (u32)sizeof(pMessage->finalMessage);                                                  \
                                                                                                                       \
		if (*pIndex + len >= bufferSize)                                                                               \
		{                                                                                                              \
			return NTT_RESULT_BUFFER_OVERFLOW;                                                                         \
		}                                                                                                              \
                                                                                                                       \
		strncpy(&pMessage->finalMessage[*pIndex], str, len);                                                           \
		*pIndex += (u32)len;                                                                                           \
		pMessage->finalMessage[*pIndex] = '\0';                                                                        \
                                                                                                                       \
		return NTT_RESULT_SUCCESS;                                                                                     \
	}

REPLACE_FUNCTION(ReplaceLevel, ntt_LoggingLevel_ToString(pMessage->level))
REPLACE_FUNCTION(ReplaceType, ntt_LoggingType_ToString(pMessage->type))
REPLACE_FUNCTION(ReplaceFile, ((ntt_LoggingMessageUserData*)pUserData)->finalFile)
REPLACE_FUNCTION(ReplaceLine, ((ntt_LoggingMessageUserData*)pUserData)->strLine)
REPLACE_FUNCTION(ReplaceMessage, pMessage->message)

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

	ntt_LoggingMessageUserData userData;
	ntt_FormatMessage(userData.strLine, sizeof(userData.strLine), "%u", pMessage->line);

	// Extract the file name from the full file path
	usize baseDirectoryLength = ntt_StrLen(STRINGIFY(NTT_GRAPHICS_ENGINE_DIRECTORY)) - 1;
	usize filePathLength	  = ntt_StrLen(pMessage->file);
	memcpy(userData.finalFile, pMessage->file + baseDirectoryLength, sizeof(userData.finalFile) - filePathLength - 1);
	userData.finalFile[sizeof(userData.finalFile) - 1] = '\0';

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
						u32 index = (u32)strlen(pMessage->finalMessage);
						NTT_SUCCESS_ASSERT(
							s_keyWordReplacements[i].replacementFunction(pMessage, &index, (void*)&userData));
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