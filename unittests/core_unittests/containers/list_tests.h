#ifndef _LIST_TESTS_H_
#define _LIST_TESTS_H_

#include "engine/engine.h"
#include "tools/tools.h"

void list_tests_before_each()
{
	ntt_Result result = ntt_MemoryGlobals_Initialize();
	TEST_ASSERT(result == NTT_RESULT_SUCCESS);
}

void list_tests_after_each()
{
	ntt_Result result = ntt_MemoryGlobals_Destroy();
	TEST_ASSERT(result == NTT_RESULT_SUCCESS);
}

TEST_CASE(ListCreateAndDestroy)
{
	ntt_ListResult result = ntt_List_Create(NULL, NULL);
	TEST_ASSERT(result.result == NTT_RESULT_SUCCESS);

	ntt_Result destroyResult = ntt_List_Destroy(&result.data);
	TEST_ASSERT(destroyResult == NTT_RESULT_SUCCESS);
}

TEST_CASE(ListDestroyMissingAllocator)
{
	TEST_ASSERT(ntt_MemoryGlobals_Initialize() == NTT_RESULT_SUCCESS);

	ntt_ListResult result = ntt_List_Create(NULL, NULL);
	TEST_ASSERT(result.result == NTT_RESULT_SUCCESS);

	int i = 42;

	ntt_Result appendResult = ntt_List_Append(&result.data, &i, sizeof(i));
	TEST_ASSERT(appendResult == NTT_RESULT_SUCCESS);

	result.data.pAllocator = NULL;

	ntt_Result destroyResult = ntt_List_Destroy(&result.data);
	TEST_ASSERT(destroyResult == NTT_RESULT_MISSING_ALLOCATOR);

	ntt_Result destroyGlobalsResult = ntt_MemoryGlobals_Destroy();
	TEST_ASSERT(destroyGlobalsResult == NTT_RESULT_MEMORY_LEAK);
}

TEST_CASE(ListAppendAndClear)
{
	ntt_ListResult result = ntt_List_Create(NULL, NULL);
	TEST_ASSERT(result.result == NTT_RESULT_SUCCESS);

	int i = 42;
	int j = 84;

	ntt_Result appendResult = ntt_List_Append(&result.data, &i, sizeof(i));
	TEST_ASSERT(appendResult == NTT_RESULT_SUCCESS);
	TEST_ASSERT(result.data.length == 1);

	appendResult = ntt_List_Append(&result.data, &j, sizeof(j));
	TEST_ASSERT(appendResult == NTT_RESULT_SUCCESS);
	TEST_ASSERT(result.data.length == 2);

	ntt_Result clearResult = ntt_List_Clear(&result.data);
	TEST_ASSERT(clearResult == NTT_RESULT_SUCCESS);
	TEST_ASSERT(result.data.length == 0);

	TEST_ASSERT(ntt_List_Destroy(&result.data) == NTT_RESULT_SUCCESS);
}

TEST_CASE(ListInsertWithPointer)
{
	ntt_ListResult result = ntt_List_Create(NULL, NULL);
	TEST_ASSERT(result.result == NTT_RESULT_SUCCESS);

	int a = 42;
	int b = 84;
	int c = 168;
	int d = 336;

	TEST_ASSERT(ntt_List_InsertAfterNode(&result.data, NULL, &a, sizeof(a)) == NTT_RESULT_SUCCESS);
	TEST_ASSERT(result.data.length == 1);

	TEST_ASSERT(ntt_List_InsertAfterNode(&result.data, result.data.pHead, &b, sizeof(b)) == NTT_RESULT_SUCCESS);
	TEST_ASSERT(result.data.length == 2);

	TEST_ASSERT(ntt_List_InsertBeforeNode(&result.data, result.data.pHead->pNext, &c, sizeof(c)) == NTT_RESULT_SUCCESS);
	TEST_ASSERT(result.data.length == 3);

	TEST_ASSERT(ntt_List_InsertBeforeNode(&result.data, NULL, &d, sizeof(d)) == NTT_RESULT_SUCCESS);
	TEST_ASSERT(result.data.length == 4);

	TEST_ASSERT(*(int*)(ntt_List_Get(&result.data, 0).pData) == d);
	TEST_ASSERT(*(int*)(ntt_List_Get(&result.data, 1).pData) == a);
	TEST_ASSERT(*(int*)(ntt_List_Get(&result.data, 2).pData) == c);
	TEST_ASSERT(*(int*)(ntt_List_Get(&result.data, 3).pData) == b);

	TEST_ASSERT(ntt_List_RemoveNode(&result.data, result.data.pHead->pNext) == NTT_RESULT_SUCCESS);

	// insert random node -> return invalid node owner
	ntt_ListNode fakeNode = {0};
	TEST_ASSERT(ntt_List_InsertBeforeNode(&result.data, &fakeNode, &c, sizeof(c)) == NTT_RESULT_INVALID_NODE_OWNER);
	TEST_ASSERT(ntt_List_InsertAfterNode(&result.data, &fakeNode, &c, sizeof(c)) == NTT_RESULT_INVALID_NODE_OWNER);

	TEST_ASSERT(result.data.length == 3);
	TEST_ASSERT(*(int*)(ntt_List_Get(&result.data, 0).pData) == d);
	TEST_ASSERT(*(int*)(ntt_List_Get(&result.data, 1).pData) == c);
	TEST_ASSERT(*(int*)(ntt_List_Get(&result.data, 2).pData) == b);

	TEST_ASSERT(ntt_List_Destroy(&result.data) == NTT_RESULT_SUCCESS);
}

b8 IsEqualTo(void* pElement, usize elementSize, void* pUserData, usize userDataSize)
{
	NTT_UNUSED(elementSize);
	NTT_UNUSED(userDataSize);
	return *(i32*)pElement == *(i32*)pUserData;
}

TEST_CASE(ListContains)
{
	ntt_ListResult result = ntt_List_Create(NULL, NULL);
	TEST_ASSERT(result.result == NTT_RESULT_SUCCESS);

	i32 a = 42;
	i32 b = 84;

	TEST_ASSERT(ntt_List_Append(&result.data, &a, sizeof(a)) == NTT_RESULT_SUCCESS);
	TEST_ASSERT(ntt_List_Append(&result.data, &b, sizeof(b)) == NTT_RESULT_SUCCESS);

	TEST_ASSERT(ntt_List_Contains(&result.data, IsEqualTo, &a, sizeof(a)) == TRUE);
	TEST_ASSERT(ntt_List_Contains(&result.data, IsEqualTo, &b, sizeof(b)) == TRUE);
	TEST_ASSERT(ntt_List_Contains(&result.data, IsEqualTo, &(i32){999}, sizeof(i32)) == FALSE);
	TEST_ASSERT(ntt_List_Contains(NULL, IsEqualTo, &a, sizeof(a)) == FALSE);
	TEST_ASSERT(ntt_List_Contains(&result.data, NULL, &a, sizeof(a)) == FALSE);

	TEST_ASSERT(ntt_List_Destroy(&result.data) == NTT_RESULT_SUCCESS);
}

TEST_CASE(ListRemoveByIndex)
{
	ntt_ListResult result = ntt_List_Create(NULL, NULL);
	TEST_ASSERT(result.result == NTT_RESULT_SUCCESS);

	i32 a = 10;
	i32 b = 20;
	i32 c = 30;
	i32 d = 40;

	TEST_ASSERT(ntt_List_Append(&result.data, &a, sizeof(a)) == NTT_RESULT_SUCCESS);
	TEST_ASSERT(ntt_List_Append(&result.data, &b, sizeof(b)) == NTT_RESULT_SUCCESS);
	TEST_ASSERT(ntt_List_Append(&result.data, &c, sizeof(c)) == NTT_RESULT_SUCCESS);
	TEST_ASSERT(ntt_List_Append(&result.data, &d, sizeof(d)) == NTT_RESULT_SUCCESS);

	// Remove middle element (20)
	TEST_ASSERT(ntt_List_Remove(&result.data, 1) == NTT_RESULT_SUCCESS);
	TEST_ASSERT(result.data.length == 3);
	TEST_ASSERT(*(i32*)ntt_List_Get(&result.data, 0).pData == 10);
	TEST_ASSERT(*(i32*)ntt_List_Get(&result.data, 1).pData == 30);
	TEST_ASSERT(*(i32*)ntt_List_Get(&result.data, 2).pData == 40);

	// Remove tail element (40)
	TEST_ASSERT(ntt_List_Remove(&result.data, 2) == NTT_RESULT_SUCCESS);
	TEST_ASSERT(result.data.length == 2);
	TEST_ASSERT(*(i32*)ntt_List_Get(&result.data, 1).pData == 30);

	// Remove head element (10)
	TEST_ASSERT(ntt_List_Remove(&result.data, 0) == NTT_RESULT_SUCCESS);
	TEST_ASSERT(result.data.length == 1);
	TEST_ASSERT(*(i32*)ntt_List_Get(&result.data, 0).pData == 30);

	// Index out of bounds
	TEST_ASSERT(ntt_List_Remove(&result.data, 2) == NTT_RESULT_INDEX_OUT_OF_BOUNDS);
	TEST_ASSERT(ntt_List_Remove(NULL, 0) == NTT_RESULT_NULL_POINTER);

	ntt_Allocator* pAllocator = result.data.pAllocator;
	result.data.pAllocator	  = NULL;
	TEST_ASSERT(ntt_List_Remove(&result.data, 0) == NTT_RESULT_MISSING_ALLOCATOR);
	result.data.pAllocator = pAllocator;

	TEST_ASSERT(ntt_List_Destroy(&result.data) == NTT_RESULT_SUCCESS);
}

TEST_SUITE_DEFINE(list,
				  list_tests_before_each,
				  list_tests_after_each,
				  TEST_CASE_DECLARE(ListCreateAndDestroy),
				  TEST_CASE_DECLARE(ListAppendAndClear),
				  TEST_CASE_DECLARE(ListInsertWithPointer),
				  TEST_CASE_DECLARE(ListContains),
				  TEST_CASE_DECLARE(ListRemoveByIndex),
				  TEST_CASE_DECLARE_WITHOUT_WRAPPER(ListDestroyMissingAllocator))

#endif /* _LIST_TESTS_H_ */