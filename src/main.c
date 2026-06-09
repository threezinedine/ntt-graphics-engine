#include "engine/engine.h"
#include <stdio.h>

int main(void)
{
	struct ntt_Allocator* allocator = ntt_CreateMallocAllocator();
	void*				  ptr		= ntt_Allocate(allocator, 128);
	ntt_Deallocate(allocator, ptr, 128);
	ntt_DestroyAllocator(allocator);

	return 0;
}