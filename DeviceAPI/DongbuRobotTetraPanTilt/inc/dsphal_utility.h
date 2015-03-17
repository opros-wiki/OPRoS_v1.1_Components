/*
*******************************************************************************
* File name     : dsphal_utility.h
* Programmer(s) : Park Jin-Seok
* Description   : This is header which declares function prototypes about
*                 DSPHAL utility.
*******************************************************************************
*/

#ifndef DSPHAL_UTILITY_H
#define DSPHAL_UTILITY_H

#ifdef _WIN32
	#include <winsock2.h>
#else
	#include <unistd.h>
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

#ifdef __cplusplus
	extern "C" {
#endif

DSPHALEXPORT int dsphal_utility_readn(register int fd, register char *ptr, register int nbytes);
DSPHALEXPORT int dsphal_utility_writen(register int fd, register char *ptr, register int nbytes);
DSPHALEXPORT int dsphal_utility_readline(register int fd, register char *ptr, register int maxlen);

#ifdef __cplusplus
	}
#endif

#endif
