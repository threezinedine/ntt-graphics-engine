#ifndef _LIST_TESTS_H_
#define _LIST_TESTS_H_

#include "engine/engine.h"
#include "tools/tools.h"

void list_tests_before_each()
{
	ntt_Result result = ntt_MemoryGlobalsInitialize();
	TEST_ASSERT(result == NTT_RESULT_SUCCESS);
}

void list_tests_after_each()
{
	ntt_Result result = ntt_MemoryGlobalsDestroy();
	TEST_ASSERT(result == NTT_RESULT_SUCCESS);
}

TEST_CASE(ListCreateAndDestroy)
{
	ntt_ListResult result = ntt_ListCreate(NULL);
	TEST_ASSERT(result.result == NTT_RESULT_SUCCESS);

	ntt_Result destroyResult = ntt_ListDestroy(&result.data);
	TEST_ASSERT(destroyResult == NTT_RESULT_SUCCESS);
}

TEST_SUITE_DEFINE(list, list_tests_before_each, list_tests_after_each, TEST_CASE_DECLARE(ListCreateAndDestroy))

#endif /* _LIST_TESTS_H_ */