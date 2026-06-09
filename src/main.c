#include "engine/engine.h"
#include <stdio.h>

int main(void)
{
	struct ntt_Allocator* allocator = ntt_CreateMallocAllocator();
	void*				  ptr		= ntt_Allocate(allocator, 128);
	ntt_Deallocate(allocator, ptr, 128);
	ntt_DestroyAllocator(allocator);

	ntt_ConsoleSetColor(NTT_COLOR_GREEN);
	ntt_ConsolePrint("Hello, World!\n");
	ntt_ConsoleResetColor();
	ntt_ConsolePrint("This is a graphics engine.\n");

	NTT_ASSERT(3 == 4);

	return 0;
}