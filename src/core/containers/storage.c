#include "engine/core/containers/storage.h"
#include "engine/core/memory/memory.h"

ntt_Result ntt_Storage_Initialize(ntt_Storage*			  pStorage,
								  ntt_Allocator*		  pAllocator,
								  u32					  blockSize,
								  ntt_Storage_CreateInfo* pCreateInfo)
{
	NTT_ASSERT_IF(pStorage == NULL)
	{
		return NTT_RESULT_NULL_POINTER;
	}

	NTT_ASSERT_IF(pStorage->pAllocator != NULL)
	{
		return NTT_RESULT_ALREADY_INITIALIZED;
	}

	ntt_Allocator* pFinalAllocator = pAllocator == NULL ? g_memoryGlobals.mallocAllocator : pAllocator;
	pStorage->pAllocator		   = pFinalAllocator;

	pStorage->createElement	 = pCreateInfo->createElement;
	pStorage->newElement	 = pCreateInfo->newElement;
	pStorage->freeElement	 = pCreateInfo->freeElement;
	pStorage->destroyElement = pCreateInfo->destroyElement;

	ntt_ArrayResult blocksResult = ntt_Array_Create(blockSize, 0, pFinalAllocator, NULL);
	NTT_SUCCESS_ASSERT_VAR(blocksResult);

	pStorage->blocks = blocksResult.data;

	ntt_StackResult freeBlocksResult = ntt_Stack_Create(pFinalAllocator);
	NTT_SUCCESS_ASSERT_VAR(freeBlocksResult);

	pStorage->freeBlocks = freeBlocksResult.data;

	return NTT_RESULT_SUCCESS;
}

ntt_Result ntt_Storage_Destroy(ntt_Storage* pStorage)
{
	NTT_ASSERT_IF(pStorage == NULL)
	{
		return NTT_RESULT_NULL_POINTER;
	}

	NTT_ASSERT_IF(pStorage->pAllocator == NULL)
	{
		return NTT_RESULT_INITIALIZE_WITHOUT_ALLOCATOR;
	}

	NTT_SUCCESS_ASSERT(ntt_Array_Destroy(&pStorage->blocks));
	NTT_SUCCESS_ASSERT(ntt_Stack_Destroy(&pStorage->freeBlocks));

	pStorage->pAllocator = NULL;

	return NTT_RESULT_SUCCESS;
}