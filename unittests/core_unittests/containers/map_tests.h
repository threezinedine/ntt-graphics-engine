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

	TEST_ASSERT(ntt_MapDestroy(&result.data) == NTT_RESULT_SUCCESS);
}

TEST_SUITE_DEFINE(map, map_tests_before_each, map_tests_after_each, TEST_CASE_DECLARE(SimpleCase))

#endif /* _MAP_TESTS_H_ */