#ifndef _COMPONENT_H_
#define _COMPONENT_H_

#include "engine/core/object.h"

struct ntt_Component
{
	ntt_Object base; // Base object data, must be the first member for inheritance
};

OBJECT_DECLARE(ntt_Component)

#endif /* _COMPONENT_H_ */