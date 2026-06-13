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

	ntt_InitializeIDSystem();
	ntt_ConsolePrint("%s\n", g_argv[0]);

	struct ntt_Allocator* allocator = ntt_CreateMallocAllocator();
	void*				  ptr		= ntt_Allocate(allocator, 128);
	ntt_Deallocate(allocator, ptr, 128);

	void* testPtr = ntt_Allocate(allocator, 256);

	ntt_ConsoleSetColor(NTT_COLOR_GREEN);
	ntt_ConsolePrint("Hello, World!\n");
	ntt_ConsoleResetColor();
	ntt_ConsolePrint("This is a graphics engine.\n");

	ntt_Deallocate(allocator, testPtr, 200);
	//ntt_Deallocate(allocator, testPtr, 256);

	ID id  = ntt_NewID(NTT_OBJECT_TYPE_OBJECT);
	ID id2 = id;

	ntt_UpdateID(&id);

	ntt_ConsolePrint("id valid: '%s'\n", ntt_IsIDValid(&id) ? "true" : "false");
	ntt_ConsolePrint("id2 valid: '%s'\n", ntt_IsIDValid(&id2) ? "true" : "false");

	//test(0);
	ntt_ConsolePrint("%s\n", ntt_ColorToString(NTT_COLOR_RED));

	ntt_DestroyAllocator(allocator);
	ntt_DestroyIDSystem();

	return 0;
}