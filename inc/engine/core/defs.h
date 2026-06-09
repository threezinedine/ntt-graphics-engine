#ifndef _DEFS_H_
#define _DEFS_H_

#if NTT_DEBUG
#if NTT_PLATFORM_UNIX
#include <builtins.h>

#define NTT_ASSERT(cond)                                                                                               \
	do                                                                                                                 \
	{                                                                                                                  \
		if (!(cond))                                                                                                   \
		{                                                                                                              \
			__builtin_trap();                                                                                          \
		}                                                                                                              \
	} while (0)

#elif NTT_PLATFORM_WINDOWS
#error "Windows platform is not supported yet."
#else
#error "Unknown platform."
#endif
#else
#define NTT_ASSERT(cond) ((void)0)
#endif

#endif /* _DEFS_H_ */