#include "engine/core/memory/malloc.h"
#include "engine/core/defs.h"
#include <stdlib.h>

struct ntt_MallocAllocator
{
	u32 allocatedBytes;
};

static void* allocate(struct ntt_Allocator* allocator, usize size);
static void	 deallocate(struct ntt_Allocator* allocator, void* ptr, usize size);
static void	 destroy(struct ntt_Allocator* allocator);

struct ntt_Allocator* ntt_CreateMallocAllocator()
{
	struct ntt_Allocator* allocator = (struct ntt_Allocator*)malloc(sizeof(struct ntt_Allocator));
	NTT_ASSERT(allocator != NULL);

	allocator->allocate	  = allocate;
	allocator->deallocate = deallocate;
	allocator->destroy	  = destroy;

	allocator->internalState = malloc(sizeof(struct ntt_MallocAllocator));
	NTT_ASSERT(allocator->internalState != NULL);
	struct ntt_MallocAllocator* mallocState = (struct ntt_MallocAllocator*)allocator->internalState;

	mallocState->allocatedBytes = 0;

	return allocator;
}

static void* allocate(struct ntt_Allocator* allocator, usize size)
{
	NTT_ASSERT(allocator != NULL);
	NTT_ASSERT(allocator->internalState != NULL);

	struct ntt_MallocAllocator* mallocState = (struct ntt_MallocAllocator*)allocator->internalState;
	mallocState->allocatedBytes += size;

	return malloc(size);
}

static void deallocate(struct ntt_Allocator* allocator, void* ptr, usize size)
{
	NTT_ASSERT(allocator != NULL);
	NTT_ASSERT(allocator->internalState != NULL);
	NTT_ASSERT(ptr != NULL);
	NTT_UNUSED(size); // Size is not needed for deallocation in this allocator

	struct ntt_MallocAllocator* mallocState = (struct ntt_MallocAllocator*)allocator->internalState;
	if (mallocState->allocatedBytes >= size)
	{
		mallocState->allocatedBytes -= size;
	}

	free(ptr);
}

static void destroy(struct ntt_Allocator* allocator)
{
	NTT_ASSERT(allocator != NULL);
	NTT_ASSERT(allocator->internalState != NULL);

	NTT_ASSERT_M(((struct ntt_MallocAllocator*)allocator->internalState)->allocatedBytes == 0,
				 "Memory leak detected: not all allocated memory has been deallocated.");

	free(allocator->internalState);
	free(allocator);
}