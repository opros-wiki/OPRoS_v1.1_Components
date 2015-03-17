/*
*******************************************************************************
* File name     : dsphal_method.h
* Programmer(s) : Park Jin-Seok
* Description   : This is header which declares types and function prototypes
*                 about DSPHAL method.
*******************************************************************************
*/

#ifndef DSPHAL_METHOD_H
#define DSPHAL_METHOD_H

#include <stdlib.h>
#include <string.h>

#include "dsphal_datalist.h"

/*
*******************************************************************************
*                              TYPE DEFINITIONS
*******************************************************************************
*/

typedef enum {
	DSPHAL_METHOD_RETURN_OK,
	DSPHAL_METHOD_RETURN_ERR,
} dsphal_method_return_t;

typedef struct dsphal_method_s {
	char *name;
	dsphal_method_return_t (*func)(dsphal_datalist_t *dsphal_datalist_arg, dsphal_datalist_t **dsphal_datalist_ret);
	dsphal_method_return_t (*exfunc)(dsphal_datalist_t *dsphal_datalist_arg, dsphal_datalist_t **dsphal_datalist_ret, void *ptr);
	char *sign_datalist_arg;
	char *sign_datalist_ret;
	char *help;
} dsphal_method_t;

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

DSPHALEXPORT dsphal_method_t *dsphal_method_new(void);
DSPHALEXPORT void dsphal_method_free(dsphal_method_t *dsphal_method);

DSPHALEXPORT int dsphal_method_set_name(dsphal_method_t *dsphal_method, char *new_name);
DSPHALEXPORT int dsphal_method_set_func(dsphal_method_t *dsphal_method, dsphal_method_return_t (*new_func)(dsphal_datalist_t *, dsphal_datalist_t **));
DSPHALEXPORT int dsphal_method_set_exfunc(dsphal_method_t *dsphal_method, dsphal_method_return_t (*new_exfunc)(dsphal_datalist_t *, dsphal_datalist_t **, void *));
DSPHALEXPORT int dsphal_method_set_sign_datalist_arg(dsphal_method_t *dsphal_method, char *new_sign_datalist_arg);
DSPHALEXPORT int dsphal_method_set_sign_datalist_ret(dsphal_method_t *dsphal_method, char *new_sign_datalist_ret);
DSPHALEXPORT int dsphal_method_set_help(dsphal_method_t *dsphal_method, char *new_help);

DSPHALEXPORT char *dsphal_method_get_name(dsphal_method_t *dsphal_method);
DSPHALEXPORT char *dsphal_method_get_sign_datalist_arg(dsphal_method_t *dsphal_method);
DSPHALEXPORT char *dsphal_method_get_sign_datalist_ret(dsphal_method_t *dsphal_method);
DSPHALEXPORT char *dsphal_method_get_help(dsphal_method_t *dsphal_method);

DSPHALEXPORT dsphal_method_t *dsphal_method_create(char *name, dsphal_method_return_t (*func)(dsphal_datalist_t *, dsphal_datalist_t **), char *sign_datalist_arg, char *sign_datalist_ret, char *help);
DSPHALEXPORT dsphal_method_t *dsphal_method_ex_create(char *name, dsphal_method_return_t (*exfunc)(dsphal_datalist_t *, dsphal_datalist_t **, void *), char *sign_datalist_arg, char *sign_datalist_ret, char *help);
DSPHALEXPORT dsphal_method_return_t dsphal_method_execute(dsphal_method_t *dsphal_method, dsphal_datalist_t *dsphal_datalist_arg, dsphal_datalist_t **dsphal_datalist_ret);
DSPHALEXPORT dsphal_method_return_t dsphal_method_ex_execute(dsphal_method_t *dsphal_method, dsphal_datalist_t *dsphal_datalist_arg, dsphal_datalist_t **dsphal_datalist_ret, void *ptr);

#ifdef __cplusplus
	}
#endif

#endif
