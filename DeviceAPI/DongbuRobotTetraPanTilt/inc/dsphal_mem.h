/*
*******************************************************************************
* File name     : dsphal_mem.h
* Programmer(s) : Park Jin-Seok
* Description   : This is a header which declares function prototypes about
*                 the DSPHAL memory management.
*******************************************************************************
*/

#ifndef DSPHAL_MEM_H
#define DSPHAL_MEM_H

#include <stdlib.h>

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

DSPHALEXPORT void dsphal_mem_free(void *ptr);

#ifdef __cplusplus
	}
#endif

#endif
