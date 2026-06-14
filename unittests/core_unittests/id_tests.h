#ifndef _ID_TESTS_H_
#define _ID_TESTS_H_

#include "engine/engine.h"
#include "tools/tools.h"

void id_tests_before_each()
{
	ntt_InitializeIDSystem();
}

void id_tests_after_each()
{
	ntt_DestroyIDSystem();
}

TEST_CASE(CreateIDTest)
{
	IDResult result = ntt_NewID(NTT_OBJECT_TYPE_OBJECT_ID);
	TEST_ASSERT(result.result == NTT_RESULT_SUCCESS);
}

TEST_CASE(ExceedMaxObjectsTest)
{
	for (u64 i = 0; i < MAX_OBJECTS; i++)
	{
		IDResult result = ntt_NewID(NTT_OBJECT_TYPE_OBJECT_ID);
		TEST_ASSERT(result.result == NTT_RESULT_SUCCESS);
	}

	IDResult result = ntt_NewID(NTT_OBJECT_TYPE_OBJECT_ID);
	TEST_ASSERT(result.result == NTT_RESULT_EXCEEDED_MAX_OBJECTS);
}

TEST_SUITE_DEFINE(id_tests,
				  id_tests_before_each,
				  id_tests_after_each,
				  TEST_CASE_DECLARE(CreateIDTest),
				  TEST_CASE_DECLARE(ExceedMaxObjectsTest))

#endif /* _ID_TESTS_H_ */