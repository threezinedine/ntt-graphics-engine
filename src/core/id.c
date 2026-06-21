#include "engine/core/id.h"
#include "engine/core/defs.h"
#include "engine/core/logging/logging.h"

static u64 s_currentIndex = 0;
static u64 s_IDMetas[MAX_OBJECTS];

#define INVALID_META							  ((u64) - 1)
#define META_FROM_TYPE_AND_VERSION(type, version) (((u64)type << 32) | version)
#define TYPE_FROM_META(meta)					  ((meta >> 32) & 0xFFFFFFFF)
#define VERSION_FROM_META(meta)					  (meta & 0xFFFFFFFF)

ntt_Result ntt_InitializeIDSystem()
{
	NTT_CORE_INFO("Initializing ID System...");
	s_currentIndex = 0;
	for (u64 i = 0; i < MAX_OBJECTS; i++)
	{
		s_IDMetas[i] = INVALID_META;
	}
	return NTT_RESULT_SUCCESS;
}

ntt_Result ntt_DestroyIDSystem()
{
	s_currentIndex = 0;
	for (u64 i = 0; i < MAX_OBJECTS; i++)
	{
		s_IDMetas[i] = INVALID_META;
	}

	NTT_CORE_INFO("ID System destroyed.");

	return NTT_RESULT_SUCCESS;
}

IDResult ntt_NewID(ntt_ObjectType type, void* pUserData)
{
	NTT_ASSERT_IF(s_currentIndex >= MAX_OBJECTS)
	{
		return (IDResult){.result = NTT_RESULT_EXCEEDED_MAX_OBJECTS, .data = INVALID_ID};
	}

	u64 index = s_currentIndex;
	s_currentIndex++;

	s_IDMetas[index] = META_FROM_TYPE_AND_VERSION(type, 0);

	NTT_CORE_TRACE("Created new ID: type=%s, index=%u", ntt_ObjectType_ToString(type), index);

	return (IDResult){
		.result = NTT_RESULT_SUCCESS, .data = {.type = type, .version = 0, .index = index, .pUserData = pUserData}
	  };
}

b8 ntt_IsIDValid(ID* pId)
{
	NTT_ASSERT_IF(pId == NULL)
	{
		return FALSE;
	}

	u64 meta = s_IDMetas[pId->index];

	if (meta == INVALID_META)
	{
		return FALSE;
	}

	if (TYPE_FROM_META(meta) != pId->type || VERSION_FROM_META(meta) != pId->version)
	{
		return FALSE;
	}

	return TRUE;
}

ntt_Result ntt_UpdateID(ID* id)
{
	NTT_ASSERT_IF(id == NULL)
	{
		return NTT_RESULT_UPDATE_NULL_ID;
	}
	u64 meta = s_IDMetas[id->index];

	NTT_ASSERT_IF(!ntt_IsIDValid(id))
	{
		return NTT_RESULT_UPDATE_INVALID_ID;
	}

	u64 type	= TYPE_FROM_META(meta);
	u64 version = VERSION_FROM_META(meta);

	version++;

	s_IDMetas[id->index] = META_FROM_TYPE_AND_VERSION(type, version);
	id->version			 = (u32)version;

	return NTT_RESULT_SUCCESS;
}

IDResult ntt_GetIDByID(ID* pId)
{
	u64 meta = s_IDMetas[pId->index];

	NTT_ASSERT_IF(meta == INVALID_META)
	{
		return (IDResult){.result = NTT_RESULT_ID_IS_NOT_ALLOCATED, .data = INVALID_ID};
	}

	return (IDResult){
		.result = NTT_RESULT_SUCCESS,
		.data	= {.type	  = TYPE_FROM_META(meta),
				   .version	  = VERSION_FROM_META(meta),
				   .index	  = pId->index,
				   .pUserData = pId->pUserData}
	   };
}

b8 ntt_IsIDEqual(ID* a, ID* b)
{
	if (a == NULL || b == NULL)
	{
		return FALSE;
	}

	if (a->type == b->type && a->version == b->version && a->index == b->index)
	{
		return TRUE;
	}

	return FALSE;
}