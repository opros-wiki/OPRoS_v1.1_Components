/*
*******************************************************************************
* File name     : dsphal_tcp_client.h
* Programmer(s) : Park Jin-Seok
* Description   : This is a header which declares types and function prototypes
*                 about the DSPHAL TCP client.
*******************************************************************************
*/

#ifndef DSPHAL_TCP_CLIENT_H
#define DSPHAL_TCP_CLIENT_H

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

typedef struct dsphal_tcp_client_s {
	char *server_ip_addr;
	int server_port;

	int socket_fd;
} dsphal_tcp_client_t;

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

DSPHALEXPORT dsphal_tcp_client_t *dsphal_tcp_client_new(void);
DSPHALEXPORT void dsphal_tcp_client_free(dsphal_tcp_client_t *dsphal_tcp_client);

DSPHALEXPORT int dsphal_tcp_client_set_server_ip_addr(dsphal_tcp_client_t *dsphal_tcp_client, char *new_server_ip_addr);
DSPHALEXPORT int dsphal_tcp_client_set_server_port(dsphal_tcp_client_t *dsphal_tcp_client, int new_server_port);
DSPHALEXPORT int dsphal_tcp_client_set_socket_fd(dsphal_tcp_client_t *dsphal_tcp_client, int new_socket_fd);

DSPHALEXPORT char *dsphal_tcp_client_get_server_ip_addr(dsphal_tcp_client_t *dsphal_tcp_client);
DSPHALEXPORT int dsphal_tcp_client_get_server_port(dsphal_tcp_client_t *dsphal_tcp_client);
DSPHALEXPORT int dsphal_tcp_client_get_socket_fd(dsphal_tcp_client_t *dsphal_tcp_client);

DSPHALEXPORT dsphal_tcp_client_t *dsphal_tcp_client_create(char *server_ip_addr, int server_port);
DSPHALEXPORT int dsphal_tcp_client_connect(dsphal_tcp_client_t *dsphal_tcp_client);
DSPHALEXPORT void dsphal_tcp_client_destroy(dsphal_tcp_client_t *dsphal_tcp_client);

#ifdef __cplusplus
	}
#endif

#endif
