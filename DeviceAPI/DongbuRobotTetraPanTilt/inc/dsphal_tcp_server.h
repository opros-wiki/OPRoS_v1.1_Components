/*
*******************************************************************************
* File name     : dsphal_tcp_server.h
* Programmer(s) : Park Jin-Seok
* Description   : This is a header which declares types and function prototypes
*                 about the DSPHAL TCP server.
*******************************************************************************
*/

#ifndef DSPHAL_TCP_SERVER_H
#define DSPHAL_TCP_SERVER_H

#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
	#include <winsock2.h>
#else
	#include <unistd.h>
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
#endif

/*
*******************************************************************************
*                              TYPE DEFINITIONS
*******************************************************************************
*/

typedef struct dsphal_tcp_server_s {
	int port;

	int socket_fd;
} dsphal_tcp_server_t;

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

DSPHALEXPORT dsphal_tcp_server_t *dsphal_tcp_server_new(void);
DSPHALEXPORT void dsphal_tcp_server_free(dsphal_tcp_server_t *dsphal_tcp_server);

DSPHALEXPORT int dsphal_tcp_server_set_port(dsphal_tcp_server_t *dsphal_tcp_server, int new_port);
DSPHALEXPORT int dsphal_tcp_server_set_socket_fd(dsphal_tcp_server_t *dsphal_tcp_server, int new_socket_fd);

DSPHALEXPORT int dsphal_tcp_server_get_port(dsphal_tcp_server_t *dsphal_tcp_server);
DSPHALEXPORT int dsphal_tcp_server_get_socket_fd(dsphal_tcp_server_t *dsphal_tcp_server);

DSPHALEXPORT dsphal_tcp_server_t *dsphal_tcp_server_create(int port);
DSPHALEXPORT dsphal_tcp_server_t *dsphal_tcp_server_accept(dsphal_tcp_server_t *dsphal_tcp_server);
DSPHALEXPORT void dsphal_tcp_server_destroy(dsphal_tcp_server_t *dsphal_tcp_server);

#ifdef __cplusplus
	}
#endif

#endif
