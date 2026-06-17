#include "engine/core/virtual_table.h"

b8 ntt_VirtualTableNodeIsDerivedFrom(ID* pTypeID, ID* pCheckTypeID)
{
	NTT_UNUSED(pTypeID);
	NTT_UNUSED(pCheckTypeID);
	return FALSE; // Placeholder implementation, needs to be implemented based on the virtual table logic
}

b8 ntt_VirtualTableNodeIsInstanceOf(ID* pTypeID, ID* pCheckTypeID)
{
	NTT_UNUSED(pTypeID);
	NTT_UNUSED(pCheckTypeID);
	return FALSE; // Placeholder implementation, needs to be implemented based on the virtual table logic
}