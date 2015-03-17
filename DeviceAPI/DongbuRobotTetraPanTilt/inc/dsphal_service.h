/*
*******************************************************************************
* File name     : dsphal_service.h
* Programmer(s) : Park Jin-Seok
* Description   : This is a header which declares types and function prototypes
*                 about the DSPHAL service.
*******************************************************************************
*/

#ifndef DSPHAL_SERVICE_H
#define DSPHAL_SERVICE_H

#include <stdlib.h>
#include <string.h>

#include "dsphal_datalist.h"
#include "dsphal_registry.h"
#include "dsphal_tcp.h"
#include "dsphal_tcp_server.h"
#include "dsphal_xml.h"

/*
*******************************************************************************
*                              TYPE DEFINITIONS
*******************************************************************************
*/

typedef struct dsphal_service_s {
	dsphal_registry_t *registry;
} dsphal_service_t;

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

DSPHALEXPORT dsphal_service_t *dsphal_service_new(void);
DSPHALEXPORT void dsphal_service_free(dsphal_service_t *dsphal_service);

DSPHALEXPORT int dsphal_service_set_registry(dsphal_service_t *dsphal_service, dsphal_registry_t *new_registry);
DSPHALEXPORT dsphal_registry_t *dsphal_service_get_registry(dsphal_service_t *dsphal_service);

DSPHALEXPORT dsphal_service_t *dsphal_service_create(dsphal_registry_t *registry);

DSPHALEXPORT int dsphal_service(dsphal_tcp_server_t *dsphal_tcp_server, dsphal_service_t *dsphal_service);

#ifdef __cplusplus
	}
#endif

#endif
