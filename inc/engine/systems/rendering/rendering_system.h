#ifndef _RENDERING_SYSTEM_H_
#define _RENDERING_SYSTEM_H_

#include "engine/core/object.h"
#include "engine/systems/system.h"

struct ntt_RenderingSystem
{
	ntt_System base;
};

OBJECT_DECLARE(ntt_RenderingSystem)

#endif /* _RENDERING_SYSTEM_H_ */