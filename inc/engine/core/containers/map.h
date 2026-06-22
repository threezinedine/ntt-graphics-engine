#ifndef _MAP_H_
#define _MAP_H_

#include "engine/core/result.h"
#include "engine/core/types.h"
#include "list.h"

#define NTT_MAP_DEFAULT_BUCKET_COUNT 16

/**
 * Just store the pointer to the key and value, the user no needa manage the lifetime of it.
 */
struct ntt_KeyValuePair
{
	void* pKey;
	void* pValue;
};

typedef struct ntt_KeyValuePair ntt_KeyValuePair;
DEFINE_RETURN_RESULT_TYPE(ntt_KeyValuePair)

typedef u32 (*ntt_HashFunction)(void* pKey, usize keySize);

struct ntt_Map
{
	ntt_List*		 bucks;
	usize			 bucketCount;
	usize			 count;
	ntt_HashFunction hashFunction;
	ntt_Allocator*	 pAllocator;
};

typedef struct ntt_Map ntt_Map;
DEFINE_RETURN_RESULT_TYPE(ntt_Map)

/**
 * @param bucketCount Be default if <= 0
 * @param pAllocator  Be malloc allocator if NULL
 * @param hashFunction cannot be null
 */
ntt_MapResult		   ntt_Map_Create(ntt_HashFunction hashFunction, usize bucketCount, ntt_Allocator* pAllocator);
ntt_Result			   ntt_Map_Insert(ntt_Map* pMap, void* pKey, usize keySize, void* pValue, usize valueSize);
b8					   ntt_Map_Contains(ntt_Map* pMap, void* pKey, usize keySize);
ntt_KeyValuePairResult ntt_Map_Get(ntt_Map* pMap, void* pKey, usize keySize);
ntt_Result			   ntt_Map_Remove(ntt_Map* pMap, void* pKey, usize keySize);
ntt_Result			   ntt_Map_Clear(ntt_Map* pMap);
ntt_Result			   ntt_Map_Destroy(ntt_Map* pMap);

#endif /* _MAP_H_ */