#ifndef _MAT_TESTS_H_
#define _MAT_TESTS_H_

#include "engine/engine.h"
#include "tools/tools.h"

void mat_before_each()
{
}

void mat_after_each()
{
}

TEST_CASE(Mat2x2f_Add)
{
	ntt_Mat2x2f a = {
		.data = {{1.0f, 2.0f}, {3.0f, 4.0f}}
	};
	ntt_Mat2x2f b = {
		.data = {{5.0f, 6.0f}, {7.0f, 8.0f}}
	};

	ntt_Mat2x2f result = ntt_Mat2x2f_Add(a, b);

	TEST_ASSERT(result.data[0][0] == 6.0f);
	TEST_ASSERT(result.data[0][1] == 8.0f);
	TEST_ASSERT(result.data[1][0] == 10.0f);
	TEST_ASSERT(result.data[1][1] == 12.0f);
}

TEST_CASE(Mat3x3f_Sub)
{
	ntt_Mat3x3f a = {
		.data = {{1.0f, 2.0f, 3.0f}, {4.0f, 5.0f, 6.0f}, {7.0f, 8.0f, 9.0f}}
	};
	ntt_Mat3x3f b = {
		.data = {{9.0f, 8.0f, 7.0f}, {6.0f, 5.0f, 4.0f}, {3.0f, 2.0f, 1.0f}}
	};

	ntt_Mat3x3f result = ntt_Mat3x3f_Sub(a, b);

	TEST_ASSERT(result.data[0][0] == -8.0f);
	TEST_ASSERT(result.data[0][1] == -6.0f);
	TEST_ASSERT(result.data[0][2] == -4.0f);
	TEST_ASSERT(result.data[1][0] == -2.0f);
	TEST_ASSERT(result.data[1][1] == 0.0f);
	TEST_ASSERT(result.data[1][2] == 2.0f);
	TEST_ASSERT(result.data[2][0] == 4.0f);
	TEST_ASSERT(result.data[2][1] == 6.0f);
	TEST_ASSERT(result.data[2][2] == 8.0f);
}

TEST_CASE(Mat2x3f_Mul)
{
	ntt_Mat2x3f a = {
		.data = {{1.0f, 2.0f, 3.0f}, {4.0f, 5.0f, 6.0f}}
	};
	ntt_Mat2x3f b = {
		.data = {{7.0f, 8.0f, 9.0f}, {10.0f, 11.0f, 12.0f}}
	   };

	ntt_Mat2x3f result = ntt_Mat2x3f_Mul(a, b);

	TEST_ASSERT(result.data[0][0] == 7.0f);
	TEST_ASSERT(result.data[0][1] == 16.0f);
	TEST_ASSERT(result.data[0][2] == 27.0f);
	TEST_ASSERT(result.data[1][0] == 40.0f);
	TEST_ASSERT(result.data[1][1] == 55.0f);
	TEST_ASSERT(result.data[1][2] == 72.0f);
}

TEST_CASE(Mat3x2F_Div)
{
	ntt_Mat3x2f a = {
		.data = {{1.0f, 2.0f}, {3.0f, 4.0f}, {5.0f, 6.0f}}
	  };
	ntt_Mat3x2f b = {
		.data = {{7.0f, 8.0f}, {9.0f, 10.0f}, {11.0f, 12.0f}}
	 };

	ntt_Mat3x2f result = ntt_Mat3x2f_Div(a, b);

	TEST_ASSERT(result.data[0][0] == 1.0f / 7.0f);
	TEST_ASSERT(result.data[0][1] == 2.0f / 8.0f);
	TEST_ASSERT(result.data[1][0] == 3.0f / 9.0f);
	TEST_ASSERT(result.data[1][1] == 4.0f / 10.0f);
	TEST_ASSERT(result.data[2][0] == 5.0f / 11.0f);
	TEST_ASSERT(result.data[2][1] == 6.0f / 12.0f);
}

TEST_SUITE_DEFINE(mat,
				  mat_before_each,
				  mat_after_each,
				  TEST_CASE_DECLARE(Mat2x2f_Add),
				  TEST_CASE_DECLARE(Mat3x3f_Sub),
				  TEST_CASE_DECLARE(Mat2x3f_Mul),
				  TEST_CASE_DECLARE(Mat3x2F_Div))

#endif /* _MAT_TESTS_H_ */