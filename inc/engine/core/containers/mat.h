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
	typedef ntt_Mat##rows##x##cols##abbrev ntt_Mat##rows##x##cols##abbrev;                                             \
	ntt_Mat##rows##x##cols##abbrev		   ntt_Mat##rows##x##cols##abbrev##_Add(ntt_Mat##rows##x##cols##abbrev a,      \
																		ntt_Mat##rows##x##cols##abbrev b);     \
	ntt_Mat##rows##x##cols##abbrev		   ntt_Mat##rows##x##cols##abbrev##_Sub(ntt_Mat##rows##x##cols##abbrev a,      \
																		ntt_Mat##rows##x##cols##abbrev b);     \
	ntt_Mat##rows##x##cols##abbrev		   ntt_Mat##rows##x##cols##abbrev##_Mul(ntt_Mat##rows##x##cols##abbrev a,      \
																		ntt_Mat##rows##x##cols##abbrev b);     \
	ntt_Mat##rows##x##cols##abbrev		   ntt_Mat##rows##x##cols##abbrev##_Div(ntt_Mat##rows##x##cols##abbrev a,      \
																		ntt_Mat##rows##x##cols##abbrev b);     \
	ntt_Mat##rows##x##cols##abbrev		   ntt_Mat##rows##x##cols##abbrev##_Dot(ntt_Mat##rows##x##cols##abbrev a,      \
																		ntt_Mat##rows##x##cols##abbrev b);     \
	ntt_Vec##cols##abbrev				   ntt_Mat##rows##x##cols##abbrev##_DotVec(ntt_Mat##rows##x##cols##abbrev mat, \
																   ntt_Vec##cols##abbrev		  vec);

MAT_DECLARE(2, 2, f32, f)
MAT_DECLARE(2, 3, f32, f)
MAT_DECLARE(3, 2, f32, f)
MAT_DECLARE(3, 3, f32, f)
MAT_DECLARE(4, 4, f32, f)

#endif /* _MAT_H_ */