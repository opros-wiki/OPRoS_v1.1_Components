/*
*******************************************************************************
* File name     : dsphal_header.h
* Programmer(s) : Park Jin-Seok
* Description   : This is a header which declares types and function prototypes
*                 about the DSPHAL protocol header.
*******************************************************************************
*/

#ifndef DSPHAL_HEADER_H
#define DSPHAL_HEADER_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*
*******************************************************************************
*                                  CONSTANTS
*******************************************************************************
*/

#define DSPHAL_NAME_VER "DSPHAL-1.1"

/*
*******************************************************************************
*                              TYPE DEFINITIONS
*******************************************************************************
*/

typedef struct dsphal_header_s {
	char *name_ver;
	int bodylen;
} dsphal_header_t;

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

DSPHALEXPORT dsphal_header_t *dsphal_header_new(void);
DSPHALEXPORT void dsphal_header_free(dsphal_header_t *dsphal_header);

DSPHALEXPORT int dsphal_header_set_name_ver(dsphal_header_t *dsphal_header, char *new_name_ver);
DSPHALEXPORT int dsphal_header_set_bodylen(dsphal_header_t *dsphal_header, int new_bodylen);

DSPHALEXPORT char *dsphal_header_get_name_ver(dsphal_header_t *dsphal_header);
DSPHALEXPORT int dsphal_header_get_bodylen(dsphal_header_t *dsphal_header);

DSPHALEXPORT dsphal_header_t *dsphal_header_create(char *name_ver, int bodylen);
DSPHALEXPORT void dsphal_header_destroy(dsphal_header_t *dsphal_header);

DSPHALEXPORT int dsphal_header_serialize(dsphal_header_t *dsphal_header, char **mem, int *size);
DSPHALEXPORT int dsphal_header_deserialize(dsphal_header_t **dsphal_header, char *mem);

#ifdef __cplusplus
	}
#endif

#endif
