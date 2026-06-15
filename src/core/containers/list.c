#include "engine/core/containers/list.h"
#include "engine/core/memory/memory.h"

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

	result.data.pHead  = NULL;
	result.data.pTail  = NULL;
	result.data.length = 0;

	return result;
}

ntt_Result ntt_ListAppend(ntt_List* pList, void* pData)
{
	NTT_ASSERT_IF(pList == NULL)
	{
		return NTT_RESULT_NULL_POINTER;
	}

	voidPtrResult allocateResult = pList->pAllocator->allocate(pList->pAllocator, sizeof(ntt_ListNode));
	NTT_SUCCESS_ASSERT_VAR(allocateResult);
	ntt_ListNode* pNewNode = (ntt_ListNode*)allocateResult.pData;
	pNewNode->pData		   = pData;
	pNewNode->pNext		   = NULL;
	pNewNode->pPrev		   = NULL;

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

ntt_Result ntt_ListDestroy(ntt_List* pList)
{
	NTT_ASSERT_IF(pList == NULL)
	{
		return NTT_RESULT_NULL_POINTER;
	}

	return NTT_RESULT_SUCCESS;
}