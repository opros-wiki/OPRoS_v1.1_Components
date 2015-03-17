/*
*******************************************************************************
* File name     : dsphal_registry.h
* Programmer(s) : Park Jin-Seok
* Description   : This is header which declares types and function prototypes
*                 about DSPHAL registry.
*******************************************************************************
*/

#ifndef DSPHAL_REGISTRY_H
#define DSPHAL_REGISTRY_H

#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#include "dsphal_method.h"

/*
*******************************************************************************
*                              TYPE DEFINITIONS
*******************************************************************************
*/

typedef struct dsphal_registry_elmt_s {
	dsphal_method_t *method;

	struct dsphal_registry_elmt_s *next;
} dsphal_registry_elmt_t;

typedef struct dsphal_registry_s {
	size_t size;

	dsphal_registry_elmt_t *head;
	dsphal_registry_elmt_t *tail;
} dsphal_registry_t;

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

DSPHALEXPORT dsphal_registry_elmt_t *dsphal_registry_elmt_new(void);
DSPHALEXPORT void dsphal_registry_elmt_free(dsphal_registry_elmt_t *dsphal_registry_elmt);

DSPHALEXPORT int dsphal_registry_elmt_set_method(dsphal_registry_elmt_t *dsphal_registry_elmt, dsphal_method_t *new_method);
DSPHALEXPORT int dsphal_registry_elmt_set_next(dsphal_registry_elmt_t *dsphal_registry_elmt, dsphal_registry_elmt_t *dsphal_registry_elmt_next);

DSPHALEXPORT dsphal_method_t *dsphal_registry_elmt_get_method(dsphal_registry_elmt_t *dsphal_registry_elmt);
DSPHALEXPORT dsphal_registry_elmt_t *dsphal_registry_elmt_get_next(dsphal_registry_elmt_t *dsphal_registry_elmt);

DSPHALEXPORT dsphal_registry_elmt_t *dsphal_registry_elmt_create(void);

DSPHALEXPORT dsphal_registry_t *dsphal_registry_new(void);
DSPHALEXPORT void dsphal_registry_free(dsphal_registry_t *dsphal_registry);

DSPHALEXPORT int dsphal_registry_ins_next(dsphal_registry_t *dsphal_registry, dsphal_registry_elmt_t *dsphal_registry_elmt, dsphal_registry_elmt_t *new_dsphal_registry_elmt);

DSPHALEXPORT int dsphal_registry_set_size(dsphal_registry_t *dsphal_registry, size_t new_size);
DSPHALEXPORT int dsphal_registry_set_head(dsphal_registry_t *dsphal_registry, dsphal_registry_elmt_t *dsphal_registry_elmt_head);
DSPHALEXPORT int dsphal_registry_set_tail(dsphal_registry_t *dsphal_registry, dsphal_registry_elmt_t *dsphal_registry_elmt_tail);

DSPHALEXPORT size_t dsphal_registry_get_size(dsphal_registry_t *dsphal_registry);
DSPHALEXPORT dsphal_registry_elmt_t *dsphal_registry_get_head(dsphal_registry_t *dsphal_registry);
DSPHALEXPORT dsphal_registry_elmt_t *dsphal_registry_get_tail(dsphal_registry_t *dsphal_registry);

DSPHALEXPORT dsphal_registry_t *dsphal_registry_create(void);
DSPHALEXPORT int dsphal_registry_add_method(dsphal_registry_t *dsphal_registry, dsphal_method_t *dsphal_method);
DSPHALEXPORT dsphal_method_t *dsphal_registry_get_method(dsphal_registry_t *dsphal_registry, char *name);

#ifdef __cplusplus
	}
#endif

#endif
