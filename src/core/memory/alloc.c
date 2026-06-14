#include "engine/core/memory/alloc.h"
#include "engine/core/defs.h"
#include "engine/core/types.h"

voidPtrResult ntt_Allocate(ntt_Allocator* allocator, usize size)
{
	NTT_ASSERT(allocator != NULL);
	NTT_ASSERT(allocator->allocate != NULL);

	return allocator->allocate(allocator, size);
}

ntt_Result ntt_Deallocate(ntt_Allocator* allocator, void* ptr, usize size)
{
	NTT_ASSERT(allocator != NULL);
	NTT_ASSERT(allocator->deallocate != NULL);

	return allocator->deallocate(allocator, ptr, size);
}

ntt_Result ntt_DestroyAllocator(ntt_Allocator* allocator)
{
	NTT_ASSERT(allocator != NULL);
	NTT_ASSERT(allocator->destroy != NULL);

	return allocator->destroy(allocator);
}