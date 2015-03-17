/*
*******************************************************************************
* File name     : dsphal_builddecomp.h
* Programmer(s) : Park Jin-Seok
* Description   : This is header which declares function prototypes about
*                 DSPHAL data structure building and decomposition.
*******************************************************************************
*/

#ifndef DSPHAL_BUILDDECOMP_H
#define DSPHAL_BUILDDECOMP_H

#include <stddef.h>
#include <stdarg.h>
#include <string.h>

#include "dsphal_data_int.h"
#include "dsphal_data_string.h"
#include "dsphal_datalist.h"

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

DSPHALEXPORT dsphal_datalist_t *dsphal_build_root_datalist(char *format, ...);
DSPHALEXPORT dsphal_datalist_t *dsphal_build_datalist(char **formatp, va_list *app);
DSPHALEXPORT int dsphal_build_data(char **formatp, va_list *app, dsphal_datalist_t *dsphal_datalist);
DSPHALEXPORT int dsphal_build_data_int(char **formatp, va_list *app, dsphal_data_t *dsphal_data);
DSPHALEXPORT int dsphal_build_data_string(char **formatp, va_list *app, dsphal_data_t *dsphal_data);
DSPHALEXPORT int dsphal_build_data_datalist(char **formatp, va_list *app, dsphal_data_t *dsphal_data);

DSPHALEXPORT int dsphal_decompose_root_datalist(dsphal_datalist_t *dsphal_datalist, char *format, ...);
DSPHALEXPORT int dsphal_decompose_datalist(dsphal_datalist_t *dsphal_datalist, char **formatp, va_list *app);
DSPHALEXPORT int dsphal_decompose_data(dsphal_data_t *dsphal_data, char **formatp, va_list *app);
DSPHALEXPORT int dsphal_decompose_data_int(dsphal_data_t *dsphal_data, char **formatp, va_list *app);
DSPHALEXPORT int dsphal_decompose_data_string(dsphal_data_t *dsphal_data, char **formatp, va_list *app);
DSPHALEXPORT int dsphal_decompose_data_datalist(dsphal_data_t *dsphal_data, char **formatp, va_list *app);

#ifdef __cplusplus
	}
#endif

#endif
