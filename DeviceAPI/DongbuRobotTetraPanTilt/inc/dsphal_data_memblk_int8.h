/*
*******************************************************************************
* File name     : dsphal_data_memblk_int8.h
* Programmer(s) : Park Jin-Seok
* Description   : This is header which declares types and function prototypes
*                 about DSPHAL 8-bit type memory block data.
*******************************************************************************
*/

#ifndef DSPHAL_DATA_MEMBLK_INT8_H
#define DSPHAL_DATA_MEMBLK_INT8_H

#include <stdlib.h>
#include <string.h>
#ifdef _MSC_VER
	#include "pstdint.h"
#else
	#include <stdint.h>
#endif

/*
*******************************************************************************
*                              TYPE DEFINITIONS
*******************************************************************************
*/

typedef struct dsphal_data_memblk_int8_s {
	size_t size;
	int8_t *memblk;
} dsphal_data_memblk_int8_t;

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

DSPHALEXPORT dsphal_data_memblk_int8_t *dsphal_data_memblk_int8_new(void);
DSPHALEXPORT void dsphal_data_memblk_int8_free(dsphal_data_memblk_int8_t *dsphal_data_memblk_int8);

DSPHALEXPORT int dsphal_data_memblk_int8_set_size(dsphal_data_memblk_int8_t *dsphal_data_memblk_int8, size_t new_size);
DSPHALEXPORT int dsphal_data_memblk_int8_set_memblk(dsphal_data_memblk_int8_t *dsphal_data_memblk_int8, int8_t *new_memblk);

DSPHALEXPORT size_t dsphal_data_memblk_int8_get_size(dsphal_data_memblk_int8_t *dsphal_data_memblk_int8);
DSPHALEXPORT int8_t *dsphal_data_memblk_int8_get_memblk(dsphal_data_memblk_int8_t *dsphal_data_memblk_int8);

DSPHALEXPORT dsphal_data_memblk_int8_t *dsphal_data_memblk_int8_create(size_t size);
DSPHALEXPORT void dsphal_data_memblk_int8_destroy(dsphal_data_memblk_int8_t *dsphal_data_memblk_int8);
DSPHALEXPORT int dsphal_data_memblk_int8_memcpy(dsphal_data_memblk_int8_t *dsphal_data_memblk_int8, void *from, size_t size);
DSPHALEXPORT int dsphal_data_memblk_int8_memset(dsphal_data_memblk_int8_t *dsphal_data_memblk_int8, int c, size_t size);

#ifdef __cplusplus
	}
#endif

#endif
