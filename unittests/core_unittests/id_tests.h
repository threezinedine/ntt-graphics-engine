#ifndef _ID_TESTS_H_
#define _ID_TESTS_H_

#include "tools/tools.h"

TEST_CASE(ExampleTest)
{
	TEST_ASSERT(1 == 1);
}

TEST_CASE(ExampleTest2)
{
	TEST_ASSERT(2 == 3);
}

TEST_SUITE_DEFINE(id_tests, TEST_CASE_DECLARE(ExampleTest), TEST_CASE_DECLARE(ExampleTest2))

#endif /* _ID_TESTS_H_ */