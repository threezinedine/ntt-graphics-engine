#ifndef _STRING_VIEW_TESTS_H_
#define _STRING_VIEW_TESTS_H_

#include "engine/engine.h"
#include "tools/tools.h"

void string_view_tests_before_each()
{
	ntt_Result result = ntt_MemoryGlobalsInitialize();
	TEST_ASSERT(result == NTT_RESULT_SUCCESS);
}

void string_view_tests_after_each()
{
	ntt_Result result = ntt_MemoryGlobalsDestroy();
	TEST_ASSERT(result == NTT_RESULT_SUCCESS);
}

TEST_CASE(ShortString)
{
	ntt_StringViewResult helloWorld = ntt_StringViewFromCString("Hello, World!");
	TEST_ASSERT(helloWorld.result == NTT_RESULT_SUCCESS);
	TEST_ASSERT(helloWorld.data.length == 13);
	TEST_ASSERT(helloWorld.data.pLongBuffer == NULL);
}

TEST_CASE(LongString)
{
	const char* longString =
		"This is a long string that exceeds the short string optimization threshold. It should be stored in the "
		"pLongBuffer.";
	ntt_StringViewResult result = ntt_StringViewFromCString(longString);
	TEST_ASSERT(result.result == NTT_RESULT_SUCCESS);
	TEST_ASSERT(result.data.length == (usize)strlen(longString));
	TEST_ASSERT(result.data.pLongBuffer != NULL);
	TEST_ASSERT(result.data.pBuffer[0] == '\0'); // The pBuffer should be empty for long strings

	ntt_Result destroyResult = ntt_StringViewDestroy(&result.data);
	TEST_ASSERT(destroyResult == NTT_RESULT_SUCCESS);
}

TEST_CASE(StringViewMemoryLeak)
{
	ntt_MemoryGlobalsInitialize();

	const char* longString =
		"This is a long string that exceeds the short string optimization threshold. It should be stored in the "
		"pLongBuffer.";

	ntt_StringViewFromCString(longString);

	ntt_Result destroyResult = ntt_MemoryGlobalsDestroy();
	TEST_ASSERT(destroyResult == NTT_RESULT_MEMORY_LEAK);
}

TEST_SUITE_DEFINE(string_view,
				  string_view_tests_before_each,
				  string_view_tests_after_each,
				  TEST_CASE_DECLARE(ShortString),
				  TEST_CASE_DECLARE(LongString),
				  TEST_CASE_DECLARE_WITHOUT_WRAPPER(StringViewMemoryLeak))

#endif /* _STRING_VIEW_TESTS_H_ */