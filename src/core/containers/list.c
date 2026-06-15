#include "engine/core/containers/list.h"
#include "engine/core/memory/memory.h"
#include "engine/core/utils/utils.h"
#include <string.h>

ntt_ListResult ntt_ListCreate(ntt_Allocator* pAllocator)
{
	ntt_ListResult result;
	result.result = NTT_RESULT_SUCCESS;

	ntt_Allocator* pFinalAllocator = pAllocator != NULL ? pAllocator : g_memoryGlobals.mallocAllocator;

	NTT_ASSERT_IF(pFinalAllocator == NULL)
	{
		result.result = NTT_RESULT_INITIALZE_WITHOUT_ALLOCATOR;
		return result;
	}

	result.data.pHead	   = NULL;
	result.data.pTail	   = NULL;
	result.data.length	   = 0;
	result.data.pAllocator = pFinalAllocator;

	return result;
}

ntt_Result ntt_ListAppend(ntt_List* pList, void* pData, usize dataSize)
{
	NTT_ASSERT_IF(pList == NULL)
	{
		return NTT_RESULT_NULL_POINTER;
	}

	NTT_ASSERT_IF(pList->pAllocator == NULL)
	{
		return NTT_RESULT_MISSING_ALLOCATOR;
	}

	voidPtrResult allocateResult = ntt_Allocate(pList->pAllocator, sizeof(ntt_ListNode));
	NTT_SUCCESS_ASSERT_VAR(allocateResult);
	ntt_ListNode* pNewNode = (ntt_ListNode*)allocateResult.pData;

	voidPtrResult allocateDataResult = ntt_Allocate(pList->pAllocator, dataSize);
	NTT_SUCCESS_ASSERT_VAR(allocateDataResult);
	pNewNode->pData = allocateDataResult.pData;

	memcpy(pNewNode->pData, pData, dataSize);
	pNewNode->dataSize = dataSize;
	pNewNode->pNext	   = NULL;
	pNewNode->pPrev	   = NULL;

	if (pList->pHead == NULL)
	{
		pList->pHead  = pNewNode;
		pList->length = 1;
	}
	else
	{
		pNewNode->pPrev		= pList->pTail;
		pList->pTail->pNext = pNewNode;
		pList->length++;
	}

	pList->pTail = pNewNode;

	return NTT_RESULT_SUCCESS;
}

ntt_Result ntt_ListClear(ntt_List* pList)
{
	NTT_ASSERT_IF(pList == NULL)
	{
		return NTT_RESULT_NULL_POINTER;
	}

	NTT_ASSERT_IF(pList->pAllocator == NULL)
	{
		return NTT_RESULT_MISSING_ALLOCATOR;
	}

	if (pList->pHead == NULL)
	{
		return NTT_RESULT_SUCCESS;
	}

	ntt_ListNode* pCurrentNode = pList->pHead;

	while (pCurrentNode != NULL)
	{
		ntt_ListNode* pNextNode = pCurrentNode->pNext;

		ntt_Result deallocateDataResult =
			ntt_Deallocate(pList->pAllocator, pCurrentNode->pData, pCurrentNode->dataSize);
		NTT_SUCCESS_ASSERT(deallocateDataResult);

		ntt_Result deallocateResult = ntt_Deallocate(pList->pAllocator, pCurrentNode, sizeof(ntt_ListNode));
		NTT_SUCCESS_ASSERT(deallocateResult);

		pCurrentNode = pNextNode;
	}

	pList->pHead  = NULL;
	pList->pTail  = NULL;
	pList->length = 0;

	return NTT_RESULT_SUCCESS;
}

ntt_Result ntt_ListDestroy(ntt_List* pList)
{
	NTT_ASSERT_IF(pList == NULL)
	{
		return NTT_RESULT_NULL_POINTER;
	}

	NTT_ASSERT_IF(pList->pAllocator == NULL)
	{
		return NTT_RESULT_MISSING_ALLOCATOR;
	}

	NTT_SUCCESS_ASSERT(ntt_ListClear(pList));

	return NTT_RESULT_SUCCESS;
}