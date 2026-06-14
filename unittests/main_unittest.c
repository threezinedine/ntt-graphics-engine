#include "engine/engine.h"

int	   g_argc;
char** g_argv;

int main(i32 argc, char** argv)
{
	g_argc = argc;
	g_argv = argv;

	ntt_ConsolePrint("Hello, World!\n");

	return 0;
}