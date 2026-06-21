#include "engine/resources/resource_type.h"

const char* ntt_ResourceType_ToString(ntt_ResourceType type)
{
	switch (type)
	{
#define RESOURCE_TYPE(option)                                                                                          \
	case NTT_RESOURCE_TYPE_##option:                                                                                   \
		return #option;
#define RESOURCE_TYPE_END()                                                                                            \
	case NTT_RESOURCE_TYPE_COUNT:                                                                                      \
		return "NTT_RESOURCE_TYPE_COUNT";
#include "engine/resources/resource_type.inl"
#undef RESOURCE_TYPE
#undef RESOURCE_TYPE_END
	default:
		NTT_UNREACHABLE();
		return "Unknown Resource Type";
	}
}