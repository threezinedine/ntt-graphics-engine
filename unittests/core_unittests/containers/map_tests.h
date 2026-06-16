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

u32 stringHashFunction(void* pKey, usize keySize)
{
	NTT_UNUSED(keySize);
	const char* str	 = (const char*)pKey;
	u32			hash = 5381;

	while (*str != '\0')
	{
		hash = ((hash << 5) + hash) + (u32)(*str);
		str++;
	}

	return hash;
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

TEST_CASE(MapWithStringKeys)
{
	ntt_MapResult result = ntt_MapCreate(stringHashFunction, 0, NULL);
	TEST_ASSERT(result.result == NTT_RESULT_SUCCESS);

	const char* key1 = "name";
	const char* key2 = "age";
	const char* key3 = "city";

	const char* value1 = "Alice";
	const char* value2 = "30";
	const char* value3 = "New York";

	usize key1Size = ntt_StrLen(key1) + 1;
	usize key2Size = ntt_StrLen(key2) + 1;
	usize key3Size = ntt_StrLen(key3) + 1;

	usize value1Size = ntt_StrLen(value1) + 1;
	usize value2Size = ntt_StrLen(value2) + 1;
	usize value3Size = ntt_StrLen(value3) + 1;

	TEST_ASSERT(ntt_MapInsert(&result.data, (void*)key1, key1Size, (void*)value1, value1Size) == NTT_RESULT_SUCCESS);
	TEST_ASSERT(ntt_MapInsert(&result.data, (void*)key2, key2Size, (void*)value2, value2Size) == NTT_RESULT_SUCCESS);
	TEST_ASSERT(ntt_MapInsert(&result.data, (void*)key3, key3Size, (void*)value3, value3Size) == NTT_RESULT_SUCCESS);

	TEST_ASSERT(ntt_MapContains(&result.data, (void*)key1, key1Size) == TRUE);
	TEST_ASSERT(ntt_MapContains(&result.data, (void*)key2, key2Size) == TRUE);
	TEST_ASSERT(ntt_MapContains(&result.data, (void*)key3, key3Size) == TRUE);

	ntt_KeyValuePairResult getResult = ntt_MapGet(&result.data, (void*)key1, key1Size);
	TEST_ASSERT(getResult.result == NTT_RESULT_SUCCESS);
	TEST_ASSERT(ntt_StrEquals((const char*)getResult.data.pValue, value1) == TRUE);

	getResult = ntt_MapGet(&result.data, (void*)key2, key2Size);
	TEST_ASSERT(getResult.result == NTT_RESULT_SUCCESS);
	TEST_ASSERT(ntt_StrEquals((const char*)getResult.data.pValue, value2) == TRUE);

	getResult = ntt_MapGet(&result.data, (void*)key3, key3Size);
	TEST_ASSERT(getResult.result == NTT_RESULT_SUCCESS);
	TEST_ASSERT(ntt_StrEquals((const char*)getResult.data.pValue, value3) == TRUE);

	const char* nonExistentKey	   = "email";
	usize		nonExistentKeySize = ntt_StrLen(nonExistentKey) + 1;
	TEST_ASSERT(ntt_MapContains(&result.data, (void*)nonExistentKey, nonExistentKeySize) == FALSE);

	TEST_ASSERT(ntt_MapRemove(&result.data, (void*)key2, key2Size) == NTT_RESULT_SUCCESS);
	TEST_ASSERT(ntt_MapContains(&result.data, (void*)key2, key2Size) == FALSE);
	TEST_ASSERT(ntt_MapContains(&result.data, (void*)key1, key1Size) == TRUE);
	TEST_ASSERT(ntt_MapContains(&result.data, (void*)key3, key3Size) == TRUE);

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
				  TEST_CASE_DECLARE(MapContainsWithUpdate),
				  TEST_CASE_DECLARE(MapWithStringKeys))

#endif /* _MAP_TESTS_H_ */