/*
*******************************************************************************
* File name     : dsphal.h
* Programmer(s) : Park Jin-Seok
* Description   : This is DSPHAL main header.
*******************************************************************************
*/

#ifndef DSPHAL_H
#define DSPHAL_H

#include "dsphal_base64.h"
#include "dsphal_builddecomp.h"
#include "dsphal_data_int.h"
#include "dsphal_data_memblk_int8.h"
#include "dsphal_data_string.h"
#include "dsphal_datalist.h"
#include "dsphal_header.h"
#include "dsphal_introspection.h"
#include "dsphal_mem.h"
#include "dsphal_method.h"
#include "dsphal_registry.h"
#include "dsphal_request.h"
#include "dsphal_service.h"
#include "dsphal_tcp.h"
#include "dsphal_tcp_client.h"
#include "dsphal_tcp_server.h"
#include "dsphal_utility.h"
#include "dsphal_winsock2.h"
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

DSPHALEXPORT int dsphal_initialize(void);
DSPHALEXPORT int dsphal_finalize(void);

#ifdef __cplusplus
	}
#endif

#endif
