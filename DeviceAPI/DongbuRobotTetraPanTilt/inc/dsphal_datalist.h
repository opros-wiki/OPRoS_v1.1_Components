/*
*******************************************************************************
* File name     : dsphal_datalist.h
* Programmer(s) : Park Jin-Seok
* Description   : This is header which declares types and function prototypes
*                 about DSPHAL datalist.
*******************************************************************************
*/

#ifndef DSPHAL_DATALIST_H
#define DSPHAL_DATALIST_H

#include <stdlib.h>
#include <sys/types.h>
#include <string.h>

#include "dsphal_data_int.h"
#include "dsphal_data_memblk_int8.h"
#include "dsphal_data_string.h"

/*
*******************************************************************************
*                              TYPE DEFINITIONS
*******************************************************************************
*/

typedef struct dsphal_data_s {
	char *datatype;
	size_t size;
	void *data;

	struct dsphal_data_s *next;
} dsphal_data_t;

typedef struct dsphal_datalist_s {
	size_t size;

	dsphal_data_t *head;
	dsphal_data_t *tail;
} dsphal_datalist_t;

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

DSPHALEXPORT dsphal_datalist_t *dsphal_datalist_new(void);
DSPHALEXPORT void dsphal_datalist_free(dsphal_datalist_t *dsphal_datalist);

DSPHALEXPORT int dsphal_datalist_ins_next(dsphal_datalist_t *dsphal_datalist, dsphal_data_t *dsphal_data, dsphal_data_t *new_dsphal_data);
DSPHALEXPORT int dsphal_datalist_rem_next(dsphal_datalist_t *dsphal_datalist, dsphal_data_t *dsphal_data, dsphal_data_t **rem_dsphal_data);

DSPHALEXPORT int dsphal_datalist_set_size(dsphal_datalist_t *dsphal_datalist, size_t new_size);
DSPHALEXPORT int dsphal_datalist_set_head(dsphal_datalist_t *dsphal_datalist, dsphal_data_t *dsphal_data_head);
DSPHALEXPORT int dsphal_datalist_set_tail(dsphal_datalist_t *dsphal_datalist, dsphal_data_t *dsphal_data_tail);

DSPHALEXPORT size_t dsphal_datalist_get_size(dsphal_datalist_t *dsphal_datalist);
DSPHALEXPORT dsphal_data_t *dsphal_datalist_get_head(dsphal_datalist_t *dsphal_datalist);
DSPHALEXPORT dsphal_data_t *dsphal_datalist_get_tail(dsphal_datalist_t *dsphal_datalist);

DSPHALEXPORT dsphal_datalist_t *dsphal_datalist_create(void);
DSPHALEXPORT void dsphal_datalist_destroy(dsphal_datalist_t *dsphal_datalist);

DSPHALEXPORT dsphal_data_t *dsphal_data_new(void);
DSPHALEXPORT void dsphal_data_free(dsphal_data_t *dsphal_data);

DSPHALEXPORT int dsphal_data_set_datatype(dsphal_data_t *dsphal_data, char *new_datatype);
DSPHALEXPORT int dsphal_data_set_size(dsphal_data_t *dsphal_data, size_t new_size);
DSPHALEXPORT int dsphal_data_set_data(dsphal_data_t *dsphal_data, void *new_data);
DSPHALEXPORT int dsphal_data_set_next(dsphal_data_t *dsphal_data, dsphal_data_t *dsphal_data_next);

DSPHALEXPORT char *dsphal_data_get_datatype(dsphal_data_t *dsphal_data);
DSPHALEXPORT size_t dsphal_data_get_size(dsphal_data_t *dsphal_data);
DSPHALEXPORT void *dsphal_data_get_data(dsphal_data_t *dsphal_data);
DSPHALEXPORT dsphal_data_t *dsphal_data_get_next(dsphal_data_t *dsphal_data);

DSPHALEXPORT dsphal_data_t *dsphal_data_create(void);
DSPHALEXPORT void dsphal_data_destroy(dsphal_data_t *dsphal_data);

DSPHALEXPORT int dsphal_data_datalist_link(dsphal_data_t *dsphal_data, dsphal_datalist_t *dsphal_datalist);
DSPHALEXPORT int dsphal_data_int_link(dsphal_data_t *dsphal_data, dsphal_data_int_t *dsphal_data_int);
DSPHALEXPORT int dsphal_data_memblk_int8_link(dsphal_data_t *dsphal_data, dsphal_data_memblk_int8_t *dsphal_data_memblk_int8);
DSPHALEXPORT int dsphal_data_string_link(dsphal_data_t *dsphal_data, dsphal_data_string_t *dsphal_data_string);

#ifdef __cplusplus
	}
#endif

#endif
