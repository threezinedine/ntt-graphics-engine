#ifndef _ARRAY_TESTS_H_
#define _ARRAY_TESTS_H_

#include "engine/engine.h"
#include "tools/tools.h"

void array_tests_before_each()
{
	ntt_Result result = ntt_MemoryGlobalsInitialize();
	TEST_ASSERT(result == NTT_RESULT_SUCCESS);
}

void array_tests_after_each()
{
	ntt_Result result = ntt_MemoryGlobalsDestroy();
	TEST_ASSERT(result == NTT_RESULT_SUCCESS);
}

TEST_CASE(CreateArray)
{
	ntt_ArrayResult result = ntt_ArrayCreate(sizeof(i32), 4, NULL);
	TEST_ASSERT(result.result == NTT_RESULT_SUCCESS);

	ntt_ArrayDestroy(&result.data);
}

TEST_SUITE_DEFINE(array, array_tests_before_each, array_tests_after_each, TEST_CASE_DECLARE(CreateArray))

#endif /* _ARRAY_TESTS_H_ */