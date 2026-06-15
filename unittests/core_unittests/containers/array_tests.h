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

	TEST_ASSERT(result.data.elementSize == sizeof(i32));
	TEST_ASSERT(result.data.length == 0);
	TEST_ASSERT(result.data.capacity == 4);
	TEST_ASSERT(result.data.pData != NULL);

	ntt_ArrayDestroy(&result.data);
}

TEST_CASE(CreateArrayWithZeroInitialCapacity)
{
	ntt_ArrayResult result = ntt_ArrayCreate(sizeof(i32), 0, NULL);
	TEST_ASSERT(result.result == NTT_RESULT_SUCCESS);

	TEST_ASSERT(result.data.elementSize == sizeof(i32));
	TEST_ASSERT(result.data.length == 0);
	TEST_ASSERT(result.data.capacity == NTT_ARRAY_INITIAL_CAPACITY);
	TEST_ASSERT(result.data.pData != NULL);

	ntt_ArrayDestroy(&result.data);
}

TEST_CASE(CreateArrayWithNegativeInitialCapacity)
{
	ntt_ArrayResult result = ntt_ArrayCreate(sizeof(i32), (usize)-1, NULL);
	TEST_ASSERT(result.result == NTT_RESULT_NEGATIVE_INITIAL_CAPACITY);
}

TEST_CASE(CreateArrayWithoutAllocator)
{
	ntt_ArrayResult result = ntt_ArrayCreate(sizeof(i32), 4, NULL);
	TEST_ASSERT(result.result == NTT_RESULT_SUCCESS);

	TEST_ASSERT(result.data.pAllocator != NULL);

	ntt_Result destroyResult = ntt_ArrayDestroy(&result.data);
	TEST_ASSERT(destroyResult == NTT_RESULT_SUCCESS);
}

TEST_CASE(DestroyMissingAllocator)
{
	ntt_MemoryGlobalsInitialize();

	ntt_ArrayResult result = ntt_ArrayCreate(sizeof(i32), 4, NULL);
	TEST_ASSERT(result.result == NTT_RESULT_SUCCESS);

	result.data.pAllocator = NULL;

	ntt_Result destroyResult = ntt_ArrayDestroy(&result.data);
	TEST_ASSERT(destroyResult == NTT_RESULT_INITIALZE_ARRAY_WITHOUT_ALLOCATOR);

	ntt_Result destroyGlobalsResult = ntt_MemoryGlobalsDestroy();
	TEST_ASSERT(destroyGlobalsResult == NTT_RESULT_MEMORY_LEAK);
}

TEST_CASE(AppendAndGet)
{
	ntt_ArrayResult result = ntt_ArrayCreate(sizeof(i32), 2, NULL);
	TEST_ASSERT(result.result == NTT_RESULT_SUCCESS);

	i32 value1 = 42;
	i32 value2 = 84;
	i32 value3 = 168;

	ntt_Result appendResult1 = ntt_ArrayAppend(&result.data, &value1);
	TEST_ASSERT(appendResult1 == NTT_RESULT_SUCCESS);
	TEST_ASSERT(result.data.length == 1);

	ntt_Result appendResult2 = ntt_ArrayAppend(&result.data, &value2);
	TEST_ASSERT(appendResult2 == NTT_RESULT_SUCCESS);
	TEST_ASSERT(result.data.length == 2);

	ntt_Result appendResult3 = ntt_ArrayAppend(&result.data, &value3);
	TEST_ASSERT(appendResult3 == NTT_RESULT_SUCCESS);
	TEST_ASSERT(result.data.length == 3);
	TEST_ASSERT(result.data.capacity == 4); // Capacity should have been resized

	i32* pValue1 = (i32*)ntt_ArrayGet(&result.data, 0);
	i32* pValue2 = (i32*)ntt_ArrayGet(&result.data, 1);
	i32* pValue3 = (i32*)ntt_ArrayGet(&result.data, 2);

	TEST_ASSERT(pValue1 != NULL && *pValue1 == value1);
	TEST_ASSERT(pValue2 != NULL && *pValue2 == value2);
	TEST_ASSERT(pValue3 != NULL && *pValue3 == value3);

	ntt_ArrayDestroy(&result.data);
}

TEST_CASE(IsolatedAppend)
{
	ntt_ArrayResult result = ntt_ArrayCreate(sizeof(i32), 2, NULL);
	TEST_ASSERT(result.result == NTT_RESULT_SUCCESS);

	i32 value = 42;

	ntt_Result appendResult = ntt_ArrayAppend(&result.data, &value);
	TEST_ASSERT(appendResult == NTT_RESULT_SUCCESS);
	TEST_ASSERT(result.data.length == 1);

	value++;

	i32* pValue = (i32*)ntt_ArrayGet(&result.data, 0);
	TEST_ASSERT(pValue != NULL && *pValue != value);

	ntt_ArrayDestroy(&result.data);
}

TEST_CASE(EraseElement)
{
	ntt_ArrayResult result = ntt_ArrayCreate(sizeof(i32), 2, NULL);
	TEST_ASSERT(result.result == NTT_RESULT_SUCCESS);

	i32 value1 = 42;
	i32 value2 = 84;

	ntt_Result appendResult1 = ntt_ArrayAppend(&result.data, &value1);
	TEST_ASSERT(appendResult1 == NTT_RESULT_SUCCESS);
	TEST_ASSERT(result.data.length == 1);

	ntt_Result appendResult2 = ntt_ArrayAppend(&result.data, &value2);
	TEST_ASSERT(appendResult2 == NTT_RESULT_SUCCESS);
	TEST_ASSERT(result.data.length == 2);

	// Erase the first element by shifting the second element to the left
	ntt_Result eraseResult = ntt_ArrayErase(&result.data, 0);
	TEST_ASSERT(eraseResult == NTT_RESULT_SUCCESS);

	i32* pValue = (i32*)ntt_ArrayGet(&result.data, 0);
	TEST_ASSERT(pValue != NULL && *pValue == value2);

	ntt_ArrayDestroy(&result.data);
}

TEST_CASE(CleanArray)
{
	ntt_ArrayResult result = ntt_ArrayCreate(sizeof(i32), 2, NULL);
	TEST_ASSERT(result.result == NTT_RESULT_SUCCESS);

	i32 value1 = 42;
	i32 value2 = 84;

	ntt_Result appendResult1 = ntt_ArrayAppend(&result.data, &value1);
	TEST_ASSERT(appendResult1 == NTT_RESULT_SUCCESS);
	TEST_ASSERT(result.data.length == 1);

	ntt_Result appendResult2 = ntt_ArrayAppend(&result.data, &value2);
	TEST_ASSERT(appendResult2 == NTT_RESULT_SUCCESS);
	TEST_ASSERT(result.data.length == 2);

	// Clean the array by setting the length to 0, this should not free the memory of the array
	ntt_Result clearResult = ntt_ArrayClear(&result.data);
	TEST_ASSERT(clearResult == NTT_RESULT_SUCCESS);

	TEST_ASSERT(result.data.length == 0);
	TEST_ASSERT(result.data.capacity == 2); // Capacity should remain unchanged

	ntt_ArrayDestroy(&result.data);
}

b8 IsEven(void* pElement)
{
	i32* pValue = (i32*)pElement;
	return (*pValue % 2) == 0;
}

b8 IsGreaterThan50(void* pElement)
{
	i32* pValue = (i32*)pElement;
	return *pValue > 50;
}

b8 IsGreaterThan100(void* pElement)
{
	i32* pValue = (i32*)pElement;
	return *pValue > 100;
}

TEST_CASE(AnyTest)
{
	ntt_ArrayResult result = ntt_ArrayCreate(sizeof(i32), 2, NULL);
	TEST_ASSERT(result.result == NTT_RESULT_SUCCESS);

	i32 value1 = 42;
	i32 value2 = 84;

	ntt_Result appendResult1 = ntt_ArrayAppend(&result.data, &value1);
	TEST_ASSERT(appendResult1 == NTT_RESULT_SUCCESS);
	TEST_ASSERT(result.data.length == 1);

	ntt_Result appendResult2 = ntt_ArrayAppend(&result.data, &value2);
	TEST_ASSERT(appendResult2 == NTT_RESULT_SUCCESS);
	TEST_ASSERT(result.data.length == 2);

	TEST_ASSERT(ntt_ArrayAny(&result.data, IsEven) == TRUE);
	TEST_ASSERT(ntt_ArrayAny(&result.data, IsGreaterThan50) == TRUE);
	TEST_ASSERT(ntt_ArrayAny(&result.data, IsGreaterThan100) == FALSE);

	ntt_ArrayDestroy(&result.data);
}

TEST_CASE(AllTest)
{
	ntt_ArrayResult result = ntt_ArrayCreate(sizeof(i32), 2, NULL);
	TEST_ASSERT(result.result == NTT_RESULT_SUCCESS);

	i32 value1 = 42;
	i32 value2 = 84;

	ntt_Result appendResult1 = ntt_ArrayAppend(&result.data, &value1);
	TEST_ASSERT(appendResult1 == NTT_RESULT_SUCCESS);
	TEST_ASSERT(result.data.length == 1);

	ntt_Result appendResult2 = ntt_ArrayAppend(&result.data, &value2);
	TEST_ASSERT(appendResult2 == NTT_RESULT_SUCCESS);
	TEST_ASSERT(result.data.length == 2);

	TEST_ASSERT(ntt_ArrayAll(&result.data, IsEven) == TRUE);
	TEST_ASSERT(ntt_ArrayAll(&result.data, IsGreaterThan50) == FALSE);
	TEST_ASSERT(ntt_ArrayAll(&result.data, IsGreaterThan100) == FALSE);

	ntt_ArrayDestroy(&result.data);
}

b8 IsEqualTo42(void* pElement)
{
	i32* pValue = (i32*)pElement;
	return *pValue == 42;
}

b8 IsEqualTo84(void* pElement)
{
	i32* pValue = (i32*)pElement;
	return *pValue == 84;
}

b8 IsEqualTo168(void* pElement)
{
	i32* pValue = (i32*)pElement;
	return *pValue == 168;
}

TEST_CASE(FindIndex)
{
	ntt_ArrayResult result = ntt_ArrayCreate(sizeof(i32), 2, NULL);
	TEST_ASSERT(result.result == NTT_RESULT_SUCCESS);

	i32 value1 = 42;
	i32 value2 = 84;

	ntt_ArrayAppend(&result.data, &value1);
	ntt_ArrayAppend(&result.data, &value2);

	usize findIndexResult1 = ntt_ArrayFind(&result.data, IsEqualTo42);
	TEST_ASSERT(findIndexResult1 == 0);

	usize findIndexResult2 = ntt_ArrayFind(&result.data, IsEqualTo84);
	TEST_ASSERT(findIndexResult2 == 1);

	usize findIndexResult3 = ntt_ArrayFind(&result.data, IsEqualTo168);
	TEST_ASSERT(findIndexResult3 == NTT_ARRAY_INDEX_NOT_FOUND);

	ntt_ArrayDestroy(&result.data);
}

TEST_CASE(InsertIntoArray)
{
	ntt_ArrayResult result = ntt_ArrayCreate(sizeof(i32), 2, NULL);
	TEST_ASSERT(result.result == NTT_RESULT_SUCCESS);

	i32 value1 = 42;
	i32 value2 = 84;
	i32 value3 = 168;

	ntt_Result insertResult1 = ntt_ArrayInsert(&result.data, 0, &value1);
	TEST_ASSERT(insertResult1 == NTT_RESULT_SUCCESS);
	TEST_ASSERT(result.data.length == 1);

	ntt_Result insertResult2 = ntt_ArrayInsert(&result.data, 0, &value2);
	TEST_ASSERT(insertResult2 == NTT_RESULT_SUCCESS);
	TEST_ASSERT(result.data.length == 2);

	ntt_Result insertResult3 = ntt_ArrayInsert(&result.data, 1, &value3);
	TEST_ASSERT(insertResult3 == NTT_RESULT_SUCCESS);
	TEST_ASSERT(result.data.length == 3);

	i32* pValue1 = (i32*)ntt_ArrayGet(&result.data, 0);
	i32* pValue2 = (i32*)ntt_ArrayGet(&result.data, 1);
	i32* pValue3 = (i32*)ntt_ArrayGet(&result.data, 2);

	TEST_ASSERT(pValue1 != NULL && *pValue1 == value2);
	TEST_ASSERT(pValue2 != NULL && *pValue2 == value3);
	TEST_ASSERT(pValue3 != NULL && *pValue3 == value1);

	ntt_ArrayDestroy(&result.data);
}

TEST_SUITE_DEFINE(array,
				  array_tests_before_each,
				  array_tests_after_each,
				  TEST_CASE_DECLARE(CreateArray),
				  TEST_CASE_DECLARE(CreateArrayWithZeroInitialCapacity),
				  TEST_CASE_DECLARE(CreateArrayWithoutAllocator),
				  TEST_CASE_DECLARE_WITHOUT_WRAPPER(DestroyMissingAllocator),
				  TEST_CASE_DECLARE(AppendAndGet),
				  TEST_CASE_DECLARE(CreateArrayWithNegativeInitialCapacity),
				  TEST_CASE_DECLARE(EraseElement),
				  TEST_CASE_DECLARE(CleanArray),
				  TEST_CASE_DECLARE(AnyTest),
				  TEST_CASE_DECLARE(AllTest),
				  TEST_CASE_DECLARE(FindIndex),
				  TEST_CASE_DECLARE(InsertIntoArray),
				  TEST_CASE_DECLARE(IsolatedAppend))

#endif /* _ARRAY_TESTS_H_ */