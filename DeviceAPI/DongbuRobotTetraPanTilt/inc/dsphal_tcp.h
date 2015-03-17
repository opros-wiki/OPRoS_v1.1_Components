/*
*******************************************************************************
* File name     : dsphal_tcp.h
* Programmer(s) : Park Jin-Seok
* Description   : This is a header which declares function prototypes about
*                 the DSPHAL TCP transaction.
*******************************************************************************
*/

#ifndef DSPHAL_TCP_H
#define DSPHAL_TCP_H

#include <stdlib.h>
#include <string.h>

#include "dsphal_header.h"
#include "dsphal_utility.h"

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

DSPHALEXPORT int dsphal_tcp_send(int socket_fd, char *send_string);
DSPHALEXPORT int dsphal_tcp_recv(int socket_fd, char **recv_string);

#ifdef __cplusplus
	}
#endif

#endif
