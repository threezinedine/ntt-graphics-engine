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
	pNewNode->pOwner   = pList;

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

ntt_Result ntt_ListHeadAppend(ntt_List* pList, void* pData, usize dataSize)
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
	pNewNode->pOwner   = pList;

	if (pList->pHead == NULL)
	{
		pList->pHead  = pNewNode;
		pList->length = 1;
	}
	else
	{
		pNewNode->pNext		= pList->pHead;
		pList->pHead->pPrev = pNewNode;
		pList->length++;
	}

	pList->pHead = pNewNode;

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

ntt_Result ntt_ListInsert(ntt_List* pList, usize index, void* pData, usize dataSize)
{
	NTT_ASSERT_IF(pList == NULL)
	{
		return NTT_RESULT_NULL_POINTER;
	}

	NTT_ASSERT_IF(pList->pAllocator == NULL)
	{
		return NTT_RESULT_MISSING_ALLOCATOR;
	}

	NTT_ASSERT_IF(pData == NULL)
	{
		return NTT_RESULT_NULL_POINTER;
	}

	NTT_UNUSED(dataSize);

	usize		  currentIndex = 0;
	ntt_ListNode* pCurrentNode = pList->pHead;

	while (pCurrentNode != NULL)
	{
		if (currentIndex == index)
		{
			// Insert logic here
			break;
		}

		pCurrentNode = pCurrentNode->pNext;
		currentIndex++;
	}

	return NTT_RESULT_SUCCESS;
}

ntt_Result ntt_ListRemove(ntt_List* pList, usize index)
{
	NTT_UNUSED(pList);
	NTT_UNUSED(index);
	return NTT_RESULT_SUCCESS;
}

voidPtrResult ntt_ListGet(ntt_List* pList, usize index)
{
	NTT_ASSERT_IF(pList == NULL)
	{
		return (voidPtrResult){.result = NTT_RESULT_NULL_POINTER, .pData = NULL};
	}

	NTT_ASSERT_IF(index >= pList->length)
	{
		return (voidPtrResult){.result = NTT_RESULT_INDEX_OUT_OF_BOUNDS, .pData = NULL};
	}

	ntt_ListNode* pCurrentNode = pList->pHead;
	usize		  currentIndex = 0;

	while (currentIndex < index)
	{
		pCurrentNode = pCurrentNode->pNext;
		currentIndex++;
	}

	NTT_ASSERT_IF(pCurrentNode == NULL)
	{
		return (voidPtrResult){.result = NTT_RESULT_UNKNOWN_ERROR, .pData = NULL};
	}

	return (voidPtrResult){.result = NTT_RESULT_SUCCESS, .pData = pCurrentNode->pData};
}

ntt_Result ntt_ListInsertAfterNode(ntt_List* pList, ntt_ListNode* pNode, void* pData, usize dataSize)
{
	NTT_ASSERT_IF(pList == NULL)
	{
		return NTT_RESULT_NULL_POINTER;
	}

	NTT_ASSERT_IF(pList->pAllocator == NULL)
	{
		return NTT_RESULT_MISSING_ALLOCATOR;
	}

	if (pNode == NULL)
	{
		return ntt_ListAppend(pList, pData, dataSize);
	}

	NTT_ASSERT_IF(pNode->pOwner != pList)
	{
		return NTT_RESULT_INVALID_NODE_OWNER;
	}

	voidPtrResult allocateResult = ntt_Allocate(pList->pAllocator, sizeof(ntt_ListNode));
	NTT_SUCCESS_ASSERT_VAR(allocateResult);
	ntt_ListNode* pNewNode = (ntt_ListNode*)allocateResult.pData;

	voidPtrResult allocateDataResult = ntt_Allocate(pList->pAllocator, dataSize);
	NTT_SUCCESS_ASSERT_VAR(allocateDataResult);
	pNewNode->pData = allocateDataResult.pData;
	memcpy(pNewNode->pData, pData, dataSize);
	pNewNode->dataSize = dataSize;
	pNewNode->pOwner   = pList;

	ntt_ListNode* pNextNode = pNode->pNext;

	pNode->pNext	= pNewNode;
	pNewNode->pPrev = pNode;
	pNewNode->pNext = pNextNode;

	if (pNextNode != NULL)
	{
		pNextNode->pPrev = pNewNode;
	}
	else
	{
		pList->pTail = pNewNode;
	}

	pList->length++;

	return NTT_RESULT_SUCCESS;
}

ntt_Result ntt_ListInsertBeforeNode(ntt_List* pList, ntt_ListNode* pNode, void* pData, usize dataSize)
{
	NTT_ASSERT_IF(pList == NULL)
	{
		return NTT_RESULT_NULL_POINTER;
	}

	NTT_ASSERT_IF(pList->pAllocator == NULL)
	{
		return NTT_RESULT_MISSING_ALLOCATOR;
	}

	if (pNode == NULL)
	{
		return ntt_ListHeadAppend(pList, pData, dataSize);
	}

	NTT_ASSERT_IF(pNode->pOwner != pList)
	{
		return NTT_RESULT_INVALID_NODE_OWNER;
	}

	voidPtrResult allocateResult = ntt_Allocate(pList->pAllocator, sizeof(ntt_ListNode));
	NTT_SUCCESS_ASSERT_VAR(allocateResult);
	ntt_ListNode* pNewNode = (ntt_ListNode*)allocateResult.pData;

	voidPtrResult allocateDataResult = ntt_Allocate(pList->pAllocator, dataSize);
	NTT_SUCCESS_ASSERT_VAR(allocateDataResult);
	pNewNode->pData = allocateDataResult.pData;
	memcpy(pNewNode->pData, pData, dataSize);
	pNewNode->dataSize = dataSize;
	pNewNode->pOwner   = pList;

	ntt_ListNode* pPrevNode = pNode->pPrev;

	pNode->pPrev	= pNewNode;
	pNewNode->pNext = pNode;
	pNewNode->pPrev = pPrevNode;

	if (pPrevNode != NULL)
	{
		pPrevNode->pNext = pNewNode;
	}
	else
	{
		pList->pHead = pNewNode;
	}

	pList->length++;

	return NTT_RESULT_SUCCESS;
}

ntt_Result ntt_ListRemoveNode(ntt_List* pList, ntt_ListNode* pNode)
{
	NTT_ASSERT_IF(pList == NULL)
	{
		return NTT_RESULT_NULL_POINTER;
	}

	NTT_ASSERT_IF(pList->pAllocator == NULL)
	{
		return NTT_RESULT_MISSING_ALLOCATOR;
	}

	NTT_ASSERT_IF(pNode == NULL)
	{
		return NTT_RESULT_NULL_POINTER;
	}

	NTT_ASSERT_IF(pNode->pOwner != pList)
	{
		return NTT_RESULT_INVALID_NODE_OWNER;
	}

	ntt_ListNode* pPrevNode = pNode->pPrev;
	ntt_ListNode* pNextNode = pNode->pNext;

	if (pPrevNode != NULL)
	{
		pPrevNode->pNext = pNextNode;
	}

	if (pNextNode != NULL)
	{
		pNextNode->pPrev = pPrevNode;
	}

	if (pNode == pList->pHead)
	{
		pList->pHead = pNextNode;
	}

	if (pNode == pList->pTail)
	{
		pList->pTail = pPrevNode;
	}

	pList->length--;

	// Deallocate the node's data and the node itself
	ntt_Result deallocateDataResult = ntt_Deallocate(pList->pAllocator, pNode->pData, pNode->dataSize);
	NTT_SUCCESS_ASSERT(deallocateDataResult);

	ntt_Result deallocateNodeResult = ntt_Deallocate(pList->pAllocator, pNode, sizeof(ntt_ListNode));
	NTT_SUCCESS_ASSERT(deallocateNodeResult);

	return NTT_RESULT_SUCCESS;
}