#include "biccp.h"

int Addresses[0x78];

int BICCP_ScanBus(uint8_t* list)
{
	return I2CScan(1, list);
}

int GetFDFromAddress(int address)
{
	if (address < 0x08 || address > 0x77)
		return 0;

	if (Addresses[address] == 0)
		Addresses[address] = I2CSetup(address, 1);

	return Addresses[address];
}

int PollModuleForResponse(int fd, uint8_t* bfrRecv)
{
	bfrRecv[0] = 0xCC;
	int iCnt = 0;
	while((bfrRecv[0] == 0xCC) && iCnt < 80) // 80x50ms = 4s timeout
	{
		int retVal = I2CReadBlock(fd, bfrRecv);

		if (bfrRecv[0] == 0xCC)
		{
			// No data, waiting 50ms to retry.
			usleep(50000);
		}

		iCnt++;
	}

	if (bfrRecv[0] == 0xCC)
		return 0;
	else
		return -1;
}

int RequestToModuleFromBuffer(int fd, uint8_t* buffer)
{
	PutCRC(buffer);
	int l = I2CWriteBlock(fd, buffer);
	memset(buffer, 0, PACKETSIZE);
	return PollModuleForResponse(fd, buffer);
}

int RequestToModule(int addr, union BICCP_Data* data, uint8_t group, uint8_t command, uint8_t datasize, ...)
{
	int fd = GetFDFromAddress(addr);
	uint8_t buffer[PACKETSIZE];
	memset(buffer, 0, PACKETSIZE);
	buffer[0] = group;
	buffer[1] = command;

	va_list pa;
	int i = 2;
	va_start(pa, datasize);
	for(int i = 0; i < datasize; i++)
	{
		if (i < DATASIZE)
			buffer[i + 2] = (uint8_t)va_arg(pa, int);
	}
	va_end(pa);

	int l = RequestToModuleFromBuffer(fd, buffer);
	if (!l)
		return l;
	else if (buffer[0] != group || buffer[1] != command)
		return 0;
	else
	{
		memcpy((*data).Data, buffer + 2, DATASIZE);
		return -1;
	}
}
