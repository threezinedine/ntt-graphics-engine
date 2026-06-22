#include "engine/core/logging/logging_message.h"
#include "engine/core/containers/string.h"
#include "engine/core/utils/console.h"
#include <string.h>

struct KeyWordReplacement;

typedef ntt_Result (*ReplacementFunction)(ntt_LoggingMessage*		 pMessage,
										  u32*						 pIndex,
										  struct KeyWordReplacement* pReplacement,
										  void*						 pUserData);

struct KeyWordReplacement
{
	const char* keyword;
	i32 keywordLength; /// -1 -> no precomputed length, otherwise the length of the keyword string. This is used to
					   /// optimize the search for keywords in the format string.
	b8 leftAlign;	   /// Whether to left align the replacement string in the final message. If false, it will be right
					   /// aligned. This is used when the keyword is shorter than the precomputed keyword length.
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
	ntt_Result name(ntt_LoggingMessage* pMessage, u32* pIndex, KeyWordReplacement* pReplacement, void* pUserData)      \
	{                                                                                                                  \
		NTT_UNUSED(pUserData);                                                                                         \
		i32			keywordLength = pReplacement->keywordLength;                                                       \
		const char* str			  = getStr;                                                                            \
		char		buffer[128]	  = {0};                                                                               \
		memcpy(buffer, str, strlen(str) + 1);                                                                          \
		if (keywordLength > 0)                                                                                         \
		{                                                                                                              \
			if ((i32)strlen(buffer) < keywordLength)                                                                   \
			{                                                                                                          \
				if (pReplacement->leftAlign)                                                                           \
				{                                                                                                      \
					while (strlen(buffer) < (size_t)keywordLength)                                                     \
					{                                                                                                  \
						char temp[128] = {0};                                                                          \
						memcpy(temp, buffer, strlen(buffer) + 1);                                                      \
						ntt_FormatMessage(buffer, sizeof(buffer), "%s ", temp);                                        \
					}                                                                                                  \
				}                                                                                                      \
				else                                                                                                   \
				{                                                                                                      \
					while (strlen(buffer) < (size_t)keywordLength)                                                     \
					{                                                                                                  \
						char temp[128] = {0};                                                                          \
						memcpy(temp, buffer, strlen(buffer) + 1);                                                      \
						ntt_FormatMessage(buffer, sizeof(buffer), " %s", temp);                                        \
					}                                                                                                  \
				}                                                                                                      \
			}                                                                                                          \
			else                                                                                                       \
			{                                                                                                          \
				buffer[keywordLength] = '\0';                                                                          \
			}                                                                                                          \
		}                                                                                                              \
                                                                                                                       \
		usize len		 = (usize)strlen(buffer);                                                                      \
		usize bufferSize = (usize)sizeof(pMessage->finalMessage);                                                      \
		if (*pIndex + (u32)len >= (u32)bufferSize)                                                                     \
		{                                                                                                              \
			return NTT_RESULT_BUFFER_OVERFLOW;                                                                         \
		}                                                                                                              \
                                                                                                                       \
		memcpy(&pMessage->finalMessage[*pIndex], buffer, strlen(buffer) + 1);                                          \
		*pIndex += len;                                                                                                \
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
	{"%(level)"  , 7 , TRUE , ReplaceLevel  },
	{"%(type)"   , 8 , TRUE , ReplaceType   },
	{"%(file)"   , 40, FALSE, ReplaceFile   },
	{"%(line)"   , 4 , TRUE , ReplaceLine   },
	{"%(message)", -1, TRUE , ReplaceMessage},
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
						NTT_SUCCESS_ASSERT(s_keyWordReplacements[i].replacementFunction(
							pMessage, &index, &s_keyWordReplacements[i], (void*)&userData));
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
			usize len		 = 1;
			usize bufferSize = sizeof(pMessage->finalMessage);
			if ((usize)strlen(pMessage->finalMessage) + len >= bufferSize)
			{
				return NTT_RESULT_BUFFER_OVERFLOW;
			}
			memcpy(&pMessage->finalMessage[strlen(pMessage->finalMessage)], &pFormat[formatCharIndex], len);
			formatCharIndex++;
		}
	}

	return NTT_RESULT_SUCCESS;
}