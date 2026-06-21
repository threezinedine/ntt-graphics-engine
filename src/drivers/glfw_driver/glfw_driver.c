#include "engine/drivers/glfw_driver/glfw_driver.h"
#include "engine/core/containers/map.h"
#include "engine/core/id.h"
#include "engine/core/logging/logging.h"
#include "engine/core/memory/memory.h"
#include "engine/core/types.h"
#include <stddef.h>
#include <string.h>

#include <GLFW/glfw3.h>

ntt_DisplayDriver* g_GLFW_DisplayDriver = NULL;

#if NTT_GRAPHICS_DRIVER_GLFW

struct ntt_GLFW_WindowData
{
	char		title[256];
	i32			width;
	i32			height;
	GLFWwindow* pWindow;
};

typedef struct ntt_GLFW_WindowData ntt_GLFW_WindowData;

static ID	   g_defaultWindowID = INVALID_ID_INIT;
static ntt_Map g_GLFW_WindowMap; /// Map from window ID to GLFWwindow*

static IDResult	   ntt_GLFW_CreateWindow(const char* title, i32 width, i32 height);
static ntt_Result  ntt_GLFW_DestroyWindow(ID windowID);
static GLFWwindow* ntt_GLFW_GetWindowByID(ID windowID);
static u32		   ntt_WindowIDHashFunction(void* pKey, usize keySize);

ntt_Result ntt_GLFW_Register()
{
	voidPtrResult result = ntt_Allocate(g_memoryGlobals.mallocAllocator, sizeof(ntt_DisplayDriver));
	NTT_ASSERT_IF(result.result != NTT_RESULT_SUCCESS)
	{
		return result.result;
	}

	ntt_MapResult mapResult = ntt_MapCreate(ntt_WindowIDHashFunction, 16, g_memoryGlobals.mallocAllocator);
	NTT_ASSERT_IF(mapResult.result != NTT_RESULT_SUCCESS)
	{
		ntt_Deallocate(g_memoryGlobals.mallocAllocator, result.pData, sizeof(ntt_DisplayDriver));
		return mapResult.result;
	}
	g_GLFW_WindowMap = mapResult.data;

	if (glfwInit() != GLFW_TRUE)
	{
		NTT_DRIVER_ERROR("Failed to initialize GLFW.");
		return NTT_RESULT_GLFW_INITIALIZATION_FAILURE;
	}

	g_GLFW_DisplayDriver				= (ntt_DisplayDriver*)result.pData;
	g_GLFW_DisplayDriver->CreateWindow	= ntt_GLFW_CreateWindow;
	g_GLFW_DisplayDriver->DestroyWindow = ntt_GLFW_DestroyWindow;

	IDResult defaultWindowIDResult = ntt_GLFW_CreateWindow("Default Window", 800, 600);
	NTT_SUCCESS_ASSERT_VAR(defaultWindowIDResult);
	g_defaultWindowID = defaultWindowIDResult.data;

	GLFWwindow* pDefaultWindow = ntt_GLFW_GetWindowByID(g_defaultWindowID);

	NTT_ASSERT_IF(pDefaultWindow == NULL)
	{
		return NTT_RESULT_GLFW_WINDOW_NOT_FOUND;
	}

	glfwHideWindow(pDefaultWindow);

	NTT_DRIVER_INFO("GLFW Driver registered successfully.");
	return NTT_RESULT_SUCCESS;
}

ntt_Result ntt_GLFW_Unregister()
{
	NTT_ASSERT_IF(g_GLFW_DisplayDriver == NULL)
	{
		return NTT_RESULT_NULL_POINTER;
	}

	// Terminate GLFW and clean up resources
	NTT_ASSERT_IF(ntt_IsIDEqual(&g_defaultWindowID, &INVALID_ID) == TRUE)
	{
		return NTT_RESULT_GLFW_WINDOW_NOT_FOUND;
	}

	ntt_GLFW_DestroyWindow(g_defaultWindowID);

	NTT_SUCCESS_ASSERT(ntt_MapDestroy(&g_GLFW_WindowMap));

	glfwTerminate();

	NTT_SUCCESS_ASSERT(
		ntt_Deallocate(g_memoryGlobals.mallocAllocator, g_GLFW_DisplayDriver, sizeof(ntt_DisplayDriver)));
	g_GLFW_DisplayDriver = NULL;

	NTT_DRIVER_INFO("GLFW Driver unregistered successfully.");
	return NTT_RESULT_SUCCESS;
}

static IDResult ntt_GLFW_CreateWindow(const char* title, i32 width, i32 height)
{
	IDResult result = {
		.result = NTT_RESULT_SUCCESS,
		.data	= INVALID_ID_INIT,
	};

	GLFWwindow* pWindow = glfwCreateWindow(width, height, title, NULL, NULL);
	NTT_ASSERT_IF(pWindow == NULL)
	{
		NTT_DRIVER_ERROR("Failed to create GLFW window.");
		result.result = NTT_RESULT_GLFW_WINDOW_CREATION_FAILURE;
		return result;
	}

	ntt_GLFW_WindowData windowData = {
		.pWindow = pWindow,
		.width	 = width,
		.height	 = height,
	};

	strncpy(windowData.title, title, sizeof(windowData.title) - 1);

	IDResult newIDResult = ntt_NewID(NTT_OBJECT_TYPE_WINDOW, NULL);
	NTT_ASSERT_IF(newIDResult.result != NTT_RESULT_SUCCESS)
	{
		glfwDestroyWindow(pWindow);
		result.result = newIDResult.result;
		return result;
	}

	NTT_SUCCESS_ASSERT(
		ntt_MapInsert(&g_GLFW_WindowMap, &newIDResult.data, sizeof(ID), &windowData, sizeof(ntt_GLFW_WindowData)));

	result.data = newIDResult.data;

	glfwMakeContextCurrent(pWindow);
	return result;
}

static ntt_Result ntt_GLFW_DestroyWindow(ID windowID)
{
	NTT_ASSERT_IF(ntt_IsIDEqual(&windowID, &INVALID_ID) == TRUE)
	{
		return NTT_RESULT_NULL_POINTER;
	}

	ntt_KeyValuePairResult result = ntt_MapGet(&g_GLFW_WindowMap, &windowID, (usize)sizeof(ID));
	NTT_ASSERT_IF(result.result != NTT_RESULT_SUCCESS)
	{
		return NTT_RESULT_GLFW_WINDOW_NOT_FOUND;
	}

	ntt_GLFW_WindowData* pWindowData = (ntt_GLFW_WindowData*)result.data.pValue;

	glfwDestroyWindow(pWindowData->pWindow);

	NTT_SUCCESS_ASSERT(ntt_MapRemove(&g_GLFW_WindowMap, &windowID, (usize)sizeof(ID)));

	return NTT_RESULT_SUCCESS;
}

static GLFWwindow* ntt_GLFW_GetWindowByID(ID windowID)
{
	NTT_ASSERT_IF(ntt_IsIDEqual(&windowID, &INVALID_ID) == TRUE)
	{
		return NULL;
	}

	ntt_KeyValuePairResult result = ntt_MapGet(&g_GLFW_WindowMap, &windowID, (usize)sizeof(ID));
	NTT_ASSERT_IF(result.result != NTT_RESULT_SUCCESS)
	{
		return NULL;
	}

	ntt_GLFW_WindowData* pWindowData = (ntt_GLFW_WindowData*)result.data.pValue;
	return pWindowData->pWindow;
}

static u32 ntt_WindowIDHashFunction(void* pKey, usize keySize)
{
	NTT_ASSERT_IF(pKey == NULL)
	{
		return 0;
	}

	NTT_ASSERT_IF(keySize != sizeof(ID))
	{
		return 0;
	}

	ID* pID = (ID*)pKey;

	u32 hash = 2166136261u; // FNV-1a hash initial value

	for (usize i = 0; i < (usize)sizeof(ID); i++)
	{
		u8 byte = ((u8*)pID)[i];
		hash ^= byte;
		hash *= 16777619u; // FNV-1a prime
	}

	return hash;
}

#endif /* NTT_GRAPHICS_DRIVER_GLFW */
