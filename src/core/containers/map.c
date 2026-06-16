#include "engine/core/containers/map.h"
#include "engine/core/memory/memory.h"

ntt_MapResult ntt_MapCreate(ntt_HashFunction hashFunction, usize bucketCount, ntt_Allocator* pAllocator)
{
	ntt_MapResult result;
	result.result = NTT_RESULT_SUCCESS;

	NTT_ASSERT_IF(hashFunction == NULL)
	{
		result.result = NTT_RESULT_NULL_POINTER;
		return result;
	}

	usize		   finalBucketCount = bucketCount > 0 ? bucketCount : NTT_MAP_DEFAULT_BUCKET_COUNT;
	ntt_Allocator* pFinalAllocator	= pAllocator != NULL ? pAllocator : g_memoryGlobals.mallocAllocator;

	NTT_UNUSED(finalBucketCount);

	NTT_ASSERT_IF(pFinalAllocator == NULL)
	{
		result.result = NTT_RESULT_INITIALZE_WITHOUT_ALLOCATOR;
		return result;
	}

	return result;
}

ntt_Result ntt_MapInsert(ntt_Map* pMap, void* pKey, usize keySize, void* pValue, usize valueSize)
{
	NTT_ASSERT_IF(pMap == NULL)
	{
		return NTT_RESULT_NULL_POINTER;
	}

	NTT_ASSERT_IF(pKey == NULL || pValue == NULL)
	{
		return NTT_RESULT_NULL_POINTER;
	}

	NTT_ASSERT_IF(pMap->bucks.pAllocator == NULL)
	{
		return NTT_RESULT_MISSING_ALLOCATOR;
	}

	NTT_UNUSED(keySize);

	NTT_UNUSED(valueSize);

	return NTT_RESULT_SUCCESS;
}

b8 ntt_MapContains(ntt_Map* pMap, void* pKey, usize keySize)
{
	NTT_ASSERT_IF(pMap == NULL)
	{
		return FALSE;
	}

	NTT_ASSERT_IF(pKey == NULL)
	{
		return FALSE;
	}

	NTT_UNUSED(keySize);

	return FALSE;
}

ntt_KeyValuePairResult ntt_MapGet(ntt_Map* pMap, void* pKey, usize keySize)
{
	ntt_KeyValuePairResult result;
	result.data.pKey   = NULL;
	result.data.pValue = NULL;
	result.result	   = NTT_RESULT_SUCCESS;

	NTT_ASSERT_IF(pMap == NULL)
	{
		result.result = NTT_RESULT_NULL_POINTER;
		return result;
	}

	NTT_ASSERT_IF(pKey == NULL)
	{
		result.result = NTT_RESULT_NULL_POINTER;
		return result;
	}

	NTT_UNUSED(keySize);

	return result;
}

ntt_Result ntt_MapRemove(ntt_Map* pMap, void* pKey, usize keySize)
{
	NTT_ASSERT_IF(pMap == NULL)
	{
		return NTT_RESULT_NULL_POINTER;
	}

	NTT_ASSERT_IF(pKey == NULL)
	{
		return NTT_RESULT_NULL_POINTER;
	}

	NTT_UNUSED(keySize);

	return NTT_RESULT_SUCCESS;
}

ntt_Result ntt_MapClear(ntt_Map* pMap)
{
	NTT_ASSERT_IF(pMap == NULL)
	{
		return NTT_RESULT_NULL_POINTER;
	}

	return NTT_RESULT_SUCCESS;
}

ntt_Result ntt_MapDestroy(ntt_Map* pMap)
{
	NTT_ASSERT_IF(pMap == NULL)
	{
		return NTT_RESULT_NULL_POINTER;
	}

	return NTT_RESULT_SUCCESS;
}