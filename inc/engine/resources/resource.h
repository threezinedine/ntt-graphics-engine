#ifndef _RESOURCE_H_
#define _RESOURCE_H_

#include "engine/core/object.h"
#include "engine/core/result.h"
#include "engine/core/types.h"
#include "resource_type.h"

struct ntt_Resource
{
	ntt_Object		 base;
	ntt_ResourceType resourceType;
	b8				 isLoading;
	b8				 isLoaded;

	ntt_Result (*pfnLoad)(struct ntt_Resource* pResource);
	ntt_Result (*pfnUnload)(struct ntt_Resource* pResource);
};

OBJECT_DECLARE(ntt_Resource)

ntt_Result ntt_Resource_Load(ntt_Resource* pResource);
b8		   ntt_Resource_IsLoading(ntt_Resource* pResource);
b8		   ntt_Resource_IsLoaded(ntt_Resource* pResource);
ntt_Result ntt_Resource_Unload(ntt_Resource* pResource);

#endif /** _RESOURCE_H_ */