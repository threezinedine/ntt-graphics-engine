#include "engine/core/utils/debug_info.h"
#include "engine/core/common.h"
#include "engine/core/utils/console.h"
#include "engine/core/utils/file.h"
#include "engine/core/utils/time.h"
#include "stdlib.h"
#include <stdio.h>
#include <string.h>

#if NTT_DEBUG
#if NTT_PLATFORM_UNIX
#include <execinfo.h>
#include <stdint.h>
#endif /* NTT_PLATFORM_UNIX */
#endif

#if NTT_DEBUG
void ntt_PrintCallStack()
{
#if NTT_PLATFORM_UNIX

	void* callstack[128];
	int	  frames = backtrace(callstack, 128);
	void* offsets[128];

#if 1
	char** strs = backtrace_symbols(callstack, frames);
	for (int i = 0; i < frames; i++)
	{
		char cmd[256];
		snprintf(cmd, sizeof(cmd), "addr2line -e %s -a -piCf %p", g_argv[0], callstack[i]);
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
#endif

#elif NTT_PLATFORM_WINDOWS
#error "Windows platform is not supported yet."
#else
#error "Unknown platform."
#endif /* NTT_PLATFORM_UNIX */
}

#endif /* NTT_DEBUG */