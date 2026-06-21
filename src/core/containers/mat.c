#include "engine/core/containers/mat.h"

// TODO: Implement SIMD optimizations for matrix operations

#define MAT_DEFINE(rows, cols, type, abbrev)                                                                           \
	ntt_Mat##rows##x##cols##abbrev ntt_Mat##rows##x##cols##abbrev##_Add(ntt_Mat##rows##x##cols##abbrev a,              \
																		ntt_Mat##rows##x##cols##abbrev b)              \
	{                                                                                                                  \
		ntt_Mat##rows##x##cols##abbrev result = {0};                                                                   \
		for (size_t i = 0; i < rows; i++)                                                                              \
		{                                                                                                              \
			for (size_t j = 0; j < cols; j++)                                                                          \
			{                                                                                                          \
				result.data[i][j] = a.data[i][j] + b.data[i][j];                                                       \
			}                                                                                                          \
		}                                                                                                              \
		return result;                                                                                                 \
	}                                                                                                                  \
	ntt_Mat##rows##x##cols##abbrev ntt_Mat##rows##x##cols##abbrev##_Sub(ntt_Mat##rows##x##cols##abbrev a,              \
																		ntt_Mat##rows##x##cols##abbrev b)              \
	{                                                                                                                  \
		ntt_Mat##rows##x##cols##abbrev result = {0};                                                                   \
		for (size_t i = 0; i < rows; i++)                                                                              \
		{                                                                                                              \
			for (size_t j = 0; j < cols; j++)                                                                          \
			{                                                                                                          \
				result.data[i][j] = a.data[i][j] - b.data[i][j];                                                       \
			}                                                                                                          \
		}                                                                                                              \
		return result;                                                                                                 \
	}                                                                                                                  \
	ntt_Mat##rows##x##cols##abbrev ntt_Mat##rows##x##cols##abbrev##_Mul(ntt_Mat##rows##x##cols##abbrev a,              \
																		ntt_Mat##rows##x##cols##abbrev b)              \
	{                                                                                                                  \
		ntt_Mat##rows##x##cols##abbrev result = {0};                                                                   \
		for (size_t i = 0; i < rows; i++)                                                                              \
		{                                                                                                              \
			for (size_t j = 0; j < cols; j++)                                                                          \
			{                                                                                                          \
				result.data[i][j] = a.data[i][j] * b.data[i][j];                                                       \
			}                                                                                                          \
		}                                                                                                              \
		return result;                                                                                                 \
	}                                                                                                                  \
	ntt_Mat##rows##x##cols##abbrev ntt_Mat##rows##x##cols##abbrev##_Div(ntt_Mat##rows##x##cols##abbrev a,              \
																		ntt_Mat##rows##x##cols##abbrev b)              \
	{                                                                                                                  \
		ntt_Mat##rows##x##cols##abbrev result = {0};                                                                   \
		for (size_t i = 0; i < rows; i++)                                                                              \
		{                                                                                                              \
			for (size_t j = 0; j < cols; j++)                                                                          \
			{                                                                                                          \
				result.data[i][j] = a.data[i][j] / b.data[i][j];                                                       \
			}                                                                                                          \
		}                                                                                                              \
		return result;                                                                                                 \
	}                                                                                                                  \
	ntt_Vec##rows##abbrev ntt_Mat##rows##x##cols##abbrev##_DotVec(ntt_Mat##rows##x##cols##abbrev mat,                  \
																  ntt_Vec##cols##abbrev			 vec)                  \
	{                                                                                                                  \
		ntt_Vec##rows##abbrev result = {0};                                                                            \
		for (size_t i = 0; i < rows; i++)                                                                              \
		{                                                                                                              \
			for (size_t j = 0; j < cols; j++)                                                                          \
			{                                                                                                          \
				result.data[i] += mat.data[i][j] * vec.data[j];                                                        \
			}                                                                                                          \
		}                                                                                                              \
		return result;                                                                                                 \
	}                                                                                                                  \
	ntt_Mat##rows##x##rows##abbrev ntt_Mat##rows##x##cols##abbrev##_Dot(ntt_Mat##rows##x##cols##abbrev a,              \
																		ntt_Mat##cols##x##rows##abbrev b)              \
	{                                                                                                                  \
		ntt_Mat##rows##x##rows##abbrev result = {0};                                                                   \
		for (size_t i = 0; i < rows; i++)                                                                              \
		{                                                                                                              \
			for (size_t j = 0; j < rows; j++)                                                                          \
			{                                                                                                          \
				for (size_t k = 0; k < cols; k++)                                                                      \
				{                                                                                                      \
					result.data[i][j] += a.data[i][k] * b.data[k][j];                                                  \
				}                                                                                                      \
			}                                                                                                          \
		}                                                                                                              \
		return result;                                                                                                 \
	}

MAT_DEFINE(2, 2, f32, f)
MAT_DEFINE(2, 3, f32, f)
MAT_DEFINE(3, 2, f32, f)
MAT_DEFINE(3, 3, f32, f)
MAT_DEFINE(4, 4, f32, f)

MAT_DEFINE(2, 2, f64, d)
MAT_DEFINE(2, 3, f64, d)
MAT_DEFINE(3, 2, f64, d)
MAT_DEFINE(3, 3, f64, d)
MAT_DEFINE(4, 4, f64, d)

MAT_DEFINE(2, 2, i32, i)
MAT_DEFINE(2, 3, i32, i)
MAT_DEFINE(3, 2, i32, i)
MAT_DEFINE(3, 3, i32, i)
MAT_DEFINE(4, 4, i32, i)

MAT_DEFINE(2, 2, u32, u)
MAT_DEFINE(2, 3, u32, u)
MAT_DEFINE(3, 2, u32, u)
MAT_DEFINE(3, 3, u32, u)
MAT_DEFINE(4, 4, u32, u)
