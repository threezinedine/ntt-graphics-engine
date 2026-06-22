#include "engine/core/containers/array.h"
#include "engine/core/memory/memory.h"
#include <string.h>

ntt_ArrayResult
ntt_Array_Create(usize elementSize, usize initialCapacity, ntt_Allocator* pAllocator, ntt_Array_Destructor destructor)
{
	ntt_ArrayResult result;
	result.result = NTT_RESULT_SUCCESS;

	ntt_Allocator* pFinalAllocator = pAllocator != NULL ? pAllocator : g_memoryGlobals.mallocAllocator;
	NTT_ASSERT(pFinalAllocator != NULL);
	if (pFinalAllocator == NULL)
	{
		result.result = NTT_RESULT_INITIALIZE_WITHOUT_ALLOCATOR;
		return result;
	}

	result.data.destructor = destructor;

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

	voidPtrResult allocateResult = ntt_Allocate(pFinalAllocator, elementSize * initialCapacity);
	if (allocateResult.result != NTT_RESULT_SUCCESS)
	{
		result.result = allocateResult.result;
		return result;
	}

	result.data.pData	   = allocateResult.pData;
	result.data.pAllocator = pFinalAllocator;

	return result;
}

ntt_Result ntt_Array_Resize(ntt_Array* pArray, usize newCapacity)
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

	voidPtrResult allocateResult = ntt_Allocate(pArray->pAllocator, pArray->elementSize * newCapacity);
	NTT_SUCCESS_ASSERT_VAR(allocateResult);

	memcpy(allocateResult.pData, pArray->pData, pArray->elementSize * pArray->length);
	ntt_Result deallocateResult =
		ntt_Deallocate(pArray->pAllocator, pArray->pData, pArray->elementSize * pArray->capacity);
	NTT_SUCCESS_ASSERT(deallocateResult);

	pArray->pData	 = allocateResult.pData;
	pArray->capacity = newCapacity;

	return NTT_RESULT_SUCCESS;
}

ntt_Result ntt_Array_Destroy(ntt_Array* pArray)
{
	NTT_ASSERT_IF(pArray == NULL)
	{
		return NTT_RESULT_NULL_POINTER;
	}

	NTT_ASSERT_IF(pArray->pAllocator == NULL)
	{
		return NTT_RESULT_INITIALIZE_WITHOUT_ALLOCATOR;
	}

	NTT_ASSERT_IF(pArray->pData == NULL)
	{
		return NTT_RESULT_NULL_POINTER;
	}

	NTT_SUCCESS_ASSERT(ntt_Array_Clear(pArray));

	ntt_Result deallocateResult =
		ntt_Deallocate(pArray->pAllocator, pArray->pData, pArray->elementSize * pArray->capacity);
	NTT_SUCCESS_ASSERT(deallocateResult);

	pArray->pData		= NULL;
	pArray->length		= 0;
	pArray->capacity	= 0;
	pArray->elementSize = 0;

	return NTT_RESULT_SUCCESS;
}

ntt_Result ntt_Array_Append(ntt_Array* pArray, void* pElement)
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
		ntt_Result resizeResult = ntt_Array_Resize(pArray, pArray->capacity * 2);
		NTT_SUCCESS_ASSERT(resizeResult);
	}

	memcpy((char*)pArray->pData + (pArray->length * pArray->elementSize), pElement, pArray->elementSize);
	pArray->length++;

	return NTT_RESULT_SUCCESS;
}

void* ntt_Array_Get(ntt_Array* pArray, usize index)
{
	NTT_ASSERT_IF(pArray == NULL)
	{
		return NULL;
	}

	NTT_ASSERT_IF(index >= pArray->length)
	{
		return NULL;
	}

	return (char*)pArray->pData + (index * pArray->elementSize);
}

void ntt_Array_Set(ntt_Array* pArray, usize index, void* pElement)
{
	NTT_ASSERT_IF(pArray == NULL)
	{
		return;
	}

	NTT_ASSERT_IF(pElement == NULL)
	{
		return;
	}

	NTT_ASSERT_IF(index >= pArray->length)
	{
		return;
	}

	memcpy((char*)pArray->pData + (index * pArray->elementSize), pElement, pArray->elementSize);
}

ntt_Result ntt_Array_Erase(ntt_Array* pArray, usize index)
{
	NTT_ASSERT_IF(pArray == NULL)
	{
		return NTT_RESULT_NULL_POINTER;
	}

	NTT_ASSERT_IF(index >= pArray->length)
	{
		return NTT_RESULT_INDEX_OUT_OF_BOUNDS;
	}

	if (index < pArray->length - 1)
	{
		memmove((char*)pArray->pData + (index * pArray->elementSize),
				(char*)pArray->pData + ((index + 1) * pArray->elementSize),
				(pArray->length - index - 1) * pArray->elementSize);
	}

	pArray->length--;

	return NTT_RESULT_SUCCESS;
}

ntt_Result ntt_Array_Clear(ntt_Array* pArray)
{
	NTT_ASSERT_IF(pArray == NULL)
	{
		return NTT_RESULT_NULL_POINTER;
	}

	for (usize i = pArray->length; i > 0; i--)
	{
		void* pElement = (char*)pArray->pData + ((i - 1) * pArray->elementSize);
		if (pArray->destructor != NULL)
		{
			ntt_Result destructorResult = pArray->destructor(pElement);
			NTT_SUCCESS_ASSERT(destructorResult);
		}
	}

	pArray->length = 0;

	return NTT_RESULT_SUCCESS;
}

b8 ntt_Array_Any(ntt_Array* pArray, ntt_ArrayElementPredicate predicate)
{
	NTT_ASSERT_IF(pArray == NULL)
	{
		return FALSE;
	}

	NTT_ASSERT_IF(predicate == NULL)
	{
		return FALSE;
	}

	for (usize i = 0; i < pArray->length; i++)
	{
		void* pElement = (char*)pArray->pData + (i * pArray->elementSize);
		if (predicate(pElement))
		{
			return TRUE;
		}
	}

	return FALSE;
}

b8 ntt_Array_All(ntt_Array* pArray, ntt_ArrayElementPredicate predicate)
{
	NTT_ASSERT_IF(pArray == NULL)
	{
		return FALSE;
	}

	NTT_ASSERT_IF(predicate == NULL)
	{
		return FALSE;
	}

	for (usize i = 0; i < pArray->length; i++)
	{
		void* pElement = (char*)pArray->pData + (i * pArray->elementSize);
		if (!predicate(pElement))
		{
			return FALSE;
		}
	}

	return TRUE;
}

usize ntt_Array_Find(ntt_Array* pArray, ntt_ArrayElementPredicate predicate)
{
	NTT_ASSERT_IF(pArray == NULL)
	{
		return NTT_ARRAY_INDEX_NOT_FOUND;
	}

	NTT_ASSERT_IF(predicate == NULL)
	{
		return NTT_ARRAY_INDEX_NOT_FOUND;
	}

	for (usize i = 0; i < pArray->length; i++)
	{
		void* pElement = (char*)pArray->pData + (i * pArray->elementSize);
		if (predicate(pElement))
		{
			return i;
		}
	}

	return NTT_ARRAY_INDEX_NOT_FOUND;
}

ntt_Result ntt_Array_Insert(ntt_Array* pArray, usize index, void* pElement)
{
	NTT_ASSERT_IF(pArray == NULL)
	{
		return NTT_RESULT_NULL_POINTER;
	}

	NTT_ASSERT_IF(pElement == NULL)
	{
		return NTT_RESULT_NULL_POINTER;
	}

	NTT_ASSERT_IF(index > pArray->length)
	{
		return NTT_RESULT_INDEX_OUT_OF_BOUNDS;
	}

	if (pArray->length >= pArray->capacity)
	{
		ntt_Result resizeResult = ntt_Array_Resize(pArray, pArray->capacity * 2);
		NTT_SUCCESS_ASSERT(resizeResult);
	}

	if (index < pArray->length)
	{
		memmove((char*)pArray->pData + ((index + 1) * pArray->elementSize),
				(char*)pArray->pData + (index * pArray->elementSize),
				(pArray->length - index) * pArray->elementSize);
	}

	memcpy((char*)pArray->pData + (index * pArray->elementSize), pElement, pArray->elementSize);
	pArray->length++;

	return NTT_RESULT_SUCCESS;
}