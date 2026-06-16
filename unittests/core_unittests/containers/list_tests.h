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

TEST_CASE(ListDestroyMissingAllocator)
{
	TEST_ASSERT(ntt_MemoryGlobalsInitialize() == NTT_RESULT_SUCCESS);

	ntt_ListResult result = ntt_ListCreate(NULL);
	TEST_ASSERT(result.result == NTT_RESULT_SUCCESS);

	int i = 42;

	ntt_Result appendResult = ntt_ListAppend(&result.data, &i, sizeof(i));
	TEST_ASSERT(appendResult == NTT_RESULT_SUCCESS);

	result.data.pAllocator = NULL;

	ntt_Result destroyResult = ntt_ListDestroy(&result.data);
	TEST_ASSERT(destroyResult == NTT_RESULT_MISSING_ALLOCATOR);

	ntt_Result destroyGlobalsResult = ntt_MemoryGlobalsDestroy();
	TEST_ASSERT(destroyGlobalsResult == NTT_RESULT_MEMORY_LEAK);
}

TEST_CASE(ListAppendAndClear)
{
	ntt_ListResult result = ntt_ListCreate(NULL);
	TEST_ASSERT(result.result == NTT_RESULT_SUCCESS);

	int i = 42;
	int j = 84;

	ntt_Result appendResult = ntt_ListAppend(&result.data, &i, sizeof(i));
	TEST_ASSERT(appendResult == NTT_RESULT_SUCCESS);
	TEST_ASSERT(result.data.length == 1);

	appendResult = ntt_ListAppend(&result.data, &j, sizeof(j));
	TEST_ASSERT(appendResult == NTT_RESULT_SUCCESS);
	TEST_ASSERT(result.data.length == 2);

	ntt_Result clearResult = ntt_ListClear(&result.data);
	TEST_ASSERT(clearResult == NTT_RESULT_SUCCESS);
	TEST_ASSERT(result.data.length == 0);

	TEST_ASSERT(ntt_ListDestroy(&result.data) == NTT_RESULT_SUCCESS);
}

TEST_CASE(ListInsertWithPointer)
{
	ntt_ListResult result = ntt_ListCreate(NULL);
	TEST_ASSERT(result.result == NTT_RESULT_SUCCESS);

	int a = 42;
	int b = 84;
	int c = 168;
	int d = 336;

	TEST_ASSERT(ntt_ListInsertAfterNode(&result.data, NULL, &a, sizeof(a)) == NTT_RESULT_SUCCESS);
	TEST_ASSERT(result.data.length == 1);

	TEST_ASSERT(ntt_ListInsertAfterNode(&result.data, result.data.pHead, &b, sizeof(b)) == NTT_RESULT_SUCCESS);
	TEST_ASSERT(result.data.length == 2);

	TEST_ASSERT(ntt_ListInsertBeforeNode(&result.data, result.data.pHead->pNext, &c, sizeof(c)) == NTT_RESULT_SUCCESS);
	TEST_ASSERT(result.data.length == 3);

	TEST_ASSERT(ntt_ListInsertBeforeNode(&result.data, NULL, &d, sizeof(d)) == NTT_RESULT_SUCCESS);
	TEST_ASSERT(result.data.length == 4);

	TEST_ASSERT(*(int*)(ntt_ListGet(&result.data, 0).pData) == d);
	TEST_ASSERT(*(int*)(ntt_ListGet(&result.data, 1).pData) == a);
	TEST_ASSERT(*(int*)(ntt_ListGet(&result.data, 2).pData) == c);
	TEST_ASSERT(*(int*)(ntt_ListGet(&result.data, 3).pData) == b);

	TEST_ASSERT(ntt_ListDestroy(&result.data) == NTT_RESULT_SUCCESS);
}

TEST_SUITE_DEFINE(list,
				  list_tests_before_each,
				  list_tests_after_each,
				  TEST_CASE_DECLARE(ListCreateAndDestroy),
				  TEST_CASE_DECLARE(ListAppendAndClear),
				  TEST_CASE_DECLARE(ListInsertWithPointer),
				  TEST_CASE_DECLARE_WITHOUT_WRAPPER(ListDestroyMissingAllocator))

#endif /* _LIST_TESTS_H_ */