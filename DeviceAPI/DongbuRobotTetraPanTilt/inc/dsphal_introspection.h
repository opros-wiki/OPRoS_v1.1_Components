/*
*******************************************************************************
* File name     : dsphal_introspection.h
* Programmer(s) : Park Jin-Seok
* Description   : This is a header which declares function prototypes about
*                 the DSPHAL introspection.
*******************************************************************************
*/

#ifndef DSPHAL_INTROSPECTION_H
#define DSPHAL_INTROSPECTION_H

#include <string.h>

#include "dsphal_builddecomp.h"
#include "dsphal_datalist.h"
#include "dsphal_registry.h"

/*
*******************************************************************************
*                             FUNCTION PROTOTYPES
*******************************************************************************
*/

#ifdef _WIN32
	#ifdef BUILD_DLL
		#define DSPHALEXPORT __declspec(dllexport)
	#else
		#define DSPHALEXPORT __declspec(dllimport)
	#endif
#else
	#define DSPHALEXPORT
#endif

#ifdef __cplusplus
	extern "C" {
#endif

DSPHALEXPORT int dsphal_introspection_register(dsphal_registry_t *dsphal_registry);

#ifdef __cplusplus
	}
#endif

#endif
