#include "engine/core/containers/array.h"
#include "engine/core/memory/memory.h"
#include <string.h>

ntt_ArrayResult ntt_ArrayCreate(usize elementSize, usize initialCapacity, ntt_Allocator* pAllocator)
{
	ntt_ArrayResult result;
	result.result = NTT_RESULT_SUCCESS;

	ntt_Allocator* pFinalAllocator = pAllocator != NULL ? pAllocator : g_memoryGlobals.mallocAllocator;
	NTT_ASSERT(pFinalAllocator != NULL);
	if (pFinalAllocator == NULL)
	{
		result.result = NTT_RESULT_INITIALZE_ARRAY_WITHOUT_ALLOCATOR;
		return result;
	}

	NTT_ASSERT(initialCapacity > 0);
	if (initialCapacity == 0)
	{
		initialCapacity = NTT_ARRAY_INITIAL_CAPACITY;
	}
	else if (initialCapacity < 0)
	{
		result.result = NTT_RESULT_NEGATIVE_INITIAL_CAPACITY;
		return result;
	}

	result.data.elementSize = elementSize;
	result.data.length		= 0;
	result.data.capacity	= initialCapacity;

	voidPtrResult allocateResult = pFinalAllocator->allocate(pFinalAllocator, elementSize * initialCapacity);
	if (allocateResult.result != NTT_RESULT_SUCCESS)
	{
		result.result = allocateResult.result;
		return result;
	}

	result.data.pData	   = allocateResult.pData;
	result.data.pAllocator = pFinalAllocator;

	return result;
}

ntt_Result ntt_ArrayResize(ntt_Array* pArray, usize newCapacity)
{
	NTT_ASSERT(pArray != NULL);
	if (pArray == NULL)
	{
		return NTT_RESULT_NULL_POINTER;
	}

	if (newCapacity <= pArray->capacity)
	{
		return NTT_RESULT_SUCCESS;
	}

	voidPtrResult allocateResult = pArray->pAllocator->allocate(pArray->pAllocator, pArray->elementSize * newCapacity);
	NTT_SUCCESS_ASSERT_RETURN(allocateResult);

	memcpy(allocateResult.pData, pArray->pData, pArray->elementSize * pArray->length);
	ntt_Result deallocateResult =
		pArray->pAllocator->deallocate(pArray->pAllocator, pArray->pData, pArray->elementSize * pArray->capacity);
	NTT_SUCCESS_ASSERT(deallocateResult);

	pArray->pData	 = allocateResult.pData;
	pArray->capacity = newCapacity;

	return NTT_RESULT_SUCCESS;
}

ntt_Result ntt_ArrayDestroy(ntt_Array* pArray)
{
	NTT_ASSERT(pArray != NULL);
	if (pArray == NULL)
	{
		return NTT_RESULT_NULL_POINTER;
	}

	NTT_ASSERT(pArray->pAllocator != NULL);
	if (pArray->pAllocator == NULL)
	{
		return NTT_RESULT_INITIALZE_ARRAY_WITHOUT_ALLOCATOR;
	}

	NTT_ASSERT(pArray->pData != NULL);
	if (pArray->pData == NULL)
	{
		return NTT_RESULT_NULL_POINTER;
	}

	ntt_Result deallocateResult =
		pArray->pAllocator->deallocate(pArray->pAllocator, pArray->pData, pArray->elementSize * pArray->capacity);
	NTT_SUCCESS_ASSERT(deallocateResult);

	pArray->pData		= NULL;
	pArray->length		= 0;
	pArray->capacity	= 0;
	pArray->elementSize = 0;

	return NTT_RESULT_SUCCESS;
}

ntt_Result ntt_ArrayAppend(ntt_Array* pArray, void* pElement)
{
	NTT_ASSERT(pArray != NULL);
	if (pArray == NULL)
	{
		return NTT_RESULT_NULL_POINTER;
	}

	NTT_ASSERT(pElement != NULL);
	if (pElement == NULL)
	{
		return NTT_RESULT_NULL_POINTER;
	}

	if (pArray->length >= pArray->capacity)
	{
		ntt_Result resizeResult = ntt_ArrayResize(pArray, pArray->capacity * 2);
		NTT_SUCCESS_ASSERT(resizeResult);
	}

	memcpy((char*)pArray->pData + (pArray->length * pArray->elementSize), pElement, pArray->elementSize);
	pArray->length++;

	return NTT_RESULT_SUCCESS;
}

void* ntt_ArrayGet(ntt_Array* pArray, usize index)
{
	NTT_ASSERT(pArray != NULL);
	if (pArray == NULL)
	{
		return NULL;
	}

	NTT_ASSERT(index < pArray->length);
	if (index >= pArray->length)
	{
		return NULL;
	}

	return (char*)pArray->pData + (index * pArray->elementSize);
}