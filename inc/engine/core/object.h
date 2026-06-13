#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "engine/core/defs.h"
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
	ntt_Result type##RegisterType();                                                                                   \
	ntt_Result type##UnregisterType();

#define OBJECT_ID_DEFINE(objectType, baseType)                                                                         \
	ID		   objectType##ID = INVALID_ID_INIT;                                                                       \
	static ID* s_pBaseTypeID  = NULL;                                                                                  \
	ntt_Result objectType##RegisterType()                                                                              \
	{                                                                                                                  \
		objectType##ID = ntt_NewID(NTT_OBJECT_TYPE_OBJECT_ID);                                                         \
		s_pBaseTypeID  = &baseType##ID;                                                                                \
		return NTT_RESULT_SUCCESS;                                                                                     \
	}                                                                                                                  \
	ntt_Result objectType##UnregisterType()                                                                            \
	{                                                                                                                  \
		objectType##ID = INVALID_ID;                                                                                   \
		s_pBaseTypeID  = NULL;                                                                                         \
		return NTT_RESULT_SUCCESS;                                                                                     \
	}

/**
 * Helper macro for inheriting mechanism for the object and its derived types.
 */
#define OBJECT_DECLARE(type)                                                                                           \
	typedef struct type type;                                                                                          \
	ntt_Result			type##Initialize(type* pObject, ntt_Allocator* pAllocator);                                    \
	ntt_Result			type##Destroy(type*);                                                                          \
	b8					type##IsInstanceOf(type* pObject);                                                             \
	b8					type##IsDerivedFrom(type* pObject);                                                            \
	OBJECT_ID_DECLARE(type)

#define OBJECT_DEFINE(derivedType, baseType)                                                                           \
	OBJECT_ID_DEFINE(derivedType, baseType)                                                                            \
	b8 derivedType##IsInstanceOf(derivedType* pObject)                                                                 \
	{                                                                                                                  \
		NTT_ASSERT(pObject != NULL);                                                                                   \
		return ntt_IsIDEqual(&((ntt_Object*)pObject)->type, &derivedType##ID) ? TRUE : FALSE;                          \
	}                                                                                                                  \
	b8 derivedType##IsDerivedFrom(derivedType* pObject)                                                                \
	{                                                                                                                  \
		NTT_ASSERT(pObject != NULL);                                                                                   \
		if (derivedType##IsInstanceOf(pObject))                                                                        \
		{                                                                                                              \
			return TRUE;                                                                                               \
		}                                                                                                              \
		if (ntt_IsIDEqual(s_pBaseTypeID, &INVALID_ID) == TRUE)                                                         \
		{                                                                                                              \
			return baseType##IsDerivedFrom((baseType*)pObject);                                                        \
		}                                                                                                              \
		else                                                                                                           \
		{                                                                                                              \
			return FALSE;                                                                                              \
		}                                                                                                              \
	}

#define OBJECT_INITIALIZE(pObj, derivedType, baseType)                                                                 \
	NTT_SUCCESS_ASSERT(baseType##Initialize(&(pObj->base), pAllocator));                                               \
	((ntt_Object*)(pObj))->type = derivedType##ID;

#define OBJECT_DESTROY(pObj, derivedType, baseType) NTT_SUCCESS_ASSERT(baseType##Destroy(&(pObj->base)));

OBJECT_DECLARE(ntt_Object)

#endif /* _OBJECT_H_ */