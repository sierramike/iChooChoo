#include "header.h"
#include "modconf.h"
#include "biccp.h"
#include "log.h"

int ModConfSetAddress(int addr, uint8_t newaddr)
{
	int iReturn = 0;
	union BICCP_Data answer;

	if (RequestToModule(addr, &answer, BICCP_GRP_CONF, BICCP_CMD_CONF_ADDR, 1, newaddr))
		iReturn = (answer.Data[0] == BICCP_SUCCESS);

	LogMessageAddress(iReturn, (char*)"Set address", addr, newaddr);

	return iReturn;
}

int ModConfSetType(int addr, uint8_t type)
{
	int iReturn = 0;
	union BICCP_Data answer;

	if (RequestToModule(addr, &answer, BICCP_GRP_CONF, BICCP_CMD_CONF_TYPE, 1, type))
		iReturn = (answer.Data[0] == BICCP_SUCCESS);

	LogMessageInt(iReturn, (char*)"Set type", addr, type);

	return iReturn;
}

int ModConfSetDescription(int addr, char* desc)
{
	int iReturn = 0;
	union BICCP_Data answer;

	uint8_t data[DESCSIZE];
	memset(data, 0, DESCSIZE);
	int i = strlen(desc);
	if (i > DESCSIZE)
		i = DESCSIZE;
	memcpy(data, desc, i);

	for (int i = 0; i < DESCSIZE; i++)
		if (data[i] == ' ') data[i] = '-';

	if (RequestToModule(addr, &answer, BICCP_GRP_CONF, BICCP_CMD_CONF_DESC, 14, data[0], data[1], data[2],
				data[3], data[4], data[5], data[6], data[7], data[8], data[9],
				data[10], data[11], data[12], data[13]))
		iReturn = (answer.Data[0] == BICCP_SUCCESS);

	LogMessageText(iReturn, (char*)"Set description", addr, desc);

	return iReturn;
}

int ModConfSoftReset(int addr)
{
	int iReturn = 0;
	union BICCP_Data answer;

	if (RequestToModule(addr, &answer, BICCP_GRP_CONF, BICCP_CMD_CONF_SOFTRST, 0))
		iReturn = (answer.Data[0] == BICCP_SUCCESS);

	LogMessage(iReturn, (char*)"SoftReset", addr);

	return iReturn;
}

int ModConfHardReset(int addr)
{
	int iReturn = 0;
	union BICCP_Data answer;

	if (RequestToModule(addr, &answer, BICCP_GRP_CONF, BICCP_CMD_CONF_HARDRST, 0))
		iReturn = (answer.Data[0] == BICCP_SUCCESS);

	LogMessage(iReturn, (char*)"HardReset", addr);

	return iReturn;
}
