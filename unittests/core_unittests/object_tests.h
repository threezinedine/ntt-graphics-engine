#ifndef _OBJECT_TESTS_H_
#define _OBJECT_TESTS_H_

#include "engine/engine.h"
#include "tools/tools.h"

void object_tests_before_each()
{
	TEST_ASSERT(ntt_InitializeIDSystem() == NTT_RESULT_SUCCESS);
	TEST_ASSERT(ntt_MemoryGlobals_Initialize() == NTT_RESULT_SUCCESS);
	TEST_ASSERT(ntt_Object_RegisterType() == NTT_RESULT_SUCCESS);
	TEST_ASSERT(ntt_Systems_Register() == NTT_RESULT_SUCCESS);
}

void object_tests_after_each()
{
	TEST_ASSERT(ntt_Systems_Unregister() == NTT_RESULT_SUCCESS);
	TEST_ASSERT(ntt_Object_UnregisterType() == NTT_RESULT_SUCCESS);
	TEST_ASSERT(ntt_MemoryGlobals_Destroy() == NTT_RESULT_SUCCESS);
	TEST_ASSERT(ntt_DestroyIDSystem() == NTT_RESULT_SUCCESS);
}

// --- ntt_Object instance ---

TEST_CASE(ObjectIsInstanceOfObject)
{
	ntt_Object object;
	ntt_Object_Initialize(&object, g_memoryGlobals.mallocAllocator);

	TEST_ASSERT(ntt_Object_HasInstance(&object) == TRUE);

	ntt_Object_Destroy(&object);
}

TEST_CASE(ObjectIsDerivedFromObject)
{
	ntt_Object object;
	ntt_Object_Initialize(&object, g_memoryGlobals.mallocAllocator);

	TEST_ASSERT(ntt_Object_IsParentOf(&object) == TRUE);

	ntt_Object_Destroy(&object);
}

TEST_CASE(ObjectIsNotInstanceOfSystem)
{
	ntt_Object object;
	ntt_Object_Initialize(&object, g_memoryGlobals.mallocAllocator);

	TEST_ASSERT(ntt_System_HasInstance((ntt_System*)&object) == FALSE);

	ntt_Object_Destroy(&object);
}

TEST_CASE(ObjectIsNotDerivedFromSystem)
{
	ntt_Object object;
	ntt_Object_Initialize(&object, g_memoryGlobals.mallocAllocator);

	TEST_ASSERT(ntt_System_IsParentOf((ntt_System*)&object) == FALSE);

	ntt_Object_Destroy(&object);
}

// --- ntt_System instance ---

TEST_CASE(SystemIsInstanceOfSystem)
{
	ntt_System system;
	ntt_System_Initialize(&system, g_memoryGlobals.mallocAllocator);

	TEST_ASSERT(ntt_System_HasInstance(&system) == TRUE);

	ntt_System_Destroy(&system);
}

TEST_CASE(SystemIsDerivedFromSystem)
{
	ntt_System system;
	ntt_System_Initialize(&system, g_memoryGlobals.mallocAllocator);

	TEST_ASSERT(ntt_System_IsParentOf(&system) == TRUE);

	ntt_System_Destroy(&system);
}

TEST_CASE(SystemIsDerivedFromObject)
{
	ntt_System system;
	ntt_System_Initialize(&system, g_memoryGlobals.mallocAllocator);

	TEST_ASSERT(ntt_Object_IsParentOf((ntt_Object*)&system) == TRUE);

	ntt_System_Destroy(&system);
}

TEST_CASE(SystemIsNotInstanceOfObject)
{
	ntt_System system;
	ntt_System_Initialize(&system, g_memoryGlobals.mallocAllocator);

	TEST_ASSERT(ntt_Object_HasInstance((ntt_Object*)&system) == FALSE);

	ntt_System_Destroy(&system);
}

// --- ntt_DisplaySystem instance ---

TEST_CASE(DisplaySystemIsInstanceOfDisplaySystem)
{
	ntt_DisplaySystem displaySystem;
	ntt_DisplaySystem_Initialize(&displaySystem, g_memoryGlobals.mallocAllocator);

	TEST_ASSERT(ntt_DisplaySystem_HasInstance(&displaySystem) == TRUE);

	ntt_DisplaySystem_Destroy(&displaySystem);
}

TEST_CASE(DisplaySystemIsDerivedFromDisplaySystem)
{
	ntt_DisplaySystem displaySystem;
	ntt_DisplaySystem_Initialize(&displaySystem, g_memoryGlobals.mallocAllocator);

	TEST_ASSERT(ntt_DisplaySystem_IsParentOf(&displaySystem) == TRUE);

	ntt_DisplaySystem_Destroy(&displaySystem);
}

TEST_CASE(DisplaySystemIsDerivedFromSystem)
{
	ntt_DisplaySystem displaySystem;
	ntt_DisplaySystem_Initialize(&displaySystem, g_memoryGlobals.mallocAllocator);

	TEST_ASSERT(ntt_System_IsParentOf((ntt_System*)&displaySystem) == TRUE);

	ntt_DisplaySystem_Destroy(&displaySystem);
}

TEST_CASE(DisplaySystemIsDerivedFromObject)
{
	ntt_DisplaySystem displaySystem;
	ntt_DisplaySystem_Initialize(&displaySystem, g_memoryGlobals.mallocAllocator);

	TEST_ASSERT(ntt_Object_IsParentOf((ntt_Object*)&displaySystem) == TRUE);

	ntt_DisplaySystem_Destroy(&displaySystem);
}

TEST_CASE(DisplaySystemIsNotInstanceOfSystem)
{
	ntt_DisplaySystem displaySystem;
	ntt_DisplaySystem_Initialize(&displaySystem, g_memoryGlobals.mallocAllocator);

	TEST_ASSERT(ntt_System_HasInstance((ntt_System*)&displaySystem) == FALSE);

	ntt_DisplaySystem_Destroy(&displaySystem);
}

TEST_CASE(DisplaySystemIsNotInstanceOfObject)
{
	ntt_DisplaySystem displaySystem;
	ntt_DisplaySystem_Initialize(&displaySystem, g_memoryGlobals.mallocAllocator);

	TEST_ASSERT(ntt_Object_HasInstance((ntt_Object*)&displaySystem) == FALSE);

	ntt_DisplaySystem_Destroy(&displaySystem);
}

TEST_SUITE_DEFINE(object,
				  object_tests_before_each,
				  object_tests_after_each,
				  TEST_CASE_DECLARE(ObjectIsInstanceOfObject),
				  TEST_CASE_DECLARE(ObjectIsDerivedFromObject),
				  TEST_CASE_DECLARE(ObjectIsNotInstanceOfSystem),
				  TEST_CASE_DECLARE(ObjectIsNotDerivedFromSystem),
				  TEST_CASE_DECLARE(SystemIsInstanceOfSystem),
				  TEST_CASE_DECLARE(SystemIsDerivedFromSystem),
				  TEST_CASE_DECLARE(SystemIsDerivedFromObject),
				  TEST_CASE_DECLARE(SystemIsNotInstanceOfObject),
				  TEST_CASE_DECLARE(DisplaySystemIsInstanceOfDisplaySystem),
				  TEST_CASE_DECLARE(DisplaySystemIsDerivedFromDisplaySystem),
				  TEST_CASE_DECLARE(DisplaySystemIsDerivedFromSystem),
				  TEST_CASE_DECLARE(DisplaySystemIsDerivedFromObject),
				  TEST_CASE_DECLARE(DisplaySystemIsNotInstanceOfSystem),
				  TEST_CASE_DECLARE(DisplaySystemIsNotInstanceOfObject))

#endif /* _OBJECT_TESTS_H_ */
