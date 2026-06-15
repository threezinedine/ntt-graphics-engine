#include "engine/core/memory/alloc.h"
#include "engine/core/defs.h"
#include "engine/core/types.h"

voidPtrResult ntt_Allocate(ntt_Allocator* allocator, usize size)
{
	NTT_ASSERT_IF(allocator == NULL)
	{
		return (voidPtrResult){.result = NTT_RESULT_NULL_POINTER, .pData = NULL};
	}

	NTT_ASSERT_IF(allocator->allocate == NULL)
	{
		return (voidPtrResult){.result = NTT_RESULT_NULL_POINTER, .pData = NULL};
	}

	return allocator->allocate(allocator, size);
}

ntt_Result ntt_Deallocate(ntt_Allocator* allocator, void* ptr, usize size)
{
	NTT_ASSERT_IF(allocator == NULL)
	{
		return NTT_RESULT_NULL_POINTER;
	}

	NTT_ASSERT_IF(allocator->deallocate == NULL)
	{
		return NTT_RESULT_NULL_POINTER;
	}

	return allocator->deallocate(allocator, ptr, size);
}

ntt_Result ntt_DestroyAllocator(ntt_Allocator* allocator)
{
	NTT_ASSERT_IF(allocator == NULL)
	{
		return NTT_RESULT_NULL_POINTER;
	}

	NTT_ASSERT_IF(allocator->destroy == NULL)
	{
		return NTT_RESULT_NULL_POINTER;
	}

	return allocator->destroy(allocator);
}