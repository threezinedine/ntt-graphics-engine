#ifndef _DISPLAY_DRIVER_H_
#define _DISPLAY_DRIVER_H_

#include "engine/core/result.h"
#include "engine/core/types.h"

/**
 * Placeholder methods for all display driver like glfw, sdl, win32 api, x11, etc.
 */
struct ntt_DisplayDriver
{
	ntt_Result (*Initialize)(void);
	ntt_Result (*CreateWindow)(const char* title, i32 width, i32 height, void** ppWindow);
	ntt_Result (*DestroyWindow)(void* pWindow);
	ntt_Result (*Destroy)(void);
};

#endif /* _DISPLAY_DRIVER_H_ */