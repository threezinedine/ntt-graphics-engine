#ifndef _RESULT_H_
#define _RESULT_H_

#include "defs.h"
#include "engine/core/types.h"
#include "result_type.h"

const char* ntt_ResultToString(ntt_Result result);

#define DEFINE_RETURN_RESULT_TYPE(type)                                                                                \
	typedef struct type##Result                                                                                        \
	{                                                                                                                  \
		ntt_Result result;                                                                                             \
		type	   data;                                                                                               \
	} type##Result;                                                                                                    \
	typedef struct type##PtrResult                                                                                     \
	{                                                                                                                  \
		ntt_Result result;                                                                                             \
		type*	   pData;                                                                                              \
	} type##PtrResult;

DEFINE_RETURN_RESULT_TYPE(u64)
DEFINE_RETURN_RESULT_TYPE(u32)
DEFINE_RETURN_RESULT_TYPE(u16)
DEFINE_RETURN_RESULT_TYPE(u8)
DEFINE_RETURN_RESULT_TYPE(i64)
DEFINE_RETURN_RESULT_TYPE(i32)
DEFINE_RETURN_RESULT_TYPE(i16)
DEFINE_RETURN_RESULT_TYPE(i8)
DEFINE_RETURN_RESULT_TYPE(f64)
DEFINE_RETURN_RESULT_TYPE(f32)
DEFINE_RETURN_RESULT_TYPE(b8)
DEFINE_RETURN_RESULT_TYPE(usize)

#if NTT_DEBUG && NTT_ENABLE_ASSERT
#define NTT_SUCCESS_ASSERT(express)                                                                                    \
	do                                                                                                                 \
	{                                                                                                                  \
		ntt_Result _result = (express);                                                                                 \
		NTT_ASSERT(_result == NTT_RESULT_SUCCESS);                                                                      \
	} while (0)

#define NTT_SUCCESS_ASSERT_VAR(res) NTT_ASSERT(res.result == NTT_RESULT_SUCCESS);

#else /* NTT_DEBUG */
#define NTT_SUCCESS_ASSERT(express)                                                                                    \
	do                                                                                                                 \
	{                                                                                                                  \
		ntt_Result _result = (express);                                                                                 \
		if (_result != NTT_RESULT_SUCCESS)                                                                              \
		{                                                                                                              \
			return _result;                                                                                             \
		}                                                                                                              \
	} while (0)

#define NTT_SUCCESS_ASSERT_VAR(res)                                                                                    \
	do                                                                                                                 \
	{                                                                                                                  \
		if (res.result != NTT_RESULT_SUCCESS)                                                                          \
		{                                                                                                              \
			return res.result;                                                                                         \
		}                                                                                                              \
	} while (0)
#endif /* NTT_DEBUG */

#define NTT_ASSERT_IF(exp)                                                                                             \
	NTT_ASSERT(!(exp));                                                                                                \
	if ((exp))

#define NTT_ASSERT_IF_M(exp, msg, ...)                                                                                 \
	NTT_ASSERT_M(!(exp), msg, ##__VA_ARGS__);                                                                          \
	if ((exp))

#endif /* _RESULT_H_ */