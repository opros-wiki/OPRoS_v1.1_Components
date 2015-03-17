/*
*******************************************************************************
* File name     : dsphal_xml.h
* Programmer(s) : Park Jin-Seok
* Description   : This is header which declares function prototypes about
*                 dumping DSPHAL data structure to XML and loading DSPHAL data
*                 structure from XML. This is dependent on libxml2 XML library.
*******************************************************************************
*/

#ifndef DSPHAL_XML_H
#define DSPHAL_XML_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <libxml/parser.h>
#include <libxml/tree.h>

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

DSPHALEXPORT xmlNodePtr dsphal_xmlnode_dump_data_int(dsphal_data_int_t *dsphal_data_int);
DSPHALEXPORT xmlNodePtr dsphal_xmlnode_dump_data_string(dsphal_data_string_t *dsphal_data_string);
DSPHALEXPORT xmlNodePtr dsphal_xmlnode_dump_data(dsphal_data_t *dsphal_data);
DSPHALEXPORT xmlNodePtr dsphal_xmlnode_dump_datalist(dsphal_datalist_t *dsphal_datalist);

DSPHALEXPORT dsphal_data_int_t *dsphal_xmlnode_load_data_int(xmlNodePtr xmlnode);
DSPHALEXPORT dsphal_data_string_t *dsphal_xmlnode_load_data_string(xmlNodePtr xmlnode);
DSPHALEXPORT dsphal_data_t *dsphal_xmlnode_load_data(xmlNodePtr xmlnode);
DSPHALEXPORT dsphal_datalist_t *dsphal_xmlnode_load_datalist(xmlNodePtr xmlnode);

DSPHALEXPORT int dsphal_xml_serialize_method_call(char *method_name, dsphal_datalist_t *method_datalist_arg, char **mem, int *size);
DSPHALEXPORT int dsphal_xml_serialize_method_response(dsphal_datalist_t *method_datalist_ret, char **mem, int *size);
DSPHALEXPORT void dsphal_xml_free(char *mem);

DSPHALEXPORT int dsphal_xml_deserialize_method_call(char **method_name, dsphal_datalist_t **method_datalist_arg, char *mem);
DSPHALEXPORT int dsphal_xml_deserialize_method_response(dsphal_datalist_t **method_datalist_ret, char *mem);

DSPHALEXPORT void dsphal_xml_init_parser(void);
DSPHALEXPORT void dsphal_xml_cleanup_parser(void);

#ifdef __cplusplus
	}
#endif

#endif
