#include "engine/engine.h"
#include <stdio.h>

void test(int a)
{
	NTT_UNUSED(a);
	NTT_ASSERT(1 == 2);
}

int	   g_argc;
char** g_argv;

int main(i32 argc, char** argv)
{
	g_argc = argc;
	g_argv = argv;

	ntt_ConsolePrint("%s\n", g_argv[0]);

	struct ntt_Allocator* allocator = ntt_CreateMallocAllocator();
	void*				  ptr		= ntt_Allocate(allocator, 128);
	ntt_Deallocate(allocator, ptr, 128);

	ntt_Allocate(allocator, 256);

	ntt_ConsoleSetColor(NTT_COLOR_GREEN);
	ntt_ConsolePrint("Hello, World!\n");
	ntt_ConsoleResetColor();
	ntt_ConsolePrint("This is a graphics engine.\n");

	// test(0);
	ntt_ConsolePrint("%s\n", ntt_ColorToString(NTT_COLOR_RED));

	ntt_DestroyAllocator(allocator);

	return 0;
}