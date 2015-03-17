/*
*******************************************************************************
* File name     : dsphal_data_int.h
* Programmer(s) : Park Jin-Seok
* Description   : This is header which declares types and function prototypes
*                 about DSPHAL integer data.
*******************************************************************************
*/

#ifndef DSPHAL_DATA_INT_H
#define DSPHAL_DATA_INT_H

#include <stdlib.h>

/*
*******************************************************************************
*                              TYPE DEFINITIONS
*******************************************************************************
*/

typedef int dsphal_data_int_t;

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

DSPHALEXPORT dsphal_data_int_t *dsphal_data_int_new(void);
DSPHALEXPORT void dsphal_data_int_free(dsphal_data_int_t *dsphal_data_int);

DSPHALEXPORT int dsphal_data_int_set(dsphal_data_int_t *dsphal_data_int, dsphal_data_int_t new_data_int);
DSPHALEXPORT dsphal_data_int_t dsphal_data_int_get(dsphal_data_int_t *dsphal_data_int);

DSPHALEXPORT dsphal_data_int_t *dsphal_data_int_create(int data_int);
DSPHALEXPORT void dsphal_data_int_destroy(dsphal_data_int_t *dsphal_data_int);

#ifdef __cplusplus
	}
#endif

#endif
