#ifndef _QUEUE_TESTS_H_
#define _QUEUE_TESTS_H_

#include "engine/engine.h"
#include "tools/tools.h"

void queue_tests_before_each()
{
	ntt_Result result = ntt_MemoryGlobals_Initialize();
	TEST_ASSERT(result == NTT_RESULT_SUCCESS);
}

void queue_tests_after_each()
{
	ntt_Result result = ntt_MemoryGlobals_Destroy();
	TEST_ASSERT(result == NTT_RESULT_SUCCESS);
}

TEST_CASE(QueueHappyPath)
{
	ntt_QueueResult result = ntt_QueueCreate(NULL);
	TEST_ASSERT(result.result == NTT_RESULT_SUCCESS);

	i32 a = 10;
	i32 b = 20;
	i32 c = 30;

	TEST_ASSERT(ntt_QueueEnqueue(&result.data, &a, sizeof(a)) == NTT_RESULT_SUCCESS);
	TEST_ASSERT(ntt_QueueEnqueue(&result.data, &b, sizeof(b)) == NTT_RESULT_SUCCESS);
	TEST_ASSERT(ntt_QueueEnqueue(&result.data, &c, sizeof(c)) == NTT_RESULT_SUCCESS);

	voidPtrResult frontResult = ntt_QueueFront(&result.data);
	TEST_ASSERT(frontResult.result == NTT_RESULT_SUCCESS);
	TEST_ASSERT(*(i32*)frontResult.pData == 10);

	TEST_ASSERT(ntt_QueueDequeue(&result.data) == NTT_RESULT_SUCCESS);
	frontResult = ntt_QueueFront(&result.data);
	TEST_ASSERT(frontResult.result == NTT_RESULT_SUCCESS);
	TEST_ASSERT(*(i32*)frontResult.pData == 20);

	TEST_ASSERT(ntt_QueueDequeue(&result.data) == NTT_RESULT_SUCCESS);
	frontResult = ntt_QueueFront(&result.data);
	TEST_ASSERT(frontResult.result == NTT_RESULT_SUCCESS);
	TEST_ASSERT(*(i32*)frontResult.pData == 30);

	TEST_ASSERT(ntt_QueueDequeue(&result.data) == NTT_RESULT_SUCCESS);
	frontResult = ntt_QueueFront(&result.data);
	TEST_ASSERT(frontResult.result == NTT_RESULT_QUEUE_EMPTY);
	TEST_ASSERT(frontResult.pData == NULL);

	TEST_ASSERT(ntt_QueueDestroy(&result.data) == NTT_RESULT_SUCCESS);
}

TEST_CASE(QueueIsEmptyAndClear)
{
	ntt_QueueResult result = ntt_QueueCreate(NULL);
	TEST_ASSERT(result.result == NTT_RESULT_SUCCESS);

	TEST_ASSERT(ntt_QueueIsEmpty(&result.data) == TRUE);

	i32 value = 42;
	TEST_ASSERT(ntt_QueueEnqueue(&result.data, &value, sizeof(value)) == NTT_RESULT_SUCCESS);
	TEST_ASSERT(ntt_QueueIsEmpty(&result.data) == FALSE);

	TEST_ASSERT(ntt_QueueClear(&result.data) == NTT_RESULT_SUCCESS);
	TEST_ASSERT(ntt_QueueIsEmpty(&result.data) == TRUE);

	voidPtrResult frontResult = ntt_QueueFront(&result.data);
	TEST_ASSERT(frontResult.result == NTT_RESULT_QUEUE_EMPTY);
	TEST_ASSERT(frontResult.pData == NULL);

	TEST_ASSERT(ntt_QueueDestroy(&result.data) == NTT_RESULT_SUCCESS);
}

TEST_CASE(QueueNullAndEmptyEdgeCases)
{
	TEST_ASSERT(ntt_QueueEnqueue(NULL, NULL, 0) == NTT_RESULT_NULL_POINTER);

	voidPtrResult frontResult = ntt_QueueFront(NULL);
	TEST_ASSERT(frontResult.result == NTT_RESULT_NULL_POINTER);
	TEST_ASSERT(frontResult.pData == NULL);

	TEST_ASSERT(ntt_QueueDequeue(NULL) == NTT_RESULT_NULL_POINTER);
	TEST_ASSERT(ntt_QueueIsEmpty(NULL) == TRUE);
	TEST_ASSERT(ntt_QueueClear(NULL) == NTT_RESULT_NULL_POINTER);
	TEST_ASSERT(ntt_QueueDestroy(NULL) == NTT_RESULT_NULL_POINTER);

	ntt_QueueResult result = ntt_QueueCreate(NULL);
	TEST_ASSERT(result.result == NTT_RESULT_SUCCESS);

	TEST_ASSERT(ntt_QueueDequeue(&result.data) == NTT_RESULT_NULL_POINTER);

	TEST_ASSERT(ntt_QueueDestroy(&result.data) == NTT_RESULT_SUCCESS);
}

TEST_CASE(QueueDestroyMissingAllocator)
{
	TEST_ASSERT(ntt_MemoryGlobals_Initialize() == NTT_RESULT_SUCCESS);

	ntt_QueueResult result = ntt_QueueCreate(NULL);
	TEST_ASSERT(result.result == NTT_RESULT_SUCCESS);

	i32 value = 42;
	TEST_ASSERT(ntt_QueueEnqueue(&result.data, &value, sizeof(value)) == NTT_RESULT_SUCCESS);

	result.data.list.pAllocator = NULL;

	TEST_ASSERT(ntt_QueueDestroy(&result.data) == NTT_RESULT_MISSING_ALLOCATOR);

	ntt_Result destroyGlobalsResult = ntt_MemoryGlobals_Destroy();
	TEST_ASSERT(destroyGlobalsResult == NTT_RESULT_MEMORY_LEAK);
}

TEST_SUITE_DEFINE(queue,
				  queue_tests_before_each,
				  queue_tests_after_each,
				  TEST_CASE_DECLARE(QueueHappyPath),
				  TEST_CASE_DECLARE(QueueIsEmptyAndClear),
				  TEST_CASE_DECLARE(QueueNullAndEmptyEdgeCases),
				  TEST_CASE_DECLARE_WITHOUT_WRAPPER(QueueDestroyMissingAllocator))

#endif /* _QUEUE_TESTS_H_ */