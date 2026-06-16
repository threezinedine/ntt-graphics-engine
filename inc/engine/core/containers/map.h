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

struct ntt_Map
{
	ntt_List bucks;
	usize	 bucketCount;
	usize	 count;
};

typedef struct ntt_Map ntt_Map;
DEFINE_RETURN_RESULT_TYPE(ntt_Map)

typedef u32 (*ntt_HashFunction)(void* pKey, usize keySize);

/**
 * @param bucketCount Be default if <= 0
 * @param pAllocator  Be malloc allocator if NULL
 * @param hashFunction cannot be null
 */
ntt_MapResult		   ntt_MapCreate(ntt_HashFunction hashFunction, usize bucketCount, ntt_Allocator* pAllocator);
ntt_Result			   ntt_MapInsert(ntt_Map* pMap, void* pKey, usize keySize, void* pValue, usize valueSize);
b8					   ntt_MapContains(ntt_Map* pMap, void* pKey, usize keySize);
ntt_KeyValuePairResult ntt_MapGet(ntt_Map* pMap, void* pKey, usize keySize);
ntt_Result			   ntt_MapRemove(ntt_Map* pMap, void* pKey, usize keySize);
ntt_Result			   ntt_MapClear(ntt_Map* pMap);
ntt_Result			   ntt_MapDestroy(ntt_Map* pMap);

#endif /* _MAP_H_ */