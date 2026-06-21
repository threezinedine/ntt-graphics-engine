#include "engine/engine.h"

int	   g_argc;
char** g_argv;

int main(i32 argc, char** argv)
{
	g_argc = argc;
	g_argv = argv;

	ntt_Application application = {0};

	NTT_SUCCESS_ASSERT(ntt_Application_Initialize(&application));
	NTT_SUCCESS_ASSERT(ntt_Application_Run(&application));
	NTT_SUCCESS_ASSERT(ntt_Application_Shutdown(&application));

	return 0;
}