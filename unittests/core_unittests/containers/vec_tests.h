#ifndef _VEC_TESTS_H_
#define _VEC_TESTS_H_

#include "engine/engine.h"
#include "tools/tools.h"

void vec_before_each()
{
}

void vec_after_each()
{
}

TEST_CASE(Vec2f_Add)
{
	ntt_Vec2f a = {.x = 1.0f, .y = 2.0f};
	ntt_Vec2f b = {.x = 3.0f, .y = 4.0f};

	ntt_Vec2f result = ntt_Vec2f_Add(a, b);

	TEST_ASSERT(result.x == 4.0f);
	TEST_ASSERT(result.y == 6.0f);
}

TEST_CASE(Vec2d_Div)
{
	ntt_Vec2d a = {.x = 10.0, .y = 20.0};
	ntt_Vec2d b = {.x = 2.0, .y = 4.0};

	ntt_Vec2d result = ntt_Vec2d_Div(a, b);

	TEST_ASSERT(result.x == 5.0);
	TEST_ASSERT(result.y == 5.0);
}

TEST_CASE(Vec3i_Sub)
{
	ntt_Vec3i a = {.x = 5, .y = 7, .z = 9};
	ntt_Vec3i b = {.x = 2, .y = 3, .z = 4};

	ntt_Vec3i result = ntt_Vec3i_Sub(a, b);

	TEST_ASSERT(result.x == 3);
	TEST_ASSERT(result.y == 4);
	TEST_ASSERT(result.z == 5);
}

TEST_CASE(Vec4d_Mul)
{
	ntt_Vec4d a = {.x = 1.0, .y = 2.0, .z = 3.0, .w = 4.0};
	ntt_Vec4d b = {.x = 5.0, .y = 6.0, .z = 7.0, .w = 8.0};

	ntt_Vec4d result = ntt_Vec4d_Mul(a, b);

	TEST_ASSERT(result.x == 5.0);
	TEST_ASSERT(result.y == 12.0);
	TEST_ASSERT(result.z == 21.0);
	TEST_ASSERT(result.w == 32.0);
}

TEST_SUITE_DEFINE(vec,
				  vec_before_each,
				  vec_after_each,
				  TEST_CASE_DECLARE(Vec2f_Add),
				  TEST_CASE_DECLARE(Vec3i_Sub),
				  TEST_CASE_DECLARE(Vec4d_Mul))

#endif /* _VEC_TESTS_H_ */