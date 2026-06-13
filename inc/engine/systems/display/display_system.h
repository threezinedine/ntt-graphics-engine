#ifndef _DISPLAY_SYSTEM_H_
#define _DISPLAY_SYSTEM_H_

#include "engine/systems/system.h"

/**
 * Responsible for handling the windows and the rendering context, can provide multiple windows, works with them.
 */
struct ntt_DisplaySystem
{
	ntt_System base; // Base system data, must be the first member for inheritance
};

OBJECT_DECLARE(ntt_DisplaySystem)

#endif /* _DISPLAY_SYSTEM_H_ */