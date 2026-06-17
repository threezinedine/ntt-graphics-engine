#ifndef _DISPLAY_COMPONENT_H_
#define _DISPLAY_COMPONENT_H_

#include "engine/systems/component.h"

struct ntt_DisplayComponent
{
	ntt_Component base; // Base component data, must be the first member for inheritance
};

OBJECT_DECLARE(ntt_DisplayComponent)

#endif /* _DISPLAY_COMPONENT_H_ */