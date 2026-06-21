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

TEST_CASE(Mat4x4i_DotVec)
{
	ntt_Mat4x4f mat = {
		.data = {{1.0f, 2.0f, 3.0f, 4.0f},
				 {5.0f, 6.0f, 7.0f, 8.0f},
				 {9.0f, 10.0f, 11.0f, 12.0f},
				 {13.0f, 14.0f, 15.0f, 16.0f}}
	  };
	ntt_Vec4f vec = {
		.data = {1.0f, 2.0f, 3.0f, 4.0f}
	};

	ntt_Vec4f result = ntt_Mat4x4f_DotVec(mat, vec);

	TEST_ASSERT(result.data[0] == 30.0f);
	TEST_ASSERT(result.data[1] == 70.0f);
	TEST_ASSERT(result.data[2] == 110.0f);
	TEST_ASSERT(result.data[3] == 150.0f);
}

TEST_CASE(Mat3x2i_DotVec)
{
	ntt_Mat3x2i mat = {
		.data = {{1, 2}, {3, 4}, {5, 6}}
	};
	ntt_Vec2i vec = {
		.data = {7, 8}
	  };

	ntt_Vec3i result = ntt_Mat3x2i_DotVec(mat, vec);

	TEST_ASSERT(result.data[0] == 23);
	TEST_ASSERT(result.data[1] == 53);
	TEST_ASSERT(result.data[2] == 83);
}

TEST_CASE(Mat3x3u_Dot)
{
	ntt_Mat3x3u a = {
		.data = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}}
	 };
	ntt_Mat3x3u result = ntt_Mat3x3u_Dot(a, a);

	TEST_ASSERT(result.data[0][0] == 30);
	TEST_ASSERT(result.data[0][1] == 36);
	TEST_ASSERT(result.data[0][2] == 42);
	TEST_ASSERT(result.data[1][0] == 66);
	TEST_ASSERT(result.data[1][1] == 81);
	TEST_ASSERT(result.data[1][2] == 96);
	TEST_ASSERT(result.data[2][0] == 102);
	TEST_ASSERT(result.data[2][1] == 126);
	TEST_ASSERT(result.data[2][2] == 150);
}

TEST_CASE(ntt_Mat2x3i_Dot)
{
	ntt_Mat2x3i a = {
		.data = {{7, 8, 9}, {10, 11, 12}}
	 };
	ntt_Mat3x2i b = {
		.data = {{1, 2}, {3, 4}, {5, 6}}
	};

	ntt_Mat2x2i result = ntt_Mat2x3i_Dot(a, b);

	TEST_ASSERT(result.data[0][0] == 76);
	TEST_ASSERT(result.data[0][1] == 100);
	TEST_ASSERT(result.data[1][0] == 103);
	TEST_ASSERT(result.data[1][1] == 136);
}

TEST_SUITE_DEFINE(mat,
				  mat_before_each,
				  mat_after_each,
				  TEST_CASE_DECLARE(Mat2x2f_Add),
				  TEST_CASE_DECLARE(Mat3x3f_Sub),
				  TEST_CASE_DECLARE(Mat2x3f_Mul),
				  TEST_CASE_DECLARE(Mat3x2F_Div),
				  TEST_CASE_DECLARE(Mat3x3u_Dot),
				  TEST_CASE_DECLARE(ntt_Mat2x3i_Dot),
				  TEST_CASE_DECLARE(Mat3x2i_DotVec),
				  TEST_CASE_DECLARE(Mat4x4i_DotVec))

#endif /* _MAT_TESTS_H_ */