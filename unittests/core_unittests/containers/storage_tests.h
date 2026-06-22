#ifndef _STORAGE_TESTS_H_
#define _STORAGE_TESTS_H_

#include "engine/engine.h"
#include "tools/tools.h"

/**
 * Helper to zero-initialize a storage struct so that Initialize checks pass.
 */
static void zero_storage(ntt_Storage* pStorage)
{
	ntt_Memset(pStorage, 0, sizeof(ntt_Storage));
}

void storage_tests_before_each()
{
	ntt_Result result = ntt_MemoryGlobals_Initialize();
	TEST_ASSERT(result == NTT_RESULT_SUCCESS);
}

void storage_tests_after_each()
{
	ntt_Result result = ntt_MemoryGlobals_Destroy();
	TEST_ASSERT(result == NTT_RESULT_SUCCESS);
}

/**
 * Callback counters for verifying that callbacks are invoked correctly.
 */
static i32 g_storageCreateCount	 = 0;
static i32 g_storageNewCount	 = 0;
static i32 g_storageFreeCount	 = 0;
static i32 g_storageDestroyCount = 0;

static ntt_Result storage_create_callback(ntt_Storage* pStorage)
{
	(void)pStorage;
	g_storageCreateCount++;
	return NTT_RESULT_SUCCESS;
}

static ntt_Result storage_new_callback(ntt_Storage* pStorage)
{
	(void)pStorage;
	g_storageNewCount++;
	return NTT_RESULT_SUCCESS;
}

static ntt_Result storage_free_callback(ntt_Storage* pStorage, ID elementID)
{
	(void)pStorage;
	(void)elementID;
	g_storageFreeCount++;
	return NTT_RESULT_SUCCESS;
}

static ntt_Result storage_destroy_callback(ntt_Storage* pStorage)
{
	(void)pStorage;
	g_storageDestroyCount++;
	return NTT_RESULT_SUCCESS;
}

TEST_CASE(InitializeStorage)
{
	ntt_Storage storage;
	zero_storage(&storage);

	ntt_Storage_CreateInfo createInfo = {0};
	ntt_Result			   result	  = ntt_Storage_Initialize(&storage, NULL, sizeof(i32), 4, &createInfo);
	TEST_ASSERT(result == NTT_RESULT_SUCCESS);

	TEST_ASSERT(storage.pAllocator != NULL);
	TEST_ASSERT(storage.blockSize == sizeof(i32));
	TEST_ASSERT(storage.blockCount == 4);

	ntt_Result destroyResult = ntt_Storage_Destroy(&storage);
	TEST_ASSERT(destroyResult == NTT_RESULT_SUCCESS);
}

TEST_CASE(InitializeStorageWithAllocator)
{
	ntt_Storage storage;
	zero_storage(&storage);

	ntt_Storage_CreateInfo createInfo = {0};
	ntt_Allocator*		   pAllocator = g_memoryGlobals.mallocAllocator;
	ntt_Result			   result	  = ntt_Storage_Initialize(&storage, pAllocator, sizeof(i32), 4, &createInfo);
	TEST_ASSERT(result == NTT_RESULT_SUCCESS);

	TEST_ASSERT(storage.pAllocator == pAllocator);

	ntt_Result destroyResult = ntt_Storage_Destroy(&storage);
	TEST_ASSERT(destroyResult == NTT_RESULT_SUCCESS);
}

TEST_CASE(InitializeNullPointer)
{
	ntt_Result result = ntt_Storage_Initialize(NULL, NULL, sizeof(i32), 4, NULL);
	TEST_ASSERT(result == NTT_RESULT_NULL_POINTER);
}

TEST_CASE(AlreadyInitialized)
{
	ntt_Storage storage;
	zero_storage(&storage);

	ntt_Storage_CreateInfo createInfo = {0};
	ntt_Result			   result	  = ntt_Storage_Initialize(&storage, NULL, sizeof(i32), 4, &createInfo);
	TEST_ASSERT(result == NTT_RESULT_SUCCESS);

	// Initialize again should fail
	result = ntt_Storage_Initialize(&storage, NULL, sizeof(i32), 4, &createInfo);
	TEST_ASSERT(result == NTT_RESULT_ALREADY_INITIALIZED);

	ntt_Result destroyResult = ntt_Storage_Destroy(&storage);
	TEST_ASSERT(destroyResult == NTT_RESULT_SUCCESS);
}

TEST_CASE(NewAndFree)
{
	ntt_Storage storage;
	zero_storage(&storage);

	ntt_Storage_CreateInfo createInfo = {0};
	ntt_Result			   result	  = ntt_Storage_Initialize(&storage, NULL, sizeof(i32), 4, &createInfo);
	TEST_ASSERT(result == NTT_RESULT_SUCCESS);

	IDResult newResult = ntt_Storage_New(&storage);
	TEST_ASSERT(newResult.result == NTT_RESULT_SUCCESS);
	TEST_ASSERT(newResult.data.type != NTT_OBJECT_TYPE_NONE);

	ntt_Result freeResult = ntt_Storage_Free(&storage, newResult.data);
	TEST_ASSERT(freeResult == NTT_RESULT_SUCCESS);

	ntt_Result destroyResult = ntt_Storage_Destroy(&storage);
	TEST_ASSERT(destroyResult == NTT_RESULT_SUCCESS);
}

TEST_CASE(ReuseFreedID)
{
	ntt_Storage storage;
	zero_storage(&storage);

	ntt_Storage_CreateInfo createInfo = {0};
	ntt_Result			   result	  = ntt_Storage_Initialize(&storage, NULL, sizeof(i32), 4, &createInfo);
	TEST_ASSERT(result == NTT_RESULT_SUCCESS);

	IDResult firstNew = ntt_Storage_New(&storage);
	TEST_ASSERT(firstNew.result == NTT_RESULT_SUCCESS);

	IDResult secondNew = ntt_Storage_New(&storage);
	TEST_ASSERT(secondNew.result == NTT_RESULT_SUCCESS);

	// Free the first one
	ntt_Result freeResult = ntt_Storage_Free(&storage, firstNew.data);
	TEST_ASSERT(freeResult == NTT_RESULT_SUCCESS);

	// Now get a new one — it should reuse the freed ID
	IDResult thirdNew = ntt_Storage_New(&storage);
	TEST_ASSERT(thirdNew.result == NTT_RESULT_SUCCESS);

	// The reused ID should match the first one (since it was returned to the free stack)
	TEST_ASSERT(thirdNew.data.index == firstNew.data.index);

	ntt_Result destroyResult = ntt_Storage_Destroy(&storage);
	TEST_ASSERT(destroyResult == NTT_RESULT_SUCCESS);
}

TEST_CASE(StorageFull)
{
	ntt_Storage storage;
	zero_storage(&storage);

	ntt_Storage_CreateInfo createInfo = {0};
	ntt_Result			   result	  = ntt_Storage_Initialize(&storage, NULL, sizeof(i32), 2, &createInfo);
	TEST_ASSERT(result == NTT_RESULT_SUCCESS);

	IDResult newResult1 = ntt_Storage_New(&storage);
	TEST_ASSERT(newResult1.result == NTT_RESULT_SUCCESS);

	IDResult newResult2 = ntt_Storage_New(&storage);
	TEST_ASSERT(newResult2.result == NTT_RESULT_SUCCESS);

	// Third allocation should fail since blockCount is 2
	IDResult newResult3 = ntt_Storage_New(&storage);
	TEST_ASSERT(newResult3.result == NTT_RESULT_STORAGE_IS_FULL);

	ntt_Result destroyResult = ntt_Storage_Destroy(&storage);
	TEST_ASSERT(destroyResult == NTT_RESULT_SUCCESS);
}

TEST_CASE(FreeAndRefillStorage)
{
	ntt_Storage storage;
	zero_storage(&storage);

	ntt_Storage_CreateInfo createInfo = {0};
	ntt_Result			   result	  = ntt_Storage_Initialize(&storage, NULL, sizeof(i32), 2, &createInfo);
	TEST_ASSERT(result == NTT_RESULT_SUCCESS);

	IDResult newResult1 = ntt_Storage_New(&storage);
	TEST_ASSERT(newResult1.result == NTT_RESULT_SUCCESS);

	IDResult newResult2 = ntt_Storage_New(&storage);
	TEST_ASSERT(newResult2.result == NTT_RESULT_SUCCESS);

	// Free both
	TEST_ASSERT(ntt_Storage_Free(&storage, newResult1.data) == NTT_RESULT_SUCCESS);
	TEST_ASSERT(ntt_Storage_Free(&storage, newResult2.data) == NTT_RESULT_SUCCESS);

	// Fill again — should work since the free stack has 2 entries
	IDResult newResult3 = ntt_Storage_New(&storage);
	TEST_ASSERT(newResult3.result == NTT_RESULT_SUCCESS);
	TEST_ASSERT(newResult3.data.index == newResult2.data.index); // Should reuse the last freed ID

	IDResult newResult4 = ntt_Storage_New(&storage);
	TEST_ASSERT(newResult4.result == NTT_RESULT_SUCCESS);
	TEST_ASSERT(newResult4.data.index == newResult1.data.index); // Should reuse the first freed ID

	// Now it should be full again
	IDResult newResult5 = ntt_Storage_New(&storage);
	TEST_ASSERT(newResult5.result == NTT_RESULT_STORAGE_IS_FULL);

	ntt_Result destroyResult = ntt_Storage_Destroy(&storage);
	TEST_ASSERT(destroyResult == NTT_RESULT_SUCCESS);
}

TEST_CASE(NewNullPointer)
{
	IDResult result = ntt_Storage_New(NULL);
	TEST_ASSERT(result.result == NTT_RESULT_NULL_POINTER);
}

TEST_CASE(FreeNullPointer)
{
	ntt_Result result = ntt_Storage_Free(NULL, INVALID_ID);
	TEST_ASSERT(result == NTT_RESULT_NULL_POINTER);
}

TEST_CASE(DestroyNullPointer)
{
	ntt_Result result = ntt_Storage_Destroy(NULL);
	TEST_ASSERT(result == NTT_RESULT_NULL_POINTER);
}

TEST_CASE(NewElementCallback)
{
	g_storageNewCount = 0;

	ntt_Storage storage;
	zero_storage(&storage);

	ntt_Storage_CreateInfo createInfo = {0};
	createInfo.newElement			  = storage_new_callback;

	ntt_Result result = ntt_Storage_Initialize(&storage, NULL, sizeof(i32), 4, &createInfo);
	TEST_ASSERT(result == NTT_RESULT_SUCCESS);

	IDResult newResult = ntt_Storage_New(&storage);
	TEST_ASSERT(newResult.result == NTT_RESULT_SUCCESS);
	TEST_ASSERT(g_storageNewCount == 1);

	ntt_Result destroyResult = ntt_Storage_Destroy(&storage);
	TEST_ASSERT(destroyResult == NTT_RESULT_SUCCESS);
}

TEST_CASE(FreeElementCallback)
{
	g_storageFreeCount = 0;

	ntt_Storage storage;
	zero_storage(&storage);

	ntt_Storage_CreateInfo createInfo = {0};
	createInfo.freeElement			  = storage_free_callback;

	ntt_Result result = ntt_Storage_Initialize(&storage, NULL, sizeof(i32), 4, &createInfo);
	TEST_ASSERT(result == NTT_RESULT_SUCCESS);

	IDResult newResult = ntt_Storage_New(&storage);
	TEST_ASSERT(newResult.result == NTT_RESULT_SUCCESS);

	ntt_Result freeResult = ntt_Storage_Free(&storage, newResult.data);
	TEST_ASSERT(freeResult == NTT_RESULT_SUCCESS);
	TEST_ASSERT(g_storageFreeCount == 1);

	ntt_Result destroyResult = ntt_Storage_Destroy(&storage);
	TEST_ASSERT(destroyResult == NTT_RESULT_SUCCESS);
}

TEST_CASE(CreateAndDestroyCallbacks)
{
	g_storageCreateCount  = 0;
	g_storageDestroyCount = 0;

	ntt_Storage storage;
	zero_storage(&storage);

	ntt_Storage_CreateInfo createInfo = {0};
	createInfo.createElement		  = storage_create_callback;
	createInfo.destroyElement		  = storage_destroy_callback;

	ntt_Result result = ntt_Storage_Initialize(&storage, NULL, sizeof(i32), 4, &createInfo);
	TEST_ASSERT(result == NTT_RESULT_SUCCESS);
	TEST_ASSERT(g_storageCreateCount == 0); // createElement is reserved, not called during Initialize

	ntt_Result destroyResult = ntt_Storage_Destroy(&storage);
	TEST_ASSERT(destroyResult == NTT_RESULT_SUCCESS);
	TEST_ASSERT(g_storageDestroyCount == 0); // destroyElement is reserved, not called during Destroy
}

TEST_CASE(StorageDestroyMissingAllocator)
{
	ntt_Result result = ntt_MemoryGlobals_Initialize();
	TEST_ASSERT(result == NTT_RESULT_SUCCESS);

	ntt_Storage storage;
	zero_storage(&storage);

	ntt_Storage_CreateInfo createInfo = {0};
	result							  = ntt_Storage_Initialize(&storage, NULL, sizeof(i32), 2, &createInfo);
	TEST_ASSERT(result == NTT_RESULT_SUCCESS);

	// Corrupt the allocator to simulate missing allocator
	storage.pAllocator = NULL;

	result = ntt_Storage_Destroy(&storage);
	TEST_ASSERT(result == NTT_RESULT_INITIALIZE_WITHOUT_ALLOCATOR);

	// Memory leak is expected since destroy failed
	result = ntt_MemoryGlobals_Destroy();
	TEST_ASSERT(result == NTT_RESULT_MEMORY_LEAK);
}

TEST_SUITE_DEFINE(storage,
				  storage_tests_before_each,
				  storage_tests_after_each,
				  TEST_CASE_DECLARE(InitializeStorage),
				  TEST_CASE_DECLARE(InitializeStorageWithAllocator),
				  TEST_CASE_DECLARE(InitializeNullPointer),
				  TEST_CASE_DECLARE(AlreadyInitialized),
				  TEST_CASE_DECLARE(NewAndFree),
				  TEST_CASE_DECLARE(ReuseFreedID),
				  TEST_CASE_DECLARE(StorageFull),
				  TEST_CASE_DECLARE(FreeAndRefillStorage),
				  TEST_CASE_DECLARE(NewNullPointer),
				  TEST_CASE_DECLARE(FreeNullPointer),
				  TEST_CASE_DECLARE(DestroyNullPointer),
				  TEST_CASE_DECLARE(NewElementCallback),
				  TEST_CASE_DECLARE(FreeElementCallback),
				  TEST_CASE_DECLARE(CreateAndDestroyCallbacks),
				  TEST_CASE_DECLARE_WITHOUT_WRAPPER(StorageDestroyMissingAllocator))

#endif /* _STORAGE_TESTS_H_ */
