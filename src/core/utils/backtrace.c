#include "engine/core/utils/backtrace.h"
#include "engine/core/common.h"
#include "engine/core/utils/console.h"
#include "engine/core/defs.h"
#include "engine/core/utils/file.h"
#include "engine/core/utils/time.h"
#include "stdlib.h"
#include <stdio.h>
#include <string.h>

#if NTT_DEBUG
#if NTT_PLATFORM_UNIX
#include <execinfo.h>
#include <stdint.h>
#elif NTT_PLATFORM_WINDOWS /* NTT_PLATFORM_UNIX */
#include <windows.h>
#endif /* NTT_PLATFORM_UNIX */
#endif

#if NTT_DEBUG
void ntt_PrintCallStack(struct ntt_BacktraceInfo* pInfo)
{
#if NTT_PLATFORM_UNIX
	if (pInfo == NULL)
	{
		struct ntt_BacktraceInfo info = ntt_CaptureCallStack();
		pInfo						  = &info;
	}

	char** strs = backtrace_symbols(pInfo->backtraces, pInfo->frames);
	for (int i = 0; i < pInfo->frames; i++)
	{
		char cmd[256];
		snprintf(cmd, sizeof(cmd), "addr2line -e %s -a -piCf %p", g_argv[0], pInfo->backtraces[i]);
		FILE* fp = popen(cmd, "r");
		if (fp)
		{
			char response[1024];
			while (fgets(response, sizeof(response), fp))
			{
				printf("     -> %s", response);
			}
			pclose(fp);
		}
	}
	free(strs);

#elif NTT_PLATFORM_WINDOWS
	NTT_UNUSED(pInfo);
#else
#error "Unknown platform."
#endif /* NTT_PLATFORM_UNIX */
}

struct ntt_BacktraceInfo ntt_CaptureCallStack()
{
	struct ntt_BacktraceInfo info = {0};
#if NTT_PLATFORM_UNIX
	info.frames = backtrace(info.backtraces, MAX_CALLSTACK_DEPTH);
#elif NTT_PLATFORM_WINDOWS /* NTT_PLATFORM_UNIX */
	info.frames = (int)CaptureStackBackTrace(0, MAX_CALLSTACK_DEPTH, info.backtraces, NULL);
#else
#error "Unknown platform."
#endif /* NTT_PLATFORM_UNIX */
	return info;
}

#endif /* NTT_DEBUG */