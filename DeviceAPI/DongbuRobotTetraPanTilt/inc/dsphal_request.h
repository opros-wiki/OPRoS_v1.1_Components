/*
*******************************************************************************
* File name     : dsphal_request.h
* Programmer(s) : Park Jin-Seok
* Description   : This is a header which declares function prototypes about the
*                 DSPHAL request.
*******************************************************************************
*/

#ifndef DSPHAL_REQUEST_H
#define DSPHAL_REQUEST_H

#include <stdlib.h>

#include "dsphal_datalist.h"
#include "dsphal_tcp.h"
#include "dsphal_tcp_client.h"
#include "dsphal_xml.h"

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

DSPHALEXPORT dsphal_datalist_t *dsphal_request_method_call(dsphal_tcp_client_t *dsphal_tcp_client, char *method_name, dsphal_datalist_t *dsphal_datalist_arg);

#ifdef __cplusplus
	}
#endif

#endif
