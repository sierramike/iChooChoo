#include "header.h"
#include "log.h"
#include <time.h>

void LogDateTime()
{
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);

	printf("%04d/%02d/%02d %02d:%02d:%02d ", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
}

void LogText(char* message)
{
	printf("%s\n");
}

void LogMessage(int status, char* message, int address)
{
	LogDateTime();
	if (!status)
		printf("%s on %#02x: error.\n", message, address);
	else
		printf("%s on %#02x: done.\n", message, address);
}

void LogMessageAddress(int status, char* message, int address, int data)
{
	LogDateTime();
	if (!status)
		printf("%s on %#02x to %#02x: error.\n", message, address, data);
	else
		printf("%s on %#02x to %#02x: done.\n", message, address, data);
}

void LogMessageInt(int status, char* message, int address, int data)
{
	LogDateTime();
	if (!status)
		printf("%s on %#02x to %d: error.\n", message, address, data);
	else
		printf("%s on %#02x to %d: done.\n", message, address, data);
}

void LogMessageText(int status, char* message, int address, char* data)
{
	LogDateTime();
	if (!status)
		printf("%s on %#02x to %s: error.\n", message, address, data);
	else
		printf("%s on %#02x to %s: done.\n", message, address, data);
}
