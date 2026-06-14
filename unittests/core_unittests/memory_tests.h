#ifndef _MEMORY_TESTS_H_
#define _MEMORY_TESTS_H_

#include "engine/engine.h"
#include "tools/tools.h"

void memory_tests_before_each()
{
	ntt_Result result = ntt_MemoryGlobalsInitialize();
	TEST_ASSERT(result == NTT_RESULT_SUCCESS);
}

void memory_tests_after_each()
{
	ntt_Result result = ntt_MemoryGlobalsDestroy();
	TEST_ASSERT(result == NTT_RESULT_SUCCESS);
}

TEST_CASE(AllocateAndDeallocate)
{
	voidPtrResult result = g_memoryGlobals.mallocAllocator->allocate(g_memoryGlobals.mallocAllocator, 128);
	TEST_ASSERT(result.result == NTT_RESULT_SUCCESS);
	ntt_Result deallocateResult =
		g_memoryGlobals.mallocAllocator->deallocate(g_memoryGlobals.mallocAllocator, result.pData, 128);
	TEST_ASSERT(deallocateResult == NTT_RESULT_SUCCESS);
}

TEST_CASE(MemoryLeak)
{
	ntt_Result result = ntt_MemoryGlobalsInitialize();
	TEST_ASSERT(result == NTT_RESULT_SUCCESS);

	voidPtrResult allocateResult = g_memoryGlobals.mallocAllocator->allocate(g_memoryGlobals.mallocAllocator, 128);
	TEST_ASSERT(allocateResult.result == NTT_RESULT_SUCCESS);
	ntt_Result deallocateResult =
		g_memoryGlobals.mallocAllocator->deallocate(g_memoryGlobals.mallocAllocator, allocateResult.pData, 100);
	TEST_ASSERT(deallocateResult == NTT_RESULT_FREE_MISMATCH_SIZE);

	ntt_Result destroyResult = ntt_MemoryGlobalsDestroy();
	TEST_ASSERT(destroyResult == NTT_RESULT_MEMORY_LEAK);
}

TEST_SUITE_DEFINE(memory,
				  memory_tests_before_each,
				  memory_tests_after_each,
				  TEST_CASE_DECLARE(AllocateAndDeallocate),
				  TEST_CASE_DECLARE_WITHOUT_WRAPPER(MemoryLeak))

#endif /* _MEMORY_TESTS_H_ */