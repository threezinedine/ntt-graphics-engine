#ifndef _MAT_H_
#define _MAT_H_

#include "engine/core/types.h"
#include "vec.h"

#define MAT_DECLARE(rows, cols, type, abbrev)                                                                          \
	typedef union {                                                                                                    \
		struct                                                                                                         \
		{                                                                                                              \
			type data[rows][cols];                                                                                     \
		};                                                                                                             \
		type flatData[rows * cols];                                                                                    \
	} ntt_Mat##rows##x##cols##abbrev;                                                                                  \
	typedef ntt_Mat##rows##x##cols##abbrev ntt_Mat##rows##x##cols##abbrev;

#define MAT_OPS_DECLARE(rows, cols, abbrev)                                                                            \
	ntt_Mat##rows##x##cols##abbrev ntt_Mat##rows##x##cols##abbrev##_Add(ntt_Mat##rows##x##cols##abbrev a,              \
																		ntt_Mat##rows##x##cols##abbrev b);             \
	ntt_Mat##rows##x##cols##abbrev ntt_Mat##rows##x##cols##abbrev##_Sub(ntt_Mat##rows##x##cols##abbrev a,              \
																		ntt_Mat##rows##x##cols##abbrev b);             \
	ntt_Mat##rows##x##cols##abbrev ntt_Mat##rows##x##cols##abbrev##_Mul(ntt_Mat##rows##x##cols##abbrev a,              \
																		ntt_Mat##rows##x##cols##abbrev b);             \
	ntt_Mat##rows##x##cols##abbrev ntt_Mat##rows##x##cols##abbrev##_Div(ntt_Mat##rows##x##cols##abbrev a,              \
																		ntt_Mat##rows##x##cols##abbrev b);             \
	ntt_Mat##rows##x##rows##abbrev ntt_Mat##rows##x##cols##abbrev##_Dot(ntt_Mat##rows##x##cols##abbrev a,              \
																		ntt_Mat##cols##x##rows##abbrev b);             \
	ntt_Vec##rows##abbrev		   ntt_Mat##rows##x##cols##abbrev##_DotVec(ntt_Mat##rows##x##cols##abbrev mat,         \
																   ntt_Vec##cols##abbrev		  vec);

MAT_DECLARE(2, 2, f32, f)
MAT_DECLARE(2, 3, f32, f)
MAT_DECLARE(3, 2, f32, f)
MAT_DECLARE(3, 3, f32, f)
MAT_DECLARE(4, 4, f32, f)

MAT_DECLARE(2, 2, f64, d)
MAT_DECLARE(2, 3, f64, d)
MAT_DECLARE(3, 2, f64, d)
MAT_DECLARE(3, 3, f64, d)
MAT_DECLARE(4, 4, f64, d)

MAT_DECLARE(2, 2, i32, i)
MAT_DECLARE(2, 3, i32, i)
MAT_DECLARE(3, 2, i32, i)
MAT_DECLARE(3, 3, i32, i)
MAT_DECLARE(4, 4, i32, i)

MAT_DECLARE(2, 2, u32, u)
MAT_DECLARE(2, 3, u32, u)
MAT_DECLARE(3, 2, u32, u)
MAT_DECLARE(3, 3, u32, u)
MAT_DECLARE(4, 4, u32, u)

MAT_OPS_DECLARE(2, 2, f)
MAT_OPS_DECLARE(2, 3, f)
MAT_OPS_DECLARE(3, 2, f)
MAT_OPS_DECLARE(3, 3, f)
MAT_OPS_DECLARE(4, 4, f)

MAT_OPS_DECLARE(2, 2, d)
MAT_OPS_DECLARE(2, 3, d)
MAT_OPS_DECLARE(3, 2, d)
MAT_OPS_DECLARE(3, 3, d)
MAT_OPS_DECLARE(4, 4, d)

MAT_OPS_DECLARE(2, 2, i)
MAT_OPS_DECLARE(2, 3, i)
MAT_OPS_DECLARE(3, 2, i)
MAT_OPS_DECLARE(3, 3, i)
MAT_OPS_DECLARE(4, 4, i)

MAT_OPS_DECLARE(2, 2, u)
MAT_OPS_DECLARE(2, 3, u)
MAT_OPS_DECLARE(3, 2, u)
MAT_OPS_DECLARE(3, 3, u)
MAT_OPS_DECLARE(4, 4, u)

#endif /* _MAT_H_ */