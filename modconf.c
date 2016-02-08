#include "header.h"
#include "modconf.h"
#include "biccp.h"
#include "log.h"

int ScanBus(struct ModuleIdent** list)
{
	uint8_t* moduleList = calloc(0x78, sizeof(uint8_t));
	int iNbModules = BICCP_ScanBus(moduleList);

	*list = calloc(iNbModules, sizeof(struct ModuleIdent));
	struct ModuleIdent* myList = *list;
	int i = 0;
	for (int l = 0; l < 0x78; l++)
	{
		if (moduleList[l] > 0)
		{
			ModConfGetIdent2(l, &myList[i]);
			usleep(10000);
			ModConfGetIdent2(l, &myList[i]);
			i++;
		}
	}

	free(moduleList);

	return i;
}

struct ModuleIdent ModConfGetIdent(int addr)
{
	struct ModuleIdent mi;
	ModConfGetIdent2(addr, &mi);
	return mi;
}

int ModConfGetIdent2(int addr, struct ModuleIdent* mi)
{
	int iReturn = 0;
	union BICCP_Data answer;

	mi->Address = addr;

	if(RequestToModule(addr, &answer, BICCP_GRP_CONF, BICCP_CMD_CONF_VERSION, 0))
	{
		mi->Major = answer.Data[0];
		mi->Minor = answer.Data[1];
		mi->Build = answer.Data[2];
		if (RequestToModule(addr, &answer, BICCP_GRP_CONF, BICCP_CMD_CONF_IDENT, 0))
		{
			mi->InfoPresent = true;
			mi->Address = answer.Data[0];
			mi->Type = answer.Data[1];
			memcpy(mi->Description, answer.Data + 2, DESCSIZE);
			mi->Description[DESCSIZE] = 0;
			iReturn = true;
		}
		else
			mi->InfoPresent = false;
	}
	else
		mi->InfoPresent = false;

	LogMessage(iReturn, "Module identification", addr);

	return iReturn;
}

int ModConfSetAddress(int addr, uint8_t newaddr)
{
	int iReturn = 0;
	union BICCP_Data answer;

	if (RequestToModule(addr, &answer, BICCP_GRP_CONF, BICCP_CMD_CONF_ADDR, 1, newaddr))
		iReturn = (answer.Data[0] == BICCP_SUCCESS);

	LogMessageAddress(iReturn, "Set address", addr, newaddr);

	return iReturn;
}

int ModConfSetType(int addr, uint8_t type)
{
	int iReturn = 0;
	union BICCP_Data answer;

	if (RequestToModule(addr, &answer, BICCP_GRP_CONF, BICCP_CMD_CONF_TYPE, 1, type))
		iReturn = (answer.Data[0] == BICCP_SUCCESS);

	LogMessageInt(iReturn, "Set type", addr, type);

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

	if (RequestToModule(addr, &answer, BICCP_GRP_CONF, BICCP_CMD_CONF_DESC, 14, data[0], data[1], data[2],
				data[3], data[4], data[5], data[6], data[7], data[8], data[9],
				data[10], data[11], data[12], data[13]))
		iReturn = (answer.Data[0] == BICCP_SUCCESS);

	LogMessageText(iReturn, "Set description", addr, desc);

	return iReturn;
}

int ModConfSoftReset(int addr)
{
	int iReturn = 0;
	union BICCP_Data answer;

	if (RequestToModule(addr, &answer, BICCP_GRP_CONF, BICCP_CMD_CONF_SOFTRST, 0))
		iReturn = (answer.Data[0] == BICCP_SUCCESS);

	LogMessage(iReturn, "SoftReset", addr);

	return iReturn;
}

int ModConfHardReset(int addr)
{
	int iReturn = 0;
	union BICCP_Data answer;

	if (RequestToModule(addr, &answer, BICCP_GRP_CONF, BICCP_CMD_CONF_HARDRST, 0))
		iReturn = (answer.Data[0] == BICCP_SUCCESS);

	LogMessage(iReturn, "HardReset", addr);

	return iReturn;
}
