#include "engine/core/containers/stack.h"

ntt_StackResult ntt_Stack_Create(ntt_Allocator* pAllocator)
{
	ntt_StackResult result;
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

ntt_Result ntt_Stack_Push(ntt_Stack* pStack, void* pData, usize dataSize)
{
	NTT_ASSERT_IF(pStack == NULL)
	{
		return NTT_RESULT_NULL_POINTER;
	}

	return ntt_List_Append(&pStack->list, pData, dataSize);
}

voidPtrResult ntt_Stack_Top(ntt_Stack* pStack)
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

ntt_Result ntt_Stack_Pop(ntt_Stack* pStack)
{
	NTT_ASSERT_IF(pStack == NULL)
	{
		return NTT_RESULT_NULL_POINTER;
	}

	return ntt_List_RemoveNode(&pStack->list, pStack->list.pTail);
}

b8 ntt_Stack_IsEmpty(ntt_Stack* pStack)
{
	NTT_ASSERT_IF(pStack == NULL)
	{
		return TRUE;
	}

	return pStack->list.length == 0;
}

ntt_Result ntt_Stack_Clear(ntt_Stack* pStack)
{
	NTT_ASSERT_IF(pStack == NULL)
	{
		return NTT_RESULT_NULL_POINTER;
	}

	return ntt_List_Clear(&pStack->list);
}

ntt_Result ntt_Stack_Destroy(ntt_Stack* pStack)
{
	NTT_ASSERT_IF(pStack == NULL)
	{
		return NTT_RESULT_NULL_POINTER;
	}

	return ntt_List_Destroy(&pStack->list);
}