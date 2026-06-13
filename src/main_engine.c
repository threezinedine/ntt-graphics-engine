#include "engine/engine.h"
#include <stdio.h>

int	   g_argc;
char** g_argv;

int main(i32 argc, char** argv)
{
	g_argc = argc;
	g_argv = argv;

	ntt_InitializeIDSystem();
	ntt_MemoryGlobalsInitialize();

	ntt_ObjectRegisterType();
	ntt_SystemRegisterType();

	ntt_Object object;
	ntt_ObjectInitialize(&object, g_memoryGlobals.mallocAllocator);
	ntt_ConsolePrint("Object ID: type = %d\n", object.type.index);
	ntt_ConsolePrint("Object is instance of Object: %s\n", ntt_ObjectIsInstanceOf(&object) ? "TRUE" : "FALSE");
	ntt_ConsolePrint("Object is derived from Object: %s\n", ntt_ObjectIsDerivedFrom(&object) ? "TRUE" : "FALSE");
	ntt_ConsolePrint("Object is instance of System: %s\n",
					 ntt_SystemIsInstanceOf((ntt_System*)&object) ? "TRUE" : "FALSE");
	ntt_ConsolePrint("Object is derived from System: %s\n",
					 ntt_SystemIsDerivedFrom((ntt_System*)&object) ? "TRUE" : "FALSE");

	ntt_System system;
	ntt_SystemInitialize(&system, g_memoryGlobals.mallocAllocator);
	ntt_ConsolePrint("System ID: type = %d\n", ((ntt_Object*)&system)->type.index);
	ntt_ConsolePrint("System is instance of System: %s\n", ntt_SystemIsInstanceOf(&system) ? "TRUE" : "FALSE");
	ntt_ConsolePrint("System is derived from Object: %s\n", ntt_SystemIsDerivedFrom(&system) ? "TRUE" : "FALSE");
	ntt_ConsolePrint("System is instance of Object: %s\n",
					 ntt_ObjectIsInstanceOf((ntt_Object*)&system) ? "TRUE" : "FALSE");
	ntt_ConsolePrint("System is derived from System: %s\n",
					 ntt_SystemIsDerivedFrom((ntt_System*)&system) ? "TRUE" : "FALSE");

	ntt_ObjectDestroy(&object);
	ntt_SystemDestroy(&system);

	ntt_SystemUnregisterType();
	ntt_ObjectUnregisterType();

	ntt_MemoryGlobalsDestroy();
	ntt_DestroyIDSystem();

	return 0;
}