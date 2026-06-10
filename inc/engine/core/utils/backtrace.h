#ifndef _DEBUG_INFO_H_
#define _DEBUG_INFO_H_

#include "engine/core/types.h"

#define MAX_CALLSTACK_DEPTH 128

#if NTT_DEBUG
/**
 * Show all the current call stack information into the console.
 */
void ntt_PrintCallStack();

#endif /* NTT_DEBUG */

#endif /* _DEBUG_INFO_H_ */
