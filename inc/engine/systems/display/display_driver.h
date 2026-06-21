#ifndef _DISPLAY_DRIVER_H_
#define _DISPLAY_DRIVER_H_

#include "display_driver_type.h"
#include "engine/core/id.h"
#include "engine/core/result.h"
#include "engine/core/types.h"

/**
 * Placeholder methods for all display driver like glfw, sdl, win32 api, x11, etc.
 */
struct ntt_DisplayDriver
{
	IDResult (*CreateWindow)(const char* title, i32 width, i32 height);
	ntt_Result (*DestroyWindow)(ID windowID);
};

typedef struct ntt_DisplayDriver ntt_DisplayDriver;
DEFINE_RETURN_RESULT_TYPE(ntt_DisplayDriver)

ntt_DisplayDriverPtrResult ntt_DisplayDriver_Register(ntt_DisplayDriverType driverType);

#endif /* _DISPLAY_DRIVER_H_ */