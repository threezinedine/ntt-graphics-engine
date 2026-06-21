#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#include "engine/core/core.h"
#include "engine/resources/resources.h"

struct ntt_Application
{
	ntt_WindowResource defaultWindowResource;

	ntt_Result (*pfn_Initialize)(struct ntt_Application* pApplication);
	ntt_Result (*pfn_Run)(struct ntt_Application* pApplication);
	ntt_Result (*pfn_Shutdown)(struct ntt_Application* pApplication);
};

typedef struct ntt_Application ntt_Application;

ntt_Result ntt_Application_Initialize(ntt_Application* pApplication);

ntt_Result ntt_Application_Run(ntt_Application* pApplication);

ntt_Result ntt_Application_Shutdown(ntt_Application* pApplication);

#endif /** _APPLICATION_H_ */