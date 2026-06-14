#ifndef _ID_H_
#define _ID_H_

#include "object_type.h"
#include "result.h"
#include "types.h"

/**
 * The number which is used for managing the objectives inside the engine, each object in side the engine must
 *      contains 1 and only 1 ID, the ID is unique for each object, and it is used for identifying the object,
 */
struct ID
{
	/**
	 * The type of the object which is used for identifying the object.
	 */
	ntt_ObjectType type;
	/**
	 * If an object is updated, the version (of the id manager) will be increased by 1, this is
	 * used for the objects checking the validity of the ID, if the version is different,
	 * it means the object is updated, need to check the validity of the ID again.
	 */
	u32 version;
	/**
	 * The index of the object which is used for identifying the object.
	 */
	u64 index;
};

typedef struct ID ID;

DEFINE_RETURN_RESULT_TYPE(ID)

// #define INVALID_ID ((ID){.type = NTT_OBJECT_TYPE_NONE, .version = (u32) - 1, .index = (u64) - 1})
#define INVALID_ID_INIT {.type = NTT_OBJECT_TYPE_NONE, .version = (u32) - 1, .index = (u64) - 1}

#define INVALID_ID ((ID)INVALID_ID_INIT)

/**
 * Be called at the start of the engine, this will starting up, allocate the resources for the system.
 */
ntt_Result ntt_InitializeIDSystem();

/**
 * Be called at the end of the engine, this will clean up, deallocate the resources for the system.
 */
ntt_Result ntt_DestroyIDSystem();

/**
 * Each time needa get a new ID, use this function, it will automatically return a new, unique ID for the object,
 * 		later, other methods will use this ID for interacting with the id system.
 *
 * @param type The type of the object which is used for identifying the object.
 * @return A new, unique ID for the object.
 */
IDResult ntt_NewID(ntt_ObjectType type);

/**
 * Check if the ID is valid or not (the version of the ID is the same as the version at the ID manager).
 *
 * @param pId The ID to be checked. If NULL, it will return FALSE.
 * @return TRUE if the ID is valid (type, version, index are matched), FALSE otherwise.
 */
b8 ntt_IsIDValid(ID* pId);

/**
 * Obtain the copied ID from the ID manager, this is used for the objects to identify themselves.
 *
 * @param pId The ID to be obtained, if the ID is invalid, it will return an invalid ID. No matter the version, type
 * 		of the ID, the same index is used only for this method.
 * @return The copied ID from the ID manager, if the ID is invalid, it will return an invalid ID.
 */
IDResult ntt_GetIDByID(ID* pId);

/**
 * Notify that the ID has been updated, this will increase the version of the ID.
 *
 * @param id The ID to be updated.
 */
ntt_Result ntt_UpdateID(ID* id);

/**
 * Check if the two IDs are equal or not, this is used for the objects to compare with each other.
 *
 * @param a The first ID to be compared.
 * @param b The second ID to be compared.
 * @return TRUE if the two IDs are equal (type, version, index are matched), FALSE otherwise.
 */
b8 ntt_IsIDEqual(ID* a, ID* b);

#endif /* _ID_H_ */