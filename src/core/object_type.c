#include "engine/core/object_type.h"

const char* ntt_ObjectType_ToString(ntt_ObjectType type)
{
	switch (type)
	{
#define ENUM_OPTION(name)                                                                                              \
	case name:                                                                                                         \
		return #name;
#define ENUM_END(name)                                                                                                 \
	default:                                                                                                           \
		return "Unknown Object Type";
#include "engine/core/object_type.inl"
#undef ENUM_OPTION
#undef ENUM_END
	}
}