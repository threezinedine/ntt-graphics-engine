#ifndef _STACK_TESTS_H_
#define _STACK_TESTS_H_

#include "engine/engine.h"
#include "tools/tools.h"

void stack_tests_before_each()
{
	ntt_Result result = ntt_MemoryGlobals_Initialize();
	TEST_ASSERT(result == NTT_RESULT_SUCCESS);
}

void stack_tests_after_each()
{
	ntt_Result result = ntt_MemoryGlobals_Destroy();
	TEST_ASSERT(result == NTT_RESULT_SUCCESS);
}

TEST_CASE(StackHappyPath)
{
	ntt_StackResult result = ntt_Stack_Create(NULL);
	TEST_ASSERT(result.result == NTT_RESULT_SUCCESS);

	i32 a = 10;
	i32 b = 20;
	i32 c = 30;

	TEST_ASSERT(ntt_Stack_Push(&result.data, &a, sizeof(a)) == NTT_RESULT_SUCCESS);
	TEST_ASSERT(ntt_Stack_Push(&result.data, &b, sizeof(b)) == NTT_RESULT_SUCCESS);
	TEST_ASSERT(ntt_Stack_Push(&result.data, &c, sizeof(c)) == NTT_RESULT_SUCCESS);

	voidPtrResult topResult = ntt_Stack_Top(&result.data);
	TEST_ASSERT(topResult.result == NTT_RESULT_SUCCESS);
	TEST_ASSERT(*(i32*)topResult.pData == 10);

	TEST_ASSERT(ntt_Stack_Pop(&result.data) == NTT_RESULT_SUCCESS);

	topResult = ntt_Stack_Top(&result.data);
	TEST_ASSERT(topResult.result == NTT_RESULT_SUCCESS);
	TEST_ASSERT(*(i32*)topResult.pData == 20);

	TEST_ASSERT(ntt_Stack_Pop(&result.data) == NTT_RESULT_SUCCESS);

	topResult = ntt_Stack_Top(&result.data);
	TEST_ASSERT(topResult.result == NTT_RESULT_SUCCESS);
	TEST_ASSERT(*(i32*)topResult.pData == 30);

	TEST_ASSERT(ntt_Stack_Pop(&result.data) == NTT_RESULT_SUCCESS);

	topResult = ntt_Stack_Top(&result.data);
	TEST_ASSERT(topResult.result == NTT_RESULT_STACK_EMPTY);
	TEST_ASSERT(topResult.pData == NULL);

	TEST_ASSERT(ntt_Stack_Destroy(&result.data) == NTT_RESULT_SUCCESS);
}

TEST_CASE(StackIsEmptyAndClear)
{
	ntt_StackResult result = ntt_Stack_Create(NULL);
	TEST_ASSERT(result.result == NTT_RESULT_SUCCESS);

	TEST_ASSERT(ntt_Stack_IsEmpty(&result.data) == TRUE);

	i32 value = 42;
	TEST_ASSERT(ntt_Stack_Push(&result.data, &value, sizeof(value)) == NTT_RESULT_SUCCESS);
	TEST_ASSERT(ntt_Stack_IsEmpty(&result.data) == FALSE);

	TEST_ASSERT(ntt_Stack_Clear(&result.data) == NTT_RESULT_SUCCESS);
	TEST_ASSERT(ntt_Stack_IsEmpty(&result.data) == TRUE);

	voidPtrResult topResult = ntt_Stack_Top(&result.data);
	TEST_ASSERT(topResult.result == NTT_RESULT_STACK_EMPTY);
	TEST_ASSERT(topResult.pData == NULL);

	TEST_ASSERT(ntt_Stack_Destroy(&result.data) == NTT_RESULT_SUCCESS);
}

TEST_CASE(StackNullAndEmptyEdgeCases)
{
	TEST_ASSERT(ntt_Stack_Push(NULL, NULL, 0) == NTT_RESULT_NULL_POINTER);

	voidPtrResult topResult = ntt_Stack_Top(NULL);
	TEST_ASSERT(topResult.result == NTT_RESULT_NULL_POINTER);
	TEST_ASSERT(topResult.pData == NULL);

	TEST_ASSERT(ntt_Stack_Pop(NULL) == NTT_RESULT_NULL_POINTER);
	TEST_ASSERT(ntt_Stack_IsEmpty(NULL) == TRUE);
	TEST_ASSERT(ntt_Stack_Clear(NULL) == NTT_RESULT_NULL_POINTER);
	TEST_ASSERT(ntt_Stack_Destroy(NULL) == NTT_RESULT_NULL_POINTER);

	ntt_StackResult result = ntt_Stack_Create(NULL);
	TEST_ASSERT(result.result == NTT_RESULT_SUCCESS);

	TEST_ASSERT(ntt_Stack_Pop(&result.data) == NTT_RESULT_NULL_POINTER);

	TEST_ASSERT(ntt_Stack_Destroy(&result.data) == NTT_RESULT_SUCCESS);
}

TEST_CASE(StackDestroyMissingAllocator)
{
	TEST_ASSERT(ntt_MemoryGlobals_Initialize() == NTT_RESULT_SUCCESS);

	ntt_StackResult result = ntt_Stack_Create(NULL);
	TEST_ASSERT(result.result == NTT_RESULT_SUCCESS);

	i32 value = 42;
	TEST_ASSERT(ntt_Stack_Push(&result.data, &value, sizeof(value)) == NTT_RESULT_SUCCESS);

	result.data.list.pAllocator = NULL;

	TEST_ASSERT(ntt_Stack_Destroy(&result.data) == NTT_RESULT_MISSING_ALLOCATOR);

	ntt_Result destroyGlobalsResult = ntt_MemoryGlobals_Destroy();
	TEST_ASSERT(destroyGlobalsResult == NTT_RESULT_MEMORY_LEAK);
}

TEST_SUITE_DEFINE(stack,
				  stack_tests_before_each,
				  stack_tests_after_each,
				  TEST_CASE_DECLARE(StackHappyPath),
				  TEST_CASE_DECLARE(StackIsEmptyAndClear),
				  TEST_CASE_DECLARE(StackNullAndEmptyEdgeCases),
				  TEST_CASE_DECLARE_WITHOUT_WRAPPER(StackDestroyMissingAllocator))

#endif /* _STACK_TESTS_H_ */