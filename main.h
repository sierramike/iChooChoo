#pragma once

#include "ConfManager.hpp"

extern ConfManager* conf;
extern pthread_mutex_t m_conf;

int ProcessArguments(int argc, char* argv[]);

int CheckArgument(int argc, char* argv[], int argi, char* argname, int nparam);

int CheckArgumentGetByte(int argc, char* argv[], int argi, char* argname, int nparam, uint8_t* data);

void PrintModuleList();

int PrintVersion();

int PrintHelp(char* executableName);
