#ifndef _SYSTEM_H_
#define _SYSTEM_H_

#include "engine/core/object.h"

/**
 * The base data that all systems will use. The system is a manager so that handles the logics for a specific
 *      component which is used by the entities.
 * Note that, all systems must be singleton, so that they can be easily accessed by the entities and other systems. The
 * 		system will be responsible.
 */
struct ntt_System
{
	ntt_Object base; // Base object data, must be the first member for inheritance
};

OBJECT_DECLARE(ntt_System)

#endif /* _SYSTEM_H_ */