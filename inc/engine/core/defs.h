#ifndef _DEFS_H_
#define _DEFS_H_

/**
 * Used for bypassing unused variable warnings
 *
 * @example
 * ```c
 * void myFunction(int unusedParam)
 * {
 * 		NTT_UNUSED(unusedParam);
 * 		// Function implementation...
 * }
 * ```
 */
#define NTT_UNUSED(x) (void)(x)

#if NTT_DEBUG && NTT_ENABLE_ASSERT
#include "engine/core/utils/utils.h"
#if NTT_PLATFORM_UNIX
#include <stdlib.h>

#if defined(__clang__) || defined(__GNUC__)
#define NTT_DEBUG_BREAK() __builtin_trap()
#else
#define NTT_DEBUG_BREAK() abort()
#endif
#elif NTT_PLATFORM_WINDOWS /* NTT_PLATFORM_UNIX */
#define NTT_DEBUG_BREAK() __debugbreak()
#else /* NTT_PLATFORM_UNIX*/
#error "Unknown platform."
#endif /* NTT_PLATFORM_UNIX */

/**
 * **Runtime** checking for a certain condition.
 * If the condition is false, it prints an error message with the condition, file name, and line number, then prints the
 * 		call stack and breaks into the debugger.
 */
#define NTT_ASSERT(cond)                                                                                               \
	do                                                                                                                 \
	{                                                                                                                  \
		if (!(cond))                                                                                                   \
		{                                                                                                              \
			ntt_ConsoleSetColor(NTT_COLOR_RED);                                                                        \
			ntt_ConsolePrint("Assertion failed: %s, file: %s, line: %d\n", #cond, __FILE__, __LINE__);                 \
			ntt_PrintCallStack(NULL);                                                                                  \
			ntt_ConsoleResetColor();                                                                                   \
			NTT_DEBUG_BREAK();                                                                                         \
		}                                                                                                              \
	} while (0)

#define NTT_ASSERT_M(cond, message, ...)                                                                               \
	do                                                                                                                 \
	{                                                                                                                  \
		if (!(cond))                                                                                                   \
		{                                                                                                              \
			char assertMessage[512];                                                                                   \
			ntt_FormatMessage(assertMessage, sizeof(assertMessage), message, ##__VA_ARGS__);                           \
			char finalMessage[512];                                                                                    \
			ntt_FormatMessage(finalMessage,                                                                            \
							  sizeof(finalMessage),                                                                    \
							  "Assertion failed: %s, file: %s, line: %d\nMessage: %s\n",                               \
							  #cond,                                                                                   \
							  __FILE__,                                                                                \
							  __LINE__,                                                                                \
							  assertMessage);                                                                          \
			ntt_ConsoleSetColor(NTT_COLOR_RED);                                                                        \
			ntt_ConsolePrint("%s", finalMessage);                                                                      \
			ntt_PrintCallStack(NULL);                                                                                  \
			ntt_ConsoleResetColor();                                                                                   \
			NTT_DEBUG_BREAK();                                                                                         \
		}                                                                                                              \
	} while (0)

#else /* NTT_DEBUG */
#define NTT_ASSERT(cond)				 ((void)0)
#define NTT_ASSERT_M(cond, message, ...) ((void)0)
#endif /* NTT_DEBUG */

#endif /* _DEFS_H_ */