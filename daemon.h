#ifndef DAEMON_H
#define DAEMON_H

#include "settings.h"

int start_daemon();

void init_daemon();

//Si nous sommes sous Windows
#if defined (WIN32)

	#include <winsock2.h>

	// typedef qui nous serviront par la suite
	typedef int socklen_t;

#elif defined (LINUX)

	#include <sys/types.h>
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
	#include <unistd.h>

	// define qui nous serviront par la suite
	#define INVALID_SOCKET -1
	#define SOCKET_ERROR -1
	#define closesocket(s) close(s)

	typedef int SOCKET;
	typedef struct sockaddr_in SOCKADDR_IN;
	typedef struct sockaddr SOCKADDR;

#endif

// On inclut les fichiers standards
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <pthread.h>

#include "modconf.h"

extern struct ModuleIdent* _moduleList;
extern int _iModuleList;
extern pthread_mutex_t m_moduleList;

typedef struct
{
	int sock;
	struct sockaddr address;
	socklen_t addr_len;
//	int addr_len;
} connection_t;

#endif /* DAEMON_H */
