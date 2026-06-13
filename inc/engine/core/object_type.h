#ifndef _OBJECT_TYPE_H_
#define _OBJECT_TYPE_H_

enum ntt_ObjectType
{
#define ENUM_OPTION(name) name,
#define ENUM_END(name)	  name
#include "object_type.inl"
#undef ENUM_OPTION
#undef ENUM_END
};

typedef enum ntt_ObjectType ntt_ObjectType;

const char* ntt_ObjectTypeToString(ntt_ObjectType type);

#endif /* _OBJECT_TYPE_H_ */