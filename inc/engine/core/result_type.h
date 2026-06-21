#ifndef _RESULT_TYPE_
#define _RESULT_TYPE_

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
#endif /* _RESULT_TYPE_ */