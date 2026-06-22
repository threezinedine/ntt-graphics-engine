#include "engine/core/containers/queue.h"

ntt_QueueResult ntt_QueueCreate(ntt_Allocator* pAllocator)
{
	ntt_QueueResult result;
	result.result = NTT_RESULT_SUCCESS;

	ntt_ListResult listResult = ntt_List_Create(pAllocator, NULL);
	NTT_ASSERT_IF(listResult.result != NTT_RESULT_SUCCESS)
	{
		result.result = listResult.result;
		return result;
	}

	result.data.list = listResult.data;
	return result;
}

ntt_Result ntt_QueueEnqueue(ntt_Queue* pQueue, void* pData, usize dataSize)
{
	NTT_ASSERT_IF(pQueue == NULL)
	{
		return NTT_RESULT_NULL_POINTER;
	}

	return ntt_List_Append(&pQueue->list, pData, dataSize);
}

voidPtrResult ntt_QueueFront(ntt_Queue* pQueue)
{
	NTT_ASSERT_IF(pQueue == NULL)
	{
		return (voidPtrResult){.result = NTT_RESULT_NULL_POINTER, .pData = NULL};
	}

	if (pQueue->list.pHead == NULL)
	{
		return (voidPtrResult){.result = NTT_RESULT_QUEUE_EMPTY, .pData = NULL};
	}

	return (voidPtrResult){.result = NTT_RESULT_SUCCESS, .pData = pQueue->list.pHead->pData};
}

ntt_Result ntt_QueueDequeue(ntt_Queue* pQueue)
{
	NTT_ASSERT_IF(pQueue == NULL)
	{
		return NTT_RESULT_NULL_POINTER;
	}

	return ntt_List_RemoveNode(&pQueue->list, pQueue->list.pHead);
}

b8 ntt_QueueIsEmpty(ntt_Queue* pQueue)
{
	NTT_ASSERT_IF(pQueue == NULL)
	{
		return TRUE;
	}

	return pQueue->list.length == 0;
}

ntt_Result ntt_QueueClear(ntt_Queue* pQueue)
{
	NTT_ASSERT_IF(pQueue == NULL)
	{
		return NTT_RESULT_NULL_POINTER;
	}

	return ntt_List_Clear(&pQueue->list);
}

ntt_Result ntt_QueueDestroy(ntt_Queue* pQueue)
{
	NTT_ASSERT_IF(pQueue == NULL)
	{
		return NTT_RESULT_NULL_POINTER;
	}

	return ntt_List_Destroy(&pQueue->list);
}