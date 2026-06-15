#ifndef _ARRAY_H_
#define _ARRAY_H_

#include "engine/core/memory/alloc.h"
#include "engine/core/result.h"
#include "engine/core/types.h"

#define NTT_ARRAY_INITIAL_CAPACITY 2

/**
 * Internal structure for the array the size of the array is automatically assigned if the array append
 *      new element beyond its capacity
 */
struct ntt_Array
{
	void*		   pData;
	usize		   elementSize;
	usize		   length;
	usize		   capacity;
	ntt_Allocator* pAllocator; // The allocator used for managing the memory of the array, this is stored in the array
							   // structure for future use when resizing the array, this can be NULL if the default
							   // malloc allocator is used
};

typedef struct ntt_Array ntt_Array;
DEFINE_RETURN_RESULT_TYPE(ntt_Array)

/**
 * Making a new array with all NULL values, the length at the beginning is 0
 *      and the capacity is determined by the initialCapacity parameter.
 *
 * @param elementSize The size of each element in the array (use sizeof(type))
 * @param initialCapacity The initial capacity of the array, this is the number
 *      of elements that the array can hold before it needs to resize.
 * @param pAllocator The allocator to be used for memory management. Can be NULL -> use
 *      malloc allocator by default.
 */
ntt_ArrayResult ntt_ArrayCreate(usize elementSize, usize initialCapacity, ntt_Allocator* pAllocator);

/**
 * Destroys the old array and creates a new array with the same elements and new capacity.
 *
 * @param pArray The array to be resized, this should be a valid array created by ntt_ArrayCreate,
 *      and it should not be NULL.
 * @param newCapacity The new capacity of the array, this should be greater than or equal to the
 *      current length of the array.
 */
ntt_Result ntt_ArrayResize(ntt_Array* pArray, usize newCapacity);

/**
 * Appends a new element to the end of the array, if the length of the array exceeds its capacity after appending,
 *      the array will be resized to double its current capacity.
 *
 * @param pArray The array to which the element will be appended, this should be a valid array created by
 *      ntt_ArrayCreate, and it should not be NULL.
 * @param pElement The element to be appended, this should be a pointer to the element data, and it should not be NULL.
 */
ntt_Result ntt_ArrayAppend(ntt_Array* pArray, void* pElement);

/**
 * Gets the pointer to the element at the specified index in the array. The caller should ensure that the index is
 *     within the bounds of the array (0 <= index < length), otherwise, return NULL.
 */
void* ntt_ArrayGet(ntt_Array* pArray, usize index);

/**
 * Erases the element at the specified index in the array, and shifts all the subsequent elements to fill the gap.
 *
 * @param pArray The array from which the element will be erased, this should be a valid array created by
 *      ntt_ArrayCreate, and it should not be NULL.
 * @param index if the index is out of bounds, error will be returned or assertion failure will be triggered.
 */
ntt_Result ntt_ArrayErase(ntt_Array* pArray, usize index);

/**
 * Destroys the array and frees the memory used by the array. After this function is called, the array should not be
 *     used anymore, and it should be set to NULL to avoid dangling pointer issues.
 *
 * @param pArray The array to be destroyed, this should be a valid array created by ntt_ArrayCreate,
 *      and it should not be NULL.
 */
ntt_Result ntt_ArrayDestroy(ntt_Array* pArray);

#endif /* _ARRAY_H_ */