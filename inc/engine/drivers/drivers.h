#ifndef _DRIVERS_H_
#define _DRIVERS_H_

#include "engine/core/result.h"
#include "engine/core/types.h"
#include "glfw_driver/glfw_driver.h"

extern ntt_DisplayDriver* g_DisplayDriver;

ntt_Result ntt_Drivers_Register();
ntt_Result ntt_Drivers_Unregister();

ntt_Result ntt_Drivers_Initialize();
ntt_Result ntt_Drivers_Destroy();

#endif /* _DRIVERS_H_ */