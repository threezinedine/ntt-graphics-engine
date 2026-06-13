#include "engine/core/id.h"
#include "engine/core/defs.h"

#define MAX_OBJECTS 1000

static u64 s_currentIndex = 0;
static u64 s_IDMetas[MAX_OBJECTS];

#define INVALID_META							  ((u64) - 1)
#define META_FROM_TYPE_AND_VERSION(type, version) (((u64)type << 32) | version)
#define TYPE_FROM_META(meta)					  ((meta >> 32) & 0xFFFFFFFF)
#define VERSION_FROM_META(meta)					  (meta & 0xFFFFFFFF)

void ntt_InitializeIDSystem()
{
	s_currentIndex = 0;
	for (u64 i = 0; i < MAX_OBJECTS; i++)
	{
		s_IDMetas[i] = INVALID_META;
	}
}

void ntt_DestroyIDSystem()
{
	s_currentIndex = 0;
	for (u64 i = 0; i < MAX_OBJECTS; i++)
	{
		s_IDMetas[i] = INVALID_META;
	}
}

ID ntt_NewID(ntt_ObjectType type)
{
	NTT_ASSERT_M(s_currentIndex < MAX_OBJECTS, "Exceeded the maximum number of objects: %llu", MAX_OBJECTS);
	u64 index = s_currentIndex;
	s_currentIndex++;

	s_IDMetas[index] = META_FROM_TYPE_AND_VERSION(type, 0);

	return (ID){.type = type, .version = 0, .index = index};
}

b8 ntt_IsIDValid(ID* pId)
{
	if (pId == NULL)
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

void ntt_UpdateID(ID* id)
{
	NTT_ASSERT_M(id != NULL, "ID pointer is NULL");
	u64 meta = s_IDMetas[id->index];

	NTT_ASSERT_M(ntt_IsIDValid(id), "Cannot update an invalid ID");

	u64 type	= TYPE_FROM_META(meta);
	u64 version = VERSION_FROM_META(meta);

	version++;

	s_IDMetas[id->index] = META_FROM_TYPE_AND_VERSION(type, version);
	id->version			 = (u32)version;
}

ID ntt_GetIDByID(ID* pId)
{
	u64 meta = s_IDMetas[pId->index];

	if (meta == INVALID_META)
	{
		return INVALID_ID;
	}

	return (ID){.type = TYPE_FROM_META(meta), .version = VERSION_FROM_META(meta), .index = pId->index};
}

b8 ntt_IsIDEqual(ID* a, ID* b)
{
	if (a == NULL || b == NULL)
	{
		return FALSE;
	}

	return a->type == b->type && a->version == b->version && a->index == b->index;
}