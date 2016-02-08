#pragma once

#define BICCP_ERROR   0x00
#define BICCP_SUCCESS 0xFF

#define BICCP_GRP_CONF 0x01

#define BICCP_CMD_CONF_IDENT    0x01
#define BICCP_CMD_CONF_VERSION  0x02
#define BICCP_CMD_CONF_ADDR     0x10
#define BICCP_CMD_CONF_TYPE     0x11
#define BICCP_CMD_CONF_DESC     0x12
#define BICCP_CMD_CONF_SOFTRST  0xFE
#define BICCP_CMD_CONF_HARDRST  0xFF

union BICCP_Data
{
	uint8_t Data[DATASIZE];
};

int BICCP_ScanBus(uint8_t* list);

int GetFDFromAddress(int address);

int PollModuleForResponse(int addr, uint8_t* bfrRecv);

int RequestToModuleFromBuffer(int addr, uint8_t* buffer);

int RequestToModule(int addr, union BICCP_Data* data, uint8_t group, uint8_t command, uint8_t datasize, ...);
