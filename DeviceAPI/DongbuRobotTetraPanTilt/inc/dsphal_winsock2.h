/*
*******************************************************************************
* File name     : dsphal_winsock2.h
* Programmer(s) : Park Jin-Seok
* Description   : This is header which declares function prototypes about
*                 Microsoft Windows dependent socket library. This is dependent
*                 on Microsoft winsock2 library.
*******************************************************************************
*/

#ifndef DSPHAL_WINSOCK2_H
#define DSPHAL_WINSOCK2_H

#ifdef _WIN32
	#include <winsock2.h>
#endif

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

#ifdef _WIN32

#ifdef __cplusplus
	extern "C" {
#endif

DSPHALEXPORT int dsphal_winsock2_wsastartup(void);
DSPHALEXPORT int dsphal_winsock2_wsacleanup(void);

#ifdef __cplusplus
	}
#endif

#endif

#endif
