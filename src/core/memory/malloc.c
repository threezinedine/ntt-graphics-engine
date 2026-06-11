#include "engine/core/memory/malloc.h"
#include "engine/core/defs.h"
#include "engine/core/utils/backtrace.h"
#include <stdlib.h>

/**
 * Allocated block layouts:
 *
 * [        Header        ][       User Data       ]
 * [ size + (debug info)  ][       Memory Block    ]
 *
 * If wanna check detailed of the memory leak, can enable the backtrace to capture the allocation call stack, and
 * 		store it into the header. At the normal case, header only contains the size of the allocated block, which
 *      is later used for tracking the deallocation size modification.
 */

struct ntt_MallocAllocator
{
	u32 allocatedBytes;
};

struct ntt_MallocBlockHeader
{
	usize size;
#if NTT_STRICT_MEMORY_MANAGEMENT
	struct ntt_BacktraceInfo backtraceInfo;
#endif /* NTT_STRICT_MEMORY_MANAGEMENT */
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

	allocator->pInternalState = malloc(sizeof(struct ntt_MallocAllocator));
	NTT_ASSERT(allocator->pInternalState != NULL);
	struct ntt_MallocAllocator* pMallocState = (struct ntt_MallocAllocator*)allocator->pInternalState;

	pMallocState->allocatedBytes = 0;

	return allocator;
}

static void* allocate(struct ntt_Allocator* allocator, usize size)
{
	NTT_ASSERT(allocator != NULL);
	NTT_ASSERT(allocator->pInternalState != NULL);

	struct ntt_MallocAllocator* pMallocState = (struct ntt_MallocAllocator*)allocator->pInternalState;
	pMallocState->allocatedBytes += size;
	void*						  block	  = malloc(sizeof(struct ntt_MallocBlockHeader) + size);
	void*						  ptr	  = (char*)block + sizeof(struct ntt_MallocBlockHeader);
	struct ntt_MallocBlockHeader* pHeader = (struct ntt_MallocBlockHeader*)block;
	pHeader->size						  = size;

#if NTT_STRICT_MEMORY_MANAGEMENT
	pHeader->backtraceInfo = ntt_CaptureCallStack();
#endif /* NTT_STRICT_MEMORY_MANAGEMENT */

	return ptr;
}

static void deallocate(struct ntt_Allocator* allocator, void* ptr, usize size)
{
	NTT_ASSERT(allocator != NULL);
	NTT_ASSERT(allocator->pInternalState != NULL);
	NTT_ASSERT(ptr != NULL);

	struct ntt_MallocBlockHeader* pHeader =
		(struct ntt_MallocBlockHeader*)((char*)ptr - sizeof(struct ntt_MallocBlockHeader));

#if NTT_STRICT_MEMORY_MANAGEMENT
	if (pHeader->size != size)
	{
		// Print the call stack information of the allocation point.
		ntt_ConsoleSetColor(NTT_COLOR_MAGENTA);
		ntt_ConsolePrint("=================================================\n");
		ntt_ConsolePrint("[MEMORY] Allocate stack:");
		ntt_PrintCallStack(&pHeader->backtraceInfo);
		ntt_ConsolePrint("=================================================\n");
		ntt_ConsolePrint("[MEMORY] Deallocate stack:");
		ntt_PrintCallStack(NULL);
		ntt_ConsoleResetColor();
	}

	NTT_ASSERT_M(pHeader->size == size,
				 "Deallocation size does not match allocated size. Allocated size: %zu, deallocation size: %zu.\n",
				 pHeader->size,
				 size);
#else
	if (pHeader->size != size)
	{
		// Warning that the deallocation size does not match the allocated size.
		ntt_ConsoleSetColor(NTT_COLOR_YELLOW);
		ntt_ConsolePrint(
			"Warning: deallocation size (%zu) does not match allocated size (%zu).\n", size, pHeader->size);
		ntt_ConsoleResetColor();
	}
#endif /* NTT_STRICT_MEMORY_MANAGEMENT */

	struct ntt_MallocAllocator* pMallocState = (struct ntt_MallocAllocator*)allocator->pInternalState;
	if (pMallocState->allocatedBytes >= size)
	{
		pMallocState->allocatedBytes -= size;
	}
	else
	{
#if NTT_STRICT_MEMORY_MANAGEMENT
		// Print the call stack information of the allocation point.
		ntt_ConsoleSetColor(NTT_COLOR_RED);
		ntt_PrintCallStack(&pHeader->backtraceInfo);
		ntt_ConsoleResetColor();

#else
		// Warning that the allocated bytes tracking is corrupted, which may indicate a bug in the
		// allocation/deallocation logic.
		ntt_ConsoleSetColor(NTT_COLOR_YELLOW);
		ntt_ConsolePrint(
			"Warning: allocated bytes tracking is corrupted. Allocated bytes: %zu, deallocation size: %zu.\n",
			pMallocState->allocatedBytes,
			size);
		ntt_ConsoleResetColor();
#endif /* NTT_STRICT_MEMORY_MANAGEMENT */

		pMallocState->allocatedBytes = 0;
	}

	free(pHeader);
}

static void destroy(struct ntt_Allocator* allocator)
{
	NTT_ASSERT(allocator != NULL);
	NTT_ASSERT(allocator->pInternalState != NULL);

	NTT_ASSERT_M(((struct ntt_MallocAllocator*)allocator->pInternalState)->allocatedBytes == 0,
				 "Memory leak detected: %zu bytes still allocated.",
				 (((struct ntt_MallocAllocator*)allocator->pInternalState)->allocatedBytes));

	free(allocator->pInternalState);
	free(allocator);
}