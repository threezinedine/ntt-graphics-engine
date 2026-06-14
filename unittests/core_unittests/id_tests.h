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
	TEST_ASSERT(ntt_IsIDValid(&(result.data)) == TRUE);
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

TEST_CASE(InvalidIDTest)
{
	ID invalid = INVALID_ID;
	TEST_ASSERT(ntt_IsIDValid(&invalid) == FALSE);
}

TEST_CASE(UpdateIDTest)
{
	IDResult result = ntt_NewID(NTT_OBJECT_TYPE_OBJECT_ID);
	TEST_ASSERT(result.result == NTT_RESULT_SUCCESS);

	ID clone = result.data;

	ntt_Result updateResult = ntt_UpdateID(&result.data);
	TEST_ASSERT(updateResult == NTT_RESULT_SUCCESS);

	TEST_ASSERT(ntt_IsIDValid(&result.data) == TRUE);
	TEST_ASSERT(ntt_IsIDValid(&clone) == FALSE);
}

TEST_CASE(UpdateIDWithInvalidIDTest)
{
	ID		   invalid		= INVALID_ID;
	ntt_Result updateResult = ntt_UpdateID(&invalid);
	TEST_ASSERT(updateResult == NTT_RESULT_UPDATE_INVALID_ID);
	TEST_ASSERT(ntt_IsIDValid(&invalid) == FALSE);
}

TEST_CASE(UpdateOutdatedID)
{
	IDResult result = ntt_NewID(NTT_OBJECT_TYPE_OBJECT_ID);
	TEST_ASSERT(result.result == NTT_RESULT_SUCCESS);

	ID clone = result.data;

	ntt_Result updateResult = ntt_UpdateID(&result.data);
	TEST_ASSERT(updateResult == NTT_RESULT_SUCCESS);

	updateResult = ntt_UpdateID(&clone);
	TEST_ASSERT(updateResult == NTT_RESULT_UPDATE_INVALID_ID);
	TEST_ASSERT(ntt_IsIDValid(&clone) == FALSE);
}

TEST_CASE(CheckingEqualityOfIDs)
{
	IDResult result1 = ntt_NewID(NTT_OBJECT_TYPE_OBJECT_ID);
	IDResult result2 = ntt_NewID(NTT_OBJECT_TYPE_OBJECT_ID);

	TEST_ASSERT(ntt_IsIDValid(&result1.data) == TRUE);
	TEST_ASSERT(ntt_IsIDValid(&result2.data) == TRUE);
	TEST_ASSERT(ntt_IsIDEqual(&result1.data, &result2.data) == FALSE);

	ID clone1 = result1.data;
	ID clone2 = result2.data;

	TEST_ASSERT(ntt_IsIDEqual(&result1.data, &clone1) == TRUE);
	TEST_ASSERT(ntt_IsIDEqual(&result2.data, &clone2) == TRUE);
	TEST_ASSERT(ntt_IsIDEqual(&clone1, &clone2) == FALSE);
}

TEST_SUITE_DEFINE(id,
				  id_tests_before_each,
				  id_tests_after_each,
				  TEST_CASE_DECLARE(CreateIDTest),
				  TEST_CASE_DECLARE(ExceedMaxObjectsTest),
				  TEST_CASE_DECLARE(InvalidIDTest),
				  TEST_CASE_DECLARE(UpdateIDTest),
				  TEST_CASE_DECLARE(UpdateOutdatedID),
				  TEST_CASE_DECLARE(UpdateIDWithInvalidIDTest),
				  TEST_CASE_DECLARE(CheckingEqualityOfIDs))

#endif /* _ID_TESTS_H_ */