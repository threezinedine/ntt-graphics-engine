#include "engine/core/containers/vec.h"

#define VEC_DEFINE(count, type, abbrev)                                                                                \
	ntt_Vec##count##abbrev ntt_Vec##count##abbrev##_Add(ntt_Vec##count##abbrev a, ntt_Vec##count##abbrev b)            \
	{                                                                                                                  \
		ntt_Vec##count##abbrev result = {0};                                                                           \
		for (size_t i = 0; i < count; i++)                                                                             \
		{                                                                                                              \
			result.data[i] = a.data[i] + b.data[i];                                                                    \
		}                                                                                                              \
		return result;                                                                                                 \
	}                                                                                                                  \
	ntt_Vec##count##abbrev ntt_Vec##count##abbrev##_Sub(ntt_Vec##count##abbrev a, ntt_Vec##count##abbrev b)            \
	{                                                                                                                  \
		ntt_Vec##count##abbrev result = {0};                                                                           \
		for (size_t i = 0; i < count; i++)                                                                             \
		{                                                                                                              \
			result.data[i] = a.data[i] - b.data[i];                                                                    \
		}                                                                                                              \
		return result;                                                                                                 \
	}                                                                                                                  \
	ntt_Vec##count##abbrev ntt_Vec##count##abbrev##_Mul(ntt_Vec##count##abbrev a, ntt_Vec##count##abbrev b)            \
	{                                                                                                                  \
		ntt_Vec##count##abbrev result = {0};                                                                           \
		for (size_t i = 0; i < count; i++)                                                                             \
		{                                                                                                              \
			result.data[i] = a.data[i] * b.data[i];                                                                    \
		}                                                                                                              \
		return result;                                                                                                 \
	}                                                                                                                  \
	ntt_Vec##count##abbrev ntt_Vec##count##abbrev##_Div(ntt_Vec##count##abbrev a, ntt_Vec##count##abbrev b)            \
	{                                                                                                                  \
		ntt_Vec##count##abbrev result = {0};                                                                           \
		for (size_t i = 0; i < count; i++)                                                                             \
		{                                                                                                              \
			result.data[i] = a.data[i] / b.data[i];                                                                    \
		}                                                                                                              \
		return result;                                                                                                 \
	}

VEC_DEFINE(2, f32, f)
VEC_DEFINE(2, f64, d)
VEC_DEFINE(2, i32, i)
VEC_DEFINE(2, u32, u)

VEC_DEFINE(3, f32, f)
VEC_DEFINE(3, f64, d)
VEC_DEFINE(3, i32, i)
VEC_DEFINE(3, u32, u)

VEC_DEFINE(4, f32, f)
VEC_DEFINE(4, f64, d)
VEC_DEFINE(4, i32, i)
VEC_DEFINE(4, u32, u)