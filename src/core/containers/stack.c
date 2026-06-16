#include "engine/core/containers/stack.h"

ntt_StackResult ntt_StackCreate(ntt_Allocator* pAllocator)
{
	ntt_StackResult result;
	result.result = NTT_RESULT_SUCCESS;

	ntt_ListResult listResult = ntt_ListCreate(pAllocator, NULL);
	NTT_ASSERT_IF(listResult.result != NTT_RESULT_SUCCESS)
	{
		result.result = listResult.result;
		return result;
	}

	result.data.list = listResult.data;
	return result;
}

ntt_Result ntt_StackPush(ntt_Stack* pStack, void* pData, usize dataSize)
{
	NTT_ASSERT_IF(pStack == NULL)
	{
		return NTT_RESULT_NULL_POINTER;
	}

	return ntt_ListHeadAppend(&pStack->list, pData, dataSize);
}

voidPtrResult ntt_StackTop(ntt_Stack* pStack)
{
	NTT_ASSERT_IF(pStack == NULL)
	{
		return (voidPtrResult){.result = NTT_RESULT_NULL_POINTER, .pData = NULL};
	}

	if (pStack->list.pTail == NULL)
	{
		return (voidPtrResult){.result = NTT_RESULT_STACK_EMPTY, .pData = NULL};
	}

	return (voidPtrResult){.result = NTT_RESULT_SUCCESS, .pData = pStack->list.pTail->pData};
}

ntt_Result ntt_StackPop(ntt_Stack* pStack)
{
	NTT_ASSERT_IF(pStack == NULL)
	{
		return NTT_RESULT_NULL_POINTER;
	}

	return ntt_ListRemoveNode(&pStack->list, pStack->list.pTail);
}

b8 ntt_StackIsEmpty(ntt_Stack* pStack)
{
	NTT_ASSERT_IF(pStack == NULL)
	{
		return TRUE;
	}

	return pStack->list.length == 0;
}

ntt_Result ntt_StackClear(ntt_Stack* pStack)
{
	NTT_ASSERT_IF(pStack == NULL)
	{
		return NTT_RESULT_NULL_POINTER;
	}

	return ntt_ListClear(&pStack->list);
}

ntt_Result ntt_StackDestroy(ntt_Stack* pStack)
{
	NTT_ASSERT_IF(pStack == NULL)
	{
		return NTT_RESULT_NULL_POINTER;
	}

	return ntt_ListDestroy(&pStack->list);
}