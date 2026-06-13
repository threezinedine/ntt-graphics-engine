#ifndef _RESULT_H_
#define _RESULT_H_

#include "defs.h"

/**
 * The methods inside the engine should return a result code to indicate the function status.
 */
enum ntt_Result
{
#define ENUM_OPTION(name)	  name,
#define ENUM_OPTION_END(name) name
#include "result.inl"
#undef ENUM_OPTION
#undef ENUM_OPTION_END
};

typedef enum ntt_Result ntt_Result;

const char* ntt_ResultToString(ntt_Result result);

#if NTT_DEBUG
#define NTT_SUCCESS_ASSERT(express)                                                                                    \
	do                                                                                                                 \
	{                                                                                                                  \
		ntt_Result result = (express);                                                                                 \
		NTT_ASSERT(result == NTT_RESULT_SUCCESS);                                                                      \
	} while (0)
#else /* NTT_DEBUG */
#define NTT_SUCCESS_ASSERT(express)                                                                                    \
	do                                                                                                                 \
	{                                                                                                                  \
		ntt_Result result = (express);                                                                                 \
		if (result != NTT_RESULT_SUCCESS)                                                                              \
		{                                                                                                              \
			return result;                                                                                             \
		}                                                                                                              \
	} while (0)
#endif /* NTT_DEBUG */

#endif /* _RESULT_H_ */