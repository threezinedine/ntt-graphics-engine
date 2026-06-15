#include "engine/core/memory/malloc.h"
#include "engine/core/defs.h"
#include "engine/core/utils/backtrace.h"
#include "engine/core/utils/console.h"
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

typedef struct ntt_MallocAllocator
{
	usize allocatedBytes;
} ntt_MallocAllocator;

typedef struct ntt_MallocBlockHeader
{
	usize size;
#if NTT_STRICT_MEMORY_MANAGEMENT
	ntt_BacktraceInfo backtraceInfo;
#endif /* NTT_STRICT_MEMORY_MANAGEMENT */
} ntt_MallocBlockHeader;

static voidPtrResult allocate(ntt_Allocator* allocator, usize size);
static ntt_Result	 deallocate(ntt_Allocator* allocator, void* ptr, usize size);
static ntt_Result	 destroy(ntt_Allocator* allocator);

AllocatorResult ntt_CreateMallocAllocator()
{
	AllocatorResult result;
	ntt_Allocator*	allocator = (ntt_Allocator*)malloc(sizeof(ntt_Allocator));
	NTT_ASSERT_IF(allocator == NULL)
	{
		result.result	  = NTT_RESULT_ALLOCATION_FAILURE;
		result.pAllocator = NULL;
		return result;
	}

	allocator->allocate	  = allocate;
	allocator->deallocate = deallocate;
	allocator->destroy	  = destroy;

	allocator->pInternalState = malloc(sizeof(ntt_MallocAllocator));
	NTT_ASSERT_IF(allocator->pInternalState == NULL)
	{
		free(allocator);
		result.result	  = NTT_RESULT_ALLOCATION_FAILURE;
		result.pAllocator = NULL;
		return result;
	}

	ntt_MallocAllocator* pMallocState = (ntt_MallocAllocator*)allocator->pInternalState;

	pMallocState->allocatedBytes = 0;

	result.result	  = NTT_RESULT_SUCCESS;
	result.pAllocator = allocator;
	return result;
}

static voidPtrResult allocate(ntt_Allocator* allocator, usize size)
{
	voidPtrResult result;
	NTT_ASSERT_IF(allocator == NULL)
	{
		return (voidPtrResult){.result = NTT_RESULT_NULL_POINTER, .pData = NULL};
	}
	NTT_ASSERT_IF(allocator->pInternalState == NULL)
	{
		return (voidPtrResult){.result = NTT_RESULT_NULL_POINTER, .pData = NULL};
	}

	ntt_MallocAllocator* pMallocState = (ntt_MallocAllocator*)allocator->pInternalState;
	pMallocState->allocatedBytes += size;
	void*				   block   = malloc(sizeof(ntt_MallocBlockHeader) + size);
	void*				   ptr	   = (char*)block + sizeof(ntt_MallocBlockHeader);
	ntt_MallocBlockHeader* pHeader = (ntt_MallocBlockHeader*)block;
	pHeader->size				   = size;

#if NTT_STRICT_MEMORY_MANAGEMENT
	pHeader->backtraceInfo = ntt_CaptureCallStack();
#endif /* NTT_STRICT_MEMORY_MANAGEMENT */

	result.pData  = ptr;
	result.result = NTT_RESULT_SUCCESS;
	return result;
}

static ntt_Result deallocate(ntt_Allocator* allocator, void* ptr, usize size)
{
	NTT_ASSERT_IF(allocator == NULL)
	{
		return NTT_RESULT_NULL_POINTER;
	}
	NTT_ASSERT_IF(allocator->pInternalState == NULL)
	{
		return NTT_RESULT_NULL_POINTER;
	}
	NTT_ASSERT_IF(ptr == NULL)
	{
		return NTT_RESULT_NULL_POINTER;
	}

	ntt_MallocBlockHeader* pHeader = (ntt_MallocBlockHeader*)((char*)ptr - sizeof(ntt_MallocBlockHeader));

#if NTT_STRICT_MEMORY_MANAGEMENT
#if !NTT_TESTS
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
#endif /* !NTT_TESTS */

	NTT_ASSERT_M(pHeader->size == size,
				 "Deallocation size does not match allocated size. Allocated size: %zu, deallocation size: %zu.\n",
				 pHeader->size,
				 size);
	if (pHeader->size != size)
	{
		return NTT_RESULT_FREE_MISMATCH_SIZE;
	}
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

	ntt_MallocAllocator* pMallocState = (ntt_MallocAllocator*)allocator->pInternalState;
	if (pMallocState->allocatedBytes >= size)
	{
		pMallocState->allocatedBytes -= size;
	}
	else
	{
#if !NTT_TESTS
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
#endif /* !NTT_TESTS */

		pMallocState->allocatedBytes = 0;
	}

	free(pHeader);
	return NTT_RESULT_SUCCESS;
}

static ntt_Result destroy(ntt_Allocator* allocator)
{
	NTT_ASSERT_IF(allocator == NULL)
	{
		return NTT_RESULT_NULL_POINTER;
	}
	NTT_ASSERT_IF(allocator->pInternalState == NULL)
	{
		return NTT_RESULT_NULL_POINTER;
	}

	NTT_ASSERT_IF_M(((ntt_MallocAllocator*)allocator->pInternalState)->allocatedBytes != 0,
					"Memory leak detected: %zu bytes still allocated.",
					(((ntt_MallocAllocator*)allocator->pInternalState)->allocatedBytes))
	{
		return NTT_RESULT_MEMORY_LEAK;
	}

	free(allocator->pInternalState);
	free(allocator);
	return NTT_RESULT_SUCCESS;
}