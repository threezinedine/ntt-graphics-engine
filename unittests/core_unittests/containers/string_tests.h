#ifndef _STRING_TESTS_H_
#define _STRING_TESTS_H_

#include "engine/engine.h"
#include "tools/tools.h"
#include <string.h>

void string_tests_before_each()
{
	ntt_Result result = ntt_MemoryGlobalsInitialize();
	TEST_ASSERT(result == NTT_RESULT_SUCCESS);
}

void string_tests_after_each()
{
	ntt_Result result = ntt_MemoryGlobalsDestroy();
	TEST_ASSERT(result == NTT_RESULT_SUCCESS);
}

TEST_CASE(ShortString)
{
	ntt_StringResult helloWorld = ntt_StringFromCString("Hello, World!");
	TEST_ASSERT(helloWorld.result == NTT_RESULT_SUCCESS);
	TEST_ASSERT(helloWorld.data.length == 13);
	TEST_ASSERT(helloWorld.data.pLongBuffer == NULL);
}

TEST_CASE(LongString)
{
	const char* longString =
		"This is a long string that exceeds the short string optimization threshold. It should be stored in the "
		"pLongBuffer.";
	ntt_StringResult result = ntt_StringFromCString(longString);
	TEST_ASSERT(result.result == NTT_RESULT_SUCCESS);
	TEST_ASSERT(result.data.length == ntt_StrLen(longString));
	TEST_ASSERT(result.data.pLongBuffer != NULL);
	TEST_ASSERT(result.data.pBuffer[0] == '\0'); // The pBuffer should be empty for long strings

	ntt_Result destroyResult = ntt_StringDestroy(&result.data);
	TEST_ASSERT(destroyResult == NTT_RESULT_SUCCESS);
}

TEST_CASE(StringViewMemoryLeak)
{
	ntt_MemoryGlobalsInitialize();

	const char* longString =
		"This is a long string that exceeds the short string optimization threshold. It should be stored in the "
		"pLongBuffer.";

	ntt_StringFromCString(longString);

	ntt_Result destroyResult = ntt_MemoryGlobalsDestroy();
	TEST_ASSERT(destroyResult == NTT_RESULT_MEMORY_LEAK);
}

TEST_CASE(StringView)
{
	ntt_StringResult result = ntt_StringFromCString("Hello, World!");
	TEST_ASSERT(result.result == NTT_RESULT_SUCCESS);

	ntt_StringView stringView = ntt_stringToView(&result.data);
	TEST_ASSERT(stringView.pBuffer != NULL);
	TEST_ASSERT(stringView.length == 13);
	TEST_ASSERT(memcmp(stringView.pBuffer, "Hello, World!", 13) == 0);
}

TEST_CASE(StrLen)
{
	const char* testString = "Hello, World!";
	usize		length	   = ntt_StrLen(testString);
	TEST_ASSERT(length == 13);
}

TEST_SUITE_DEFINE(string,
				  string_tests_before_each,
				  string_tests_after_each,
				  TEST_CASE_DECLARE(ShortString),
				  TEST_CASE_DECLARE(LongString),
				  TEST_CASE_DECLARE(StringView),
				  TEST_CASE_DECLARE(StrLen),
				  TEST_CASE_DECLARE_WITHOUT_WRAPPER(StringViewMemoryLeak))

#endif /* _STRING_TESTS_H_ */