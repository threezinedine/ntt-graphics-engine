#ifndef _MAP_TESTS_H_
#define _MAP_TESTS_H_

#include "engine/engine.h"
#include "tools/tools.h"

void map_tests_before_each()
{
	ntt_Result result = ntt_MemoryGlobalsInitialize();
	TEST_ASSERT(result == NTT_RESULT_SUCCESS);
}

void map_tests_after_each()
{
	ntt_Result result = ntt_MemoryGlobalsDestroy();
	TEST_ASSERT(result == NTT_RESULT_SUCCESS);
}

u32 intHashFunction(void* pKey, usize keySize)
{
	NTT_UNUSED(keySize);
	return (u32)(*(i32*)pKey);
}

TEST_CASE(SimpleCase)
{
	ntt_MapResult result = ntt_MapCreate(intHashFunction, 0, NULL);
	TEST_ASSERT(result.result == NTT_RESULT_SUCCESS);

	i32			a	   = 10;
	const char* aValue = "ten";

	TEST_ASSERT(ntt_MapContains(&result.data, &a, sizeof(a)) == FALSE);
	TEST_ASSERT(ntt_MapInsert(&result.data, &a, sizeof(a), (void*)aValue, sizeof(aValue)) == NTT_RESULT_SUCCESS);
	TEST_ASSERT(ntt_MapContains(&result.data, &a, sizeof(a)) == TRUE);

	ntt_KeyValuePairResult getResult = ntt_MapGet(&result.data, &a, sizeof(a));
	TEST_ASSERT(getResult.result == NTT_RESULT_SUCCESS);
	TEST_ASSERT(*((i32*)getResult.data.pKey) == a);
	TEST_ASSERT(ntt_StrEquals((const char*)getResult.data.pValue, aValue) == TRUE);

	TEST_ASSERT(ntt_MapRemove(&result.data, &a, sizeof(a)) == NTT_RESULT_SUCCESS);
	TEST_ASSERT(ntt_MapContains(&result.data, &a, sizeof(a)) == FALSE);

	TEST_ASSERT(ntt_MapDestroy(&result.data) == NTT_RESULT_SUCCESS);
}

TEST_CASE(RemapTheBuckets)
{
	ntt_MapResult result = ntt_MapCreate(intHashFunction, 2, NULL);
	TEST_ASSERT(result.result == NTT_RESULT_SUCCESS);

	for (i32 i = 0; i < 10; i++)
	{
		TEST_ASSERT(ntt_MapInsert(&result.data, &i, sizeof(i), &i, sizeof(i)) == NTT_RESULT_SUCCESS);
	}

	TEST_ASSERT(result.data.bucketCount >= 2);

	for (i32 i = 0; i < 10; i++)
	{
		TEST_ASSERT(ntt_MapContains(&result.data, &i, sizeof(i)) == TRUE);

		ntt_KeyValuePairResult getResult = ntt_MapGet(&result.data, &i, sizeof(i));
		TEST_ASSERT(getResult.result == NTT_RESULT_SUCCESS);
		TEST_ASSERT(*((i32*)getResult.data.pKey) == i);
		TEST_ASSERT(*((i32*)getResult.data.pValue) == i);
	}

	TEST_ASSERT(ntt_MapDestroy(&result.data) == NTT_RESULT_SUCCESS);
}

TEST_CASE(MapContainsEmptyMap)
{
	ntt_MapResult result = ntt_MapCreate(intHashFunction, 0, NULL);
	TEST_ASSERT(result.result == NTT_RESULT_SUCCESS);

	i32 key = 42;
	TEST_ASSERT(ntt_MapContains(&result.data, &key, sizeof(key)) == FALSE);

	TEST_ASSERT(ntt_MapDestroy(&result.data) == NTT_RESULT_SUCCESS);
}

TEST_CASE(MapContainsAfterInsert)
{
	ntt_MapResult result = ntt_MapCreate(intHashFunction, 0, NULL);
	TEST_ASSERT(result.result == NTT_RESULT_SUCCESS);

	i32 key1   = 10;
	i32 key2   = 20;
	i32 key3   = 30;
	i32 value1 = 100;
	i32 value2 = 200;
	i32 value3 = 300;

	TEST_ASSERT(ntt_MapInsert(&result.data, &key1, sizeof(key1), &value1, sizeof(value1)) == NTT_RESULT_SUCCESS);
	TEST_ASSERT(ntt_MapInsert(&result.data, &key2, sizeof(key2), &value2, sizeof(value2)) == NTT_RESULT_SUCCESS);
	TEST_ASSERT(ntt_MapInsert(&result.data, &key3, sizeof(key3), &value3, sizeof(value3)) == NTT_RESULT_SUCCESS);

	TEST_ASSERT(ntt_MapContains(&result.data, &key1, sizeof(key1)) == TRUE);
	TEST_ASSERT(ntt_MapContains(&result.data, &key2, sizeof(key2)) == TRUE);
	TEST_ASSERT(ntt_MapContains(&result.data, &key3, sizeof(key3)) == TRUE);

	TEST_ASSERT(ntt_MapDestroy(&result.data) == NTT_RESULT_SUCCESS);
}

TEST_CASE(MapContainsNonExistentKey)
{
	ntt_MapResult result = ntt_MapCreate(intHashFunction, 0, NULL);
	TEST_ASSERT(result.result == NTT_RESULT_SUCCESS);

	i32 key1		   = 10;
	i32 key2		   = 20;
	i32 key3		   = 30;
	i32 nonExistentKey = 99;
	i32 value		   = 100;

	TEST_ASSERT(ntt_MapInsert(&result.data, &key1, sizeof(key1), &value, sizeof(value)) == NTT_RESULT_SUCCESS);
	TEST_ASSERT(ntt_MapInsert(&result.data, &key2, sizeof(key2), &value, sizeof(value)) == NTT_RESULT_SUCCESS);
	TEST_ASSERT(ntt_MapInsert(&result.data, &key3, sizeof(key3), &value, sizeof(value)) == NTT_RESULT_SUCCESS);

	TEST_ASSERT(ntt_MapContains(&result.data, &nonExistentKey, sizeof(nonExistentKey)) == FALSE);

	TEST_ASSERT(ntt_MapDestroy(&result.data) == NTT_RESULT_SUCCESS);
}

TEST_CASE(MapContainsAfterRemoval)
{
	ntt_MapResult result = ntt_MapCreate(intHashFunction, 0, NULL);
	TEST_ASSERT(result.result == NTT_RESULT_SUCCESS);

	i32 key1  = 10;
	i32 key2  = 20;
	i32 value = 100;

	TEST_ASSERT(ntt_MapInsert(&result.data, &key1, sizeof(key1), &value, sizeof(value)) == NTT_RESULT_SUCCESS);
	TEST_ASSERT(ntt_MapInsert(&result.data, &key2, sizeof(key2), &value, sizeof(value)) == NTT_RESULT_SUCCESS);

	TEST_ASSERT(ntt_MapContains(&result.data, &key1, sizeof(key1)) == TRUE);
	TEST_ASSERT(ntt_MapContains(&result.data, &key2, sizeof(key2)) == TRUE);

	TEST_ASSERT(ntt_MapRemove(&result.data, &key1, sizeof(key1)) == NTT_RESULT_SUCCESS);

	TEST_ASSERT(ntt_MapContains(&result.data, &key1, sizeof(key1)) == FALSE);
	TEST_ASSERT(ntt_MapContains(&result.data, &key2, sizeof(key2)) == TRUE);

	TEST_ASSERT(ntt_MapDestroy(&result.data) == NTT_RESULT_SUCCESS);
}

TEST_CASE(MapContainsNullPointer)
{
	ntt_MapResult result = ntt_MapCreate(intHashFunction, 0, NULL);
	TEST_ASSERT(result.result == NTT_RESULT_SUCCESS);

	i32 key = 42;

	TEST_ASSERT(ntt_MapContains(NULL, &key, sizeof(key)) == FALSE);
	TEST_ASSERT(ntt_MapContains(&result.data, NULL, sizeof(key)) == FALSE);

	TEST_ASSERT(ntt_MapDestroy(&result.data) == NTT_RESULT_SUCCESS);
}

TEST_CASE(MapContainsCollisionAndRemap)
{
	ntt_MapResult result = ntt_MapCreate(intHashFunction, 2, NULL);
	TEST_ASSERT(result.result == NTT_RESULT_SUCCESS);

	for (i32 i = 0; i < 15; i++)
	{
		TEST_ASSERT(ntt_MapInsert(&result.data, &i, sizeof(i), &i, sizeof(i)) == NTT_RESULT_SUCCESS);
		TEST_ASSERT(ntt_MapContains(&result.data, &i, sizeof(i)) == TRUE);
	}

	for (i32 i = 0; i < 15; i++)
	{
		TEST_ASSERT(ntt_MapContains(&result.data, &i, sizeof(i)) == TRUE);
	}

	i32 nonExistentKey = 999;
	TEST_ASSERT(ntt_MapContains(&result.data, &nonExistentKey, sizeof(nonExistentKey)) == FALSE);

	TEST_ASSERT(ntt_MapDestroy(&result.data) == NTT_RESULT_SUCCESS);
}

TEST_CASE(MapContainsWithUpdate)
{
	ntt_MapResult result = ntt_MapCreate(intHashFunction, 0, NULL);
	TEST_ASSERT(result.result == NTT_RESULT_SUCCESS);

	i32 key	   = 10;
	i32 value1 = 100;
	i32 value2 = 200;

	TEST_ASSERT(ntt_MapInsert(&result.data, &key, sizeof(key), &value1, sizeof(value1)) == NTT_RESULT_SUCCESS);
	TEST_ASSERT(ntt_MapContains(&result.data, &key, sizeof(key)) == TRUE);

	ntt_KeyValuePairResult getResult = ntt_MapGet(&result.data, &key, sizeof(key));
	TEST_ASSERT(getResult.result == NTT_RESULT_SUCCESS);
	TEST_ASSERT(*((i32*)getResult.data.pValue) == value1);

	TEST_ASSERT(ntt_MapInsert(&result.data, &key, sizeof(key), &value2, sizeof(value2)) == NTT_RESULT_SUCCESS);
	TEST_ASSERT(ntt_MapContains(&result.data, &key, sizeof(key)) == TRUE);

	TEST_ASSERT(result.data.count == 1);

	getResult = ntt_MapGet(&result.data, &key, sizeof(key));
	TEST_ASSERT(getResult.result == NTT_RESULT_SUCCESS);

	TEST_ASSERT(ntt_MapDestroy(&result.data) == NTT_RESULT_SUCCESS);
}

TEST_SUITE_DEFINE(map,
				  map_tests_before_each,
				  map_tests_after_each,
				  TEST_CASE_DECLARE(SimpleCase),
				  TEST_CASE_DECLARE(RemapTheBuckets),
				  TEST_CASE_DECLARE(MapContainsEmptyMap),
				  TEST_CASE_DECLARE(MapContainsAfterInsert),
				  TEST_CASE_DECLARE(MapContainsNonExistentKey),
				  TEST_CASE_DECLARE(MapContainsAfterRemoval),
				  TEST_CASE_DECLARE(MapContainsNullPointer),
				  TEST_CASE_DECLARE(MapContainsCollisionAndRemap),
				  TEST_CASE_DECLARE(MapContainsWithUpdate))

#endif /* _MAP_TESTS_H_ */