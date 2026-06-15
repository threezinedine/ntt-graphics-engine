#ifndef _ARRAY_H_
#define _ARRAY_H_

#include "engine/core/memory/alloc.h"
#include "engine/core/result.h"
#include "engine/core/types.h"

#define NTT_ARRAY_INITIAL_CAPACITY 2
#define NTT_ARRAY_INDEX_NOT_FOUND  ((usize) - 1)

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

typedef b8 (*ntt_ArrayElementComparator)(void* pElement1, void* pElement2);
typedef b8 (*ntt_ArrayElementPredicate)(void* pElement);

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
 * Inserts a new element at the specified index in the array, and shifts all the subsequent elements to make space
 * 		for the new element.
 *
 * @param pArray The array to which the element will be inserted, this should be a valid array created by
 * 		ntt_ArrayCreate, and it should not be NULL.
 * @param index The index at which the new element will be inserted, this should be within the bounds of the array
 * 		(0 <= index <= length), if the index is out of bounds, error will be returned or assertion failure will be
 * triggered.
 * @param pElement The element to be inserted, this should be a pointer to the element data, and it should not be NULL.
 * @return NTT_RESULT_SUCCESS if the element is successfully inserted, or an appropriate error code if the operation
 * fails (such as NTT_RESULT_NULL_POINTER if pArray or pElement is NULL, or NTT_RESULT_INDEX_OUT_OF_BOUNDS if index is
 * out of bounds
 */
ntt_Result ntt_ArrayInsert(ntt_Array* pArray, usize index, void* pElement);

/**
 * Clears the array by setting the length to 0, but does not change the capacity or free the memory. This allows
 *    	the array to be reused without the overhead of reallocating memory.
 *
 * @param pArray The array to be cleared, this should be a valid array created by ntt_ArrayCreate, and it should
 * 		not be NULL.
 * @return NTT_RESULT_SUCCESS if the array is successfully cleared, or an appropriate error code if the operation fails
 * 		(such as NTT_RESULT_NULL_POINTER if pArray is NULL).
 */
ntt_Result ntt_ArrayClear(ntt_Array* pArray);

/**
 * Destroys the array and frees the memory used by the array. After this function is called, the array should not be
 *     used anymore, and it should be set to NULL to avoid dangling pointer issues.
 *
 * @param pArray The array to be destroyed, this should be a valid array created by ntt_ArrayCreate,
 *      and it should not be NULL.
 */
ntt_Result ntt_ArrayDestroy(ntt_Array* pArray);

/**
 * Checks if at least 1 element in the array satisfies the given predicate function.
 *
 * @param pArray The array to be checked, this should be a valid array created by
 * 		ntt_ArrayCreate, and it should not be NULL.
 * @param predicate The predicate function to be applied to each element in the array,
 * 		this should not be NULL.
 *
 * @return true if at least 1 element in the array satisfies the given predicate function, false otherwise.
 */
b8 ntt_ArrayAny(ntt_Array* pArray, ntt_ArrayElementPredicate predicate);

/**
 * Checks if all elements in the array satisfy the given predicate function.
 *
 * @param pArray The array to be checked, this should be a valid array created by
 * 		ntt_ArrayCreate, and it should not be NULL.
 * @param predicate The predicate function to be applied to each element in the array,
 * 		this should not be NULL.
 *
 * @return true if all elements in the array satisfy the given predicate function, false otherwise.
 */
b8 ntt_ArrayAll(ntt_Array* pArray, ntt_ArrayElementPredicate predicate);

/**
 * Finds the index of the first element in the array that satisfies the given predicate function.
 *
 * @param pArray The array to be searched, this should be a valid array created by
 * 		ntt_ArrayCreate, and it should not be NULL.
 * @param predicate The predicate function to be applied to each element in the array,
 * 		this should not be NULL.
 *
 * @return The index of the first element that satisfies the predicate function, or
 * 		NTT_ARRAY_INDEX_NOT_FOUND if no such element is found.
 */
usize ntt_ArrayFind(ntt_Array* pArray, ntt_ArrayElementPredicate predicate);

#endif /* _ARRAY_H_ */