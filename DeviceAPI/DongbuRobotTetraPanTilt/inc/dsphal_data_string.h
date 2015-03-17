/*
*******************************************************************************
* File name     : dsphal_data_string.h
* Programmer(s) : Park Jin-Seok
* Description   : This is header which declares types and function prototypes
*                 about DSPHAL string data.
*******************************************************************************
*/

#ifndef DSPHAL_DATA_STRING_H
#define DSPHAL_DATA_STRING_H

#include "dsphal_data_memblk_int8.h"

/*
*******************************************************************************
*                              TYPE DEFINITIONS
*******************************************************************************
*/

typedef dsphal_data_memblk_int8_t dsphal_data_string_t;

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

DSPHALEXPORT dsphal_data_string_t *dsphal_data_string_new(void);
DSPHALEXPORT void dsphal_data_string_free(dsphal_data_string_t *dsphal_data_string);

DSPHALEXPORT dsphal_data_string_t *dsphal_data_string_create(char *string);
DSPHALEXPORT void dsphal_data_string_destroy(dsphal_data_string_t *dsphal_data_string);

DSPHALEXPORT char *dsphal_data_string_get_string(dsphal_data_string_t *dsphal_data_string);
DSPHALEXPORT size_t dsphal_data_string_strlen(dsphal_data_string_t *dsphal_data_string);

#ifdef __cplusplus
	}
#endif

#endif
