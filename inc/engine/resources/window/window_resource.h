#ifndef _WINDOW_RESOURCE_H_
#define _WINDOW_RESOURCE_H_

#include "engine/core/result.h"
#include "engine/core/types.h"
#include "engine/resources/resource.h"

struct ntt_WindowResourceCreateInfo
{
	const char* title;
	u32			width;
	u32			height;
};

typedef struct ntt_WindowResourceCreateInfo ntt_WindowResourceCreateInfo;

struct ntt_WindowResource
{
	ntt_Resource base;
	ID			 windowID;
};

OBJECT_DECLARE(ntt_WindowResource)

#endif /** _WINDOW_RESOURCE_H_ */