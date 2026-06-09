#include "engine/core/memory/malloc.h"
#include "engine/core/defs.h"
#include <stdlib.h>

struct ntt_MallocAllocator
{
};

static void* allocate(struct ntt_Allocator* allocator, usize size);
static void	 deallocate(struct ntt_Allocator* allocator, void* ptr, usize size);
static void	 destroy(struct ntt_Allocator* allocator);

struct ntt_Allocator* ntt_CreateMallocAllocator()
{
	struct ntt_Allocator* allocator = (struct ntt_Allocator*)malloc(sizeof(struct ntt_Allocator));
	NTT_ASSERT(allocator != NULL);

	allocator->internalState = (struct ntt_MallocAllocator*)malloc(sizeof(struct ntt_MallocAllocator));
	NTT_ASSERT(allocator->internalState != NULL);

	allocator->allocate	  = allocate;
	allocator->deallocate = deallocate;
	allocator->destroy	  = destroy;

	return allocator;
}

static void* allocate(struct ntt_Allocator* allocator, usize size)
{
	NTT_ASSERT(allocator != NULL);
	NTT_ASSERT(allocator->internalState != NULL);

	return malloc(size);
}

static void deallocate(struct ntt_Allocator* allocator, void* ptr, usize size)
{
	NTT_ASSERT(allocator != NULL);
	NTT_ASSERT(allocator->internalState != NULL);
	NTT_ASSERT(ptr != NULL);

	free(ptr);
}

static void destroy(struct ntt_Allocator* allocator)
{
	NTT_ASSERT(allocator != NULL);
	NTT_ASSERT(allocator->internalState != NULL);

	free(allocator->internalState);
	free(allocator);
}