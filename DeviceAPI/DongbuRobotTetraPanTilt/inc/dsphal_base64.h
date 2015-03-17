/*
*******************************************************************************
* File name     : dsphal_base64.h
* Programmer(s) : Park Jin-Seok
* Description   : This is header which declares function prototypes about
*                 DSPHAL Base64 encoder, decoder.
*******************************************************************************
*/

#ifndef DSPHAL_BASE64_H
#define DSPHAL_BASE64_H

#include <stdlib.h>
#include <string.h>

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

DSPHALEXPORT char *dsphal_base64_encode(const char *src, int len, int *ret_len);
DSPHALEXPORT char *dsphal_base64_decode(const char *src, int len, int *ret_len);

#ifdef __cplusplus
	}
#endif

#endif
