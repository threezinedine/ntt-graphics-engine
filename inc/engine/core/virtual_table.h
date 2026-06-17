#ifndef _VIRTUAL_TABLE_H_
#define _VIRTUAL_TABLE_H_

#include "engine/core/object.h"

/**
 * Storing the inheritance information for the base object of the engine.
 */
struct ntt_VirtualTableNode
{
	ID* pTypeID;
	ID* pBaseTypeID;
};

typedef struct ntt_VirtualTableNode ntt_VirtualTableNode;

b8 ntt_VirtualTableNodeIsDerivedFrom(ID* pTypeID, ID* pCheckTypeID);
b8 ntt_VirtualTableNodeIsInstanceOf(ID* pTypeID, ID* pCheckTypeID);

#endif /* _VIRTUAL_TABLE_H_ */