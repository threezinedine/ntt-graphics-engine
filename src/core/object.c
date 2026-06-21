#include "engine/core/object.h"
#include "engine/core/defs.h"

ID		   ntt_ObjectID = INVALID_ID_INIT;
ntt_Result ntt_Object_RegisterType()
{
	IDResult result = ntt_NewID(NTT_OBJECT_TYPE_OBJECT_ID, NULL);
	NTT_SUCCESS_ASSERT_VAR(result);
	ntt_ObjectID = result.data;
	return NTT_RESULT_SUCCESS;
}

ntt_Result ntt_Object_UnregisterType()
{
	ntt_ObjectID = INVALID_ID;
	return NTT_RESULT_SUCCESS;
}

ntt_Result ntt_Object_Initialize(ntt_Object* pObject, ntt_Allocator* pAllocator, void* pUserData)
{
	NTT_UNUSED(pUserData);
	NTT_ASSERT(pObject != NULL);
	NTT_ASSERT(pAllocator != NULL);
	NTT_ASSERT_M(ntt_IsIDEqual(&ntt_ObjectID, &INVALID_ID) == FALSE, "The \"Object\" type is not registered.");

	IDResult result = ntt_NewID(NTT_OBJECT_TYPE_OBJECT, NULL);
	NTT_SUCCESS_ASSERT_VAR(result);
	pObject->id			= result.data;
	pObject->type		= ntt_ObjectID;
	pObject->pAllocator = pAllocator;

	return NTT_RESULT_SUCCESS;
}

ntt_Result ntt_Object_Destroy(ntt_Object* pObject)
{
	NTT_ASSERT(pObject != NULL);
	NTT_ASSERT_M(ntt_IsIDEqual(&pObject->id, &INVALID_ID) == FALSE,
				 "The object is not initialized or already destroyed.");

	// Invalidate the ID of the object, so that it can not be used anymore.
	pObject->id = INVALID_ID;

	return NTT_RESULT_SUCCESS;
}

INHERIT_CHECKING(ntt_Object)

ID* ntt_Object_GetBaseTypeID(ntt_Object* pObject)
{
	IDResult result = ntt_GetIDByID(&pObject->type);

	if (result.result != NTT_RESULT_SUCCESS)
	{
		return NULL;
	}

	if (result.data.type != NTT_OBJECT_TYPE_OBJECT_ID)
	{
		return NULL;
	}

	return result.data.pUserData;
}