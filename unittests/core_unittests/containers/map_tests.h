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

TEST_CASE(SimpleCase)
{
}

TEST_SUITE_DEFINE(map, map_tests_before_each, map_tests_after_each, TEST_CASE_DECLARE(SimpleCase))

#endif /* _MAP_TESTS_H_ */