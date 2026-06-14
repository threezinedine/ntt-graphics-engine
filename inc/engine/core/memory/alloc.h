#ifndef _ALLOC_H_
#define _ALLOC_H_

#include "engine/core/result.h"
#include "engine/core/types.h"

struct voidPtrResult
{
	ntt_Result result;
	void*	   pData;
};

typedef struct voidPtrResult voidPtrResult;

/**
 * The common internal state of the allocator interface, which is used for allocating and deallocating memory
 *      inside the whole engine. This engine provides a various types of allocators, which are used for
 *      different purposes, such as stack allocator, linear allocator, pool allocator, etc. Each allocator has
 *      its own implementation and characteristics, but they all share the same interface defined by this structure.
 */
typedef struct ntt_Allocator
{
	/**
	 * The dedicated data for the specific allocator implementation, which is used for storing the internal
	 *      state of the allocator, such as the memory pool, the stack pointer, the free list, etc. This data
	 *      is opaque to the user of the allocator.
	 */
	void* pInternalState;

	/**
	 * The function pointer which is the detailed implementation of the allocation logic for the specific allocator.
	 * @param allocator The allocator instance which is used for allocating memory. This parameter is passed to the
	 *      allocation function, so that it can access the internal state of the allocator and perform the allocation
	 *      logic accordingly. This parameter can not be NULL or trigger an assertion failure.
	 *
	 * @param size The size of the memory block to be allocated, in bytes. This parameter is passed to the allocation
	 *      function, so that it can determine how much memory to allocate and return a pointer to the allocated block.
	 *
	 * @return A pointer to the allocated memory block, or NULL if the allocation fails. The behavior of this return
	 *      value depends on the specific allocator implementation, but it generally indicates whether the allocation
	 *      was successful or not. If NULL is returned, it means that the allocation failed, the owner should handle
	 *      the failure case accordingly, such as logging an error message, trying a different allocator, etc.
	 */
	voidPtrResult (*allocate)(struct ntt_Allocator* allocator, usize size);

	/**
	 * The function pointer which is the detailed implementation of the deallocation logic for the specific allocator.
	 * The behavior of this function depends on the specific allocator implementation, but it generally frees the memory
	 *      block pointed by the given pointer, making it available for future allocations. The specific deallocation
	 *      logic may vary based on the allocator type, such as adding the block back to a free list, resetting a stack
	 *      pointer, or simply ignoring the deallocation in a linear allocator.
	 *
	 * @param allocator The allocator instance which is used for deallocating memory. This parameter is passed to the
	 *      deallocation function, so that it can access the internal state of the allocator and perform the
	 *      deallocation logic accordingly. This parameter can not be NULL or trigger an assertion failure.
	 *
	 * @param ptr The pointer to the memory block to be deallocated. This parameter is passed to the deallocation
	 *      function, so that it can identify which memory block to free based on the specific allocator's internal
	 *      state and logic. The behavior of this parameter depends on the specific allocator implementation, but it
	 *      generally indicates the memory block that should be deallocated. This parameter can not be NULL or trigger
	 *      an assertion failure.
	 *
	 * @param size The size of the memory block to be deallocated, in bytes. This parameter is used only for debugging
	 *      purposes, which can help the allocator implementation to verify the correctness of the deallocation
	 *      logic, such as checking for for allocated size, detecting the memory corruption, etc. The behavior of this
	 *      parameter depends on the specific allocator implementation, but it generally provides additional information
	 *      for debugging purposes.
	 */
	ntt_Result (*deallocate)(struct ntt_Allocator* allocator, void* ptr, usize size);

	/**
	 * The function pointer which is the detailed implementation of the destroy logic for the specific allocator, which
	 *      is used for cleaning up the internal state of the allocator and freeing any resources associated with it.
	 *      The behavior of this function depends on the specific allocator implementation, but it generally performs
	 *      the necessary cleanup operations.
	 *
	 * @param allocator The allocator instance which is used for destroying the allocator. This parameter is passed to
	 *      the destroy function, so that it can access the internal state of the allocator and perform the necessary
	 *      cleanup operations accordingly. This parameter can not be NULL or trigger an assertion failure.
	 */
	ntt_Result (*destroy)(struct ntt_Allocator* allocator);
} ntt_Allocator;

struct AllocatorResult
{
	ntt_Result	   result;
	ntt_Allocator* pAllocator;
};

typedef struct AllocatorResult AllocatorResult;

/**
 * Interface for allocating a memory block of the specified size using the given allocator.
 *      The behavior of this function depends on the specific allocator implementation,
 *      but it generally returns a pointer to a memory block of the requested size,
 *      or NULL if the allocation fails.
 * @param allocator The allocator instance which is used for allocating memory. This parameter is passed to the
 *      allocation function, so that it can access the internal state of the allocator and perform the allocation logic
 *      accordingly.
 * @param size The size of the memory block to be allocated, in bytes. This parameter
 *      is passed to the allocation function, so that it can determine how much memory to allocate and return a pointer
 *      to the allocated block.
 *
 * @return A pointer to the allocated memory block, or NULL if the allocation fails. The behavior of this return value
 *      depends on the specific allocator implementation, but it generally indicates whether the allocation was
 *      successful or not. If NULL is returned, it means that the allocation failed, the owner should handle the failure
 *      case accordingly, such as logging an error message, trying a different allocator, etc.
 */
voidPtrResult ntt_Allocate(ntt_Allocator* allocator, usize size);

/**
 * Interface for deallocating a memory block pointed by the given pointer using the specified allocator. The behavior of
 *      this function depends on the specific allocator implementation, but it generally frees the memory block
 *      pointed by the given pointer, making it available for future allocations. The specific deallocation logic may
 *      vary based on the allocator type, such as adding the block back to a free list, resetting a stack pointer, or
 *      simply ignoring the deallocation in a linear allocator.
 *
 * @param allocator The allocator instance which is used for deallocating memory. This parameter is passed to the
 *      deallocation function, so that it can access the internal state of the allocator and perform the deallocation
 *      logic accordingly. This parameter can not be NULL or trigger an assertion failure.
 *
 * @param ptr The pointer to the memory block to be deallocated. This parameter is passed to the deallocation function,
 *      so that it can identify which memory block to free based on the specific allocator's internal state and logic.
 *      The behavior of this parameter depends on the specific allocator implementation, but it generally indicates the
 *      memory block that should be deallocated. This parameter can not be NULL or trigger an assertion failure.
 *
 * @param size The size of the memory block to be deallocated, in bytes. This parameter is used only for debugging
 *      purposes, which can help the allocator implementation to verify the correctness of the deallocation logic, such
 *      as checking for for allocated size, detecting the memory corruption, etc. The behavior of this parameter depends
 *      on the specific allocator implementation, but it generally provides additional information for the deallocation
 *      process, which can be useful for debugging and error checking. At the `Release` build, this parameter is
 *      ignored.
 *
 */
ntt_Result ntt_Deallocate(ntt_Allocator* allocator, void* ptr, usize size);

/**
 * The method for destroying the any types of the allocator instance, which is used for cleaning up the internal
 *      state of the allocator and freeing any resources associated with it. The behavior of this function depends
 *      on the specific allocator implementation, but it generally performs the necessary cleanup operations.
 *
 * @param allocator The allocator instance which is used for destroying the allocator. This parameter is passed to the
 *      destroy function, so that it can access the internal state of the allocator and perform the necessary cleanup
 *      operations accordingly. This parameter can not be NULL or trigger an assertion failure.
 */
ntt_Result ntt_DestroyAllocator(ntt_Allocator* allocator);

#endif /* _ALLOC_H_ */