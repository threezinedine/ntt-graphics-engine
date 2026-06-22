#include "engine/core/containers/storage.h"
#include "engine/core/memory/memory.h"

ntt_Result ntt_Storage_Initialize(ntt_Storage*			  pStorage,
								  ntt_Allocator*		  pAllocator,
								  usize					  blockSize,
								  u32					  blockCount,
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
	pStorage->blockSize			   = blockSize;
	pStorage->blockCount		   = blockCount;

	pStorage->createElement	 = pCreateInfo->createElement;
	pStorage->newElement	 = pCreateInfo->newElement;
	pStorage->freeElement	 = pCreateInfo->freeElement;
	pStorage->destroyElement = pCreateInfo->destroyElement;

	ntt_ArrayResult blocksResult = ntt_Array_Create(blockSize, 0, pFinalAllocator, NULL);
	NTT_SUCCESS_ASSERT_VAR(blocksResult);

	pStorage->blocks = blocksResult.data;

	ntt_ArrayResult idsResult = ntt_Array_Create(sizeof(ID), 0, pFinalAllocator, NULL);
	NTT_SUCCESS_ASSERT_VAR(idsResult);

	pStorage->ids = idsResult.data;

	for (usize i = 0; i < blockCount; i++)
	{
		IDResult newIDRes = ntt_NewID(NTT_OBJECT_TYPE_OBJECT, NULL);
		NTT_SUCCESS_ASSERT_VAR(newIDRes);
		ntt_Array_Append(&pStorage->ids, &newIDRes.data);
	}

	ntt_StackResult freeBlocksResult = ntt_Stack_Create(pFinalAllocator);
	NTT_SUCCESS_ASSERT_VAR(freeBlocksResult);

	pStorage->freeBlocks = freeBlocksResult.data;

	pStorage->m_blockIndexCounter = 0;

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
	NTT_SUCCESS_ASSERT(ntt_Array_Destroy(&pStorage->ids));
	NTT_SUCCESS_ASSERT(ntt_Stack_Destroy(&pStorage->freeBlocks));

	pStorage->pAllocator = NULL;

	return NTT_RESULT_SUCCESS;
}

IDResult ntt_Storage_New(ntt_Storage* pStorage)
{
	IDResult result;
	result.result = NTT_RESULT_SUCCESS;

	NTT_ASSERT_IF(pStorage == NULL)
	{
		result.result = NTT_RESULT_NULL_POINTER;
		return result;
	}

	if (pStorage->m_blockIndexCounter >= pStorage->blockCount && ntt_Stack_IsEmpty(&pStorage->freeBlocks))
	{
		result.result = NTT_RESULT_STORAGE_IS_FULL;
		return result;
	}

	if (!ntt_Stack_IsEmpty(&pStorage->freeBlocks))
	{
		voidPtrResult topResult = ntt_Stack_Top(&pStorage->freeBlocks);
		if (topResult.result != NTT_RESULT_SUCCESS)
		{
			result.result = topResult.result;
			return result;
		}
		result.data			 = *(ID*)topResult.pData;
		ntt_Result popResult = ntt_Stack_Pop(&pStorage->freeBlocks);
		if (popResult != NTT_RESULT_SUCCESS)
		{
			result.result = popResult;
			return result;
		}
	}
	else
	{
		void* pID = ntt_Array_Get(&pStorage->ids, pStorage->m_blockIndexCounter);
		NTT_ASSERT_IF(pID == NULL)
		{
			result.result = NTT_RESULT_NULL_POINTER;
			return result;
		}
		result.data = *(ID*)pID;
		pStorage->m_blockIndexCounter++;
	}

	if (pStorage->newElement != NULL)
	{
		ntt_Result newElementResult = pStorage->newElement(pStorage);
		NTT_ASSERT_IF(newElementResult != NTT_RESULT_SUCCESS)
		{
			// If creating a new element fails, we need to push the ID back to the free stack
			result.result = newElementResult;
			return result;
		}
	}

	return result;
}

ntt_Result ntt_Storage_Free(ntt_Storage* pStorage, ID elementID)
{
	NTT_ASSERT_IF(pStorage == NULL)
	{
		return NTT_RESULT_NULL_POINTER;
	}

	if (pStorage->freeElement != NULL)
	{
		NTT_SUCCESS_ASSERT(pStorage->freeElement(pStorage, elementID));
	}

	NTT_SUCCESS_ASSERT(ntt_Stack_Push(&pStorage->freeBlocks, &elementID, sizeof(ID)));

	return NTT_RESULT_SUCCESS;
}