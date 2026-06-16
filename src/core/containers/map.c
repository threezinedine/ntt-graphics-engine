#include "engine/core/containers/map.h"
#include "engine/core/memory/memory.h"
#include <string.h>

struct ntt_MapNodeData
{
	void* pKey;
	usize keySize;
	void* pValue;
	usize valueSize;
};

typedef struct ntt_MapNodeData ntt_MapNodeData;
static b8		  _MapNodeDataKeyEquals(void* pElement, usize elementSize, void* pUserData, usize userDataSize);
static ntt_Result _remap(ntt_Map* pMap);

ntt_Result _destructorNodeData(void* pData, usize dataSize)
{
	NTT_UNUSED(dataSize);

	ntt_MapNodeData* pNodeData = (ntt_MapNodeData*)pData;

	ntt_Result deallocateKeyResult =
		ntt_Deallocate(g_memoryGlobals.mallocAllocator, pNodeData->pKey, pNodeData->keySize);
	NTT_SUCCESS_ASSERT(deallocateKeyResult);

	ntt_Result deallocateValueResult =
		ntt_Deallocate(g_memoryGlobals.mallocAllocator, pNodeData->pValue, pNodeData->valueSize);
	NTT_SUCCESS_ASSERT(deallocateValueResult);

	return NTT_RESULT_SUCCESS;
}

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

	result.data.bucketCount	 = finalBucketCount;
	result.data.count		 = 0;
	result.data.hashFunction = hashFunction;
	result.data.pAllocator	 = pFinalAllocator;

	voidPtrResult bucksAllocateResult = ntt_Allocate(pFinalAllocator, sizeof(ntt_List) * finalBucketCount);
	NTT_ASSERT_IF(bucksAllocateResult.result != NTT_RESULT_SUCCESS)
	{
		result.result = bucksAllocateResult.result;
		return result;
	}

	result.data.bucks = (ntt_List*)bucksAllocateResult.pData;

	for (usize i = 0; i < finalBucketCount; i++)
	{
		result.data.bucks[i].pHead			= NULL;
		result.data.bucks[i].pTail			= NULL;
		result.data.bucks[i].length			= 0;
		result.data.bucks[i].pAllocator		= pFinalAllocator;
		result.data.bucks[i].nodeDestructor = _destructorNodeData;
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

	NTT_ASSERT_IF(pMap->pAllocator == NULL)
	{
		return NTT_RESULT_MISSING_ALLOCATOR;
	}

	NTT_ASSERT_IF(pMap->hashFunction == NULL)
	{
		return NTT_RESULT_NULL_POINTER;
	}

	if (pMap->count >= pMap->bucketCount * 4)
	{
		NTT_SUCCESS_ASSERT(_remap(pMap));
	}

	u32		  hash		  = pMap->hashFunction(pKey, keySize);
	u32		  bucketIndex = hash % pMap->bucketCount;
	ntt_List* pBucket	  = &pMap->bucks[bucketIndex];

	// check if the key already exists in the map, if so, update the value and return
	ntt_ListNode* pNode = ntt_ListFindNode(pBucket, (ntt_ListElementPredicate)_MapNodeDataKeyEquals, pKey, keySize);
	if (pNode != NULL)
	{
		ntt_MapNodeData* pNodeData		 = (ntt_MapNodeData*)pNode->pData;
		ntt_Result deallocateValueResult = ntt_Deallocate(pMap->pAllocator, pNodeData->pValue, pNodeData->valueSize);
		NTT_SUCCESS_ASSERT(deallocateValueResult);

		voidPtrResult allocateValueResult = ntt_Allocate(pMap->pAllocator, valueSize);
		NTT_SUCCESS_ASSERT_VAR(allocateValueResult);
		pNodeData->pValue = allocateValueResult.pData;
		memcpy(pNodeData->pValue, pValue, valueSize);
		pNodeData->valueSize = valueSize;

		return NTT_RESULT_SUCCESS;
	}

	ntt_MapNodeData nodeData;

	voidPtrResult allocateKeyResult = ntt_Allocate(pMap->pAllocator, keySize);
	NTT_SUCCESS_ASSERT_VAR(allocateKeyResult);
	nodeData.pKey = allocateKeyResult.pData;
	memcpy(nodeData.pKey, pKey, keySize);
	nodeData.keySize = keySize;

	voidPtrResult allocateValueResult = ntt_Allocate(pMap->pAllocator, valueSize);
	NTT_SUCCESS_ASSERT_VAR(allocateValueResult);
	nodeData.pValue = allocateValueResult.pData;
	memcpy(nodeData.pValue, pValue, valueSize);
	nodeData.valueSize = valueSize;

	ntt_ListAppend(pBucket, &nodeData, sizeof(ntt_MapNodeData));
	pMap->count++;

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

	u32		  hash		  = pMap->hashFunction(pKey, keySize);
	u32		  bucketIndex = hash % pMap->bucketCount;
	ntt_List* pBucket	  = &pMap->bucks[bucketIndex];

	return ntt_ListContains(pBucket, (ntt_ListElementPredicate)_MapNodeDataKeyEquals, pKey, keySize);
}

b8 _MapNodeDataKeyEquals(void* pElement, usize elementSize, void* pUserData, usize userDataSize)
{
	NTT_UNUSED(elementSize);
	NTT_UNUSED(userDataSize);

	ntt_MapNodeData* pNodeData = (ntt_MapNodeData*)pElement;
	return pNodeData->keySize == userDataSize && memcmp(pNodeData->pKey, pUserData, userDataSize) == 0;
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

	u32			  hash		  = pMap->hashFunction(pKey, keySize);
	u32			  bucketIndex = hash % pMap->bucketCount;
	ntt_List*	  pBucket	  = &pMap->bucks[bucketIndex];
	ntt_ListNode* pNode = ntt_ListFindNode(pBucket, (ntt_ListElementPredicate)_MapNodeDataKeyEquals, pKey, keySize);

	NTT_ASSERT_IF(pNode == NULL)
	{
		result.result = NTT_RESULT_KEY_NOT_FOUND;
		return result;
	}

	ntt_MapNodeData* pNodeData = (ntt_MapNodeData*)pNode->pData;
	result.data.pKey		   = pNodeData->pKey;
	result.data.pValue		   = pNodeData->pValue;

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

	u32			  hash		  = pMap->hashFunction(pKey, keySize);
	u32			  bucketIndex = hash % pMap->bucketCount;
	ntt_List*	  pBucket	  = &pMap->bucks[bucketIndex];
	ntt_ListNode* pNode = ntt_ListFindNode(pBucket, (ntt_ListElementPredicate)_MapNodeDataKeyEquals, pKey, keySize);

	NTT_ASSERT_IF(pNode == NULL)
	{
		return NTT_RESULT_KEY_NOT_FOUND;
	}

	return ntt_ListRemoveNode(pBucket, pNode);
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

	NTT_ASSERT_IF(pMap->pAllocator == NULL)
	{
		return NTT_RESULT_MISSING_ALLOCATOR;
	}

	for (usize i = 0; i < pMap->bucketCount; i++)
	{
		ntt_List* pBucket = &pMap->bucks[i];
		NTT_SUCCESS_ASSERT(ntt_ListClear(pBucket));
	}

	NTT_SUCCESS_ASSERT(ntt_Deallocate(pMap->pAllocator, pMap->bucks, sizeof(ntt_List) * pMap->bucketCount));

	return NTT_RESULT_SUCCESS;
}

static ntt_Result _remap(ntt_Map* pMap)
{
	ntt_List* oldBucks		 = pMap->bucks;
	usize	  oldBucketCount = pMap->bucketCount;

	usize		  newBucketCount		 = oldBucketCount * 2;
	voidPtrResult newBucksAllocateResult = ntt_Allocate(pMap->pAllocator, sizeof(ntt_List) * newBucketCount);
	NTT_SUCCESS_ASSERT_VAR(newBucksAllocateResult);

	ntt_List* newBucks = (ntt_List*)newBucksAllocateResult.pData;
	pMap->bucks		   = newBucks;
	pMap->count		   = 0;
	pMap->bucketCount  = newBucketCount;

	for (usize i = 0; i < newBucketCount; i++)
	{
		pMap->bucks[i].pHead		  = NULL;
		pMap->bucks[i].pTail		  = NULL;
		pMap->bucks[i].length		  = 0;
		pMap->bucks[i].pAllocator	  = pMap->pAllocator;
		pMap->bucks[i].nodeDestructor = _destructorNodeData;
	}

	for (usize i = 0; i < oldBucketCount; i++)
	{
		ntt_List*	  pOldBucket   = &oldBucks[i];
		ntt_ListNode* pCurrentNode = pOldBucket->pHead;

		while (pCurrentNode != NULL)
		{
			ntt_MapNodeData* pNodeData = (ntt_MapNodeData*)pCurrentNode->pData;
			NTT_SUCCESS_ASSERT(
				ntt_MapInsert(pMap, pNodeData->pKey, pNodeData->keySize, pNodeData->pValue, pNodeData->valueSize));

			pCurrentNode = pCurrentNode->pNext;
		}

		NTT_SUCCESS_ASSERT(ntt_ListClear(pOldBucket));
	}

	NTT_SUCCESS_ASSERT(ntt_Deallocate(pMap->pAllocator, oldBucks, sizeof(ntt_List) * oldBucketCount));
	return NTT_RESULT_SUCCESS;
}