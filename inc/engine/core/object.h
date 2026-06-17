#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "engine/core/defs.h"
#include "engine/core/utils/utils.h"
#include "id.h"
#include "memory/alloc.h"
#include "result.h"

/**
 * The based for all things inside the game engine. This provides the common interface for all objects.
 *      - Listener for events
 *      - Has an unique ID
 */
struct ntt_Object
{
	/**
	 * Unique identifier for any object, must be allocated, if wanna use the object, must actively check
	 *      the validity of the ID first.
	 */
	ID id;
	/**
	 * The type of the object which is used for type checking.
	 */
	ID type;
	/**
	 * The allocator which is used for allocating the memory for this object, this is used for the object to manage its
	 *      own memory, when the object is destroyed, it use this allocator to free the memory if needed.
	 */
	ntt_Allocator* pAllocator;
};

#define OBJECT_ID_DECLARE(type)                                                                                        \
	extern ID  type##ID;                                                                                               \
	ntt_Result type##_RegisterType();                                                                                  \
	ntt_Result type##_UnregisterType();

#define OBJECT_ID_DEFINE(objectType, baseType)                                                                         \
	ID		   objectType##ID = INVALID_ID_INIT;                                                                       \
	ntt_Result objectType##_RegisterType()                                                                             \
	{                                                                                                                  \
		IDResult result = ntt_NewID(NTT_OBJECT_TYPE_OBJECT_ID, &baseType##ID);                                         \
		NTT_SUCCESS_ASSERT_VAR(result);                                                                                \
		objectType##ID = result.data;                                                                                  \
		return NTT_RESULT_SUCCESS;                                                                                     \
	}                                                                                                                  \
	ntt_Result objectType##_UnregisterType()                                                                           \
	{                                                                                                                  \
		objectType##ID = INVALID_ID;                                                                                   \
		return NTT_RESULT_SUCCESS;                                                                                     \
	}

/**
 * Helper macro for inheriting mechanism for the object and its derived types.
 */
#define OBJECT_DECLARE(type)                                                                                           \
	typedef struct type type;                                                                                          \
	ntt_Result			type##_Initialize(type* pObject, ntt_Allocator* pAllocator);                                   \
	ntt_Result			type##_Destroy(type*);                                                                         \
	b8					type##_IsTypeOf(type* pObject);                                                                \
	b8					type##_HasInstance(type* pObject);                                                             \
	OBJECT_ID_DECLARE(type)

#define INHERIT_CHECKING(objType)                                                                                      \
	b8 objType##_IsTypeOf(objType* pObject)                                                                            \
	{                                                                                                                  \
		NTT_ASSERT(pObject != NULL);                                                                                   \
		return ntt_IsIDEqual(&((ntt_Object*)pObject)->type, &objType##ID) ? TRUE : FALSE;                              \
	}                                                                                                                  \
	b8 objType##_HasInstance(objType* pObject)                                                                         \
	{                                                                                                                  \
		NTT_ASSERT(pObject != NULL);                                                                                   \
		if (objType##_IsTypeOf(pObject))                                                                               \
		{                                                                                                              \
			return TRUE;                                                                                               \
		}                                                                                                              \
		ID* objBaseTypeID = ntt_Object_GetBaseTypeID((ntt_Object*)pObject);                                            \
		if (objBaseTypeID == NULL)                                                                                     \
		{                                                                                                              \
			return FALSE;                                                                                              \
		}                                                                                                              \
		while (1)                                                                                                      \
		{                                                                                                              \
			if (ntt_IsIDEqual(objBaseTypeID, &objType##ID) == TRUE)                                                    \
			{                                                                                                          \
				return TRUE;                                                                                           \
			}                                                                                                          \
			ntt_ConsolePrint("is null: %d", objBaseTypeID == NULL); /* Debugging print, can be removed later */        \
			objBaseTypeID = (ID*)(objBaseTypeID->pUserData); /* Assuming pUserData points to the base type ID */       \
			if (objBaseTypeID == NULL)                                                                                 \
			{                                                                                                          \
				return FALSE;                                                                                          \
			}                                                                                                          \
			/* If the base type ID is NULL, break the loop */                                                          \
			if (objBaseTypeID->type !=                                                                                 \
				NTT_OBJECT_TYPE_OBJECT_ID) /* If the base type ID is not of type Object, break the loop */             \
			{                                                                                                          \
				break;                                                                                                 \
			}                                                                                                          \
		}                                                                                                              \
		return FALSE;                                                                                                  \
	}

#define OBJECT_DEFINE(derivedType, baseType)                                                                           \
	OBJECT_ID_DEFINE(derivedType, baseType)                                                                            \
	INHERIT_CHECKING(derivedType)

#define OBJECT_INITIALIZE(pObj, derivedType, baseType)                                                                 \
	NTT_SUCCESS_ASSERT(baseType##_Initialize(&(pObj->base), pAllocator));                                              \
	((ntt_Object*)(pObj))->type = derivedType##ID;

#define OBJECT_DESTROY(pObj, derivedType, baseType) NTT_SUCCESS_ASSERT(baseType##_Destroy(&(pObj->base)));

OBJECT_DECLARE(ntt_Object)

#define REGISTER_SYSTEM_DECLARE(system)                                                                                \
	ntt_Result ntt_##system##_Register();                                                                              \
	ntt_Result ntt_##system##_Unregister();

ID* ntt_Object_GetBaseTypeID(ntt_Object* pObject);

#endif /* _OBJECT_H_ */