#ifndef _ENTITY_H_
#define _ENTITY_H_

#include "engine/core/object.h"

struct ntt_Entity
{
	ntt_Object base; // Base object data, must be the first member for inheritance
};

OBJECT_DECLARE(ntt_Entity)

#endif /* _ENTITY_H_ */