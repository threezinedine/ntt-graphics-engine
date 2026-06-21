#include "engine/drivers/glfw_driver/glfw_driver.h"
#include "engine/core/logging/logging.h"
#include "engine/core/memory/memory.h"
#include "engine/core/types.h"
#include <stddef.h>

#include <GLFW/glfw3.h>

ntt_DisplayDriver* g_GLFW_DisplayDriver = NULL;
GLFWwindow*		   g_GLFW_HiddenWindow	= NULL;

#if NTT_GRAPHICS_DRIVER_GLFW

static ntt_Result ntt_GLFW_CreateWindow(const char* title, i32 width, i32 height, void** ppWindow);
static ntt_Result ntt_GLFW_DestroyWindow(void* pWindow);

ntt_Result ntt_GLFW_Register()
{
	voidPtrResult result = ntt_Allocate(g_memoryGlobals.mallocAllocator, sizeof(ntt_DisplayDriver));
	NTT_ASSERT_IF(result.result != NTT_RESULT_SUCCESS)
	{
		return result.result;
	}

	if (glfwInit() != GLFW_TRUE)
	{
		NTT_DRIVER_ERROR("Failed to initialize GLFW.");
		return NTT_RESULT_GLFW_INITIALIZATION_FAILURE;
	}

	g_GLFW_DisplayDriver				= (ntt_DisplayDriver*)result.pData;
	g_GLFW_DisplayDriver->CreateWindow	= ntt_GLFW_CreateWindow;
	g_GLFW_DisplayDriver->DestroyWindow = ntt_GLFW_DestroyWindow;

#if 0
	// Always needa create a window to initialize the OpenGL context, so we create a hidden window here.
	NTT_SUCCESS_ASSERT(ntt_GLFW_CreateWindow("Hidden Window", 600, 800, (void**)&g_GLFW_HiddenWindow));
	// glfwHideWindow(g_GLFW_HiddenWindow);

	// TODO: remove later
	while (!glfwWindowShouldClose(g_GLFW_HiddenWindow))
	{
		glfwPollEvents();
	}
#endif /* 0 */

	NTT_DRIVER_INFO("GLFW Driver registered successfully.");
	return NTT_RESULT_SUCCESS;
}

ntt_Result ntt_GLFW_Unregister()
{
	NTT_ASSERT_IF(g_GLFW_DisplayDriver == NULL)
	{
		return NTT_RESULT_NULL_POINTER;
	}

#if 0
	// Terminate GLFW and clean up resources
	ntt_GLFW_DestroyWindow(g_GLFW_HiddenWindow);
#endif /* 0 */

	glfwTerminate();

	NTT_SUCCESS_ASSERT(
		ntt_Deallocate(g_memoryGlobals.mallocAllocator, g_GLFW_DisplayDriver, sizeof(ntt_DisplayDriver)));
	g_GLFW_DisplayDriver = NULL;

	NTT_DRIVER_INFO("GLFW Driver unregistered successfully.");
	return NTT_RESULT_SUCCESS;
}

static ntt_Result ntt_GLFW_CreateWindow(const char* title, i32 width, i32 height, void** ppWindow)
{
	GLFWwindow* pWindow = glfwCreateWindow(width, height, title, NULL, NULL);
	NTT_ASSERT_IF(pWindow == NULL)
	{
		NTT_DRIVER_ERROR("Failed to create GLFW window.");
		return NTT_RESULT_GLFW_WINDOW_CREATION_FAILURE;
	}
	glfwMakeContextCurrent(pWindow);
	*ppWindow = pWindow;
	return NTT_RESULT_SUCCESS;
}

static ntt_Result ntt_GLFW_DestroyWindow(void* pWindow)
{
	NTT_ASSERT_IF(pWindow == NULL)
	{
		return NTT_RESULT_NULL_POINTER;
	}
	glfwDestroyWindow((GLFWwindow*)pWindow);
	return NTT_RESULT_SUCCESS;
}

#endif /* NTT_GRAPHICS_DRIVER_GLFW */
