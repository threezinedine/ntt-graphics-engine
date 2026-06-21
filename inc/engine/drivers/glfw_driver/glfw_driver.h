#ifndef _GLFW_DRIVER_H_
#define _GLFW_DRIVER_H_

struct ntt_DisplayDriver;
typedef struct ntt_DisplayDriver ntt_DisplayDriver;

extern struct ntt_DisplayDriver* g_GLFW_DisplayDriver;

#if NTT_GRAPHICS_DRIVER_GLFW

#include "engine/core/result.h"
#include "engine/core/types.h"
#include "engine/systems/display/display_driver.h"

ntt_Result ntt_GLFW_Register();
ntt_Result ntt_GLFW_Unregister();

#endif /* NTT_GRAPHICS_DRIVER_GLFW */

#endif /* _GLFW_DRIVER_H_ */