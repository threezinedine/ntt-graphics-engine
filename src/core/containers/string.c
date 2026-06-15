#include "engine/core/containers/string.h"
#include "engine/core/memory/memory.h"
#include "engine/core/result.h"
#include "engine/core/utils/console.h"
#include <stdarg.h>
#include <string.h>

ntt_StringResult ntt_StringFromCString(const char* cString)
{
	NTT_ASSERT_IF(cString == NULL)
	{
		ntt_StringResult result = {.result = NTT_RESULT_NULL_POINTER};
		return result;
	}

	ntt_StringResult result;
	result.result = NTT_RESULT_SUCCESS;

	usize length	   = (usize)strlen(cString);
	result.data.length = length;

	if (length < NTT_SHORT_STRING_MAX_LENGTH)
	{
		memcpy(result.data.pBuffer, cString, length + 1);
		result.data.isShortString = TRUE;
		result.data.pLongBuffer	  = NULL;
	}
	else
	{
		voidPtrResult allocateResult =
			g_memoryGlobals.mallocAllocator->allocate(g_memoryGlobals.mallocAllocator, length + 1);
		if (allocateResult.result != NTT_RESULT_SUCCESS)
		{
			result.result = allocateResult.result;
			return result;
		}

		result.data.pLongBuffer = (char*)allocateResult.pData;
		memcpy(result.data.pLongBuffer, cString, length + 1);
		memset(result.data.pBuffer, 0, NTT_SHORT_STRING_MAX_LENGTH); // Clear the pBuffer for long strings
		result.data.length		  = length;
		result.data.isShortString = FALSE;
	}

	return result;
}

ntt_StringResult ntt_StringFromFormat(const char* format, ...)
{
	char	formattedString[1024] = {0};
	va_list args;
	va_start(args, format);
	ntt_FormatMessage(formattedString, sizeof(formattedString), format, args);
	va_end(args);

	return ntt_StringFromCString(formattedString);
}

usize ntt_StringLength(const ntt_String* stringView)
{
	return stringView->length;
}

ntt_Result ntt_StringDestroy(ntt_String* stringView)
{
	if (stringView->length >= NTT_SHORT_STRING_MAX_LENGTH)
	{
		NTT_ASSERT_IF(stringView->pLongBuffer == NULL)
		{
			return NTT_RESULT_INVALID_POINTER;
		}

		ntt_Result deallocateResult = g_memoryGlobals.mallocAllocator->deallocate(
			g_memoryGlobals.mallocAllocator, stringView->pLongBuffer, stringView->length + 1);
		NTT_SUCCESS_ASSERT(deallocateResult);
	}

	return NTT_RESULT_SUCCESS;
}

ntt_StringView ntt_stringToView(ntt_String* pStr)
{
	return (ntt_StringView){
		.pBuffer = pStr->isShortString ? pStr->pBuffer : pStr->pLongBuffer,
		.length	 = pStr->length,
	};
}

usize ntt_StrLen(const char* cString)
{
	return (usize)strlen(cString);
}