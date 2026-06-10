#include "engine/engine.h"
#include <stdio.h>

void test()
{
	NTT_ASSERT(1 == 2);
}

int	   g_argc;
char** g_argv;

int main(u32 argc, char** argv)
{
	g_argc = argc;
	g_argv = argv;

	ntt_ConsolePrint("%s\n", g_argv[0]);

	struct ntt_Allocator* allocator = ntt_CreateMallocAllocator();
	void*				  ptr		= ntt_Allocate(allocator, 128);
	ntt_Deallocate(allocator, ptr, 128);
	ntt_DestroyAllocator(allocator);

	ntt_ConsoleSetColor(NTT_COLOR_GREEN);
	ntt_ConsolePrint("Hello, World!\n");
	ntt_ConsoleResetColor();
	ntt_ConsolePrint("This is a graphics engine.\n");

	test();
	ntt_ConsolePrint("%s\n", ntt_ColorToString(NTT_COLOR_RED));

	return 0;
}