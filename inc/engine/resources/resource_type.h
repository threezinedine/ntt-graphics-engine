#ifndef _RESOURCE_TYPE_H_
#define _RESOURCE_TYPE_H_

#include "engine/core/result.h"
#include "engine/core/types.h"

enum ntt_ResourceType
{
#define RESOURCE_TYPE(option) NTT_RESOURCE_TYPE_##option,
#define RESOURCE_TYPE_END()	  NTT_RESOURCE_TYPE_COUNT
#include "engine/resources/resource_type.inl"
#undef RESOURCE_TYPE
#undef RESOURCE_TYPE_END
};

typedef enum ntt_ResourceType ntt_ResourceType;

const char* ntt_ResourceType_ToString(ntt_ResourceType type);

#endif /** _RESOURCE_TYPE_H_ */