#include "engine/core/object.h"
#include "engine/core/defs.h"

ID		   ntt_ObjectID	 = INVALID_ID_INIT;
static ID* s_pBaseTypeID = NULL;
ntt_Result ntt_ObjectRegisterType()
{
	IDResult result = ntt_NewID(NTT_OBJECT_TYPE_OBJECT_ID);
	NTT_SUCCESS_ASSERT_VAR(result);
	ntt_ObjectID = result.data;
	return NTT_RESULT_SUCCESS;
}

ntt_Result ntt_ObjectUnregisterType()
{
	ntt_ObjectID  = INVALID_ID;
	s_pBaseTypeID = NULL;
	return NTT_RESULT_SUCCESS;
}

ntt_Result ntt_ObjectInitialize(ntt_Object* pObject, ntt_Allocator* pAllocator)
{
	NTT_ASSERT(pObject != NULL);
	NTT_ASSERT(pAllocator != NULL);
	NTT_ASSERT_M(ntt_IsIDEqual(&ntt_ObjectID, &INVALID_ID) == FALSE, "The \"Object\" type is not registered.");

	IDResult result = ntt_NewID(NTT_OBJECT_TYPE_OBJECT_ID);
	NTT_SUCCESS_ASSERT_VAR(result);
	pObject->id			= result.data;
	pObject->type		= ntt_ObjectID;
	pObject->pAllocator = pAllocator;

	return NTT_RESULT_SUCCESS;
}

ntt_Result ntt_ObjectDestroy(ntt_Object* pObject)
{
	NTT_ASSERT(pObject != NULL);
	NTT_ASSERT_M(ntt_IsIDEqual(&pObject->id, &INVALID_ID) == FALSE,
				 "The object is not initialized or already destroyed.");

	// Invalidate the ID of the object, so that it can not be used anymore.
	pObject->id = INVALID_ID;

	return NTT_RESULT_SUCCESS;
}

b8 ntt_ObjectIsInstanceOf(ntt_Object* pObject)
{
	NTT_ASSERT(pObject != NULL);
	return ntt_IsIDEqual(&pObject->type, &ntt_ObjectID) ? TRUE : FALSE;
}

b8 ntt_ObjectIsDerivedFrom(ntt_Object* pObject)
{
	NTT_ASSERT(pObject != NULL);
	return ntt_ObjectIsInstanceOf(pObject);
}