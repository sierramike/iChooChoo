#include "daemon.h"
#include "server.h"

#define BUFSIZE 2048

void* ServerThread(void* ptr)
{
	if (!ptr) pthread_exit(0);
	connection_t* conn = (connection_t*)ptr;
	SOCKADDR_IN* sock = (SOCKADDR_IN*)&conn->address;

	char buffer_out[BUFSIZE] = "";
	char buffer_in[BUFSIZE] = "";
	int iPosition = 0;
	int sock_err;

	// Assure que le buffer soit complètement à zéro
	memset(&buffer_in[0], 0, sizeof(buffer_in));

	printf("Un client se connecte avec la socket %d de %s:%d\n",
		conn->sock,
		inet_ntoa(sock->sin_addr),
		htons(sock->sin_port));

	while (!CheckMessage(buffer_in, BUFSIZE))
	{
		sock_err = recv(conn->sock, &buffer_in[iPosition], BUFSIZE - iPosition, 0);
		buffer_in[BUFSIZE - 1] = 0;
		iPosition = strlen(buffer_in);
	}
	if (sock_err != SOCKET_ERROR)
	{
		printf("Chaîne reçue (%d): %s", strlen(buffer_in), buffer_in);
/*		for(int i = 0; i < BUFSIZE && buffer_in[i] != 0 && buffer_in[i] != 13 && buffer_in[i] != 10; i++)
		{
			printf(" - %d:%c(%u)", i, buffer_in[i], buffer_in[i]);
		}
		printf("\n");*/
		int iSize = ProcessMessage(buffer_in, buffer_out);
		sock_err = send(conn->sock, buffer_out, iSize, 0);

		if (sock_err != SOCKET_ERROR)
			printf("Chaîne envoyée : %s\n", buffer_out);
		else
			printf("Erreur de transmission.\n");
	}
	else
		printf("Erreur de réception.\n");

	closesocket(conn->sock);
	free(conn);
	pthread_exit(0);
}

bool CheckMessage(char* message, int size)
{
	bool b = false;
	int position = 0;
	while ((position < size) && (message[position] != 0))
	{
		if (message[position] == 10)
			b = true;

		position++;
	}

	return b;
}

int ProcessMessage(char* message, char* buffer_out)
{
	int iReturn = 0;
	buffer_out[0] = 0;

	if (strncmp(message, "DO_HARDRESET ", 13) == 0)
	{
		int iAddr = -1;
		sscanf(message, "DO_HARDRESET %02x", &iAddr);
		if (iAddr > 7 && iAddr <= 0x77)
		{
			ModConfHardReset(iAddr);
			memcpy(buffer_out, "+OK", 3);
			iReturn += 3;
		}
		else
		{
			memcpy(buffer_out, "-KO Wrong address", 17);
			iReturn += 17;
		}
	}
	else if (strncmp(message, "DO_RESCAN\n", 10) == 0)
	{
		pthread_mutex_lock(&m_conf);
		conf->ScanBus();
		pthread_mutex_unlock(&m_conf);

		memcpy(buffer_out, "+OK", 3);
		iReturn += 3;
	}
	else if (strncmp(message, "DO_SOFTRESET ", 13) == 0)
	{
		int iAddr = -1;
		sscanf(message, "DO_SOFTRESET %02x", &iAddr);
		if (iAddr > 7 && iAddr <= 0x77)
		{
			ModConfSoftReset(iAddr);
			memcpy(buffer_out, "+OK", 3);
			iReturn += 3;
		}
		else
		{
			memcpy(buffer_out, "-KO Wrong address", 17);
			iReturn += 17;
		}
	}
	else if (strncmp(message, "DO_SETOUT ", 10) == 0)
	{
		int iAddr = -1;
		int iOutput = -1;
		int iValue = 0;
		sscanf(message, "DO_SETOUT %02x %02x %d", &iAddr, &iOutput, &iValue);
		if (iAddr > 7 && iAddr < 0x77)
		{
			pthread_mutex_lock(&m_conf);
			cConfModule* mod = conf->Modules.find(iAddr)->second;
			IConfModuleOnOffOutputs* modOnOff = dynamic_cast<IConfModuleOnOffOutputs*>(mod);
			if (modOnOff != 0)
			{
				if (modOnOff->setOutput(iOutput, (iValue != 0)))
				{
					memcpy(buffer_out, "+OK", 3);
					iReturn += 3;
				}
				else
				{
					memcpy(buffer_out, "-KO Error writing to module", 27);
					iReturn += 27;
				}
			}
			else
			{
				memcpy(buffer_out, "-KO No module at address", 24);
				iReturn += 24;
			}
			pthread_mutex_unlock(&m_conf);
		}
		else
		{
			memcpy(buffer_out, "-KO Wrong address", 17);
			iReturn += 17;
		}
	}
	else if (strncmp(message, "DO_SETDIMOUT ", 13) == 0)
	{
		int iAddr = -1;
		int iOutput = -1;
		int iValue = 0;
		sscanf(message, "DO_SETDIMOUT %02x %02x %d", &iAddr, &iOutput, &iValue);
		if (iAddr > 7 && iAddr < 0x77)
		{
			pthread_mutex_lock(&m_conf);
			cConfModule* mod = conf->Modules.find(iAddr)->second;
			IConfModuleDimmableOutputs* modDim = dynamic_cast<IConfModuleDimmableOutputs*>(mod);
			if (modDim != 0)
			{
				if (modDim->setDimmableOutput(iOutput, (uint8_t)iValue))
				{
					memcpy(buffer_out, "+OK", 3);
					iReturn += 3;
				}
				else
				{
					memcpy(buffer_out, "-KO Error writing to module", 27);
					iReturn += 27;
				}
			}
			else
			{
				memcpy(buffer_out, "-KO No module at address", 24);
				iReturn += 24;
			}
			pthread_mutex_unlock(&m_conf);
		}
		else
		{
			memcpy(buffer_out, "-KO Wrong address", 17);
			iReturn += 17;
		}
	}
	else if (strncmp(message, "GET_STATUS ", 11) == 0)
	{
		int iAddr = -1;
		sscanf(message, "GET_STATUS %02x", &iAddr);
		if (iAddr > 7 && iAddr < 0x77)
		{
			pthread_mutex_lock(&m_conf);
			cConfModule* mod = conf->Modules.find(iAddr)->second;
			if (mod != 0)
			{
				memcpy(buffer_out, "+OK", 3);
				iReturn += 3;
				iReturn += mod->writeStatus(buffer_out + iReturn);
			}
			else
			{
				memcpy(buffer_out, "-KO No module at address", 24);
				iReturn += 24;
			}
			pthread_mutex_unlock(&m_conf);
		}
		else
		{
			memcpy(buffer_out, "-KO Wrong address", 17);
			iReturn += 17;
		}
	}
	else if (strncmp(message, "GET_MODULE ", 11) == 0)
	{
		int iAddr = -1;
		sscanf(message, "GET_MODULE %02x", &iAddr);
		if (iAddr > 7 && iAddr < 0x77)
		{
			pthread_mutex_lock(&m_conf);
			cConfModule* ccMod = conf->Modules[iAddr];
			if (ccMod != 0)
			{
				memcpy(buffer_out, "+OK", 3);
				iReturn += 3;
				iReturn += sprintf(buffer_out + iReturn, " 1");

				iReturn += sprintf(buffer_out + iReturn, " %02x %d.%d.%d %02x %s", iAddr, ccMod->getMajor(), ccMod->getMinor(),
					ccMod->getBuild(), ccMod->getType(), (strlen(ccMod->getDescription()) == 0 ? "?" : ccMod->getDescription()));
			}
			else
			{
				memcpy(buffer_out, "-KO No module at address", 24);
				iReturn += 24;
			}
			pthread_mutex_unlock(&m_conf);
		}
		else
		{
			memcpy(buffer_out, "-KO Wrong address", 17);
			iReturn += 17;
		}
	}
	else if (strncmp(message, "GET_MODULELIST\n", 15) == 0)
	{
		pthread_mutex_lock(&m_conf);
		memcpy(buffer_out, "+OK", 3);
		iReturn += 3;
		iReturn += sprintf(buffer_out + iReturn, " %d", conf->Modules.size());

		typedef std::map<int, cConfModule*>::iterator it_mod;
		for(it_mod iterator = conf->Modules.begin(); iterator != conf->Modules.end(); ++iterator)
		{
			cConfModule* ccMod = iterator->second;
			iReturn += sprintf(buffer_out + iReturn, " %02x %d.%d.%d %02x %s", ccMod->getID(), ccMod->getMajor(), ccMod->getMinor(),
				ccMod->getBuild(), ccMod->getType(), (strlen(ccMod->getDescription()) == 0 ? "?" : ccMod->getDescription()));
		}
		pthread_mutex_unlock(&m_conf);
	}
	else if (strncmp(message, "SET_ADDR ", 9) == 0)
	{
		int iNewAddr = -1;
		sscanf(message, "SET_ADDR %02x", &iNewAddr);
		if (iNewAddr > 7 && iNewAddr < 0x77)
		{
			int iRet = ModConfSetAddress(0x77, iNewAddr);
			if (iRet != 0)
				memcpy(buffer_out, "+OK", 3);
			else
				memcpy(buffer_out, "-KO", 3);
			iReturn += 3;
		}
		else
		{
			memcpy(buffer_out, "-KO Wrong address", 17);
			iReturn += 17;
		}
	}
	else if (strncmp(message, "SET_DESC ", 9) == 0)
	{
		int iAddr = -1;
		char* sDesc;
		sscanf(message, "SET_DESC %02x %ms", &iAddr, &sDesc);
		if (iAddr > 7 && iAddr < 0x77 && strlen(sDesc) < 15 && strlen(sDesc) > 0)
		{
			ModConfSetDescription(iAddr, sDesc);
			memcpy(buffer_out, "+OK", 3);
			iReturn += 3;
		}
		else
		{
			memcpy(buffer_out, "-KO Wrong address or missing description or description too long", 64);
			iReturn += 64;
		}
		free(sDesc);
	}
	else if (strncmp(message, "SET_TYPE ", 9) == 0)
	{
		int iNewType = -1;
		sscanf(message, "SET_TYPE %02x", &iNewType);
		if (iNewType > 0 && iNewType < 0xFF)
		{
			ModConfSetType(0x77, iNewType);
			memcpy(buffer_out, "+OK", 3);
			iReturn += 3;
		}
		else
		{
			memcpy(buffer_out, "-KO Wrong type", 14);
			iReturn += 14;
		}
	}
	else
	{
		memcpy(buffer_out, "-KO Wrong command", 17);
		iReturn += 17;
	}
	buffer_out[iReturn] = '\n';
	iReturn++;
	buffer_out[iReturn] = 0;
	return iReturn;
}
