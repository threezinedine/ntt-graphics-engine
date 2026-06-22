#ifndef _STORAGE_H_
#define _STORAGE_H_

#include "array.h"
#include "engine/core/id.h"
#include "engine/core/result.h"
#include "engine/core/types.h"
#include "stack.h"

struct ntt_Storage;

typedef ntt_Result (*ntt_Storage_CreateElement)(struct ntt_Storage* pStorage);

/**
 * Be run each time when an element is fetched from the pool
 */
typedef IDResult (*ntt_Storage_NewElement)(struct ntt_Storage* pStorage);

/**
 * Be called each time when an element is returned to the pool
 */
typedef ntt_Result (*ntt_Storage_FreeElement)(struct ntt_Storage* pStorage, ID elementID);

typedef ntt_Result (*ntt_Storage_DestroyElement)(struct ntt_Storage* pStorage);

struct ntt_Storage_CreateInfo
{
	ntt_Storage_CreateElement  createElement;
	ntt_Storage_NewElement	   newElement;
	ntt_Storage_FreeElement	   freeElement;
	ntt_Storage_DestroyElement destroyElement;
};

typedef struct ntt_Storage_CreateInfo ntt_Storage_CreateInfo;

struct ntt_Storage
{
	ntt_Allocator*			   pAllocator;
	ntt_Array				   blocks;
	ntt_Stack				   freeBlocks;
	ntt_Storage_CreateElement  createElement;
	ntt_Storage_NewElement	   newElement;
	ntt_Storage_FreeElement	   freeElement;
	ntt_Storage_DestroyElement destroyElement;
};

typedef struct ntt_Storage ntt_Storage;

ntt_Result ntt_Storage_Initialize(ntt_Storage*			  pStorage,
								  ntt_Allocator*		  pAllocator,
								  u32					  blockSize,
								  ntt_Storage_CreateInfo* pCreateInfo);
ntt_Result ntt_Storage_Destroy(ntt_Storage* pStorage);

#endif /** _STORAGE_H_ */