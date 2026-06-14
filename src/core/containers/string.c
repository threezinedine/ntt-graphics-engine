#include "engine/core/containers/string.h"
#include "engine/core/memory/memory.h"
#include "engine/core/result.h"
#include <string.h>

ntt_StringResult ntt_StringFromCString(const char* cString)
{
	NTT_ASSERT_M(cString != NULL, "cString cannot be NULL");
	if (cString == NULL)
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
		result.data.pLongBuffer = NULL;
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
	}

	return result;
}

usize ntt_StringLength(const ntt_String* stringView)
{
	return stringView->length;
}

ntt_Result ntt_StringDestroy(ntt_String* stringView)
{
	if (stringView->length >= NTT_SHORT_STRING_MAX_LENGTH)
	{
		NTT_ASSERT_M(stringView->pLongBuffer != NULL,
					 "pLongBuffer cannot be NULL when length >= NTT_SHORT_STRING_MAX_LENGTH");
		if (stringView->pLongBuffer == NULL)
		{
			return NTT_RESULT_INVALID_POINTER;
		}

		ntt_Result deallocateResult = g_memoryGlobals.mallocAllocator->deallocate(
			g_memoryGlobals.mallocAllocator, stringView->pLongBuffer, stringView->length + 1);
		NTT_SUCCESS_ASSERT(deallocateResult);
	}

	return NTT_RESULT_SUCCESS;
}