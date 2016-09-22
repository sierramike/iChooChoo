#include "daemon.h"
#include "server.h"

//struct ModuleIdent* _moduleList;
//int _iModuleList;
//pthread_mutex_t m_moduleList = PTHREAD_MUTEX_INITIALIZER;

int start_daemon()
{
	init_daemon();

#if defined (WIN32)
	WSADATA WSAData;
	int erreur = WSAStartup(MAKEWORD(2,2), &WSAData);
#else
	int erreur = 0;
#endif

	// ICI on mettra notre code sur les socket

	/* Socket et contexte d'adressage du serveur */
	SOCKADDR_IN sin;
	SOCKET sock;
	socklen_t recsize = sizeof(sin);

	int sock_err;

	if (!erreur)
	{
		/* Création d'une socket */
		sock = socket(AF_INET, SOCK_STREAM, 0);

		/* Si la socket est valide */
		if (sock != INVALID_SOCKET)
		{
			printf("La socket %d est maintenant ouverte en mode TCP/IP\n", sock);

			/* Configuration */
			sin.sin_addr.s_addr = htonl(INADDR_ANY);
			sin.sin_family = AF_INET;
			sin.sin_port = htons(PORT);
			sock_err = bind(sock, (SOCKADDR*)&sin, recsize);

			/* Si la socket fonctionne */
			if (sock_err != SOCKET_ERROR)
			{
				/* Démarrage de l'écoute (mode serveur) */
				sock_err = listen(sock, 5);
				printf("Ecoute sur le port %d...\n", PORT);

				/* Si la socket fonctionne */
				if (sock_err != SOCKET_ERROR)
				{
					while(1)
					{
						connection_t* connection = (connection_t*)malloc(sizeof(connection_t));
						connection->addr_len = sizeof(connection->address);
						connection->sock = accept(sock, &(connection->address), &(connection->addr_len));
						if (connection->sock <= 0)
							free(connection);
						else
						{
							pthread_t thread;
							pthread_create(&thread, 0, ServerThread, (void*)connection);
							pthread_detach(thread);
						}
					}
				}
				else
					perror("listen");
			}
			else
				perror("bind");

			/* Fermeture de la socket serveur */
			printf("Fermeture de la socket serveur\n");
			closesocket(sock);
			printf("Fermeture du serveur terminée\n");
		}
		else
			perror("socket");
	}


	//Si la plateforme est Windows
#if defined (WIN32)
	WSACleanup();
#endif

	return EXIT_SUCCESS;
}

void init_daemon()
{
}
