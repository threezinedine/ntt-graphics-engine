#ifndef _DEBUG_INFO_H_
#define _DEBUG_INFO_H_

#include "engine/core/types.h"

#define MAX_CALLSTACK_DEPTH 12

#if NTT_DEBUG
typedef struct ntt_BacktraceInfo
{
	void* backtraces[MAX_CALLSTACK_DEPTH];
	int	  frames;
} ntt_BacktraceInfo;

/**
 * Show all the current call stack information into the console.
 *
 * @param pInfo Optional pointer to a `ntt_BacktraceInfo` struct to show in the console.
 * If NULL, the function will show the current call stack information by itself.
 */
void ntt_PrintCallStack(ntt_BacktraceInfo* pInfo);

/**
 * Obtains the current call stack information and returns it as a `ntt_BacktraceInfo` struct.
 *
 * @return A `ntt_BacktraceInfo` struct containing the current call stack information.
 */
ntt_BacktraceInfo ntt_CaptureCallStack();

#endif /* NTT_DEBUG */

#endif /* _DEBUG_INFO_H_ */
